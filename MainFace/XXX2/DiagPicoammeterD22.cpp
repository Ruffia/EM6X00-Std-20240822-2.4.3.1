// DiagLogging.cpp : implementation file
//
// Last Modify : 2013.01.05

#include "stdafx.h"
#include "resource.h"
#include "XXX2.h"
#include "MainFrm.h"
#include "afxdialogex.h"
#include "DlgDiagLog.h"
#include "DlgDiagLogGraph.h"
#include "HVPowerDataManager.h"
#include "D22StatusDetector.h"
#include "ControlLayer.h"
#include "StageXYManager.h"
#include "ExportStageMC600Func.h"
#include "ExportStageMC6600Func.h"
#include "ExportStageTMCMFunc.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "PLCvacBrdStatusDetector.h"
#include "Comm_Beam.h"
#include "DlgBeamAperture.h"
#include "DlgPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int			g_nBeamCLTar = 350, g_nBeamLand = 100;
const long	c_nBeamConStep[7] = {10*32, 8*32, 5*32, 3*32, 2*32, 1*32, 0.5*32};
const long	c_nBeamTimeStep[3] = {1000,450,450};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

BOOL NotZero( float fValue)
{
	if( fabs(fValue) < FLT_EPSILON )	// �ǳ�С
		return FALSE;
	else
		return TRUE;
}

BOOL StopCondition( float fValue)
{
	if((fabs(fValue)/abs(g_nBeamLand)) < 0.05)
		return FALSE;
	else
		return TRUE;
}

INT BeamAnalysis(float fCurrentBeam)
{
	int nCID;
	if(abs(fCurrentBeam)<20)
		nCID = 0;
	else if(abs(fCurrentBeam)<80)
		nCID = 1;
	else if(abs(fCurrentBeam)<300)
		nCID =2;
	else
		nCID = 3;
    return nCID;	
}

INT PercentAnalysis(float fCurrentBeam,int nLandBeam)
{
	int nPerID;
	if((abs(fCurrentBeam - nLandBeam)/(abs(nLandBeam))) > 0.7 )
		nPerID = 0;
	else if((abs(fCurrentBeam - nLandBeam)/(abs(nLandBeam))) > 0.5)
		nPerID = 1;
	else if((abs(fCurrentBeam - nLandBeam)/(abs(nLandBeam))) > 0.2)
		nPerID =2;
	else if((abs(fCurrentBeam - nLandBeam)/(abs(nLandBeam))) > 0.1)
		nPerID =3;
	else
		nPerID = 4;
    return nPerID;	
}

void CMainFrame::D22_Init()
{
	m_nPortD22 = -1;
	m_bIsD22Ready = FALSE;

	D22_InitParam();
	
	//Ŀ������������
	m_comboBeamTar.AddString("-3 pA" );
	m_comboBeamTar.AddString("-5 pA" );
	m_comboBeamTar.AddString("-10 pA" );
	m_comboBeamTar.AddString("-20 pA" );
	m_comboBeamTar.AddString("-30 pA" );
	m_comboBeamTar.AddString("-50 pA" );
	m_comboBeamTar.SetCurSel(1);
	//�˿�
	int i=0;
	CString str;
	for(i=0;i<30;i++)
	{
		str.Format("COM %d", i+1);
		m_comboBeamPort.AddString(str);
	}
	m_comboBeamPort.SetCurSel(m_nPortD22-1);
	//����
	m_comboBeamAper.AddString("30");
	m_comboBeamAper.AddString("50" );
	m_comboBeamAper.AddString("70" );
	if((m_nBeamAper-50) > 0)
		m_comboBeamAper.SetCurSel(2);
	else if((m_nBeamAper-50) < 0)
		m_comboBeamAper.SetCurSel(0);
	else
		m_comboBeamAper.SetCurSel(1);
	//����ʵʱֵ
	m_editCurrentBeam.SetWindowText("0.0" );

	m_nBeamLandNumNew	= 0;
	m_nBeamLandNumOld	= 0;
//	m_nAccVolt			= 0;
	m_nBeamLand			= 0;
	m_dBeamCondenser	= 0;
	m_bBeamFlag			= FALSE;
	m_nBeamStateAper	= 0;
	m_nBeamStatesShow	= 0;
	m_dBeamMagTar		= 250;
	m_fBeamCur			= 0.0;
	m_nBeamCount		= 0;
	for(i=0; i<5; i++)
		m_fBeamStagePosCur[i] = 0.0f;

	D22_Relink();
}

