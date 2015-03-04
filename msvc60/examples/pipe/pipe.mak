# Microsoft Developer Studio Generated NMAKE File, Based on pipe.dsp
!IF "$(CFG)" == ""
CFG=pipe - Win32 Release
!MESSAGE No configuration specified. Defaulting to pipe - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "pipe - Win32 Release" && "$(CFG)" != "pipe - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pipe.mak" CFG="pipe - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pipe - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pipe - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "pipe - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\pipe.exe"


CLEAN :
	-@erase "$(INTDIR)\display.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\numgen.obj"
	-@erase "$(INTDIR)\stage1.obj"
	-@erase "$(INTDIR)\stage2.obj"
	-@erase "$(INTDIR)\stage3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\pipe.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\pipe.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pipe.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\pipe.pdb" /machine:I386 /out:"$(OUTDIR)\pipe.exe" 
LINK32_OBJS= \
	"$(INTDIR)\display.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\numgen.obj" \
	"$(INTDIR)\stage1.obj" \
	"$(INTDIR)\stage2.obj" \
	"$(INTDIR)\stage3.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\pipe.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pipe - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\pipe.exe"


CLEAN :
	-@erase "$(INTDIR)\display.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\numgen.obj"
	-@erase "$(INTDIR)\stage1.obj"
	-@erase "$(INTDIR)\stage2.obj"
	-@erase "$(INTDIR)\stage3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\pipe.exe"
	-@erase "$(OUTDIR)\pipe.ilk"
	-@erase "$(OUTDIR)\pipe.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\pipe.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pipe.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\pipe.pdb" /debug /machine:I386 /out:"$(OUTDIR)\pipe.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\display.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\numgen.obj" \
	"$(INTDIR)\stage1.obj" \
	"$(INTDIR)\stage2.obj" \
	"$(INTDIR)\stage3.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\pipe.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("pipe.dep")
!INCLUDE "pipe.dep"
!ELSE 
!MESSAGE Warning: cannot find "pipe.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "pipe - Win32 Release" || "$(CFG)" == "pipe - Win32 Debug"
SOURCE=..\..\..\examples\systemc\pipe\display.cpp

"$(INTDIR)\display.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pipe\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pipe\numgen.cpp

"$(INTDIR)\numgen.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pipe\stage1.cpp

"$(INTDIR)\stage1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pipe\stage2.cpp

"$(INTDIR)\stage2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pipe\stage3.cpp

"$(INTDIR)\stage3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

