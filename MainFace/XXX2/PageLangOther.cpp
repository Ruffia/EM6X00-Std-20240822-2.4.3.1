// PageLangOther.cpp : implementation file
//
// Last Modify : 2010.07.02

/////////////////////////////////////////////////////////////////////////////
// 10.07.02 增加优先存储格式
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "PageLangOther.h"
#include "ScanView.h"
#include "SheetSetup.h"
#include "DlgPrompt.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "VACDataManager.h"
#include "CCPS3603tatusDetector.h"
#include "HVPowerDataManager.h"
#include "CommonFunctionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageLangOther property page

IMPLEMENT_DYNCREATE(CPageLangOther, CPropertyPage)

CPageLangOther::CPageLangOther() : CPropertyPage(CPageLangOther::IDD)
{
	//{{AFX_DATA_INIT(CPageLangOther)
	m_nPortHV = -1;
	m_nPortCCG = -1;
	m_nPortIPG = -1;
	//}}AFX_DATA_INIT
	m_nPortHVbak = -1;
	m_nPortCCGbak = -1;
	m_nPortIPGbak = -1;
}

CPageLangOther::~CPageLangOther()
{
	m_Font.DeleteObject();
}

void CPageLangOther::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageLangOther)
	DDX_CBIndex(pDX, IDC_COMBO_Debug_IPG_Port, m_nPortIPG);
	DDX_CBIndex(pDX, IDC_COMBO_Debug_CCG_Port, m_nPortCCG);
	DDX_CBIndex(pDX, IDC_COMBO_Debug_HVPower_Port, m_nPortHV);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageLangOther, CPropertyPage)
	//{{AFX_MSG_MAP(CPageLangOther)
	ON_BN_CLICKED(IDC_PAGE_LANG_CHECK_OpenImageAfterSnap, OnPageLangCheckOpenImageAfterSnap)
	ON_BN_CLICKED(IDC_CHECK_Debug_LogAll, OnCHECKDebugLogAll)
	ON_CBN_SELCHANGE(IDC_COMBO_Debug_HVPower_Port, OnSelchangeCOMBODebugCPS3603Port)
	ON_CBN_SELCHANGE(IDC_COMBO_Debug_CCG_Port, OnSelchangeCOMBODebugCCGPort)
	ON_CBN_SELCHANGE(IDC_COMBO_Debug_IPG_Port, OnSelchangeCOMBODebugIPGPort)
	ON_BN_CLICKED(IDC_BUTTON_WriteOneKey, OnWriteOneKey)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_Default, OnBtnDefault)
	ON_BN_CLICKED(IDC_CHECK_SwScrMag, OnCHECKSwScrMag)
	ON_BN_CLICKED(IDC_BUTTON_ShowCameraImage, OnCHECKOpenCCD)
	ON_BN_CLICKED(IDC_CHECK_SwDate, OnCHECKSwDate)
	ON_BN_CLICKED(IDC_CHECK_SwNote, OnCHECKSwNote)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_PAGE_LANG_RADIO_CHS, IDC_PAGE_LANG_RADIO_RUS, OnRadioLanguage)
	ON_COMMAND_RANGE(IDC_PAGE_LANG_RADIO_Prior_bmp, IDC_PAGE_LANG_RADIO_Prior_sem, OnRadioPriorSaveType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageLangOther message handlers

void CPageLangOther::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_PAGE_LANG_STATIC_GROUP_Lang)->SetWindowText(GetResString(IDS_PAGE_LANG_STATIC_GROUP_Lang)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_CHS)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_CHS)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_ENG)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_ENG)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_RUS)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_RUS)); 

	GetDlgItem(IDC_PAGE_LANG_STATIC_GROUP_Other)->SetWindowText(GetResString(IDS_PAGE_LANG_STATIC_GROUP_Other)); 
	GetDlgItem(IDC_PAGE_LANG_CHECK_OpenImageAfterSnap)->SetWindowText(GetResString(IDS_PAGE_LANG_CHECK_OpenImageAfterSnap)); 
	GetDlgItem(IDC_CHECK_Debug_LogAll)->SetWindowText(GetResString( IDS_CHECK_Debug_LogAll ));
	GetDlgItem(IDC_CHECK_SwScrMag)->SetWindowText(GetResString( IDS_CHECK_SwScrMag ));

	GetDlgItem(IDC_PAGE_LANG_STATIC_GROUP_Prior)->SetWindowText(GetResString(IDS_PAGE_LANG_STATIC_GROUP_Prior)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_sem)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_sem)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_bmp)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_bmp)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_gif)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_gif)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_jpg)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_jpg)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_png)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_png)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_mng)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_mng)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_ico)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_ico)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_tif)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_tif)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_tga)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_tga)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_pcx)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_pcx)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_jbg)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_jbg)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_jp2)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_jp2)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_jpc)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_jpc)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_pgx)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_pgx)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_ras)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_ras)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_pnm)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_pnm)); 
	GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_ska)->SetWindowText(GetResString(IDS_PAGE_LANG_RADIO_Prior_ska)); 

	GetDlgItem(IDC_BUTTON_WriteOneKey)->SetWindowText(GetResString( IDS_BUTTON_WriteOneKey ));
	GetDlgItem(IDC_PAGE_SYS_Color_Btn_Default)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Btn_Default)); 
	GetDlgItem(IDS_COMBO_Debug_HVPower_Port)->SetWindowText(GetResString( IDS_COMBO_Debug_HVPower_Port ));
	if( CSemCtrl::Instance().m_nVACBoardType < 1 )
		GetDlgItem(IDS_COMBO_Debug_CCG_Port)->SetWindowText(GetResString( IDS_COMBO_Debug_CCG_Port ));
	else
		GetDlgItem(IDS_COMBO_Debug_CCG_Port)->SetWindowText(GetResString( IDS_COMBO_Debug_Board_Port ));
	if( CSemCtrl::Instance().m_nVACBoardType > 1 )
		GetDlgItem(IDS_COMBO_Debug_IPG_Port)->SetWindowText(GetResString( IDS_COMBO_Debug_IPG_Port ));

	GetDlgItem(IDC_CHECK_SwDate)->SetWindowText(GetResString( IDS_CHECK_SwDate ));
	GetDlgItem(IDC_CHECK_SwNote)->SetWindowText(GetResString( IDS_CHECK_SwNote ));
	GetDlgItem(IDC_BUTTON_ShowCameraImage)->SetWindowText(GetResString( IDS_BUTTON_ShowCameraImage ));
} 