void CMainFrame::D22_InitParam()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char szBuf[255], szBuf2[255];
	//�˿�
	m_nPortD22 = 20;
	if(GetPrivateProfileString("PicoammeterD22", "Port", "", szBuf, 255, path) != 0)
		m_nPortD22 = atoi(szBuf);
	//����
	m_nBeamAper = 30;
	if(GetPrivateProfileString("PicoammeterD22", "Aperture", "", szBuf, 255, path) != 0)
		m_nBeamAper = atoi(szBuf);
	//�����ڱ���λ��
	m_fBeamFaradayCupPos[0] = 0.0;
	m_fBeamFaradayCupPos[1] = 0.0;
	int i=0;
	if(GetPrivateProfileString("PicoammeterD22", "FlangecupPos ", "", szBuf, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf2, strchr(szBuf, ',')+1);
			*strchr(szBuf, ',') = '\0';
			m_fBeamFaradayCupPos[0] = atof(szBuf);
			m_fBeamFaradayCupPos[1] = atof(szBuf2);
		}
	}
}

void CMainFrame::D22_Relink()
{
	if( m_bIsD22Ready )
	{
		CD22StatusDetector::Instance().Release();
		m_bIsD22Ready = FALSE;
	}
	CD22StatusDetector::Instance().Init( this->m_hWnd, m_nPortD22 );
	CD22StatusDetector::Instance().Start();
}

LRESULT CMainFrame::OnUpdateD22Value(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch( wParam )
	{
	case	0:
		{
			BOOL bReady = (BOOL)lParam;
			if(bReady)
				str.Format("D22: Open D22 COM %d successfully!", m_nPortD22);
			else
			{
				D22_SetStatus(FALSE);
				str.Format("D22: Open D22 COM %d Failed!", m_nPortD22);
			}
			theApp.DisplayMsg( str );
		}
		break;
	case	1:
		m_bIsD22Ready = (BOOL)lParam;
		if(m_bIsD22Ready)
		{
			D22_SetStatus(TRUE);
			str = "D22: D22 is Ready!";
		}
		else
		{   
			D22_SetStatus(FALSE);
			str = "D22: D22 is NOT ready!";
			//::PostMessage( GetParent()->m_hWnd, WM_USER_D22_UpdateValue, 0, 0 );
		}
		theApp.DisplayMsg( str );
		break;

	case	11:
		m_fBeamCur = *((float*)lParam);
		str.Format( "%.5f", m_fBeamCur );
		m_editCurrentBeam.SetWindowText(str);
		break;
	}
	return 0;
}

void CMainFrame::D22_SetStatus(BOOL bEnable)
{
	if(bEnable)
	{
		m_comboBeamTar.EnableWindow(TRUE);
		m_editCurrentBeam.EnableWindow(TRUE);
		m_btnBeamSetup.EnableWindow(TRUE);
		m_btnBeamDetection.EnableWindow(TRUE);
		m_btnBeamDetection.Invalidate(FALSE);
	}
	else
	{
		m_comboBeamTar.EnableWindow(FALSE);
		m_editCurrentBeam.EnableWindow(FALSE);
		m_btnBeamSetup.EnableWindow(FALSE);
		m_btnBeamDetection.EnableWindow(FALSE);
		m_btnBeamDetection.Invalidate(FALSE);
	}
}

void CMainFrame::OnSelchangeBeamD22Port() 
{
	// TODO: Add your control notification handler code here
	m_nPortD22 = m_comboBeamPort.GetCurSel() +1;
	Beam_SaveToINI(0);
	D22_Relink();
}

void CMainFrame::OnSelchangeBeamD22Aperture() 
{
	// TODO: Add your control notification handler code here
	int nAperture = 0;
	nAperture = m_comboBeamAper.GetCurSel();
	m_nBeamAper = 30 + nAperture * 20;
	Beam_SaveToINI(1);
}

void CMainFrame::OnSelchangeBeamD22Target()
{
	m_nBeamLandNumNew = m_comboBeamTar.GetCurSel();
	if( m_nBeamLandNumOld != m_nBeamLandNumNew)
	{
		m_nBeamLandNumOld = m_nBeamLandNumNew;
		m_bBeamFlag = TRUE;
	}
}

