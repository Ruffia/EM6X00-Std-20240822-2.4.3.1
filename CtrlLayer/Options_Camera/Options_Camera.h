// Options_Camera.h : main header file for the OPTIONS_CAMERA DLL
//

#if !defined(AFX_OPTIONS_CAMERA_H__465E4288_1232_4BE1_B777_5C3F5D499939__INCLUDED_)
#define AFX_OPTIONS_CAMERA_H__465E4288_1232_4BE1_B777_5C3F5D499939__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "cv.h"
#include "highgui.h"
#include "CameraDS.h"

#define IMAGE_WIDTH		800	// 256
#define IMAGE_HEIGHT	600	// 256
#define IMAGE_CHANNELS	3

#include "PanelCamera.h"
#include "CameraWnd.h"

/////////////////////////////////////////////////////////////////////////////
// COptions_CameraApp
// See Options_Camera.cpp for the implementation of this class
//

class COptions_CameraApp : public CWinApp
{
public:
	COptions_CameraApp();

	CPanelCamera*	m_pPanelCamera;
	CCameraWnd*		m_pCameraWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions_CameraApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptions_CameraApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern COptions_CameraApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_CAMERA_H__465E4288_1232_4BE1_B777_5C3F5D499939__INCLUDED_)
