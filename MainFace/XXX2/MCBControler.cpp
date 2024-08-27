#include "stdafx.h"
#include "MCBControler.h"
#include "SemDataType.h"
#include <math.h>
#include "DebugMessageDispatcher.h"
#include "ControlLayer.h"
#include "SemCtrl.h"
#include "GlobalParamSetting.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

extern double	g_dScaleScanTable[];
extern int		g_nScaleScanIndex;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
CMCBControler& CMCBControler::Instance()
{
	static CMCBControler Inst;
	return Inst;
}

CMCBControler::CMCBControler()
{
	// 最先接收时间戳，设置标志位
	m_bMCB_ExpectLowByte	= FALSE;
	m_bMCB_ExpectHighByte	= FALSE;
	// 手控盒旋钮累计值清零
	for(int i=0; i<9; i++ )
		m_nMCB_TicksTotal[i] = 0;

	m_nMCBffTotal	= 0;
}


BOOL CMCBControler::PumpMCBSemMessage()
{
	return TRUE;
}

BOOL CMCBControler::ProcessMCBSemMessage( MicrocomputerID Z80ID, USB_SEM_ControlMessage& mcMSG, BOOL blUnlock_HV, BOOL blOverhaulState )
{
	const BYTE bCtrlParam	= (BYTE)(mcMSG.bData[1]);

	CString csTemp;
	csTemp.Format( "MCB Message: 0x%02X", mcMSG.bData[1] );
	CDebugMessageDispatcher::Instance().DisplayMsg( csTemp );

	if( !m_bMCB_ExpectHighByte && !m_bMCB_ExpectLowByte )
	{
		// Time Stamp ( count stamp )
		int nRet = MCB_ReceiveCountStamp( bCtrlParam );
		if( nRet < 0 )
		{
			// not count stamp
			CDebugMessageDispatcher::Instance().DisplayMsg( "EER: not count stamp" );
			// try high byte
			nRet = MCB_ReceiveHighByte( bCtrlParam );
			if( nRet == 0 )
			{
				CDebugMessageDispatcher::Instance().DisplayMsg( "and not hi, abandon" );
				CDebugMessageDispatcher::Instance().DisplayMsg( " " );
				m_bMCB_ExpectHighByte	= FALSE;
				m_bMCB_ExpectLowByte	= FALSE;
				return FALSE;
			}
			CDebugMessageDispatcher::Instance().DisplayMsg( "EER: it is high" );
		}
		else
			m_byteMCB_DataBak = bCtrlParam;	// if it is stamp, backup it
	}
	else if( !m_bMCB_ExpectLowByte )
	{
		BOOL bRet = MCB_ReceiveHighByte( bCtrlParam );
		if( !bRet )
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "EER: not high byte" );
			// not high byte, maybe it is low byte
			// so the backup data maybe high byte
			bRet = MCB_ReceiveHighByte( m_byteMCB_DataBak );
			if( bRet )
			{
				// If HiByteGood is false, then this must be a low byte of data
				if( MCB_ReceiveLowByte( bCtrlParam ) )
				{
					MCB_Ctrl_Flush_WP( m_nMCB_Channel, m_nMCB_SwPos, m_nMCB_RotDir, m_nMCB_TicksABS,blUnlock_HV, blOverhaulState);
					CDebugMessageDispatcher::Instance().DisplayMsg( "EER: the backup is hi, HI & LO are both OK" );
					CDebugMessageDispatcher::Instance().DisplayMsg( " " );
				}
				else
				{
					// if the backup data is not high byte, abandon
					CDebugMessageDispatcher::Instance().DisplayMsg( "EER: Neither backup or now is high byte, abandon" );
					CDebugMessageDispatcher::Instance().DisplayMsg( " " );
					m_bMCB_ExpectHighByte	= FALSE;
					m_bMCB_ExpectLowByte	= FALSE;
				}
			}
		}
		return TRUE;
	}	// (If NOT expecting low byte)
	else
	{
		if( MCB_ReceiveLowByte( bCtrlParam ) )
		{
			MCB_Ctrl_Flush_WP( m_nMCB_Channel, m_nMCB_SwPos, m_nMCB_RotDir, m_nMCB_TicksABS, blUnlock_HV, blOverhaulState );
			CDebugMessageDispatcher::Instance().DisplayMsg( " " );
		}
		else
		{
			CDebugMessageDispatcher::Instance().DisplayMsg( "EER: not low byte, try count stamp" );
			CDebugMessageDispatcher::Instance().DisplayMsg( " " );
			MCB_ReceiveCountStamp( bCtrlParam );
		}
	}

	return TRUE;
}