void CMainFrame::OnDropdownComboBeamTarget()
{
	m_comboBeamTar.ResetContent();
	double dHV_Beam = 0.0;
	CString str;
	dHV_Beam = CHVPowerDataManager::Instance().m_dHV;
	int nID =0;
	if(dHV_Beam <= 3)
		nID = 0;
	else if(dHV_Beam <= 5)
		nID = 1;
	else if(dHV_Beam <= 10)
		nID = 2;
	else if(dHV_Beam <= 15)
		nID = 3;
	else if(dHV_Beam <= 25)
		nID = 4;
	else
		nID = 5;
	switch(nID)
	{
	case 0:
		m_nBeamSelect[0] = -3;	m_nBeamSelect[1] = -5;	m_nBeamSelect[2] = -10;
		m_nBeamSelect[3] = -20;	m_nBeamSelect[4] = -30;	m_nBeamSelect[5] = -50;
		break;
	case 1:
		m_nBeamSelect[0] = -3;	m_nBeamSelect[1] = -5;	m_nBeamSelect[2] = -10;
		m_nBeamSelect[3] = -30;	m_nBeamSelect[4] = -50;	m_nBeamSelect[5] = -80;
		break;
	case 2:
		m_nBeamSelect[0] = -3;	m_nBeamSelect[1] = -5;	m_nBeamSelect[2] = -30;
		m_nBeamSelect[3] = -50;	m_nBeamSelect[4] = -80;	m_nBeamSelect[5] = -100;
		break;
	case 3:
		m_nBeamSelect[0] = -3;	m_nBeamSelect[1] = -5;	m_nBeamSelect[2] = -30;
		m_nBeamSelect[3] = -50;	m_nBeamSelect[4] = -100;m_nBeamSelect[5] = -120;
		break;
	case 4:
		m_nBeamSelect[0] = -5;	m_nBeamSelect[1] = -30;	m_nBeamSelect[2] = -50;
		m_nBeamSelect[3] = -100;m_nBeamSelect[4] = -120;m_nBeamSelect[5] = -150;
		break;
	case 5:
		m_nBeamSelect[0] = -5;	m_nBeamSelect[1] = -30;	m_nBeamSelect[2] = -50;
		m_nBeamSelect[3] = -100;m_nBeamSelect[4] = -150;m_nBeamSelect[5] = -200;
		break;
	}
	for(int i=0;i<6;i++)
	{
		str.Format( "%d pA", m_nBeamSelect[i] );
		m_comboBeamTar.AddString(str);
	}
	/*m_comboBeamTar.SetCurSel(0);*/
}

void CMainFrame::OnBtnBeamSetup()
{
	CString str;
	//if(GetDlgItemText(IDC_BUTTON_BEAM_PORT,str),str == "<<")
	if(m_btnBeamSetup.GetWindowText(str),str == "<<")
		m_btnBeamSetup.SetWindowText(">>");
	else
		m_btnBeamSetup.SetWindowText("<<");
	
	if(str=="<<")
	{
		m_staticBeamPort.ShowWindow( SW_SHOW );
		m_staticBeamAper.ShowWindow( SW_SHOW );
		m_comboBeamPort.ShowWindow( SW_SHOW );
		m_comboBeamAper.ShowWindow( SW_SHOW );
		m_btnBeamSaveXYpos.ShowWindow( SW_SHOW );
	}
	else
	{
		m_staticBeamPort.ShowWindow( SW_HIDE );
		m_staticBeamAper.ShowWindow( SW_HIDE );
		m_comboBeamPort.ShowWindow( SW_HIDE );
		m_comboBeamAper.ShowWindow( SW_HIDE );
		m_btnBeamSaveXYpos.ShowWindow( SW_HIDE );
	}
}

