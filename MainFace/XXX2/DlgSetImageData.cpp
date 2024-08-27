// DlgSetImageData.cpp : 实现文件
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgSetImageData.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "CommonFunctionManager.h"
#include "VACDataManager.h"
#include "HVPowerDataManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "SemDataType.h"
#include "SemCoreAssemble.h"
#include "GlobalParamSetting.h"

// CDlgSetImageData 对话框

IMPLEMENT_DYNAMIC(CDlgSetImageData, CDialog)

CDlgSetImageData::CDlgSetImageData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetImageData::IDD, pParent)
{

}

CDlgSetImageData::~CDlgSetImageData()
{
}

void CDlgSetImageData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetImageData, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SetLoad, OnBnClickedLoadData)
	ON_BN_CLICKED(IDC_BUTTON_SetSave, OnBnClickedSaveToVal)
	ON_BN_CLICKED(IDC_BUTTON_SetChoose, OnBnClickedChooseVal)
END_MESSAGE_MAP()

void CDlgSetImageData::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND1CTRL_HV)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_HV));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Heater)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Heater));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Bias)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_SP_Bias));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND1CTRL_Brightness)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Brightness));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND1CTRL_Contrast)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Contrast));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND6CTRL_CondenserLens)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_CondenserLens));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND9CTRL_Magnifier)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND9CTRL_Magnifier));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND2CTRL_BeamAlign)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_BeamAlign));
	GetDlgItem(IDS_SEMCORECTRLPANELSTAND2CTRL_Stig)->SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND2CTRL_Stig));
	GetDlgItem(IDC_BUTTON_SetLoad)->SetWindowText(GetResString(IDC_BUTTON_SetLoad));
	GetDlgItem(IDC_BUTTON_SetSave)->SetWindowText(GetResString(IDC_BUTTON_SetSave));
	GetDlgItem(IDC_BUTTON_SetChoose)->SetWindowText(GetResString(IDC_BUTTON_SetChoose));
	
	SetWindowText(GetResString( IDD_DLG_SetImageData ));
}

BOOL CDlgSetImageData::OnInitDialog() 
{
	__super::OnInitDialog();

	Localize();
	LoadIniData();

	return TRUE;
}

// CDlgSetImageData 消息处理程序
void CDlgSetImageData::LoadIniData()
{
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char		stabuff[20];
	char		staitembuffint[20];
	int			nTotal	= SetImageData_Counts;
	int*		pParam	= new int[SetImageData_Counts];
	sprintf_s(stabuff, "OneKEY");
	sprintf_s(staitembuffint, "param2");
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuffint, INTSTYLE, nTotal, pParam) < 2 )
	{
		// 如果ini中没有Align和Stig参数，就按原一键看像参数读取
		sprintf_s(staitembuffint, "param");
		CCommonFunctionManager::Instance().ReadInOneLine(path, stabuff, staitembuffint, INTSTYLE, nTotal, pParam);
		// Align和Stig参数，就按当前值
		pParam[8]	= CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos().lVal;
		pParam[9]	= CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos().lVal;
		pParam[10]	= CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
		pParam[11]	= CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
	}

	CString str;
	str.Format( "%.1f", pParam[0]/1000.0);
	GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(str);
	str.Format( "%d", pParam[1]);
	GetDlgItem(IDC_EDIT_SetBeam)->SetWindowText(str);
	str.Format( "%.2f", pParam[2]*0.01);
	GetDlgItem(IDC_EDIT_SetFilament)->SetWindowText(str);

	str.Format( "%.2f", pParam[3]*100.0/2048);
	GetDlgItem(IDC_EDIT_SetBrightness)->SetWindowText(str);
	str.Format( "%.2f", pParam[4]*0.25);
	GetDlgItem(IDC_EDIT_SetContrast)->SetWindowText(str);

	str.Format( "%.2f", pParam[5]/32.0);
	GetDlgItem(IDC_EDIT_SetCondenserLens)->SetWindowText(str);
	str.Format( "%.2f",  pParam[6]/32.0);
	GetDlgItem(IDC_EDIT_SetObjectiveLens)->SetWindowText(str);

	str.Format( "%d", pParam[7]);
	GetDlgItem(IDC_EDIT_SetMag)->SetWindowText(str);

	str.Format( "%d", pParam[8]);
	GetDlgItem(IDC_EDIT_SetBeamAlignX)->SetWindowText(str);
	str.Format( "%d", pParam[9]);
	GetDlgItem(IDC_EDIT_SetBeamAlignY)->SetWindowText(str);

	str.Format( "%d", pParam[10]);
	GetDlgItem(IDC_EDIT_SetStigX)->SetWindowText(str);
	str.Format( "%d", pParam[11]);
	GetDlgItem(IDC_EDIT_SetStigY)->SetWindowText(str);

	delete [] pParam;
}