BOOL CPageLangOther::OnInitDialog() 
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

	if( m_nLanguage > 1 )
	{
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_CHS )))->SetCheck(0);
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_ENG )))->SetCheck(0);
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_RUS )))->SetCheck(1);
	}
	else if( m_nLanguage > 0 )
	{
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_CHS )))->SetCheck(0);
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_ENG )))->SetCheck(1);
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_RUS )))->SetCheck(0);
	}
	else
	{
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_CHS )))->SetCheck(1);
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_ENG )))->SetCheck(0);
		((CButton*)(GetDlgItem( IDC_PAGE_LANG_RADIO_RUS )))->SetCheck(0);
	}
	((CButton*)(GetDlgItem( IDC_PAGE_LANG_CHECK_OpenImageAfterSnap)))->SetCheck( m_bOpenImageAfterSnap );
	((CButton*)(GetDlgItem( IDC_CHECK_Debug_LogAll)))->SetCheck( m_bLogAll );
	((CButton*)(GetDlgItem( IDC_CHECK_SwScrMag)))->SetCheck( m_bShowScreenMag );
	((CButton*)(GetDlgItem( IDC_CHECK_SwDate)))->SetCheck( m_bShowDate );
	((CButton*)(GetDlgItem( IDC_CHECK_SwNote)))->SetCheck( m_bShowNote );
	((CButton*)(GetDlgItem( IDC_BUTTON_ShowCameraImage)))->SetCheck( m_bShowCCD );
	for(int i=0; i<IDC_PAGE_LANG_RADIO_Prior_sem-IDC_PAGE_LANG_RADIO_Prior_bmp+1; i++ )
		((CButton*)(GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_bmp +i)))->SetCheck(0);
	((CButton*)(GetDlgItem(IDC_PAGE_LANG_RADIO_Prior_bmp +m_nPriorSaveType)))->SetCheck(1);
	m_nLanguageBak				= m_nLanguage;
	m_bOpenImageAfterSnapBak	= m_bOpenImageAfterSnap;
	m_nPriorSaveTypeBak			= m_nPriorSaveType;
	m_bLogAllBak				= m_bLogAll;
	m_bShowScreenMagBak			= m_bShowScreenMag;
	m_bShowDateBak				= m_bShowDate;
	m_bShowNoteBak				= m_bShowNote;
	m_bShowCCDBak				= m_bShowCCD;

	CComboBox* pPortHV = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_HVPower_Port)));
	if( pPortHV != NULL )
	{
		CString str;
		for(int i=0; i<30; i++ )
		{
			str.Format( "COM %d", i+1 );
			pPortHV->AddString( str );
		}
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			pPortHV->SetCurSel( CHVPowerDataManager::Instance().m_CPS3603s.nPort );
			m_nPortHV		= CHVPowerDataManager::Instance().m_CPS3603s.nPort;
		}
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			pPortHV->SetCurSel( theApp.m_cfgWSM30KV.nPort -1 );
			m_nPortHV		= theApp.m_cfgWSM30KV.nPort -1;
		}
		m_nPortHVbak	= m_nPortHV;
	}
	CComboBox* pPortCCG = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_CCG_Port)));
	if( pPortCCG != NULL )
	{
		CString str;
		for(int i=0; i<10; i++ )
		{
			str.Format( "COM %d", i+1 );
			pPortCCG->AddString( str );
		}
		switch( CSemCtrl::Instance().m_nVACBoardType )
		{
		case	21:
		case	22:
			GetDlgItem(IDS_COMBO_Debug_CCG_Port)->ShowWindow(SW_HIDE);
			pPortCCG->ShowWindow(SW_HIDE);
			break;
		case	0:
			m_nPortCCG		= CVACDataManager::Instance().m_cfgVacCCG.nPort -1;
			break;
		default:
			m_nPortCCG		= CVACDataManager::Instance().m_portBoard.nPort -1;
			break;
		}
		pPortCCG->SetCurSel( m_nPortCCG );
		m_nPortCCGbak	= m_nPortCCG;
	}
	if( theApp.m_bHasCCD )
		((CButton*)(GetDlgItem( IDC_BUTTON_ShowCameraImage)))->ShowWindow(SW_SHOW);
	else
		((CButton*)(GetDlgItem( IDC_BUTTON_ShowCameraImage)))->ShowWindow(SW_HIDE);

	if( CHVPowerDataManager::Instance().m_dwHVtype == 1 )
	{
		GetDlgItem(IDS_COMBO_Debug_HVPower_Port)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_Debug_HVPower_Port)->ShowWindow(SW_HIDE);
	}

	// IPG
	if( CSemCtrl::Instance().m_nVACBoardType > 1 && CSemCtrl::Instance().m_nVACBoardType < 10 )
	{
		GetDlgItem(IDC_COMBO_Debug_IPG_Port)->ShowWindow(SW_SHOW);
		GetDlgItem(IDS_COMBO_Debug_IPG_Port)->ShowWindow(SW_SHOW);
		CComboBox* pPortIPG = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_IPG_Port)));
		if( pPortIPG != NULL )
		{
			CString str;
			for(int i=0; i<10; i++ )
			{
				str.Format( "COM %d", i+1 );
				pPortIPG->AddString( str );
			}
			m_nPortIPG		= CVACDataManager::Instance().m_cfgVacIPG.nPort -1;
			pPortIPG->SetCurSel( m_nPortIPG );
			m_nPortIPGbak	= m_nPortIPG;
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_Debug_IPG_Port)->ShowWindow(SW_HIDE);
		GetDlgItem(IDS_COMBO_Debug_IPG_Port)->ShowWindow(SW_HIDE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageLangOther::OnRadioLanguage(UINT nID) 
{
	// TODO: Add your control notification handler code here
	m_nLanguage = nID - IDC_PAGE_LANG_RADIO_CHS;
	if( m_nLanguage != m_nLanguageBak )
		SetModified();
}

void CPageLangOther::OnPageLangCheckOpenImageAfterSnap() 
{
	// TODO: Add your control notification handler code here
	m_bOpenImageAfterSnap = ((CButton*)(GetDlgItem(IDC_PAGE_LANG_CHECK_OpenImageAfterSnap)))->GetCheck();
	if( m_bOpenImageAfterSnap != m_bOpenImageAfterSnapBak )
		SetModified();
}

void CPageLangOther::OnCHECKDebugLogAll() 
{
	// TODO: Add your control notification handler code here
	m_bLogAll = ((CButton*)(GetDlgItem(IDC_CHECK_Debug_LogAll)))->GetCheck();
	if( m_bLogAll != m_bLogAllBak )
		SetModified();
}

void CPageLangOther::OnCHECKSwScrMag() 
{
	// TODO: Add your control notification handler code here
	m_bShowScreenMag = ((CButton*)(GetDlgItem(IDC_CHECK_SwScrMag)))->GetCheck();
	if( m_bShowScreenMag != m_bShowScreenMagBak )
		SetModified();
}

void CPageLangOther::OnCHECKSwDate() 
{
	// TODO: Add your control notification handler code here
	m_bShowDate = ((CButton*)(GetDlgItem(IDC_CHECK_SwDate)))->GetCheck();
	if( m_bShowDate != m_bShowDateBak )
		SetModified();
}

void CPageLangOther::OnCHECKSwNote() 
{
	// TODO: Add your control notification handler code here
	m_bShowNote = ((CButton*)(GetDlgItem(IDC_CHECK_SwNote)))->GetCheck();
	if( m_bShowNote != m_bShowNoteBak )
		SetModified();
}

void CPageLangOther::OnCHECKOpenCCD() 
{
	// TODO: Add your control notification handler code here
	m_bShowCCD = ((CButton*)(GetDlgItem(IDC_BUTTON_ShowCameraImage)))->GetCheck();
	if( m_bShowCCD != m_bShowCCDBak )
		SetModified();
}

void CPageLangOther::OnRadioPriorSaveType(UINT nID) 
{
	// TODO: Add your control notification handler code here
	m_nPriorSaveType = nID - IDC_PAGE_LANG_RADIO_Prior_bmp;
	if( m_nPriorSaveType != m_nPriorSaveTypeBak )
		SetModified();
/*	if( m_nPriorSaveType != IDC_PAGE_LANG_RADIO_Prior_sem - IDC_PAGE_LANG_RADIO_Prior_bmp -1 )
	{
		CString strPrompt, strTitle;
		strPrompt = GetResString( IDS_SD_PROMPT_DOSAVE1 );
		strTitle = GetResString( IDS_SD_PROMPT_DOSAVE_TITLE );
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x04;
		dlg.m_strMsg = strPrompt;
		dlg.m_strTitle = strTitle;
		dlg.DoModal();
	}*/
}

void CPageLangOther::OnSelchangeCOMBODebugCPS3603Port() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pPortHV = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_HVPower_Port)));
	if( pPortHV->GetCurSel() != m_nPortHV )
		SetModified();
}

