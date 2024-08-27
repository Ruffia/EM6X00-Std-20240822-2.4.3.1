// SEMCommClient.cpp: implementation of the CSEMCommClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SEMCommClient.h"
#include "CSCmdThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HWND g_hMain, g_hMain2;
extern KYKYCommMsg_ImgData g_msgImageData[MALLOC_SIZE];
extern int g_nRecvLastIndex;
#define nSleepTime	100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSEMCommClient::CSEMCommClient()
{
	m_pThreadCmd = NULL;
	m_pThreadRecvImg = NULL;
}

CSEMCommClient::~CSEMCommClient()
{
}

void CSEMCommClient::Init(HWND hWnd)
{
	g_hMain = hWnd;
	g_hMain2= hWnd;
}

bool CSEMCommClient::Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4)
{
	CString m_strIPAddr;
	m_strIPAddr.Format("%d.%d.%d.%d",IP1,IP2,IP3,IP4);

	///////////////////////开启命令发送接收线程//////////////////////
	m_pThreadCmd = (CCSCmdThread*)AfxBeginThread(RUNTIME_CLASS(CCSCmdThread),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_pThreadCmd->SetServerIP(m_strIPAddr);
	m_pThreadCmd->ResumeThread();

	///////////////////////打开接收图像数据的线程////////////////////
/*	Sleep(500);
	char *pstr;
	pstr = m_strIPAddr.GetBuffer(15);
	m_pThreadRecvImg = new CThreadRecvImage();
	m_pThreadRecvImg->SetServerIP(pstr);
	m_strIPAddr.ReleaseBuffer();
 	m_pThreadRecvImg->RequestImage();
*/
	return true;
}

void CSEMCommClient::Disconnect(bool bActive)   //主动/被动断开
{
	if (!m_pThreadCmd)  return;
	if (bActive)
	{
		//通知服务器停止发送图像数据
	    TCP_SendCommand(KYKYComm_CMD_Image_Stop);
		m_pThreadCmd->PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Command, 0, KYKYComm_CMD_Sys_Disconnect);
		TRACE("Disconnect active.\r\n");
		Sleep(1000);
	}

	m_pThreadCmd->PostThreadMessage(WM_QUIT, 0, 0);
	m_pThreadCmd = NULL;
	TRACE("delete cmdThread\r\n");

	if (!m_pThreadRecvImg) return;

	m_pThreadRecvImg->StopRecvThread();
	TRACE("stop recvthread\r\n");
	Sleep(2);

	delete m_pThreadRecvImg;
	m_pThreadRecvImg = NULL;
	TRACE("delete recvThread\r\n");
}

bool CSEMCommClient::GetServerStatus(int* status)
{
	if( m_pThreadCmd != NULL )
	{
		m_pThreadCmd->GetServerStatus( status );
		return true;
	}
	return false;
}

bool CSEMCommClient::GetVersion( WORD* ver )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Sys_GetVersion);
//		Sleep(100);
		m_pThreadCmd->GetVersion( ver );
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
BOOL CSEMCommClient::TCP_SendCommand(const int nCommand)
{
	if (!m_pThreadCmd)  return FALSE;
	return m_pThreadCmd->PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Command, 0, LPARAM(nCommand));
}

BOOL CSEMCommClient::TCP_SendData(const int nCommand, void* pData)
{
	if (!m_pThreadCmd)  return FALSE;
	return m_pThreadCmd->PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, nCommand, LPARAM(pData));
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
LPVOID CSEMCommClient::Img_RequestImg()
{
	//开始请求发送图像
	TCP_SendCommand(KYKYComm_CMD_Image_Sync);
	TRACE("g_msgImageData:%x\r\n", g_msgImageData);
	return g_msgImageData;
}

bool CSEMCommClient::Img_GetLastRecvIndex( int* index )
{
	*index = g_nRecvLastIndex;
	return true;
}

bool CSEMCommClient::Img_GetMsgImgSize( int* size )
{
	*size = KYKYComm_MALLOC_SIZE;
	return true;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CSEMCommClient::GetDoubleParam( int index, double* param )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(index);
//		Sleep(nSleepTime);
		return m_pThreadCmd->GetDoubleParam( index, param );
	}
	return false;
}

