INCLUDES = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-slurm.la @AIXSLURM_MODULE@
EXTRA_LTLIBRARIES = slurm-yogrt-plugin.la

libyogrt_slurm_la_SOURCES = internal.c
libyogrt_slurm_la_LIBADD = $(SLURM_LIBADD)
libyogrt_slurm_la_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_slurm_la_LDFLAGS = $(SLURM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)

slurm_yogrt_plugin_la_SOURCES = slurm_yogrt_plugin.c
slurm_yogrt_plugin_la_CPPFLAGS = $(SLURM_CPPFLAGS) \
	-DEXTERNALPROGPATH="$(libexecdir)/libyogrt_slurm_time"

libexec_PROGRAMS = @AIXSLURM_PROGRAM@
EXTRA_PROGRAMS = libyogrt_slurm_time
libyogrt_slurm_time_SOURCES = libyogrt_slurm_time.c
libyogrt_slurm_time_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_slurm_time_LDFLAGS = $(SLURM_LDFLAGS) $(SLURM_LIBADD)
