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
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dlfcn.h>

#include "yogrt.h"
#include "internal_yogrt.h"

#define FAILED_UPDATE_INTERVAL 300
#define REMAINING(now) (cached_time_rem - ((now) - last_update))

int _yogrt_verbosity = 0;
static int initialized = 0; /* flag */
static int rank = 0;
static int valid = 1; /* Is the environment valid for contacting the
                         resource manager? */

static int interval1 = 900; /* 15 minutes */
static int interval2 = 300; /* 5 minutes */
static int interval2_start = 1800; /* 30 minutes before the end */

static int cached_time_rem = -1;
static time_t last_update = (time_t)-1; /* of cached_time_rem */
static int last_update_failed = 0; /* flag */
static int fudge_factor = 0;
static int fudge_factor_set = 0;

static char backend_name[64];
static void *backend_handle = NULL;
static struct backend_operations {
	int    (*init)     (int verb);
	char * (*name)     (void);
	int    (*remaining)(time_t now, time_t last_update, int chached);
	int    (*rank)     (void);
	int    (*fudge)    (void);
} backend;


static inline void strip_whitespace(char *str)
{
	int i, j;
	int len = strlen(str);

	for (i = 0, j = 0; i < len; i++) {
		if (!isspace(str[i])) {
			str[j] = str[i];
			j++;
		}
	}
	str[j] = '\0';
}

static inline void read_config_file(void)
{
	FILE *fp;
	char line[256];
	char *eq_ptr;
	int len;
	int content_flag;
	int i;

	fp = fopen(CONFIGPATH "/yogrt.conf", "r");
	if (fp == NULL) {
		debug("Config file " CONFIGPATH "/yogrt.conf not found.\n");
		return;
	}

	debug("Reading config file " CONFIGPATH "/yogrt.conf.\n");
	while (fgets(line, sizeof(line), fp)) {
		char *key, *value;

		len = strlen(line);
		if (len == 0)
			continue;
		if (line[0] == '#')
			continue;
		content_flag = 0;
		eq_ptr = NULL;
		for (i = 0; i < len; i++) {
			if (!isspace(line[i])) {
				content_flag = 1;
				if (line[i] == '=')
					eq_ptr = line + i;
			}
			if (line[i] == '\n') /* strip newline character */
				line[i] = '\0';
		}
		if (!content_flag)
			continue;
		if (eq_ptr == NULL)
			continue;

		key = line;
		value = eq_ptr + 1;
		*eq_ptr = '\0';

		strip_whitespace(key);
		strip_whitespace(value);

		if (strcasecmp(key, "debug") == 0
		    || strcasecmp(key, "yogrt_debug") == 0) {
			_yogrt_verbosity = (int)atol(value);
			debug("In yogrt.conf: %s=%d\n", key, _yogrt_verbosity);
		} else if (strcasecmp(key, "interval1") == 0
			   || strcasecmp(key, "yogrt_interval1") == 0) {
			interval1 = (int)atol(value);
			debug("In yogrt.conf: %s=%d\n", key, interval1);
			if (interval1 < 0) {
				interval1 = 0;
				debug("Negative number not allowed,"
				      " setting interval1 to 0\n");
			}
		} else if (strcasecmp(key, "interval2") == 0
			   || strcasecmp(key, "yogrt_interval2") == 0) {
			interval2 = (int)atol(value);
			debug("In yogrt.conf: %s=%d\n", key, interval2);
			if (interval2 < 0) {
				interval2 = 0;
				debug("Negative number not allowed,"
				      " setting interval2 to 0\n");
			}
		} else if (strcasecmp(key, "interval2_start") == 0
			   || strcasecmp(key, "yogrt_interval2_start") == 0) {
			interval2_start = (int)atol(value);
			debug("In yogrt.conf: %s=%d\n", key, interval2_start);
			if (interval2_start < 0) {
				interval2_start = 0;
				debug("Negative number not allowed,"
				      " setting interval2_start to 0\n");
			}
		} else if (strcasecmp(key, "remaining") == 0
			   || strcasecmp(key, "yogrt_remaining") == 0) {
			cached_time_rem = (int)atol(value);
			last_update = time(NULL);
			debug("In yogrt.conf: %s=%d\n", key, cached_time_rem);
			if (cached_time_rem < 0) {
				cached_time_rem = -1;
				debug("Negative number not allowed,  leaving"
				      " YOGRT_REMAINING uninitialized\n");
			}
		} else if (strcasecmp(key, "backend") == 0
			   || strcasecmp(key, "yogrt_backend") == 0) {
			strncpy(backend_name, value, sizeof(backend_name)-1);
			debug("In yogrt.conf: %s=%s\n", key, backend_name);
		} else if (strcasecmp(key, "fudge_factor") == 0
			   || strcasecmp(key, "yogrt_fudge_factor") == 0) {
			fudge_factor = (int)atol(value);
			fudge_factor_set = 1;
			debug("In yogrt.conf: %s=%d\n", key, fudge_factor);
		}
	}
	fclose(fp);
}

