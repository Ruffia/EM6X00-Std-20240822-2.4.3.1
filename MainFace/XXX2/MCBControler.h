#pragma once
#include "USB_Functions.h"
#include "MCIO.H"

//�޸�ʱ�䣺  2020-08-21
//�޸��ˣ�  �ܺ�
//�޸�ԭ�򣺽�ԭ MCB ��������ع��ܴ� App ���ж�������
//���ܣ� MCB ������

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

	int		m_nMCB_TicksTotal[9];	// �ֿغи���ť�ۼ���ת��

	int		m_nMCBffTotal;

	int		m_nDFforFFBase;	// DF����ΪFFʱ�Ľ�λ����ֵ(�������ļ���ȡ)����DF�ۼƸı����ﵽ��ֵʱ��DF�ۼ������㡢����ֵ�ػ�0x8000��OL����ֵ��1

protected:
	CMCBControler();

};
