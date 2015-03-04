# Microsoft Developer Studio Generated NMAKE File, Based on systemc.dsp
!IF "$(CFG)" == ""
CFG=systemc - Win32 Release
!MESSAGE No configuration specified. Defaulting to systemc - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "systemc - Win32 Release" && "$(CFG)" != "systemc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "systemc.mak" CFG="systemc - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "systemc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "systemc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "systemc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\systemc.lib"


CLEAN :
	-@erase "$(INTDIR)\sc_attribute.obj"
	-@erase "$(INTDIR)\sc_bit.obj"
	-@erase "$(INTDIR)\sc_bit_ids.obj"
	-@erase "$(INTDIR)\sc_bv_base.obj"
	-@erase "$(INTDIR)\sc_clock.obj"
	-@erase "$(INTDIR)\sc_communication_ids.obj"
	-@erase "$(INTDIR)\sc_cor_fiber.obj"
	-@erase "$(INTDIR)\sc_cor_qt.obj"
	-@erase "$(INTDIR)\sc_event.obj"
	-@erase "$(INTDIR)\sc_event_finder.obj"
	-@erase "$(INTDIR)\sc_exception.obj"
	-@erase "$(INTDIR)\sc_fx_ids.obj"
	-@erase "$(INTDIR)\sc_fxcast_switch.obj"
	-@erase "$(INTDIR)\sc_fxdefs.obj"
	-@erase "$(INTDIR)\sc_fxnum.obj"
	-@erase "$(INTDIR)\sc_fxnum_observer.obj"
	-@erase "$(INTDIR)\sc_fxtype_params.obj"
	-@erase "$(INTDIR)\sc_fxval.obj"
	-@erase "$(INTDIR)\sc_fxval_observer.obj"
	-@erase "$(INTDIR)\sc_hash.obj"
	-@erase "$(INTDIR)\sc_int32_mask.obj"
	-@erase "$(INTDIR)\sc_int64_io.obj"
	-@erase "$(INTDIR)\sc_int64_mask.obj"
	-@erase "$(INTDIR)\sc_int_base.obj"
	-@erase "$(INTDIR)\sc_int_ids.obj"
	-@erase "$(INTDIR)\sc_interface.obj"
	-@erase "$(INTDIR)\sc_isdb_trace.obj"
	-@erase "$(INTDIR)\sc_kernel_ids.obj"
	-@erase "$(INTDIR)\sc_lambda.obj"
	-@erase "$(INTDIR)\sc_length_param.obj"
	-@erase "$(INTDIR)\sc_list.obj"
	-@erase "$(INTDIR)\sc_logic.obj"
	-@erase "$(INTDIR)\sc_lv_base.obj"
	-@erase "$(INTDIR)\sc_main.obj"
	-@erase "$(INTDIR)\sc_mempool.obj"
	-@erase "$(INTDIR)\sc_module.obj"
	-@erase "$(INTDIR)\sc_module_name.obj"
	-@erase "$(INTDIR)\sc_module_registry.obj"
	-@erase "$(INTDIR)\sc_mutex.obj"
	-@erase "$(INTDIR)\sc_name_gen.obj"
	-@erase "$(INTDIR)\sc_nbdefs.obj"
	-@erase "$(INTDIR)\sc_nbexterns.obj"
	-@erase "$(INTDIR)\sc_nbutils.obj"
	-@erase "$(INTDIR)\sc_object.obj"
	-@erase "$(INTDIR)\sc_object_manager.obj"
	-@erase "$(INTDIR)\sc_port.obj"
	-@erase "$(INTDIR)\sc_pq.obj"
	-@erase "$(INTDIR)\sc_prim_channel.obj"
	-@erase "$(INTDIR)\sc_process_int.obj"
	-@erase "$(INTDIR)\sc_report.obj"
	-@erase "$(INTDIR)\sc_report_handler.obj"
	-@erase "$(INTDIR)\sc_runnable.obj"
	-@erase "$(INTDIR)\sc_semaphore.obj"
	-@erase "$(INTDIR)\sc_sensitive.obj"
	-@erase "$(INTDIR)\sc_signal.obj"
	-@erase "$(INTDIR)\sc_signal_ports.obj"
	-@erase "$(INTDIR)\sc_signal_resolved.obj"
	-@erase "$(INTDIR)\sc_signal_resolved_ports.obj"
	-@erase "$(INTDIR)\sc_signed.obj"
	-@erase "$(INTDIR)\sc_simcontext.obj"
	-@erase "$(INTDIR)\sc_stop_here.obj"
	-@erase "$(INTDIR)\sc_string.obj"
	-@erase "$(INTDIR)\sc_time.obj"
	-@erase "$(INTDIR)\sc_trace.obj"
	-@erase "$(INTDIR)\sc_uint_base.obj"
	-@erase "$(INTDIR)\sc_unsigned.obj"
	-@erase "$(INTDIR)\sc_utils_ids.obj"
	-@erase "$(INTDIR)\sc_vcd_trace.obj"
	-@erase "$(INTDIR)\sc_vector.obj"
	-@erase "$(INTDIR)\sc_ver.obj"
	-@erase "$(INTDIR)\sc_wait.obj"
	-@erase "$(INTDIR)\sc_wait_cthread.obj"
	-@erase "$(INTDIR)\sc_wif_trace.obj"
	-@erase "$(INTDIR)\scfx_mant.obj"
	-@erase "$(INTDIR)\scfx_pow10.obj"
	-@erase "$(INTDIR)\scfx_rep.obj"
	-@erase "$(INTDIR)\scfx_utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\systemc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../src" /D "SC_INCLUDE_FX" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\systemc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\systemc.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\systemc.lib" 
