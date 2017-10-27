AM_CPPFLAGS = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-lcrm.la
libyogrt_lcrm_la_SOURCES = internal.c
libyogrt_lcrm_la_LIBADD = -llrm
libyogrt_lcrm_la_CPPFLAGS = $(AM_CPPFLAGS) $(LCRM_CPPFLAGS)
libyogrt_lcrm_la_LDFLAGS = $(LCRM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)
