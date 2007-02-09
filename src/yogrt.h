/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#ifndef YOGRT_H
#define YOGRT_H 1

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

/*
 * Return the number of seconds remaining in the current resource allocation.
 * (This call may ONLY be used by process rank 0 of a parallel job.)
 *
 * yogrt_get_time()'s goal is to unify and simplify the process of retrieving
 * the remaining time available to a job regardless of the
 * underlying system resource manager (e.g. SLURM, Moab, etc.).
 *
 * Performance is a very important design criteria for yogrt_get_time().
 * To acheive the required speed, yogrt_get_time() talks to the resource manager
 * only rarely and caches the remaining time.  Most calls to yogrt_get_time()
 * will only involve a time() library call and simple arithmetic to determine
 * the current remaining time.
 *
 * Exactly how often yogrt_get_time() will talk to the resource manager
 * may be modified with the functions below.
 */
extern int yogrt_get_time(void);

/*
 * The following functions allow the user to fine tune the internal behavior
 * of the yogrt_get_time() function.
 *
 * On the first call to yogrt_get_time(), it will cache the "remaining time"
 * value reported by the resource manager.  Subsequent calls to yogrt_get_time()
 * will use simple arithmetic to calculate the remaining time.  If the cached
 * time is older than the update interval, yogrt_get_time() will connect to the
 * the resource manager again to ask for the latest "reamining time" value.
 *
 * There are actually two update intervals that may be set.  The first,
 * "interval1", is used as long as the remaining time is greater than
 * "interval2_start" seconds.  Once the remaining time is less than
 * "interval2_start" seconds, yogrt_get_time() will use the second update
 * interval, "interval2".
 *
 * It should be noted that yogrt_get_time() can and will switch back to using
 * interval1 if the resource manager sufficiently increases the 
 * remaining time of a job.
 */
extern void yogrt_set_interval1(int seconds);
extern void yogrt_set_interval2(int seconds);
extern void yogrt_set_interval2_start(int seconds_before_end);

/*
 * Report the current value of update interval1.
 */
extern int yogrt_get_interval1(void);

/*
 * Report the current value of update interval2.
 */
extern int yogrt_get_interval2(void);

/*
 * Report the current value of interval2_start.
 */
extern int yogrt_get_interval2_start(void);

#endif /* !YOGRT_H */
