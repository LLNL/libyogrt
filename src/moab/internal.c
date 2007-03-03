/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <stdlib.h>
#include <moab/mapi.h>

#include "internal_yogrt.h"

int verbosity = 0;

void internal_init(int verb)
{
	verbosity = verb;
}

char *internal_backend_name(void)
{
	return "MOAB";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	long rem = 0;
	char errmsg[MCMAXLINE];

	if (-1 == MCCJobGetRemainingTime(NULL, NULL, &rem, errmsg)) {
		debug("MOAB MCCJobGetRemainingTime failed: %s\n", errmsg);
		return -1;
	}

	debug2("MOAB reports remaining time of %d sec.\n", (int)rem);
	return (int)rem;
}

int internal_get_rank(void)
{
	int rank;
	char *r;

	if ((r = getenv("SLURM_PROCID")) != NULL) {
		/*
		 * If SLURM is the underlying job launcher, we know
		 * the task rank.
		 */
		rank = (int)atol(r);
	} else {
		/* Otherwise, just let all tasks use the Moab time call. */
		rank = 0;
	}

	return rank;
}
