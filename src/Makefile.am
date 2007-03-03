if WITH_SLURM
  SLURM_DIR = slurm
endif
if WITH_LCRM
  LCRM_DIR = lcrm
endif
#if WITH_MOAB
#  MOAB_DIR = moab
#endif

SUBDIRS = none $(SLURM_DIR) $(LCRM_DIR) $(MOAB_DIR)

lib_LTLIBRARIES = libyogrt.la
libyogrt_la_SOURCES = yogrt.c
libyogrt_la_LIBADD = -ldl
libyogrt_la_CPPFLAGS = \
	-DBACKENDDIR=\"$(libdir)/libyogrt\" \
	-DCONFIGPATH=\"$(sysconfdir)\"
libyogrt_la_LDFLAGS = \
	-no-undefined \
	-export-symbols-regex "^i?yogrt_.*" \
	-version-info $(META_LT_CURRENT):$(META_LT_REVISION):$(META_LT_AGE)

pkginclude_HEADERS = yogrt.h
