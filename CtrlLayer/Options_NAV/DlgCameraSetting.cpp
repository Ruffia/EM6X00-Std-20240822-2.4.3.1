// CameraSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCameraSetting.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include <fstream >
#include <string.h>
#include <string>
#include "PanelNAV.h"

// CDlgCameraSetting 对话框

//IMPLEMENT_DYNAMIC(CDlgCameraSetting, CDialog)

CDlgCameraSetting::CDlgCameraSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCameraSetting::IDD, pParent)
{
	m_pcMyCamera		= NULL;
	m_fMoveTimeStart	= 20000;
	m_fMoveTimeEnd		= 12000;
	m_navSets.move.posOpen.fPosX	= 0;
	m_navSets.move.posOpen.fPosY	= 0;
	m_navSets.move.posOpen.fPosZ	= 0;
	m_navSets.move.posOpen.fPosR	= 0;
	m_navSets.move.posOpen.fPosT	= 0;
	m_navSets.imgShow.nHorizontal		= 0;
	m_navSets.imgShow.nVertical			= 0;
	m_navSets.imgShow.nLeftRotateCount	= 0;
	m_navSets.imgShow.nRightRotateCount	= 0;
}

CDlgCameraSetting::~CDlgCameraSetting()
{
}

void CDlgCameraSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgCameraSetting, CDialog)	
	ON_BN_CLICKED(IDC_RADIO_NAV_Trigger_OFF, &CDlgCameraSetting::OnTriggerContinuous)	
	ON_BN_CLICKED(IDC_RADIO_NAV_Trigger_ON, &CDlgCameraSetting::OnTriggerHard)
	ON_BN_CLICKED(IDC_BUTTON_NAV_SavePath, &CDlgCameraSetting::OnSavePath)	
	ON_BN_CLICKED(IDC_BUTTON_NAV_WORLD, &CDlgCameraSetting::OnCreateWorldCoordinateSystem)
	ON_BN_CLICKED(IDC_BUTTON_NAV_DistortionCorrection, &CDlgCameraSetting::OnDistortionCorrection)
	ON_BN_CLICKED(IDC_BUTTON_NAV_SetOpenImage, &CDlgCameraSetting::OnOpenImage)
	ON_BN_CLICKED(IDC_BUTTON_NAV_ReadParam, &CDlgCameraSetting::OnReadParam)
	ON_BN_CLICKED(IDC_BUTTON_NAV_Calibration, &CDlgCameraSetting::OnCalibration)
	ON_BN_CLICKED(IDC_BUTTON_NAV_Browse, &CDlgCameraSetting::OnBrowse)
END_MESSAGE_MAP()

// CDlgCameraSetting 消息处理程序

void CDlgCameraSetting::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	char szBuf[255];
	int nLen = 50;
	// 界面元素
	if(GetPrivateProfileString("CameraSetting Label", "Cap", "", szBuf, nLen, m_cPathLocalize) != 0)
		SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "ExposeTime", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_NAV_Exposure)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "Gain", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_NAV_Gain)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "ContinuousMode", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_NAV_Trigger_OFF)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "HardMode", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_NAV_Trigger_ON)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "HardModeDirectory", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_NAV_HardModeDirectory)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "Browse", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_Browse)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "OpenImage", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_SetOpenImage)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "ReadArgument", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_ReadParam)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "DistortionCorrection", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_DistortionCorrection)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "CreateWordAxis", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_WORLD)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "Save", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_SavePath)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "Calibration", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_NAV_Calibration)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("CameraSetting Label", "PARAMETER_STATIC", "", szBuf, nLen, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_NAV_Group_Param)->SetWindowText( szBuf );
} 

BOOL CDlgCameraSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// ch:获取曝光时间 | en:Get Exposure Time
int CDlgCameraSetting::GetExposureTime()
{
	MVCC_FLOATVALUE stFloatValue = {0};	
	int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &stFloatValue);	
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nExposureTime = stFloatValue.fCurValue;
	return MV_OK;
}

