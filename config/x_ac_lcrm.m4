##*****************************************************************************
 ## $Id: x_ac_lcrm.m4 8192 2006-05-25 00:15:05Z morrone $
##*****************************************************************************
#  SYNOPSIS:
#    X_AC_LCRM()
#
#  DESCRIPTION:
#    Check the usual suspects for an LCRM installation,
#    updating CPPFLAGS and LDFLAGS as necessary.
#
#  WARNINGS:
#    This macro must be placed after AC_PROG_CC and before AC_PROG_LIBTOOL.
##*****************************************************************************

AC_DEFUN([X_AC_LCRM], [

  _x_ac_lcrm_dirs="/dpcs"
  _x_ac_lcrm_libs="lib64 lib"

  AC_ARG_WITH(
    [lcrm],
    AS_HELP_STRING(--with-lcrm=PATH,Specify path to lcrm installation),
    [_x_ac_lcrm_dirs="$withval"
     with_lcrm=yes],
    [with_lcrm=no])

  _backup_libs="$LIBS"
  if test "$with_lcrm" = no; then
    # Check for LCRM library in the default location.
    AC_CHECK_LIB([lrm], [lrmgettime])
  fi
  LIBS="$_backup_libs"

  if test "$ac_cv_lib_lrm_lrmgettime" != yes; then
    AC_CACHE_CHECK(
      [for lcrm installation],
      [x_ac_cv_lcrm_dir],
      [
        for d in $_x_ac_lcrm_dirs; do
          test -d "$d" || continue
          test -d "$d/include" || continue
          test -f "$d/include/liblrm.h" || continue
          for bit in $_x_ac_lcrm_libs; do
            test -d "$d/$bit" || continue

            _x_ac_lcrm_libs_save="$LIBS"
            LIBS="-L$d/$bit -llrm $LIBS"
            AC_LINK_IFELSE(
              [AC_LANG_PROGRAM([],[lrmgettime(NULL, NULL, NULL, NULL, NULL);])],
              [AS_VAR_SET([x_ac_cv_lcrm_dir], [$d])
               AS_VAR_SET([x_ac_cv_lcrm_libdir], [$d/$bit])]
            )
            LIBS="$_x_ac_lcrm_libs_save"
            test -n "$x_ac_cv_lcrm_dir" && break
          done
          test -n "$x_ac_cv_lcrm_dir" && break
        done
    ])
  fi

  if test "$with_lcrm" = no \
     && test "$ac_cv_lib_lrm_lrmgettime" = yes; then
    LCRM_CPPFLAGS=""
    LCRM_LDFLAGS=""
  elif test -n "$x_ac_cv_lcrm_dir"; then
    LCRM_CPPFLAGS="-I$x_ac_cv_lcrm_dir/include"
    LCRM_LDFLAGS="-L$x_ac_cv_lcrm_libdir"
  else
    if test "$with_lcrm" = yes; then
      AC_MSG_ERROR([lcrm is not in specified location!])
    else
      AC_MSG_WARN([unable to locate lcrm installation])
    fi
  fi

  AC_SUBST(LCRM_CPPFLAGS)
  AC_SUBST(LCRM_LDFLAGS)

  AM_CONDITIONAL(WITH_LCRM,
	test -n "$x_ac_cv_lcrm_dir" || test "$ac_cv_lib_lrm_lrmgettime" = yes)
])
