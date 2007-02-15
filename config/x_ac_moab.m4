##*****************************************************************************
## $Id: x_ac_moab.m4 8192 2006-05-25 00:15:05Z morrone $
##*****************************************************************************
#  AUTHOR:
#    Christopher Morrone <morrone2@llnl.gov>
#    (Originally Chris Dunlap <cdunlap@llnl.gov>)
#
#  SYNOPSIS:
#    X_AC_MOAB()
#
#  DESCRIPTION:
#    Check the usual suspects for a MOAB installation,
#    updating CPPFLAGS and LDFLAGS as necessary.
#
#  WARNINGS:
#    This macro must be placed after AC_PROG_CC and before AC_PROG_LIBTOOL.
##*****************************************************************************

AC_DEFUN([X_AC_MOAB], [

  MOAB_LIBADD="-lcmoab -lmoab -lminit -lpthread -lm -lmcom"
  _x_ac_moab_dirs="/usr /dpcs"
  _x_ac_moab_libs="lib64 lib"

  AC_ARG_WITH(
    [moab],
    AS_HELP_STRING(--with-moab=PATH,Specify path to moab installation),
    [_x_ac_moab_dirs="$withval"
     with_moab=yes],
    [with_moab=no])

  if test "$with_moab" = no; then
    # Check for MOAB library in the default location.
    AC_CHECK_LIB([cmoab], [MCCJobGetRemainingTime])
  fi

  if test "$ac_cv_lib_cmoab_MCCJobGetRemainingTime" != yes; then
    AC_CACHE_CHECK(
      [for moab installation],
      [x_ac_cv_moab_dir],
      [
        for d in $_x_ac_moab_dirs; do
          test -d "$d" || continue
          test -d "$d/include" || continue
          test -d "$d/include/moab" || continue
          test -f "$d/include/moab/mapi.h" || continue
          for bit in $_x_ac_moab_libs; do
            test -d "$d/$bit" || continue
        
            _x_ac_moab_libs_save="$LIBS"
            LIBS="-L$d/$bit $MOAB_LIBADD $LIBS"
            AC_LINK_IFELSE(
              AC_LANG_CALL([], [MCCJobGetRemainingTime]),
              AS_VAR_SET([x_ac_cv_moab_dir], [$d]))
            LIBS="$_x_ac_moab_libs_save"
            test -n "$x_ac_cv_moab_dir" && break
          done
          test -n "$x_ac_cv_moab_dir" && break
        done
    ])
  fi

  if test "$with_moab" = no \
     && test "$ac_cv_lib_cmoab_MCCJobGetRemainingTime" = yes; then
    MOAB_CPPFLAGS=""
    MOAB_LDFLAGS=""
  elif test -n "$x_ac_cv_moab_dir"; then
    MOAB_CPPFLAGS="-I$x_ac_cv_moab_dir/include"
    MOAB_LDFLAGS="-L$x_ac_cv_moab_dir/$bit"
  else
    if test "$with_moab" = yes; then
      AC_MSG_ERROR([moab is not in specified location!])
    else
      AC_MSG_WARN([unable to locate moab installation])
    fi
  fi

  AC_SUBST(MOAB_LIBADD)
  AC_SUBST(MOAB_CPPFLAGS)
  AC_SUBST(MOAB_LDFLAGS)

  AM_CONDITIONAL(WITH_MOAB,
	test -n "$x_ac_cv_moab_dir" || test "$ac_cv_lib_cmoab_MCCJobGetRemainingTime" = yes)
])
