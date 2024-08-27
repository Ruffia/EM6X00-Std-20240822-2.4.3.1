// KYKYSEMComm.cpp : implementation file
//

#include "stdafx.h"
#include "SEMCommClient.h"
#include "KYKYSEMComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKYKYSEMComm

CKYKYSEMComm::CKYKYSEMComm()
{
	m_pClient = NULL;
}

CKYKYSEMComm::~CKYKYSEMComm()
{
	if( m_pClient != NULL )
	{
		delete m_pClient;
		m_pClient = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKYKYSEMComm message handlers

void CKYKYSEMComm::KYKYSEM_Init(HWND hWnd)
{
	m_pClient = new CSEMCommClient();
	if( m_pClient != NULL )
		m_pClient->Init(hWnd);
}

bool CKYKYSEMComm::KYKYSEM_Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4)
{
	if( m_pClient != NULL )
		return m_pClient->Connect(IP1, IP2, IP3, IP4);
	else
		return false;
}

void CKYKYSEMComm::KYKYSEM_Disconnect(bool bActive)   //主动/被动断开
{
	if( m_pClient != NULL )
		m_pClient->Disconnect(bActive);
}

bool CKYKYSEMComm::KYKYSEM_GetServerStatus(int* status)
{
	if( m_pClient != NULL )
		return m_pClient->GetServerStatus( status );
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

bool CKYKYSEMComm::KYKYSEM_GetVersion(WORD* ver)
{
	if( m_pClient != NULL )
		return m_pClient->GetVersion( ver );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetSEMType(int* type)
{
	if( m_pClient != NULL )
		return m_pClient->GetIntParam( KYKYComm_CMD_Sys_GetSEMType, type );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetHVPowerType(int* type)
{
	if( m_pClient != NULL )
		return m_pClient->GetIntParam( KYKYComm_CMD_Sys_GetHighVoltageType, type );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetMagnRange( double* mgn1, double* mgn2 )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParams( KYKYComm_CMD_Get_MagRange, mgn1, mgn2 );
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CKYKYSEMComm::KYKYSEM_GetMagn( double* mgn )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_Magnification, mgn );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetMagn( double mgn )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_Magnification, mgn );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetFOV( double* x, double* y, int* unit )
{
	if( m_pClient != NULL )
	{
		m_pClient->GetDoubleParams( KYKYComm_CMD_Get_FieldOfView, x, y );
		m_pClient->GetIntParam( KYKYComm_CMD_Get_FieldOfView, unit );
		return true;
	}
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetWD( double* wd )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_WorkDistance, wd );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetWD( double wd )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_WorkDistance, wd );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetBrightness( double* brt )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_Brightness, brt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetBrightness( double brt )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_Brightness, brt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetContrast( double* cst )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_Contrast, cst );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetContrast( double cst )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_Contrast, cst );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetStig( double* stx, double* sty )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParams( KYKYComm_CMD_Get_Stigs, stx, sty );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetStig( double stx, double sty )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParams( KYKYComm_CMD_Set_Stigs, stx, sty );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetStigX( double* stx )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_StigX, stx );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetStigX( double stx )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_StigX, stx );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetStigY( double* sty )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_StigY, sty );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetStigY( double sty )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_StigY, sty );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetScnRotAngleInit( double* angle )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_ScanRotAngleInit, angle );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetScnRotAngle( double* angle )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_ScanRotAngle, angle );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetScnRotAngle( double angle )
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_ScanRotAngle, angle );
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CKYKYSEMComm::KYKYSEM_GetVacuumStatus(int* status)
{
	if( m_pClient != NULL )
		return m_pClient->GetIntParam( KYKYComm_CMD_Get_VacStatus, status );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetGunStatus(int* status)
{
	if( m_pClient != NULL )
		return m_pClient->GetIntParam( KYKYComm_CMD_Get_GunStatus, status );
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CKYKYSEMComm::KYKYSEM_GetAccVoltage(double* volt)
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_HighVoltage, volt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetAccVoltage(double volt)	// KV
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_HighVoltage, volt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetFilament(double* fila)
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_Fila, fila );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetFilament(double fila)
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_Fila, fila );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetBeamCurrent(double* curr)
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_BeamCurrent, curr );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetBeamCurrent(double curr)
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_BeamCurrent, curr );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetBias(double* bias)
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_Bias, bias );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_SetBias(double bias)
{
	if( m_pClient != NULL )
		return m_pClient->SetDoubleParam( KYKYComm_CMD_Set_Bias, bias );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_GetEMI(double* emi)
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_EMI, emi );
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

