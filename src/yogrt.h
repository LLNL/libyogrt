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

#ifndef YOGRT_H
#define YOGRT_H 1

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Return the number of seconds remaining in the current resource allocation.
 * (This call may ONLY be used by process rank 0 of a parallel job.)
 *
 * yogrt_remaining()'s goal is to unify and simplify the process of retrieving
 * the remaining time available to a job regardless of the
 * underlying system resource manager (e.g. SLURM, Moab, etc.).
 *
 * Performance is a very important design criteria for yogrt_remaining().
 * To acheive the required speed, yogrt_remaining() talks to the resource manager
 * only rarely and caches the remaining time.  Most calls to yogrt_remaining()
 * will only involve a time() library call and simple arithmetic to determine
 * the current remaining time.
 *
 * Exactly how often yogrt_remaining() will talk to the resource manager
 * may be modified with the functions below.
 */
extern int yogrt_remaining(void);

/*
 * The following functions allow the user to fine tune the internal behavior
 * of the yogrt_remaining() function.
 *
 * On the first call to yogrt_remaining(), it will cache the "remaining time"
 * value reported by the resource manager.  Subsequent calls to yogrt_remaining()
 * will use simple arithmetic to calculate the remaining time.  If the cached
 * time is older than the update interval, yogrt_remaining() will connect to the
 * the resource manager again to ask for the latest "remaining time" value.
 *
 * There are actually two update intervals that may be set.  The first,
 * "interval1", is used as long as the remaining time is greater than
 * "interval2_start" seconds.  Once the remaining time is less than
 * "interval2_start" seconds, yogrt_remaining() will use the second update
 * interval, "interval2".
 *
 * It should be noted that yogrt_remaining() can and will switch back to using
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

#ifdef __cplusplus
}
#endif

#endif /* !YOGRT_H */