// ch:设置曝光时间 | en:Set Exposure Time
int CDlgCameraSetting::SetExposureTime()
{
	// ch:调节这两个曝光模式，才能让曝光时间生效
	// en:Adjust these two exposure mode to allow exposure time effective
	int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
	return m_pcMyCamera->SetFloatValue("ExposureTime", (float)m_navSets.camera.nExposureTime);
}

// ch:获取增益 | en:Get Gain
int CDlgCameraSetting::GetGain()
{
	MVCC_FLOATVALUE stFloatValue = {0};
	int nRet = m_pcMyCamera->GetFloatValue("Gain", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nGain = stFloatValue.fCurValue;
	return MV_OK;
}

// ch:设置增益 | en:Set Gain
int CDlgCameraSetting::SetGain()
{
	// ch:设置增益前先把自动增益关闭，失败无需返回
	//en:Set Gain after Auto Gain is turned off, this failure does not need to return
	m_pcMyCamera->SetEnumValue("GainAuto", 0);
	return m_pcMyCamera->SetFloatValue("Gain", (float)(m_navSets.camera.nGain));
}

// ch:获取帧率 | en:Get Frame Rate
int CDlgCameraSetting::GetFrameRate()
{
	MVCC_FLOATVALUE stFloatValue = {0};
	int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nFrame = stFloatValue.fCurValue;
	return MV_OK;
}

// ch:设置帧率 | en:Set Frame Rate
int CDlgCameraSetting::SetFrameRate()
{
	int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	return m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", (float)(m_navSets.camera.nFrame));
}

//ch:设置相机分辨率
int CDlgCameraSetting::SetCameraSize()
{
	int nRet = m_pcMyCamera->SetIntValue("Width", m_navSets.camera.nCameraW);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	nRet = m_pcMyCamera->SetIntValue("Height", m_navSets.camera.nCameraH);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	return MV_OK;
}

//ch:获取相机分辨率
int CDlgCameraSetting::GetCameraSize()
{
	MVCC_INTVALUE_EX stFloatValue = {0};
	int nRet = m_pcMyCamera->GetIntValue("Width", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nCameraW = stFloatValue.nCurValue;

	nRet = m_pcMyCamera->GetIntValue("Height", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nCameraH = stFloatValue.nCurValue;

	nRet = m_pcMyCamera->GetIntValue("OffsetX", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nOffsetX = stFloatValue.nCurValue;

	nRet = m_pcMyCamera->GetIntValue("OffsetY", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_navSets.camera.nOffsetY = stFloatValue.nCurValue;
	return MV_OK;
}

int CDlgCameraSetting::SetCameraROI()
{
	int nRet = m_pcMyCamera->SetIntValue("OffsetX", m_navSets.camera.nOffsetX);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	nRet = m_pcMyCamera->SetIntValue("OffsetY", m_navSets.camera.nOffsetY);
	if (MV_OK != nRet)
	{
		return nRet;
	}  
	return MV_OK;
}

int CDlgCameraSetting::GetCameraROI()
{
	return 0;
}

void CDlgCameraSetting::ConfigRead()
{
	//读取设置选项文件
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\NAV\\nav.ini");

	char szBuf[255];
	int nSize = 50;
	if(GetPrivateProfileString("StageMoveSetting", "StartMove_X", "", szBuf, nSize, path) != 0)
		m_navSets.move.posStart.fPosX = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "StartMove_Y", "", szBuf, nSize, path) != 0)
		m_navSets.move.posStart.fPosY = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "StartMove_Z", "", szBuf, nSize, path) != 0)
		m_navSets.move.posStart.fPosZ = atof(szBuf);
	sprintf_s( szBuf, "NAV: StartMove = [%.6f, %.6f, %.6f]\r\n", m_navSets.move.posStart.fPosX, m_navSets.move.posStart.fPosY, m_navSets.move.posStart.fPosZ );
	OutputDebugString(szBuf);

	if(GetPrivateProfileString("StageMoveSetting", "EndMove_X", "", szBuf, nSize, path) != 0)
		m_navSets.move.posEnd.fPosX = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "EndMove_Y", "", szBuf, nSize, path) != 0)
		m_navSets.move.posEnd.fPosY = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "EndMove_Z", "", szBuf, nSize, path) != 0)
		m_navSets.move.posEnd.fPosZ = atof(szBuf);
	sprintf_s( szBuf, "NAV: EndMove = [%.6f, %.6f, %.6f]\r\n", m_navSets.move.posEnd.fPosX, m_navSets.move.posEnd.fPosY, m_navSets.move.posEnd.fPosZ );
	OutputDebugString(szBuf);

	// opensettings
	if(GetPrivateProfileString("StageMoveSetting", "OpenMove_X", "", szBuf, nSize, path) != 0)
		m_navSets.move.posOpen.fPosX = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "OpenMove_Y", "", szBuf, nSize, path) != 0)
		m_navSets.move.posOpen.fPosY = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "OpenMove_Z", "", szBuf, nSize, path) != 0)
		m_navSets.move.posOpen.fPosZ = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "OpenMove_R", "", szBuf, nSize, path) != 0)
		m_navSets.move.posOpen.fPosR = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "OpenMove_T", "", szBuf, nSize, path) != 0)
		m_navSets.move.posOpen.fPosT = atof(szBuf);
	sprintf_s( szBuf, "NAV: OpenMove = [%.6f, %.6f, %.6f, %.6f, %.6f]\r\n",
			m_navSets.move.posOpen.fPosX, m_navSets.move.posOpen.fPosY, m_navSets.move.posOpen.fPosZ, m_navSets.move.posOpen.fPosR, m_navSets.move.posOpen.fPosT );
	OutputDebugString(szBuf);

	if(GetPrivateProfileString("StageMoveSetting", "gX1", "", szBuf, nSize, path) != 0)
		m_navSets.move.strExpressionX = szBuf;
	if(GetPrivateProfileString("StageMoveSetting", "gY1", "", szBuf, nSize, path) != 0)
		m_navSets.move.strExpressionY = szBuf;
	if(GetPrivateProfileString("StageMoveSetting", "diffPos", "", szBuf, nSize, path) != 0)
		m_navSets.move.fDiffPos = atof(szBuf);
	if(GetPrivateProfileString("StageMoveSetting", "Process", "", szBuf, nSize, path) != 0)
		m_navSets.move.nProcess = atof(szBuf);
	sprintf_s( szBuf, "NAV: Expression = %s, %s, diffPos = %.6f, Process = %d\r\n",
			m_navSets.move.strExpressionX, m_navSets.move.strExpressionY, m_navSets.move.fDiffPos, m_navSets.move.nProcess );
	OutputDebugString(szBuf);

	if(GetPrivateProfileString("ImageStorageSetting", "Storage_Imgpath", "", szBuf, nSize, path) != 0)
		m_navSets.imgStorage.strPath = szBuf;
	if(GetPrivateProfileString("ImageStorageSetting", "Storgae_ImgTick", "", szBuf, nSize, path) != 0)
		m_navSets.imgStorage.nTick = atoi(szBuf);
	sprintf_s( szBuf, "NAV: imgStoragePath = %s, imgStorageTick = %d\r\n", m_navSets.imgStorage.strPath, m_navSets.imgStorage.nTick );
	OutputDebugString(szBuf);

	if(GetPrivateProfileString("CameraSetting", "Zoom", "", szBuf, nSize, path) != 0)
		m_navSets.camera.fZoom = atof(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_Exposure", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nExposureTime = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_Gain", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nGain = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_Frame", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nFrame = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_W", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nCameraW = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_H", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nCameraH = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_X", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nOffsetX = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_y", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nOffsetY = atoi(szBuf);
	sprintf_s( szBuf, "NAV: ExposureTime = %d, Gain = %d, Frame = %d, Zoom = %.6f\r\n",
			m_navSets.camera.nExposureTime, m_navSets.camera.nGain, m_navSets.camera.nFrame, m_navSets.camera.fZoom );
	OutputDebugString(szBuf);
	sprintf_s( szBuf, "NAV: CameraW = %d, CameraH = %d, OffsetX = %d, OffsetY = %d\r\n",
			m_navSets.camera.nCameraW, m_navSets.camera.nCameraH, m_navSets.camera.nOffsetX, m_navSets.camera.nOffsetY );
	OutputDebugString(szBuf);

	if(GetPrivateProfileString("CameraSetting", "Camera_TiggerMode", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nTriggerMode = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_TriggerSource", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nTriggerSource = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_LineSelector", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nTriggerLine = atoi(szBuf);
	if(GetPrivateProfileString("CameraSetting", "Camera_TiggerActivation", "", szBuf, nSize, path) != 0)
		m_navSets.camera.nTriggerActivate = atoi(szBuf);
	sprintf_s( szBuf, "NAV: TriggerMode = %d, TriggerSource = %d, TriggerLine = %d, TriggerActivate = %d\r\n",
			m_navSets.camera.nTriggerMode, m_navSets.camera.nTriggerSource, m_navSets.camera.nTriggerLine, m_navSets.camera.nTriggerActivate );
	OutputDebugString(szBuf);

	if(GetPrivateProfileString("ImageShowSetting", "Hor", "", szBuf, nSize, path) != 0)
		m_navSets.imgShow.nHorizontal = atoi(szBuf);
	if(GetPrivateProfileString("ImageShowSetting", "Ver", "", szBuf, nSize, path) != 0)
		m_navSets.imgShow.nVertical = atoi(szBuf);
	if(GetPrivateProfileString("ImageShowSetting", "LeftRotateCount", "", szBuf, nSize, path) != 0)
		m_navSets.imgShow.nLeftRotateCount = atoi(szBuf);
	if(GetPrivateProfileString("ImageShowSetting", "RightRotateCount", "", szBuf, nSize, path) != 0)
		m_navSets.imgShow.nRightRotateCount = atoi(szBuf);
	sprintf_s( szBuf, "NAV: Hor = %d, Ver = %d, LeftRotateCount = %d, RightRotateCount = %d\r\n",
			m_navSets.imgShow.nHorizontal, m_navSets.imgShow.nVertical, m_navSets.imgShow.nLeftRotateCount, m_navSets.imgShow.nRightRotateCount );
	OutputDebugString(szBuf);

	if(m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_OFF)
	{
		((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_OFF ))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_ON ))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_ON ))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_OFF ))->SetCheck(0);
		if( m_navSets.move.nProcess < 1 )
			m_navSets.camera.nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
		else if( m_navSets.move.nProcess < 2 )
			m_navSets.camera.nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
	}

	sprintf_s( szBuf, "%d", m_navSets.camera.nExposureTime );
	GetDlgItem(IDC_EDIT_NAV_Exposure)->SetWindowText(szBuf);
	sprintf_s( szBuf, "%d", m_navSets.camera.nGain );
	GetDlgItem(IDC_EDIT_NAV_Gain)->SetWindowText(szBuf);
	GetDlgItem(IDC_EDIT_NAV_ImgPath)->SetWindowText(m_navSets.imgStorage.strPath);
}

