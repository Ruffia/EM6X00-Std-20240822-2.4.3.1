#if !defined(AFX_CDlgBeamAperture_H__B2A2837B_27C4_4491_A149_9827B54D0E7B__INCLUDED_)
#define AFX_CDlgBeamAperture_H__B2A2837B_27C4_4491_A149_9827B54D0E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


// CDlgBeamAperture �Ի���

class CDlgBeamAperture : public CDialog
{
	DECLARE_DYNAMIC(CDlgBeamAperture)

public:
	CDlgBeamAperture(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBeamAperture();

	void	Localize();

	int		m_nBeamAper;
	int     m_nBeamAperNum[5];
// �Ի�������
	enum { IDD = IDD_DLG_BeamAperture };
	int m_nSelectAper;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
#endif // !defined(AFX_CDlgBeamAperture_H__B2A2837B_27C4_4491_A149_9827B54D0E7B__INCLUDED_)
