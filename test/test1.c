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
 * If libyogrt is installed in a non-standard directory, use the following
 * to compile:
 *
 * gcc foo.c -o foo -I${YOGRT}/include -L${YOGRT}/lib -Wl,--rpath -Wl,${YOGRT}/lib -lyogrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <yogrt.h>

main(int argc, char *argv[])
{
	int rem;
        int i;

        yogrt_init();

	for (i=0; i<10; i++) {
		rem = yogrt_remaining();
		sleep(1);
	}

        yogrt_fini();
	exit(0);
}
