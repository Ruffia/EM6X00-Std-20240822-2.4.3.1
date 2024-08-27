// Last Modify : 2008.05.09

#if !defined(AFX_SEMCORECTRLPANEL_STAND6CTL_H__80CA6947_1B94_4DC5_A427_DF49368301A4__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND6CTL_H__80CA6947_1B94_4DC5_A427_DF49368301A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand6Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand6Ctrl ActiveX Control class.
#include "GradeDlg.h"

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
/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl : See SemCoreCtrlPanel_Stand6Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand6Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand6Ctrl)

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CString 		m_strX;
	CString 		m_strY;

	BOOL			m_blCtrlEnableX;

	CRect			m_ExpectInvalidRect;

	CScrollBar		m_sbScrollBarX;
	BOOL CreateScrollBarH(const CRect &rcBounds);
	void DrawHScroll(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	void DrawDlgGrade(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	void OnSizeHScroll( const CRect &rcBounds );

	int				m_nCtrlGrade, m_lCtrlVernierScale;
	long			m_lVernierBasise, m_lVernierMultiple;
	long			m_lVernierMin, m_lVernierMax, m_lVernierPos;
	CGradeDlg		m_dlgGrade;
	int GetDlgHeight();
	void ReadjustScollbarCtrlParam( BOOL blResetVernier = FALSE );

	CFontHolder		m_fontGradeFont;
	OLE_COLOR		m_clrGradeColor;

	CString			m_strFast[8];
	int				m_nFastTotal;

// Constructor
public:
	CSemCoreCtrlPanel_Stand6Ctrl();

	void	ReportGradeChanged( int nGrade );
//-----2008.05.09-----
	void	NotifyGradeChanged();
//-----2008.05.09-----
	void	NotifyActionChanged( int nIndex, int nCheck );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand6Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand6Ctrl();

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand6Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand6Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand6Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand6Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand6Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnFastChanged(UINT nID);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand6Ctrl)
	BOOL m_bCaptionVisible;
	afx_msg void OnCaptionVisibleChanged();
	BOOL m_bDebugInfoVisible;
	afx_msg void OnDebugInfoVisibleChanged();
	CString m_csXMinString;
	afx_msg void OnXMinStringChanged();
	CString m_csXMaxString;
	afx_msg void OnXMaxStringChanged();
	BOOL m_bGradeVisible;
	afx_msg void OnGradeVisibleChanged();
	BOOL m_bAutoVisible;
	afx_msg void OnAutoVisibleChanged();
	afx_msg OLE_COLOR GetGradeColor();
	afx_msg void SetGradeColor(OLE_COLOR nNewValue);
	afx_msg LPFONTDISP GetGradeFont();
	afx_msg void SetGradeFont(LPFONTDISP newValue);
	afx_msg short GetGradeState();
	afx_msg void SetGradeState(short nNewValue);
	afx_msg void Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber);
	afx_msg void Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag);
	afx_msg void SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax);
	afx_msg void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag);
	afx_msg void SyncResString(const VARIANT FAR& ResourceID, LPCTSTR strRes);
	afx_msg void SyncString(VARIANT &ResourceID, VARIANT &ResourceString);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand6Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	void FireReportAction(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportAction,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidSyncString = 17L,
		//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand6Ctrl)
	dispidCaptionVisible = 1L,
	dispidDebugInfoVisible = 2L,
	dispidXMinString = 3L,
	dispidXMaxString = 4L,
	dispidGradeVisible = 5L,
	dispidAutoVisible = 6L,
	dispidGradeColor = 7L,
	dispidGradeFont = 8L,
	dispidGradeState = 9L,
	dispidBind = 10L,
	dispidEnable = 11L,
	dispidSetRange = 12L,
	dispidSetPos = 13L,
	dispidSyncPos = 14L,
	dispidSyncText = 15L,
	dispidSyncResString = 16L,
	eventidReportPos = 1L,
	eventidReportAction = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND6CTL_H__80CA6947_1B94_4DC5_A427_DF49368301A4__INCLUDED)
