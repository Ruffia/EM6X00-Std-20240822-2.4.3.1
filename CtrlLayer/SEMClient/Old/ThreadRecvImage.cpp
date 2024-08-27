// ThreadRecvImage.cpp: implementation of the CThreadRecvImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadRecvImage.h"

#include "Include\\Json\\Json.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

KYKYCommMsg_ImgData g_msgImageData[KYKYComm_MALLOC_SIZE];
int g_nRecvLastIndex = 0;
/*
#pragma data_seg("SharedData")
//MSGIMAGEDATA *g_msgImageData[KYKYComm_MALLOC_SIZE] = {} ;
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
	sin.sin_port		= htons(KYKYComm_ListenPort2);
	if ( connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR )
	{
		char szRecv[512], szAnalyze[512], tempsize[4];
		int nRecvLen2;

//		nRecvLen2 = recv(sock,tempsize, 2, 0);     //接收accept应答
		nRecvLen2 = recv(sock,szRecv, 500, 0);     //接收accept应答
		if( strchr( szRecv, '{') != NULL )
		{
			strcpy( szAnalyze, strchr( szRecv, '{') );
			char* exd = strrchr( szAnalyze, ']' );
			if( exd != NULL )
				*exd = '\0';
			std::string strData = szAnalyze;
			// 解析json用Json::Reader
			Json::Reader *readerinfo = new Json::Reader(Json::Features::strictMode());
			// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...
			Json::Value root;       
			if (readerinfo->parse(strData, root))
			{
				std::string strJsonrpc, strID, strMethod, strParams;
				if (root.isMember("jsonrpc"))
					strJsonrpc = root["jsonrpc"].asString();
				
				if (root.isMember("method"))
				{
					strMethod = root["method"].asString();
					const char *method = strMethod.c_str();
					if( strstr( method, "app" ) != NULL )
					{
						const Json::Value param = root["result"];
						if( param.isMember("Name") )
						{
							std::string strMsg = param["Name"].asString();
							const char *name = strMsg.c_str();
							if( strcmp(name, "Connect") == 0 )
							{
								BOOL bValue = param["Value"].asBool();
								if( bValue )
								{
									PostMessage(g_hMain, WM_USER_KYKYComm_Connect_Status, KYKYComm_CMD_Sys_Connect_Success, 3);
									pImageThd->m_bEndRecvThread = FALSE;
								}
							}
						}
					}
				}
			}
			::delete readerinfo;
			readerinfo = NULL;
		}
/*		if (tempsize[0] == 0x5a)
		{
			if(tempsize[1] == KYKYComm_CMD_Sys_Connect_Success)
			{
				PostMessage(g_hMain, WM_USER_KYKYComm_Connect_Status, tempsize[1], 3);
				pImageThd->m_bEndRecvThread = FALSE;
			}
		}
*/
		fd_set fdRecv;
		timeval seltime;
		seltime.tv_sec = 10;
		seltime.tv_usec = 0;

		g_nRecvLastIndex = 0;

		while(!pImageThd->m_bEndRecvThread)
		{
			// 实验：什么都不做
			Sleep(1);
			continue;

			FD_ZERO(&fdRecv);
			FD_SET(sock, &fdRecv);

			int sel = select(0, &fdRecv, NULL, NULL, &seltime);
			if( sel == SOCKET_ERROR )
			{
				int ret = WSAGetLastError();
			}
			if( sel <= 0 || !FD_ISSET(sock, &fdRecv) )
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

			if( rcDest.Width() > 2048 || rcDest.Height() > 1536 )
				continue;
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
			if (g_nRecvLastIndex == KYKYComm_MALLOC_SIZE)
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
