#pragma once


// CDlgLimitParam 对话框

class CDlgLimitParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgLimitParam)

public:
	CDlgLimitParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLimitParam();

	int		m_nLeftLimit[5], m_nRightLimit[5], m_nExchangeLimit[5];
	int		m_nLeftLimitLevel[5],m_nRightLimitLevel[5];
	char	m_cPathLocalize[256];
	void	Localize();

// 对话框数据
	enum { IDD = IDD_DIALOG_LimitParam };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLimitchange();
	afx_msg void OnSelchangeComboAxis();
	afx_msg void OnBnClickedButtonLimitlevelreversal();
};
