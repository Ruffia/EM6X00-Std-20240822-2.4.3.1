#if !defined(AFX_PANELCAMERA_H__8B52FF4D_245F_4050_AE2D_EDDE3534CB21__INCLUDED_)
#define AFX_PANELCAMERA_H__8B52FF4D_245F_4050_AE2D_EDDE3534CB21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelCamera.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelCamera dialog

class CPanelCamera : public CDialog
{
// Construction
public:
	CPanelCamera(CWnd* pParent = NULL);   // standard constructor
	void	ImageShow( IplImage* img, UINT ID );	
	void	ImageResize(IplImage* img);
	void	CamInit();
	void	CamStop();

	int		m_nCapture;

// Dialog Data
	//{{AFX_DATA(CPanelCamera)
	enum { IDD = IDD_PANEL_Camera };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelCamera)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelCamera)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnRun();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELCAMERA_H__8B52FF4D_245F_4050_AE2D_EDDE3534CB21__INCLUDED_)
