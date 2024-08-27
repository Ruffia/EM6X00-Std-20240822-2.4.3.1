#if !defined(AFX_DLGANGLEOPTIONS_H__EE20D800_B790_4282_9D31_993F3EDBA415__INCLUDED_)
#define AFX_DLGANGLEOPTIONS_H__EE20D800_B790_4282_9D31_993F3EDBA415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAngleOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAngleOptions dialog

class CDlgAngleOptions : public CDialog
{
// Construction
public:
	CDlgAngleOptions(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

	AngleOption m_AO;
	BOOL		m_bSetDefault;

// Dialog Data
	//{{AFX_DATA(CDlgAngleOptions)
	enum { IDD = IDD_Angle_Options };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAngleOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAngleOptions)
	virtual void OnOK();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnAngleOptionsColorLine();
	afx_msg void OnAngleOptionsColorArc();
	afx_msg void OnAngleOptionsColorText();
	afx_msg void OnAngleOptionsSetDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANGLEOPTIONS_H__EE20D800_B790_4282_9D31_993F3EDBA415__INCLUDED_)
