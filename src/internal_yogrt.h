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

#include <time.h>

long _internal_get_rem_time(time_t now, time_t last_update, long cached);

#endif /* !YOGRT_H */
