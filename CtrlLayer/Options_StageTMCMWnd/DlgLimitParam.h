#pragma once


// CDlgLimitParam �Ի���

class CDlgLimitParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgLimitParam)

public:
	CDlgLimitParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLimitParam();

	int		m_nLeftLimit[5], m_nRightLimit[5], m_nExchangeLimit[5];
	int		m_nLeftLimitLevel[5],m_nRightLimitLevel[5];
	char	m_cPathLocalize[256];
	void	Localize();

// �Ի�������
	enum { IDD = IDD_DIALOG_LimitParam };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLimitchange();
	afx_msg void OnSelchangeComboAxis();
	afx_msg void OnBnClickedButtonLimitlevelreversal();
};