LIB32_OBJS= \
	"$(INTDIR)\sc_clock.obj" \
	"$(INTDIR)\sc_communication_ids.obj" \
	"$(INTDIR)\sc_event_finder.obj" \
	"$(INTDIR)\sc_interface.obj" \
	"$(INTDIR)\sc_mutex.obj" \
	"$(INTDIR)\sc_port.obj" \
	"$(INTDIR)\sc_prim_channel.obj" \
	"$(INTDIR)\sc_semaphore.obj" \
	"$(INTDIR)\sc_signal.obj" \
	"$(INTDIR)\sc_signal_ports.obj" \
	"$(INTDIR)\sc_signal_resolved.obj" \
	"$(INTDIR)\sc_signal_resolved_ports.obj" \
	"$(INTDIR)\sc_fx_ids.obj" \
	"$(INTDIR)\sc_fxcast_switch.obj" \
	"$(INTDIR)\sc_fxdefs.obj" \
	"$(INTDIR)\sc_fxnum.obj" \
	"$(INTDIR)\sc_fxnum_observer.obj" \
	"$(INTDIR)\sc_fxtype_params.obj" \
	"$(INTDIR)\sc_fxval.obj" \
	"$(INTDIR)\sc_fxval_observer.obj" \
	"$(INTDIR)\scfx_mant.obj" \
	"$(INTDIR)\scfx_pow10.obj" \
	"$(INTDIR)\scfx_rep.obj" \
	"$(INTDIR)\scfx_utils.obj" \
	"$(INTDIR)\sc_attribute.obj" \
	"$(INTDIR)\sc_cor_fiber.obj" \
	"$(INTDIR)\sc_cor_qt.obj" \
	"$(INTDIR)\sc_event.obj" \
	"$(INTDIR)\sc_kernel_ids.obj" \
	"$(INTDIR)\sc_lambda.obj" \
	"$(INTDIR)\sc_main.obj" \
	"$(INTDIR)\sc_module.obj" \
	"$(INTDIR)\sc_module_name.obj" \
	"$(INTDIR)\sc_module_registry.obj" \
	"$(INTDIR)\sc_name_gen.obj" \
	"$(INTDIR)\sc_object.obj" \
	"$(INTDIR)\sc_object_manager.obj" \
	"$(INTDIR)\sc_process_int.obj" \
	"$(INTDIR)\sc_runnable.obj" \
	"$(INTDIR)\sc_sensitive.obj" \
	"$(INTDIR)\sc_simcontext.obj" \
	"$(INTDIR)\sc_time.obj" \
	"$(INTDIR)\sc_ver.obj" \
	"$(INTDIR)\sc_wait.obj" \
	"$(INTDIR)\sc_wait_cthread.obj" \
	"$(INTDIR)\sc_isdb_trace.obj" \
	"$(INTDIR)\sc_trace.obj" \
	"$(INTDIR)\sc_vcd_trace.obj" \
	"$(INTDIR)\sc_wif_trace.obj" \
	"$(INTDIR)\sc_exception.obj" \
	"$(INTDIR)\sc_hash.obj" \
	"$(INTDIR)\sc_list.obj" \
	"$(INTDIR)\sc_mempool.obj" \
	"$(INTDIR)\sc_pq.obj" \
	"$(INTDIR)\sc_report.obj" \
	"$(INTDIR)\sc_report_handler.obj" \
	"$(INTDIR)\sc_stop_here.obj" \
	"$(INTDIR)\sc_string.obj" \
	"$(INTDIR)\sc_utils_ids.obj" \
	"$(INTDIR)\sc_vector.obj" \
	"$(INTDIR)\sc_bit.obj" \
	"$(INTDIR)\sc_bit_ids.obj" \
	"$(INTDIR)\sc_bv_base.obj" \
	"$(INTDIR)\sc_logic.obj" \
	"$(INTDIR)\sc_lv_base.obj" \
	"$(INTDIR)\sc_int_base.obj" \
	"$(INTDIR)\sc_int_ids.obj" \
	"$(INTDIR)\sc_int32_mask.obj" \
	"$(INTDIR)\sc_int64_io.obj" \
	"$(INTDIR)\sc_int64_mask.obj" \
	"$(INTDIR)\sc_length_param.obj" \
	"$(INTDIR)\sc_nbdefs.obj" \
	"$(INTDIR)\sc_nbexterns.obj" \
	"$(INTDIR)\sc_nbutils.obj" \
	"$(INTDIR)\sc_signed.obj" \
	"$(INTDIR)\sc_uint_base.obj" \
	"$(INTDIR)\sc_unsigned.obj"

