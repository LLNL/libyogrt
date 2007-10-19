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

#include <limits.h>

#include "internal_yogrt.h"

int verbosity = 0;

void internal_init(int verb)
{
	verbosity = verb;
}

char *internal_backend_name(void)
{
	return "none";
}

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

int internal_get_rank(void)
{
	return 0;
}

int internal_fudge(void)
{
	return 0;
}
