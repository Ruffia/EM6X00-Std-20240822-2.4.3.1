#if !defined(AFX_SEMCORECTRLPANEL_STAND3CTL_H__9AF6228B_7B70_42A3_A0CA_D85C98423FAC__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND3CTL_H__9AF6228B_7B70_42A3_A0CA_D85C98423FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand3Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand3Ctrl ActiveX Control class.
typedef struct tagSemCtrlFigures{
    VARIANT varMin;
    VARIANT varMax;
	VARIANT varPos;
} SemCtrlFigures;
typedef SemCtrlFigures FAR *LPSemCtrlFigures;

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;

class CRoundSliderDlg;
/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl : See SemCoreCtrlPanel_Stand3Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand3Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand3Ctrl)

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CRoundSliderDlg	m_dlgRoundSlider;

	CString 		m_strX;
	CString 		m_strY;

// Constructor
public:
	CSemCoreCtrlPanel_Stand3Ctrl();

	CString GetDisplayString();
	void ReportPos( int nX, int nY=0 );


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand3Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnTextChanged();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand3Ctrl();

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand3Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand3Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand3Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand3Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand3Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand3Ctrl)
	BOOL m_bCaptionVisible;
	afx_msg void OnCaptionVisibleChanged();
	BOOL m_bDebugInfoVisible;
	afx_msg void OnDebugInfoVisibleChanged();
	BOOL m_bGradeVisible;
	afx_msg void OnGradeVisibleChanged();
	afx_msg void Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber);
	afx_msg void Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag);
	afx_msg void SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax);
	afx_msg void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag);
	afx_msg void SyncResString(const VARIANT FAR& ResourceID, LPCTSTR strRes);
	afx_msg void SyncString(const VARIANT FAR& ResourceID, const VARIANT FAR& ResourceString);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand3Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand3Ctrl)
	dispidCaptionVisible = 1L,
	dispidDebugInfoVisible = 2L,
	dispidGradeVisible = 3L,
	dispidBind = 4L,
	dispidEnable = 5L,
	dispidSetRange = 6L,
	dispidSetPos = 7L,
	dispidSyncPos = 8L,
	dispidSyncText = 9L,
	dispidSyncResString = 10L,
	eventidReportPos = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND3CTL_H__9AF6228B_7B70_42A3_A0CA_D85C98423FAC__INCLUDED)
