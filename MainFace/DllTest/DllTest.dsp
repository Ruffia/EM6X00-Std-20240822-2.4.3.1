# Microsoft Developer Studio Project File - Name="DllTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DllTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DllTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DllTest.mak" CFG="DllTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DllTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DllTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DllTest - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "DllTest - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Bin"
# PROP Intermediate_Dir "..\..\Bin\SharedDebug\Main\DllTest"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Common\Include" /I "..\..\Common\Include\USB" /I "..\..\Common\Template" /I "..\..\CtrlLayer\SemScan" /I "..\..\CtrlLayer\SemCore" /I "..\..\CtrlLayer\SemControl" /I "..\..\CtrlLayer\VirtualMCICard" /I "..\..\CtrlLayer\SemControlCardAdapter" /I "..\..\CtrlLayer\SemCoreMsgComm" /I "..\..\CtrlLayer\VACStateMachine" /I "..\..\CtrlLayer\VACObtain" /I "..\..\Common\Include\SemCoreCtrlPanelManager" /I "..\..\Language\LanguageManager" /I "..\..\AutoFunctions\AutoTest" /I "..\..\CtrlLayer\Options_Camera" /I "..\..\CtrlLayer\Options_XYStage" /I "..\..\Common\Include\Stage" /I "..\..\CtrlLayer\Options_StageMC6600Wnd" /I "..\..\CtrlLayer\Options_StagePod" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SemCoreCtrlPanelManager.lib VirtualMCICard.lib SemScan.lib SemControl.lib winmm.lib LanguageManager.lib AutoTest.lib Options_Camera.lib Options_XYStage.lib Options_StageMC6600Wnd.lib Options_StagePod.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\Bin"

!ENDIF 

# Begin Target

# Name "DllTest - Win32 Release"
# Name "DllTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DlgAngleOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCCDtest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMobileUScale.cpp
# End Source File
# Begin Source File

SOURCE=.\DllTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DllTest.rc
# End Source File
# Begin Source File

SOURCE=.\DllTestDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DllTestView.cpp
# End Source File
# Begin Source File

SOURCE=.\LineTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TransparentImage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DlgAngleOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgCCDtest.h
# End Source File
# Begin Source File

SOURCE=.\DlgLanguage.h
# End Source File
# Begin Source File

SOURCE=.\DlgMobileUScale.h
# End Source File
# Begin Source File

SOURCE=.\DllTest.h
# End Source File
# Begin Source File

SOURCE=.\DllTestDoc.h
# End Source File
# Begin Source File

SOURCE=.\DllTestView.h
# End Source File
# Begin Source File

SOURCE=.\LineTracker.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TransparentImage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp_led_red_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_led_red_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DllTest.ico
# End Source File
# Begin Source File

SOURCE=.\res\DllTest.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DllTestDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\PneumaticDiagram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
