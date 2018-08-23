##*****************************************************************************
## $Id: x_ac_lsf.m4 8192 2006-05-25 00:15:05Z morrone $
##*****************************************************************************
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

  # IBM likes to install LSF in a non-standard paths.
  #
  # An LSF install might look something like this:
  #
  #   /opt/ibm/spectrumcomputing/lsf/
  #     conf/
  #       lsf.conf
  #       profile.lsf
  #     10.1/
  #       include/
  #         lsbatch.h
  #       linux3.10-glibc2.17-ppc64le/lib/
  #         libbat.so
  #
  # IBM suggests that the "profile.lsf" file be sourced in all users'
  # login shell initialization files.  This will result in
  # users having (as of Aug 22, 2018) the following environment variables:
  #
  #   LSF_LIBDIR
  #   LSF_BINDIR
  #   LSF_ENVDIR (actually the path to the "conf" directory)
  #
  # Noteably missing is a way to easily find LSF's include directory.
  # Even though the system does not currently set LSF_INCLUDEDIR,
  # we make LSF_INCLUDEDIR an AC_ARG_VAR anyway so that the
  # user has a simple way to specify that path.  When LSF_INCLUDEDIR
  # is not set, we will employ LSF_ENVDIR to find the lsf.conf file
  # and attempt to parse the include path out of that.
  AC_ARG_VAR([LSF_LIBDIR], [Directory containing LSF libraries])
  AC_ARG_VAR([LSF_INCLUDEDIR], [Directory containing LSF header files])
  AC_ARG_VAR([LSF_ENVDIR], [Directory containing LSF configuration files])

  # various libs needed to call lsb_ functions
  LSF_LIBADD="-lbat -llsf -lrt -lnsl"

  AC_ARG_WITH(
    [lsf],
    AS_HELP_STRING(--with-lsf=PATH,Specify path to lsf installation),
    [],
    [with_lsf=check])

  AS_IF([test "x$with_lsf" != xno],[
    # Check for LSF library in the default location.
    _backup_libs="$LIBS"
    AC_CHECK_LIB([bat], [lsb_init], [found_lsf=yes], [found_lsf=no], [$LSF_LIBADD])
    LIBS="$_backup_libs"
  ])

  AS_IF([test "$ac_cv_lib_bat_lsb_init" != yes],[
    AC_CACHE_CHECK(
      [for LSF installation],
      [x_ac_cv_lsf_libdir],
      [
        # If LSF_INCLUDEDIR is not set, we'll need get it from lsf.conf instead
        AS_IF([test -z "$LSF_INCLUDEDIR"],
              [LSF_INCLUDEDIR=`grep LSF_INCLUDEDIR= $LSF_ENVDIR/lsf.conf 2>/dev/null| cut -d= -f2 `])
        AS_IF([test -f "$LSF_INCLUDEDIR/lsf/lsbatch.h" -a -d "$LSF_LIBDIR"],[
          _x_ac_lsf_libs_save="$LIBS"
          LIBS="-L$LSF_LIBDIR $LSF_LIBADD $LIBS"
          AC_LINK_IFELSE(
            [AC_LANG_PROGRAM([lsb_init(NULL);])],
            [AS_VAR_SET([x_ac_cv_lsf_includedir], [$LSF_INCLUDEDIR])
             AS_VAR_SET([x_ac_cv_lsf_libdir], [$LSF_LIBDIR])
             LSF_CPPFLAGS="-I$LSF_INCLUDEDIR"
             LSF_LDFLAGS="-L$LSF_LIBDIR"
             found_lsf=yes],
            [found_lsf=no]
          )
          LIBS="$_x_ac_lsf_libs_save"
        ])
      ]
    )
  ])

  AS_IF([test "x$found_lsf" != xyes -a "$with_lsf" != xno],[
    AS_IF([test "x$with_lsf" = xyes],
      [AC_MSG_ERROR([LSF is not in specified location!])],
      [AC_MSG_WARN([unable to locate LSF installation])])
  ])

  AC_SUBST(LSF_LIBADD)
  AC_SUBST(LSF_CPPFLAGS)
  AC_SUBST(LSF_LDFLAGS)

  AM_CONDITIONAL(WITH_LSF, test "x$found_lsf" = xyes)
])
