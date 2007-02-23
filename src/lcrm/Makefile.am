INCLUDES = -I$(top_srcdir)/src

lcrmlibdir = $(libdir)/libyogrt/lcrm
lcrmlib_LTLIBRARIES = libyogrt.la
libyogrt_la_SOURCES = internal.c
libyogrt_la_LIBADD = ../yogrt.lo @LTLIBOBJS@ -llrm
libyogrt_la_CPPFLAGS = $(LCRM_CPPFLAGS)
libyogrt_la_LDFLAGS = $(LCRM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^i?yogrt_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)

pkginclude_HEADERS = ../yogrt.h
