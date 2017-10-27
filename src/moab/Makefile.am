AM_CPPFLAGS = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-moab.la
libyogrt_moab_la_SOURCES = internal.c
libyogrt_moab_la_LIBADD = $(MOAB_LIBADD)
libyogrt_moab_la_CPPFLAGS = $(AM_CPPFLAGS) $(MOAB_CPPFLAGS)
libyogrt_moab_la_LDFLAGS = $(MOAB_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)
