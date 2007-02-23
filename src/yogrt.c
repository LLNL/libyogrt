/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "yogrt.h"
#include "internal_yogrt.h"

int verbosity = 0;
static int initialized = 0; /* flag */
static int rank = 0;

static int interval1 = 900; /* 15 minutes */
static int interval2 = 300; /* 5 minutes */
static int interval2_start = 1800; /* 30 minutes before the end */

static int cached_time_rem = -1;
static time_t last_update = (time_t)-1; /* of cached_time_rem */
static int last_update_failed = 0; /* flag */
#define FAILED_UPDATE_INTERVAL 300

#define REMAINING(now) (cached_time_rem - ((now) - last_update))

static inline void init_yogrt(void)
{
	if (initialized == 0) {
		char *p;

		initialized = 1;

		if ((p = getenv("YOGRT_DEBUG")) != NULL) {
			verbosity = atol(p);
		}
		debug("Backend implementation is \"%s\".\n",
		      internal_backend_name());
		if (p != NULL) {
			debug("Found YOGRT_DEBUG=%d\n", verbosity);
		}
		if ((p = getenv("YOGRT_INTERVAL1")) != NULL) {
			interval1 = atol(p);
			debug("Found YOGRT_INTERVAL1=%d\n", interval1);
		}
		if ((p = getenv("YOGRT_INTERVAL2")) != NULL) {
			interval2 = atol(p);
			debug("Found YOGRT_INTERVAL2=%d\n", interval2);
		}
		if ((p = getenv("YOGRT_INTERVAL2_START")) != NULL) {
			interval2_start = atol(p);
			debug("Found YOGRT_INTERVAL2_START=%d\n",
			      interval2_start);
		}
		if ((p = getenv("YOGRT_DEFAULT_LIMIT")) != NULL) {
			cached_time_rem = atol(p);
			last_update = time(NULL);
			debug("Found YOGRT_DEFAULT_LIMIT=%d\n",
			      cached_time_rem);
		}
		rank = internal_get_rank();
		debug("Rank is %d\n", rank);
	}
}

/*
 * Returns 1 if update is needed, 0 otherwise.
 */
static inline int need_update(time_t now)
{
	int rem = REMAINING(now);
	int last = now - last_update;
	int rc;

	if (last_update == -1) {
		/* first time yogrt_get_time has been called */
		rc = 1;
	} else if ((rem >= interval2_start) && (last >= interval1)) {
		rc = 1;
	} else if ((rem < interval2_start) && (last >= interval2)) {
		rc = 1;
	} else if (last_update_failed && (last >= FAILED_UPDATE_INTERVAL)) {
		/* update sooner than the normal interval
		   would require if the last update failed */
		debug("Last update failed, trying again early.\n");
		rc = 1;
	} else {
		rc = 0;
	}

	return rc;
}

int yogrt_get_time(void)
{
	time_t now = time(NULL);
	int rem;
	int rc;

	init_yogrt();

	if (rank != 0) {
		debug("This is not task rank 0.  Returning -1.\n");
		return -1;
	}

	if (need_update(now)) {
		rem = internal_get_rem_time(now, last_update, cached_time_rem);
		if (rem != -1) {
			last_update_failed = 0;
			cached_time_rem = rem;
			last_update = now;
		} else {
			debug("Update failed, will try again in"
			      " at least %d sec.\n", FAILED_UPDATE_INTERVAL);
			last_update_failed = 1;
			cached_time_rem -= (now - last_update);
			last_update = now;
		}
	}

	if (cached_time_rem == -1) {
		rc = INT_MAX;
	} else {
		rem = REMAINING(now);
		if (rem >= 0) {
			rc = rem;
		} else {
			rc = 0;
		}
	}

	debug3("Reporting remaining time of %d sec.\n", rc);
	return rc;
}

void yogrt_set_interval1(int seconds)
{
	init_yogrt();
	interval1 = seconds;
	debug("interval1 changed to %d\n", interval1);
}

void yogrt_set_interval2(int seconds)
{
	init_yogrt();
	interval2 = seconds;
	debug("interval1 changed to %d\n", interval2);
}

void yogrt_set_interval2_start(int seconds_before_end)
{
	init_yogrt();
	interval2_start = seconds_before_end;
	debug("interval2_start changed to %d\n", interval2);
}

int yogrt_get_interval1(void)
{
	init_yogrt();
	return interval1;
}

int yogrt_get_interval2(void)
{
	init_yogrt();
	return interval2;
}

int yogrt_get_interval2_start(void)
{
	init_yogrt();
	return interval2_start;
}

/**********************************************************************
 * Fortran wrappers (single underscore version)
 **********************************************************************/
int iyogrt_get_time_(void)
{
	return yogrt_get_time();
}

int iyogrt_set_interval1_(int *seconds)
{
	yogrt_set_interval1(*seconds);
	return *seconds;
}

int iyogrt_set_interval2_(int *seconds)
{
	yogrt_set_interval2(*seconds);
	return *seconds;
}

int iyogrt_set_interval2_start_(int *seconds_before_end)
{
	yogrt_set_interval2_start(*seconds_before_end);
	return *seconds_before_end;
}

int iyogrt_get_interval1_(void)
{
	return yogrt_get_interval1();
}

int iyogrt_get_interval2_(void)
{
	return yogrt_get_interval2();
}

int iyogrt_get_interval2_start_(void)
{
	return yogrt_get_interval2_start();
}

/**********************************************************************
 * Fortran wrappers (double underscore version)
 **********************************************************************/
int iyogrt_get_time__(void)
{
	return yogrt_get_time();
}

int iyogrt_set_interval1__(int *seconds)
{
	yogrt_set_interval1(*seconds);
	return *seconds;
}

int iyogrt_set_interval2__(int *seconds)
{
	yogrt_set_interval2(*seconds);
	return *seconds;
}

int iyogrt_set_interval2_start__(int *seconds_before_end)
{
	yogrt_set_interval2_start(*seconds_before_end);
	return *seconds_before_end;
}

int iyogrt_get_interval1__(void)
{
	return yogrt_get_interval1();
}

int iyogrt_get_interval2__(void)
{
	return yogrt_get_interval2();
}

int iyogrt_get_interval2_start__(void)
{
	return yogrt_get_interval2_start();
}
