// ThreadRecvImage.cpp: implementation of the CThreadRecvImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadRecvImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

KYKYCommMsg_ImgData g_msgImageData[MALLOC_SIZE];
int g_nRecvLastIndex = 0;
/*
#pragma data_seg("SharedData")
//MSGIMAGEDATA *g_msgImageData[MALLOC_SIZE] = {} ;
int g_nRecvLastIndex = 0;
#pragma data_seg()
#pragma comment(linker,"/SECTION:SharedData,RWS")
*/

extern HWND g_hMain;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadRecvImage::CThreadRecvImage()
{
	strcpy(m_chIPAddr, "127.0.0.1");
	m_bEndRecvThread = TRUE;
}

CThreadRecvImage::~CThreadRecvImage()
{
}

void CThreadRecvImage::RequestImage()
{
	DWORD id;
	g_nRecvLastIndex = 0;

	HANDLE h = CreateThread(NULL, 0, ThreadRequestImage, this, 0, &id);
	CloseHandle(h);	
}

DWORD WINAPI CThreadRecvImage::ThreadRequestImage(LPVOID lParam)
{
	CThreadRecvImage* pImageThd = (CThreadRecvImage*)lParam;
	
	////////conncect server/////////////////
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sin;
	sin.sin_family		= AF_INET;
	sin.sin_addr.s_addr	= inet_addr(pImageThd->m_chIPAddr);
	sin.sin_port		= htons(nPort2);
	if ( connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR )
	{
		char tempsize[4];
		int nRecvLen2;

		nRecvLen2 = recv(sock,tempsize, 2, 0);     //接收accept应答
		if (tempsize[0] == 0x5a)
		{
			if(tempsize[1] == KYKYComm_CMD_Sys_Connect_Success)
			{
				PostMessage(g_hMain, WM_USER_KYKYComm_Connect_Status, tempsize[1], 1);
				pImageThd->m_bEndRecvThread = FALSE;
			}
		}

		fd_set fdRecv;
		timeval seltime;
		seltime.tv_sec = 10;
		seltime.tv_usec = 0;

		g_nRecvLastIndex = 0;

		while(!pImageThd->m_bEndRecvThread)
		{
			FD_ZERO(&fdRecv);
			FD_SET(sock, &fdRecv);

			if(select(0, &fdRecv, NULL, NULL, &seltime) <= 0 || !FD_ISSET(sock, &fdRecv) )
			continue;
/*
			int nRevBuf=0;
			int nRevOpt= sizeof(nRevBuf);
			getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(CHAR *)&nRevBuf,&nRevOpt);
			CString s1;
			s1.Format ("nRevBuf = %d \r\n",nRevBuf);
			TRACE( s1 );   //8192
*/
			///////开始接收图像数据///////////////////
			int i;
			CRect rcDest = CRect(0,0,0,0);
			for ( i=0; i<4; i++)
			{
				nRecvLen2 = recv(sock, tempsize, 4, 0);
				switch (i)
				{
				case	0:
					rcDest.left		= *(int *)tempsize; 
					break;
				case	1:
					rcDest.top		= *(int *)tempsize;
					break;
				case	2:
					rcDest.right	= *(int *)tempsize;
					break;
				case	3:
					rcDest.bottom	= *(int *)tempsize;
					break;
				}		
			}

			g_msgImageData[g_nRecvLastIndex].rcDest = rcDest;    //有问题
			int nSize = rcDest.Width()*rcDest.Height();//;32
			if (nSize<=0)
				continue;
			TRACE("rcDest= %d, %d\r\n",rcDest.top, rcDest.bottom);

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

				nRecvLen2 = recv(sock, (char *)pBuf, n, 0);
				memcpy((g_msgImageData[g_nRecvLastIndex].bImageBuff) + nRecvDataCnt, pBuf, nRecvLen2);
				nRecvDataCnt += nRecvLen2;
			}
			free(pBuf);
			pBuf = NULL;
			g_nRecvLastIndex ++;
			if (g_nRecvLastIndex == MALLOC_SIZE)
			{
				g_nRecvLastIndex = 0;
			}
		}
	}
	else
	{
		TRACE("图像线程连接客户端失败！\r\n");	
	}

	closesocket(sock);
	return 0;
}

void CThreadRecvImage::SetServerIP(char * sIP)
{
	strcpy(m_chIPAddr, sIP);
}

void CThreadRecvImage::StopRecvThread()
{
	m_bEndRecvThread = TRUE;
}
