#if !defined(AFX_CAMERAWND_H__9EE2CFBF_9C36_4D2C_9CAC_5AA043D9CE32__INCLUDED_)
#define AFX_CAMERAWND_H__9EE2CFBF_9C36_4D2C_9CAC_5AA043D9CE32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraWnd.h : header file
//

#include "DlgCCDCameraSetting.h"

/////////////////////////////////////////////////////////////////////////////
// CCameraWnd window

class CCameraWnd : public CWnd
{
// Construction
public:
	CCameraWnd();

	///////////////////////////////////
	// 本地化相关
	WORD	m_wLanguageID;				// 语言类型
	void	SetLanguageID( WORD wID );	// 主程序设置的语言类型
	void	Localize();					// 本地化
	///////////////////////////////////

// Attributes
public:
	int						m_nCapture;
	CMenu					m_Menu;
	CDlgCCDCameraSetting	m_dlgCCDCameraSetting;

// Operations
public:
	void	CamInit();
	void	CamStop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCameraWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCameraWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCameraSettings();
	afx_msg void OnUpdateCameraSettings(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERAWND_H__9EE2CFBF_9C36_4D2C_9CAC_5AA043D9CE32__INCLUDED_)