void CDlgSetImageData::OnOK()
{     
	//获取对话框输入值
 	CString str;
	GetDlgItem(IDC_EDIT_SetHV)->GetWindowText(str);
	m_dHV = atof(str);
	GetDlgItem(IDC_EDIT_SetFilament)->GetWindowText(str);
	m_dFilament = atof(str);
	GetDlgItem(IDC_EDIT_SetBeam)->GetWindowText(str);
	m_dBeam = atof(str);

	GetDlgItem(IDC_EDIT_SetBrightness)->GetWindowText(str);
	m_dBrightness = atof(str);
	GetDlgItem(IDC_EDIT_SetContrast)->GetWindowText(str);
	m_dContrast = atof(str);

	GetDlgItem(IDC_EDIT_SetCondenserLens)->GetWindowText(str);
	m_dCondenserLens = atof(str);
	GetDlgItem(IDC_EDIT_SetObjectiveLens)->GetWindowText(str);
	m_dObjectiveLens = atof(str);

	GetDlgItem(IDC_EDIT_SetMag)->GetWindowText(str);
	m_nMag = atoi(str);

	GetDlgItem(IDC_EDIT_SetBeamAlignX)->GetWindowText(str);
	m_nBeamAlignX = atol(str);
	GetDlgItem(IDC_EDIT_SetBeamAlignY)->GetWindowText(str);
	m_nBeamAlignY = atol(str);

	GetDlgItem(IDC_EDIT_SetStigX)->GetWindowText(str);
	m_nStigX = atol(str);
	GetDlgItem(IDC_EDIT_SetStigY)->GetWindowText(str);
	m_nStigY = atol(str);

	//如果灯丝电流过大（>2.6A），弹出对话框
	if(m_dFilament > 2.6)
	{
		MessageBox("灯丝电流值不大于2.6A！");
		return;
	}

	//将修改的参数传递给ini（目的：下次打开对话框直接显示上次的参数）
	char path[256];
	::GetModuleFileName(0, path, 255);
	char* exd = strrchr(path, (int)'\\');
	if (exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	int nTotal = SetImageData_Counts;
	int* pParam = new int[SetImageData_Counts];
	pParam[0] = m_dHV*1000;
	pParam[1] = m_dBeam;
	pParam[2] = m_dFilament*100;
	pParam[3] = floor(m_dBrightness*2048/100 + 0.5);
	pParam[4] = m_dContrast*4;
	pParam[5] = m_dCondenserLens*32;
	pParam[6] = m_dObjectiveLens*32;
	pParam[7] = m_nMag;
	pParam[8] = m_nBeamAlignX;
	pParam[9] = m_nBeamAlignY;
	pParam[10] = m_nStigX;
	pParam[11] = m_nStigY;
	
	CCommonFunctionManager::Instance().SaveInOneLine(path, "OneKEY", "param2", INTSTYLE, nTotal, pParam);
	delete [] pParam;
	theApp.DisplayMsg("Press OK on SetImageData panel, parameters has been save to ini");
	
	//如果V1未打开，点击V1后直接加载ini中的数据；
	if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnV1.GetCheck())
	{
		//如果V1打开，将ini中参数加载到主控面板上并生效
		LoadDataToPanel(); 
	}
	else
		theApp.DisplayMsg("while V1 is opened, the parameters will be sent to mail panel");
	CDialog::OnOK();
}

