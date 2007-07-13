/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

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

	slurm_debug("slurm_spank_user_init 1");
	spank_get_item(sp, S_JOB_UID, &uid);
	spank_get_item(sp, S_JOB_ID, &jobid);
	spank_get_item(sp, S_JOB_STEPID, &stepid);
	spank_get_item(sp, S_JOB_NODEID, &nodeid);

	if (nodeid != 0)
		return 0;

	snprintf(socket_name, sizeof(socket_name),
		 "/tmp/.aixslurm_%d_%u.%u", uid, jobid, stepid);

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
		execl(helper_program, helper_program,
		      socket_name, jobid_str, NULL);
		exit(1);
	}
}
