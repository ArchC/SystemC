# Microsoft Developer Studio Generated NMAKE File, Based on fir_rtl.dsp
!IF "$(CFG)" == ""
CFG=fir_rtl - Win32 Release
!MESSAGE No configuration specified. Defaulting to fir_rtl - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "fir_rtl - Win32 Release" && "$(CFG)" != "fir_rtl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fir_rtl.mak" CFG="fir_rtl - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fir_rtl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "fir_rtl - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "fir_rtl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\fir_rtl.exe"


CLEAN :
	-@erase "$(INTDIR)\display.obj"
	-@erase "$(INTDIR)\fir.obj"
	-@erase "$(INTDIR)\fir_data.obj"
	-@erase "$(INTDIR)\fir_fsm.obj"
	-@erase "$(INTDIR)\main_rtl.obj"
	-@erase "$(INTDIR)\stimulus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\fir_rtl.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\fir_rtl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fir_rtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\fir_rtl.pdb" /machine:I386 /out:"$(OUTDIR)\fir_rtl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\display.obj" \
	"$(INTDIR)\fir.obj" \
	"$(INTDIR)\fir_data.obj" \
	"$(INTDIR)\fir_fsm.obj" \
	"$(INTDIR)\main_rtl.obj" \
	"$(INTDIR)\stimulus.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\fir_rtl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fir_rtl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\fir_rtl.exe"


CLEAN :
	-@erase "$(INTDIR)\display.obj"
	-@erase "$(INTDIR)\fir.obj"
	-@erase "$(INTDIR)\fir_data.obj"
	-@erase "$(INTDIR)\fir_fsm.obj"
	-@erase "$(INTDIR)\main_rtl.obj"
	-@erase "$(INTDIR)\stimulus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\fir_rtl.exe"
	-@erase "$(OUTDIR)\fir_rtl.ilk"
	-@erase "$(OUTDIR)\fir_rtl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\fir_rtl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fir_rtl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\fir_rtl.pdb" /debug /machine:I386 /out:"$(OUTDIR)\fir_rtl.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\display.obj" \
	"$(INTDIR)\fir.obj" \
	"$(INTDIR)\fir_data.obj" \
	"$(INTDIR)\fir_fsm.obj" \
	"$(INTDIR)\main_rtl.obj" \
	"$(INTDIR)\stimulus.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\fir_rtl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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
!IF EXISTS("fir_rtl.dep")
!INCLUDE "fir_rtl.dep"
!ELSE 
!MESSAGE Warning: cannot find "fir_rtl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "fir_rtl - Win32 Release" || "$(CFG)" == "fir_rtl - Win32 Debug"
SOURCE=..\..\..\examples\systemc\fir\display.cpp

"$(INTDIR)\display.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fir\fir.cpp

"$(INTDIR)\fir.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fir\fir_data.cpp

"$(INTDIR)\fir_data.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fir\fir_fsm.cpp

"$(INTDIR)\fir_fsm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fir\main_rtl.cpp

"$(INTDIR)\main_rtl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fir\stimulus.cpp

"$(INTDIR)\stimulus.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