int CMCBControler::MCB_ReceiveCountStamp( BYTE bData )
{
	int nRet = 1;
	if( (bData & 0x01) != 0x01 )
	{
		nRet = -1;
		return nRet;
	}

	int nCountStamp = bData >> 1;		// 右移1位
	if( nCountStamp != m_nMCB_CountStamp )
	{
		m_nMCB_CountStamp = nCountStamp;
		nRet = 0;
	}

	m_nMCB_CountStamp += 1;
	if( m_nMCB_CountStamp == 0x80 )
		m_nMCB_CountStamp = 0;

	m_bMCB_ExpectHighByte = TRUE;
	return nRet;
}

BOOL CMCBControler::MCB_CheckHighByte( BYTE bData )
{
	BOOL bRet = FALSE;

	switch( bData )
	{
	case	0x04:
	case	0x07:

	case	0x08:
	case	0x0B:

	case	0x14:
	case	0x17:

	case	0x18:
	case	0x1B:

	case	0x24:
	case	0x27:

	case	0x28:
	case	0x2B:

	case	0x34:
	case	0x37:

	case	0x38:
	case	0x3B:

	case	0x44:
	case	0x47:

	case	0x48:
	case	0x4B:

	case	0x54:
	case	0x57:

	case	0x58:
	case	0x5B:

	case	0x64:
	case	0x67:

	case	0x68:
	case	0x6B:

	case	0x74:
	case	0x77:
	case	0x78:
	case	0x7B:

	case	0x84:
	case	0x87:

	case	0x88:
	case	0x8B:
		bRet = TRUE;
		break;
	}

	return bRet;
}


BOOL CMCBControler::MCB_ReceiveHighByte( BYTE bData )
{
	if( !MCB_CheckHighByte( bData ) )
		return FALSE;

	int nChanNow;
	BOOL bChanGood, bSwPosGood, bRotationGood, bHiByteGood;	// Sanity checks in case out of sync

	// Process current byte as a HIGH byte, then reject if not valid
	// Expect High byte first pass, then low byte 2nd pass:
	// 15 14 13 12 || 11 || 10 || 09 || 08 07 06 05 04 03 02 01 00
	// Knob ID	   || fA || fB || neg|| net pulses
	nChanNow = (bData & 0xF0) /16;
	if( nChanNow < 9 )					// 9 knobs total
		bChanGood = TRUE;
	else
		bChanGood = FALSE;

	m_nMCB_SwPos = (bData & 0x0C) /4;
	if( m_nMCB_SwPos == 1 || m_nMCB_SwPos == 2 )	// 1 means fB, 2 means fA
		bSwPosGood = TRUE;
	else
		bSwPosGood = FALSE;

	m_nMCB_TicksHi = ( bData & 0x03 );
	if( (m_nMCB_TicksHi == 3) || (m_nMCB_TicksHi == 0) )
		bRotationGood = TRUE;
	else
		bRotationGood = FALSE;

	if( bChanGood && bSwPosGood && bRotationGood )
		bHiByteGood = TRUE;
	else
		bHiByteGood = FALSE;

	if( bHiByteGood )
	{
		m_nMCB_Channel = nChanNow;

		m_bMCB_ExpectLowByte	= TRUE;		// Finished processed what appeared to be a high byte, so expect low byte next
		m_bMCB_ExpectHighByte	= FALSE;
	}

	return bHiByteGood;
}


BOOL CMCBControler::MCB_ReceiveLowByte( BYTE bData )
{
	BOOL bRet = TRUE;
	// Data byte is LOW, so append 8 bit TicksLo to 2 bit TicksHi value for new encoder position
	int nMCB_TicksLo	= bData;
	int nMCB_Ticks		= m_nMCB_TicksHi *256 +nMCB_TicksLo;
	if( nMCB_Ticks > 0x39F )	// > 0x39F, so CCW
	{
		m_nMCB_TicksABS	= 1024 - nMCB_Ticks;
		m_nMCB_RotDir		= -1;
	}
	else						// CW Rotation
	{
		m_nMCB_TicksABS	= nMCB_Ticks;
		m_nMCB_RotDir		= 1;
	}
	//	if( m_nMCB_TicksABS > 0x30 )		// so big value
	//		bRet = FALSE;

	m_bMCB_ExpectLowByte	= FALSE;		// Since low byte was just processed, expect count stamp next //high byte next
	m_bMCB_ExpectHighByte	= FALSE;

	return bRet;
}


