#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
#include "VACobtain.h"

class AFX_EXT_CLASS CCCGStatusDetector : public CStatusDetector
{
protected:
	CCCGStatusDetector(void);
	~CCCGStatusDetector(void){};

public:
	static CCCGStatusDetector& Instance();

	void	Init( HWND hWnd,int nPort );
	void	InitThreshold(double dThreshhold1,double dThreshhold2);
	void	SetPause( BOOL bPause );
	virtual	BOOL Start();

	virtual	void Release();
	virtual	DWORD DoJob();

protected:
	void	CCG_ConvertSendData( const CString strOutput, short nCom );
	int		CCG_ConvertRequestMeasureData( CString strIn, char* data );
	void	CCG_ReceiveText( CString& strData );
	CString	CCG_ConvertReceiveData( CString strData );
	void	CommWithCCG( int nSleep );
	void	InitRecordFile();
	void	RecordVacuum( CString strVacuum );

	BOOL	_ReqUnit();
	BOOL	_ReqSP1();
	BOOL	_ReqPR1();

public:
	CfgVAC_IP_CCG	m_cfgCCG;

protected:
	HWND	m_hWnd;
	CSerial	m_CCG;

	CString	m_strCCG;
	CString	m_strCCGSaved;

	HANDLE	m_hFileCCG;
	int		m_nLoopsCCG;
};
