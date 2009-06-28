# Microsoft Developer Studio Project File - Name="AnimatLibrary" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AnimatLibrary - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AnimatLibrary.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AnimatLibrary.mak" CFG="AnimatLibrary - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AnimatLibrary - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AnimatLibrary - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AnimatLibrary - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ANIMATLIBRARY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\STLport-4.6.1\stlport" /I "..\STLport-4.6.1\src" /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ANIMATLIBRARY_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /nodefaultlib:"LIBC" /out:"Release/AnimatLibrary_vc6.dll" /libpath:"..\STLport-4.6.1\lib" /libpath:"..\..\lib"
# Begin Custom Build
OutDir=.\Release
TargetPath=.\Release\AnimatLibrary_vc6.dll
InputPath=.\Release\AnimatLibrary_vc6.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Copy "$(OutDir)\AnimatLibrary_vc6.lib" ..\..\lib\AnimatLibrary_vc6.lib 
	Copy "$(TargetPath)" ..\..\bin\AnimatLibrary_vc6.dll 
	Copy "$(TargetPath)" %systemroot%\system32 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "AnimatLibrary - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ANIMATLIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\STLport-4.6.1\stlport" /I "..\STLport-4.6.1\src" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ANIMATLIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /nodefaultlib:"LIBCMT" /out:"Debug/AnimatLibrary_vc6D.dll" /pdbtype:sept /libpath:"..\STLport-4.6.1\lib" /libpath:"..\..\lib"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build
OutDir=.\Debug
TargetPath=.\Debug\AnimatLibrary_vc6D.dll
InputPath=.\Debug\AnimatLibrary_vc6D.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Copy "$(OutDir)\AnimatLibrary_vc6D.lib" ..\..\lib\AnimatLibrary_vc6D.lib 
	Copy "$(TargetPath)" ..\..\bin\AnimatLibrary_vc6D.dll 
	Copy "$(TargetPath)" %systemroot%\system32 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "AnimatLibrary - Win32 Release"
# Name "AnimatLibrary - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AlClassFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\AlCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\AlHingeJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\AlJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\AlMuscle.cpp
# End Source File
# Begin Source File

SOURCE=.\AlMuscleAttachment.cpp
# End Source File
# Begin Source File

SOURCE=.\AlOrganism.cpp
# End Source File
# Begin Source File

SOURCE=.\AlPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\AlRigidBody.cpp
# End Source File
# Begin Source File

SOURCE=.\AlSensor.cpp
# End Source File
# Begin Source File

SOURCE=.\AlSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\AlStaticJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\AlStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimatUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlBox.h
# End Source File
# Begin Source File

SOURCE=.\AlClassFactory.h
# End Source File
# Begin Source File

SOURCE=.\AlCylinder.h
# End Source File
# Begin Source File

SOURCE=.\AlHingeJoint.h
# End Source File
# Begin Source File

SOURCE=.\AlJoint.h
# End Source File
# Begin Source File

SOURCE=.\AlMuscle.h
# End Source File
# Begin Source File

SOURCE=.\AlMuscleAttachment.h
# End Source File
# Begin Source File

SOURCE=.\AlOrganism.h
# End Source File
# Begin Source File

SOURCE=.\AlPlane.h
# End Source File
# Begin Source File

SOURCE=.\AlRigidBody.h
# End Source File
# Begin Source File

SOURCE=.\AlSensor.h
# End Source File
# Begin Source File

SOURCE=.\AlSimulator.h
# End Source File
# Begin Source File

SOURCE=.\AlStaticJoint.h
# End Source File
# Begin Source File

SOURCE=.\AlStructure.h
# End Source File
# Begin Source File

SOURCE=.\AnimatUtils.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