"$(OUTDIR)\systemc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "systemc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\systemc.lib"


CLEAN :
	-@erase "$(INTDIR)\sc_attribute.obj"
	-@erase "$(INTDIR)\sc_bit.obj"
	-@erase "$(INTDIR)\sc_bit_ids.obj"
	-@erase "$(INTDIR)\sc_bv_base.obj"
	-@erase "$(INTDIR)\sc_clock.obj"
	-@erase "$(INTDIR)\sc_communication_ids.obj"
	-@erase "$(INTDIR)\sc_cor_fiber.obj"
	-@erase "$(INTDIR)\sc_cor_qt.obj"
	-@erase "$(INTDIR)\sc_event.obj"
	-@erase "$(INTDIR)\sc_event_finder.obj"
	-@erase "$(INTDIR)\sc_exception.obj"
	-@erase "$(INTDIR)\sc_fx_ids.obj"
	-@erase "$(INTDIR)\sc_fxcast_switch.obj"
	-@erase "$(INTDIR)\sc_fxdefs.obj"
	-@erase "$(INTDIR)\sc_fxnum.obj"
	-@erase "$(INTDIR)\sc_fxnum_observer.obj"
	-@erase "$(INTDIR)\sc_fxtype_params.obj"
	-@erase "$(INTDIR)\sc_fxval.obj"
	-@erase "$(INTDIR)\sc_fxval_observer.obj"
	-@erase "$(INTDIR)\sc_hash.obj"
	-@erase "$(INTDIR)\sc_int32_mask.obj"
	-@erase "$(INTDIR)\sc_int64_io.obj"
	-@erase "$(INTDIR)\sc_int64_mask.obj"
	-@erase "$(INTDIR)\sc_int_base.obj"
	-@erase "$(INTDIR)\sc_int_ids.obj"
	-@erase "$(INTDIR)\sc_interface.obj"
	-@erase "$(INTDIR)\sc_isdb_trace.obj"
	-@erase "$(INTDIR)\sc_kernel_ids.obj"
	-@erase "$(INTDIR)\sc_lambda.obj"
	-@erase "$(INTDIR)\sc_length_param.obj"
	-@erase "$(INTDIR)\sc_list.obj"
	-@erase "$(INTDIR)\sc_logic.obj"
	-@erase "$(INTDIR)\sc_lv_base.obj"
	-@erase "$(INTDIR)\sc_main.obj"
	-@erase "$(INTDIR)\sc_mempool.obj"
	-@erase "$(INTDIR)\sc_module.obj"
	-@erase "$(INTDIR)\sc_module_name.obj"
	-@erase "$(INTDIR)\sc_module_registry.obj"
	-@erase "$(INTDIR)\sc_mutex.obj"
	-@erase "$(INTDIR)\sc_name_gen.obj"
	-@erase "$(INTDIR)\sc_nbdefs.obj"
	-@erase "$(INTDIR)\sc_nbexterns.obj"
	-@erase "$(INTDIR)\sc_nbutils.obj"
	-@erase "$(INTDIR)\sc_object.obj"
	-@erase "$(INTDIR)\sc_object_manager.obj"
	-@erase "$(INTDIR)\sc_port.obj"
	-@erase "$(INTDIR)\sc_pq.obj"
	-@erase "$(INTDIR)\sc_prim_channel.obj"
	-@erase "$(INTDIR)\sc_process_int.obj"
	-@erase "$(INTDIR)\sc_report.obj"
	-@erase "$(INTDIR)\sc_report_handler.obj"
	-@erase "$(INTDIR)\sc_runnable.obj"
	-@erase "$(INTDIR)\sc_semaphore.obj"
	-@erase "$(INTDIR)\sc_sensitive.obj"
	-@erase "$(INTDIR)\sc_signal.obj"
	-@erase "$(INTDIR)\sc_signal_ports.obj"
	-@erase "$(INTDIR)\sc_signal_resolved.obj"
	-@erase "$(INTDIR)\sc_signal_resolved_ports.obj"
	-@erase "$(INTDIR)\sc_signed.obj"
	-@erase "$(INTDIR)\sc_simcontext.obj"
	-@erase "$(INTDIR)\sc_stop_here.obj"
	-@erase "$(INTDIR)\sc_string.obj"
	-@erase "$(INTDIR)\sc_time.obj"
	-@erase "$(INTDIR)\sc_trace.obj"
	-@erase "$(INTDIR)\sc_uint_base.obj"
	-@erase "$(INTDIR)\sc_unsigned.obj"
	-@erase "$(INTDIR)\sc_utils_ids.obj"
	-@erase "$(INTDIR)\sc_vcd_trace.obj"
	-@erase "$(INTDIR)\sc_vector.obj"
	-@erase "$(INTDIR)\sc_ver.obj"
	-@erase "$(INTDIR)\sc_wait.obj"
	-@erase "$(INTDIR)\sc_wait_cthread.obj"
	-@erase "$(INTDIR)\sc_wif_trace.obj"
	-@erase "$(INTDIR)\scfx_mant.obj"
	-@erase "$(INTDIR)\scfx_pow10.obj"
	-@erase "$(INTDIR)\scfx_rep.obj"
	-@erase "$(INTDIR)\scfx_utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\systemc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../src" /D "SC_INCLUDE_FX" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\systemc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\systemc.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\systemc.lib" 
