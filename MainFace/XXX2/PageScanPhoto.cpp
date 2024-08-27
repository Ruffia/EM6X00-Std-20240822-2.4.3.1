// PageScanPhoto.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2010.05.20

/////////////////////////////////////////////////////////////////////////////
// 10.05.20 将ComboBox的初始化移至Localize中，以便改变语言选项后可正确显示
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "PageScanPhoto.h"
#include "ScanView.h"
#include "ScanParameterManager.h"
#include "DlgPrompt.h"
#include "ControlLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageScanPhoto property page

IMPLEMENT_DYNCREATE(CPageScanPhoto, CPropertyPage)

CPageScanPhoto::CPageScanPhoto() : CPropertyPage(CPageScanPhoto::IDD)
{
	//{{AFX_DATA_INIT(CPageScanPhoto)
	m_nPixRateIndex1 = -1;
	m_nPixRateIndex2 = -1;
	m_nPixRateIndex3 = -1;
	m_nPixRateIndex4 = -1;
	m_nCameraPixRateIndex = -1;
	m_nResoX1 = -1;
	m_nResoX2 = -1;
	m_nResoX3 = -1;
	m_nResoX4 = -1;
	m_nChn1 = -1;
	m_nChn2 = -1;
	m_nSEratio = -1;
	//}}AFX_DATA_INIT
}

CPageScanPhoto::~CPageScanPhoto()
{
	m_Font.DeleteObject();
}

void CPageScanPhoto::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageScanPhoto)
	DDX_CBIndex(pDX, IDC_COMBO_SCAN_SPEED_1, m_nPixRateIndex1);
	DDX_CBIndex(pDX, IDC_COMBO_SCAN_SPEED_2, m_nPixRateIndex2);
	DDX_CBIndex(pDX, IDC_COMBO_SCAN_SPEED_3, m_nPixRateIndex3);
	DDX_CBIndex(pDX, IDC_COMBO_SCAN_SPEED_4, m_nPixRateIndex4);
	DDX_Radio(pDX, IDC_RADIO_CAMERASPEED_1, m_nCameraPixRateIndex);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_ResoX_1, m_nResoX1);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_ResoX_2, m_nResoX2);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_ResoX_3, m_nResoX3);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_ResoX_4, m_nResoX4);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_Chn1, m_nChn1);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_Chn2, m_nChn2);
	DDX_CBIndex(pDX, IDC_PAGE_SCAN_COMBO_SEratio, m_nSEratio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageScanPhoto, CPropertyPage)
	//{{AFX_MSG_MAP(CPageScanPhoto)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SPEED_1, OnSelchangeComboScanPixRate1)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SPEED_2, OnSelchangeComboScanPixRate2)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SPEED_3, OnSelchangeComboScanPixRate3)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SPEED_4, OnSelchangeComboScanPixRate4)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_ResoX_1, OnSelchangeComboResoX1)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_ResoX_2, OnSelchangeComboResoX2)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_ResoX_3, OnSelchangeComboResoX3)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_ResoX_4, OnSelchangeComboResoX4)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_Chn1, OnSelchangeComboChn1)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_Chn2, OnSelchangeComboChn2)
	ON_CBN_SELCHANGE(IDC_PAGE_SCAN_COMBO_SEratio, OnSelchangeComboSEratio)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_CAMERASPEED_1, IDC_RADIO_CAMERASPEED_120, OnRadioCameraspeed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageScanPhoto message handlers

