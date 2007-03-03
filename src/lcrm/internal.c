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

int verbosity = 0;

char *internal_backend_name(void)
{
	return "LCRM";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	long rem = 0;
	int status = 0;
	long dontcare;

	if (0 == lrmgettime(&dontcare, &dontcare, &dontcare, &rem, &status)) {
		if (status == 0) {
			debug("LCRM lrmgettime polled too soon: rem = %ld\n",
			      rem);
		} else {
			debug("LCRM lrmgettime failed: status = %d, rem = %ld\n",
			      status, rem);
		}
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

void internal_set_verbosity(int verb)
{
	verbosity = verb;
}
