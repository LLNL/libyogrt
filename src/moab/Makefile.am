INCLUDES = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-moab.la
libyogrt_moab_la_SOURCES = internal.c
libyogrt_moab_la_LIBADD = $(MOAB_LIBADD)
libyogrt_moab_la_CPPFLAGS = $(MOAB_CPPFLAGS)
libyogrt_moab_la_LDFLAGS = $(MOAB_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)
