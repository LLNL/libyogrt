INCLUDES = -I$(top_srcdir)/src

sublibdir = $(libdir)/libyogrt
sublib_LTLIBRARIES = libyogrt-slurm.la
libyogrt_slurm_la_SOURCES = internal.c
libyogrt_slurm_la_LIBADD = -lslurm
libyogrt_slurm_la_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_slurm_la_LDFLAGS = $(SLURM_LDFLAGS) \
	-no-undefined \
	-export-symbols-regex "^internal_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE) \
	-DEXTERNALPROGPATH=\"$(libexecdir)/libyogrt_slurm_time\"

# Only build the libyogrt_slurm_time program on AIX, where we need it
# because libslurm is only compiled 32-bit, and libyogrt needs to access
# it from both 32-bit and 64-bit programs.
libexec_PROGRAMS = @SLURM_AIX_PROGRAMS@
EXTRA_PROGRAMS = libyogrt_slurm_time
libyogrt_slurm_time_SOURCES = libyogrt_slurm_time.c
libyogrt_slurm_time_CPPFLAGS = $(SLURM_CPPFLAGS)
libyogrt_slurm_time_LDFLAGS = $(SLURM_LDFLAGS) -lslurm