static inline void read_env_variables(void)
{
	char *p;

	if ((p = getenv("YOGRT_INTERVAL1")) != NULL) {
		interval1 = (int)atol(p);
		debug("In environment: YOGRT_INTERVAL1=%d\n", interval1);
		if (interval1 < 0) {
			interval1 = 0;
			debug("Negative number not allowed,"
			      " setting interval1 to 0\n");
		}
	}
	if ((p = getenv("YOGRT_INTERVAL2")) != NULL) {
		interval2 = (int)atol(p);
		debug("In environment: YOGRT_INTERVAL2=%d\n", interval2);
		if (interval2 < 0) {
			interval2 = 0;
			debug("Negative number not allowed,"
			      " setting interval2 to 0\n");
		}
	}
	if ((p = getenv("YOGRT_INTERVAL2_START")) != NULL) {
		interval2_start = (int)atol(p);
		debug("In environment: YOGRT_INTERVAL2_START=%d\n",
		      interval2_start);
		if (interval2_start < 0) {
			interval2 = 0;
			debug("Negative number not allowed,"
			      " setting interval2_start to 0\n");
		}
	}
	if ((p = getenv("YOGRT_REMAINING")) != NULL) {
		cached_time_rem = (int)atol(p);
		last_update = time(NULL);
		debug("In environment: YOGRT_REMAINING=%d\n", cached_time_rem);
		if (cached_time_rem < 0) {
			cached_time_rem = -1;
			debug("Negative number not allowed,  leaving"
			      " YOGRT_REMAINING uninitialized\n");
		}
	}
	if ((p = getenv("YOGRT_BACKEND")) != NULL) {
		strncpy(backend_name, p, sizeof(backend_name)-1);
		debug("In environment: YOGRT_BACKEND=%s\n", backend_name);
	}
	if ((p = getenv("YOGRT_FUDGE_FACTOR")) != NULL) {
		fudge_factor = (int)atol(p);
		fudge_factor_set = 1;
		debug("In environment: YOGRT_FUDGE_FACTOR=%d\n", fudge_factor);
	}
}

static inline int load_backend(void)
{
	char path[512];
	int flags;
	struct stat st[1];

	if (strlen(backend_name) == 0) {
		debug("No backend name specified.  Defaulting to \"none\".\n");
		strcpy(backend_name, "none");
	}

	snprintf(path, sizeof(path), "%s/libyogrt-%s.so", BACKENDDIR, backend_name);
	debug3("Testing for %s.\n", path);
	if (stat(path, st) == -1) {
		snprintf(path, sizeof(path), "%s/libyogrt-%s.a",
			 BACKENDDIR, backend_name);
		debug3("Testing for %s.\n", path);
		if (stat(path, st) == -1) {
			error("Unable to locate backend library module!\n");
			return 0;
		}
	}
#ifndef HAVE_AIX
	flags = RTLD_NOW|RTLD_GLOBAL;
#else /* system is AIX */
	/* flags = RTLD_NOW|RTLD_GLOBAL|RTLD_MEMBER; */
	flags = RTLD_NOW|RTLD_MEMBER;
	/* append the member name to make the AIX loader happy */
	snprintf(path, sizeof(path), "%s/libyogrt-%s.a(libyogrt-%s.so.%s)",
		 BACKENDDIR, backend_name, backend_name, LIBYOGRT_LT_CURRENT);
#endif
	debug3("Will use %s.\n", path);

	if ((backend_handle = dlopen(path, flags)) == NULL) {
		error("dlopen failed: %s\n", dlerror());
		return 0;
	}

	backend.init =      dlsym(backend_handle, "internal_init");
	backend.name =      dlsym(backend_handle, "internal_backend_name");
	backend.remaining = dlsym(backend_handle, "internal_get_rem_time");
	backend.rank =      dlsym(backend_handle, "internal_get_rank");
	backend.fudge =     dlsym(backend_handle, "internal_fudge");

	if (backend.init != NULL)
		valid = backend.init(_yogrt_verbosity);
	if (backend.rank != NULL)
		rank = backend.rank();
	debug("Rank is %d\n", rank);
	if (backend.name != NULL)
		debug("Backend implementation is \"%s\".\n", backend.name());
	if (backend.fudge != NULL && !fudge_factor_set) {
		fudge_factor = backend.fudge();
		debug("Using backend fudge factor of %d sec.\n", fudge_factor);
	}
        if (!valid)
                debug("Environment is not valid for selected backend.\n");

	return 1;
}

