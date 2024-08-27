#if !defined(AFX_VAC_H__6C09C495_A076_4B90_96FB_1E61698B1520__INCLUDED_)
#define AFX_VAC_H__6C09C495_A076_4B90_96FB_1E61698B1520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Vac.h : header file
//

#include <USB_Functions.h>
#include "MP.h"
#include "UsbDI16A.h"

// CVac thread
class CVac : public CWinThread
{
	DECLARE_DYNCREATE(CVac)
protected:
	BOOL		m_bReady;
	DWORD		m_lastSys;
	DWORD		m_lastTC;		// last tick count
	DWORD		m_lastTCamp;	// last tick count for TC Amp readback
	DWORD		m_lastTCdelay;	// last tick count for delay
	DWORD		m_lastRecord;	// last tick count for record
	DWORD		m_lastVent;		// last tick count for Vent Valve

	HANDLE		m_hEventKill;
	HANDLE		m_hEventDead;
	HANDLE		m_hTimeDelay;

	HWND		m_hMain;
	int			m_nErrReads;

	SMvac_Param	m_VacParamMain;	// 主程序中控制的参数：电源、样品室放气、电子枪放气
	SMvac_Param	m_VacParamSM;	// 状态机中监测的参数：TC0、TC1、V1、高压联锁信号
	SMvac_TCAmp	m_TCAmp;
	int			m_nState;		// 当前状态
	int			m_nMsgType;		// 用户界面应该提示的信息类型

	unsigned short* m_pnReadData;
	BOOL		m_bDelay;
	BOOL		m_bChanged;

	BOOL		m_bDebug;
	BOOL		m_bScanning;
	BOOL		m_bPause;
	BOOL		m_bV1CtrlMode;	// V1阀控制方式：FALSE为传统手动，TRUE为气动阀

	HANDLE		m_hFileVAC;
	int			m_nLoopsVAC;

	double		m_dCCGvac;		// 为了将CCG值与各真空状态一起保存，从主程序中传递CCG值
//	double		m_dTMPThreshold;// 为了使用CCG值作为判断是否开TMP的阈值，从主程序中传递该阈值
	double		m_dThreshold[4];// 用TC Amp判断TC3/4好坏，从主程序中传递这些阈值
	int			m_nVacSrc;		// 真空判断源：0为TC翻转值；1为CCG值

// Operations
public:
	CVac();           // protected constructor used by dynamic creation

	BOOL	Close();
	BOOL	Reset( CWnd* pWnd );
	void	Init();
	BOOL	isReady(){ return m_bReady; };
	void	KillThread();
	BOOL	SetParams( int nType, DWORD dwParam );
	BOOL	SetParams2( int nType, double dParam );

	void	Delay( int msecond );
	void	USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	void	VAC_InitCtrls( BOOL bCtrlBySoft );
	void	OnlySendReadSelectedADCcmd( int nADCchan );
	BOOL	ReadStates();
	BOOL	ReadStates2();
	BOOL	ReadTCsAmp();
	BOOL	CheckValidity( int nType, DWORD dwData );

	void	FuncPowerOpen( int * nxt_state );
	void	FuncPowerClose( int * nxt_state );
	void	FuncVentOpen( int * nxt_state );
	void	FuncVentClose( int * nxt_state );
	void	FuncV1Open( int * nxt_state );
	void	FuncV1Close( int * nxt_state );

	void	FuncTCForelOK( int * nxt_state );
	void	FuncTCForelBad( int * nxt_state );
	void	FuncTCColumnOK( int * nxt_state );
	void	FuncTCColumnBad( int * nxt_state );

	void	FuncTMPOpen( int * nxt_state );
	void	FuncTMPClose( int * nxt_state );
	void	FuncVfOpen( int * nxt_state );
	void	FuncVfClose( int * nxt_state );

	BOOL	ADCvconv2( DWORD dw1, DWORD dw2, double& dValue );
	void	Record();
	void	Record2( CString strRcd );

private:
	void	MainLoop();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
public:
	virtual ~CVac();
	virtual void Delete();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_VAC_H__6C09C495_A076_4B90_96FB_1E61698B1520__INCLUDED_)
