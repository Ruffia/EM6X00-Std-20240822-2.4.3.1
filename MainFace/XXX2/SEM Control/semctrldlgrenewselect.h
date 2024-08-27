// Last Modify : 2011.07.19

///////////////////////////////////////////////////////////////////////////////
// 11.07.19 加入隐含项，全选时可以记录下位机发送的当前未直接记录的消息
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRLDLGRENEWSELECT_H__0A73F557_4554_4664_BD60_0D1F38BD176F__INCLUDED_)
#define AFX_SEMCTRLDLGRENEWSELECT_H__0A73F557_4554_4664_BD60_0D1F38BD176F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrlDlgRenewSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgRenewSelect dialog

class CSemCtrlDlgRenewSelect : public CDialog
{
// Construction
public:
	CSemCtrlDlgRenewSelect(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

// Dialog Data
	//{{AFX_DATA(CSemCtrlDlgRenewSelect)
	enum { IDD = IDD_SEMCTL_RenewSelect };
	CString	m_csMsg;
	BOOL	m_boolHV;
	BOOL	m_boolScanShift;
	BOOL	m_boolBeemAlignment;
	BOOL	m_boolBrightness;
	BOOL	m_boolContrast;
	BOOL	m_boolStigmator;
	BOOL	m_boolObjectiveLens;
	BOOL	m_boolCondenserLens;
	BOOL	m_boolFilamentCurrent;
	BOOL	m_boolMagnifier;
	BOOL	m_boolWorkDistance;
	BOOL	m_boolObjectiveLensReverse;
	BOOL	m_boolCondenserLensReverse;
	BOOL	m_boolAuxiliaryStigmation;
	BOOL	m_boolDCondenserLens;
	BOOL	m_boolScanRotate;
	BOOL	m_boolSACP;
	BOOL	m_boolDFocus;
	BOOL	m_boolDFocusSwitch;
	BOOL	m_boolDObjectiveLens;
	BOOL	m_boolDObjectiveLensSwitch;
	BOOL	m_boolRobinsonDetectorSwitch;
	BOOL	m_boolSignal2Brightness;
	BOOL	m_boolSignal2Contrast;
	BOOL	m_boolYAdjust;
	BOOL	m_boolImageReverse;
	BOOL	m_boolGunCurrent;
	BOOL	m_boolMsgFromMicroPC;
	//}}AFX_DATA

	BOOL	m_boolIsRecord;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCtrlDlgRenewSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSemCtrlDlgRenewSelect)
	afx_msg void OnSelectAll();
	afx_msg void OnClearAll();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRLDLGRENEWSELECT_H__0A73F557_4554_4664_BD60_0D1F38BD176F__INCLUDED_)
