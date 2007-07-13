INCLUDES = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-slurm.la @AIXSLURM_MODULE@
EXTRA_LTLIBRARIES = libyogrt-spank-plugin.la

libyogrt_slurm_la_SOURCES = internal.c
libyogrt_slurm_la_LIBADD = $(SLURM_LIBADD)
libyogrt_slurm_la_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_slurm_la_LDFLAGS = $(SLURM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)

libyogrt_spank_plugin_la_SOURCES = libyogrt_spank_plugin.c
libyogrt_spank_plugin_la_CPPFLAGS = $(SLURM_CPPFLAGS) \
	-DEXTERNALPROGPATH="$(libexecdir)/libyogrt_slurm_timed"

libexec_PROGRAMS = @AIXSLURM_PROGRAM@
EXTRA_PROGRAMS = libyogrt_slurm_timed
libyogrt_slurm_timed_SOURCES = libyogrt_slurm_timed.c
libyogrt_slurm_timed_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_slurm_timed_LDFLAGS = $(SLURM_LDFLAGS) $(SLURM_LIBADD)
