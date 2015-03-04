# Microsoft Developer Studio Generated NMAKE File, Based on risc_cpu.dsp
!IF "$(CFG)" == ""
CFG=risc_cpu - Win32 Release
!MESSAGE No configuration specified. Defaulting to risc_cpu - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "risc_cpu - Win32 Release" && "$(CFG)" != "risc_cpu - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "risc_cpu.mak" CFG="risc_cpu - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "risc_cpu - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "risc_cpu - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "risc_cpu - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\risc_cpu.exe"


CLEAN :
	-@erase "$(INTDIR)\bios.obj"
	-@erase "$(INTDIR)\dcache.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\exec.obj"
	-@erase "$(INTDIR)\fetch.obj"
	-@erase "$(INTDIR)\floating.obj"
	-@erase "$(INTDIR)\icache.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mmxu.obj"
	-@erase "$(INTDIR)\paging.obj"
	-@erase "$(INTDIR)\pic.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\risc_cpu.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\risc_cpu.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\risc_cpu.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\risc_cpu.pdb" /machine:I386 /out:"$(OUTDIR)\risc_cpu.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bios.obj" \
	"$(INTDIR)\dcache.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\exec.obj" \
	"$(INTDIR)\fetch.obj" \
	"$(INTDIR)\floating.obj" \
	"$(INTDIR)\icache.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mmxu.obj" \
	"$(INTDIR)\paging.obj" \
	"$(INTDIR)\pic.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\risc_cpu.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "risc_cpu - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\risc_cpu.exe"


CLEAN :
	-@erase "$(INTDIR)\bios.obj"
	-@erase "$(INTDIR)\dcache.obj"
	-@erase "$(INTDIR)\decode.obj"
	-@erase "$(INTDIR)\exec.obj"
	-@erase "$(INTDIR)\fetch.obj"
	-@erase "$(INTDIR)\floating.obj"
	-@erase "$(INTDIR)\icache.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\mmxu.obj"
	-@erase "$(INTDIR)\paging.obj"
	-@erase "$(INTDIR)\pic.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\risc_cpu.exe"
	-@erase "$(OUTDIR)\risc_cpu.ilk"
	-@erase "$(OUTDIR)\risc_cpu.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\risc_cpu.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\risc_cpu.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\risc_cpu.pdb" /debug /machine:I386 /out:"$(OUTDIR)\risc_cpu.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bios.obj" \
	"$(INTDIR)\dcache.obj" \
	"$(INTDIR)\decode.obj" \
	"$(INTDIR)\exec.obj" \
	"$(INTDIR)\fetch.obj" \
	"$(INTDIR)\floating.obj" \
	"$(INTDIR)\icache.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\mmxu.obj" \
	"$(INTDIR)\paging.obj" \
	"$(INTDIR)\pic.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\risc_cpu.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("risc_cpu.dep")
!INCLUDE "risc_cpu.dep"
!ELSE 
!MESSAGE Warning: cannot find "risc_cpu.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "risc_cpu - Win32 Release" || "$(CFG)" == "risc_cpu - Win32 Debug"
SOURCE=..\..\..\examples\systemc\risc_cpu\bios.cpp

"$(INTDIR)\bios.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\dcache.cpp

"$(INTDIR)\dcache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\decode.cpp

"$(INTDIR)\decode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\exec.cpp

"$(INTDIR)\exec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\fetch.cpp

"$(INTDIR)\fetch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\floating.cpp

"$(INTDIR)\floating.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\icache.cpp

"$(INTDIR)\icache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\mmxu.cpp

"$(INTDIR)\mmxu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\paging.cpp

"$(INTDIR)\paging.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\risc_cpu\pic.cpp

"$(INTDIR)\pic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

