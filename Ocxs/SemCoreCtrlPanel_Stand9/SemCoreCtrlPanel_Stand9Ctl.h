// Last Modify : 2009.05.08

#if !defined(AFX_SEMCORECTRLPANEL_STAND9CTL_H__EEA9D813_07D7_4AB3_82A9_B8DEE5277BBB__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND9CTL_H__EEA9D813_07D7_4AB3_82A9_B8DEE5277BBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand9Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand9Ctrl ActiveX Control class.
#include "ColorCtrl.h"
#include "MySpinButtonCtrl.h"

typedef struct tagSemCtrlFigures{
    VARIANT varMin;
    VARIANT varMax;
	VARIANT varPos;
} SemCtrlFigures;
typedef SemCtrlFigures FAR *LPSemCtrlFigures;

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;

const UINT nIDSpin1Ctrl			= 900;
const UINT nIDSpin2Ctrl			= 901;
const UINT nIDMultiply10Ctrl	= 902;
const UINT nIDDivide10Ctrl		= 903;
const UINT nIDStaticCtrl		= 904;

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl : See SemCoreCtrlPanel_Stand9Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand9Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand9Ctrl)

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CString 		m_strX;
	CString 		m_strY;

	CMySpinButtonCtrl	m_ctrlSpin1;
	CMySpinButtonCtrl	m_ctrlSpin2;
	CButton				m_ctrlMultiply10;
	CButton				m_ctrlDivide10;

	CFont				m_fontOnSize;
	CColorCtrl<CStatic> m_ctrlStatic;
	CBitmap				m_bmpMul10;
	CBitmap				m_bmpDiv10;

	//08.11.27
	HICON				m_hIconMul10;
	HICON				m_hIconDiv10;
	//08.11.27

	// 12.11.16
	BOOL				m_bEnabled;
	// 12.11.16

	CString			m_strFast[8];
	int				m_nFastTotal;

// Constructor
public:
	CSemCoreCtrlPanel_Stand9Ctrl();
	// 09.02.17
	int		CreateControl( CRect rcBounds );
	BOOL	CreateStatic( CRect rcBounds );
	BOOL	CreateMultiply10( CRect rcBounds );
	BOOL	CreateDivide10( CRect rcBounds );
	BOOL	CreateSpin1( CRect rcBounds );
	BOOL	CreateSpin2( CRect rcBounds );
	void	ResetControl();
	BOOL	m_bMode1Line;
	// 09.02.17

	CString GetDisplayString();

	void	SetCtrlPos( double dblPos );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand9Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnTextChanged();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand9Ctrl();
	void OnSizeFont(CWnd* pCtrl);

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand9Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand9Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand9Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand9Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand9Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMultiply10();
	afx_msg void OnButtonDivide10();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnFastChanged(UINT nID);	// 09.05.08
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand9Ctrl)
	BOOL m_bCaptionVisible;
	afx_msg void OnCaptionVisibleChanged();
	BOOL m_bDebugInfoVisible;
	afx_msg void OnDebugInfoVisibleChanged();
	BOOL m_bNewDisplayMode;
	afx_msg void OnNewDisplayModeChanged();
	afx_msg void Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber);
	afx_msg void Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag);
	afx_msg void SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax);
	afx_msg void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand9Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	void FireReportAction(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportAction,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand9Ctrl)
	dispidCaptionVisible = 1L,
	dispidDebugInfoVisible = 2L,
	dispidNewDisplayMode = 3L,
	dispidBind = 4L,
	dispidEnable = 5L,
	dispidSetRange = 6L,
	dispidSetPos = 7L,
	dispidSyncPos = 8L,
	dispidSyncText = 9L,
	eventidReportPos = 1L,
	eventidReportAction = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND9CTL_H__EEA9D813_07D7_4AB3_82A9_B8DEE5277BBB__INCLUDED)
