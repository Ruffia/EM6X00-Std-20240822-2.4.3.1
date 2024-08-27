#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
#include "VACobtain.h"

class AFX_EXT_CLASS CIPCStatusDetector : public CStatusDetector
{
protected:
	CIPCStatusDetector(void);
	~CIPCStatusDetector(void){};

public:
	static CIPCStatusDetector& Instance();

	void Init( HWND hWnd,int nPort, BOOL nIPtype );
	void SetChannel(int nChn,CString strIPGChn);
	virtual BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

protected:
	int CommWithIPC( int nSleep );
	void RecordVacuum( CString strVacuum );
	int AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp );

public:
	CfgVAC_IP_CCG m_cfgIPC;
	CString	m_strWinIPC;

protected:
	HWND	m_hWnd;
	CSerial	m_IPC;

	CString m_strIPC;
	CString m_strIPCSaved;


	HANDLE	m_hFileIPC;
	int		m_nLoopsIPC;

	BOOL	m_bPauseComm;	 // 是否暂停与设备通讯
	BOOL	g_bPausePost;	 // 是否暂停上送监测值（但不停止与设备通讯）
	BOOL	m_nIPtype;		 // 离子泵控制器是老式还是新式（影响通讯协议），默认为新式
	BOOL    m_bIsSameChn;    // 是否是不同单通道而相同通道号
};