bool CKYKYSEMComm::KYKYSEM_StageGetStatus( int* status )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetStatus( status );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetGratings( int* xSt, int* ySt )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetGratings( xSt, ySt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetRange( int axis, float* range )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetDoubleParam( KYKYComm_CMD_Stage_GetRange, axis, range );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetRange2( float* range )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetDoubleParam2( KYKYComm_CMD_Stage_GetRange2, range );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetRange5( float* range )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetDoubleParam5( KYKYComm_CMD_Stage_GetRange5, range );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetPos( int axis, float* pos )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetDoubleParam( KYKYComm_CMD_Stage_GetPos, axis, pos );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetPos2( float* pos )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetDoubleParam2( KYKYComm_CMD_Stage_GetPos2, pos );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGetPos5( float* pos )
{
	if( m_pClient != NULL )
		return m_pClient->StageGetDoubleParam5( KYKYComm_CMD_Stage_GetPos5, pos );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageSetPos( int axis, float pos )
{
	if( m_pClient != NULL )
		return m_pClient->StageSetPos( axis, pos );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageSetPos2( float xSt, float ySt )
{
	if( m_pClient != NULL )
		return m_pClient->StageSetPos2( xSt, ySt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageSetPos5( float xSt, float ySt, float zSt, float rSt, float tSt )
{
	if( m_pClient != NULL )
		return m_pClient->StageSetPos5( xSt, ySt, zSt, rSt, tSt );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageStop()
{
	if( m_pClient != NULL )
		return m_pClient->StageStop();
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_StageGoHome()
{
	if( m_pClient != NULL )
		return m_pClient->StageGoHome();
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CKYKYSEMComm::KYKYSEM_FuncAutoCstBrt()
{
	if( m_pClient != NULL )
		return m_pClient->FuncAutoCstBrt();
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_FuncShutdownBeam()
{
	if( m_pClient != NULL )
		return m_pClient->FuncShutdownBeam();
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 获取当前图像分辨率：X和Y分别是多少像素
bool CKYKYSEMComm::KYKYSEM_ScanGetResolution( int* rx, int* ry )
{
	if( m_pClient != NULL )
	{
		double dx, dy;
		m_pClient->GetDoubleParams( KYKYComm_CMD_Get_ScanCtrl_Reso, &dx, &dy );
		*rx = (int)dx;
		*ry = (int)dy;
		return true;
	}
	else
		return false;
}

// 设置（切换）图像分辨率档位
bool CKYKYSEMComm::KYKYSEM_ScanSetResolution( int gear )
{
	if( m_pClient != NULL )
//		return m_pClient->ScanChangeResoGear( gear );
		return m_pClient->SetIntParam( KYKYComm_CMD_Set_ScanCtrl_Reso, gear );
	else
		return false;
}

// 获取当前扫描速度是多少us
bool CKYKYSEMComm::KYKYSEM_ScanGetSpeed( double* sp )
{
	if( m_pClient != NULL )
		return m_pClient->GetDoubleParam( KYKYComm_CMD_Get_ScanCtrl_Speed, sp );
	else
		return false;
}

// 设置（切换）扫描速度档位
bool CKYKYSEMComm::KYKYSEM_ScanSetSpeed( int gear )
{
	if( m_pClient != NULL )
//		return m_pClient->ScanChangeSpeedGear( gear );
		return m_pClient->SetIntParam( KYKYComm_CMD_Set_ScanCtrl_Speed, gear );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanStart()
{
	if( m_pClient != NULL )
		return m_pClient->ScanStart();
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanFreeze()
{
	if( m_pClient != NULL )
		return m_pClient->ScanFreeze();
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanStartSpot( int x, int y )
{
	if( m_pClient != NULL )
		return m_pClient->ScanStartSpot( x, y );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanStartHLine( int y )
{
	if( m_pClient != NULL )
		return m_pClient->ScanStartHLine( y );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanStartVLine( int x )
{
	if( m_pClient != NULL )
		return m_pClient->ScanStartVLine( x );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanStartArea(int left, int top, int right, int bottom)
{
	if( m_pClient != NULL )
		return m_pClient->ScanStartArea( left, top, right, bottom );
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanStartSync()
{
	if( m_pClient != NULL )
		return m_pClient->ScanStartSync();
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ScanIsStop(int* status)
{
	if( m_pClient != NULL )
		return m_pClient->GetIntParam( KYKYComm_CMD_ScanCtrl_IsStopScan, status );
	else
		return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

bool CKYKYSEMComm::KYKYSEM_ImgGetSize(int* size)
{
	if( m_pClient != NULL )
		return m_pClient->Img_GetMsgImgSize( size );
	else
		return false;
}
bool CKYKYSEMComm::KYKYSEM_ImgGetLastRecvIndex( int* index)
{
	//获取上次的图像保存索引
	if( m_pClient != NULL )
		return m_pClient->Img_GetLastRecvIndex( index );
	else
		return false;
}

LPVOID CKYKYSEMComm::KYKYSEM_ImgRequestFrame()
{
	if( m_pClient != NULL )
		return m_pClient->Img_RequestFrame();
	else
		return false;
}

LPVOID CKYKYSEMComm::KYKYSEM_ImgRequest()
{
	if( m_pClient != NULL )
		return m_pClient->Img_Request();
	else
		return false;
}

bool CKYKYSEMComm::KYKYSEM_ImgStop()
{
	if( m_pClient != NULL )
		return m_pClient->Img_Stop();
	else
		return false;
}