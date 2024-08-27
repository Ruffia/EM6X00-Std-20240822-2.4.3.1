// ComAccess.h: interface for the ComAccess class.
//串口通讯类
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
	HANDLE      m_hThreadEndEvent;       //接收线程运行结束事件
	HANDLE      m_hThread;               //接收线程句柄
	HANDLE      m_hCom;                  //串口句柄

	OVERLAPPED  m_ovRead;   
	OVERLAPPED  m_ovWrite;  
	BYTE        m_RecvBuffer[RECV_BUFFER_LENGTH]; //接收数据缓冲区
	int         m_RecvDataLength;                 //接收到数据的长度

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
	函数名: Open
	功能:  打开串口
	输入:   PortNum  串口数 默认为COM1
	dwBaudRate 串口波特率
	byParity   奇偶位
	byStopBits 停止位
	byByteSize 数字位
	输出: 返回0为打开成功 返回1为打开失败
	*///////////////////////////////////////////////////////////////////
	bool Open(char*  PortNum     = "COM1",
				DWORD  dwBaudRate  = CBR_9600, 
				BYTE   byParity    = NOPARITY,
				BYTE   byStopBits  = ONESTOPBIT,
				BYTE   byByteSize  = 8);
	/******************************************************************
	函数名: Close
	功能:  关闭串口
	输入:  无
	输出:  无
	*///////////////////////////////////////////////////////////////////
	void Close(void);
	/******************************************************************
	函数名: WriteData
	功能:  串口写数据
	输入:  Data 数据内容
	Len 数据长度
	输出:  返回0
	*///////////////////////////////////////////////////////////////////
	DWORD WriteData(void* Data, DWORD Len);
	/******************************************************************
	函数名: SetDataArriverProc
	功能:  接收串口数据
	输入:  DataArriveProc 接收串口数据回调函数
	UserData 数据指针
	输出:  无
	*///////////////////////////////////////////////////////////////////
	void    SetDataArriverProc(LPCOM_DATA_ARRIVE_PROC DataArriveProc,void* UserData=NULL);
};

#endif //AFX_COMACCESS_H__44E0290D_DAD6_43D6_9DFE_1973975C9CDD__INCLUDED_

