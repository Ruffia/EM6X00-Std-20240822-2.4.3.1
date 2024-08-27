// ComAccess.h: interface for the ComAccess class.
//����ͨѶ��
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMACCESS_H__44E0290D_DAD6_43D6_9DFE_1973975C9CDD__INCLUDED_)
#define AFX_COMACCESS_H__44E0290D_DAD6_43D6_9DFE_1973975C9CDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const DWORD RECV_BUFFER_LENGTH=4096;
typedef int (WINAPI* LPCOM_DATA_ARRIVE_PROC)(void* Data,DWORD Len,void* UserData);

class ComAccess 
{
private:
	HANDLE      m_hThreadEndEvent;       //�����߳����н����¼�
	HANDLE      m_hThread;               //�����߳̾��
	HANDLE      m_hCom;                  //���ھ��

	OVERLAPPED  m_ovRead;   
	OVERLAPPED  m_ovWrite;  
	BYTE        m_RecvBuffer[RECV_BUFFER_LENGTH]; //�������ݻ�����
	int         m_RecvDataLength;                 //���յ����ݵĳ���

private:
	int		(WINAPI* ComDataArriveProc)(void* Data, DWORD Len,void* UserData);

	void	*ComUserData;
	DWORD	ReadData(void* Data, DWORD Len);
	static	DWORD WINAPI ReadComThread(void* arg);
	bool	RecvComData(void);

public:
	ComAccess();
	~ComAccess();
	/******************************************************************
	������: Open
	����:  �򿪴���
	����:   PortNum  ������ Ĭ��ΪCOM1
	dwBaudRate ���ڲ�����
	byParity   ��żλ
	byStopBits ֹͣλ
	byByteSize ����λ
	���: ����0Ϊ�򿪳ɹ� ����1Ϊ��ʧ��
	*///////////////////////////////////////////////////////////////////
	bool Open(char*  PortNum     = "COM1",
				DWORD  dwBaudRate  = CBR_9600, 
				BYTE   byParity    = NOPARITY,
				BYTE   byStopBits  = ONESTOPBIT,
				BYTE   byByteSize  = 8);
	/******************************************************************
	������: Close
	����:  �رմ���
	����:  ��
	���:  ��
	*///////////////////////////////////////////////////////////////////
	void Close(void);
	/******************************************************************
	������: WriteData
	����:  ����д����
	����:  Data ��������
	Len ���ݳ���
	���:  ����0
	*///////////////////////////////////////////////////////////////////
	DWORD WriteData(void* Data, DWORD Len);
	/******************************************************************
	������: SetDataArriverProc
	����:  ���մ�������
	����:  DataArriveProc ���մ������ݻص�����
	UserData ����ָ��
	���:  ��
	*///////////////////////////////////////////////////////////////////
	void    SetDataArriverProc(LPCOM_DATA_ARRIVE_PROC DataArriveProc,void* UserData=NULL);
};

#endif //AFX_COMACCESS_H__44E0290D_DAD6_43D6_9DFE_1973975C9CDD__INCLUDED_

