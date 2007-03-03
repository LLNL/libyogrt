INCLUDES = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-lcrm.la
libyogrt_lcrm_la_SOURCES = internal.c
libyogrt_lcrm_la_LIBADD = -llrm
libyogrt_lcrm_la_CPPFLAGS = $(LCRM_CPPFLAGS)
libyogrt_lcrm_la_LDFLAGS = $(LCRM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)
