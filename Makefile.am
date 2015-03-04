SUBDIRS = src examples
EXTRA_DIST = \
	include \
	lib-gccsparcOS5 \
	lib-hpux10 \
	lib-linux \
	lib-sparcOS5 \
	docs \
	RELEASENOTES \
	LICENSE \
	examples/Makefile.am \
	examples/Makefile.defs

debug:
	$(MAKE) CXXFLAGS="$(EXTRA_CXXFLAGS) $(DEBUG_CXXFLAGS)" all

opt:
	$(MAKE) CXXFLAGS="$(EXTRA_CXXFLAGS) $(OPT_CXXFLAGS)" all

