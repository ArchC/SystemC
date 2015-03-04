# Microsoft Developer Studio Generated NMAKE File, Based on fft_fxpt.dsp
!IF "$(CFG)" == ""
CFG=fft_fxpt - Win32 Release
!MESSAGE No configuration specified. Defaulting to fft_fxpt - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "fft_fxpt - Win32 Release" && "$(CFG)" != "fft_fxpt - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fft_fxpt.mak" CFG="fft_fxpt - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fft_fxpt - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "fft_fxpt - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "fft_fxpt - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\fft_fxpt.exe"


CLEAN :
	-@erase "$(INTDIR)\fft.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\sink.obj"
	-@erase "$(INTDIR)\source.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\fft_fxpt.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\fft_fxpt.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fft_fxpt.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\fft_fxpt.pdb" /machine:I386 /out:"$(OUTDIR)\fft_fxpt.exe" 
LINK32_OBJS= \
	"$(INTDIR)\fft.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\sink.obj" \
	"$(INTDIR)\source.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\fft_fxpt.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fft_fxpt - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\fft_fxpt.exe"


CLEAN :
	-@erase "$(INTDIR)\fft.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\sink.obj"
	-@erase "$(INTDIR)\source.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\fft_fxpt.exe"
	-@erase "$(OUTDIR)\fft_fxpt.ilk"
	-@erase "$(OUTDIR)\fft_fxpt.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\fft_fxpt.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fft_fxpt.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\fft_fxpt.pdb" /debug /machine:I386 /out:"$(OUTDIR)\fft_fxpt.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\fft.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\sink.obj" \
	"$(INTDIR)\source.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\fft_fxpt.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("fft_fxpt.dep")
!INCLUDE "fft_fxpt.dep"
!ELSE 
!MESSAGE Warning: cannot find "fft_fxpt.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "fft_fxpt - Win32 Release" || "$(CFG)" == "fft_fxpt - Win32 Debug"
SOURCE=..\..\..\examples\systemc\fft\fft_fxpt\fft.cpp

"$(INTDIR)\fft.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fft\fft_fxpt\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fft\fft_fxpt\sink.cpp

"$(INTDIR)\sink.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\fft\fft_fxpt\source.cpp

"$(INTDIR)\source.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

