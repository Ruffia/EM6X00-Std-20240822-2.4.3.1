#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
//#include "CommD22.h"

#define WM_USER_D22_UpdateValue	(WM_USER+101)

class AFX_EXT_CLASS CD22StatusDetector : public CStatusDetector
{
protected:
	CD22StatusDetector(void);
	~CD22StatusDetector(void){};

public:
	static CD22StatusDetector& Instance();

	void	Init( HWND hWnd, int nPort);
	void	SetParam( int nIndex, int nValue );
	virtual	BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

protected:
	void	InitRecordFile();
	void	Record( LPCTSTR lpszVacuum );

	BOOL	GetVersion();
	float	GetCurrentBeam();

	unsigned short	CRCcheck(BYTE* DataCode, int nLen);
	int		SendCmd( BYTE bFunc, BYTE bK, BYTE bM );
	BOOL	ReadFromDevice( BYTE* data );
	float	ConvertReadData(BYTE* lbuff);

protected:
	HWND	m_hWnd;
	CSerial	m_D22;
	int		m_nPortD22;
	float	m_fBeam;

	CString m_strD22;
	CString m_strD22Saved;

	HANDLE	m_hFileD22;
	int		m_nLoopsD22;

	BOOL	m_bPauseComm;	// 是否暂停与设备通讯
	BOOL	m_bPauseInquire;
};
