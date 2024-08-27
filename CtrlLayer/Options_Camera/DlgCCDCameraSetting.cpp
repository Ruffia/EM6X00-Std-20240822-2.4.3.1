// DlgCameraSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Options_Camera.h"
#include "DlgCCDCameraSetting.h"
#include "afxdialogex.h"


// CDlgCCDCameraSetting 对话框
IMPLEMENT_DYNAMIC(CDlgCCDCameraSetting, CDialog)

CDlgCCDCameraSetting::CDlgCCDCameraSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCCDCameraSetting::IDD, pParent)
	, m_nBrightness(0)
	, m_nContrast(0)
	, m_nHue(0)
	, m_nGain(0)
{
	m_pVideoProcAmp = NULL;
}

CDlgCCDCameraSetting::~CDlgCCDCameraSetting()
{
}

void CDlgCCDCameraSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_CCD_Brightness, m_scrollBrightness);
	DDX_Control(pDX, IDC_SCROLLBAR_CCD_Contrast, m_scrollContrast);
	DDX_Control(pDX, IDC_SCROLLBAR_CCD_Gain, m_scrollGain);
	DDX_Control(pDX, IDC_SCROLLBAR_CCD_Hue, m_scrollHue);
	DDX_Text(pDX, IDC_EDIT_CCD_Brightness, m_nBrightness);
	DDX_Text(pDX, IDC_EDIT_CCD_Contrast, m_nContrast);
	DDX_Text(pDX, IDC_EDIT_CCD_Gain, m_nGain);
	DDX_Text(pDX, IDC_EDIT_CCD_Hue, m_nHue);
}

BEGIN_MESSAGE_MAP(CDlgCCDCameraSetting, CDialog)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_CCD_Brightness, OnChangeEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_CCD_Contrast, OnChangeEditContrast)
	ON_EN_CHANGE(IDC_EDIT_CCD_Gain, OnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_CCD_Hue, OnChangeEditHue)
	ON_BN_CLICKED(IDC_BUTTON_CCD_Reset, OnButtonCCDReset)
	ON_MESSAGE(WM_SET_CAMERA_DS, OnSetCameraDS)
END_MESSAGE_MAP()

BOOL CDlgCCDCameraSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCCDCameraSetting::Localize()
{
	char szBuf[50];
	int nSize = 50;
	// 界面元素
	if(GetPrivateProfileString("Label", "Cap_Set", "", szBuf, nSize, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "Brt", "", szBuf, nSize, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CCD_Brightness)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "Cst", "", szBuf, nSize, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CCD_Contrast)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "Gain", "", szBuf, nSize, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CCD_Gain)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "Hue", "", szBuf, nSize, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CCD_Hue)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "Reset", "", szBuf, nSize, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_CCD_Reset)->SetWindowText( szBuf );
}

LRESULT CDlgCCDCameraSetting::OnSetCameraDS(WPARAM wParam, LPARAM lParam)
{
	CCameraDS* ds = (CCameraDS*)wParam;
	SetCameraDS( ds );
	return 0;
}

