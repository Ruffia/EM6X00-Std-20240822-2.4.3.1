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

	BOOL	m_bPauseComm;	 // �Ƿ���ͣ���豸ͨѶ
	int		m_nIs1845;		// �����1����1845������տ��ư�ÿ���IPG�������2�������ϵͳ�Ż�������տ��ư�ÿ���CCG
	BOOL	m_bPauseInquire;
	BOOL	m_bIsDual;	    // ���ӱÿ������Ƿ���˫ͨ����
	BOOL	m_nIPtype;		// ���ӱÿ���������ʽ������ʽ��Ӱ��ͨѶЭ�飩��Ĭ��Ϊ��ʽ
};
