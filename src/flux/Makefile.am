AM_CPPFLAGS = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-flux.la
libyogrt_flux_la_SOURCES = internal.c
libyogrt_flux_la_LIBADD = $(FLUX_LIBADD)
libyogrt_flux_la_CPPFLAGS = $(AM_CPPFLAGS) $(FLUX_CFLAGS)
libyogrt_flux_la_LDFLAGS = $(FLUX_LIBS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)
