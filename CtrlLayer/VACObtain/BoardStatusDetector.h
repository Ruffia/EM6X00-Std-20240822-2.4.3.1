#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
#include "VACobtain.h"

class AFX_EXT_CLASS CBoardStatusDetector : public CStatusDetector
{
protected:
	CBoardStatusDetector(void);
	~CBoardStatusDetector(void){};

public:
	static CBoardStatusDetector& Instance();

	void Init( HWND hWnd, int nPort, BOOL nIPtype, BOOL bIsDual);
	virtual BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

protected:
	int CommWithBoard( int nSleep );
	void RecordVacuum( CString strVacuum );
	void Board_SendCmd( BYTE byteOpc, BYTE byteOpr );
	void Board_ConvertSendData( BYTE byteOpc, BYTE byteOpr, char* data );
	int  Board_ConvertReceiveData( int nLen, BYTE* data );

protected:
	HWND	m_hWnd;
	CSerial	m_Board;
	CfgVAC_IP_CCG m_cfgBoard;

	StatusFEGBrdVAC_board	m_boardStatusServer;
	StatusFEGBrdVAC_board	m_boardStatusMain;

	CString m_strBoard;
	CString m_strBoardSaved;
	CString	m_strWinBoard;

	HANDLE	m_hFileBoard;
	int		m_nLoopsBoard;

	BOOL	m_bPauseComm;	 // 是否暂停与设备通讯
	int		m_nIs1845;		// 如果是1，是1845，则真空控制板每秒读IPG；如果是2，是真空系统优化后，则真空控制板每秒读CCG
	BOOL	m_bPauseInquire;
	BOOL	m_bIsDual;	    // 离子泵控制器是否是双通道的
	BOOL	m_nIPtype;		// 离子泵控制器是老式还是新式（影响通讯协议），默认为新式
};
