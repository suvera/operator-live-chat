# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=TrayIcon - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to TrayIcon - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TrayIcon - Win32 Release" && "$(CFG)" !=\
 "TrayIcon - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrayIcon.mak" CFG="TrayIcon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrayIcon - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TrayIcon - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "TrayIcon - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "TrayIcon - Win32 Release"

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
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\TrayIcon.dll"

CLEAN : 
	-@erase ".\Release\TrayIcon.dll"
	-@erase ".\Release\WindowsTrayIcon.obj"
	-@erase ".\Release\TrayIcon.lib"
	-@erase ".\Release\TrayIcon.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /I "e:\jdk\include" /I "e:\jdk\include\win32" /I "D:\Java\include" /I "D:\Java\include\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /I "e:\jdk\include" /I "e:\jdk\include\win32" /I\
 "D:\Java\include" /I "D:\Java\include\win32" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/TrayIcon.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x813 /d "NDEBUG"
# ADD RSC /l 0x813 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TrayIcon.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d:\Java\lib\jvm.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib d:\Java\lib\jvm.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/TrayIcon.pdb" /machine:I386\
 /def:".\TrayIcon.def" /out:"$(OUTDIR)/TrayIcon.dll"\
 /implib:"$(OUTDIR)/TrayIcon.lib" 
DEF_FILE= \
	".\TrayIcon.def"
LINK32_OBJS= \
	"$(INTDIR)/WindowsTrayIcon.obj"

"$(OUTDIR)\TrayIcon.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TrayIcon - Win32 Debug"

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
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\TrayIcon.dll"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\TrayIcon.dll"
	-@erase ".\Debug\WindowsTrayIcon.obj"
	-@erase ".\Debug\TrayIcon.ilk"
	-@erase ".\Debug\TrayIcon.lib"
	-@erase ".\Debug\TrayIcon.exp"
	-@erase ".\Debug\TrayIcon.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "D:\Java\include" /I "D:\Java\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I "D:\Java\include" /I\
 "D:\Java\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/TrayIcon.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x813 /d "_DEBUG"
# ADD RSC /l 0x813 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TrayIcon.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d:\Java\lib\jvm.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib d:\Java\lib\jvm.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/TrayIcon.pdb" /debug /machine:I386\
 /def:".\TrayIcon.def" /out:"$(OUTDIR)/TrayIcon.dll"\
 /implib:"$(OUTDIR)/TrayIcon.lib" 
DEF_FILE= \
	".\TrayIcon.def"
LINK32_OBJS= \
	"$(INTDIR)/WindowsTrayIcon.obj"

"$(OUTDIR)\TrayIcon.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "TrayIcon - Win32 Release"
# Name "TrayIcon - Win32 Debug"

!IF  "$(CFG)" == "TrayIcon - Win32 Release"

!ELSEIF  "$(CFG)" == "TrayIcon - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WindowsTrayIcon.cpp
DEP_CPP_WINDO=\
	"D:\Java\include\jni.h"\
	".\jeans_trayicon_WindowsTrayIcon.h"\
	"D:\Java\include\win32\jni_md.h"\
	

"$(INTDIR)\WindowsTrayIcon.obj" : $(SOURCE) $(DEP_CPP_WINDO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TrayIcon.def

!IF  "$(CFG)" == "TrayIcon - Win32 Release"

!ELSEIF  "$(CFG)" == "TrayIcon - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
