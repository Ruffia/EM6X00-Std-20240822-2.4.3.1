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

//�򿪴���
bool ComAccess::Open(char*  PortNum, 
					DWORD  dwBaudRate, 
					BYTE   byParity,
					BYTE   byStopBits,
					BYTE   byByteSize)
{
	DCB  dcb;  
	COMMTIMEOUTS TimeOut;
	//�򿪴���
	m_hCom = CreateFile(PortNum,
					GENERIC_READ|GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_OVERLAPPED,
					NULL);

	if(m_hCom == INVALID_HANDLE_VALUE)
		return false;

	//���ô��ڷ��ͽ��ջ�������С
	if(!SetupComm(m_hCom, 4096, 4096))
	{
		Close();
		return false; 
	}
	//ȡ�õ�ǰ���ڿ��Ʋ���
	if(!GetCommState(m_hCom, &dcb)) 
	{
		Close();
		return false;
	}
	//���ô��ڿ��Ʋ���
	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = byByteSize;
	dcb.Parity   = byParity;
	dcb.StopBits = byStopBits;
	if(!SetCommState(m_hCom, &dcb) ) 
	{
		Close();
		return false;
	}
	//���ó�ʱʱ��
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
	//���ô�������
	if(!SetCommMask(m_hCom, EV_RLSD|EV_ERR))
	{
		Close();
		return false;
	}
	//���ڶ��¼�
	m_ovRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_ovRead.hEvent == NULL)
	{
		Close();
		return false;
	}
	//����д�¼�
	m_ovWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_ovWrite.hEvent == NULL)
	{
		Close();
		return false;
	}
	//�߳̽����¼�
	m_hThreadEndEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_hThreadEndEvent == NULL)
	{
		Close();
		return false;
	}

	//���������߳�
	m_hThread = CreateThread(NULL, 0, ReadComThread, this, 0, NULL);
	if(m_hThread == NULL)
	{
		Close();
		return false;
	}

	PurgeComm(m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	return true;;
}

//�ر��߳�
void ComAccess::Close()
{
	ComUserData = NULL;
	ComDataArriveProc = NULL;
	//�ر��߳�
	if(m_hThread != NULL)
	{
		m_hThread = NULL;

		//����GetOverlappedResult
		if(m_ovRead.hEvent != NULL)
			SetEvent(m_ovRead.hEvent);
		//�ȴ��߳̽���
		if(m_hThreadEndEvent != NULL)//INFINITE
			WaitForSingleObject(m_hThreadEndEvent,200);
	}

	//�ر��߳̽����¼�
	if(m_hThreadEndEvent != NULL)
	{
		CloseHandle(m_hThreadEndEvent);
		m_hThreadEndEvent = NULL;
	}

	//�رն��¼�
	if(m_ovRead.hEvent != NULL)
	{
		CloseHandle(m_ovRead.hEvent);
		m_ovRead.hEvent = NULL; 
	}
	//�ر�д�¼�
	if(m_ovWrite.hEvent != NULL)
	{
		CloseHandle(m_ovWrite.hEvent);
		m_ovWrite.hEvent = NULL; 
	}
	//�رմ��ھ��
	if(m_hCom!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
		m_hCom = INVALID_HANDLE_VALUE; 
	}
}

//�򴮿ڷ�����
DWORD ComAccess::WriteData(void* Data,DWORD Len)
{
	DWORD m_Written = 0;
	//������͵����ݳ���С��1���ߴ��ھ����Ч
	if(Len < 1 || m_hCom == INVALID_HANDLE_VALUE)
		return(0);
	ClearCommError(m_hCom, NULL, NULL);
	//�򴮿ڷ�������
	WriteFile(m_hCom, Data, Len, &m_Written, &m_ovWrite);
	//�ȴ����ͽ���
	if(GetLastError() == ERROR_IO_PENDING)
	{
		GetOverlappedResult(m_hCom, &m_ovWrite, &m_Written, TRUE);
	}
	//��λ�¼�
	if(m_ovWrite.hEvent != NULL)
		ResetEvent(m_ovWrite.hEvent);
	return m_Written;
}

//�Ӵ��ڽ�������
DWORD ComAccess::ReadData(void* Data,DWORD Len)
{
	DWORD m_Read = 0;    
	//������յ����ݻ���������С��1���ߴ��ھ����Ч
	if(Len < 1 || m_hCom == INVALID_HANDLE_VALUE) 
		return(0);
	ClearCommError(m_hCom, NULL, NULL);
	//�Ӵ��ڽ�������
	ReadFile(m_hCom, Data, Len, &m_Read, &m_ovRead);
	//�ȴ����յ�����
	if(GetLastError() == ERROR_IO_PENDING)
	{
		GetOverlappedResult(m_hCom, &m_ovRead, &m_Read, TRUE);  
	}
	//��λ�¼�
	if(m_ovRead.hEvent != NULL)
		ResetEvent(m_ovRead.hEvent);
	return m_Read;
}

//���ý��յ����ݵĻص�����
void ComAccess::SetDataArriverProc(LPCOM_DATA_ARRIVE_PROC DataArriveProc,void* UserData)
{
	ComDataArriveProc	= DataArriveProc;
	ComUserData			= UserData;
}

//���������߳�
DWORD WINAPI ComAccess::ReadComThread(void* arg)
{
	//��������
	while(((ComAccess*)arg)->RecvComData());
	return 0;
}

//���մ��ڵ�����
bool ComAccess::RecvComData()
{
	//�ж��Ƿ�ֹͣ�����߳�
	if(m_hThread == NULL)
	{
		//��������߳̽����¼�
		if(m_hThreadEndEvent != NULL)
			SetEvent(m_hThreadEndEvent);
		return false;
	}
 
	//��������
	m_RecvDataLength = ReadData(m_RecvBuffer,RECV_BUFFER_LENGTH);

	//������յ����ݲ��һص�������Ϊ�գ����ûص�����
	if( m_RecvDataLength > 0 && ComDataArriveProc != NULL )
	{
		(*ComDataArriveProc)(m_RecvBuffer, (DWORD)m_RecvDataLength, ComUserData);
	}

	return true;
}