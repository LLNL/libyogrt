/***************************************************************************
 *  Copyright (C) 2017, Lawrence Livermore National Security, LLC.
 *  Produced at the Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Olaf Faaland <faaland1@llnl.gov>
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


#include <stdio.h>
#include <jansson.h>
#include <time.h>
#include <limits.h>
#include <flux/core.h>

#include "internal_yogrt.h"

#define BOGUS_TIME -1

int jobid_valid = 0;
int verbosity = 0;

static flux_jobid_t jobid = 0;

int internal_init(int verb)
{
	char *jobid_str;

	verbosity = verb;
	jobid_valid = 0;

	if ((jobid_str = getenv("FLUX_JOB_ID")) == NULL) {
		error("ERROR: FLUX_JOB_ID is not set."
			  " Remaining time will be a bogus value.\n");
		return jobid_valid;
	}

	if (flux_job_id_parse(jobid_str, &jobid) < 0) {
		error("ERROR: Unable to parse FLUX_JOB_ID %s."
			  " Remaining time will be a bogus value.\n", jobid_str);
		return jobid_valid;
	}

	jobid_valid = 1;

	return jobid_valid;
}

char *internal_backend_name(void)
{
	return "FLUX";
}

static int get_job_expiration(flux_jobid_t id, long int *expiration)
{
	flux_t *h = NULL;
	flux_t *child_handle = NULL;
	flux_future_t *f;
	json_t *job;
	json_t *value;
	double exp;
	const char *uri = NULL;
	int rc = -1;

	if (!(h = flux_open(NULL, 0))) {
		error("ERROR: flux_open() failed with errno %d\n", errno);
		goto out;
	}

	/*
	 * Determine whether to ask our parent or not
	 * See https://github.com/flux-framework/flux-core/issues/3817
	 */

	if (!getenv("FLUX_KVS_NAMESPACE")) {
		uri = flux_attr_get(h, "parent-uri");
		if (!uri) {
			error("ERROR: no FLUX_KVS_NAMESPACE and flux_attr_get failed with "
				  "errno %d\n", errno);
			goto out;
		}

		child_handle = h;
		h = flux_open(uri, 0);
		if (!h) {
			printf("flux_open with parent-uri %s failed with errno %d\n", uri,
				   errno);
			goto out;
		}
	}

	if (!(f = flux_job_list_id(h, jobid, "[\"expiration\"]"))) {
		error("ERROR: flux_job_list failed with errno %d.\n", errno);
		goto out;
	}

	if (flux_rpc_get_unpack (f, "{s:{s:f}}", "job", "expiration", &exp) < 0) {
		error("ERROR: flux_rpc_get_unpack failed with errno %d.\n", errno);
		goto out;
	}

	*expiration = (long int) exp;
	rc = 0;

out:
	flux_future_destroy(f);
	flux_close(h);
	flux_close(child_handle);

	return rc;
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	long int expiration;
	int remaining_sec = BOGUS_TIME;

	if (! jobid_valid) {
		error("FLUX: No valid jobid to lookup!\n");
		return BOGUS_TIME;
	}

	if (get_job_expiration(jobid, &expiration)) {
		error("FLUX: get_job_expiration failed\n");
		goto out;
	}

	remaining_sec = (int) (expiration - time(NULL));
	debug("flux remaining seconds is %ld\n", remaining_sec);

out:
	return remaining_sec;
}

int internal_get_rank(void)
{
	char *rank_str;

	rank_str = getenv("FLUX_TASK_RANK");

	if (rank_str) {
		return atoi(rank_str);
	} else {
		return 0;
	}
}

int internal_fudge(void)
{
	return 0;
}

/*
 * vim: tabstop=8 shiftwidth=8 smartindent:
 */
