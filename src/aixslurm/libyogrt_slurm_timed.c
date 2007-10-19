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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#include <slurm/slurm.h>

int create_socket(const char *name);
void sigint_handler(int sig);

int got_signal = 0;

int main(int argc, char **argv)
{
       char *socket_name;
       uint32_t jobid;
       int fd;
       long t;

       signal(SIGINT, sigint_handler);
       if (argc != 3)
	       exit(1);

       socket_name = argv[1];
       jobid = (uint32_t)atol(argv[2]);

       unlink(socket_name);
       umask(0177);
       fd = create_socket(socket_name);

       while (1) {
	       int conn;
	       struct sockaddr_un addr;
	       int len = sizeof(addr);
	       FILE *fconn = NULL;

	       if (got_signal)
		       break;

	       conn = accept(fd, (struct sockaddr *)&addr, (socklen_t *)&len);
	       if (conn < 0)
		       break;

	       fconn = fdopen(conn, "w");
	       fprintf(fconn, "%ld\n", slurm_get_rem_time(jobid));
	       fclose(fconn); /* closes "conn" too */
       }

       close(fd);
       unlink(socket_name);

       exit(0);
}


/*
 * Create a named unix domain listening socket.
 * (cf, Stevens APUE 1st ed., section 15.5.2)
 */
int
create_socket(const char *name)
{
        int fd;
        int len;
        struct sockaddr_un addr;

        /* create a unix domain stream socket */
        if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
                return -1;

        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, name);
        len = strlen(addr.sun_path)+1 + sizeof(addr.sun_family);

        /* bind the name to the descriptor */
        if (bind(fd, (struct sockaddr *) &addr, len) < 0)
                return -2;

        if (listen(fd, 5) < 0)
                return -3;

	chmod(name, 0600);

        return fd;
}

void sigint_handler(int sig)
{
	got_signal = 1;
}
