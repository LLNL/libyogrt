INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libyogrt-moab.la
libyogrt_moab_la_SOURCES = internal.c
libyogrt_moab_la_LIBADD = ../yogrt.lo @LTLIBOBJS@ $(MOAB_LIBADD)
libyogrt_moab_la_CPPFLAGS = $(MOAB_CPPFLAGS)
libyogrt_moab_la_LDFLAGS = $(MOAB_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^yogrt_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)

pkginclude_HEADERS = ../yogrt.h
