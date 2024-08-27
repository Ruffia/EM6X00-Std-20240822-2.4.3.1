#pragma once
#include "SemCtrl_AdjustParameter.h"
#include "semcorectrlpanel_stand1.h"

// CTestParameterDlg 对话框

class CTestParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestParameterDlg)

public:
	CTestParameterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestParameterDlg();

// 对话框数据
	enum { IDD = IDD_DLG_TestParameter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnImageparameterApply();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedBtnImagealgoAdjustmentparameterApply();
	afx_msg void OnBnClickedCheckAlgoAvg();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	DECLARE_EVENTSINK_MAP()
public:
	int m_nMaxPix;
	int m_nDiv;	

	BOOL  m_bEnableAlgoAdjust;
	float m_fAlgoAdjust; 
	CSemCoreCtrlPanel_Stand1 m_Ctrl_ImageAlgo_AdjustmentParameter;	
	DWORD m_dwSleepAutoFocus;
	afx_msg void OnBnClickedBtnAutofocussleepparameterApply();
};
