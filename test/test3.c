/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

main(int argc, char *argv[])
{
	int i;
	int rem;
	struct timeval tv1[1], tv2[2], tv3[1];
	double t1, t2, t3;

	while(1) {
	 	gettimeofday(tv1, NULL);
		rem = yogrt_remaining();
	 	gettimeofday(tv2, NULL);

		t1 = tv1->tv_sec + (tv1->tv_usec * (double)0.000001);
		t2 = tv2->tv_sec + (tv2->tv_usec * (double)0.000001);

		printf("rem = %-8d, yogrt_remaining overhead: %.03f msec\n",
		       rem, (t2 - t1) * 1000);
		sleep(1);
	}
	exit(0);
}
