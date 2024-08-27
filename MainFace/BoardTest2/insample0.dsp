# Microsoft Developer Studio Project File - Name="insample0" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=insample0 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "insample0.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "insample0.mak" CFG="insample0 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "insample0 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "insample0 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "insample0 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "insample0 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Bin"
# PROP Intermediate_Dir "..\..\Bin\SharedDebug\main\insample0-ft232"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "Include" /I "MsOffice" /I "CxImage2\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib winusb.lib ftd2xx.lib png.lib mng.lib jasper.lib jpeg.lib zlib.lib tiff.lib libdcr.lib cximage.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\Bin/BrdTest2.exe" /pdbtype:sept /libpath:"..\Bin" /libpath:"CxImage2\Lib"

!ENDIF 

# Begin Target

# Name "insample0 - Win32 Release"
# Name "insample0 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Calc.cpp
# End Source File
# Begin Source File

SOURCE=.\DetectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestLNS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestMCI.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestPower.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestSCN.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestSPR.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestVAC.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDiagLog.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDiagLogGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\FT232HDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\insample0.cpp
# End Source File
# Begin Source File

SOURCE=.\insample0.rc
# End Source File
# Begin Source File

SOURCE=.\insample0Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MacButtons.cpp
# End Source File
# Begin Source File

SOURCE=.\MCIBasicFunctionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsOffice\MsWord9.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMCI.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSL.cpp
# End Source File
# Begin Source File

SOURCE=.\PageVS.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabDialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Calc.h
# End Source File
# Begin Source File

SOURCE=.\DetectDialog.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestLNS.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestMCI.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestPower.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestSCN.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestSPR.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestTab.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTestVAC.h
# End Source File
# Begin Source File

SOURCE=.\DlgDiagLog.h
# End Source File
# Begin Source File

SOURCE=.\DlgDiagLogGraph.h
# End Source File
# Begin Source File

SOURCE=.\DlgDisplay.h
# End Source File
# Begin Source File

SOURCE=.\FT232HDevice.h
# End Source File
# Begin Source File

SOURCE=.\FTD2XX.H
# End Source File
# Begin Source File

SOURCE=.\insample0.h
# End Source File
# Begin Source File

SOURCE=.\insample0Dlg.h
# End Source File
# Begin Source File

SOURCE=.\MacButtons.h
# End Source File
# Begin Source File

SOURCE=.\MCIBasicFunctionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\PageMCI.h
# End Source File
# Begin Source File

SOURCE=.\PageSL.h
# End Source File
# Begin Source File

SOURCE=.\PageVS.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabDialog.h
# End Source File
# Begin Source File

SOURCE=.\USB_Functions.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp_led_blue_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_led_blue_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\insample0.ico
# End Source File
# Begin Source File

SOURCE=.\res\insample0.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\VCAIOUSB.lib
# End Source File
# End Target
# End Project
# Section insample0 : {717E53FF-C367-4148-95EA-B79503D71A83}
# 	1:17:IDD_DETECT_DIALOG:103
# 	2:16:Resource Include:resource.h
# 	2:16:DetectDialog.cpp:DetectDialog.cpp
# 	2:10:ENUM: enum:enum
# 	2:14:DetectDialog.h:DetectDialog.h
# 	2:19:CLASS: DetectDialog:DetectDialog
# 	2:17:IDD_DETECT_DIALOG:IDD_DETECT_DIALOG
# 	2:19:Application Include:insample0.h
# End Section
