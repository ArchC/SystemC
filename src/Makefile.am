MODULE = libsystemc.a

noinst_LIBRARIES = $(MODULE) 

HFILES = sc_bool_vector.h \
	sc_cmnhdr.h \
	sc_isdb_trace.h \
	sc_old.h \
	sc_2d.h \
	sc_aproc.h \
	sc_array.h \
	sc_array_concat.h \
	sc_async.h \
	sc_channel.h \
	sc_channel_array.h \
	sc_clock.h \
	sc_clock_int.h \
	sc_context_switch.h \
	sc_dump.h \
	sc_except.h \
	sc_hash.h \
	sc_lambda.h \
	sc_list.h \
	sc_logic.h \
	sc_logic_vector.h \
	sc_macros.h \
	sc_measure.h \
	sc_mempool.h \
	sc_module.h \
	sc_module_int.h \
	sc_object.h \
	sc_object_manager.h \
	sc_pq.h \
	sc_reslv.h \
	sc_reslv_vector.h \
	sc_resolved.h \
	sc_resolved_array.h \
	sc_signal.h \
	sc_signal_optimize.h \
	sc_signal_array.h \
	sc_signal_delay.h \
	sc_signal_deval.h \
	sc_simcontext.h \
	sc_stl.h \
	sc_string.h \
	sc_sync.h \
	sc_templ.h \
	sc_vector.h \
	sc_ver.h \
	sc_write_macros.h \
	systemc.h \
	sc_externs.h \
	sc_trace.h \
	sc_vcd_trace.h \
	sc_wif_trace.h \
	sc_sensitive.h \
	sc_helpers.h \
	sc_bit.h \
	sc_bvrep.h \
	sc_macros_int.h \
	sc_port.h \
	sc_port_sc_bit.h \
	sc_polarity.h \
	sc_port_clk.h \
	sc_module_name.h \
	sc_simcontext_int.h \
	sc_process_int.h \
	sc_port_manager.h \
	sc_bv.h \
	sc_lv.h \
	sc_module_prm.h \
	sc_signal_rv.h \
	sc_constants.h \
	sc_process.h \
	sc_ptr.h \
	sc_signal_sc_bit.h \
	sc_wait.h \
	sc_exception.h \
	sc_proxy.h \
	sc_bit_proxies.h \
	sc_iostream.h

H_FILES = sc_bool_vector_defs.h_ \
	sc_2d_defs.h_ \
	sc_array_defs.h_ \
	sc_channel_array_defs.h_ \
	sc_channel_defs.h_ \
	sc_lambda_defs.h_ \
	sc_lambda_exps.h_ \
	sc_lambda_friends.h_ \
	sc_logic_macros.h_ \
	sc_logic_vector_defs.h_ \
	sc_resolved_array_defs.h_ \
	sc_resolved_defs.h_ \
	sc_signal_array_defs.h_ \
	sc_signal_defs.h_ \
	sc_signal_delay_defs.h_ \
	sc_signal_spec.h_	\
	sc_port_spec.h_ \
	sc_port_defs.h_

M4FILES =  


CXXFILES = sc_array.cpp \
	sc_array_concat.cpp \
	sc_clock.cpp \
	sc_simcontext.cpp \
	sc_context_switch.cpp \
	sc_signal.cpp \
	sc_signal_optimize.cpp \
	sc_signal_array.cpp \
	sc_signal_edgy.cpp \
	sc_lambda.cpp \
	sc_module.cpp \
	sc_sync_process.cpp \
	sc_async_process.cpp \
	sc_aproc_process.cpp \
	sc_sync_opt.cpp \
	sc_async.cpp \
	sc_aproc.cpp \
	sc_logic.cpp \
	sc_logic_vector.cpp \
	sc_bool_vector.cpp \
	sc_string.cpp \
	sc_list.cpp \
	sc_hash.cpp \
	sc_pq.cpp \
	sc_vector.cpp \
	sc_dump.cpp \
	sc_object.cpp \
	sc_object_manager.cpp \
	sc_measure.cpp \
	sc_mempool.cpp \
	sc_trace.cpp \
	sc_vcd_trace.cpp \
	sc_wif_trace.cpp \
	sc_isdb_trace.cpp \
	sc_channel_array.cpp \
	sc_ver.cpp \
	sc_sync_noopt.cpp \
	sc_main.cpp \
	sc_reslv.cpp \
	sc_reslv_vector.cpp \
	sc_sensitive.cpp \
	sc_wait.cpp \
	sc_port.cpp \
	sc_port_manager.cpp \
	sc_module_name.cpp \
	sc_bv.cpp \
	sc_lv.cpp \
	sc_module_prm.cpp \
	sc_signal_rv.cpp \
	sc_bit.cpp \
	sc_exception.cpp


noinst_HEADERS = $(HFILES) \
		  $(H_FILES)  

libsystemc_a_SOURCES = $(HFILES) \
		       $(H_FILES) \
		       $(CXXFILES) \
		       $(M4FILES)

SUBDIRS = numeric_bit

INCLUDES = -I@top_srcdir@/include

install-data-local: 
	@$(NORMAL_INSTALL)
	$(INSTALL_DATA) $(MODULE) @top_srcdir@/lib-$(TARGET_ARCH)/$(MODULE)


uninstall-local:
	@$(NORMAL_UNINSTALL)
	rm -f @top_srcdir@/lib-$(TARGET_ARCH)/$(MODULE)

EXTRA_DIST = sc_bool_vector_and.inc sc_bool_vector_or.inc sc_bool_vector_xor.inc sc_logic_vector_and.inc sc_logic_vector_or.inc sc_logic_vector_xor.inc