LIB32_OBJS= \
	"$(INTDIR)\sc_clock.obj" \
	"$(INTDIR)\sc_communication_ids.obj" \
	"$(INTDIR)\sc_event_finder.obj" \
	"$(INTDIR)\sc_interface.obj" \
	"$(INTDIR)\sc_mutex.obj" \
	"$(INTDIR)\sc_port.obj" \
	"$(INTDIR)\sc_prim_channel.obj" \
	"$(INTDIR)\sc_semaphore.obj" \
	"$(INTDIR)\sc_signal.obj" \
	"$(INTDIR)\sc_signal_ports.obj" \
	"$(INTDIR)\sc_signal_resolved.obj" \
	"$(INTDIR)\sc_signal_resolved_ports.obj" \
	"$(INTDIR)\sc_fx_ids.obj" \
	"$(INTDIR)\sc_fxcast_switch.obj" \
	"$(INTDIR)\sc_fxdefs.obj" \
	"$(INTDIR)\sc_fxnum.obj" \
	"$(INTDIR)\sc_fxnum_observer.obj" \
	"$(INTDIR)\sc_fxtype_params.obj" \
	"$(INTDIR)\sc_fxval.obj" \
	"$(INTDIR)\sc_fxval_observer.obj" \
	"$(INTDIR)\scfx_mant.obj" \
	"$(INTDIR)\scfx_pow10.obj" \
	"$(INTDIR)\scfx_rep.obj" \
	"$(INTDIR)\scfx_utils.obj" \
	"$(INTDIR)\sc_attribute.obj" \
	"$(INTDIR)\sc_cor_fiber.obj" \
	"$(INTDIR)\sc_cor_qt.obj" \
	"$(INTDIR)\sc_event.obj" \
	"$(INTDIR)\sc_kernel_ids.obj" \
	"$(INTDIR)\sc_lambda.obj" \
	"$(INTDIR)\sc_main.obj" \
	"$(INTDIR)\sc_module.obj" \
	"$(INTDIR)\sc_module_name.obj" \
	"$(INTDIR)\sc_module_registry.obj" \
	"$(INTDIR)\sc_name_gen.obj" \
	"$(INTDIR)\sc_object.obj" \
	"$(INTDIR)\sc_object_manager.obj" \
	"$(INTDIR)\sc_process_int.obj" \
	"$(INTDIR)\sc_runnable.obj" \
	"$(INTDIR)\sc_sensitive.obj" \
	"$(INTDIR)\sc_simcontext.obj" \
	"$(INTDIR)\sc_time.obj" \
	"$(INTDIR)\sc_ver.obj" \
	"$(INTDIR)\sc_wait.obj" \
	"$(INTDIR)\sc_wait_cthread.obj" \
	"$(INTDIR)\sc_isdb_trace.obj" \
	"$(INTDIR)\sc_trace.obj" \
	"$(INTDIR)\sc_vcd_trace.obj" \
	"$(INTDIR)\sc_wif_trace.obj" \
	"$(INTDIR)\sc_exception.obj" \
	"$(INTDIR)\sc_hash.obj" \
	"$(INTDIR)\sc_list.obj" \
	"$(INTDIR)\sc_mempool.obj" \
	"$(INTDIR)\sc_pq.obj" \
	"$(INTDIR)\sc_report.obj" \
	"$(INTDIR)\sc_report_handler.obj" \
	"$(INTDIR)\sc_stop_here.obj" \
	"$(INTDIR)\sc_string.obj" \
	"$(INTDIR)\sc_utils_ids.obj" \
	"$(INTDIR)\sc_vector.obj" \
	"$(INTDIR)\sc_bit.obj" \
	"$(INTDIR)\sc_bit_ids.obj" \
	"$(INTDIR)\sc_bv_base.obj" \
	"$(INTDIR)\sc_logic.obj" \
	"$(INTDIR)\sc_lv_base.obj" \
	"$(INTDIR)\sc_int_base.obj" \
	"$(INTDIR)\sc_int_ids.obj" \
	"$(INTDIR)\sc_int32_mask.obj" \
	"$(INTDIR)\sc_int64_io.obj" \
	"$(INTDIR)\sc_int64_mask.obj" \
	"$(INTDIR)\sc_length_param.obj" \
	"$(INTDIR)\sc_nbdefs.obj" \
	"$(INTDIR)\sc_nbexterns.obj" \
	"$(INTDIR)\sc_nbutils.obj" \
	"$(INTDIR)\sc_signed.obj" \
	"$(INTDIR)\sc_uint_base.obj" \
	"$(INTDIR)\sc_unsigned.obj"

