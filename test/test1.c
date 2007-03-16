/***************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Christopher J. Morrone <morrone2@llnl.gov>.
 *  UCRL-CODE-XXXXXX.
 ***************************************************************************
LICENSE
 ***************************************************************************/

/*
 * If libyogrt is installed in a non-standard directory, use the following
 * to compile:
 *
 * gcc foo.c -o foo -I${YOGRT}/include -L${YOGRT}/lib -Wl,--rpath -Wl,${YOGRT}/lib -lyogrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	int rem;

	while(1) {
		rem = yogrt_remaining();
		sleep(1);
	}

	exit(0);
}
