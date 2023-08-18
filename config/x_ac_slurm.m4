##*****************************************************************************
## $Id: x_ac_slurm.m4 8192 2006-05-25 00:15:05Z morrone $
##*****************************************************************************
#  SYNOPSIS:
#    X_AC_SLURM()
#
#  DESCRIPTION:
#    Check the usual suspects for an slurm installation,
#    updating CPPFLAGS and LDFLAGS as necessary.
#
#  WARNINGS:
#    This macro must be placed after AC_PROG_CC and before AC_PROG_LIBTOOL.
##*****************************************************************************

AC_DEFUN([X_AC_SLURM], [

  # Check for SLURM header file in the default location.
  #AC_CHECK_HEADERS([slurm/slurm.h])

  _x_ac_slurm_dirs="/opt/freeware"
  _x_ac_slurm_libs="lib64 lib"

  AC_ARG_WITH(
    [slurm],
    AS_HELP_STRING(--with-slurm=PATH,Specify path to slurm installation),
    [_x_ac_slurm_dirs="$withval"
     with_slurm=yes],
    [with_slurm=no])

  _backup_libs="$LIBS"
  if test "$with_slurm" = no; then
    # Check for SLURM library in the default location.
    AC_CHECK_LIB([slurm], [slurm_get_rem_time])
  fi
  LIBS="$_backup_libs"

  if test "$ac_cv_lib_slurm_slurm_get_rem_time" != yes; then
    AC_CACHE_CHECK(
      [for slurm installation],
      [x_ac_cv_slurm_dir],
      [
        for d in $_x_ac_slurm_dirs; do
          test -d "$d" || continue
          test -d "$d/include" || continue
          test -d "$d/include/slurm" || continue
          test -f "$d/include/slurm/slurm.h" || continue
          for bit in $_x_ac_slurm_libs; do
            test -d "$d/$bit" || continue

            _x_ac_slurm_libs_save="$LIBS"
            LIBS="-L$d/$bit -lslurm -lpthread -lcrypto $LIBS"
            AC_LINK_IFELSE(
              [AC_LANG_PROGRAM([#include <slurm/slurm.h>],[slurm_get_rem_time(0);])],
              [AS_VAR_SET([x_ac_cv_slurm_dir], [$d])
               AS_VAR_SET([x_ac_cv_slurm_libdir], [$d/$bit])]
            )
            LIBS="$_x_ac_slurm_libs_save"
            test -n "$x_ac_cv_slurm_dir" && break
          done
          test -n "$x_ac_cv_slurm_dir" && break
        done
    ])
  fi

  if test "$with_slurm" = no \
     && test "$ac_cv_lib_slurm_slurm_get_rem_time" = yes; then
    SLURM_CPPFLAGS=""
    SLURM_LDFLAGS=""
    SLURM_LIBADD="-lslurm"
  elif test -n "$x_ac_cv_slurm_dir"; then
    SLURM_CPPFLAGS="-I$x_ac_cv_slurm_dir/include"
    SLURM_LDFLAGS="-L$x_ac_cv_slurm_libdir -lpthread -lcrypto"
    SLURM_LIBADD="-lslurm"
  else
    if test "$with_slurm" = yes; then
      AC_MSG_ERROR([slurm is not in specified location!])
    else
      AC_MSG_WARN([unable to locate slurm installation])
    fi
  fi

  AC_SUBST(SLURM_CPPFLAGS)
  AC_SUBST(SLURM_LDFLAGS)
  AC_SUBST(SLURM_LIBADD)

  if test -n "$x_ac_cv_slurm_dir" || test "$ac_cv_lib_slurm_slurm_get_rem_time" = yes; then
    AS_VAR_SET([slurm_available], [yes])
  else
    AS_VAR_SET([slurm_available], [no])
  fi

  AM_CONDITIONAL([WITH_SLURM], [test "$slurm_available" = yes])
])
