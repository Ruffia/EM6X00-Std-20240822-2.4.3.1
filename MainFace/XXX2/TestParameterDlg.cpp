// TestParameterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "TestParameterDlg.h"
#include "afxdialogex.h"
#include "ControlLayer.h"
#include "SemVirtualMCICard.h"
#include "SemCtrlPanelManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "TestParameterManager.h"

// CTestParameterDlg 对话框

IMPLEMENT_DYNAMIC(CTestParameterDlg, CDialog)

CTestParameterDlg::CTestParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestParameterDlg::IDD, pParent)
	, m_nMaxPix(2559)
	, m_nDiv(10)
	, m_bEnableAlgoAdjust(FALSE)
	, m_fAlgoAdjust(1.f)
	, m_dwSleepAutoFocus(10)
{

}

CTestParameterDlg::~CTestParameterDlg()
{
}

void CTestParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Image_MaxPix, m_nMaxPix);
	DDV_MinMaxInt(pDX, m_nMaxPix, 1, 2559);
	DDX_Text(pDX, IDC_EDIT_Image_Div, m_nDiv);
	DDV_MinMaxInt(pDX, m_nDiv, 1, 2559);
	DDX_Control(pDX, IDC_STAND1CTRL_IMAGE_ALGO_ADJUSTMENT_PARAMETER, m_Ctrl_ImageAlgo_AdjustmentParameter);
	DDX_Check(pDX, IDC_CHECK_Algo_Avg, m_bEnableAlgoAdjust);
	DDX_Text(pDX, IDC_EDIT_Sleep_AutoFocus, m_dwSleepAutoFocus);
	DDV_MinMaxUInt(pDX, m_dwSleepAutoFocus, 10, 10000);
}


BEGIN_MESSAGE_MAP(CTestParameterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ImageParameter_Apply, &CTestParameterDlg::OnBnClickedBtnImageparameterApply)
	ON_BN_CLICKED(IDCANCEL, &CTestParameterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ImageAlgo_AdjustmentParameter_Apply, &CTestParameterDlg::OnBnClickedBtnImagealgoAdjustmentparameterApply)
	ON_BN_CLICKED(IDC_CHECK_Algo_Avg, &CTestParameterDlg::OnBnClickedCheckAlgoAvg)
	ON_BN_CLICKED(IDC_BUTTON_AutoFocusSleepParameter_Apply, &CTestParameterDlg::OnBnClickedBtnAutofocussleepparameterApply)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTestParameterDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(CTestParameterDlg)
	ON_EVENT(CTestParameterDlg, IDC_STAND1CTRL_IMAGE_ALGO_ADJUSTMENT_PARAMETER, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


// CTestParameterDlg 消息处理程序
BOOL CTestParameterDlg::OnInitDialog()
{
	UpdateData(FALSE);
	CSemCtrlPanelManager::Instance().Regist( &m_Ctrl_ImageAlgo_AdjustmentParameter, SemControlPanelClass_ImageAlgo_AdjustmentParameter );
	return TRUE;
}

void CTestParameterDlg::OnBnClickedBtnImageparameterApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSemVirtualMCICard::Instance().SetParams(15,m_nMaxPix);
	CSemVirtualMCICard::Instance().SetParams(16,m_nDiv);
	OnOK();
}


void CTestParameterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CTestParameterDlg::OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your control notification handler code here
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	m_fAlgoAdjust = Xpos.lVal/10.f;
}

void CTestParameterDlg::OnBnClickedBtnImagealgoAdjustmentparameterApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CTestParameterManager::Instance().m_bEnableAlgoAdjust = m_bEnableAlgoAdjust;
	CTestParameterManager::Instance().m_fAlgoAdjust = m_fAlgoAdjust;
}


void CTestParameterDlg::OnBnClickedCheckAlgoAvg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_Algo_Avg);
	m_bEnableAlgoAdjust = pBtn->GetCheck();
}


void CTestParameterDlg::OnBnClickedBtnAutofocussleepparameterApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CTestParameterManager::Instance().m_dwSleep_AutoFocus = m_dwSleepAutoFocus;
}