static void unload_backend(void)
{
        if (backend_handle != NULL) {
                dlclose(backend_handle);
                backend_handle = NULL;
                memset(&backend, 0, sizeof(backend));
        }
}

static int init_yogrt(void)
{
	int rc = 1;

	if (initialized == 0) {
		char *p;
		initialized = 1;
		if ((p = getenv("YOGRT_DEBUG")) != NULL) {
			_yogrt_verbosity = (int)atol(p);
			if (p != NULL) {
				debug("In environment: YOGRT_DEBUG=%d\n",
				      _yogrt_verbosity);
			}
		}
		read_config_file();
		read_env_variables();
		rc = load_backend();
	}

	return rc;
}

static void fini_yogrt(void)
{
        if (initialized == 1) {
                unload_backend();
                initialized = 0;
        }
}

/*
 * Returns 1 if update is needed, 0 otherwise.
 */
static inline int need_update(time_t now)
{
	int rem = REMAINING(now);
	int last = now - last_update;
	int rc;

	if (last_update == -1) {
		/* first time yogrt_remaining has been called */
		rc = 1;
	} else if ((rem > interval2_start) && (last >= interval1)) {
		rc = 1;
	} else if ((rem <= interval2_start) && (last >= interval2)) {
		rc = 1;
	} else if (last_update_failed && (last >= FAILED_UPDATE_INTERVAL)) {
		/* update sooner than the normal interval
		   would require if the last update failed */
		debug("Last update failed, trying again early.\n");
		rc = 1;
	} else {
		rc = 0;
	}

	return rc;
}

int yogrt_remaining(void)
{
	time_t now;
	int rem = -1;
	int rc;

	init_yogrt();
	if (backend_handle == NULL) {
		return INT_MAX;
	}

        if (valid == 0) {
                debug2("Environment is not valid.  Returning INT_MAX.\n");
                return INT_MAX;
        }

	if (rank != 0) {
		debug("This is not task rank 0.  Returning -1.\n");
		return -1;
	}

        now = time(NULL);
	if (need_update(now)) {
		if (backend.remaining != NULL) {
			rem = backend.remaining(now, last_update,
						cached_time_rem);
                        now = time(NULL);
		}
		if (rem != -1) {
			last_update_failed = 0;
			cached_time_rem = rem;
			last_update = now;
		} else {
			debug("Update failed, will try again in"
			      " at least %d sec.\n", FAILED_UPDATE_INTERVAL);
			last_update_failed = 1;
			if (cached_time_rem != -1) {
				cached_time_rem -= (now - last_update);
				last_update = now;
			}
		}
	}

	if (cached_time_rem == -1) {
		rc = INT_MAX;
	} else {
		rem = REMAINING(now) - fudge_factor;
		if (rem >= 0) {
			rc = rem;
		} else {
			rc = 0;
		}
	}

	debug3("Reporting %sremaining time of %d sec.\n",
	       fudge_factor != 0 ? "fudged ": "",
	       rc);
	return rc;
}

/*
 * Just an alias for yogrt_remaining().  We don't advertise this function
 * but it might appear in some documentation that predates the release
 * of libyogrt.
 */
int yogrt_get_time(void)
{
	return yogrt_remaining();
}

void yogrt_set_remaining(int seconds)
{
	init_yogrt();
	if (seconds < 0)
                cached_time_rem = 0;
	else
                cached_time_rem = seconds;
        last_update = time(NULL);
	debug("remaining time changed to %d\n", cached_time_rem);
}

void yogrt_set_interval1(int seconds)
{
	init_yogrt();
	if (seconds < 0)
		interval1 = 0;
	else
		interval1 = seconds;
	debug("interval1 changed to %d\n", interval1);
}

