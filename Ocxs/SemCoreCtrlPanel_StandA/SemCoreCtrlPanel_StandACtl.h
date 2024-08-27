// Last Modify : 2009.02.12

#if !defined(AFX_SEMCORECTRLPANEL_STANDACTL_H__71C53690_D0FE_4F37_A505_017626C68456__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STANDACTL_H__71C53690_D0FE_4F37_A505_017626C68456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_StandACtl.h : Declaration of the CSemCoreCtrlPanel_StandACtrl ActiveX Control class.
#include "ColorCtrl.h"

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;

const UINT nIDStaticCtrl		= 900;

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl : See SemCoreCtrlPanel_StandACtl.cpp for implementation.

class CSemCoreCtrlPanel_StandACtrl : public COleControl
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_StandACtrl)

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

	CString 		m_strX;
	CString 		m_strY;
	CString			m_strCaption;	//09.02.12

	CFont				m_fontOnSize;
	CColorCtrl<CStatic> m_ctrlStatic;

// Constructor
public:
	CSemCoreCtrlPanel_StandACtrl();

	CString GetDisplayString();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanel_StandACtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnTextChanged();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSemCoreCtrlPanel_StandACtrl();

	void OnSizeFont(CWnd* pCtrl);

	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_StandACtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSemCoreCtrlPanel_StandACtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSemCoreCtrlPanel_StandACtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSemCoreCtrlPanel_StandACtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSemCoreCtrlPanel_StandACtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSemCoreCtrlPanel_StandACtrl)
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
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSemCoreCtrlPanel_StandACtrl)
	void FireReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{FireEvent(eventidReportPos,EVENT_PARAM(VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT), &ClassID, &SerialNumber, &Xpos, &Ypos, &Flag);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSemCoreCtrlPanel_StandACtrl)
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

#endif // !defined(AFX_SEMCORECTRLPANEL_STANDACTL_H__71C53690_D0FE_4F37_A505_017626C68456__INCLUDED)
