// CommTestConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "KYKYSEMComm.h"


int _tmain(int argc, _TCHAR* argv[])
{
	printf("Hello World!\n");

	CKYKYSEMComm	m_Client;
	m_Client.KYKYSEM_Init(NULL);
	m_Client.KYKYSEM_Connect(127,0,0,1);

	int nStatus = -1;
	int nCounts = 0;
	do 
	{
		m_Client.KYKYSEM_GetServerStatus(&nStatus);
		nCounts++;
		Sleep(10);
	} while ( (nStatus < 1) && (nCounts < 100) );
	if( nCounts != 100 )
	{
		if( nStatus > 0 )
			printf( "connected successfully!\n" );
		else
			printf( "can not connected!\n" );
	}
	else
		printf( "can not connected!\n" );
	return 0;
}

