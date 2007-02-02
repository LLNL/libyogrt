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

long _internal_get_rem_time(time_t now, time_t last_update, long cached)
{
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

	return slurm_get_rem_time(jobid);
}