void CMainFrame::OnBtnBeamSaveXYpos()
{
	//��ȡ��ǰλ��
	if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
		StageTMCM_GetParam( 2, m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
	else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
		StageMC6600_GetParam( 2, m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
	else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
		StageMC600_GetParam( 2, m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
	//���·����ڱ�λ��XY
	m_fBeamFaradayCupPos[0] = m_fBeamStagePosCur[0];
	m_fBeamFaradayCupPos[1] = m_fBeamStagePosCur[1];
	Beam_SaveToINI(2);
}

//������� 20230915 HY
void CMainFrame::Beam_SaveToINI( int nIndex )
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char szBuf[255];
	// д�������ļ�
	switch( nIndex )
	{
	case	0:
		sprintf( szBuf, "%d", m_nPortD22);
		WritePrivateProfileString( "PicoammeterD22", "Port", szBuf, path );
		break;
	case	1:
		sprintf( szBuf, "%d", m_nBeamAper);
		WritePrivateProfileString( "PicoammeterD22", "Aperture", szBuf, path );
		break;
	case	2:
		sprintf( szBuf, "%.3f,%.3f", m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1] );
		WritePrivateProfileString( "PicoammeterD22", "FlangecupPos", szBuf, path );
		sprintf( szBuf, "BeamDetector: save current position [%.3f, %.3f] as FaradayCup Position.\n", m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1] );
		theApp.DisplayMsg(szBuf);
		break;
	}
}
/*
void CMainFrame::Beam_CovertValue()
{
	switch(m_nBeamLandNumOld)
	{
	case 0:
		m_nBeamLand = m_nBeamSelect[0];
		break;
	case 1:
		m_nBeamLand = m_nBeamSelect[1];
		break;
	case 2:
		m_nBeamLand = m_nBeamSelect[2];
		break;
	case 3:
		m_nBeamLand = m_nBeamSelect[3];
		break;
	case 4:
		m_nBeamLand = m_nBeamSelect[4];
		break;
	case 5:
		m_nBeamLand = m_nBeamSelect[5];
		break;
	}
}
*/
void CMainFrame::Beam_AutoCali()
{
	//������Ż�ȡ��Ӧ�ĸ�ѹ
	int nAccVolt = (int)(CHVPowerDataManager::Instance().m_dHV);
	//������Ż�ȡ��Ӧ������ֵ
//	CovertValueBeam();
	m_nBeamLand = m_nBeamSelect[m_nBeamLandNumOld];
	//��ȡ�۹⾵ֵ-���ڹ���ѡ��ʽ
	switch(m_nBeamAper)
	{
	case	30:
		switch(nAccVolt)
		{
		case 3:
			m_dBeamCondenser = 520.31 * pow(-m_nBeamLand,-0.094)*32;
			break;
		case 5:
			m_dBeamCondenser = 529.42 * pow(-m_nBeamLand,-0.091)*32;
			break;
		case 10:
			m_dBeamCondenser = 534.82 * pow(-m_nBeamLand,-0.087)*32;
			break;
		case 15:
			m_dBeamCondenser = 544.59 * pow(-m_nBeamLand,-0.086)*32;
			break;
		case 20:
			m_dBeamCondenser = 587.42 * pow(-m_nBeamLand,-0.092)*32;
			break;
		case 30:
			m_dBeamCondenser = 573.84 * pow(-m_nBeamLand,-0.088)*32;
			break;
		}
		break;
	case	50:
		switch(nAccVolt)
		{
		case 3:
			m_dBeamCondenser = 520.31 * pow(-m_nBeamLand,-0.094)*32;
			break;
		case 5:
			m_dBeamCondenser = 529.42 * pow(-m_nBeamLand,-0.091)*32;
			break;
		case 10:
			m_dBeamCondenser = 534.82 * pow(-m_nBeamLand,-0.087)*32;
			break;
		case 15:
			m_dBeamCondenser = 544.59 * pow(-m_nBeamLand,-0.086)*32;
			break;
		case 20:
			m_dBeamCondenser = 587.42 * pow(-m_nBeamLand,-0.092)*32;
			break;
		case 30:
			m_dBeamCondenser = 573.84 * pow(-m_nBeamLand,-0.088)*32;
			break;
		}
		break;
	case	70:
		switch(nAccVolt)
		{
		case 3:
			m_dBeamCondenser = 520.31 * pow(-m_nBeamLand,-0.094)*32;
			break;
		case 5:
			m_dBeamCondenser = 529.42 * pow(-m_nBeamLand,-0.091)*32;
			break;
		case 10:
			m_dBeamCondenser = 534.82 * pow(-m_nBeamLand,-0.087)*32;
			break;
		case 15:
			m_dBeamCondenser = 544.59 * pow(-m_nBeamLand,-0.086)*32;
			break;
		case 20:
			m_dBeamCondenser = 587.42 * pow(-m_nBeamLand,-0.092)*32;
			break;
		case 30:
			m_dBeamCondenser = 573.84 * pow(-m_nBeamLand,-0.088)*32;
			break;
		}
		break;
	case	100:
		switch(nAccVolt)
		{
		case 3:
			m_dBeamCondenser = 520.31 * pow(-m_nBeamLand,-0.094)*32;
			break;
		case 5:
			m_dBeamCondenser = 529.42 * pow(-m_nBeamLand,-0.091)*32;
			break;
		case 10:
			m_dBeamCondenser = 534.82 * pow(-m_nBeamLand,-0.087)*32;
			break;
		case 15:
			m_dBeamCondenser = 544.59 * pow(-m_nBeamLand,-0.086)*32;
			break;
		case 20:
			m_dBeamCondenser = 561.5 * pow(-m_nBeamLand,-0.088)*32;
			break;
		case 30:
			m_dBeamCondenser = 573.84 * pow(-m_nBeamLand,-0.088)*32;
			break;
		}
	  break;
	case	200:
		switch(nAccVolt)
		{
		case 3:
			m_dBeamCondenser = 630.56 * pow(-m_nBeamLand,-0.106)*32;
			break;
		case 5:
			m_dBeamCondenser = 644.36 * pow(-m_nBeamLand,-0.106)*32;
			break;
		case 10:
			m_dBeamCondenser = 655.16 * pow(-m_nBeamLand,-0.103)*32;
			break;
		case 15:
			m_dBeamCondenser = 658.6 *  pow(-m_nBeamLand,-0.1)*32;
			break;
		case 20:
			m_dBeamCondenser = 669.99 * pow(-m_nBeamLand,-0.1)*32;
			break;
		case 30:
			m_dBeamCondenser = 677.16 * pow(-m_nBeamLand,-0.097)*32;
			break;
		}
	  break;
	}
	 m_nBeamFineAper =  (int)m_dBeamCondenser;
	 g_nBeamCLTar = m_nBeamFineAper;
	 if(m_nBeamFineAper < 11200 || m_nBeamFineAper > 19200 )
	 {
		 if(m_nBeamFineAper < 11200)
			 m_nBeamFineAper = 11200;
		 else
			 m_nBeamFineAper = 19200;
	 }
//	::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,4,m_nBeamFineAper);
	 Beam_UpdateCtrls( 2 );
	
	//�����۹⾵��ʹ��½����ΪĿ��ֵ
	//::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,4,m_dBeamCondenser);
	Sleep(500);
	//�жϵ�ǰ����ֵ��Ŀ��ֵ��ֵ
	SetTimer(3,300,NULL);
}

