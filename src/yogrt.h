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
 * may be modified with the "interval" functions below.
 */
extern int yogrt_remaining(void);

/*
 * libyogrt's internally cached remaining time variable may
 * by modified using the function yogrt_set_remaining().  Keep in mind that
 * libyogrt will continue to count down from the new cached remaining time,
 * and after the appropriate interval1 or interval2 has passed after calling
 * yogrt_set_remaining(), libyogrt will contact the resource manager for
 * a new remaining time update.
 *
 * Most programs will not have a use for this function.
 */
extern void yogrt_set_remaining(int seconds);

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

/*
 * Report or set libyogrt's internal debugging level.
 * The libyogrt library can print debugging messages to stderr
 * by setting its debugging level to an integer value greater
 * than 0.  Larger values will result in more verbose debugging messages (the
 * current highest verbosity level is 3).  Users may query and set
 * the debug value using yogrt_set_debug() and yogrt_get_debug().
 */
extern int yogrt_get_debug(void);
extern void yogrt_set_debug(int val);

/*
 * Initialize the yogrt library, including loading the
 * configured libyogrt backend library. It is not necessay
 * for applications to call this, as all of the other library
 * calls will initialize the library autocatically when
 * needed. This function is mainly provided for completeness.
 */
extern int yogrt_init(void);

/*
 * Finalize (shut down) the libyogrt library. This will
 * unload the backend library, and cleanup as needed. It is
 * also not necesary for applications to call this, however
 * doing so may eliminate warnings from memory debuggers.
 */
extern void yogrt_fini(void);

#ifdef __cplusplus
}
#endif

#endif /* !YOGRT_H */