void CDlgSetImageData::LoadDataToPanel()
{
	//将参数加载到主控面板上并生效
	theApp.DisplayMsg("Send parameters to main panel and execute");
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	CString strX, strY;
	SerialNumber.lVal	= -1;

	Flag.lVal = 3;
	//BeamAlign
	{
		ClassID.lVal	= SemControlPanelClass_BeemAlignment;
		Xpos.lVal		= m_nBeamAlignX;
		Ypos.lVal		= m_nBeamAlignY;
		Flag.lVal		= 3;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	//Stigmator
	{
		ClassID.lVal	= SemControlPanelClass_Stigmator;
		Xpos.lVal		= m_nStigX;
		Ypos.lVal		= m_nStigY;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}

	Flag.lVal = 1;
	// CondenserLens
	{
		ClassID.lVal	= SemControlPanelClass_CondenserLens;
		Xpos.lVal		= m_dCondenserLens*32;	// 550.00 [C4C0]
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	// ObjectiveLens
	{
		ClassID.lVal	= SemControlPanelClass_ObjectiveLens; 
		Xpos.dblVal		= m_dObjectiveLens*32;	// 550.00 [C4C0]
		Xpos.lVal		= m_dObjectiveLens*32;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	// Magnifier
	{
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		Xpos.dblVal		= m_nMag;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	//Brightness
	{
		long lPos = CSemCoreAssemble::Instance().m_Brightness.GetPosByValue(m_dBrightness);
		ClassID.lVal	= SemControlPanelClass_Brightness;
		Xpos.lVal		= lPos;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	//Contrast:需要分步加载吗？
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			ClassID.lVal = SemControlPanelClass_Contrast;
			Xpos.lVal = m_dContrast*4;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		}
		else
		{
			ClassID.lVal = SemControlPanelClass_SP_PMT;
			Xpos.lVal = m_dContrast*4;
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangePMT_WSM30KV( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangePMT_771( ClassID, SerialNumber, Xpos, Ypos, Flag, FALSE );
		}
	}

	//HV
	{
		if( m_dHV < 10.0 )
			Xpos.lVal = (LONG)(m_dHV *10);
		else
			Xpos.lVal = (LONG)(m_dHV -10 +100);
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			ClassID.lVal = SemControlPanelClass_CPS_HV;
			((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
		}
		else
		{
			ClassID.lVal = SemControlPanelClass_SP_HV;
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
		}
	}
	//Beam
	{
		Xpos.lVal = m_dBeam;
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			ClassID.lVal = SemControlPanelClass_CPS_Bias;
			((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBias_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
		}
		else
		{
			ClassID.lVal = SemControlPanelClass_SP_Bias;
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBeam_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBeam_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
		}
	}
	//Filament
	{
		Xpos.lVal = (LONG)(m_dFilament *100);
		if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		{
			ClassID.lVal = SemControlPanelClass_CPS_FilamentCurrent;
			((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
		}
		else
		{
			ClassID.lVal = SemControlPanelClass_SP_Heater;
			if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_WSM30KV( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
		}
	}
}

void CDlgSetImageData::OnBnClickedLoadData()
{
	//点击加载之后将主控面板上参数加载到对话框中
	CString strUIValue = "";

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		//3603 束流和灯丝？？？
		strUIValue = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 3);
		GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(strUIValue);

		strUIValue = CSemCoreAssemble::Instance().m_CPS_Bias.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		GetDlgItem(IDC_EDIT_SetBeam)->SetWindowText(strUIValue);

		strUIValue = CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.FormatText();
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		GetDlgItem(IDC_EDIT_SetFilament)->SetWindowText(strUIValue);

		strUIValue = CSemCoreAssemble::Instance().m_Contrast.FormatText();
		GetDlgItem(IDC_EDIT_SetContrast)->SetWindowText(strUIValue);
	}
	else 
	{
		if (CHVPowerDataManager::Instance().m_dwHVtype > 2)
		{
			//WSM2
			strUIValue.Format(_T("%.1f"), theApp.m_setWSM30KV2.fACLv);
			GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(strUIValue);

			strUIValue.Format(_T("%.f"), theApp.m_setWSM30KV2.fACLi);
			GetDlgItem(IDC_EDIT_SetBeam)->SetWindowText(strUIValue);

			strUIValue.Format(_T("%.2f"), theApp.m_setWSM30KV2.fFILi);
			GetDlgItem(IDC_EDIT_SetFilament)->SetWindowText(strUIValue);

			strUIValue.Format(_T("%.2f"), theApp.m_setWSM30KV2.fPMTv * 100.0/1300);
			GetDlgItem(IDC_EDIT_SetContrast)->SetWindowText(strUIValue);
		}
		else if(CHVPowerDataManager::Instance().m_dwHVtype > 1)
		{   
			//WSM
			strUIValue.Format(_T("%.1f"), theApp.m_setWSM30KV.fACLv);
			GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(strUIValue);

			strUIValue.Format(_T("%.f"), theApp.m_setWSM30KV.fACLi);
			GetDlgItem(IDC_EDIT_SetBeam)->SetWindowText(strUIValue);

			strUIValue.Format(_T("%.2f"), theApp.m_setWSM30KV.fFILi);
			GetDlgItem(IDC_EDIT_SetFilament)->SetWindowText(strUIValue);

			strUIValue.Format(_T("%.2f"), theApp.m_setWSM30KV.fPMTv * 100.0/1300);
			GetDlgItem(IDC_EDIT_SetContrast)->SetWindowText(strUIValue);
		}
		else
		{
			//spellman电源
			strUIValue = CSemCoreAssemble::Instance().m_SP_HV.FormatText();
			strUIValue = strUIValue.Left(strUIValue.GetLength() - 3);
			GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(strUIValue);

			//double HV = CHVPowerDataManager::Instance().m_Spellmans.pS.dVE;
			//strUIValue.Format(_T("%.1f"), HV);
			//GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(strUIValue);

			double Beam = CHVPowerDataManager::Instance().m_Spellmans.pM.dIB;
			strUIValue.Format(_T("%.f"), Beam);
			GetDlgItem(IDC_EDIT_SetBeam)->SetWindowText(strUIValue);

			double Filament = CHVPowerDataManager::Instance().m_Spellmans.pM.dIH; 
			strUIValue.Format(_T("%.2f"), Filament);
			GetDlgItem(IDC_EDIT_SetFilament)->SetWindowText(strUIValue);

			strUIValue = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
			GetDlgItem(IDC_EDIT_SetContrast)->SetWindowText(strUIValue);
		}
	}

	strUIValue = CSemCoreAssemble::Instance().m_Brightness.FormatText();
	GetDlgItem(IDC_EDIT_SetBrightness)->SetWindowText(strUIValue);

	strUIValue = CSemCoreAssemble::Instance().m_CondenserLens.FormatText();
	GetDlgItem(IDC_EDIT_SetCondenserLens)->SetWindowText(strUIValue);
	strUIValue = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText();
	GetDlgItem(IDC_EDIT_SetObjectiveLens)->SetWindowText(strUIValue);

	strUIValue = CSemCoreAssemble::Instance().m_Magnifier.FormatText();
	int nKX = strUIValue.Find(" KX");
	if (-1 != nKX)
	{
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 3);
		int Mag = atof(strUIValue.GetBuffer()) * 1000;
		strUIValue.Format( "%d", Mag);
		GetDlgItem(IDC_EDIT_SetMag)->SetWindowText(strUIValue);
	}
	else
	{
		strUIValue = strUIValue.Left(strUIValue.GetLength() - 2);
		GetDlgItem(IDC_EDIT_SetMag)->SetWindowText(strUIValue);
	}

	strUIValue = CSemCoreAssemble::Instance().m_BeemAlignmentX.FormatText();
	GetDlgItem(IDC_EDIT_SetBeamAlignX)->SetWindowText(strUIValue);
	strUIValue = CSemCoreAssemble::Instance().m_BeemAlignmentY.FormatText();
	GetDlgItem(IDC_EDIT_SetBeamAlignY)->SetWindowText(strUIValue);

	strUIValue = CSemCoreAssemble::Instance().m_StigmatorX.FormatText();
	GetDlgItem(IDC_EDIT_SetStigX)->SetWindowText(strUIValue);
	strUIValue = CSemCoreAssemble::Instance().m_StigmatorY.FormatText();
	GetDlgItem(IDC_EDIT_SetStigY)->SetWindowText(strUIValue);
}

void CDlgSetImageData::OnBnClickedSaveToVal()
{
	CFileDialog dlg(FALSE, "val", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "val (*.val)|*.val||", NULL);
	if( dlg.DoModal() != IDOK )
		return;
	CString csFullPath = dlg.GetPathName();
	CFile file;
	file.Open( csFullPath, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite );
	if( file.m_hFile == CFile::hFileNull )
	{
		theApp.DisplayMsg("Press save params on SetImageData panel: create file failed!");
		return;
	}

	double dHV, dFilament, dBeam, dContrast, dBrightness, dCL, dOL;
	int nMag, nBeamAlignX, nBeamAlignY, nStigX, nStigY;
	CString strValue = "";

	GetDlgItem(IDC_EDIT_SetHV)->GetWindowText(strValue);
	dHV = atof(strValue);
	GetDlgItem(IDC_EDIT_SetFilament)->GetWindowText(strValue);
	dFilament = atof(strValue);
	GetDlgItem(IDC_EDIT_SetBeam)->GetWindowText(strValue);
	dBeam = atof(strValue);

	GetDlgItem(IDC_EDIT_SetContrast)->GetWindowText(strValue);
	dContrast = atof(strValue);
	GetDlgItem(IDC_EDIT_SetBrightness)->GetWindowText(strValue);
	dBrightness = atof(strValue);

	GetDlgItem(IDC_EDIT_SetCondenserLens)->GetWindowText(strValue);
	dCL = atof(strValue);
	GetDlgItem(IDC_EDIT_SetObjectiveLens)->GetWindowText(strValue);
	dOL = atof(strValue);

	GetDlgItem(IDC_EDIT_SetMag)->GetWindowText(strValue);
	nMag = atoi(strValue);

	GetDlgItem(IDC_EDIT_SetBeamAlignX)->GetWindowText(strValue);
	nBeamAlignX = atol(strValue);
	GetDlgItem(IDC_EDIT_SetBeamAlignY)->GetWindowText(strValue);
	nBeamAlignY = atol(strValue);
	GetDlgItem(IDC_EDIT_SetStigX)->GetWindowText(strValue);
	nStigX = atol(strValue);
	GetDlgItem(IDC_EDIT_SetStigY)->GetWindowText(strValue);
	nStigY = atol(strValue);

	CString str;
	str.Format( "[Values]\r\n");
	file.Write( str, str.GetLength() );
	str.Format( "HV=%.1f\r\n", dHV );
	file.Write( str, str.GetLength() );
	str.Format( "Beam=%.f\r\n", dBeam );
	file.Write( str, str.GetLength() );
	str.Format( "Filament=%.2f\r\n", dFilament );
	file.Write( str, str.GetLength() );
	str.Format( "Brightness=%.2f\r\n", dBrightness );
	file.Write( str, str.GetLength() );
	str.Format( "Contrast=%.2f\r\n", dContrast );
	file.Write( str, str.GetLength() );
	str.Format( "CondenserLens=%.2f\r\n", dCL);
	file.Write( str, str.GetLength() );
	str.Format( "ObjectiveLens=%.2f\r\n", dOL);
	file.Write( str, str.GetLength() );
	str.Format( "Mag=%d\r\n", nMag );
	file.Write( str, str.GetLength() );
	str.Format( "BeamAlignX=%d\r\n", nBeamAlignX );
	file.Write( str, str.GetLength() );
	str.Format( "BeamAlignY=%d\r\n", nBeamAlignY );
	file.Write( str, str.GetLength() );
	str.Format( "StigX=%d\r\n", nStigX );
	file.Write( str, str.GetLength() );
	str.Format( "StigY=%d\r\n", nStigY );
	file.Write( str, str.GetLength() );

	file.Close();
	theApp.DisplayMsg("Save params finish!");
}

void CDlgSetImageData::OnBnClickedChooseVal()
{
	CFileDialog dlg(TRUE, "val", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "val (*.val)|*.val||", NULL);
	if( dlg.DoModal() != IDOK )
		return;
	theApp.DisplayMsg("Press choose params on SetImageData panel");
	CString csFullPath = dlg.GetPathName();
	char path[255];
	sprintf_s(path, "%s", csFullPath);

	char	szBuf[50], stabuff[20];
	sprintf_s(stabuff, "Values");

	if(GetPrivateProfileString(stabuff, "HV", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetHV)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "Beam", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetBeam)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "Filament", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetFilament)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "Brightness", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetBrightness)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "Contrast", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetContrast)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "CondenserLens", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetCondenserLens)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "ObjectiveLens", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetObjectiveLens)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "Mag", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetMag)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "BeamAlignX", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetBeamAlignX)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "BeamAlignY", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetBeamAlignY)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "StigX", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetStigX)->SetWindowText(szBuf);
	if(GetPrivateProfileString(stabuff, "StigY", "", szBuf, 30, path) != 0)
		GetDlgItem(IDC_EDIT_SetStigY)->SetWindowText(szBuf);
}

