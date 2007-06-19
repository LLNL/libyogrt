/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <stdlib.h>
#include <slurm/slurm.h>
#include <fcntl.h>
#include <unistd.h>

#include "internal_yogrt.h"

static char *external_program = EXTERNALPROGPATH;
static uint32_t jobid = NO_VAL;
static int external_get_rem_time();

int verbosity = 0;

void internal_init(int verb)
{
	verbosity = verb;
}

char *internal_backend_name(void)
{
	return "SLURM";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	int rem;

#ifdef HAVE_AIX_64BIT
	return external_get_rem_time();
#else
	/* Get job id from environment on the first call */
	if (jobid == NO_VAL) {
		char *id;
		if ((id = getenv("SLURM_JOB_ID")) != NULL) {
			jobid = (uint32_t)atol(id);
		} else if ((id = getenv("SLURM_JOBID")) != NULL) {
			jobid = (uint32_t)atol(id);
		}
	}

	if (jobid == NO_VAL)
		return -1;

	rem = (int)slurm_get_rem_time(jobid);
	debug2("SLURM reports remaining time of %d sec.\n", rem);
	return rem;
#endif
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

/*
 * Call the external 32-bit compiled application from the 64-bit libyogrt
 * backend library, because libslurm is only build 32-bit on AIX.
 */
static int external_get_rem_time()
{
	int fds[2];
	pid_t child;

	debug2("Calling external 32-bit program %s\n", external_program);
	if (-1 == pipe(fds)) {
		return -1;
	}

	child = f_fork();

	if (child == -1) {
		close(fds[0]);
		close(fds[1]);
		return -1;
	} else if (child > 0) {
		/* parent */
		FILE *extprog;
		char buf[80];
		int rc = -1;

		close(fds[1]);
		extprog = fdopen(fds[0], "r");
		if (extprog == NULL) {
			close(fds[0]);
			return rc;
		}
		if (fgets(buf, 80, extprog) != NULL) {
			rc = (int)atol(buf);
		}
		fclose(extprog); /* closes fds[0] too */

		return rc;
	} else {
		/* child */
		if (-1 == dup2(fds[1], STDOUT_FILENO)) {
			exit(1);
		}
		fcntl(fds[1], F_SETFD, 0); /* don't close fd on exec */
		fcntl(STDOUT_FILENO, F_SETFD, 0); /* don't close fd on exec */
		close(fds[0]);
		execl(external_program, external_program, NULL);
		/* should never get here */
		fprintf(stderr, "liblrmemu: Unable to exec file: %s\n",
			external_program);
		exit(2);
	}
}
