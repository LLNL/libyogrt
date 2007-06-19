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
#include <stdint.h>
#include <limits.h>
#include <slurm/slurm.h>

main()
{
	char *jobid_str;
	uint32_t jobid;

	jobid_str = getenv("SLURM_JOB_ID");
	if (jobid_str == NULL) {
		jobid_str = getenv("SLURM_JOBID");
		if (jobid_str == NULL) {
			printf("%d\n", INT_MAX);
			exit(1);
		}
	}

	jobid = (uint32_t)atol(jobid_str);

	printf("%ld\n", slurm_get_rem_time(jobid));

	exit(0);
}
