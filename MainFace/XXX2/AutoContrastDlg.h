#pragma once
#include "SemCtrl_AdjustParameter.h"
#include "semcorectrlpanel_stand1.h"
#include "afxwin.h"


// CAutoContrastDlg 对话框

class CAutoContrastDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoContrastDlg)

public:
	CAutoContrastDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoContrastDlg();

// 对话框数据
	enum { IDD = IDD_DLG_AutoContrast };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CScrollBar m_Scrollbar_Brightness;
	CScrollBar m_Scrollbar_Contrast;
	CScrollBar m_Scrollbar_Gamma;
	int		m_min;
	int		m_max;
	int		m_nTitleIndex;
	BOOL	bOptionstate;
	BOOL	bstateA;
	BOOL	bstate;
	CRect	rect;
	
	afx_msg void OnBnClickedButtonAbc();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonAutobc();
	afx_msg void OnBnClickedButtonGradient();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	void    HistogramShow(float *Matrix ,int nCounts);
	void	Localize();
	//void	Localize(void);
};