BOOL CMCBControler::MCB_Ctrl_Flush()
{
	VARIANT		ClassID, SerialNumber, Xpos, Ypos, Flag;

	SerialNumber.lVal = -1;

	switch( m_nMCB_Channel )
	{
	case MCB_Brightness:
		Xpos = CSemCoreAssemble::Instance().m_Brightness.GetPos();
		ClassID.lVal = SemControlPanelClass_Brightness;
		Flag.lVal	 = 1;
		if( m_nMCB_CCW )
			Xpos.lVal	= Xpos.lVal - m_nMCB_TicksABS/4;
		else
			Xpos.lVal	= Xpos.lVal + m_nMCB_TicksABS/4;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case MCB_Contrast:
		Xpos = CSemCoreAssemble::Instance().m_Contrast.GetPos();
		ClassID.lVal = SemControlPanelClass_Contrast;
		Flag.lVal	 = 1;
		if( m_nMCB_CCW )
			Xpos.lVal	= Xpos.lVal - m_nMCB_TicksABS/4;
		else
			Xpos.lVal	= Xpos.lVal + m_nMCB_TicksABS/4;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case MCB_Stig_Y:
		Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal	 = 2;

		if( m_nMCB_CCW )
			Ypos.lVal	= Ypos.lVal - m_nMCB_TicksABS/4;
		else
			Ypos.lVal	= Ypos.lVal + m_nMCB_TicksABS/4;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case MCB_Stig_X:
		Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal	 = 1;

		if( m_nMCB_CCW )
			Xpos.lVal	= Xpos.lVal - m_nMCB_TicksABS/4;
		else
			Xpos.lVal	= Xpos.lVal + m_nMCB_TicksABS/4;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case MCB_Magnification:
		Xpos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		Flag.lVal		= 1;

		if( m_nMCB_CCW )
			Xpos.dblVal	= Xpos.dblVal - 1.0*m_nMCB_TicksABS;
		else
			Xpos.dblVal	= Xpos.dblVal + 1.0*m_nMCB_TicksABS;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case MCB_Shift_X:
		Xpos = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
		ClassID.lVal = SemControlPanelClass_ScanShift;
		Flag.lVal	 = 1;

		if( m_nMCB_CCW )
			Xpos.lVal	= Xpos.lVal - m_nMCB_TicksABS/4;
		else
			Xpos.lVal	= Xpos.lVal + m_nMCB_TicksABS/4;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case MCB_Shift_Y:
		Ypos = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
		ClassID.lVal = SemControlPanelClass_ScanShift;
		Flag.lVal	 = 2;

		if( m_nMCB_CCW )
			Ypos.lVal	= Ypos.lVal - m_nMCB_TicksABS/4;
		else
			Ypos.lVal	= Ypos.lVal + m_nMCB_TicksABS/4;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case	MCB_OL_Coarse:
		Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Flag.lVal	 = 1;
		if( m_nMCB_CCW )
			Xpos.lVal	= Xpos.lVal - m_nMCB_TicksABS;
		else
			Xpos.lVal	= Xpos.lVal + m_nMCB_TicksABS;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;

	case	MCB_OL_Fine:
		Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Flag.lVal	 = 1;
		if( m_nMCB_CCW )
			Xpos.lVal	= Xpos.lVal - m_nMCB_TicksABS/2;
		else
			Xpos.lVal	= Xpos.lVal + m_nMCB_TicksABS/2;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		break;
	}

	return TRUE;
}