void CMainFrame::OnBtnBeamDetection()
{
	if( !m_bIsD22Ready )
		return;

	m_nBeamCount = 0;
	switch(m_nBeamStatesShow)
	{
	case	0:
		{
			m_nBeamStatesShow =1;
			m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
			//�����������
			//CDevController_BRD::Instance().SetParam((100 +byte_opcBeam), 1);
			Sleep(300);
			//��ȡ��ǰ�Ŵ���
			m_dBeamMagCur		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;
			//������Ʒ̨-��ȡ��ǰ��Ʒ̨λ��\�·�Ŀ��λ��-�����ڱ�λ��
//			::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,1,(LPARAM)&m_fBeamFaradayCupPos);
			Beam_UpdateStagePos( 0 );
			//���Ŵ�������Ϊ�ض��Ŵ��������е������
//			::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,0,m_dBeamMagTar);
			Beam_UpdateCtrls( 0 );
			if(NotZero(m_fBeamCur))
				UpdateData(FALSE);
			//�ӵ�/�����л�
			if(CSemCtrl::Instance().m_nVACBoardType > 20)
				CPLCvacBrdStatusDetector::Instance().SetCmd(byte_PLCvacBRDopc_BeamDetect,byte_PLCvacBRDopr_Open);
			CD22StatusDetector::Instance().SetParam( 13, 0 );	// ��ʼ�������

			//��ȡ��ѹֵ
//			m_nAccVolt	= CHVPowerDataManager::Instance().m_dHV ;

			//����-���༭��ֵ��ǰ����
			SetTimer(4,500,NULL);
		}
		break;
	case	1:
		if(m_bBeamFlag)
		{
			int num = 0;
			m_nBeamStateAper = m_nBeamStateAper + 1;
			m_nBeamStatesShow = 2;
			m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
			//��������ѡ�񵯴�
			if(m_nBeamStateAper == 1)
			{
				CDlgBeamAperture dlg;
				dlg.m_nBeamAper = m_nBeamAper;
				if(dlg.DoModal() == IDOK)
				{
					m_nBeamAper = dlg.m_nBeamAper;
					Beam_SaveToINI(1);
					switch(m_nBeamAper)
					{
					case	30:
						num = 0;
						break;
					case	50:
						num = 1;
						break;
					case	70:
						num = 2;
						break;
					}
					m_comboBeamAper.SetCurSel(num);
				}
			}
			//��ʼ����
			Beam_AutoCali();
		}
		else
		{
			m_nBeamStatesShow = 0;
			m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
			//m_btnBeamDetection.Invalidate(FALSE);
			//ֹͣ��ѯ��ֹͣ����
			CD22StatusDetector::Instance().SetParam( 13, 1 );	// ��ͣ���
			if(CSemCtrl::Instance().m_nVACBoardType > 20)
				CPLCvacBrdStatusDetector::Instance().SetCmd(byte_PLCvacBRDopc_BeamDetect,byte_PLCvacBRDopr_Close);
			Sleep(100);
			KillTimer(2);
			KillTimer(3);
			//����-�·�Ŀ��λ��-��Ʒ̨�ƶ�ǰλ��
//			::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,2,NULL);
			Beam_UpdateStagePos( 1 );
			//���÷Ŵ���
//			::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,0,m_dBeamMagCur);
			Beam_UpdateCtrls( 1 );
			//�ر��������
			//CDevController_BRD::Instance().SetParam((100 +byte_opcBeam), 2);
		}
		m_bBeamFlag = FALSE;
		break;
	case	2:
		if(m_bBeamFlag)
		{
			m_nBeamStatesShow =2;
			//m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
			//��ʼ����
			Beam_AutoCali();
		}
		else
		{
			//��ӵ�����ȷ���Ƿ��ٴν�����������
//			if( MessageBox( "�Ƿ��ٴβ���? ���ǣ�����������������ص���Ʒ��ʼλ�ã����ٴε�����������⡱��ť���˳����ģʽ", "��ʾ", MB_YESNO ) == IDYES )
			CDlgPrompt dlg;
			dlg.m_byteType = (BYTE)0x04;
			dlg.m_strMsg = GetResString(IDS_SD_PROMPT_BeamDetect_Msg2);
			if( dlg.DoModal() == IDYES )
			{
				m_nBeamStatesShow =2;
				//m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
				//��ʼ����
				Beam_AutoCali();
			}
			else
			{
				m_nBeamStatesShow = 0;	//3;
				m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
				//ֹͣ��ѯ��ֹͣ����
				CD22StatusDetector::Instance().SetParam( 13, 1 );	// ��ͣ���
				if(CSemCtrl::Instance().m_nVACBoardType > 20)
					CPLCvacBrdStatusDetector::Instance().SetCmd(byte_PLCvacBRDopc_BeamDetect,byte_PLCvacBRDopr_Close);
				Sleep(100);
				KillTimer(2);
				KillTimer(3);
				//����-�·�Ŀ��λ��-��Ʒ̨�ƶ�ǰλ��
//				::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,2,NULL);
				Beam_UpdateStagePos( 1 );
				//���÷Ŵ���
//				::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,0,m_dBeamMagCur);
				Beam_UpdateCtrls( 1 );
				//�ر��������
				//CDevController_BRD::Instance().SetParam((100 +byte_opcBeam), 2);
			}
		}
		m_bBeamFlag = FALSE;
		break;
	case	3:
		m_nBeamStatesShow = 0;
		m_btnBeamDetection.OnLButtonDown(m_nBeamStatesShow);
		break;
	}
}

