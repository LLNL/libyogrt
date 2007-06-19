#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT
X_AC_META
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/yogrt.h])
AC_CONFIG_HEADER([config.h])

AC_CANONICAL_HOST
AC_CANONICAL_SYSTEM
AC_DISABLE_STATIC

# Automake support
AM_INIT_AUTOMAKE([$PACKAGE], [$VERSION], [check-news dist-bzip2 gnu no-define])
AM_MAINTAINER_MODE

case "$host" in
	*-*-aix*) AC_DEFINE(HAVE_AIX, [], [Define if system is AIX])
                  LDFLAGS="$LDFLAGS -Wl,-brtl -Wl,-bexpfull"
                  if test "$OBJECT_MODE" == "64" ; then
                      AC_DEFINE(HAVE_AIX_64BIT, [], [Define for 64-bit build on AIX])
                      SLURM_AIX_PROGRAMS=""
                  elif test "$OBJECT_MODE" == "32" ; then
                      SLURM_AIX_PROGRAMS="libyogrt_slurm_time"
                  else
                      SLURM_AIX_PROGRAMS=""
                  fi
        ;;
        default)
                  SLURM_AIX_PROGRAMS=""
        ;;
esac
AC_SUBST([SLURM_AIX_PROGRAMS])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

X_AC_SLURM
X_AC_LCRM
X_AC_MOAB

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 doc/man/Makefile
                 src/Makefile
                 src/none/Makefile
                 src/slurm/Makefile
                 src/lcrm/Makefile
                 src/moab/Makefile])

AC_OUTPUT
