// Last Modify : 2010.01.22

/////////////////////////////////////////////////////////////////////////////
// 09.12.09 ���ӵ���״̬���ܣ��������ٸ�ѹ���۹⾵��ת��ݼ�������ɢ�ķ�����ڿ�ݼ�
// 10.01.22 ����������ȡ���ѶԱȶ�ֵ�����ã�ΪABC��AF������ʵ��
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRLDLGDEBUG_H__F30FEE8F_7FA1_45A1_91A1_73911D372921__INCLUDED_)
#define AFX_SEMCTRLDLGDEBUG_H__F30FEE8F_7FA1_45A1_91A1_73911D372921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCtrlDlgDebug.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgDebug dialog

class CSemCtrlDlgDebug : public CDialog
{
// Construction
public:
	CSemCtrlDlgDebug(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

// Dialog Data
	//{{AFX_DATA(CSemCtrlDlgDebug)
	enum { IDD = IDD_SEMCTL_DEBUG };
	CString	m_strPassword;
	BOOL	m_bDebugHV;
	BOOL	m_bDebugAccel;
	BOOL	m_bDebugVac;
	double	m_dBrightness;
	double	m_dContrast;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCtrlDlgDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CWnd*	m_pParent;
	int		m_nID;

	// Generated message map functions
	//{{AFX_MSG(CSemCtrlDlgDebug)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRLDLGDEBUG_H__F30FEE8F_7FA1_45A1_91A1_73911D372921__INCLUDED_)
