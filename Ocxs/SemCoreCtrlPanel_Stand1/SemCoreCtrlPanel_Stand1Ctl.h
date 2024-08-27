// Last Modify : 2005.04.11

#if !defined(AFX_SEMCORECTRLPANEL_STAND1CTL_H__6C419AF0_B01E_48B5_93A9_135EB1C40280__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND1CTL_H__6C419AF0_B01E_48B5_93A9_135EB1C40280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand1Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand1Ctrl ActiveX Control class.
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

const UINT nIDScrollBarX = 918;
const UINT nIDScrollBarY = 888;

const UINT nIDSpin1Ctrl			= 900;
const UINT nIDSpin2Ctrl			= 901;
const UINT nIDStaticCtrl		= 903;
const UINT nIDComboCtrl			= 904;
const UINT nIDMultiply10Ctrl	= 905;
const UINT nIDDivide10Ctrl		= 906;

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl : See SemCoreCtrlPanel_Stand1Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand1Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand1Ctrl)

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CScrollBar		m_sbScrollBarX;
	BOOL			m_blCtrlEnableX;

	CString 		m_strX;
	CString 		m_strY;

	CColorCtrl<CStatic> m_ctrlStatic;
	CComboBox			m_ctrlCombo;
	CFont				m_fontOnSize;
	//2008.12.03使用标准SpinButtonCtrl
//	CMySpinButtonCtrl	m_ctrlSpin1;
	CSpinButtonCtrl		m_ctrlSpin1;
	//2008.12.03
//	CBitmapButton		m_ctrlMultiply10;
	CButton				m_ctrlMultiply10;
	CButton				m_ctrlDivide10;
	CBitmap				m_bmpMul10;
	CBitmap				m_bmpDiv10;

	CRect				m_ExpectInvalidRect;

	CString				m_strFast[8];
	int					m_nFastTotal;

	BOOL	CreateScrollBarH(const CRect &rcBounds);
	BOOL	CreateScrollBarV(const CRect &rcBounds);
	BOOL	CreateButtonAndLabel(const CRect &rcBounds);

	void	DrawHScroll(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	void	DrawVScroll(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	void	DrawBtnAndLabel(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

// Constructor
public:
	CSemCoreCtrlPanel_Stand1Ctrl();

	CString	GetDisplayString();

	void	SetCtrlPos( long lPos );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand1Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand1Ctrl();
	void OnSizeFont(CWnd* pCtrl);

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand1Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand1Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand1Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand1Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand1Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMultiply10();
	afx_msg void OnButtonDivide10();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnFastChanged(UINT nID);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand1Ctrl)
	BOOL m_bCaptionVisible;
	afx_msg void OnCaptionVisibleChanged();
	BOOL m_bDebugInfoVisible;
	afx_msg void OnDebugInfoVisibleChanged();
	CString m_csXMinString;
	afx_msg void OnXMinStringChanged();
	CString m_csXMaxString;
	afx_msg void OnXMaxStringChanged();
	short m_nControlStyle;
	afx_msg void OnControlStyleChanged();
	afx_msg void Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber);
	afx_msg void Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag);
	afx_msg void SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax);
	afx_msg void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag);
	afx_msg void SyncString(const VARIANT FAR& ResourceID, const VARIANT FAR& ResourceString);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand1Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	void FireReportAction(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportAction,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:

	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand1Ctrl)
	dispidCaptionVisible = 1L,
	dispidDebugInfoVisible = 2L,
	dispidXMinString = 3L,
	dispidXMaxString = 4L,
	dispidControlStyle = 5L,
	dispidBind = 6L,
	dispidEnable = 7L,
	dispidSetRange = 8L,
	dispidSetPos = 9L,
	dispidSyncPos = 10L,
	dispidSyncText = 11L,
	eventidReportPos = 1L,
	eventidReportAction = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND1CTL_H__6C419AF0_B01E_48B5_93A9_135EB1C40280__INCLUDED)
