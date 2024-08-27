#pragma once
#include "StatusDetector.h"
#include "USB_Functions.h"
#include "ScanParameterSaver.h"

enum AutoType{
	AutoNULL,
	AutoBC,
	AutoFocus,
	AutoFocus1,
	AutoAstigmat,
	AutoFilament,
};

class CAutoFocusThread : public CStatusDetector
{
public:
	CAutoFocusThread(void);
	~CAutoFocusThread(void);
	virtual BOOL Start();
	virtual void Release();
	virtual DWORD DoJob();
	virtual BOOL End();

    void Initialize(HWND hWnd,const DWORD dwOpr);

protected:
	void _SetPos( const long lObjectiveLens, const double dblMagnifier );
	void _ChangeScan( USB_ScanType nusbScanType );
	void _AutoFocusProcess();
	void _StopAutoFocus();

protected:
	HWND        m_hWnd;
	LPBYTE		m_bImageBuff;
	double		m_dMaxSharpness;        //�õ���ͼ������
	double      m_dObjectiveLens;       //��������
	double      m_dMagnifier;           //�Ŵ���
	static   int    m_nExecRound;           //�߳�ִ���˼���
};