void CDlgCameraSetting::InitCameraSet()
{
	if (m_pcMyCamera)
	{
		int Ret = SetExposureTime();
		if (!Ret)
		{
		}

		Ret = SetGain();
		if (!Ret)
		{
		}

		Ret = SetFrameRate();
		if (!Ret)
		{
		}

		Ret = SetCameraSize();
		if (!Ret)
		{
		}

		if (m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_OFF)
		{
			((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_OFF))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_ON))->SetCheck(0);
			m_pcMyCamera->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_ON))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_OFF))->SetCheck(0);
			m_pcMyCamera->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
			//MV_TRIGGER_SOURCE_LINE0, 0
			int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_navSets.camera.nTriggerSource);
			//m_pcMyCamera->SetEnumValue("LineSelector", triggerLine);

			//0：Rising Edge  1：Falling Edge  2：LevelHigh  3：LevelLow
			Ret = m_pcMyCamera->SetEnumValue("TriggerActivation", m_navSets.camera.nTriggerActivate);
			if(Ret != MV_OK)
			{
				Ret = 0;
			}
		}
	}
}

void  CDlgCameraSetting::ConfigSave( int nIndex )
{
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\NAV\\nav.ini");

	CString str;
	switch( nIndex )
	{
	case	0:	// StageMoveSetting - all
		str.Format( "%.6f", m_navSets.move.posStart.fPosX );
		WritePrivateProfileString( "StageMoveSetting", "StartMove_X", str, path );
		str.Format( "%.6f", m_navSets.move.posStart.fPosY );
		WritePrivateProfileString( "StageMoveSetting", "StartMove_Y", str, path );
		str.Format( "%.6f", m_navSets.move.posStart.fPosZ );
		WritePrivateProfileString( "StageMoveSetting", "StartMove_Z", str, path );
		str.Format( "%.6f", m_navSets.move.posEnd.fPosX );
		WritePrivateProfileString( "StageMoveSetting", "EndMove_X", str, path );
		str.Format( "%.6f", m_navSets.move.posEnd.fPosY );
		WritePrivateProfileString( "StageMoveSetting", "EndMove_Y", str, path );
		str.Format( "%.6f", m_navSets.move.posEnd.fPosZ );
		WritePrivateProfileString( "StageMoveSetting", "EndMove_Z", str, path );
		str.Format( "%.6f", m_navSets.move.fDiffPos );
		WritePrivateProfileString( "StageMoveSetting", "diffPos", str, path );
		WritePrivateProfileString( "StageMoveSetting", "gX1", m_navSets.move.strExpressionX, path );
		WritePrivateProfileString( "StageMoveSetting", "gY1", m_navSets.move.strExpressionY, path );
		break;
	case	1:	// StageMoveSetting: Start
		str.Format( "%.6f", m_navSets.move.posStart.fPosX );
		WritePrivateProfileString( "StageMoveSetting", "StartMove_X", str, path );
		str.Format( "%.6f", m_navSets.move.posStart.fPosY );
		WritePrivateProfileString( "StageMoveSetting", "StartMove_Y", str, path );
		break;
	case	2:
		str.Format( "%.6f", m_navSets.move.posStart.fPosZ );
		WritePrivateProfileString( "StageMoveSetting", "StartMove_Z", str, path );
		break;
	case	3:	// StageMoveSetting: End
		str.Format( "%.6f", m_navSets.move.posEnd.fPosX );
		WritePrivateProfileString( "StageMoveSetting", "EndMove_X", str, path );
		str.Format( "%.6f", m_navSets.move.posEnd.fPosY );
		WritePrivateProfileString( "StageMoveSetting", "EndMove_Y", str, path );
		break;
	case	4:
		str.Format( "%.6f", m_navSets.move.posEnd.fPosZ );
		WritePrivateProfileString( "StageMoveSetting", "EndMove_Z", str, path );
		break;
	case	5:	// Expression & diffPos
		str.Format( "%.6f", m_navSets.move.fDiffPos );
		WritePrivateProfileString( "StageMoveSetting", "diffPos", str, path );
		WritePrivateProfileString( "StageMoveSetting", "gX1", m_navSets.move.strExpressionX, path );
		WritePrivateProfileString( "StageMoveSetting", "gY1", m_navSets.move.strExpressionY, path );
		break;

	case	10:	// CameraSetting
		str.Format( "%.6f", m_navSets.camera.fZoom );
		WritePrivateProfileString( "CameraSetting", "Zoom", str, path );
		str.Format( "%d", m_navSets.camera.nExposureTime );
		WritePrivateProfileString( "CameraSetting", "Camera_Exposure", str, path );
		str.Format( "%d", m_navSets.camera.nGain );
		WritePrivateProfileString( "CameraSetting", "Camera_Gain", str, path );
		str.Format( "%d", m_navSets.camera.nFrame );
		WritePrivateProfileString( "CameraSetting", "Camera_Frame", str, path );

		str.Format( "%d", m_navSets.camera.nCameraW );
		WritePrivateProfileString( "CameraSetting", "Camera_W", str, path );
		str.Format( "%d", m_navSets.camera.nCameraH );
		WritePrivateProfileString( "CameraSetting", "Camera_H", str, path );
		str.Format( "%d", m_navSets.camera.nOffsetX );
		WritePrivateProfileString( "CameraSetting", "Camera_X", str, path );
		str.Format( "%d", m_navSets.camera.nOffsetY );
		WritePrivateProfileString( "CameraSetting", "Camera_y", str, path );

		str.Format( "%d", m_navSets.camera.nTriggerMode );
		WritePrivateProfileString( "CameraSetting", "Camera_TiggerMode", str, path );
		str.Format( "%d", m_navSets.camera.nTriggerSource );
		WritePrivateProfileString( "CameraSetting", "Camera_TriggerSource", str, path );
		str.Format( "%d", m_navSets.camera.nTriggerLine );
		WritePrivateProfileString( "CameraSetting", "Camera_LineSelector", str, path );
		str.Format( "%d", m_navSets.camera.nTriggerActivate );
		WritePrivateProfileString( "CameraSetting", "Camera_TiggerActivation", str, path );
		break;

	case	20:	// ImageStorageSetting
		WritePrivateProfileString( "ImageStorageSetting", "Storage_Imgpath", m_navSets.imgStorage.strPath, path );
		str.Format( "%d", m_navSets.imgStorage.nTick );
		WritePrivateProfileString( "ImageStorageSetting", "Storgae_ImgTick", str, path );
		break;

	case	30:	// ImageShowSetting
		str.Format( "%d", m_navSets.imgShow.nHorizontal );
		WritePrivateProfileString( "ImageShowSetting", "Hor", str, path );
		str.Format( "%d", m_navSets.imgShow.nVertical );
		WritePrivateProfileString( "ImageShowSetting", "Ver", str, path );
		str.Format( "%d", m_navSets.imgShow.nLeftRotateCount );
		WritePrivateProfileString( "ImageShowSetting", "LeftRotateCount", str, path );
		str.Format( "%d", m_navSets.imgShow.nRightRotateCount );
		WritePrivateProfileString( "ImageShowSetting", "RightRotateCount", str, path );
		break;
	}
}

