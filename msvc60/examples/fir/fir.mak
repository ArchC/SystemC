# Microsoft Developer Studio Generated NMAKE File, Based on fir.dsp
!IF "$(CFG)" == ""
CFG=fir - Win32 Release
!MESSAGE No configuration specified. Defaulting to fir - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "fir - Win32 Release" && "$(CFG)" != "fir - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fir.mak" CFG="fir - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fir - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "fir - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "fir - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\fir.exe"


CLEAN :
	-@erase "$(INTDIR)\display.obj"
	-@erase "$(INTDIR)\fir.obj"
	-@erase "$(INTDIR)\fir_data.obj"
	-@erase "$(INTDIR)\fir_fsm.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\stimulus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\fir.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\fir.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fir.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\fir.pdb" /machine:I386 /out:"$(OUTDIR)\fir.exe" 
LINK32_OBJS= \
	"$(INTDIR)\display.obj" \
	"$(INTDIR)\fir.obj" \
	"$(INTDIR)\fir_data.obj" \
	"$(INTDIR)\fir_fsm.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\stimulus.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\fir.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fir - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\fir.exe"


CLEAN :
	-@erase "$(INTDIR)\display.obj"
	-@erase "$(INTDIR)\fir.obj"
	-@erase "$(INTDIR)\fir_data.obj"
	-@erase "$(INTDIR)\fir_fsm.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\stimulus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\fir.exe"
	-@erase "$(OUTDIR)\fir.ilk"
	-@erase "$(OUTDIR)\fir.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\fir.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fir.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\fir.pdb" /debug /machine:I386 /out:"$(OUTDIR)\fir.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\display.obj" \
	"$(INTDIR)\fir.obj" \
	"$(INTDIR)\fir_data.obj" \
	"$(INTDIR)\fir_fsm.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\stimulus.obj" \
	"..\..\systemc\Debug\systemc.lib"

"$(OUTDIR)\fir.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("fir.dep")
!INCLUDE "fir.dep"
!ELSE 
!MESSAGE Warning: cannot find "fir.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "fir - Win32 Release" || "$(CFG)" == "fir - Win32 Debug"
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


SOURCE=..\..\..\examples\systemc\fir\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fir\stimulus.cpp

"$(INTDIR)\stimulus.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

