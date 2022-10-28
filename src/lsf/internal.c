/***************************************************************************
 *  Copyright (C) 2017, Lawrence Livermore National Security, LLC.
 *  Produced at the Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Adam Moody <moody20@llnl.gov>.
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
#include <lsf/lsbatch.h>

#include "internal_yogrt.h"

/* This uses LSF's LSBLIB function to get the time remaining in a job.
 * At the time of writing, documentation for this API was available here:
 *
 * https://www.ibm.com/support/knowledgecenter/SSWRJV_10.1.0/api_reference/index.html
 *
 * http://publibfp.dhe.ibm.com/epubs/pdf/c2753121.pdf */

int _yogrt_verbosity = 0;
int jobid_valid = 0;
LS_LONG_INT jobid;

int internal_init(int verb)
{
	_yogrt_verbosity = verb;

	/* initialize the lsb library */
	int rc = lsb_init(NULL);
	if (rc != 0) {
		lsb_perror("Error calling lsb_init()");
	}

	/* look up our jobid from $LSB_JOBID and store it */
	char* value = getenv("LSB_JOBID");
	if (value) {
		/* TODO: is there an lsb_ function to convert this string? */
		debug2("LSF: $LSB_JOBID=%s.\n", value);
		jobid = (LS_LONG_INT) strtol(value, NULL, 10);
		jobid_valid = 1;
	} else {
		/* ERROR: failed to get job id */
		error("LSF: Failed to get jobid. $LSB_JOBID is not set!\n");
        	return 0;
	}

        /* Don't know how to determine if job is running under Moab.
           Just assume always valid. */
        return 1;
}

char *internal_backend_name(void)
{
	return "LSF";
}

int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
        int secs_left = 0;

	/* only do this lookup with a valid jobid */
	if (! jobid_valid) {
		error("LSF: No valid jobid to lookup!\n");
		return -1;
	}

	/* open connection for job records */
	int jobs = lsb_openjobinfo(jobid, NULL, NULL, NULL, NULL, CUR_JOB);
	if (jobs == -1) {
		error("LSF: Error calling lsb_openjobinfo() for jobid=%s.\n", lsb_jobid2str(jobid));
		lsb_perror("Error opening jobs");
		return -1;
	}
	
	debug2("LSF: lsb_openjobinfo() job count=%d.\n", jobs);
	if (jobs == 1) {
		/* get job info for this job */
		struct jobInfoEnt* job = lsb_readjobinfo(&jobs);
		if (job == NULL) {
			error("LSF: Failed to read job info for jobid=%s: lsb_readjobinfo() failed.\n", lsb_jobid2str(jobid));
			lsb_perror("Failed to read job info");
			secs_left = -1;
		} else {
			/* get run limit of the job */
			int run_limit = job->submit.rLimits[LSF_RLIMIT_RUN];
			debug2("LSF: RunLimit=%d.\n", run_limit);
			if (run_limit < 0) {
				/* there is no run limit imposed on the job,
 				 * so return the max time */
				secs_left = INT_MAX;
			} else {
				/* got a valid run limit,
 				 * get current run time for the job */
				int run_time  = job->runTime;

				/* compute time remaining */
				secs_left = run_limit - run_time;
				if (secs_left < 0) {
					secs_left = 0;
				}

				debug2("LSF: RunLimit=%d RunTime=%d Rem=%d.\n", run_limit, run_time, secs_left);
			}
		}
	} else if (jobs > 1) {
		error("LSF: lsb_readjobinfo() returned more than one job for jobid=%s.\n", lsb_jobid2str(jobid));
		secs_left = -1;
	} else {
		error("LSF: Failed to find info for jobid=%s.\n", lsb_jobid2str(jobid));
		secs_left = -1;
	}

	/* close the connection */
	lsb_closejobinfo();

	debug2("LSF reports remaining time of %d sec.\n", secs_left);
	return secs_left;
}

int internal_get_rank(void)
{
	return 0;
}

int internal_fudge(void)
{
	return 0;
}
