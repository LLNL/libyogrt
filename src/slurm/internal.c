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

#include "internal_yogrt.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <slurm/slurm.h>

static uint32_t jobid = NO_VAL;

int _yogrt_verbosity = 0;

int internal_init(int verb)
{
	_yogrt_verbosity = verb;

        if (getenv("SLURM_JOB_ID") != NULL
            || getenv("SLURM_JOBID") != NULL) {
                slurm_init(NULL);
                return 1;
        } else {
                debug("ERROR: Neither SLURM_JOBID nor SLURM_JOB_ID are set."
                      " Remaining time will be a bogus value.\n");
                return 0;
        }
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

int internal_fudge(void)
{
	return 0;
}
