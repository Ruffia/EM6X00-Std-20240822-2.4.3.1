#include "StdAfx.h"
#include "ComAccess.h"

ComAccess::ComAccess()
{
	m_hThread			= NULL;

	ComUserData			= NULL;
	ComDataArriveProc	= NULL;

	m_hThreadEndEvent	= NULL;

	m_hCom = INVALID_HANDLE_VALUE;

	m_RecvDataLength = 0;
	memset(&m_ovRead, 0, sizeof(m_ovRead));
	memset(&m_ovWrite, 0, sizeof(m_ovWrite));
}

ComAccess::~ComAccess() 
{
	Close(); 
}

//打开串口
bool ComAccess::Open(char*  PortNum, 
					DWORD  dwBaudRate, 
					BYTE   byParity,
					BYTE   byStopBits,
					BYTE   byByteSize)
{
	DCB  dcb;  
	COMMTIMEOUTS TimeOut;
	//打开串口
	m_hCom = CreateFile(PortNum,
					GENERIC_READ|GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_OVERLAPPED,
					NULL);

	if(m_hCom == INVALID_HANDLE_VALUE)
		return false;

	//设置串口发送接收缓冲区大小
	if(!SetupComm(m_hCom, 4096, 4096))
	{
		Close();
		return false; 
	}
	//取得当前串口控制参数
	if(!GetCommState(m_hCom, &dcb)) 
	{
		Close();
		return false;
	}
	//设置串口控制参数
	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = byByteSize;
	dcb.Parity   = byParity;
	dcb.StopBits = byStopBits;
	if(!SetCommState(m_hCom, &dcb) ) 
	{
		Close();
		return false;
	}
	//设置超时时间
	TimeOut.ReadIntervalTimeout			= 10;
	TimeOut.ReadTotalTimeoutConstant	= 0;
	TimeOut.ReadTotalTimeoutMultiplier	= 0;
	TimeOut.WriteTotalTimeoutConstant	= 5000;
	TimeOut.WriteTotalTimeoutMultiplier	= 0;
	if(!SetCommTimeouts(m_hCom,&TimeOut))
	{
		Close();
		return false;
	}
	//设置串口掩码
	if(!SetCommMask(m_hCom, EV_RLSD|EV_ERR))
	{
		Close();
		return false;
	}
	//串口读事件
	m_ovRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_ovRead.hEvent == NULL)
	{
		Close();
		return false;
	}
	//串口写事件
	m_ovWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_ovWrite.hEvent == NULL)
	{
		Close();
		return false;
	}
	//线程结束事件
	m_hThreadEndEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_hThreadEndEvent == NULL)
	{
		Close();
		return false;
	}

	//创建接收线程
	m_hThread = CreateThread(NULL, 0, ReadComThread, this, 0, NULL);
	if(m_hThread == NULL)
	{
		Close();
		return false;
	}

	PurgeComm(m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	return true;;
}

//关闭线程
void ComAccess::Close()
{
	ComUserData = NULL;
	ComDataArriveProc = NULL;
	//关闭线程
	if(m_hThread != NULL)
	{
		m_hThread = NULL;

		//激活GetOverlappedResult
		if(m_ovRead.hEvent != NULL)
			SetEvent(m_ovRead.hEvent);
		//等待线程结束
		if(m_hThreadEndEvent != NULL)//INFINITE
			WaitForSingleObject(m_hThreadEndEvent,200);
	}

	//关闭线程结束事件
	if(m_hThreadEndEvent != NULL)
	{
		CloseHandle(m_hThreadEndEvent);
		m_hThreadEndEvent = NULL;
	}

	//关闭读事件
	if(m_ovRead.hEvent != NULL)
	{
		CloseHandle(m_ovRead.hEvent);
		m_ovRead.hEvent = NULL; 
	}
	//关闭写事件
	if(m_ovWrite.hEvent != NULL)
	{
		CloseHandle(m_ovWrite.hEvent);
		m_ovWrite.hEvent = NULL; 
	}
	//关闭串口句柄
	if(m_hCom!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
		m_hCom = INVALID_HANDLE_VALUE; 
	}
}

//向串口发数据
DWORD ComAccess::WriteData(void* Data,DWORD Len)
{
	DWORD m_Written = 0;
	//如果发送的数据长度小于1或者串口句柄无效
	if(Len < 1 || m_hCom == INVALID_HANDLE_VALUE)
		return(0);
	ClearCommError(m_hCom, NULL, NULL);
	//向串口发送数据
	WriteFile(m_hCom, Data, Len, &m_Written, &m_ovWrite);
	//等待发送结束
	if(GetLastError() == ERROR_IO_PENDING)
	{
		GetOverlappedResult(m_hCom, &m_ovWrite, &m_Written, TRUE);
	}
	//复位事件
	if(m_ovWrite.hEvent != NULL)
		ResetEvent(m_ovWrite.hEvent);
	return m_Written;
}

//从串口接收数据
DWORD ComAccess::ReadData(void* Data,DWORD Len)
{
	DWORD m_Read = 0;    
	//如果接收的数据缓冲区长度小于1或者串口句柄无效
	if(Len < 1 || m_hCom == INVALID_HANDLE_VALUE) 
		return(0);
	ClearCommError(m_hCom, NULL, NULL);
	//从串口接收数据
	ReadFile(m_hCom, Data, Len, &m_Read, &m_ovRead);
	//等待接收到数据
	if(GetLastError() == ERROR_IO_PENDING)
	{
		GetOverlappedResult(m_hCom, &m_ovRead, &m_Read, TRUE);  
	}
	//复位事件
	if(m_ovRead.hEvent != NULL)
		ResetEvent(m_ovRead.hEvent);
	return m_Read;
}

//设置接收到数据的回调函数
void ComAccess::SetDataArriverProc(LPCOM_DATA_ARRIVE_PROC DataArriveProc,void* UserData)
{
	ComDataArriveProc	= DataArriveProc;
	ComUserData			= UserData;
}

//接收数据线程
DWORD WINAPI ComAccess::ReadComThread(void* arg)
{
	//接收数据
	while(((ComAccess*)arg)->RecvComData());
	return 0;
}

//接收串口的数据
bool ComAccess::RecvComData()
{
	//判断是否停止接收线程
	if(m_hThread == NULL)
	{
		//激活接收线程结束事件
		if(m_hThreadEndEvent != NULL)
			SetEvent(m_hThreadEndEvent);
		return false;
	}
 
	//接收数据
	m_RecvDataLength = ReadData(m_RecvBuffer,RECV_BUFFER_LENGTH);

	//如果接收到数据并且回调函数不为空，调用回调函数
	if( m_RecvDataLength > 0 && ComDataArriveProc != NULL )
	{
		(*ComDataArriveProc)(m_RecvBuffer, (DWORD)m_RecvDataLength, ComUserData);
	}

	return true;
}