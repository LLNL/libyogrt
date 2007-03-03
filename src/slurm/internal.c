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

#include "internal_yogrt.h"

static uint32_t jobid = NO_VAL;

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
