/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include "internal_yogrt.h"

long _internal_get_rem_time(time_t now, time_t last_update, long cached)
{
	if (cached == -1) {
		return 7200; /* 2 hours */
	} else {
		return (cached - (now - last_update));
	}
}
