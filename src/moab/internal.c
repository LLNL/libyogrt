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

#include <stdlib.h>
#include <moab/mapi.h>

#include "internal_yogrt.h"

int _yogrt_verbosity = 0;

int internal_init(int verb)
{
	_yogrt_verbosity = verb;

        /* Don't know how to determine if job is running under Moab.
           Just assume always valid. */
        return 1;
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

int internal_fudge(void)
{
	return 0;
}