void CPageScanPhoto::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_PAGE_SCAN_STATIC_GROUP_ScanSpeed)->SetWindowText(GetResString(IDS_PAGE_SCAN_STATIC_GROUP_ScanSpeed)); 
	GetDlgItem(IDC_PAGE_SCAN_STATIC_GROUP_PhotoSpeed)->SetWindowText(GetResString(IDS_PAGE_SCAN_STATIC_GROUP_PhotoSpeed)); 
	GetDlgItem(IDC_RADIO_CAMERASPEED_1)->SetWindowText(GetResString(IDS_RADIO_CAMERASPEED_1)); 
	GetDlgItem(IDC_RADIO_CAMERASPEED_40)->SetWindowText(GetResString(IDS_RADIO_CAMERASPEED_40)); 
	GetDlgItem(IDC_RADIO_CAMERASPEED_80)->SetWindowText(GetResString(IDS_RADIO_CAMERASPEED_80)); 
	GetDlgItem(IDC_RADIO_CAMERASPEED_120)->SetWindowText(GetResString(IDS_RADIO_CAMERASPEED_120)); 
	GetDlgItem(IDC_PAGE_SCAN_STATIC_GROUP_Reso)->SetWindowText(GetResString(IDS_PAGE_SCAN_STATIC_GROUP_Reso)); 
	GetDlgItem(IDC_PAGE_SCAN_STATIC_GROUP_Chn)->SetWindowText(GetResString(IDS_PAGE_SCAN_STATIC_GROUP_Chn)); 

	CString str;
	CComboBox* pChn1 = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn1 ) );
	CComboBox* pChn2 = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn2 ) );
	str.Format( "%s+%s", CControlLayer::Instance().m_BrdMCI.strChnName[0], CControlLayer::Instance().m_BrdMCI.strChnName[1] );
	pChn1->ResetContent();
	pChn2->ResetContent();
	pChn1->AddString( CControlLayer::Instance().m_BrdMCI.strChnName[0] );
	pChn1->AddString( CControlLayer::Instance().m_BrdMCI.strChnName[1] );
	pChn1->AddString( str );
	pChn1->SetCurSel( CControlLayer::Instance().m_BrdMCI.nChnIndex[0] );
	pChn2->AddString( CControlLayer::Instance().m_BrdMCI.strChnName[0] );
	pChn2->AddString( CControlLayer::Instance().m_BrdMCI.strChnName[1] );
	pChn2->AddString( str );
	pChn2->SetCurSel( CControlLayer::Instance().m_BrdMCI.nChnIndex[1] );
} 

BOOL CPageScanPhoto::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	Localize();

	m_Font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	SetFont( &m_Font, TRUE );
	CWnd *pw = GetWindow(GW_CHILD);
	while(pw != NULL)
	{   
		pw->SetFont( &m_Font);
		pw = pw->GetWindow(GW_HWNDNEXT);
	}; 

	m_nPixRateIndex1	= m_nPixRateIndex1Bak;
	m_nPixRateIndex2	= m_nPixRateIndex2Bak;
	m_nPixRateIndex3	= m_nPixRateIndex3Bak;
	m_nPixRateIndex4	= m_nPixRateIndex4Bak;
	m_nCameraPixRateIndex	= m_nCameraPixRateIndexbak;

	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		int i;
		CString str;
		// 扫描速度
		CComboBox* pPixRate1 = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_1 ) );
		CComboBox* pPixRate2 = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_2 ) );
		CComboBox* pPixRate3 = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_3 ) );
		CComboBox* pPixRate4 = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_4 ) );
		pPixRate1->ResetContent();
		pPixRate2->ResetContent();
		pPixRate3->ResetContent();
		pPixRate4->ResetContent();

		pPixRate1->AddString( "500 ns" );
		pPixRate2->AddString( "500 ns" );
		pPixRate3->AddString( "500 ns" );
		pPixRate4->AddString( "500 ns" );
		for( i=1; i< CScanParameterManager::Instance().m_nPixRatesIndex[1] +1; i++ )
		{
			str.Format( "%3d us", i );
			pPixRate1->AddString( str );
			pPixRate2->AddString( str );
			pPixRate3->AddString( str );
			pPixRate4->AddString( str );
		}
		pPixRate1->SetCurSel( m_nPixRateIndex1 );
		pPixRate2->SetCurSel( m_nPixRateIndex2 );
		pPixRate3->SetCurSel( m_nPixRateIndex3 );
		pPixRate4->SetCurSel( m_nPixRateIndex4 );

		// 图像分辨率
		CComboBox* pResoX1 = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_1 ) );
		CComboBox* pResoX2 = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_2 ) );
		CComboBox* pResoX3 = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_3 ) );
		CComboBox* pResoX4 = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_4 ) );
		pResoX1->ResetContent();
		pResoX2->ResetContent();
		pResoX3->ResetContent();
		pResoX4->ResetContent();

		for( i = CScanParameterManager::Instance().m_nResosIndex[0]; i < CScanParameterManager::Instance().m_nResosIndex[1]+1; i++ )
		{
			str.Format( "%d", CScanParameterManager::Instance().m_nResos[i] );
			pResoX1->AddString( str );
			pResoX2->AddString( str );
			pResoX3->AddString( str );
			pResoX4->AddString( str );
		}
		pResoX1->SetCurSel( CScanParameterManager::Instance().m_nResoX[4] - CScanParameterManager::Instance().m_nResosIndex[0] );
		pResoX2->SetCurSel( CScanParameterManager::Instance().m_nResoX[5] - CScanParameterManager::Instance().m_nResosIndex[0] );
		pResoX3->SetCurSel( CScanParameterManager::Instance().m_nResoX[6] - CScanParameterManager::Instance().m_nResosIndex[0] );
		pResoX4->SetCurSel( CScanParameterManager::Instance().m_nResoX[7] - CScanParameterManager::Instance().m_nResosIndex[0] );
		m_nResoX1 =  CScanParameterManager::Instance().m_nResoX[4] - CScanParameterManager::Instance().m_nResosIndex[0];
		m_nResoX2 =  CScanParameterManager::Instance().m_nResoX[5] - CScanParameterManager::Instance().m_nResosIndex[0];
		m_nResoX3 =  CScanParameterManager::Instance().m_nResoX[6] - CScanParameterManager::Instance().m_nResosIndex[0];
		m_nResoX4 =  CScanParameterManager::Instance().m_nResoX[7] - CScanParameterManager::Instance().m_nResosIndex[0];