BOOL CMCBControler::MCB_Ctrl_Flush_WP( int nChannel, int nSwitchPos, int nDirection, int nMCB_Ticks, BOOL blUnlock_HV, BOOL blOverhaulState )
{
	m_nMCB_TicksTotal[nChannel] += nDirection *nMCB_Ticks;

	VARIANT		ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	CString str;
	int nTickChn = nChannel;
	int nFuncChn = -1;

	if( CControlLayer::Instance().m_nAdjustorManualSort < SORT_MANUAL_NULL	// 手控盒中间旋钮为多功能旋钮
		//		&& nChannel < MCB_OL_Coarse )				// Focus Coarse 和 Focus Fine旋钮仍有效
			&& nChannel == 4 )
	{
		switch( CControlLayer::Instance().m_nAdjustorManualSort )
		{
		case	SORT_MANUAL_CONDENSER_FINE:
			nFuncChn	= MCB_CL_Fine;
			break;
		case	SORT_MANUAL_CONDENSER_CORSE:
			nFuncChn	= MCB_CL_Coarse;
			break;
		case	SORT_MANUAL_STIGMATOR_X:
			nFuncChn	= MCB_Stig_X;
			break;
		case	SORT_MANUAL_STIGMATOR_Y:
			nFuncChn	= MCB_Stig_Y;
			break;
		case	SORT_MANUAL_STIGMATOR_X2:
			nFuncChn	= MCB_Stig_X2;
			break;
		case	SORT_MANUAL_STIGMATOR_Y2:
			nFuncChn	= MCB_Stig_Y2;
			break;
		case	SORT_MANUAL_BEAM_DISPLACEMENT_X:
			nFuncChn	= MCB_Shift_X;
			break;
		case	SORT_MANUAL_BEAM_DISPLACEMENT_Y:
			nFuncChn	= MCB_Shift_Y;
			break;
		case	SORT_MANUAL_BEAM_ALIGNMENT_X:
			nFuncChn	= MCB_AL_X;
			break;
		case	SORT_MANUAL_BEAM_ALIGNMENT_Y:
			nFuncChn	= MCB_AL_Y;
			break;
		case	SORT_MANUAL_BRIGHT:
			nFuncChn	= MCB_Brightness;
			break;
		case	SORT_MANUAL_CONTRAST:
			nFuncChn	= MCB_Contrast;
			break;
		default:
			break;
		}
	}
	else
		nFuncChn	= nChannel;

	switch( nFuncChn )
	{
	case MCB_Brightness:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 16 )
		{
			Xpos = CSemCoreAssemble::Instance().m_Brightness.GetPos();
			ClassID.lVal	= SemControlPanelClass_Brightness;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn] /8;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Contrast:
		// J14开了之后才可以加对比度
		if( blUnlock_HV || blOverhaulState )
		{
			if( abs(m_nMCB_TicksTotal[nTickChn]) > 8 )
			{
				Xpos = CSemCoreAssemble::Instance().m_Contrast.GetPos();
				ClassID.lVal	= SemControlPanelClass_Contrast;
				Flag.lVal		= 1;
				Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn] /8;

				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
				m_nMCB_TicksTotal[nTickChn] = 0;
			}
		}
		break;

	case MCB_Stig_Y:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 8 )
		{
			Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 2;
			Ypos.lVal		= Ypos.lVal + m_nMCB_TicksTotal[nTickChn] /2;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Stig_X:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 8 )
		{
			Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn] /2;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Stig_Y2:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 2;
			Ypos.lVal		= Ypos.lVal + m_nMCB_TicksTotal[nTickChn];

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Stig_X2:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn];

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Magnification:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 8 )
		{
			Xpos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
			ClassID.lVal	= SemControlPanelClass_Magnifier;
			Flag.lVal		= 1;
			Xpos.dblVal		= Xpos.dblVal + 1.0 *m_nMCB_TicksTotal[nTickChn] *4;// /32

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Shift_X:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 8 )
		{
			Xpos = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
			ClassID.lVal	= SemControlPanelClass_ScanShift;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn] *4;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_Shift_Y:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 8 )
		{
			Ypos = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
			ClassID.lVal	= SemControlPanelClass_ScanShift;
			Flag.lVal		= 2;
			Ypos.lVal		= Ypos.lVal + m_nMCB_TicksTotal[nTickChn] *4;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_OL_Coarse:
		MCB_Adjust_Update( nFuncChn, m_nMCB_TicksTotal[nTickChn] );
		break;
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
			ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
			Flag.lVal		= 1;
			Xpos.dblVal		= Xpos.dblVal + 1.0*m_nMCB_TicksTotal[nTickChn];
			Xpos.lVal		= (long)(Xpos.dblVal);

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_OL_Fine:
		if( CGlobalParamSetting::Instance().g_dfMag < 10000 )
		{
			if( abs(m_nMCB_TicksTotal[nTickChn]) > 64 )
			{
				// 每64个Ticks就显示值百分位变化2，设置值变化1
				Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
				ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
				Flag.lVal		= 1;
				Xpos.dblVal		= Xpos.dblVal + 1.0*m_nMCB_TicksTotal[nTickChn] /64;
				Xpos.lVal		= (long)(Xpos.dblVal);

				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
				m_nMCB_TicksTotal[nTickChn] = 0;
			}
		}
		else
		{
			if( abs(m_nMCB_TicksTotal[nTickChn]) > 64 )
			{
				// Dynamic Focus复用为FocusFine
				// 每32个Ticks就设置值变化1
				// DF累计改变量达到Base，则DF累计量清零、控制值重回0x8000，OL控制值变1
				//				m_nMCBffTotal	+= (m_nMCB_TicksTotal[nTickChn] /64);
				Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
				ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
				Flag.lVal		= 1;
				Xpos.dblVal		= Xpos.dblVal + 1.0 *m_nMCB_TicksTotal[nTickChn] /m_nDFforFFBase;
				Xpos.lVal		= (long)(Xpos.dblVal);

				CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
				m_nMCB_TicksTotal[nTickChn] = 0;
			}
		}
		break;
	case MCB_CL_Coarse:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Xpos = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
			ClassID.lVal	= SemControlPanelClass_CondenserLens;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn];

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_CL_Fine:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Xpos = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
			ClassID.lVal	= SemControlPanelClass_CondenserLens;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn] /32;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_AL_Y:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Ypos = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();
			ClassID.lVal	= SemControlPanelClass_BeemAlignment;
			Flag.lVal		= 2;
			Ypos.lVal		= Ypos.lVal + m_nMCB_TicksTotal[nTickChn];// /32;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	case MCB_AL_X:
		if( abs(m_nMCB_TicksTotal[nTickChn]) > 32 )
		{
			Xpos = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
			ClassID.lVal	= SemControlPanelClass_BeemAlignment;
			Flag.lVal		= 1;
			Xpos.lVal		= Xpos.lVal + m_nMCB_TicksTotal[nTickChn];// /32;

			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			m_nMCB_TicksTotal[nTickChn] = 0;
		}
		break;

	default:
		break;
	}

	return TRUE;
}


