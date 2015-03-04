MODULE = libfx.a

noinst_LIBRARIES = $(MODULE)

HFILES = \
	fx.h \
	sc_context.h \
	sc_fix.h \
	sc_fixed.h \
	sc_fxcast_switch.h \
	sc_fxdefs.h \
	sc_fxnum.h \
	sc_fxnum_observer.h \
	sc_fxtype_params.h \
	sc_fxval.h \
	sc_fxval_observer.h \
	sc_report.h \
	sc_report_handler.h \
	sc_ufix.h \
	sc_ufixed.h \
	scfx_ieee.h \
	scfx_mant.h \
	scfx_other_defs.h \
	scfx_params.h \
	scfx_pow10.h \
	scfx_rep.h \
	scfx_string.h \
	scfx_utils.h

H_FILES =

CXXFILES = \
	sc_fxcast_switch.cpp \
	sc_fxdefs.cpp \
	sc_fxnum.cpp \
	sc_fxnum_observer.cpp \
	sc_fxtype_params.cpp \
	sc_fxval.cpp \
	sc_fxval_observer.cpp \
	sc_report.cpp \
	sc_report_handler.cpp \
	scfx_mant.cpp \
	scfx_pow10.cpp \
	scfx_rep.cpp \
	scfx_utils.cpp

noinst_HEADERS = \
	$(HFILES) \
	$(H_FILES)

libfx_a_SOURCES = \
	$(HFILES) \
	$(H_FILES) \
	$(CXXFILES) 

INCLUDES = -I@top_srcdir@/include

install-data-local:
	@$(NORMAL_INSTALL)

uninstall-local:
	@$(NORMAL_UNINSTALL)

EXTRA_DIST =
