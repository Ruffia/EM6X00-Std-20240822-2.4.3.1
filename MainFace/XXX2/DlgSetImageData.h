#pragma once
#include "resource.h"
#include "afxwin.h"

// CDlgSetImageData 对话框

#define SetImageData_Counts	12

class CDlgSetImageData : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetImageData)

public:
	CDlgSetImageData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetImageData();

	void	Localize(void);
	void	LoadIniData();
	void	LoadDataToPanel();

public:
	double	m_dHV, m_dBeam, m_dFilament;
	double	m_dBrightness, m_dContrast;
	double	m_dCondenserLens, m_dObjectiveLens;
	int		m_nMag;
	int		m_nBeamAlignX, m_nBeamAlignY, m_nStigX, m_nStigY;

	// 对话框数据
	enum { IDD = IDD_DLG_SetImageData };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedLoadData();
	afx_msg void OnBnClickedSaveToVal();
	afx_msg void OnBnClickedChooseVal();
	DECLARE_MESSAGE_MAP()
};
