// Last Modify : 2009.05.08

#if !defined(AFX_PAGESCANPHOTO_H__DFEC5C5E_089E_451C_BF82_0B3C46158ABC__INCLUDED_)
#define AFX_PAGESCANPHOTO_H__DFEC5C5E_089E_451C_BF82_0B3C46158ABC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageScanPhoto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageScanPhoto dialog

class CPageScanPhoto : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageScanPhoto)

// Construction
public:
	CPageScanPhoto();
	~CPageScanPhoto();
	void	Localize(void);
	CFont	m_Font;

	void	ShowSEratio();

	int		m_nPixRateIndex1Bak, m_nPixRateIndex2Bak, m_nPixRateIndex3Bak, m_nPixRateIndex4Bak;
	int		m_nCameraPixRateIndexbak;
	int		m_nResoX1bak, m_nResoX2bak, m_nResoX3bak, m_nResoX4bak;
	int		m_nChn1bak, m_nChn2bak, m_nSEratiobak;

// Dialog Data
	//{{AFX_DATA(CPageScanPhoto)
	enum { IDD = IDD_3K2_PAGE_SCAN_PHOTO };
	int		m_nPixRateIndex1;
	int		m_nPixRateIndex2;
	int		m_nPixRateIndex3;
	int		m_nPixRateIndex4;
	int		m_nCameraPixRateIndex;
	int		m_nResoX1;
	int		m_nResoX2;
	int		m_nResoX3;
	int		m_nResoX4;
	int		m_nChn1;
	int		m_nChn2;
	int		m_nSEratio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageScanPhoto)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageScanPhoto)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboScanPixRate1();
	afx_msg void OnSelchangeComboScanPixRate2();
	afx_msg void OnSelchangeComboScanPixRate3();
	afx_msg void OnSelchangeComboScanPixRate4();
	afx_msg void OnSelchangeComboResoX1();
	afx_msg void OnSelchangeComboResoX2();
	afx_msg void OnSelchangeComboResoX3();
	afx_msg void OnSelchangeComboResoX4();
	afx_msg void OnSelchangeComboChn1();
	afx_msg void OnSelchangeComboChn2();
	afx_msg void OnSelchangeComboSEratio();
	//}}AFX_MSG
	afx_msg void OnRadioCameraspeed(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESCANPHOTO_H__DFEC5C5E_089E_451C_BF82_0B3C46158ABC__INCLUDED_)
