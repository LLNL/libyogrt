SUBDIRS = src doc test
EXTRA_DIST = COPYING COPYING.LESSER DISCLAIMER INSTALL libyogrt.spec README.md
dist_sysconf_DATA = yogrt.conf.example

EXTRA_DIST += $(top_srcdir)/.version
BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version