void CMainFrame::Beam_StepsParams(int nCID, int nPerID)
{
	switch(nPerID) //����70%
	{
	case	0:     //����70%
		if(m_fBeamCur - m_nBeamLand > 0 )  
			m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[3-nCID];
		else
			m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[3-nCID];
		g_nBeamCLTar = m_nBeamFineAper;
		SetTimer(3,c_nBeamTimeStep[0],NULL);
		break;
	case	1:     //����50%
		{
			if(nCID < 1)
			{
				if(m_fBeamCur - m_nBeamLand > 0 )  
					m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[3];
				else
					m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[3];
			}
			else
			{
				if(m_fBeamCur - m_nBeamLand > 0 )  
					m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[4-nCID];
				else
					m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[4-nCID];
			}
			g_nBeamCLTar = m_nBeamFineAper;
			SetTimer(3,c_nBeamTimeStep[0],NULL);
		}
		break;
	case	2:   //����20%
		{
			switch(nCID)
			{
			case 0:
				if(m_fBeamCur - m_nBeamLand > 0 )  
					m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[4];
				else
					m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[4];
				break;
			case 1:
			case 2:
			case 3:
				if(m_fBeamCur - m_nBeamLand > 0 )  
					m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[5-nCID];
				else
					m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[5-nCID];
				break;
			}
			g_nBeamCLTar = m_nBeamFineAper;
			SetTimer(3,c_nBeamTimeStep[1],NULL);
		}
		break;
	case	3:  //����10%
		{
			switch(nCID)
			{
				case 0:
					if(m_fBeamCur - m_nBeamLand > 0 )  
						m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[5];
					else
						m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[5];
					break;
				case 1:
				case 2:
				case 3:
					if(m_fBeamCur - m_nBeamLand > 0 )  
						m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[6-nCID];
					else
						m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[6-nCID];
					break;
			}
			g_nBeamCLTar = m_nBeamFineAper;
			SetTimer(3,c_nBeamTimeStep[2],NULL);
		}
		break;
	case	4:   //С��10%
		{
			switch(nCID)
			{
				case 0:
				case 1:
				case 2:
					if(m_fBeamCur - m_nBeamLand > 0 )  
						m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[6];
					else
						m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[6];
					break;
				case 3:
					if(m_fBeamCur - m_nBeamLand > 0 )  
						m_nBeamFineAper	= m_nBeamFineAper - c_nBeamConStep[4];
					else
						m_nBeamFineAper	= m_nBeamFineAper + c_nBeamConStep[4];
					break;
			}
			g_nBeamCLTar = m_nBeamFineAper;
			SetTimer(3,c_nBeamTimeStep[2],NULL);
		}
		break;
	}
		
	if(m_nBeamFineAper > 11200 && m_nBeamFineAper < 22400)
//		::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,4,m_nBeamFineAper);
		Beam_UpdateCtrls( 2 );
	else if(m_nBeamFineAper < 11200)
	{
		if(m_nBeamCount > 5)
		{
			 KillTimer(3);
//			 MessageBox("�������ڷ�Χ", "����", MB_OK|MB_ICONEXCLAMATION );
			 CDlgPrompt dlg;
			 dlg.m_byteType = 0x01;
			 dlg.m_strMsg = "\r\n";
			 dlg.m_strMsg += "\r\n";
			 dlg.m_strMsg += GetResString(IDS_SD_PROMPT_BeamDetect_Msg1);
			 dlg.m_strMsg += "\r\n";
			 dlg.m_strMsg += "\r\n";
			 dlg.DoModal();

/*			 CString strCap, strMsg;
			 strCap = GetResString(IDS_SD_PROMPT_BeamDetect_Cap1);
			 strMsg = GetResString(IDS_SD_PROMPT_BeamDetect_Msg1);
			 MessageBox(strMsg, strCap, MB_OK|MB_ICONEXCLAMATION );*/
			
		}
		else
		{
			m_nBeamFineAper = 11200;
//			::PostMessage(m_wndImpPanelAll2.m_hWnd,WM_USER_BeamDetection_Update,4,m_nBeamFineAper);
			Beam_UpdateCtrls( 2 );
		}
	}
	else
	{
		KillTimer(3);
//		MessageBox("�������ڷ�Χ", "����", MB_OK|MB_ICONEXCLAMATION );
		CDlgPrompt dlg;
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += GetResString(IDS_SD_PROMPT_BeamDetect_Msg1);
		dlg.m_strMsg += "\r\n";
		dlg.m_strMsg += "\r\n";
		dlg.DoModal();
	}
}

