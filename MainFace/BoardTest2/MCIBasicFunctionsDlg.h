#if !defined(AFX_MCIBASICFUNCTIONSDLG_H__B9DC5663_1BF8_49EE_875C_676AA18B4456__INCLUDED_)
#define AFX_MCIBASICFUNCTIONSDLG_H__B9DC5663_1BF8_49EE_875C_676AA18B4456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MCIBasicFunctionsDlg.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CMCIBasicFunctionsDlg dialog

class CMCIBasicFunctionsDlg : public CDialog
{
// Construction
public:
	CMCIBasicFunctionsDlg(CWnd* pParent = NULL);   // standard constructor
	void	UpdateButtonsState( BOOL bEnable );
	unsigned long IObitsMask;

	CStatic *staticIObitsArray[32];
	BOOL	bIObitsStateArray[32];

	void	Output_AfterConfig( unsigned char OutMask );
	void	Output_AfterWrite();

	void	UpdateConfig();
	void	UpdatePortState( unsigned char OutMask );
	void	UpdateListString_OutMask_Config( unsigned char OutMask );
	void	UpdateListString_OutMask_Write( unsigned long OutMask );
	void	UpdateListString_ReadEveryBit();
	void	UpdateListString_ReadEveryPort();

	void	ReadAll();

// Dialog Data
	//{{AFX_DATA(CMCIBasicFunctionsDlg)
	enum { IDD = IDD_MCI_BASIC_FUNCTIONS };
	CMacButton	m_btnConfig;
	CStatic	m_staticUnable;
	CStatic	m_staticRed;
	CStatic	m_staticGreen;
	CStatic	m_staticIObitD7;
	CStatic	m_staticIObitD6;
	CStatic	m_staticIObitD5;
	CStatic	m_staticIObitD4;
	CStatic	m_staticIObitD3;
	CStatic	m_staticIObitD2;
	CStatic	m_staticIObitD1;
	CStatic	m_staticIObitD0;
	CStatic	m_staticIObitC7;
	CStatic	m_staticIObitC6;
	CStatic	m_staticIObitC5;
	CStatic	m_staticIObitC4;
	CStatic	m_staticIObitC3;
	CStatic	m_staticIObitC2;
	CStatic	m_staticIObitC1;
	CStatic	m_staticIObitC0;
	CStatic	m_staticIObitB7;
	CStatic	m_staticIObitB6;
	CStatic	m_staticIObitB5;
	CStatic	m_staticIObitB4;
	CStatic	m_staticIObitB3;
	CStatic	m_staticIObitB2;
	CStatic	m_staticIObitB1;
	CStatic	m_staticIObitB0;
	CStatic	m_staticIObitA7;
	CStatic	m_staticIObitA6;
	CStatic	m_staticIObitA5;
	CStatic	m_staticIObitA4;
	CStatic	m_staticIObitA3;
	CStatic	m_staticIObitA2;
	CStatic	m_staticIObitA1;
	CStatic	m_staticIObitA0;
	CString	m_strPortA;
	CString	m_strPortB;
	CString	m_strPortC;
	CString	m_strPortD;
	int		m_nPortA;
	int		m_nPortB;
	int		m_nPortC;
	int		m_nPortD;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCIBasicFunctionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMCIBasicFunctionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnConfig3();
	afx_msg void OnConfig4();
	afx_msg void OnConfig5();
	afx_msg void OnConfig6();
	afx_msg void OnConfig7();
	afx_msg void OnReadPortA();
	afx_msg void OnWritePortA();
	afx_msg void OnReadPortB();
	afx_msg void OnWritePortB();
	afx_msg void OnReadPortC();
	afx_msg void OnWritePortC();
	afx_msg void OnReadPortD();
	afx_msg void OnWritePortD();
	afx_msg void OnReadAll();
	afx_msg void OnWriteAll();
	afx_msg void OnConfigEx();
	afx_msg void OnRADIOPortCInput();
	afx_msg void OnRADIOPortCOutput();
	afx_msg void OnRADIOPortDInput();
	afx_msg void OnRADIOPortDOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCIBASICFUNCTIONSDLG_H__B9DC5663_1BF8_49EE_875C_676AA18B4456__INCLUDED_)
