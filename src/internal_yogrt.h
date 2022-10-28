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

#ifndef INTERNAL_YOGRT_H
#define INTERNAL_YOGRT_H 1

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include <time.h>

extern int _yogrt_verbosity;

int internal_init(int verb);

char *internal_backend_name(void);

int internal_get_rem_time(time_t now, time_t last_update, int cached);

int internal_get_rank(void);

int internal_fudge(void);

#define __debug_level(level, ...)					\
	do {								\
		if (_yogrt_verbosity >= level) {				\
			fprintf(stderr, "libyogrt: "__VA_ARGS__);	\
		}							\
	} while (0)

#define error(...)  __debug_level(0, __VA_ARGS__)
#define debug(...)  __debug_level(1, __VA_ARGS__)
#define debug1(...) __debug_level(1, __VA_ARGS__)
#define debug2(...) __debug_level(2, __VA_ARGS__)
#define debug3(...) __debug_level(3, __VA_ARGS__)

#endif /* !INTERNAL_YOGRT_H */
