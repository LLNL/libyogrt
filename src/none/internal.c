/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <limits.h>

#include "internal_yogrt.h"

int verbosity = 0;

void internal_init(int verb)
{
	verbosity = verb;
}

char *internal_backend_name(void)
{
	return "none";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	int rem;

	if (cached == -1) {
		return INT_MAX;
	}

	rem = (cached - (now - last_update));
/* 	debug2("\"none\" implementation reports remaining time of %d sec.\n", rem); */
	return rem;
}

int internal_get_rank(void)
{
	return 0;
}

