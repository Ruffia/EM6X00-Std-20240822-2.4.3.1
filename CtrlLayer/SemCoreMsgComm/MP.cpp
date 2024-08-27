// MP.cpp: implementation of the CMP class.
//
// Last Modify : 2011.07.19

#include "stdafx.h"
#include "USB_TransceiverNew.h"
#include "MP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMP& CMP::Instance()
{
	static CMP Inst;
	return Inst;
}

CMP::CMP()
{
	m_bReady		= FALSE;

	m_pDI16A		= NULL;
	m_bUSBReady		= FALSE;
}

CMP::~CMP()
{
	Close();
}

void CMP::Close()
{
	m_bUSBReady = FALSE;
	if ( m_pDI16A != NULL )
	{
		m_pDI16A->KillThread();
		m_pDI16A = NULL;
	}
}

void CMP::DeleteAllTransceiver()
{
	m_bUSBReady = FALSE;
	if ( m_pDI16A != NULL )
	{
		delete m_pDI16A;
		m_pDI16A = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// USB
///////////////////////////////////////////////////////////////////////////////

BOOL CMP::USB_Init( )
{
	m_pDI16A = new CUSB_TransceiverNew();
	if ( m_pDI16A == NULL )
	{
		return FALSE;
	}

	if (!m_pDI16A->CreateThread(CREATE_SUSPENDED))
	{
		DeleteAllTransceiver();
		return FALSE;
	}

	// Sartup Comm with DI16A
	//VERIFY(m_pDI16A->SetThreadPriority(THREAD_PRIORITY_IDLE));
	VERIFY(m_pDI16A->SetThreadPriority(THREAD_PRIORITY_NORMAL));
	//VERIFY(m_pDI16A->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));

	// Now the thread can run wild
	m_pDI16A->ResumeThread();

	m_bUSBReady = TRUE;

	return TRUE;
}

//==2004-5-19_为SEM消息添加校验位======================
// 修改标示：2004-5-19_为SEM消息添加校验位
// 修改日期：2004年5月19日星期三
// 修改人：罗鸣华
// 修改原因：
//		为SEM消息添加校验位。

// 本次修改前代码副本
/*-----------------------------------------------------
-----------------------------------------------------*/

// 本次修改后代码
//-----------------------------------------------------
BOOL CMP::USB_MSGCheckXOR( USB_SEM_ControlMessage& hostMSG )
{
	BYTE	bXOR;
	BYTE	bMessageLenght = hostMSG.bCtlMSGLength;

	hostMSG.bCtlMSGLength += 1;
	bXOR = hostMSG.bCtlMSGLength;
	for(int i=0; i<bMessageLenght; i++)
	{
		bXOR ^= hostMSG.bData[i];
	}
	hostMSG.bData[ bMessageLenght ] = bXOR;

	return TRUE;
}
//-----------------------------------------------------
//==2004-5-19_为SEM消息添加校验位======================


BOOL CMP::USB_PostMSG( USB_SEM_ControlMessage& hostMSG )
{
	if ( !m_bUSBReady ) return FALSE;

//	USB_MSGCheckXOR( hostMSG );	// 2004-5-19_为SEM消息添加校验位

	return m_pDI16A->PostMSGtoMicrocomputer( hostMSG );
}

BOOL CMP::USB_PostMSGEx( USB_SEM_ControlMessage& hostMSG )
{
	if ( !m_bUSBReady ) return FALSE;

//	USB_MSGCheckXOR( hostMSG );	// 2004-5-19_为SEM消息添加校验位

	return m_pDI16A->PostMSGtoFPGA( hostMSG );
}

BOOL CMP::USB_PostMSGtoHost( USB_SEM_ControlMessage& mciMSG )
{
	if ( !m_bUSBReady ) return FALSE;

//	USB_MSGCheckXOR( hostMSG );	// 2004-5-19_为SEM消息添加校验位

	return m_pDI16A->PostMSGtoHost( mciMSG );
}

BOOL CMP::USB_GetMSG( USB_SEM_ControlMessage& mcMSG )
{
	if ( !m_bUSBReady ) return FALSE;

	return m_pDI16A->GetMSGfromMicrocomputer( mcMSG );
}

BOOL CMP::USB_GetLastError( CString& csError )
{
	if ( !m_bUSBReady ) return FALSE;

	return m_pDI16A->GetLastErrorInformation( csError );
}

BOOL CMP::USB_GetStatistics( USB_COMM_Statistics& Statistics )
{
	if ( !m_bUSBReady ) return FALSE;

	Statistics.dwHostMsgPostRate			= m_pDI16A->m_HostMsgPostRate.GetLoopRate();
	Statistics.dwHostMsgFifoOverflowRate	= m_pDI16A->m_HostMsgFifoOverflowRate.GetLoopRate();

	Statistics.dwHostMsgSendRate	= m_pDI16A->m_HostMsgSendRate.GetLoopRate();
	Statistics.dwZ80MsgReceiveRate	= m_pDI16A->m_Z80MsgReceiveRate.GetLoopRate();

	return TRUE;
}

BOOL CMP::USB_SetCommPause( BOOL blPause )
{
	if ( !m_bUSBReady ) return FALSE;

	m_pDI16A->SetPause( blPause );

	return TRUE;
}

BOOL CMP::USB_SetCoadjustParam( double dHV = 30.0 )
{
	if ( !m_bUSBReady ) return FALSE;

	m_pDI16A->SetCoadjustParam( dHV );

	return TRUE;
}