void CMainFrame::Beam_AutoProcess()
{
	BOOL fValue= TRUE;
	g_nBeamLand = m_nBeamLand;
	
	fValue = StopCondition(m_fBeamCur - m_nBeamLand);

	switch(fValue)
	{
	case	0:
		KillTimer(3);
		break;
	case	1:
		//�޸ľ۹⾵ֵ
		//�ж�Ŀ�������뵱ǰ�����Ĳ�ֵ�Ƿ����20%,����ֵΪ��
		int nCID,nPerID;
		nCID = BeamAnalysis(m_fBeamCur);
		nPerID = PercentAnalysis(m_fBeamCur,m_nBeamLand);
		Beam_StepsParams(nCID,nPerID);
		break;
	}
}

void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);	//�õ����Ƶ��豸����CDC
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	CString strText;
	((CButton *)GetDlgItem(nIDCtl))->GetWindowText(strText);
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);	//͸��
	if(nIDCtl==IDC_BUTTON_Beam_Detection)    
	{
		COLORREF clrInner, clrPen, clrBG;
		int nLeft = 3, nTop = 3, nRight = 3, nBottom = 3;
		if(m_nBeamStatesShow == 1)	//����ť����
		{
			clrInner	= RGB(50,205,50);	// �ڱ�����ˢɫ
			clrPen		= RGB(20,20,20);	// �߿򻭱�ɫ
			clrBG		= RGB(50,205,50);	// �󱳾���ˢɫ
		}
		else if(m_nBeamStatesShow == 2)
		{
			clrInner	= RGB(0,160,230);	// �ڱ�����ˢɫ
			clrPen		= RGB(80,80,80);	// �߿򻭱�ɫ
			clrBG		= RGB(0,0,250);		// �󱳾���ˢɫ
		}
		else	//�����ť����
		{
			clrInner	= RGB(128,128,128);	// �ڱ�����ˢɫ
			clrPen		= RGB(128,128,128);	// �߿򻭱�ɫ
			clrBG		= RGB(128,128,128);	// �󱳾���ˢɫ
			nLeft = nTop = 4;
		}
		CBrush brushInner(clrInner);	//�ڱ�����ˢ
		CPen pen(0,1,clrPen);			//�߿򻭱�
		CBrush brushBG(clrBG);			//�󱳾���ˢ
		dc.FillRect(&(lpDrawItemStruct->rcItem), &brushBG);	//���û�ˢbrush��������ο�	
		CRect rect = lpDrawItemStruct->rcItem;
		CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		POINT pt;
		//����ť����߿�����һ���뾶Ϊ10��Բ�Ǿ���
		pt.x = 10;
		pt.y = 10;
		CPen* hOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(&rect, pt);
		pDC->SelectObject(hOldPen);
		rect.DeflateRect(3, 3, 3, 3);	//����
		CBrush *pOldBrush = pDC->SelectObject(&brushBG);  
		pDC->Rectangle(rect);			//������
		pDC->SelectObject(pOldBrush);
		pDC->FillRect(rect,&brushInner );	//����ھ���
		//��Ϊ����������ػ�,��������ҲҪ�ػ�
		DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
		dc.Detach();
/*
		if(m_nBeamStatesShow == 1)	//����ť����
		{
			CBrush brush(RGB(50,205,50));			//�ڱ�����ˢ
			CPen m_BoundryPen(0,1,RGB(20,20,20));	//�߿򻭱�
			CBrush m_BackgroundBrush=RGB(50,205,50);//�󱳾���ˢ
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);	//���û�ˢbrush��������ο�	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//����ť����߿�����һ���뾶Ϊ10��Բ�Ǿ���
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(3, 3, 3, 3);	//����
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);			//������
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );	//����ھ���
			//��Ϊ����������ػ�,��������ҲҪ�ػ�
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
		}	
		else if(m_nBeamStatesShow ==2)
		{
			CBrush brush(RGB(0, 160, 230));			//�ڱ�����ˢ
			CPen m_BoundryPen(0,1,RGB(80,80,80));	//�߿򻭱�
			CBrush m_BackgroundBrush=RGB(0,0,250);	//�󱳾���ˢ
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);	//���û�ˢbrush��������ο�	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//����ť����߿�����һ���뾶Ϊ10��Բ�Ǿ���
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(3, 3, 3, 3);   //����
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);			//������
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );    //����ھ���
			//��Ϊ����������ػ�,��������ҲҪ�ػ�
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);	
		}
		else    //�����ť����
		{
			CBrush brush(RGB(128, 128, 128));			//�ڱ�����ˢ
			CPen m_BoundryPen(0,1,RGB(128,128,128));	//�߿򻭱�
			CBrush m_BackgroundBrush=RGB(128,128,128);	//�󱳾���ˢ
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);	//���û�ˢbrush�������ο�	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//����ť����߿�����һ���뾶Ϊ5��Բ�Ǿ���
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(4, 4, 3, 3);   
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);   
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );
			//��Ϊ����������ػ�,��������ҲҪ�ػ�
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
		}
		dc.Detach();*/
	}
}

