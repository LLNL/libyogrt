bin_PROGRAMS = yogrt
yogrt_SOURCES = yogrt.c
yogrt_CFLAGS = -I $(srcdir)/src
yogrt_LDADD = ../libyogrt.la

../libyogrt.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
