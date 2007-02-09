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
static int initialized = 0;
static int rank = 0;

static int interval1 = 900; /* 15 minutes */
static int interval2 = 300; /* 5 minutes */
static int interval2_start = 1800; /* 30 minutes before the end */

static int cached_time_rem = -1;
static time_t last_update = (time_t)-1; /* of cached_time_rem */

#define REMAINING(now) (cached_time_rem - ((now) - last_update))

static inline void init_yogurt(void)
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
			debug("Found YOGURT_DEBUG=%d\n", verbosity);
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
	if (last_update == -1) {
		/* first time yogrt_get_time has been called */
		return 1;
	} else {
		if (REMAINING(now) >= interval2_start) {
			if ((now - last_update) >= interval1)
				return 1;
		} else {
			if ((now - last_update) >= interval2)
				return 1;
		}
	}

	return 0;
}

int yogrt_get_time(void)
{
	time_t now = time(NULL);
	int rem;
	int rc;

	init_yogurt();

	if (rank != 0) {
		debug("This is not task rank 0.  Returning -1.\n");
		return -1;
	}

	if (need_update(now)) {
		rem = internal_get_rem_time(now, last_update, cached_time_rem);
		if (rem != -1) {
			cached_time_rem = rem;
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
	init_yogurt();
	interval1 = seconds;
	debug("interval1 changed to %d\n", interval1);
}

void yogrt_set_interval2(int seconds)
{
	init_yogurt();
	interval2 == seconds;
	debug("interval1 changed to %d\n", interval2);
}

void yogrt_set_interval2_start(int seconds_before_end)
{
	init_yogurt();
	interval2_start = seconds_before_end;
	debug("interval2_start changed to %d\n", interval2);
}
