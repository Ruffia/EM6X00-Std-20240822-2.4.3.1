#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
#include "VACobtain.h"

class AFX_EXT_CLASS CIPGStatusDetector : public CStatusDetector
{
protected:
	CIPGStatusDetector(void);
	~CIPGStatusDetector(void){};

public:
	static CIPGStatusDetector& Instance();

	void Init( HWND hWnd, int nPort, BOOL nIPtype, BOOL bIsDual = FALSE);
	void SetChannel(int nChn,CString strIPCChn);
	virtual BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

protected:
	int CommWithIPC( int nSleep );
	void RecordVacuum( CString strVacuum );
	int AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp );
	int CommWithIPG( int nSleep );

protected:
	HWND	m_hWnd;
	CSerial	m_IPG;
	CfgVAC_IP_CCG m_cfgIPG;

	CString m_strIPG;
	CString m_strIPGSaved;
	CString	m_strWinIPG;

	HANDLE	m_hFileIPG;
	int		m_nLoopsIPG;

	BOOL	m_bPauseComm;	 // 是否暂停与设备通讯
	BOOL	m_bIsDual;	    // 离子泵控制器是否是双通道的
	BOOL	m_nIPtype;		// 离子泵控制器是老式还是新式（影响通讯协议），默认为新式
	BOOL    m_bIsSameChn;   // 是否是不同单通道而相同通道号
	BOOL	m_bPausePost; 
};
