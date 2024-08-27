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
	double		m_dMaxSharpness;        //得到的图像的锐度
	double      m_dObjectiveLens;       //工作距离
	double      m_dMagnifier;           //放大倍数
	static   int    m_nExecRound;           //线程执行了几轮
};