void CPageLangOther::OnSelchangeCOMBODebugCCGPort() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pPortCCG = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_CCG_Port)));
	if( pPortCCG->GetCurSel() != m_nPortCCG )
		SetModified();
}

void CPageLangOther::OnSelchangeCOMBODebugIPGPort() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pPortIPG = ((CComboBox*)(GetDlgItem(IDC_COMBO_Debug_IPG_Port)));
	if( pPortIPG->GetCurSel() != m_nPortIPG )
		SetModified();
}

void CPageLangOther::OnWriteOneKey() 
{
	// 将当前参数值写入配置文件，作为一键看像时的默认值
	// [OneKEY]
	// HV,Bias,Fila,B,C,CL,OL,Mag
	// param=30000,0,190,-48,144,15360,15360,1000
	int	nB		= CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal;
	int	nCL		= CSemCoreAssemble::Instance().m_CondenserLens.GetPos().lVal;
	int	nOL		= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	int	nMag	= (int)(CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal);
	int nC, nHV, nBias, nFila, nFilaDefault = 190;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		nC		= CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal;
		CCPS3603tatusDetector::Instance().GetParam( 10, nHV );
		CCPS3603tatusDetector::Instance().GetParam( 11, nBias );
		CCPS3603tatusDetector::Instance().GetParam( 12, nFila );
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			nC		= CSemCoreAssemble::Instance().m_SP_PMT.GetPos().lVal;
			if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			{
				nHV		= (int)(theApp.m_setWSM30KV2.fACLv *1000);	// unit: V
				nBias	= (int)(theApp.m_setWSM30KV2.fACLi);
				nFila	= (int)(theApp.m_setWSM30KV2.fFILi *100);	// 1.90A，存时为1.90*100=190
			}
			else
			{
				nHV		= (int)(theApp.m_setWSM30KV.fACLv *1000);	// unit: V
				nBias	= (int)(theApp.m_setWSM30KV.fACLi);
				nFila	= (int)(theApp.m_setWSM30KV.fFILi *100);	// 1.90A，存时为1.90*100=190
			}
		}
		else
		{
			nC		= CSemCoreAssemble::Instance().m_SP_PMT.GetPos().lVal;
			nHV		= (int)(CHVPowerDataManager::Instance().m_Spellmans.pS.dVE *1000);	// unit: V
			nBias	= (int)(CHVPowerDataManager::Instance().m_Spellmans.pS.dIB);
			nFila	= (int)(CHVPowerDataManager::Instance().m_Spellmans.pS.dIH *100);	// 1.90A，存时为1.90*100=190
		}
	}
	// 威思曼新电源的灯丝电流，不做限制，加的时候直接加到位；SPM电源待实验确认
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		if( nFila < 160 )
			nFila = nFilaDefault;
		if( nFila > 200 )
			nFila = nFilaDefault;
	}

	CString strParam = "";
	CDlgPrompt dlg;
	dlg.m_byteType = (BYTE)0x04;
	dlg.m_strMsg = "Save CL/OL/Mag parameter also?";
	int nRet = dlg.DoModal();
	switch( nRet )
	{
	case	IDYES:
		// 存所有当前参数
		strParam.Format( "%d,%d,%d,%d,%d,%d,%d,%d", nHV, nBias, nFila, nB, nC, nCL, nOL, nMag );
		break;
	case	IDNO:
		// 只存HV,Bias,Fila,B,C
		strParam.Format( "%d,%d,%d,%d,%d,15360,15360,1000", nHV, nBias, nFila, nB, nC );
		break;
	default:
		break;
	}
	if( strParam.IsEmpty() )
		return;

	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	// 写入配置文件
	WritePrivateProfileString( "OneKEY", "param", strParam, path );

	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 4;
	int*		pCheck	= new int[4];
	sprintf(stabuff, "Auto");
	sprintf(staitembuff, "Fila");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
