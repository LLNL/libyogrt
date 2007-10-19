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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define CALLS 10000

main(int argc, char *argv[])
{
	int i;
	int rem;
	struct timeval tv1[1], tv2[2], tv3[1];
	double t1, t2, t3;

 	gettimeofday(tv1, NULL);
	rem = yogrt_remaining();
 	gettimeofday(tv2, NULL);
	for (i = 0; i < CALLS; i++) {
		rem = yogrt_remaining();
	}
	gettimeofday(tv3, NULL);

	t1 = tv1->tv_sec + (tv1->tv_usec * (double)0.000001);
	t2 = tv2->tv_sec + (tv2->tv_usec * (double)0.000001);
	t3 = tv3->tv_sec + (tv3->tv_usec * (double)0.000001);

	printf("yogrt_remaining first call:\n\t%.03f msec\n", (t2 - t1) * 1000);
	printf("yogrt_remaining cached timing:\n");
	printf("\t%.0f calls/sec\n", CALLS / (t3 - t2));
	printf("\t%.3f usecs/call\n", (t3 - t2) / CALLS * 100000);
	exit(0);
}
