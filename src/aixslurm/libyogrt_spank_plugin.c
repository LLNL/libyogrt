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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#include <slurm/spank.h>

/*
 * All spank plugins must define this macro for the SLURM plugin loader.
 */
SPANK_PLUGIN(yogrt, 1);

static char *helper_program = EXTERNALPROGPATH;
static pid_t helper_pid = (pid_t)-1;

static pid_t start_helper(const char *socket_name, uint32_t jobid);

int slurm_spank_user_init (spank_t sp, int ac, char **av)
{
	char socket_name[4096];
	uid_t uid;
	uint32_t jobid, stepid, nodeid;

	spank_get_item(sp, S_JOB_UID, &uid);
	spank_get_item(sp, S_JOB_ID, &jobid);
	spank_get_item(sp, S_JOB_STEPID, &stepid);
	spank_get_item(sp, S_JOB_NODEID, &nodeid);

	if (nodeid != 0)
		return 0;

	/* WARNING - If you change the file name here at all, you must
	   update the file name in src/aixslurm/internal.c as well.
	   Even though we have spank set the YOGRT_AIXSLURM_SOCKET variable,
	   the pmdv4 daemon will NOT pass the variable down to the user's
	   task. */
	snprintf(socket_name, sizeof(socket_name),
		 "/tmp/.yogrtaixslurm_%d_%u.%u", uid, jobid, stepid);

	spank_setenv(sp, "YOGRT_AIXSLURM_SOCKET", socket_name, 1);

	helper_pid = start_helper(socket_name, jobid);

	return 0;
}

int slurm_spank_exit (spank_t sp, int ac, char **av)
{
	int status;

	if (helper_pid > 1) {
		kill(helper_pid, SIGINT);
		waitpid(helper_pid, &status, 0);
	}
}

static pid_t start_helper(const char *socket_name, uint32_t jobid)
{
	pid_t child;
	char jobid_str[32];

	snprintf(jobid_str, sizeof(jobid_str), "%u", jobid);

	child = fork();

	if (child == -1) {
		return (pid_t)-1;
	} else if (child > 0) {
		/* parent */
		return child;
	} else {
		/* child */
		uid_t uid = geteuid();
		gid_t gid = getegid();
		int devnull, i;

		/* regain privileges... */
		seteuid(getuid());
		setegid(getgid());

		/* ...so we can fully drop privileges */
		setregid(gid, gid);
		setreuid(uid, uid);

		/* change working directory */
		chdir("/");

		/* close all fds */
		devnull = open("/dev/null", O_RDWR);
		dup2(devnull, STDIN_FILENO);
		dup2(devnull, STDOUT_FILENO);
		dup2(devnull, STDERR_FILENO);
		for (i = STDERR_FILENO+1; i < OPEN_MAX; i++) {
			close(i);
		}

		/* run the helper program */
		execl(helper_program, helper_program,
		      socket_name, jobid_str, NULL);
		exit(1);
	}
}
