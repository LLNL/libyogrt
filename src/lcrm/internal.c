/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <stdlib.h>
#include <liblrm.h>
#include <lrmerrno.h>

#include "internal_yogrt.h"

char *internal_backend_name(void)
{
	return "LCRM";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	long rem;
	long dontcare;
	int status, rc;

	if (lrmgettime(&dontcare, &dontcare, &dontcare, &rem, &status) == 0) {
		debug("lrmgettime failed: %d\n", status);
		return -1;
	}

	debug2("LCRM reports remaining time of %d sec.\n", (int)rem);
	return (int)rem;
}

int internal_get_rank(void)
{
	int rank = 0;
	char *r;

	/*
	 * We assume that SLURM is the underlying job launcher if
	 * LCRM is the scheduler.
	 */
	if ((r = getenv("SLURM_PROCID")) != NULL) {
		rank = (int)atol(r);
	}

	return rank;
}
