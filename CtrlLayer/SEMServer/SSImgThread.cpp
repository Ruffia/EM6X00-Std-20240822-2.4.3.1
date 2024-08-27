// SSImgThread.cpp : implementation file
//

#include "stdafx.h"
#include "SSImgThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE g_hSSocketThreadImg = CreateEvent(NULL,TRUE,FALSE,"SSocketThreadImg");

/////////////////////////////////////////////////////////////////////////////
// CSSImgThread

IMPLEMENT_DYNCREATE(CSSImgThread, CWinThread)

CSSImgThread::CSSImgThread()
{
	m_1iData.bHead1		= 0x99;
	m_1iData.bHead2		= 0x88;

	// ��֡����
	m_MsgImageFrame.bHead1	= 0x99;
	m_MsgImageFrame.bHead2	= 0x88;
	m_MsgImageFrame.bCommand1= KYKYComm_CMD_Image_GetFrameData;
	m_MsgImageFrame.bCommand2= KYKYComm_CMD_Image_GetFrameData;
	// ʵʱ����
	m_MsgImage.bHead1		= 0x99;
	m_MsgImage.bHead2		= 0x88;
	m_MsgImage.bCommand1	= KYKYComm_CMD_Image_Sync;
	m_MsgImage.bCommand2	= KYKYComm_CMD_Image_Sync;
}

CSSImgThread::~CSSImgThread()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_SSocketImg.Close();
		m_hSocket = NULL;
	}
}

BOOL CSSImgThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (!AfxSocketInit())	// ��ʼ��CSocket������õ�
	{
		return CWinThread::InitInstance();	// �����˳�
	}
	// �Է����Socket���ӵ�socket���
	if (m_SSocketImg.Attach(m_hSocket))	// ����Attach֮ǰ�����Socket
	{
		m_SSocketImg.m_pThrd = this;		// ����CSocket�����ӹ������߳�
//		m_SSocketImg.m_thrdIndex = m_thrdIndex;
		m_SSocketImg.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);

		return TRUE;	// �����߳̾Ͳ��������˳�
	}
	return CWinThread::InitInstance();		// �����˳�
}

int CSSImgThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	WaitForSingleObject(g_hSSocketThreadImg, INFINITE);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSSImgThread, CWinThread)
	//{{AFX_MSG_MAP(CSSImgThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Read, OnMsgRead)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Send, OnMsgSend)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Send_Img, OnMsgSendImg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSImgThread message handlers

void CSSImgThread::OnMsgRead(WPARAM wParam,LPARAM lParam)
{
	// ��������
	// �Ȱѽ��������¼��ص�
	// �رս������ݣ���������������ݲ�ͣ��������ɴ��������������дһ�����
// 	ResetEvent(g_hSSocketThreadImg);
// 	m_SSocketImg.AsyncSelect(FD_CLOSE);

	char szRecv[1024];
	int nRecv = m_SSocketImg.Receive(szRecv, 512);
	szRecv[nRecv] = '\0';
// 	m_SSocketImg.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
// 	SetEvent(g_hSSocketThreadImg);		
}

void CSSImgThread::OnMsgSend(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE bcommand = BYTE(wParam);    //commmand
// 	m_SSocketImg.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hSSocketThreadCmd);
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
	case	KYKYComm_CMD_Sys_Disconnect:
		{
//			Send_SetCommand( bcommand );
			m_1iData.bCommand1	= bcommand;
			m_1iData.bCommand2	= bcommand;
			m_1iData.nSendData	= (int)lParam;
			int nLen = m_SSocketImg.Send(&m_1iData, sizeof(m_1iData));
		}
		break;
	}
// 	SetEvent(g_hSSocketThreadCmd);
// 	m_SSocketImg.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
}

