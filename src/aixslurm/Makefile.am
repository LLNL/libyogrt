AM_CPPFLAGS = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-aixslurm.la
if WANT_AIXSLURM_SPANK_PLUGIN
sublib_LTLIBRARIES += libyogrt-spank-plugin.la
endif

libyogrt_aixslurm_la_SOURCES = internal.c
libyogrt_aixslurm_la_LIBADD = $(SLURM_LIBADD)
libyogrt_aixslurm_la_CPPFLAGS = $(AM_CPPFLAGS) $(SLURM_CPPFLAGS)
libyogrt_aixslurm_la_LDFLAGS = $(SLURM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)

libyogrt_spank_plugin_la_SOURCES = libyogrt_spank_plugin.c
libyogrt_spank_plugin_la_CPPFLAGS = $(AM_CPPFLAGS) $(SLURM_CPPFLAGS) \
	-DEXTERNALPROGPATH=\"$(libexecdir)/libyogrt_slurm_timed\"
libyogrt_spank_plugin_la_LDFLAGS = -Wl,-brtl \
	-export-symbols-regex "^(slurm_spank_)|(plugin_).*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)

libexec_PROGRAMS =
if WANT_AIXSLURM_PROGRAM
libexec_PROGRAMS += libyogrt_slurm_timed
endif
libyogrt_slurm_timed_SOURCES = libyogrt_slurm_timed.c
libyogrt_slurm_timed_CPPFLAGS = $(AM_CPPFLAGS) $(SLURM_CPPFLAGS)
libyogrt_slurm_timed_LDFLAGS = $(SLURM_LDFLAGS) $(SLURM_LIBADD)