void CDlgCameraSetting::OnTriggerContinuous()
{
	// MV_TRIGGER_MODE_OFF：内触发模式（见用户手册7.1.1触发模式，相机通过设备内部给出的信号采集图像）
	//			实际为视频方式，用于校准。在图片中间画十字
	m_navSets.camera.nTriggerMode = MV_TRIGGER_MODE_OFF;
	((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_OFF))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_ON))->SetCheck(0);
}

void CDlgCameraSetting::OnTriggerHard()
{
	// MV_TRIGGER_MODE_ON：外触发模式（见用户手册7.1.1触发模式，相机通过外部给出的信号采集图像）
	//						外部信号可以是软件信号，也可以是硬件信号，
	//						包含软触发、硬件触发、计数器触发、动作命令控制触发以及自由触发共5种方式）
	m_navSets.camera.nTriggerMode = MV_TRIGGER_MODE_ON;
	switch( m_navSets.move.nProcess )
	{
	case	0:	// Edge
		{
			// 硬件触发：Trigger Source Line 0/ Line 2
			//			外部设备通过相机的I/O 接口与相机进行连接，触发信号由外部设备给到相机进行采图
			m_navSets.camera.nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
			// m_nTriggerLine =  MV_TRIGGER_SOURCE_LINE2;
			// 0：Rising Edge  1：Falling Edge  2：LevelHigh  3：LevelLow
			m_navSets.camera.nTriggerActivate = 1;	// 触发响应方式为下降沿
		}
		break;

	case	1:	// In
		{
			// 软触发：Trigger Source Software
			//			触发信号由软件发出，通过千兆网传输给相机进行采图
			m_navSets.camera.nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
			// 触发源选择软触发即Trigger Source 参数选择为Software 时，可通过Trigger	Software 参数处的Execute 按键发送软触发命令进行采图
			// 设置Command型节点-发送软触发命令
			// 需要先打开【触发模式】，触发源选择【软触发】后才可以设置软触发命令
			//nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
			//if (MV_OK != nRet)
			//{
			//	printf("Error: SetCommandValue fail [%x]\n", nRet);
			//}
		}
		break;

	case	2:	// Out
		break;
	}
	((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_OFF))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_NAV_Trigger_ON))->SetCheck(1);	
}

