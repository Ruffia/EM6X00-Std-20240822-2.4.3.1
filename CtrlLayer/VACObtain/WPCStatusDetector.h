#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
#include "VACobtain.h"

class AFX_EXT_CLASS CWPCStatusDetector : public CStatusDetector
{
protected:
	CWPCStatusDetector(void);
	~CWPCStatusDetector(void){};

public:
	static CWPCStatusDetector& Instance();

	void Init( HWND hWnd, int nPort);
	virtual BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

	void InitThreshold(double dThreshhold1,double dThreshhold2);

protected:
	int CommWithWPC( int nSleep );
	void RecordVacuum( CString strVacuum );

	int   WPC_WriteToDevice( int nSleep, BYTE* data, int nLength );
	int   WPC_ReadFromDevice( int nSleep, BYTE* data );
	void  WPC_ConvertSendReqData( BYTE Addr, BYTE* data );
	void  WPC_ConvertSendSetData( BYTE Addr, DWORD dwValue, BYTE* data );
	short WPC_ConvertReceiveData( int nLen, BYTE* data );
	WORD  WPC_SendReq( int nSleep, BYTE Addr );
	int   WPC_SendSet( int nSleep, BYTE Addr, DWORD dwValue );

public:
	CfgVAC_IP_CCG m_cfgWPC;

protected:
	HWND	m_hWnd;
	CSerial	m_WPC;	

	StatusFEGBrdVAC_board	m_boardStatusServer;
	StatusFEGBrdVAC_board	m_boardStatusMain;

	CString m_strWPC;
	CString m_strWPCSaved;
	CString	m_strWinWPC;

	HANDLE	m_hFileWPC;
	int		m_nLoopsWPC;

	BOOL	m_bPauseComm;	 // 是否暂停与设备通讯
	BOOL	m_bPausePost;
};
