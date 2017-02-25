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
#include <yogrt.h>

static void 
usage(void)
{
	fprintf(stderr,
		"Usage: yogrt [OPTIONS]\n"
		" -r      Output time remaining in seconds (default action)\n" 
		" -d      Increase debug verbosity, can be specified multiple times\n"
		" -h      Output help\n"
		);
	exit(1);
}

int main(int argc, char *argv[])
{
	int ropt = 0;
	int debugcount = 0;
	int c;

	while ((c = getopt(argc, argv, "rdh")) != EOF) {
		switch (c) {
		case 'r':
			ropt++;
			break;
		case 'd':
			debugcount++;
			break;
		case 'h':
			usage();
			break;
		default:
			usage();
			break;
		}
	}

	if (!ropt) {
		ropt++;
	}

	if (debugcount) {
		yogrt_set_debug(debugcount);
	}

	if (ropt) {
		int remaining = yogrt_remaining();
		printf("%d\n", remaining);
	}

	exit(0);
}
