/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <time.h>
#include "yogrt.h"
#include "internal_yogrt.h"

static long interval1 = 900; /* 15 minutes */
static long interval2 = 300; /* 5 minutes */
static long interval2_start = 1800; /* 30 minutes before the end */

static long cached_time_rem = -1;
static time_t last_update = (time_t)-1;

#define REMAINING(now) (cached_time_rem - ((now) - last_update))

/*
 * Returns 1 if update is needed, 0 otherwise.
 */
static inline int need_update(time_t now)
{
	if (last_update == -1) {
		/* first time yogrt_get_time has been called */
		return 1;
	} else {
		if (REMAINING(now) > interval2_start) {
			if ((now - last_update) > interval1)
				return 1;
		} else {
			if ((now - last_update) > interval2)
				return 1;
		}
	}

	return 0;
}

long yogrt_get_time(void)
{
	time_t now = time(NULL);
	long rem;

	if (need_update(now)) {
		rem = _internal_get_rem_time(now, last_update, cached_time_rem);
		if (rem != -1) {
			cached_time_rem = rem;
			last_update = now;
		}
	}

	if (cached_time_rem == -1) {
		return -1;
	}

	rem = REMAINING(now);
	if (rem >= 0) {
		return rem;
	} else {
		return 0;
	}
}

void yogrt_set_update_interval1(long seconds)
{
	interval1 = seconds;
}

void yogrt_set_update_interval2(long seconds)
{
	interval2 == seconds;
}

void yogrt_set_interval2_start(long seconds_before_end)
{
	interval2_start = seconds_before_end;
}
