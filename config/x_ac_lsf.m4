##*****************************************************************************
## $Id: x_ac_lsf.m4 8192 2006-05-25 00:15:05Z morrone $
##*****************************************************************************
#  AUTHOR:
#    Adam Moody <moody20@llnl.gov>
#    (Originally Chris Dunlap <cdunlap@llnl.gov>)
#
#  SYNOPSIS:
#    X_AC_LSF()
#
#  DESCRIPTION:
#    Check the usual suspects for a LSF installation,
#    updating CPPFLAGS and LDFLAGS as necessary.
#
#  WARNINGS:
#    This macro must be placed after AC_PROG_CC and before AC_PROG_LIBTOOL.
##*****************************************************************************

AC_DEFUN([X_AC_LSF], [

  # LSF installs have an include at the top level,
  # but they bury the libs under a subdirectory as in:
  #   <path>/
  #     include/
  #       lsbatch.h
  #     linux3.10-glibc2.17-ppc64le/
  #       lib/
  #         libbat.so
  #
  # If certain LSF environment variables are set, they
  # could be used to locate include and lib dirs.
  #
  # To get path to the include file:
  #   grep LSF_INCLUDEDIR $LSF_ENVDIR/lsf.conf
  #
  # The path to the library directory:
  #   echo $LSF_LIBDIR

  # TODO: hard coded subdirectory for LSF
  lsflib_subdir="linux3.10-glibc2.17-ppc64le"

  # various libs needed to call lsb_ functions
  LSF_LIBADD="-lbat -llsf -lrt -lnsl"

  _x_ac_lsf_dirs="/usr"
  _x_ac_lsf_libs="lib64 lib"

  AC_ARG_WITH(
    [lsf],
    AS_HELP_STRING(--with-lsf=PATH,Specify path to lsf installation),
    [_x_ac_lsf_dirs="$withval"
     with_lsf=yes],
    [with_lsf=no])

  _backup_libs="$LIBS"
  if test "$with_lsf" = no; then
    # Check for LSF library in the default location.
    AC_CHECK_LIB([bat], [lsb_init], [], [], [$LSF_LIBADD])
  fi
  LIBS="$_backup_libs"

  if test "$ac_cv_lib_bat_lsb_init" != yes; then
    AC_CACHE_CHECK(
      [for LSF installation],
      [x_ac_cv_lsf_dir],
      [
        for d in $_x_ac_lsf_dirs; do
          test -d "$d" || continue
          test -d "$d/include" || continue
          test -d "$d/include/lsf" || continue
          test -f "$d/include/lsf/lsbatch.h" || continue
          for bit in $_x_ac_lsf_libs; do
            test -d "$d/$lsflib_subdir/$bit" || continue
        
            _x_ac_lsf_libs_save="$LIBS"
            LIBS="-L$d/$lsflib_subdir/$bit $LSF_LIBADD $LIBS"
            AC_LINK_IFELSE(
              [AC_LANG_PROGRAM([lsb_init(NULL);])],
              [AS_VAR_SET([x_ac_cv_lsf_dir], [$d])
               AS_VAR_SET([x_ac_cv_lsf_libdir], [$d/$lsflib_subdir/$bit])]
            )
            LIBS="$_x_ac_lsf_libs_save"
            test -n "$x_ac_cv_lsf_dir" && break
          done
          test -n "$x_ac_cv_lsf_dir" && break
        done
    ])
  fi

  if test "$with_lsf" = no \
     && test "$ac_cv_lib_bat_lsb_open" = yes; then
    LSF_CPPFLAGS=""
    LSF_LDFLAGS=""
  elif test -n "$x_ac_cv_lsf_dir"; then
    LSF_CPPFLAGS="-I$x_ac_cv_lsf_dir/include"
    LSF_LDFLAGS="-L$x_ac_cv_lsf_libdir"
  else
    if test "$with_lsf" = yes; then
      AC_MSG_ERROR([LSF is not in specified location!])
    else
      AC_MSG_WARN([unable to locate LSF installation])
    fi
  fi

  AC_SUBST(LSF_LIBADD)
  AC_SUBST(LSF_CPPFLAGS)
  AC_SUBST(LSF_LDFLAGS)

  AM_CONDITIONAL(WITH_LSF,
	test -n "$x_ac_cv_lsf_dir" || test "$ac_cv_lib_bat_lsb_init" = yes)
])
