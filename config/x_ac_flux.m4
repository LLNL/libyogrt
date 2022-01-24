##*****************************************************************************
#  SYNOPSIS:
#    X_AC_FLUX()
#
#  DESCRIPTION:
#    Check pkg-config for a FLUX installation,
#    updating CPPFLAGS and LDFLAGS as necessary.
#
#  WARNINGS:
#    This macro must be placed after AC_PROG_CC and before AC_PROG_LIBTOOL.
##*****************************************************************************

# --with-flux=no	no test for flux
# --with-flux=check	look in default location
# --with-flux		look in default location; error on fail
# --with-flux=yes	look in default location; error on fail
# --with-flux=<path>	look under <path>; error on fail

AC_DEFUN([X_AC_FLUX], [
  AC_ARG_WITH(
    [flux],
    AS_HELP_STRING(--with-flux=PATH,Specify path to flux installation),
    [],
    [with_flux=check])

  AS_IF([test x$with_flux != xno],[
    found_flux=no

    # Check for FLUX library in the default location.
    AS_IF([test x$with_flux = xyes -o x$with_flux = xcheck],[
      PKG_CHECK_MODULES([FLUX], [flux-core], [found_flux=yes], [found_flux=no])
    ])

    AS_IF([test x$with_flux != xyes -a x$with_flux != xcheck -a x$found_flux = xno ],[
      AC_CACHE_CHECK([for FLUX include directory],
                     [x_ac_cv_flux_includedir],
                     [FLUX_INCLUDEDIR="$with_flux/include/"
                      AS_IF([test -f "$FLUX_INCLUDEDIR/flux/core.h"],
                            [x_ac_cv_flux_includedir="$FLUX_INCLUDEDIR"],
                            [x_ac_cv_flux_includedir=no])
                     ])
      AC_CACHE_CHECK([for FLUX library directory],
                     [x_ac_cv_flux_libdir],
                     [x_ac_cv_flux_libdir=no
                      _x_ac_flux_libs_save=$LIBS
                      FLUX_LIBDIR="$with_flux/lib/"
                      AS_IF([test -d "$FLUX_LIBDIR"],[
                        LIBS="-L$FLUX_LIBDIR -lflux-core $LIBS"
                        CFLAGS="-I $x_ac_cv_flux_includedir"
                        AC_LINK_IFELSE(
                          [AC_LANG_PROGRAM([#include <flux/core.h>], [flux_open(NULL,0);])],
                          [x_ac_cv_flux_libdir=$FLUX_LIBDIR]
                        )
                      ])
                      LIBS="$_x_ac_flux_libs_save"
                     ])
      AS_IF([test x$x_ac_cv_flux_includedir != xno -a x$x_ac_cv_flux_libdir != xno],[
             found_flux=yes
             FLUX_CFLAGS="-I$FLUX_INCLUDEDIR"
             FLUX_LIBS="-L$FLUX_LIBDIR"
            ],[
             found_flux=no
             FLUX_CFLAGS=""
             FLUX_LIBS=""
            ])
    ])

    AS_IF([test x$found_flux != xyes],[
      AS_IF([test x$with_flux != xcheck],
        [AC_MSG_ERROR([FLUX not found!])],
        [AC_MSG_WARN([not building support for FLUX])])
    ], [
        FLUX_LIBADD="-lflux-core"
        AC_SUBST(FLUX_LIBADD)
        AC_SUBST(FLUX_CFLAGS)
        AC_SUBST(FLUX_LIBS)
        AC_DEFINE([HAVE_LIBFLUX], 1, [Define to 1 if you have the `flux-core' library (-lflux-core).])
    ])
  ])


  AM_CONDITIONAL(WITH_FLUX, test "x$found_flux" = xyes)
])
