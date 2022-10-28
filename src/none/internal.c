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

/*
 * This "none" module should serve as a good template for developing other
 * libyogrt backend modules.  Backend modules must be dynamically linked
 * libraries which are dlopen()ed by the main libyogrt library.
 *
 * There are five functions that libyogrt will look for in a backend library:
 *
 * void  internal_init(int verb);
 * char *internal_backend_name(void);
 * int   internal_get_rem_time(time_t now, time_t last_update, int cached);
 * int   internal_get_rank(void);
 * int   internal_fudge(void);
 *
 */

#include <limits.h>

#include "internal_yogrt.h"

/*
 * The error() and debug*() macros in internal_yogrt.h need the global variable
 * "verbosity" to be initalized.  All backends should have this internal_init()
 * function and set "verbosity" if they intend to use the debugging helper
 * macros.
 */
int _yogrt_verbosity = 0;

int internal_init(int verb)
{
	_yogrt_verbosity = verb;

        /* internal_init() must return non-zero if the current environment
           is valid for using the implemented resource manager.  If
           internal_init() returns zero, yogrt_remaining() will never call
           internal_get_rem_time(), instead always returning INT_MAX to the
           user. */
        return 1;
}

/*
 * Return a string identifying this module.
 */
char *internal_backend_name(void)
{
	return "none";
}

/*
 * Look up the time remaining in the current resource allocation.
 *
 * Most backend modules will probably ignore the input parameters
 * to this function, and simply contact their respective resource manager
 * to get the remaining time.  However, this "none" module just counts
 * down from some arbitrary time value, either INT_MAX or the time specified
 * in the environment variable YOGRT_REMAINING.
 *
 * IN now - The current time, in seconds since the epoch.
 * IN last_update - The last time that internal_get_rem_time() was called
 *                  by libyogrt, in seconds since the epoch.
 * IN cached - libyogrt's cached return value from the previous call to 
 *             internal_get_rem_time().  In other words, "cached" is the
 *             time remaining in the allocation at the time of the
 *             "last_update".  Will be -1 the first time this function
 *             is called.
 *
 * Return the seconds remaining in the resource allocation, or -1
 * on error.
 */
int internal_get_rem_time(time_t now, time_t last_update, int cached)
{
	int rem;

	if (cached == -1) {
		return INT_MAX;
	}

	rem = (cached - (now - last_update));
	debug2("\"none\" backend reports remaining time of %d sec.\n", rem);
	return rem;
}

/*
 * The logical rank of the calling process withing the parallel application.
 * libyogrt only guarantees that process rank 0 may call yogrt_remaining().
 * This rank number is not exposed to the user through the libyogrt API.
 * The yogrt_remaining() function will return an error of -1 for any processes
 * for which internal_get_rank() returns non-zero.
 *
 * Return the rank of the calling process.  If the rank number cannot be
 * determined, return 0 if it is safe for the the calling process to
 * call yogrt_remaining(), and non-zero otherwise.
 */
int internal_get_rank(void)
{
	return 0;
}

/*
 * The "fudge factor" that libyogrt should apply to the remaining time
 * reported by the resource manager.
 *
 * Some resource managers are infamous for the inaccurately reporting
 * the remaining time.  For instance, one resource manager will often
 * report that 5 minutes remain in a job allocation, and then kill the
 * job after only 4 minutes.  For that resource manger, we use a fudge
 * factor of 60.  With a fudge factor of 60, if internal_get_rem_time()
 * returns 300, yogrt_remaining() would return 240 to the user.
 *
 * Be aware that this value is just the default fudge factor for this backend.
 * This default can be overridden by a system administrator in the yogrt.conf
 * file, or by anyone in the YOGRT_FUDGE_FACTOR environment variable.
 *
 * Return the number of seconds that should be deducted from the time
 * remaining, as reported by internal_get_rem_time().
 */
int internal_fudge(void)
{
	return 0;
}
