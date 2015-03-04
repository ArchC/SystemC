# Microsoft Developer Studio Project File - Name="risc_cpu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=risc_cpu - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "risc_cpu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "risc_cpu.mak" CFG="risc_cpu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "risc_cpu - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "risc_cpu - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "risc_cpu - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "../../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "risc_cpu - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "../../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "risc_cpu - Win32 Release"
# Name "risc_cpu - Win32 Debug"
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\bios.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\bios.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\dcache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\dcache.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\decode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\decode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\directive.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\exec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\exec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\fetch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\fetch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\floating.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\floating.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\icache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\icache.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\mmxu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\mmxu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\paging.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\paging.h
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\pic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\systemc\risc_cpu\pic.h
# End Source File
# Begin Source File

SOURCE=..\..\systemc\Release\systemc.lib
# End Source File
# End Target
# End Project
