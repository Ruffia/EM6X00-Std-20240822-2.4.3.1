// CSImgThread.cpp : implementation file
//

#include "stdafx.h"
#include "CSImgThread.h"
//#include "Include\\Json\\Json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hMain2;
HANDLE g_hCSocketThreadImg = CreateEvent(NULL, TRUE, FALSE, "CSocketThreadImg");
KYKYCommMsg_ImgFrameData g_msgImageFrameData;//[KYKYComm_MALLOC_SIZE];
KYKYCommMsg_ImgData g_msgImageData[KYKYComm_MALLOC_SIZE];
int g_nRecvLastIndex = 0;

/////////////////////////////////////////////////////////////////////////////
// CCSImgThread

IMPLEMENT_DYNCREATE(CCSImgThread, CWinThread)

CCSImgThread::CCSImgThread()
{
	m_pCSocketImg = new CCSImg();     //!!!problem
}

CCSImgThread::~CCSImgThread()
{
	delete m_pCSocketImg;
	m_pCSocketImg = NULL;
}

BOOL CCSImgThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (!AfxSocketInit())
	{
		return CWinThread::InitInstance();
	}

	if (!m_pCSocketImg->Create())
	{
		AfxMessageBox("socket创建失败！"); 
		return CWinThread::InitInstance();
	}

	if (m_pCSocketImg->Connect(m_strIPAddr, KYKYComm_ListenPort2))
	{
		m_pCSocketImg->m_pThrd = this;
		m_pCSocketImg->m_nPort = KYKYComm_ListenPort2;
		m_pCSocketImg->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
		m_nConnectedImg = 1;
	}
	else
		TRACE("图像线程连接失败！\n");
	if( m_nConnectedImg > 0 )
	{
//		StartThreads();	//(g_hMain2);
		return TRUE;
	}
	return CWinThread::InitInstance();
}

int CCSImgThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	WaitForSingleObject(g_hCSocketThreadImg, INFINITE);    //通过等待，解决了在断开连接时程序崩溃的问题
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCSImgThread, CWinThread)
	//{{AFX_MSG_MAP(CCSImgThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Read, OnMsgRead)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSImgThread message handlers

void CCSImgThread::SetServerIP(CString sIP)
{
	m_strIPAddr = sIP;
}

void CCSImgThread::Reset(LPVOID ptr)
{
//	m_pMainWnd = (CWnd*)ptr; 
}

// 得到了从Server端发来的图像数据？？？？？
void CCSImgThread::OnMsgRead(WPARAM wParam,LPARAM lParam)
{
	BYTE bBuf[4];
	int nRecvLen = m_pCSocketImg->Receive( bBuf, 4 );
	if( bBuf[0] == 0x99 && bBuf[1] == 0x88 )
	{
		if( bBuf[2] < KYKYComm_CMD_Image_Sync )
		{
			// 连接
			if( bBuf[2] == KYKYComm_CMD_Sys_Connect_Success )
				PostMessage(g_hMain2, WM_USER_KYKYComm_Connect_Status, KYKYComm_CMD_Sys_Connect_Success, 3);
			else
				PostMessage(g_hMain2, WM_USER_KYKYComm_Connect_Status, KYKYComm_CMD_Sys_Disconnect, 3);
		}
		else
		{
			int nRecvLen = m_pCSocketImg->Receive( &g_msgImageFrameData, 4*4 );	// 收区域大小
			int nExpectedTotal	= 0;
			if( nRecvLen > 15 )
			{
				// 得到预期数据量
				nExpectedTotal = g_msgImageFrameData.rcDest.Width() * g_msgImageFrameData.rcDest.Height();
				if( nExpectedTotal > 0 )
				{
					// 图像数据
					int nActualTotal	= 0;
					nRecvLen = 0;
					do{
						nRecvLen = m_pCSocketImg->Receive( g_msgImageFrameData.bImageBuff +nActualTotal, sizeof(g_msgImageFrameData.bImageBuff) );
						if( nRecvLen > 0 )
							nActualTotal += nRecvLen;
					} while( nActualTotal < nExpectedTotal -2 );

					char szBuf[255];
					sprintf_s( szBuf, "recv %d (%d) bytes: rcDest = CRect( %d, %d, %d, %d)\n", nActualTotal, nExpectedTotal,
						g_msgImageFrameData.rcDest.left,	g_msgImageFrameData.rcDest.top,
						g_msgImageFrameData.rcDest.right, g_msgImageFrameData.rcDest.bottom );
					TRACE( szBuf );
					// 测试时看一看数据
					int iMaxLine = g_msgImageFrameData.rcDest.Height();
					long lBuffLineOffset	= g_msgImageFrameData.rcDest.Width();
					LPSTR		lpBuffLine = (LPSTR)(g_msgImageFrameData.bImageBuff);
					for ( int i=0; i<iMaxLine; i++ )
						lpBuffLine	+= lBuffLineOffset;
					TRACE( "Client RECEIVE IMAGE\n");
				}
			}
		}
	}
/*	int nCmd = (int)wParam;
	if( nCmd != KYKYComm_CMD_Image_Sync )
	{
		// 接收数据
		// 先把接收数据事件关掉
		// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
// 		ResetEvent(g_hImgCSocketThread);	
// 		m_pCSocketImg->AsyncSelect(FD_CLOSE);
	}
	else
	{
		// 图像数据？？？？？
		int nRecvLen = m_pCSocketImg->Receive( &g_msgImageData, sizeof(g_msgImageData) );
		if( nRecvLen > 0 )
			TRACE( "recv Image data %d\n", nRecvLen );
/*		char szBuf[1024];
		int rcBuf[4];
		CRect rcDest = CRect(0,0,0,0);
		int nRecvLen = m_pCSocketImg->Receive(rcBuf, 4);
		if( nRecvLen == 4 )
		{
			rcDest = CRect( rcBuf[0], rcBuf[1], rcBuf[2], rcBuf[3] );
			if( rcDest.Width() > 2048 || rcDest.Height() > 1536 )
				return;
			g_msgImageData[g_nRecvLastIndex].rcDest = rcDest;
			int nSize = rcDest.Width() *rcDest.Height();
			if( nSize <= 0 )
				return;
			sprintf_s( szBuf, "rcDest = %d, %d\n", rcDest.top, rcDest.bottom );
			TRACE(szBuf);

			int nRecvDataCnt = 0;
			int nWidth = rcDest.Width();
			PBYTE pBuf;
			pBuf = (BYTE *)malloc(nWidth);
			int n, nLeftData = nSize - nRecvDataCnt;
			while(nRecvDataCnt < nSize)
			{
				if (nLeftData < nWidth)
					n = nLeftData;	
				else
					n = nWidth;

				nRecvLen = m_pCSocketImg->Receive((char *)pBuf, n);
				memcpy((g_msgImageData[g_nRecvLastIndex].bImageBuff) + nRecvDataCnt, pBuf, nRecvLen);
				nRecvDataCnt += nRecvLen;
			}
			free(pBuf);
			pBuf = NULL;
			g_nRecvLastIndex ++;
			if (g_nRecvLastIndex == KYKYComm_MALLOC_SIZE)
				g_nRecvLastIndex = 0;
		}
*///	}
	//重新打开接收数据事件
// 	m_pCSocketImg->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
// 	SetEvent(g_hImgCSocketThread);
}