void CDlgCameraSetting::OnSavePath()
{
	CString str;
	GetDlgItem(IDC_EDIT_NAV_Exposure)->GetWindowText(str);
	m_navSets.camera.nExposureTime = atoi(str);
	GetDlgItem(IDC_EDIT_NAV_Gain)->GetWindowText(str);
	m_navSets.camera.nGain = atoi(str);
	GetDlgItem(IDC_EDIT_NAV_ImgPath)->GetWindowText(m_navSets.imgStorage.strPath);

	for( int i=0; i<31; i=i+10 )
		ConfigSave(i);
	if(m_pcMyCamera && dlg->m_bGrabbing)
	{
		dlg->m_bGrabbing = FALSE;
		Sleep(500);
		m_pcMyCamera->StopGrabbing();
		InitCameraSet();
		dlg->m_bGrabbing = TRUE;
		m_pcMyCamera->StartGrabbing();
	}	
}

void CDlgCameraSetting::OnCreateWorldCoordinateSystem()
{
	BOOL success = ::PostMessage(dlg->m_hWnd, WM_SET_CREATEWORLD, 0, 0);
}

void CDlgCameraSetting::OnDistortionCorrection()
{
	BOOL success = ::PostMessage(dlg->m_hWnd, WM_SET_WRITEWORLD, 0, 0);
}

