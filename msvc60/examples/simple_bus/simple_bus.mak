# Microsoft Developer Studio Generated NMAKE File, Based on simple_bus.dsp
!IF "$(CFG)" == ""
CFG=simple_bus - Win32 Release
!MESSAGE No configuration specified. Defaulting to simple_bus - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "simple_bus - Win32 Release" && "$(CFG)" != "simple_bus - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "simple_bus.mak" CFG="simple_bus - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "simple_bus - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "simple_bus - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "simple_bus - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\simple_bus.exe"


CLEAN :
	-@erase "$(INTDIR)\simple_bus.obj"
	-@erase "$(INTDIR)\simple_bus_arbiter.obj"
	-@erase "$(INTDIR)\simple_bus_main.obj"
	-@erase "$(INTDIR)\simple_bus_master_blocking.obj"
	-@erase "$(INTDIR)\simple_bus_master_direct.obj"
	-@erase "$(INTDIR)\simple_bus_master_non_blocking.obj"
	-@erase "$(INTDIR)\simple_bus_tools.obj"
	-@erase "$(INTDIR)\simple_bus_types.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\simple_bus.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\simple_bus.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\simple_bus.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\simple_bus.pdb" /machine:I386 /out:"$(OUTDIR)\simple_bus.exe" 
LINK32_OBJS= \
	"..\..\systemc\Release\systemc.lib" \
	"$(INTDIR)\simple_bus.obj" \
	"$(INTDIR)\simple_bus_arbiter.obj" \
	"$(INTDIR)\simple_bus_main.obj" \
	"$(INTDIR)\simple_bus_master_blocking.obj" \
	"$(INTDIR)\simple_bus_master_direct.obj" \
	"$(INTDIR)\simple_bus_master_non_blocking.obj" \
	"$(INTDIR)\simple_bus_tools.obj" \
	"$(INTDIR)\simple_bus_types.obj"

"$(OUTDIR)\simple_bus.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "simple_bus - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\simple_bus.exe"


CLEAN :
	-@erase "$(INTDIR)\simple_bus.obj"
	-@erase "$(INTDIR)\simple_bus_arbiter.obj"
	-@erase "$(INTDIR)\simple_bus_main.obj"
	-@erase "$(INTDIR)\simple_bus_master_blocking.obj"
	-@erase "$(INTDIR)\simple_bus_master_direct.obj"
	-@erase "$(INTDIR)\simple_bus_master_non_blocking.obj"
	-@erase "$(INTDIR)\simple_bus_tools.obj"
	-@erase "$(INTDIR)\simple_bus_types.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\simple_bus.exe"
	-@erase "$(OUTDIR)\simple_bus.ilk"
	-@erase "$(OUTDIR)\simple_bus.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\simple_bus.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\simple_bus.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\simple_bus.pdb" /debug /machine:I386 /out:"$(OUTDIR)\simple_bus.exe" /pdbtype:sept 
LINK32_OBJS= \
	"..\..\systemc\Release\systemc.lib" \
	"$(INTDIR)\simple_bus.obj" \
	"$(INTDIR)\simple_bus_arbiter.obj" \
	"$(INTDIR)\simple_bus_main.obj" \
	"$(INTDIR)\simple_bus_master_blocking.obj" \
	"$(INTDIR)\simple_bus_master_direct.obj" \
	"$(INTDIR)\simple_bus_master_non_blocking.obj" \
	"$(INTDIR)\simple_bus_tools.obj" \
	"$(INTDIR)\simple_bus_types.obj"

"$(OUTDIR)\simple_bus.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("simple_bus.dep")
!INCLUDE "simple_bus.dep"
!ELSE 
!MESSAGE Warning: cannot find "simple_bus.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "simple_bus - Win32 Release" || "$(CFG)" == "simple_bus - Win32 Debug"
SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus.cpp

"$(INTDIR)\simple_bus.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_arbiter.cpp

"$(INTDIR)\simple_bus_arbiter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_main.cpp

"$(INTDIR)\simple_bus_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_master_blocking.cpp

"$(INTDIR)\simple_bus_master_blocking.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_master_direct.cpp

"$(INTDIR)\simple_bus_master_direct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_master_non_blocking.cpp

"$(INTDIR)\simple_bus_master_non_blocking.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_tools.cpp

"$(INTDIR)\simple_bus_tools.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\simple_bus\simple_bus_types.cpp

"$(INTDIR)\simple_bus_types.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

