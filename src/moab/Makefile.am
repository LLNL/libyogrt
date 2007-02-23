INCLUDES = -I$(top_srcdir)/src

moablibdir = $(libdir)/libyogrt/moab
moablib_LTLIBRARIES = libyogrt.la
libyogrt_la_SOURCES = internal.c
libyogrt_la_LIBADD = ../yogrt.lo @LTLIBOBJS@ $(MOAB_LIBADD)
libyogrt_la_CPPFLAGS = $(MOAB_CPPFLAGS)
libyogrt_la_LDFLAGS = $(MOAB_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^i?yogrt_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)

pkginclude_HEADERS = ../yogrt.h