void CDlgCameraSetting::OnOpenImage()
{
	BOOL success = ::PostMessage(dlg->m_hWnd, WM_SET_OPENIMAGE, 0, 0);
}

void CDlgCameraSetting::OnReadParam()
{
	ConfigRead();
	BOOL success  = ::PostMessage(dlg->m_hWnd, WM_SET_READARGUMENT, 0, 0);
}

void CDlgCameraSetting::OnCalibration()
{
	if(dlg->m_bIsShowCenter)
	{
		dlg->NotifyPos( 0 );
	}
	dlg->m_bIsShowCenter = !dlg->m_bIsShowCenter;
}

void CDlgCameraSetting::OnBrowse()
{
	char szBuf[255]= {0};
	GetPrivateProfileString("CameraSetting Label", "Choose", "", szBuf, 50, m_cPathLocalize);

	TCHAR szBuffer[MAX_PATH] = {0};
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = szBuf;	//_T("选择文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl,szBuffer);

	GetDlgItem(IDC_EDIT_NAV_ImgPath)->SetWindowText(szBuffer);	
	m_navSets.imgStorage.strPath = szBuffer;
}

void CDlgCameraSetting::OnBnClickedSendStartSampler()
{
	BOOL success = ::PostMessage(dlg->m_hWnd, WM_SEND_START_SAMPLER, 0, 0);
}

void CDlgCameraSetting::OnBnClickedSendStopSampler()
{
	BOOL success = ::PostMessage(dlg->m_hWnd, WM_SEND_STOP_SAMPLER, 0, 0);
}

void CDlgCameraSetting::OnBnClickedSendSaveImage()
{
	BOOL success = ::PostMessage(dlg->m_hWnd, WM_SEND_SAVE_IMAGE, 0, 0);
}

void CDlgCameraSetting::OnEnChangeFrameRateEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
}