void CMainFrame::Beam_Timer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case	1:
		if(NotZero(m_fBeamCur))
			UpdateData(FALSE);
		break;
	case	2:
		m_nBeamLandNumNew = m_comboBeamTar.GetCurSel();
		if( m_nBeamLandNumOld != m_nBeamLandNumNew)
		{
			m_nBeamLandNumOld = m_nBeamLandNumNew;
			m_bBeamFlag =TRUE;
		}
		break;
	case	3:
		m_nBeamCount++;
		Beam_AutoProcess();
		break;
	case	4:
		{
			BOOL bIsMoving = TRUE;
			//�Զ�̨����
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage >4 )
				bIsMoving = StageTMCM_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage >3 )
				bIsMoving = StageMC6600_IsMoving();
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage >1 )
				bIsMoving = StageMC600_IsMoving();
			if(!bIsMoving)
			{
				KillTimer(4);
				CString str1;
				m_nBeamLand = m_fBeamCur;
				str1.Format( "%d", m_nBeamLand );
				m_comboBeamTar.DeleteString(6);
				m_comboBeamTar.InsertString( 6,str1 );
				m_comboBeamTar.SetCurSel(6);
				m_nBeamLandNumOld =6;
			}
			//�ֶ�̨-����
			/*KillTimer(4);
			CString str1;
			m_nBeamLand = m_fBeamCur;
			str1.Format( "%d", m_nBeamLand );
			m_comboBeam.DeleteString(6);
			m_comboBeam.InsertString( 6,str1 );
			m_comboBeam.SetCurSel(6);
			m_nBeamLandNumOld =6;*/
		}
		break;
	}
}

void CMainFrame::Beam_UpdateStagePos( int nIndex )
{
	switch( nIndex )
	{
	case	0:
		{
			//������Ʒ̨-��ȡ��ǰ��Ʒ̨λ��\�·�Ŀ��λ��-�����ڱ�λ��
			//��ȡ��ǰλ��
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				StageTMCM_GetParam( 2, m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				StageMC6600_GetParam( 2, m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
				StageMC600_GetParam( 2, m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
			Sleep(200);
			//�·�Ŀ��λ��-�����ڱ�λ��XY
			if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
				StageTMCM_Move2Axis( m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
				StageMC6600_Move2Axis( m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 && StageMC600_IsReady())
				StageMC600_Move5Axis( m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
			else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
				StageMC600_Move4Axis( m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3] );

			char szBuf[255];
			sprintf_s( szBuf, "Beam: MoveTo FaradayCupPos[%.3f, %.3f, %.3f, %.2f, %.2f]", m_fBeamFaradayCupPos[0], m_fBeamFaradayCupPos[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
			theApp.DisplayMsg( szBuf );
		}
		break;
	case	1:	// ������Ʒ̨λ��
		if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 && StageTMCM_IsReady())
			StageTMCM_Move2Axis( m_fBeamStagePosCur[0], m_fBeamStagePosCur[1] );
		else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 && StageMC6600_IsReady())
			StageMC6600_Move2Axis( m_fBeamStagePosCur[0], m_fBeamStagePosCur[1] );
		else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 2 && StageMC600_IsReady())
			StageMC600_Move5Axis( m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
		else if(CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 && StageMC600_IsReady())
			StageMC600_Move4Axis( m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3] );

		char szBuf[255];
		sprintf_s( szBuf, "Beam: Reset Stage Pos[%.3f, %.3f, %.3f, %.2f, %.2f]", m_fBeamStagePosCur[0], m_fBeamStagePosCur[1], m_fBeamStagePosCur[2], m_fBeamStagePosCur[3], m_fBeamStagePosCur[4] );
		theApp.DisplayMsg( szBuf );
		break;
	}
}

void CMainFrame::Beam_UpdateCtrls( int nIndex )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal	= -1;
	Flag.lVal			= 1;
	switch( nIndex )
	{
	case	0:	// ����Ŀ��Ŵ���
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		Xpos.dblVal		= m_dBeamMagTar;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag  );
		break;
	case	1:	// ���÷Ŵ���
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		Xpos.dblVal		= m_dBeamMagCur;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag  );
		break;
	case	2:	// ���þ۹⾵
		ClassID.lVal	= SemControlPanelClass_CondenserLens;
		Xpos.lVal		= m_nBeamFineAper;	// 550.00 [C4C0]
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		break;
	}
}
