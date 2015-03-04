# Microsoft Developer Studio Generated NMAKE File, Based on pkt_switch.dsp
!IF "$(CFG)" == ""
CFG=pkt_switch - Win32 Release
!MESSAGE No configuration specified. Defaulting to pkt_switch - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "pkt_switch - Win32 Release" && "$(CFG)" != "pkt_switch - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pkt_switch.mak" CFG="pkt_switch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pkt_switch - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pkt_switch - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "pkt_switch - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\pkt_switch.exe"


CLEAN :
	-@erase "$(INTDIR)\fifo.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\receiver.obj"
	-@erase "$(INTDIR)\sender.obj"
	-@erase "$(INTDIR)\switch.obj"
	-@erase "$(INTDIR)\switch_clk.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\pkt_switch.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\pkt_switch.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pkt_switch.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\pkt_switch.pdb" /machine:I386 /out:"$(OUTDIR)\pkt_switch.exe" 
LINK32_OBJS= \
	"$(INTDIR)\fifo.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\receiver.obj" \
	"$(INTDIR)\sender.obj" \
	"$(INTDIR)\switch.obj" \
	"$(INTDIR)\switch_clk.obj" \
	"..\..\systemc\Release\systemc.lib"

"$(OUTDIR)\pkt_switch.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pkt_switch - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\pkt_switch.exe"


CLEAN :
	-@erase "$(INTDIR)\fifo.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\receiver.obj"
	-@erase "$(INTDIR)\sender.obj"
	-@erase "$(INTDIR)\switch.obj"
	-@erase "$(INTDIR)\switch_clk.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\pkt_switch.exe"
	-@erase "$(OUTDIR)\pkt_switch.ilk"
	-@erase "$(OUTDIR)\pkt_switch.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\pkt_switch.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pkt_switch.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\pkt_switch.pdb" /debug /machine:I386 /out:"$(OUTDIR)\pkt_switch.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\fifo.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\receiver.obj" \
	"$(INTDIR)\sender.obj" \
	"$(INTDIR)\switch.obj" \
	"$(INTDIR)\switch_clk.obj" \
	"..\..\systemc\Debug\systemc.lib"

"$(OUTDIR)\pkt_switch.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("pkt_switch.dep")
!INCLUDE "pkt_switch.dep"
!ELSE 
!MESSAGE Warning: cannot find "pkt_switch.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "pkt_switch - Win32 Release" || "$(CFG)" == "pkt_switch - Win32 Debug"
SOURCE=..\..\..\examples\systemc\pkt_switch\fifo.cpp

"$(INTDIR)\fifo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pkt_switch\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pkt_switch\receiver.cpp

"$(INTDIR)\receiver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pkt_switch\sender.cpp

"$(INTDIR)\sender.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pkt_switch\switch.cpp

"$(INTDIR)\switch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\examples\systemc\pkt_switch\switch_clk.cpp

"$(INTDIR)\switch_clk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

