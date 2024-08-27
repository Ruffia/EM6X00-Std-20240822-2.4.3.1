#if !defined(AFX_SEMCORECTRLPANEL_STAND8CTL_H__2CEC8201_3E31_405F_8F4E_87397CA44CA3__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND8CTL_H__2CEC8201_3E31_405F_8F4E_87397CA44CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand8Ctl.h : Declaration of the CSemCoreCtrlPanel_Stand8Ctrl ActiveX Control class.
#include "ColorCtrl.h"

typedef struct tagSemCtrlFigures{
    VARIANT varMin;
    VARIANT varMax;
	VARIANT varPos;
} SemCtrlFigures;
typedef SemCtrlFigures FAR *LPSemCtrlFigures;

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;

const UINT nIDSpinCtrl		= 900;
const UINT nIDResetCtrl		= 901;
const UINT nIDSaveCtrl		= 902;
const UINT nIDLoadCtrl		= 903;
const UINT nIDParamCtrl		= 904;
const UINT nIDStaticCtrl	= 905;


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl : See SemCoreCtrlPanel_Stand8Ctl.cpp for implementation.

class CSemCoreCtrlPanel_Stand8Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand8Ctrl)

	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CString 		m_strX;
	CString 		m_strY;

	CSpinButtonCtrl		m_ctrlSpin;
	CButton				m_ctrlReset;
	CButton				m_ctrlSave;
	CButton				m_ctrlLoad;
	CButton				m_ctrlParam;

	CFont				m_fontOnSize;
	CColorCtrl<CStatic> m_ctrlStatic;

	int				m_nStepTimer;
	long			m_lSavedPos;
	CString			m_csSettingSection;
	CString			m_szStepTimerEntry;
	CString			m_szSavedPosEntry;

// Constructor
public:
	CSemCoreCtrlPanel_Stand8Ctrl();

	CString GetDisplayString();

	void SetCtrlPos( long lPos );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_Stand8Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_Stand8Ctrl();

	void OnSizeFont(CWnd* pCtrl);

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand8Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_Stand8Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_Stand8Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_Stand8Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand8Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonReset();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonParam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_Stand8Ctrl)
	BOOL m_bCaptionVisible;
	afx_msg void OnCaptionVisibleChanged();
	BOOL m_bDebugInfoVisible;
	afx_msg void OnDebugInfoVisibleChanged();
	CString m_csProgressMessage;
	afx_msg void OnProgressMessageChanged();
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
	//{{AFX_EVENT(CSemCoreCtrlPanel_Stand8Ctrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_Stand8Ctrl)
	dispidCaptionVisible = 1L,
	dispidDebugInfoVisible = 2L,
	dispidProgressMessage = 3L,
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

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND8CTL_H__2CEC8201_3E31_405F_8F4E_87397CA44CA3__INCLUDED)
