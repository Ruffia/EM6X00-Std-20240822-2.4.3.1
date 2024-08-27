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

	BOOL	m_bPauseComm;	 // �Ƿ���ͣ���豸ͨѶ
	BOOL	g_bPausePost;	 // �Ƿ���ͣ���ͼ��ֵ������ֹͣ���豸ͨѶ��
	BOOL	m_nIPtype;		 // ���ӱÿ���������ʽ������ʽ��Ӱ��ͨѶЭ�飩��Ĭ��Ϊ��ʽ
	BOOL    m_bIsSameChn;    // �Ƿ��ǲ�ͬ��ͨ������ͬͨ����
};
