INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libyogrt.la
libyogrt_la_SOURCES = internal.c
libyogrt_la_LIBADD = ../yogrt.lo @LTLIBOBJS@ -lslurm
libyogrt_la_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_la_LDFLAGS = $(SLURM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^yogrt_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)
