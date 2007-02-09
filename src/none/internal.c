/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include "internal_yogrt.h"

char *internal_backend_name(void)
{
	return "none";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	int rem;

	if (cached == -1) {
		debug("\"none\" implementation must be intialized with YOGRT_DEFAULT_LIMIT\n");
		return -1;
	}

	rem = (cached - (now - last_update));
/* 	debug2("\"none\" implementation reports remaining time of %d sec.\n", rem); */
	return rem;
}

int internal_get_rank(void)
{
	return 0;
}

