#if !defined(AFX_SEMCORECTRLPANEL_STAND7CTL_H__7C1CB435_C40C_4FD2_81FC_DD844035576A__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND7CTL_H__7C1CB435_C40C_4FD2_81FC_DD844035576A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand7Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand7Ctrl ActiveX Control class.
typedef struct tagSemCtrlFigures{
    VARIANT varMin;
    VARIANT varMax;
	VARIANT varPos;
} SemCtrlFigures;
typedef SemCtrlFigures FAR *LPSemCtrlFigures;

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;

const UINT nIDCheckX = 918;
const UINT nIDCheckY = 888;

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl : See SemCoreCtrlPanel_Stand7Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand7Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand7Ctrl)

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CButton			m_ctrlCheckX;

	CString 		m_strX;
	CString 		m_strY;

	CRect			m_ExpectInvalidRect;

// Constructor
public:
	CSemCoreCtrlPanel_Stand7Ctrl();

	CString FormatDisplay();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand7Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnTextChanged();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand7Ctrl();

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand7Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand7Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand7Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand7Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand7Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckX();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand7Ctrl)
	BOOL m_bCaptionVisible;
	afx_msg void OnCaptionVisibleChanged();
	BOOL m_bDebugInfoVisible;
	afx_msg void OnDebugInfoVisibleChanged();
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
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand7Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand7Ctrl)
	dispidCaptionVisible = 1L,
	dispidDebugInfoVisible = 2L,
	dispidBind = 3L,
	dispidEnable = 4L,
	dispidSetRange = 5L,
	dispidSetPos = 6L,
	dispidSyncPos = 7L,
	dispidSyncText = 8L,
	eventidReportPos = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND7CTL_H__7C1CB435_C40C_4FD2_81FC_DD844035576A__INCLUDED)