"$(OUTDIR)\systemc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("systemc.dep")
!INCLUDE "systemc.dep"
!ELSE 
!MESSAGE Warning: cannot find "systemc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "systemc - Win32 Release" || "$(CFG)" == "systemc - Win32 Debug"
SOURCE=..\..\src\systemc\communication\sc_clock.cpp

"$(INTDIR)\sc_clock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_communication_ids.cpp

"$(INTDIR)\sc_communication_ids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_event_finder.cpp

"$(INTDIR)\sc_event_finder.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_interface.cpp

"$(INTDIR)\sc_interface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_mutex.cpp

"$(INTDIR)\sc_mutex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_port.cpp

"$(INTDIR)\sc_port.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_prim_channel.cpp

"$(INTDIR)\sc_prim_channel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_semaphore.cpp

"$(INTDIR)\sc_semaphore.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_signal.cpp

"$(INTDIR)\sc_signal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_signal_ports.cpp

"$(INTDIR)\sc_signal_ports.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_signal_resolved.cpp

"$(INTDIR)\sc_signal_resolved.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\communication\sc_signal_resolved_ports.cpp

"$(INTDIR)\sc_signal_resolved_ports.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\bit\sc_bit.cpp

"$(INTDIR)\sc_bit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\bit\sc_bit_ids.cpp

"$(INTDIR)\sc_bit_ids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\bit\sc_bv_base.cpp

"$(INTDIR)\sc_bv_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\bit\sc_logic.cpp

"$(INTDIR)\sc_logic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\bit\sc_lv_base.cpp

