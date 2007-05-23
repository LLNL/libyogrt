/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#ifndef INTERNAL_YOGRT_H
#define INTERNAL_YOGRT_H 1

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include <time.h>

extern int verbosity;

char *internal_backend_name(void);

int internal_get_rem_time(time_t now, time_t last_update, int cached);

int internal_get_rank(void);

#define __debug_level(level, ...)					\
	do {								\
		if (verbosity >= level) {				\
			fprintf(stderr, "libyogurt: "__VA_ARGS__);	\
		}							\
	} while (0)

#define error(...)  __debug_level(0, __VA_ARGS__)
#define debug(...)  __debug_level(1, __VA_ARGS__)
#define debug1(...) __debug_level(1, __VA_ARGS__)
#define debug2(...) __debug_level(2, __VA_ARGS__)
#define debug3(...) __debug_level(3, __VA_ARGS__)

#endif /* !INTERNAL_YOGRT_H */
