#if !defined(AFX_SEMCORECTRLPANEL_STAND5CTL_H__9D887688_10F0_41AD_88E1_88A7B76F6C0D__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND5CTL_H__9D887688_10F0_41AD_88E1_88A7B76F6C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand5Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand5Ctrl ActiveX Control class.
typedef struct tagSemCtrlFigures{
    VARIANT varMin;
    VARIANT varMax;
	VARIANT varPos;
} SemCtrlFigures;
typedef SemCtrlFigures FAR *LPSemCtrlFigures;

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl : See SemCoreCtrlPanel_Stand5Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand5Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand5Ctrl)

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	CString 		m_strX;
	CString 		m_strY;

	C3DMeterCtrl		m_ctrl3DMeter;

// Constructor
public:
	CSemCoreCtrlPanel_Stand5Ctrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand5Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnTextChanged();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand5Ctrl();

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand5Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand5Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand5Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand5Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand5Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand5Ctrl)
	short m_nControlStyle;
	afx_msg void OnControlStyleChanged();
	short m_nScaleDecimals;
	afx_msg void OnScaleDecimalsChanged();
	short m_nValueDecimals;
	afx_msg void OnValueDecimalsChanged();
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
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand5Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand5Ctrl)
	dispidControlStyle = 1L,
	dispidScaleDecimals = 2L,
	dispidValueDecimals = 3L,
	dispidBind = 4L,
	dispidEnable = 5L,
	dispidSetRange = 6L,
	dispidSetPos = 7L,
	dispidSyncPos = 8L,
	dispidSyncText = 9L,
	eventidReportPos = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND5CTL_H__9D887688_10F0_41AD_88E1_88A7B76F6C0D__INCLUDED)
