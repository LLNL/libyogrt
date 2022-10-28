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
#include <liblrm.h>
#include <lrmerrno.h>

#include "internal_yogrt.h"

int _yogrt_verbosity = 0;

int internal_init(int verb)
{
        char *ptr;

	_yogrt_verbosity = verb;

        if (((ptr = getenv("LRM_ENVIRONMENT")) != NULL)
            && strcmp(ptr, "BATCH") == 0) {
                return 1;
        } else {
                debug("ERROR: Environment variable LRM_ENVIRONMENT=BATCH is"
                      " not set. Remaining time will be a bogus value.\n");

                return 0;
        }
}

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
	_yogrt_verbosity = verb;
}

int internal_fudge(void)
{
	return 60;
}
