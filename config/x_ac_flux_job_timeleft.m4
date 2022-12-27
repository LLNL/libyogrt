##*****************************************************************************
#  SYNOPSIS:
#    X_AC_FLUX_JOB_TIMELEFT()
#
#  DESCRIPTION:
#    Check flux_job_timeleft() is present
##*****************************************************************************

AC_DEFUN([X_AC_FLUX_JOB_TIMELEFT], [
  AS_IF([test "$FLUX_LIBADD"],[
    AC_MSG_CHECKING([Whether flux_job_timeleft() is present.])
    LIBS="$FLUX_LIBS $FLUX_LIBADD $LIBS"
    AC_LINK_IFELSE([
      AC_LANG_PROGRAM(
        [[#include <flux/core.h>]],
        [[double tl = flux_job_timeleft(NULL, NULL, NULL);]]
      )],
      [x_ac_cv_flux_job_timeleft=1],
      [x_ac_cv_flux_job_timeleft=0]
    )
    AS_IF([test "x$x_ac_cv_flux_job_timeleft" = x1],
      [AC_MSG_RESULT([yes])],
      [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([flux_job_timeleft() is required.  Update to a newer flux or use --without-flux.])
      ]
    )
  ])

])
