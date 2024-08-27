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

	BOOL	m_bPauseComm;	 // �Ƿ���ͣ���豸ͨѶ
	BOOL	m_bIsDual;	    // ���ӱÿ������Ƿ���˫ͨ����
	BOOL	m_nIPtype;		// ���ӱÿ���������ʽ������ʽ��Ӱ��ͨѶЭ�飩��Ĭ��Ϊ��ʽ
	BOOL    m_bIsSameChn;   // �Ƿ��ǲ�ͬ��ͨ������ͬͨ����
	BOOL	m_bPausePost; 
};