/*		if(  CScanParameterManager::Instance().m_dResoRate < 0 )	// 自定义分辨率
		{
			pResoY1->ShowWindow( SW_SHOW );
			pResoY2->ShowWindow( SW_SHOW );
			pResoY3->ShowWindow( SW_SHOW );
			pResoY4->ShowWindow( SW_SHOW );
		}
		else
		{
			pResoY1->ShowWindow( SW_HIDE );
			pResoY2->ShowWindow( SW_HIDE );
			pResoY3->ShowWindow( SW_HIDE );
			pResoY4->ShowWindow( SW_HIDE );
		}
*/
		// 通道参数
		if( CScanParameterManager::Instance().m_nusbScanType != USB_ST_Stop )
		{
			GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn1 )->EnableWindow( TRUE );
			GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_PAGE_SCAN_COMBO_SEratio )->EnableWindow( TRUE );
		}
		else
		{
			GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn1 )->EnableWindow( FALSE );
			GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn2 )->EnableWindow( FALSE );
			GetDlgItem( IDC_PAGE_SCAN_COMBO_SEratio )->EnableWindow( FALSE );
		}
		// 混合像时的SE比例
		CComboBox* pSEratio = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_SEratio ) );
		pSEratio->ResetContent();
		for( i=0; i<99; i++ )
		{
			str.Format( "%02d", i+1 );
			pSEratio->AddString( str );
		}
		pSEratio->SetCurSel( m_nSEratio );
		ShowSEratio();
	}
	m_nResoX1bak	= m_nResoX1;
	m_nResoX2bak	= m_nResoX2;
	m_nResoX3bak	= m_nResoX3;
	m_nResoX4bak	= m_nResoX4;
	m_nChn1bak		= m_nChn1;
	m_nChn2bak		= m_nChn2;
	m_nSEratiobak	= m_nSEratio;

	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageScanPhoto::ShowSEratio()
{
	int nChn1 = ((CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn1 )))->GetCurSel();
	int nChn2 = ((CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn2 )))->GetCurSel();
	if( nChn1 > 1 || nChn2 > 1 )
	{
		GetDlgItem( IDC_PAGE_SCAN_COMBO_SEratio )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_PAGE_SCAN_STATIC_Chn_SEratio )->ShowWindow( SW_SHOW );
	}
	else
	{
		GetDlgItem( IDC_PAGE_SCAN_COMBO_SEratio )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_PAGE_SCAN_STATIC_Chn_SEratio )->ShowWindow( SW_HIDE );
	}
}

