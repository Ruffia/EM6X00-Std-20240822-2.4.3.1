#include "stdafx.h"
#include "WorkItemAnalyzeClientMsg.h"
#include "KYKYCommFIFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


WorkItemAnalyzeClientMsg::WorkItemAnalyzeClientMsg(KYKYComm_Recv_Package& oMsg):
	msg(oMsg)
{ 
}

void WorkItemAnalyzeClientMsg::DoWork(void* pThreadContext)
{ 
	char szBuf[768] = {0};
	char szAnalyze[512] = {0};
	char *exd;
	do 
	{
		// [{"method":"sem.getvalue","params":{"Name":"WD"}}]
		// [{"method":"sem.getvalue","params":[{"Name":"Mag"},{"Name":"WD"}]}]
		if( strstr( msg.szRecv, "}]") == NULL )
			break;
		if( strstr( msg.szRecv, "}}]") != NULL )
		{
			exd = strstr(msg.szRecv, "}}]");	// +3;
			int result = exd -msg.szRecv +1;	// position
			if( exd != NULL )
			{
				if( result +3 < strlen(msg.szRecv) )
				{
					strcpy_s( szBuf, strstr(msg.szRecv, "}}]") +3 );
					*(exd +3) = '\0';
				}
				else
					szBuf[0] = '\0';
			}
		}
		else if( strstr(msg.szRecv, "}]}]") != NULL )
		{
			exd = strstr(msg.szRecv, "}]}]");	// +4;
			int result = exd -msg.szRecv +1;	// position
			if( exd != NULL )
			{
				if( result +4 < strlen(msg.szRecv) )
				{
					strcpy_s( szBuf, strstr(msg.szRecv, "}]}]") +4 );
					*(exd +4) = '\0';
				}
				else
					szBuf[0] = '\0';
			}
		}
		else
			szBuf[0] = '\0';
		if( strchr( msg.szRecv, '{') != NULL )
		{
			strcpy_s( szAnalyze, strchr( msg.szRecv, '{') );
			exd = strrchr( szAnalyze, ']' );
			if( exd != NULL )
				*exd = '\0';
			KYKYCommJSON_Recv recvClient;
			recvClient.strRecv = szAnalyze;
			//					EnterCriticalSection(&csFifoSendToHost);
			CFIFO<KYKYCommJSON_Recv, KYKYCommJSON_Recv&>& m_fifoSendToHost = CommFIFO::Instance().GetSendToHostFIFO();
			m_fifoSendToHost.Input(recvClient);
			//					LeaveCriticalSection(&csFifoSendToHost);
		}
		strcpy_s( msg.szRecv, szBuf );
	} while (strchr( msg.szRecv, '}]') != NULL);
}