void MCB_Adjust_Update( int nChannel, int& nTicks )
{
	if( fabs(nTicks * CGlobalParamSetting::Instance().g_dfMCBRatio) > 0.99 )
	{
		VARIANT		ClassID, SerialNumber, Xpos, Ypos, Flag;
		SerialNumber.lVal	= -1;
		ClassID.lVal		= -1;

		switch( nChannel )
		{
		case	MCB_OL_Coarse:
			Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
			ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
			Flag.lVal		= 1;
			Xpos.dblVal		= Xpos.dblVal + 1.0*nTicks * CGlobalParamSetting::Instance().g_dfMCBRatio;
			Xpos.lVal		= (long)(Xpos.dblVal);
			break;
		case	MCB_Stig_X:
			Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 1;
			Xpos.lVal		= (long)(Xpos.lVal + nTicks * CGlobalParamSetting::Instance().g_dfMCBRatio);
			break;
		case	MCB_Stig_Y:
			Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 2;
			Ypos.lVal		= (long)(Ypos.lVal + nTicks * CGlobalParamSetting::Instance().g_dfMCBRatio);
		}
		if( ClassID.lVal > 0 )
		{
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			nTicks = 0;
		}
	}
}


void MCB_Adjust_Ratio( double dMag )
{
	double dfMCBRatioLog10 = 0.0;
	if( dMag < 10000 )
	{
		/////////////////////////////////////////////
		// log10(SlowDown) = A * log10(Mag) + B
		// OffsetOLens = OffsetScreenX * dfSlowDown;
		// Mag = 10000			SlowDown =  0.1
		// Mag =     1			SlowDown =  100
		//     ==> A = -0.75   B = 2
		/////////////////////////////////////////////
		dfMCBRatioLog10 = 2 - 0.75 * log10(dMag);
	}
	else if( dMag < 100000 )
	{
		/////////////////////////////////////////////
		// log10(SlowDown) = A * log10(Mag) + B
		// OffsetOLens = OffsetScreenX * dfSlowDown;
		// Mag = 10000			SlowDown =  0.1		= 10*(-1)
		// Mag = 100000			SlowDown =  0.005	= 10*(-2.3)	约等于
		//     ==> A = -1.3   B = 4.2
		dfMCBRatioLog10 = 4.2 - 1.3 * log10(dMag);
	}
	else
		dfMCBRatioLog10 = -2.3;	// SlowDown约等于0.005，即200个像素点改变一个控制量
	//	g_dfMCBRatio = 1 / pow( 10, dfMCBRatioLog10 );	// 多少个脉冲数可以改变一个调节量
	CGlobalParamSetting::Instance().g_dfMCBRatio = pow( 10, dfMCBRatioLog10 )*5;	// 多少个脉冲数可以改变一个调节量
	//	g_dfMCBRatio = 1 / g_dfMCBRatio;	// 多少个脉冲数可以改变一个调节量
}