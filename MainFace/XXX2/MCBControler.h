#pragma once
#include "USB_Functions.h"
#include "MCIO.H"

//修改时间：  2020-08-21
//修改人：  周宏
//修改原因：将原 MCB 控制器相关功能从 App 类中独立出来
//功能： MCB 控制器

///////////////////////////////////////////////////////////
// MCB
const	UINT nMCB_SemMessageTimerID		= 120;
const	UINT nMCB_SemMessageTimerElapse	= 100;

const	UINT nMCB_TimerID				= 121;
const	UINT nMCB_TimerElapse			= 30;

#define	MCB_Brightness		0
#define	MCB_Contrast		1
#define	MCB_Stig_Y			2
#define	MCB_Stig_X			3
#define	MCB_Magnification	4
#define	MCB_Shift_X			5
#define	MCB_Shift_Y			6
#define	MCB_OL_Coarse		7
#define	MCB_OL_Fine			8
#define MCB_CL_Coarse		9
#define MCB_CL_Fine			10
#define MCB_AL_X			11
#define MCB_AL_Y			12
#define	MCB_Stig_X2			13
#define	MCB_Stig_Y2			14
///////////////////////////////////////////////////////////

void MCB_Adjust_Update( int nChannel, int& nTicks );
void MCB_Adjust_Ratio( double dMag );

class CMCBControler  
{
public:
	static CMCBControler& Instance();

	BOOL	PumpMCBSemMessage();
	int		m_nMCB_CountStamp;
	int		m_nMCB_TicksHi, m_nMCB_TicksABS, m_nMCB_SwPos, m_nMCB_Channel, m_nMCB_CCW, m_nMCB_RotDir;
	BYTE	m_byteMCB_DataBak;
	BOOL	m_bMCB_ExpectLowByte, m_bMCB_ExpectHighByte;
	int		MCB_ReceiveCountStamp( BYTE bData );
	BOOL	MCB_CheckHighByte( BYTE bData );
	BOOL	MCB_ReceiveHighByte( BYTE bData );
	BOOL	MCB_ReceiveLowByte( BYTE bData );
	BOOL	MCB_Ctrl_Flush();
	BOOL	MCB_Ctrl_Flush_WP( int nChannel, int nSwitchPos, int nDirection, int nMCB_Ticks, BOOL blUnlock_HV, BOOL blOverhaulState );
	BOOL    ProcessMCBSemMessage( MicrocomputerID Z80ID, USB_SEM_ControlMessage& mcMSG, BOOL blUnlock_HV, BOOL blOverhaulState );

	int		m_nMCB_TicksTotal[9];	// 手控盒各旋钮累计旋转量

	int		m_nMCBffTotal;

	int		m_nDFforFFBase;	// DF复用为FF时的进位设置值(从配置文件读取)，当DF累计改变量达到该值时，DF累计量清零、控制值重回0x8000，OL控制值变1

protected:
	CMCBControler();

};