bool CSEMCommClient::GetDoubleParams( int index, double* param1, double* param2 )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(index);
//		Sleep(nSleepTime);
		return m_pThreadCmd->GetDoubleParams( index, param1, param2 );
	}
	return false;
}

bool CSEMCommClient::SetDoubleParam( int index, double param )
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->SetDoubleParam( index, param );
	return false;
}

bool CSEMCommClient::SetDoubleParams( int index, double param1, double param2 )
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->SetDoubleParams( index, param1, param2 );
	return false;
}

bool CSEMCommClient::GetIntParam( int index, int* param )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(index);
//		Sleep(nSleepTime);
		return m_pThreadCmd->GetIntParam( index, param );
	}
	return false;
}

bool CSEMCommClient::SetIntParam( int index, int param )
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->SetIntParam( index, param );
	return false;
}

bool CSEMCommClient::SetCommand( int cmd )
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->SetCommand( cmd );
	return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CSEMCommClient::StageGetDoubleParam(int index, int axis, float* param)
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(index);
//		Sleep(nSleepTime);
		return m_pThreadCmd->StageGetDoubleParam( index, axis, param );
	}
	return false;
}

bool CSEMCommClient::StageGetDoubleParam2(int index, float* xSt, float* ySt)
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(index);
//		Sleep(nSleepTime);
		return m_pThreadCmd->StageGetDoubleParam2( index, xSt, ySt );
	}
	return false;
}

bool CSEMCommClient::StageGetDoubleParam5(int index, float* xSt, float* ySt, float* zSt, float* rSt, float* tSt)
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(index);
//		Sleep(nSleepTime);
		return m_pThreadCmd->StageGetDoubleParam5( index, xSt, ySt, zSt, rSt, tSt );
	}
	return false;
}

bool CSEMCommClient::StageSetPos(int axis, float pos)
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->StageSetPos( axis, pos );
	return false;
}

bool CSEMCommClient::StageSetPos2(float xSt, float ySt)
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->StageSetPos2( xSt, ySt );
	return false;
}

bool CSEMCommClient::StageSetPos5(float xSt, float ySt, float zSt, float rSt, float tSt)
{
	if( m_pThreadCmd != NULL )
		return m_pThreadCmd->StageSetPos5( xSt, ySt, zSt, rSt, tSt );
	return false;
}

/////////////////////////////////////////////////////////////////////

bool CSEMCommClient::StageGetStatus( int* status )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Stage_GetStatus);
//		Sleep(100);
		m_pThreadCmd->StageGetStatus( status );
		return true;
	}
	return false;
}

bool CSEMCommClient::StageGetGratings(int* xSt, int* ySt)
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Stage_GetGratings);
//		Sleep(100);
		m_pThreadCmd->StageGetGratings( xSt, ySt );
	}
	return false;
}

bool CSEMCommClient::StageStop()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Stage_Stop);
		return true;
	}
	return false;
}

bool CSEMCommClient::StageGoHome()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Stage_GoHome);
		return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool CSEMCommClient::FuncAutoCstBrt()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Func_AutoBC);
		return true;
	}
	return false;
}

bool CSEMCommClient::FuncShutdownBeam()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_Func_ShutdownBeam);
		return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

bool CSEMCommClient::ScanChangeSpeedGear( int gear )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_ScanCtrl_Speed1 +gear -1);
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanChangeResoGear( int gear )
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_ScanCtrl_Reso1 +gear -1);
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanStart()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_ScanCtrl_Normal);
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanFreeze()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_ScanCtrl_Freeze);
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanStartSync()
{
	if( m_pThreadCmd != NULL )
	{
		TCP_SendCommand(KYKYComm_CMD_ScanCtrl_Sync);
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanStartSpot( int x, int y )
{
	if( m_pThreadCmd != NULL )
	{
		m_pThreadCmd->ScanStartSpot( x, y );
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanStartHLine( int y )
{
	if( m_pThreadCmd != NULL )
	{
		m_pThreadCmd->ScanStartHLine( y );
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanStartVLine( int x )
{
	if( m_pThreadCmd != NULL )
	{
		m_pThreadCmd->ScanStartVLine( x );
		return true;
	}
	return false;
}

bool CSEMCommClient::ScanStartArea(int left, int top, int right, int bottom)
{
	if( m_pThreadCmd != NULL )
	{
		m_pThreadCmd->ScanStartArea( left, top, right, bottom );
		return true;
	}
	return false;
}
