if WITH_FLUX
  FLUX_DIR = flux
endif
if WITH_SLURM
  SLURM_DIR = slurm
endif
if WITH_LCRM
  LCRM_DIR = lcrm
endif
if HAVE_AIX
  AIXSLURM_DIR = aixslurm
endif
if WITH_MOAB
  MOAB_DIR = moab
endif
if WITH_LSF
  LSF_DIR = lsf
endif

SUBDIRS = none $(FLUX_DIR)  $(SLURM_DIR) $(LCRM_DIR) $(MOAB_DIR) $(AIXSLURM_DIR) $(LSF_DIR)

lib_LTLIBRARIES = libyogrt.la
libyogrt_la_SOURCES = yogrt.c
libyogrt_la_LIBADD = -ldl
libyogrt_la_CPPFLAGS = \
	-DBACKENDDIR=\"$(libdir)/libyogrt\" \
	-DCONFIGPATH=\"$(sysconfdir)\"
libyogrt_la_LDFLAGS = \
	-no-undefined \
	-export-symbols-regex "^i?yogrt_.*" \
	-version-info $(LIBYOGRT_LT_CURRENT):$(LIBYOGRT_LT_REVISION):$(LIBYOGRT_LT_AGE)

include_HEADERS = yogrt.h

noinst_HEADERS = internal_yogrt.h
