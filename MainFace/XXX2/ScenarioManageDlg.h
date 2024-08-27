#pragma once
#include "Scenario.h"

// ScenarioManageDlg 对话框
class ScenarioManageDlg : public CDialog
{
	DECLARE_DYNAMIC(ScenarioManageDlg)

public:
	ScenarioManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ScenarioManageDlg();

// 对话框数据
	enum { IDD = IDD_DLG_Scenario };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnScenarioLoad();
	afx_msg void OnBnClickedBtnScenarioSave();
	afx_msg void OnBnClickedBtnScenarioApply();
	void _UpdateUIFromScenario();
	void _Apply_HV();
	void _Apply_Bias();
	void _Apply_FilamentCurrent(); 
	void _Apply_Brightness(); 
	void _Apply_Contrast();
	void _Apply_ConsenderLens();
	void _Apply_ObjectiveLens();
	void _Apply_Magnifier();
	void _Apply_BeemAlignment();
	void _Apply_Stigmator();
	void Localize();

	DECLARE_MESSAGE_MAP()
public:
	//用当前的高压,灯丝电流等数值初始化对话框界面
	void Initialize();

public:
	CScenario m_Scenario;

protected:
	CString m_strSampleName;
	CString m_strAuthorName;
	CString m_strCreateTime;
	CString m_strDescription;
	CString m_strHVType;
	double m_dHV;
	double m_dFilamentCurrent;
	double m_dBias;
	double m_dBrightness;
	double m_dContrast;
	double m_dConsenderLens;
	double m_ObjectiveLens;
	double m_dMagnifier;
	double m_dBeemAlignmentX;
	double m_dBeemAlignmentY;
	double m_dStigmatorX;
	double m_dStigmatorY;
public:
	afx_msg void OnBnClickedDlgOk();
};