void CDlgCCDCameraSetting::SetCameraDS( CCameraDS* pCameraDS )
{
	if(m_pVideoProcAmp!=NULL)m_pVideoProcAmp->Release();
	if(pCameraDS == NULL)
	{
		m_pVideoProcAmp = NULL;
		return;
	}
	HRESULT hr = S_OK;
	hr = pCameraDS->m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pVideoProcAmp);
	if(!SUCCEEDED(hr)){
		AfxMessageBox(_T("Get settings failed!"));
		ShowWindow(false);
		m_pVideoProcAmp = NULL;
	}

	hr = m_pVideoProcAmp->GetRange(VideoProcAmp_Brightness, &m_RangeBrt.Min, &m_RangeBrt.Max, &m_RangeBrt.Step, &m_RangeBrt.Default, &m_RangeBrt.Flags);
	if (SUCCEEDED(hr)) {
		// 使用 Min 和 Max 作为亮度的范围
		m_scrollBrightness.SetScrollRange(m_RangeBrt.Min, m_RangeBrt.Max);
	}

	hr = m_pVideoProcAmp->GetRange(VideoProcAmp_Contrast, &m_RangeCst.Min, &m_RangeCst.Max, &m_RangeCst.Step, &m_RangeCst.Default, &m_RangeCst.Flags);
	if (SUCCEEDED(hr)) {
		// 使用 Min 和 Max 作为对比度的范围
		m_scrollContrast.SetScrollRange(m_RangeCst.Min, m_RangeCst.Max);
	}

	hr = m_pVideoProcAmp->GetRange(VideoProcAmp_Hue, &m_RangeHue.Min, &m_RangeHue.Max, &m_RangeHue.Step, &m_RangeHue.Default, &m_RangeHue.Flags);
	if (SUCCEEDED(hr)) {
		// 使用 Min 和 Max 作为色调的范围
		m_scrollHue.SetScrollRange(m_RangeHue.Min, m_RangeHue.Max);
	}

	hr = m_pVideoProcAmp->GetRange(VideoProcAmp_Gain, &m_RangeGain.Min, &m_RangeGain.Max, &m_RangeGain.Step, &m_RangeGain.Default, &m_RangeGain.Flags);
	if (SUCCEEDED(hr)) {
		// 使用 Min 和 Max 作为增益的范围
		m_scrollGain.SetScrollRange(m_RangeGain.Min, m_RangeGain.Max);
	}

	long currentBrightness;
	hr = GetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Brightness, currentBrightness);
	if (SUCCEEDED(hr)) {
		m_scrollBrightness.SetScrollPos(currentBrightness);
		m_nBrightness = currentBrightness;
	} 

	long currentContrast;
	hr = GetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Contrast, currentContrast);
	if (SUCCEEDED(hr)) {
		m_scrollContrast.SetScrollPos(currentContrast);
		m_nContrast = currentContrast;
	}

	long currentSaturation;
	hr = GetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Gain, currentSaturation);
	if (SUCCEEDED(hr)) {
		m_scrollGain.SetScrollPos(currentSaturation);
		m_nGain = currentSaturation;
	}
	
	long currentSharpness;
	hr = GetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Hue, currentSharpness);
	if (SUCCEEDED(hr)) {
		m_scrollHue.SetScrollPos(currentSharpness);
		m_nHue = currentSharpness;
	}
	UpdateData(FALSE);
}

HRESULT CDlgCCDCameraSetting::SetCameraParameter(IAMVideoProcAmp *pVideoProcAmp, long Property, long Value)
{
	if(!pVideoProcAmp) return E_POINTER;

	long Flags = VideoProcAmp_Flags_Manual;
	return pVideoProcAmp->Set(Property, Value, Flags);
}

// 获取相机参数
HRESULT CDlgCCDCameraSetting::GetCameraParameter(IAMVideoProcAmp *pVideoProcAmp, long Property, long &Value)
{
	if(!pVideoProcAmp) return E_POINTER;

	long Flags;
	return pVideoProcAmp->Get(Property, &Value, &Flags);
}

void CDlgCCDCameraSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int nOldPos = pScrollBar->GetScrollPos();
	int nNewPos = pScrollBar->GetScrollPos();
	SCROLLINFO  scrollInfo;
	pScrollBar->GetScrollInfo( &scrollInfo, SIF_ALL );
	switch (nSBCode) 
	{
	case SB_LINELEFT:
		nNewPos = nOldPos-1;
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_LINERIGHT:
		nNewPos = nOldPos+1;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_LEFT:
		pScrollBar->SetScrollPos( scrollInfo.nMax );
		break;

	case SB_RIGHT:
		pScrollBar->SetScrollPos( scrollInfo.nMin );
		break;

	case SB_PAGELEFT:
		nNewPos = nOldPos-10;
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_PAGERIGHT:
		nNewPos = nOldPos+10;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_ENDSCROLL:
		break;

	case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;

	case SB_THUMBTRACK:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;
	}

	nNewPos = pScrollBar->GetScrollPos();
	if( nOldPos == nNewPos )
		return;

	int id = pScrollBar->GetDlgCtrlID();	//获取滚动条ID
	switch( id )
	{
	case IDC_SCROLLBAR_CCD_Brightness:
		CCD_Brt_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_CCD_Contrast:
		CCD_Cst_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_CCD_Gain:
		CCD_Gain_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_CCD_Hue:
		CCD_Hue_Change( pScrollBar->GetScrollPos() );
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgCCDCameraSetting::CCD_Brt_Change( int nPos )
{
	// 设置相机的亮度
	m_nBrightness = nPos;
	UpdateData(FALSE);
	m_scrollBrightness.SetScrollPos(nPos);
	HRESULT hr = SetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Brightness, nPos);
	if (FAILED(hr))
		AfxMessageBox("Set brightness failed!");
}

void CDlgCCDCameraSetting::CCD_Cst_Change( int nPos )
{
	// 设置相机的对比度
	m_nContrast = nPos;
	UpdateData(FALSE);
	m_scrollContrast.SetScrollPos(nPos);
	HRESULT hr = SetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Contrast, nPos);
	if (FAILED(hr))
		AfxMessageBox("Set contrast failed!");
}

void CDlgCCDCameraSetting::CCD_Gain_Change( int nPos )
{
	// 设置相机的增益
	m_nGain = nPos;
	UpdateData(FALSE);
	m_scrollGain.SetScrollPos(nPos);
	HRESULT hr = SetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Gain, nPos);
	if (FAILED(hr))
		AfxMessageBox("Set gain failed!");
}

void CDlgCCDCameraSetting::CCD_Hue_Change( int nPos )
{
	// 设置相机的色调
	m_nHue = nPos;
	UpdateData(FALSE);
	m_scrollHue.SetScrollPos(nPos);
	HRESULT hr = SetCameraParameter(m_pVideoProcAmp, VideoProcAmp_Hue, nPos);
	if (FAILED(hr))
		AfxMessageBox("Set hue failed!");
}

void CDlgCCDCameraSetting::CheckValid( int& nValue, CameraParameterRange range )
{
	if( nValue < range.Min )
		nValue = range.Min;
	else if( nValue > range.Max )
		nValue = range.Max;
}

void CDlgCCDCameraSetting::OnChangeEditBrightness()
{
	UpdateData(TRUE);
	CheckValid( m_nBrightness, m_RangeBrt );
	CCD_Brt_Change( m_nBrightness );
}

void CDlgCCDCameraSetting::OnChangeEditContrast()
{
	UpdateData(TRUE);
	CheckValid( m_nContrast, m_RangeCst );
	CCD_Cst_Change( m_nContrast );
}

void CDlgCCDCameraSetting::OnChangeEditGain()
{
	UpdateData(TRUE);
	CheckValid( m_nGain, m_RangeGain );
	CCD_Gain_Change( m_nGain );
}

void CDlgCCDCameraSetting::OnChangeEditHue()
{
	UpdateData(TRUE);
	CheckValid( m_nHue, m_RangeHue );
	CCD_Hue_Change( m_nHue );
}

void CDlgCCDCameraSetting::OnButtonCCDReset()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pVideoProcAmp == NULL )
		return;
	// 更新编辑框变量值
	m_nBrightness	= m_RangeBrt.Default;
	m_nContrast		= m_RangeCst.Default;
	m_nGain			= m_RangeGain.Default;
	m_nHue			= m_RangeHue.Default;
	CCD_Brt_Change( m_nBrightness );
	CCD_Cst_Change( m_nContrast );
	CCD_Gain_Change( m_nGain );
	CCD_Hue_Change( m_nHue );
}
