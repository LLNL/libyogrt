/***************************************************************************
 *  Copyright (C) 2007, Lawrence Livermore National Security, LLC.
 *  Produced at the Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-235649. All rights reserved.
 *
 *  This file is part of libyogrt.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see
 *  <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "internal_yogrt.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int _yogrt_verbosity = 0;

static long get_time_from_socket(const char *socket_name);

int internal_init(int verb)
{
	_yogrt_verbosity = verb;

        if ((getenv("SLURM_JOBID") != NULL
             && getenv("SLURM_STEPID") != NULL)
            || getenv("YOGRT_AIXSLURM_SOCKET") != NULL) {
                return 1;
        } else {
                debug("ERROR: SLURM_JOBID and/or SLURM_STEPID are not set."
                      " Remaining time will be a bogus value.\n");
                return 0;
        }
}

char *internal_backend_name(void)
{
	return "AIXSLURM";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	int rem;
	static char *socket_name = NULL;

	/* Get job id from environment on the first call */
	if (socket_name == NULL) {
		uid_t uid;
		uint32_t jobid, stepid;
		char *jobptr, *stepptr, *tmp;
		char buf[128];

		/* Unfortunately, we cannot use the YOGRT_AIXSLURM_SOCKET
		   environment variable because poe's pmdv4 daemons do
		   not allow environment variables in its environment to
		   fall through to the user's tasks.  Instead we just
		   expect the filename pattern to the same as that in
		   libyogrt_spank_plugin.c. */
		if ((jobptr = getenv("SLURM_JOBID")) != NULL
		    && (stepptr = getenv("SLURM_STEPID")) != NULL) {
			jobid = (uint32_t)atol(jobptr);
			stepid = (uint32_t)atol(stepptr);
			uid = getuid();
			
			snprintf(buf, sizeof(buf),
				 "/tmp/.yogrtaixslurm_%d_%u.%u",
				 uid, jobid, stepid);
			socket_name = strdup(buf);
			debug("AIXSLURM using socket \"%s\".\n", socket_name);
		} else if ((tmp = getenv("YOGRT_AIXSLURM_SOCKET")) != NULL) {
                        /* If we aren't under a job step, fall
                           back to the environment variable. */
			debug("AIXSLURM using YOGRT_AIXSLURM_SOCKET=%s\n",tmp);
			socket_name = tmp;
                } else {
                        debug("ERROR: AIXSLURM unable to determine "
                              "socket name\n");
                }
	}

	if (socket_name == NULL)
		return -1;

	rem = (int)get_time_from_socket(socket_name);
	debug2("AIXSLURM reports remaining time of %d sec.\n", rem);
	return rem;
}

int internal_get_rank(void)
{
	int rank = 0;
	char *r;

	if ((r = getenv("SLURM_PROCID")) != NULL) {
		rank = (int)atol(r);
	}

	return rank;
}

int internal_fudge(void)
{
	return 0;
}

static long get_time_from_socket(const char *socket_name)
{
	int fd;
	int len;
	struct sockaddr_un addr;
	char buf[32];
	long rem = -1;
	static int first_time = 0;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socket_name);
	len = strlen(addr.sun_path)+1 + sizeof(addr.sun_family);

again:
	if (connect(fd, (struct sockaddr *) &addr, len) < 0) {
		if (errno == ECONNREFUSED) {
			/* do nothing */
		} else if (errno == ENOENT) {
			/* The first time through, the socket may not yet
			 * have been created.  So repeadedly stat()
			 * the file name, waiting 10ms between each stat,
			 * for up to a second.  This will normally avoid
			 * having this function return -1 on the first call.
			 */
			if (!first_time) {
				struct stat statbuf;
				int i;

				first_time = 1;
				for (i = 0; i < 100; i++) {
					if (stat(socket_name, &statbuf) == 0)
						goto again;
					usleep(10000);
				}
			}
		}
		close(fd);
		return -1;
	}

	len = read(fd, buf, 31);
	close(fd);
	if (len > 0) {
		buf[len] = '\0';
		rem = atol(buf);
	}

	return rem;
}
