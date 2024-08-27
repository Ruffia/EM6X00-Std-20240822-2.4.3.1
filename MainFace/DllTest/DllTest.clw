; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDllTestView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DllTest.h"
LastPage=0

ClassCount=10
Class1=CDllTestApp
Class2=CDllTestDoc
Class3=CDllTestView
Class4=CMainFrame

ResourceCount=7
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_DIALOG_Bar_HV_VAC
Class6=CDlgMobileUScale
Class7=CTransparentImage
Resource3=IDD_DIALOG_Language
Class8=CDlgCCDtest
Resource4=IDD_DIALOG_MobileUScale
Resource5=IDD_ABOUTBOX
Class9=CDlgLanguage
Resource6=IDD_DIALOG_CCDtest
Class10=CDlgAngleOptions
Resource7=IDD_DIALOG_Angle_Options

[CLS:CDllTestApp]
Type=0
HeaderFile=DllTest.h
ImplementationFile=DllTest.cpp
Filter=N
LastObject=CDllTestApp

[CLS:CDllTestDoc]
Type=0
HeaderFile=DllTestDoc.h
ImplementationFile=DllTestDoc.cpp
Filter=N

[CLS:CDllTestView]
Type=0
HeaderFile=DllTestView.h
ImplementationFile=DllTestView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CDllTestView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_Test_MobileUScale




[CLS:CAboutDlg]
Type=0
HeaderFile=DllTest.cpp
ImplementationFile=DllTest.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=11
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC_PneumaticDiagram,static,1342177294
Control6=IDC_STATIC_Lamp_Vacuum_Vf,static,1342177294
Control7=IDC_STATIC_Lamp_Vacuum_Vc,static,1342177294
Control8=IDC_STATIC_Lamp_Vacuum_TMP,static,1342177294
Control9=IDC_STATIC_Lamp_Vacuum_V1,static,1342177294
Control10=IDC_STATIC_Lamp_Vacuum_Va,static,1342177294
Control11=IDC_BUTTON_PneumaticDiagram_Status,button,1342242816

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_Test_AutoTest
Command10=ID_Test_Camera2
Command11=ID_Test_Camera3
Command12=ID_Test_XYstage
Command13=ID_Test_1
Command14=ID_Test_2
Command15=ID_Test_3
Command16=ID_Test_4
Command17=ID_Test_MobileUScale
Command18=ID_Test_Line
Command19=ID_Test_AngleMeasure
Command20=ID_Language_Test
Command21=ID_Test_MC6600
Command22=ID_Test_StagePod
CommandCount=22

[DLG:IDD_DIALOG_MobileUScale]
Type=1
Class=CDlgMobileUScale
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_Mag,edit,1350631552
Control4=IDC_SPIN_Mag1000,msctls_updown32,1342177312
Control5=IDC_SPIN_Mag10,msctls_updown32,1342177312
Control6=IDC_SPIN_Mag,msctls_updown32,1342177312

[CLS:CDlgMobileUScale]
Type=0
HeaderFile=DlgMobileUScale.h
ImplementationFile=DlgMobileUScale.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgMobileUScale

[CLS:CTransparentImage]
Type=0
HeaderFile=TransparentImage.h
ImplementationFile=TransparentImage.cpp
BaseClass=CStatic
Filter=W
LastObject=CTransparentImage
VirtualFilter=WC

[DLG:IDD_DIALOG_CCDtest]
Type=1
Class=CDlgCCDtest
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CDlgCCDtest]
Type=0
HeaderFile=DlgCCDtest.h
ImplementationFile=DlgCCDtest.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgCCDtest

[DLG:IDD_DIALOG_Bar_HV_VAC]
Type=1
Class=?
ControlCount=10
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,button,1342177287
Control4=IDC_CHECK_V1,button,1342246915
Control5=IDC_BUTTON_Pump,button,1342242816
Control6=IDC_BUTTON_Vent,button,1342242816
Control7=IDC_EDIT_HV,edit,1350633602
Control8=IDC_EDIT_Filament,edit,1350633602
Control9=IDC_SCROLLBAR_HV,scrollbar,1342177280
Control10=IDC_SCROLLBAR_Filament,scrollbar,1342177280

[DLG:IDD_DIALOG_Language]
Type=1
Class=CDlgLanguage
ControlCount=8
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_Path_hFile,edit,1350631552
Control5=IDC_BUTTON_Browse_hFile,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_Path_rcFile,edit,1350631552
Control8=IDC_BUTTON_Browse_rcFile,button,1342242816

[CLS:CDlgLanguage]
Type=0
HeaderFile=DlgLanguage.h
ImplementationFile=DlgLanguage.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_Browse_rcFile

[DLG:IDD_DIALOG_Angle_Options]
Type=1
Class=CDlgAngleOptions
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC_AngleOptions_Color_Line,static,1342308354
Control4=IDC_STATIC_AngleOptions_Color_Arc,static,1342308354
Control5=IDC_STATIC_AngleOptions_Color_Text,static,1342308354
Control6=IDC_STATIC_AngleOptions_Font_Size,static,1342308352
Control7=IDC_BUTTON_AngleOptions_Color_Line,button,1342242827
Control8=IDC_BUTTON_AngleOptions_Color_Arc,button,1342242827
Control9=IDC_BUTTON_AngleOptions_Color_Text,button,1342242827
Control10=IDC_COMBO_AngleOptions_Font_Size,combobox,1344339971

[CLS:CDlgAngleOptions]
Type=0
HeaderFile=DlgAngleOptions.h
ImplementationFile=DlgAngleOptions.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_Test_Line