"$(INTDIR)\sc_lv_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fx_ids.cpp

"$(INTDIR)\sc_fx_ids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxcast_switch.cpp

"$(INTDIR)\sc_fxcast_switch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxdefs.cpp

"$(INTDIR)\sc_fxdefs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxnum.cpp

"$(INTDIR)\sc_fxnum.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxnum_observer.cpp

"$(INTDIR)\sc_fxnum_observer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxtype_params.cpp

"$(INTDIR)\sc_fxtype_params.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxval.cpp

"$(INTDIR)\sc_fxval.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\sc_fxval_observer.cpp

"$(INTDIR)\sc_fxval_observer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\scfx_mant.cpp

"$(INTDIR)\scfx_mant.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\scfx_pow10.cpp

"$(INTDIR)\scfx_pow10.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\scfx_rep.cpp

"$(INTDIR)\scfx_rep.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\fx\scfx_utils.cpp

"$(INTDIR)\scfx_utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_int32_mask.cpp

"$(INTDIR)\sc_int32_mask.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_int64_io.cpp

"$(INTDIR)\sc_int64_io.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_int64_mask.cpp

"$(INTDIR)\sc_int64_mask.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_int_base.cpp

"$(INTDIR)\sc_int_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_int_ids.cpp

"$(INTDIR)\sc_int_ids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_length_param.cpp

"$(INTDIR)\sc_length_param.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_nbdefs.cpp

"$(INTDIR)\sc_nbdefs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_nbexterns.cpp

"$(INTDIR)\sc_nbexterns.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_nbutils.cpp

"$(INTDIR)\sc_nbutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_signed.cpp

"$(INTDIR)\sc_signed.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_uint_base.cpp

"$(INTDIR)\sc_uint_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\datatypes\int\sc_unsigned.cpp

"$(INTDIR)\sc_unsigned.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_attribute.cpp

"$(INTDIR)\sc_attribute.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_cor_fiber.cpp

"$(INTDIR)\sc_cor_fiber.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_cor_qt.cpp

"$(INTDIR)\sc_cor_qt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_event.cpp

"$(INTDIR)\sc_event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_kernel_ids.cpp

"$(INTDIR)\sc_kernel_ids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_lambda.cpp

"$(INTDIR)\sc_lambda.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_main.cpp

"$(INTDIR)\sc_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_module.cpp

"$(INTDIR)\sc_module.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_module_name.cpp

"$(INTDIR)\sc_module_name.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_module_registry.cpp

"$(INTDIR)\sc_module_registry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_name_gen.cpp

"$(INTDIR)\sc_name_gen.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_object.cpp

"$(INTDIR)\sc_object.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_object_manager.cpp

"$(INTDIR)\sc_object_manager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_process_int.cpp

"$(INTDIR)\sc_process_int.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_runnable.cpp

"$(INTDIR)\sc_runnable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_sensitive.cpp

"$(INTDIR)\sc_sensitive.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_simcontext.cpp

"$(INTDIR)\sc_simcontext.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_time.cpp

"$(INTDIR)\sc_time.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_ver.cpp

"$(INTDIR)\sc_ver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_wait.cpp

"$(INTDIR)\sc_wait.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\kernel\sc_wait_cthread.cpp

"$(INTDIR)\sc_wait_cthread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\tracing\sc_isdb_trace.cpp

"$(INTDIR)\sc_isdb_trace.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\tracing\sc_trace.cpp

"$(INTDIR)\sc_trace.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\tracing\sc_vcd_trace.cpp

"$(INTDIR)\sc_vcd_trace.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\tracing\sc_wif_trace.cpp

"$(INTDIR)\sc_wif_trace.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_exception.cpp

"$(INTDIR)\sc_exception.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_hash.cpp

"$(INTDIR)\sc_hash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_list.cpp

"$(INTDIR)\sc_list.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_mempool.cpp

"$(INTDIR)\sc_mempool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_pq.cpp

"$(INTDIR)\sc_pq.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_report.cpp

"$(INTDIR)\sc_report.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_report_handler.cpp

"$(INTDIR)\sc_report_handler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_stop_here.cpp

"$(INTDIR)\sc_stop_here.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_string.cpp

"$(INTDIR)\sc_string.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_utils_ids.cpp

"$(INTDIR)\sc_utils_ids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\systemc\utils\sc_vector.cpp

"$(INTDIR)\sc_vector.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