void CPageScanPhoto::OnSelchangeComboScanPixRate1() 
{
	CComboBox* pPixRate = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_1 ) );
	if( pPixRate->GetCurSel() != m_nPixRateIndex1 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboScanPixRate2() 
{
	CComboBox* pPixRate = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_2 ) );
	if( pPixRate->GetCurSel() != m_nPixRateIndex2 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboScanPixRate3() 
{
	CComboBox* pPixRate = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_3 ) );
	if( pPixRate->GetCurSel() != m_nPixRateIndex3 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboScanPixRate4() 
{
	CComboBox* pPixRate = (CComboBox*)(GetDlgItem( IDC_COMBO_SCAN_SPEED_4 ) );
	if( pPixRate->GetCurSel() != m_nPixRateIndex4 )
		SetModified();
}

void CPageScanPhoto::OnRadioCameraspeed(UINT nID) 
{
	// TODO: Add your control notification handler code here
	if( m_nCameraPixRateIndex+4 != nID - IDC_RADIO_CAMERASPEED_1 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboResoX1() 
{
	CComboBox* pReso = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_1 ) );
	if( pReso->GetCurSel() != m_nResoX1 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboResoX2() 
{
	CComboBox* pReso = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_2 ) );
	if( pReso->GetCurSel()+4 != m_nResoX2 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboResoX3() 
{
	CComboBox* pReso = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_3 ) );
	if( pReso->GetCurSel()+4 != m_nResoX3 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboResoX4() 
{
	CComboBox* pReso = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_ResoX_4 ) );
	if( pReso->GetCurSel()+4 != m_nResoX4 )
		SetModified();
}

void CPageScanPhoto::OnSelchangeComboChn1() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pIndex = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn1 ) );
	if( pIndex->GetCurSel() != m_nChn1 )
		SetModified();
	ShowSEratio();
}

void CPageScanPhoto::OnSelchangeComboChn2() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pIndex = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_Chn2 ) );
	if( pIndex->GetCurSel() != m_nChn2)
		SetModified();
	ShowSEratio();
}

void CPageScanPhoto::OnSelchangeComboSEratio() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pIndex = (CComboBox*)(GetDlgItem( IDC_PAGE_SCAN_COMBO_SEratio ) );
	if( pIndex->GetCurSel() != m_nSEratio)
		SetModified();
}

void CPageScanPhoto::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( (m_nPixRateIndex1 == m_nPixRateIndex1Bak)
		&& (m_nPixRateIndex2 == m_nPixRateIndex2Bak)
		&& (m_nPixRateIndex3 == m_nPixRateIndex3Bak)
		&& (m_nPixRateIndex4 == m_nPixRateIndex4Bak)
		&& (m_nCameraPixRateIndex == m_nCameraPixRateIndexbak)
		&& (m_nResoX1 == m_nResoX1bak)
		&& (m_nResoX2 == m_nResoX2bak)
		&& (m_nResoX3 == m_nResoX3bak)
		&& (m_nResoX4 == m_nResoX4bak)
		&& (m_nChn1 == m_nChn1bak)
		&& (m_nChn2 == m_nChn2bak)
		&& (m_nSEratio == m_nSEratiobak) )
	{
		SetModified( FALSE );
		return;
	}

	m_nPixRateIndex1Bak = m_nPixRateIndex1;
	m_nPixRateIndex2Bak = m_nPixRateIndex2;
	m_nPixRateIndex3Bak = m_nPixRateIndex3;
	m_nPixRateIndex4Bak = m_nPixRateIndex4;
	m_nCameraPixRateIndexbak = m_nCameraPixRateIndex;
	m_nResoX1bak	= m_nResoX1;
	m_nResoX2bak	= m_nResoX2;
	m_nResoX3bak	= m_nResoX3;
	m_nResoX4bak	= m_nResoX4;
	m_nChn1bak		= m_nChn1;
	m_nChn2bak		= m_nChn2;
	m_nSEratiobak	= m_nSEratio;

	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
		pView->SendMessage(WM_PARAM_CHANGED, (WPARAM)this, 0);
	CPropertyPage::OnOK();
}
