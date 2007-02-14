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

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

X_AC_SLURM
X_AC_LCRM

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/none/Makefile
                 src/slurm/Makefile
                 src/lcrm/Makefile])
 	
AC_OUTPUT