void yogrt_set_interval2(int seconds)
{
	init_yogrt();
	if (seconds < 0)
		interval2 = 0;
	else
		interval2 = seconds;
	debug("interval2 changed to %d\n", interval2);
}

void yogrt_set_interval2_start(int seconds_before_end)
{
	init_yogrt();
	if (seconds_before_end < 0)
		interval2_start = 0;
	else
		interval2_start = seconds_before_end;
	debug("interval2_start changed to %d\n", interval2);
}

void yogrt_set_fudge_factor(int seconds)
{
	init_yogrt();
	fudge_factor = seconds;
	debug("fudge_factor changed to %d\n", fudge_factor);
}

void yogrt_set_debug(int val)
{
        _yogrt_verbosity = val;
}

int yogrt_get_interval1(void)
{
	init_yogrt();
	return interval1;
}

int yogrt_get_interval2(void)
{
	init_yogrt();
	return interval2;
}

int yogrt_get_interval2_start(void)
{
	init_yogrt();
	return interval2_start;
}

int yogrt_get_fudge_factor(void)
{
	init_yogrt();
	return fudge_factor;
}

int yogrt_get_debug(void)
{
        return _yogrt_verbosity;
}

int yogrt_init(void)
{
        int rc;

        rc = init_yogrt();

        return rc;
}

void yogrt_fini(void)
{
        fini_yogrt();
}

/**********************************************************************
 * Fortran wrappers (single underscore version)
 **********************************************************************/
int iyogrt_remaining_(void)
{
	return yogrt_remaining();
}

int iyogrt_set_remaining_(int *seconds)
{
	yogrt_set_remaining(*seconds);
	return *seconds;
}

int iyogrt_set_interval1_(int *seconds)
{
	yogrt_set_interval1(*seconds);
	return *seconds;
}

int iyogrt_set_interval2_(int *seconds)
{
	yogrt_set_interval2(*seconds);
	return *seconds;
}

int iyogrt_set_interval2_start_(int *seconds_before_end)
{
	yogrt_set_interval2_start(*seconds_before_end);
	return *seconds_before_end;
}

int iyogrt_set_fudge_factor_(int *seconds)
{
	yogrt_set_fudge_factor(*seconds);
	return *seconds;
}

int iyogrt_set_debug_(int *val)
{
	yogrt_set_debug(*val);
	return *val;
}

int iyogrt_get_interval1_(void)
{
	return yogrt_get_interval1();
}

int iyogrt_get_interval2_(void)
{
	return yogrt_get_interval2();
}

int iyogrt_get_interval2_start_(void)
{
	return yogrt_get_interval2_start();
}

int iyogrt_get_fudge_factor_(void)
{
	return yogrt_get_fudge_factor();
}

int iyogrt_get_debug_(void)
{
	return yogrt_get_debug();
}

int iyogrt_init_(void)
{
        return yogrt_init();
}

void iyogrt_fini_(void)
{
        yogrt_fini();
}

/**********************************************************************
 * Fortran wrappers (double underscore version)
 **********************************************************************/
int iyogrt_remaining__(void)
{
	return yogrt_remaining();
}

int iyogrt_set_remaining__(int *seconds)
{
	yogrt_set_remaining(*seconds);
	return *seconds;
}

int iyogrt_set_interval1__(int *seconds)
{
	yogrt_set_interval1(*seconds);
	return *seconds;
}

int iyogrt_set_interval2__(int *seconds)
{
	yogrt_set_interval2(*seconds);
	return *seconds;
}

int iyogrt_set_interval2_start__(int *seconds_before_end)
{
	yogrt_set_interval2_start(*seconds_before_end);
	return *seconds_before_end;
}

int iyogrt_set_fudge_factor__(int *seconds)
{
	yogrt_set_fudge_factor(*seconds);
	return *seconds;
}

int iyogrt_set_debug__(int *val)
{
	yogrt_set_debug(*val);
	return *val;
}

int iyogrt_get_interval1__(void)
{
	return yogrt_get_interval1();
}

int iyogrt_get_interval2__(void)
{
	return yogrt_get_interval2();
}

int iyogrt_get_interval2_start__(void)
{
	return yogrt_get_interval2_start();
}

int iyogrt_get_fudge_factor__(void)
{
	return yogrt_get_fudge_factor();
}

int iyogrt_get_debug__(void)
{
	return yogrt_get_debug();
}

int iyogrt_init__(void)
{
        return yogrt_init();
}

void iyogrt_fini__(void)
{
        yogrt_fini();
}

