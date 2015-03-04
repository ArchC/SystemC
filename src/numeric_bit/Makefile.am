MODULE = libnumeric_bit.a

noinst_LIBRARIES = $(MODULE)

HFILES = numeric_bit.h \
	sc_nbdefs.h \
	sc_nbexterns.h \
	sc_nbutils.h \
	sc_nbwrite.h \
	sc_signed.h \
	sc_unsigned.h \
	sc_int.h \
	sc_uint.h \
	sc_bigint.h \
	sc_biguint.h 

CXXFILES = 	sc_signed.cpp \
		sc_unsigned.cpp \
		sc_nbutils.cpp \
		sc_nbexterns.cpp \
		sc_nbwrite.cpp \
		sc_nbassign.cpp \
	        sc_int64_io.cpp \
		sc_int64_mask.cpp \
		sc_int32_mask.cpp \
		sc_int.cpp \
		sc_uint.cpp \
		sc_nbdefs.cpp \
		sc_bigint.cpp \
		sc_biguint.cpp


noinst_HEADERS = $(HFILES)

libnumeric_bit_a_SOURCES = $(HFILES) \
			$(CXXFILES) 

# libnumeric_bit_a_LIBADD = @srcdir@/fx/*.o

INCLUDES = -I@top_srcdir@/include

SUBDIRS = @FX_DIR@

install-data-local:
	@$(NORMAL_INSTALL)
	if test -n "$(FX_DIR)"; then \
		( cd $(FX_DIR); $(AR) ru ../$(MODULE) *.o ); fi
	$(INSTALL_DATA) $(MODULE) @top_srcdir@/lib-$(TARGET_ARCH)/$(MODULE)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	rm -f @top_srcdir@/lib-$(TARGET_ARCH)/$(MODULE)

EXTRA_DIST = sc_signed_bitref.inc sc_signed_subref.inc sc_unsigned_bitref.inc sc_unsigned_subref.inc sc_nbfriends.inc sc_nbcommon.inc