void CSSImgThread::OnMsgSendImg(WPARAM wParam, LPARAM lParam)
{
	// ����ͼ������
	BYTE bcommand = BYTE(wParam);    //commmand
// 	m_SSocketImg.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hSSocketThreadImg);
	switch (bcommand)
	{
	case	KYKYComm_CMD_Image_GetFrameData:
		{
			m_MsgImageFrame.msgImgData = *PKYKYCommMsg_ImgFrameData(lParam);
			int nSize = m_MsgImageFrame.msgImgData.rcDest.Width() *m_MsgImageFrame.msgImgData.rcDest.Height();
			int nSendlen = m_SSocketImg.Send(&m_MsgImageFrame,sizeof(BYTE)*4 +sizeof(CRect) +nSize);

			// ����ʱ��һ������
			int iMaxLine = m_MsgImageFrame.msgImgData.rcDest.Height();
			long lBuffLineOffset	= m_MsgImageFrame.msgImgData.rcDest.Width();
			LPSTR		lpBuffLine = (LPSTR)(m_MsgImageFrame.msgImgData.bImageBuff);
			for ( int i=0; i<iMaxLine; i++ )
			{
				lpBuffLine	+= lBuffLineOffset;
			}
			TRACE( "RECEIVE IMAGE Frame\n");
		}
		break;
	case	KYKYComm_CMD_Image_Sync:
		{
			m_MsgImage.msgImgData = *PKYKYCommMsg_ImgData(lParam);
			int nSize = m_MsgImage.msgImgData.rcDest.Width() *m_MsgImage.msgImgData.rcDest.Height();
			int nSendlen = m_SSocketImg.Send(&m_MsgImage,sizeof(BYTE)*4 +sizeof(CRect) +nSize);

			// ����ʱ��һ������
			int iMaxLine = m_MsgImage.msgImgData.rcDest.Height();
			long lBuffLineOffset	= m_MsgImage.msgImgData.rcDest.Width();
			LPSTR		lpBuffLine = (LPSTR)(m_MsgImage.msgImgData.bImageBuff);
			for ( int i=0; i<iMaxLine; i++ )
			{
				lpBuffLine	+= lBuffLineOffset;
			}
			TRACE( "RECEIVE real IMAGE\n");
		}
		break;
	}
// 	SetEvent(g_hSSocketThreadImg);
// 	m_SSocketImg.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
}


/*
///////////////////////////////////////////////////////////
// 1.��6X00������
// 2.JsonTcpTest������IPΪ127.0.0.1��PortΪ3000
//
// 3.JsonTcpTest������
// [{"jsonrpc":"2.0","id":"2","method":"params.sem.getnames","params":[]}]
//
// 4.SEMServer��OnMsgRead���յ������ַ���
// 71:[{"jsonrpc":"2.0","id":"2","method":"params.sem.getnames","params":[]}],
///////////////////////////////////////////////////////////

void CSSImgThread::SSImgSend_JsonString( Json::Value valueSend )
{
	Json::FastWriter writer;
	std::string strSend = writer.write(valueSend);
	const char *sendData = strSend.c_str();
	ZeroMemory( &m_szSSImgSend, sizeof(m_szSSImgSend) );
	int nLen = strlen(sendData);
// 	sprintf( m_szSSImgSend, "%d:%s,", nLen, sendData );
// 	nLen = strlen(m_szSSImgSend);
// 	m_szSSImgSend[nLen] = '\0';
	sprintf( m_szSSImgSend, "%d:[%s", nLen, sendData );
	nLen = strlen(m_szSSImgSend);
	m_szSSImgSend[nLen-1] = '\0';
	strcat(m_szSSImgSend, "],");
	nLen = strlen(m_szSSImgSend);
	m_szSSImgSend[nLen] = '\0';
	
// 	m_SSocketImg.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hSSocketThreadCmd);
//	EnterCriticalSection(&csSSocketCmd);
	nLen = m_SSocketImg.Send(m_szSSImgSend, strlen(m_szSSImgSend));
//	LeaveCriticalSection(&csSSocketCmd);
// 	SetEvent(g_hSSocketThreadCmd);
// 	m_SSocketImg.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	
	char output[1024];
	sprintf( output, "Server: Send_JsonString = %s\n", m_szSSImgSend );
	OutputDebugString( output );
}

void CSSImgThread::Send_SetCommand( BYTE bcommand )
{
	Json::Value valueSend, valueParam;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
		valueSend["method"]	= "app.execute.img";
		valueSend["id"]		= "app.execute.img";
		valueParam["Name"]	= "Connect";
		valueParam["Value"]	= true;
		break;
	case	KYKYComm_CMD_Sys_Disconnect:
		valueSend["method"]	= "app.execute.img";
		valueSend["id"]		= "app.execute.img";
		valueParam["Name"]	= "Disconnect";
		valueParam["Value"]	= true;
		break;
	}
	valueSend["result"] = valueParam;
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	SSImgSend_JsonString( valueSend );
}
*/