// 	int nMin	= pCheck[0];
// 	int nMax	= pCheck[1];
// 	int	nStep	= pCheck[2];
// 	int	nDebug	= pCheck[3];
	pCheck[0]	= nFila;
	CCommonFunctionManager::Instance().SaveInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	delete [] pCheck;
}

void CPageLangOther::OnBtnDefault() 
{
	// 恢复一键看像默认参数
	CString strParam;
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		strParam.Format( "30000,0,190,-48,126,15360,15360,1000" );
	else
		strParam.Format( "30000,120,190,-48,200,15360,15360,1000" );
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	// 写入配置文件
	WritePrivateProfileString( "OneKEY", "param", strParam, path );
	
	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 4;
	int*		pCheck	= new int[4];
	sprintf(stabuff, "Auto");
	sprintf(staitembuff, "Fila");
	CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
// 	int nMin	= pCheck[0];
// 	int nMax	= pCheck[1];
// 	int	nStep	= pCheck[2];
// 	int	nDebug	= pCheck[3];
	pCheck[0]	= 190;
	CCommonFunctionManager::Instance().SaveInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	delete [] pCheck;
}

void CPageLangOther::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( (m_nLanguage == m_nLanguageBak)
		&& (m_bOpenImageAfterSnap == m_bOpenImageAfterSnapBak)
		&& (m_bLogAll == m_bLogAllBak)
		&& (m_bShowScreenMag == m_bShowScreenMagBak)
		&& (m_bShowDate == m_bShowDateBak)
		&& (m_bShowNote == m_bShowNoteBak)
		&& (m_bShowCCD == m_bShowCCDBak)
		&& (m_nPriorSaveType == m_nPriorSaveTypeBak)
		&& (m_nPortHV == m_nPortHVbak)
		&& (m_nPortCCG == m_nPortCCGbak)
		&& (m_nPortIPG == m_nPortIPGbak) )
	{
		SetModified( FALSE );
		return;
	}

	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView )
		pView->SendMessage(WM_PARAM_CHANGED, (WPARAM)this, 1);

	CSheetSetup* pSheet = (CSheetSetup*)GetParent();
	pSheet->Localize();
	pSheet->m_pageScanPhoto.Localize();
	pSheet->m_pageSys.Localize();
	Localize();

	m_nLanguageBak				= m_nLanguage;
	m_bOpenImageAfterSnapBak	= m_bOpenImageAfterSnap;
	m_bLogAllBak				= m_bLogAll;
	m_bShowScreenMagBak			= m_bShowScreenMag;
	m_bShowDateBak				= m_bShowDate;
	m_bShowNoteBak				= m_bShowNote;
	m_bShowCCDBak				= m_bShowCCD;
	m_nPriorSaveTypeBak			= m_nPriorSaveType;
	m_nPortHVbak				= m_nPortHV;
	m_nPortCCGbak				= m_nPortCCG;
	m_nPortIPGbak				= m_nPortIPG;

	CPropertyPage::OnOK();
}
