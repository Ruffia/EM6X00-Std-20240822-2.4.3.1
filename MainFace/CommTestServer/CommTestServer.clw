; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCommTestServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CommTestServer.h"

ClassCount=3
Class1=CCommTestServerApp
Class2=CCommTestServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_COMMTESTSERVER_DIALOG

[CLS:CCommTestServerApp]
Type=0
HeaderFile=CommTestServer.h
ImplementationFile=CommTestServer.cpp
Filter=N

[CLS:CCommTestServerDlg]
Type=0
HeaderFile=CommTestServerDlg.h
ImplementationFile=CommTestServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_SPIN_RotAngle

[CLS:CAboutDlg]
Type=0
HeaderFile=CommTestServerDlg.h
ImplementationFile=CommTestServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COMMTESTSERVER_DIALOG]
Type=1
Class=CCommTestServerDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_Listen_Start,button,1342242816
Control4=IDC_BUTTON_Listen_Stop,button,1342242816
Control5=IDC_STATIC,static,1342308866
Control6=IDC_EDIT_Mag,edit,1350631552
Control7=IDC_SPIN_Mag,msctls_updown32,1342177312
Control8=IDC_STATIC,static,1342308866
Control9=IDC_EDIT_RotAngle,edit,1350631552
Control10=IDC_SPIN_RotAngle,msctls_updown32,1342177312

