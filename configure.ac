#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([Your One Get Remaining Time library],
        [1.24],
        [https://github.com/LLNL/libyogrt/issues],
        [libyogrt],
        [https://github.com/LLNL/libyogrt])

# libyogrt library versions
LIBYOGRT_LT_CURRENT=1
LIBYOGRT_LT_REVISION=0
LIBYOGRT_LT_AGE=0

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/yogrt.h])
AC_CONFIG_HEADER([config.h])

AC_CANONICAL_HOST
AC_CANONICAL_SYSTEM
AC_DISABLE_STATIC

# Automake support
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Check for pkg-config support
PKG_PROG_PKG_CONFIG

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

AC_SUBST([LIBYOGRT_LT_CURRENT])
AC_SUBST([LIBYOGRT_LT_REVISION])
AC_SUBST([LIBYOGRT_LT_AGE])
AC_DEFINE([LIBYOGRT_LT_CURRENT], [], [libyogrt library version "current"])
AC_DEFINE([LIBYOGRT_LT_REVISION], [], [libyogrt library version "revision"])
AC_DEFINE([LIBYOGRT_LT_AGE], [], [libyogrt library version "age"])

X_AC_SLURM
X_AC_LCRM
X_AC_MOAB
X_AC_LSF
X_AC_FLUX

aix_64bit_mode=no
using_aix=no
case "$host" in
	*-*-aix*) AC_DEFINE(HAVE_AIX, [], [Define if system is AIX])
                  using_aix=yes
                  LDFLAGS="$LDFLAGS -Wl,-brtl -Wl,-bexpfull"
                  if test "$OBJECT_MODE" == "64" ; then
                      AC_DEFINE(HAVE_AIX_64BIT, [], [Define for 64-bit build on AIX])
                      aix_64bit_mode=yes
                  fi
        ;;
esac
AM_CONDITIONAL([WANT_AIXSLURM_PROGRAM],
  [test "$using_aix" = yes && test "$aix_64bit_mode" = no && test "$slurm_available" = yes])
AM_CONDITIONAL([WANT_AIXSLURM_SPANK_PLUGIN],
  [test "$using_aix" = yes && test "$aix_64bit_mode" = no && test "$slurm_available" = yes])
AM_CONDITIONAL([HAVE_AIX], [test "$using_aix" = yes])

AC_CONFIG_FILES([libyogrt.spec
                 Makefile
                 doc/Makefile
                 doc/man/Makefile
                 src/Makefile
                 src/none/Makefile
                 src/flux/Makefile
                 src/slurm/Makefile
                 src/lcrm/Makefile
                 src/moab/Makefile
                 src/lsf/Makefile
                 src/aixslurm/Makefile
                 test/Makefile])

AC_OUTPUT
