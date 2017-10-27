AM_CPPFLAGS = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-slurm.la
libyogrt_slurm_la_SOURCES = internal.c
libyogrt_slurm_la_LIBADD = $(SLURM_LIBADD)
libyogrt_slurm_la_CPPFLAGS = $(AM_CPPFLAGS) $(SLURM_CPPFLAGS)
libyogrt_slurm_la_LDFLAGS = $(SLURM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)
