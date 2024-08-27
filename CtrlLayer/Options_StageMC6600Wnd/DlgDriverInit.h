#if !defined(AFX_DLGDRIVERINIT_H__B2A3837B_27C4_4491_A149_9827B54D0E7B__INCLUDED_)
#define AFX_DLGDRIVERINIT_H__B2A3837B_27C4_4491_A149_9827B54D0E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDriverInit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDriverInit dialog

class CDlgDriverInit : public CDialog
{
// Construction
public:
	CDlgDriverInit(CWnd* pParent = NULL);   // standard constructor
	~CDlgDriverInit();
	void	Localize();

	int		m_nPort;
	unsigned char    m_nAxial;
	int     m_nAddress;
	char	m_cPathLocalize[256];
	int		m_nSetMotorType[5],m_nControlMode[5], m_nControlSignal[5];
	BOOL	m_bRecordMotorStatus[5];
	int		m_nEncoderLines[5], m_nMaxCurrent[5];
	float	m_fDefaultP[5], m_fDefaultI[5], m_fDefaultD[5];
	float	m_fCurP[5],m_fCurI[5],m_fCurD[5];
	BOOL	m_bSoftLimit;
	MC6600_Params_Style	m_Style;

	void    SetMotorType(unsigned char ucAxis,BYTE M);
	void    SetEncoderLines(unsigned char ucAxis,int M);
	void    SetControlSignal(  unsigned char ucAxis ,BYTE M );
	void	SetControlMode(  unsigned char ucAxis ,BYTE M );
	void    SetMaxCurrent(  unsigned char ucAxis ,int M );
	void    ModifyP(short anxi);
	void    ModifyI(short anxi);
	void    ModifyD(short anxi);
	void    SetDefaultPID(int ID,short anxi);

// Dialog Data
	//{{AFX_DATA(CDlgDriverInit)
	enum { IDD = IDD_DIALOG_Initi };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDriverInit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDriverInit)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonAaseting();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonP();
	afx_msg void OnBnClickedButtonI();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonSlimit();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDRIVERINIT_H__B2A3837B_27C4_4491_A149_9827B54D0E7B__INCLUDED_)
