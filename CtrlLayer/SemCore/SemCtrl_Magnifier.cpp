// SemCtrl_Magnifier.cpp: implementation of the CSemCtrl_Magnifier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Magnifier.h"
#include "CoadjustParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// class CAmplifier
//////////////////////////////////////////////////////////////////////
 
CAmplifier::CAmplifier()
{
	m_bReady	= FALSE;

	m_lStage	= 1;	// 250x - 2500x档
	m_lOldStage	= -1;
	m_dfRate	= 1.0;	// 250x
}

CAmplifier::~CAmplifier()
{

}

BOOL CAmplifier::SetParams( int nType, double *pdValue )
{
	if( nType == 1 )
	{
		for(int i=0; i<10; i++ )
			m_wRateXY[i] = (WORD)pdValue[i];
	}
	else if( nType == 2 )
	{
		for(int i=0; i<5; i++ )
			m_dRateMag[i] = pdValue[i];
		m_dRateMag[5] = m_dRateMag[1] / m_dRateMag[0];
		m_dRateMag[6] = m_dRateMag[2] / m_dRateMag[1];
		m_dRateMag[7] = m_dRateMag[3] / m_dRateMag[2];
		m_dRateMag[8] = m_dRateMag[4] / m_dRateMag[3];
	}
	else
	{
		m_dRateMag[0] = 15.0;
		m_dRateMag[1] = 250.0;
		m_dRateMag[2] = 2500.0;
		m_dRateMag[3] = 25000.0;
		m_dRateMag[4] = 60000.0;
		m_dRateMag[5] = m_dRateMag[1] / m_dRateMag[0];
		m_dRateMag[6] = m_dRateMag[2] / m_dRateMag[1];
		m_dRateMag[7] = m_dRateMag[3] / m_dRateMag[2];
		m_dRateMag[8] = m_dRateMag[4] / m_dRateMag[3];
	}
	return TRUE;
}

void CAmplifier::SetParam( int nType, double dParam )
{
	m_wRateXY[ nType ]	= (WORD)dParam;
	m_wRateX			= m_wRateXY[2*m_lStage];
	m_wRateY			= m_wRateXY[2*m_lStage+1];
	Flush();
}

//	==>	k	=	(0xBF -0x7FF) /9;
//		b	=	( 0x7FF *10 -0xBF ) /9
const	double	dK = -1856 /9;	// -1536 /9;
const	double	dB = 20279 /9;	//19959 /9;

BOOL CAmplifier::Flush()
{
/*	//  理论上扫描波形放大器可由公式
	//	Amplification = Rate * power( 10, Stage ) 表示
	//	Rate 范围 (1 - 10)；Stage 范围 (0 - 3)
	//
	//  实际的扫描波形放大器由两个参数控制：wStageCtl 和 wRateCtl
	//  wRateCtl	范围 (1000 - 100)	  <->  Rate 范围 (1 - 10)
	//  wStageCtl	范围 (11, 10, 01, 00) <-> Stage 范围 (0 -  3)
	//  控制参数理论参数关系如下：

	// wStageCtl = 3 - Stage = 11, 10, 01, 00
	WORD				wStageCtl, wRateCtl;
	wStageCtl = (WORD)(~m_lStage & 0x03 );

	wRateCtl = (int)(1000 / m_dfRate);
	wRateCtl &= 0x03FF;

	//  并按以下形式封装在 wCtl 中
	wCtl = (WORD)( wRateCtl | ( wStageCtl << 10 ) );

	USB_SEM_ControlMessage	usb_hostMSG;
	hostMSG.bMessageID		= (BYTE)Z80B_Amplifier;
	hostMSG.bCtlMSGLength	= (BYTE)3;
	hostMSG.bData[1]		= (BYTE)wCtl;
	hostMSG.bData[2]		= (BYTE)(wCtl >> 8);
*/
	WORD	wRateCtlX, wRateCtlY;
	USB_SEM_ControlMessage	usb_hostMSG;

	// 档位控制由SCN上不同的开关控制
	BOOL blRet = FALSE;
	if( m_lStage != m_lOldStage )
	{
		switch( m_lStage )
		{
		case	0:
			blRet = AmpMain();
			break;
		case	1:
			blRet = AmpLow1();
			break;
		case	2:
			blRet = AmpLow2();
			break;
		case	3:
			blRet = AmpLow3();
			break;
		case	4:
			blRet = AmpLow4();
			break;
		}
		m_lOldStage = m_lStage;
		m_wRateX	= m_wRateXY[2*m_lStage];
		m_wRateY	= m_wRateXY[2*m_lStage+1];
	}
/*
	// 控制值wRateCtrl传送给FineMag???
	//  wRateCtl	范围 (0x7FF - 0x1FF)	  <->  Rate 范围 (1 - 10)
	//	y		=	kx	+b;
	//	0x7FF	=	1K	+b;
	//	0x1FF	=	10K	+b;
	//	==>	k	=	(0x1FF -0x7FF) /9;
	//		b	=	( 0x7FF *10 -0x1FF ) /9
	//	RateCtl	=	( 0x7FF *10 -0x1FF ) /9 - (0x7FF -0x1FF) /9 *Rate
	wRateCtl = (WORD)(dK * m_dfRate +dB);
*/
	wRateCtlX = (WORD)(m_wRateX / m_dfRate);
	if( wRateCtlX > 0x0FFF )
		wRateCtlX = 0x0FFF;
	wRateCtlY = (WORD)(m_wRateY / m_dfRate);
	if( wRateCtlY > 0x0FFF )
		wRateCtlY = 0x0FFF;

	usb_hostMSG.bType			= (BYTE)0;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_Det45StoreDAC;
	usb_hostMSG.bData[1]		= (BYTE)TRUE;
	// FineMagX
	usb_hostMSG.bData[2]		= (BYTE)( wRateCtlX & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( (wRateCtlX >> 8)  | (0x02 << 6) ) ;
	blRet = PostMSGEx( usb_hostMSG );
	// FineMagY
	usb_hostMSG.bData[2]		= (BYTE)( wRateCtlY & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( (wRateCtlY >> 8)  | (0x03 << 6) ) ;
	blRet = PostMSGEx( usb_hostMSG );

	return blRet;
}

BOOL CAmplifier::PostMSGEx( USB_SEM_ControlMessage msg )
{
	BOOL blRet = FALSE;
	int i = 0;
	do
	{
		blRet = CMP::Instance().USB_PostMSGEx( msg );
		if( blRet )
			break;
		i++;
		Sleep(1);
	} while( i < 10 );
	if( !blRet )
		TRACE( "Amplifier can not post msg\n" );
	return blRet;
}

void CAmplifier::GetParam( int nType, double& dRateCtrl )
{
	switch( nType )
	{
	case	1:
		dRateCtrl = (WORD)(m_wRateX / m_dfRate);
		if( dRateCtrl > 0x0FFF )
			dRateCtrl = 0x0FFF;
		break;
	case	2:
		dRateCtrl = (WORD)(m_wRateY / m_dfRate);
		if( dRateCtrl > 0x0FFF )
			dRateCtrl = 0x0FFF;
		break;

	case	3:
		dRateCtrl = m_lStage;
		break;
	case	4:
		dRateCtrl = m_dfRate;
		break;

	case	10:
		dRateCtrl = m_dRateMag[0];
		break;
	case	11:
		dRateCtrl = m_dRateMag[1];
		break;
	case	12:
		dRateCtrl = m_dRateMag[2];
		break;
	case	13:
		dRateCtrl = m_dRateMag[3];
		break;
	case	14:
		dRateCtrl = m_dRateMag[4];
		break;
	case	15:
		dRateCtrl = m_dRateMag[5];
		break;
	case	16:
		dRateCtrl = m_dRateMag[6];
		break;
	case	17:
		dRateCtrl = m_dRateMag[7];
		break;
	case	18:
		dRateCtrl = m_dRateMag[8];
		break;
	}
}

long CAmplifier::Update( double dfMagMin, double dfMag )
{
	double	dfMagBasis	= dfMagMin;

	if ( ( dfMag < 1.0 ) || ( dfMagMin < 1.0 ) )
		return -1;

	if( dfMag <= dfMagMin )
	{
		m_lStage	= 0;
		m_dfRate	= 1.0;

		return m_lStage;
	}

	if( dfMag < dfMagBasis *m_dRateMag[5] )		// 15x - 250x
	{
		m_lStage	= 0;
		m_dfRate	= dfMag /dfMagBasis;
	}
	else if( dfMag *1000 < dfMagBasis *m_dRateMag[5] *m_dRateMag[6] *1000 )
	{
		m_lStage	= 1;
		m_dfRate	= dfMag / dfMagBasis /m_dRateMag[5];
	}
	else if( dfMag *1000 < dfMagBasis *m_dRateMag[5] *m_dRateMag[6] *m_dRateMag[7] *1000 )
	{
		m_lStage	= 2;
		m_dfRate	= dfMag / dfMagBasis /m_dRateMag[5] /m_dRateMag[6];
	}
	else if( dfMag *1000 < dfMagBasis *m_dRateMag[5] *m_dRateMag[6] *m_dRateMag[7] *m_dRateMag[8] *1000 )
	{
		m_lStage	= 3;
		m_dfRate	= dfMag / dfMagBasis /m_dRateMag[5] /m_dRateMag[6] /m_dRateMag[7];
	}
	else
	{
		m_lStage	= 4;
		m_dfRate	= dfMag / dfMagBasis /m_dRateMag[5] /m_dRateMag[6] /m_dRateMag[7] /m_dRateMag[8];
	}
	///////////////////////////////////////////////////////
	return m_lStage;
}

double CAmplifier::GetMagFromAmp( double dfMagMin )
{
	double	dfMagBasis	= dfMagMin;
	double	dfMag;

	///////////////////////////////////////////////////////
	// 2017.10.24
	// 将切档倍数作为可变参数存储于配置文件中
	// 2017.12.01
	// 各切档倍数可随意调节
	// RateMag[0] - RateMag[1]		为第0档，	Rate=1.0 - RateMag[1] / RateMag[0]
	// RateMag[1] - RateMag[2]		为第1档，	Rate=1.0 - RateMag[2] / RateMag[1]
	// RateMag[2] - RateMag[3]		为第2档，	Rate=1.0 - RateMag[3] / RateMag[2]
	// RateMag[3] - RateMag[4]		为第3档，	Rate=1.0 - RateMag[4] / RateMag[3]
	// RateMag[4]				以上为第4档，	Rate=1.0 - 150000 / RateMag[4]
	switch( m_lStage )
	{
	case	0:	// RateMag[0] - RateMag[1]
		dfMag = dfMagBasis * m_dfRate;
		break;
	case	1:	// RateMag[1] - RateMag[2]
		dfMag = dfMagBasis *m_dRateMag[5] * m_dfRate;
		break;
	case	2:	// RateMag[2] - RateMag[3]
		dfMag = dfMagBasis *m_dRateMag[5] *m_dRateMag[6] * m_dfRate;
		break;
	case	3:	// RateMag[3] - RateMag[4]
		dfMag = dfMagBasis *m_dRateMag[5] *m_dRateMag[6] *m_dRateMag[7] * m_dfRate;
		break;
	case	4:
		dfMag = dfMagBasis *m_dRateMag[5] *m_dRateMag[6] *m_dRateMag[7] *m_dRateMag[8] * m_dfRate;
		break;
	}
	///////////////////////////////////////////////////////

	return dfMag;
}

BOOL CAmplifier::UpdateStrobe( BYTE bID, BOOL bHigh )
{
	BYTE bytePos = bID;
	if( bHigh )
		bytePos |= 0x80;
	else
		bytePos |= 0x40;

	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_ExpanderChip;	// Low byte = ckt addr. of SCN I/O Expander
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_08;		// Serial Comm operand high byte (&H11 = 8 bit write to SCN board)
	usb_hostMSG.bData[2]		= bytePos;
	usb_hostMSG.bData[3]		= bytePos;

	BOOL blRet = FALSE;
	blRet = PostMSGEx( usb_hostMSG );

	return blRet;
}

BOOL CAmplifier::UpdateLowAmp( BOOL bHigh )
{
	// X) 24 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, bHigh ) )
		return FALSE;
	// Y) 31 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, bHigh ) )
		return FALSE;
	OutputDebugString( "Amplifier: Update LowAmpESCtrl finished" );
	return TRUE;
}

BOOL CAmplifier::UpdateGainRly( BOOL bHigh )
{
	// XGainRly 17 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, bHigh ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, bHigh ) )
		return FALSE;
	OutputDebugString( "Amplifier: Update GainRlyCtrl finished" );
	return TRUE;
}

BOOL CAmplifier::UpdateMainX( BOOL bHigh )
{
	// X) 20, 21, 22 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, bHigh ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, bHigh ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, bHigh ) )
		return FALSE;
	OutputDebugString( "Amplifier: Update XMain Ctrls finished" );
	return TRUE;
}

BOOL CAmplifier::UpdateMainY( BOOL bHigh )
{
	// X) 20, 21, 22 = ON
	if( !UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, bHigh ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, bHigh ) )
		return FALSE;
	if( !UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, bHigh ) )
		return FALSE;
	OutputDebugString( "Amplifier: Update YMain Ctrls finished" );
	return TRUE;
}

BOOL CAmplifier::UpdateShortRly( BOOL bHigh )
{
	// X) 18 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XShortRlyCtrl, bHigh ) )
		return FALSE;
	// Y) 25 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YShortRlyCtrl, bHigh ) )
		return FALSE;
	OutputDebugString("Amplifier: Clear ShoftRlyCtrl finished");
	return TRUE;
}

BOOL CAmplifier::UpdateStigES( BOOL bHigh )
{
	// X) 19 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XAmpStigESCtrl, bHigh ) )
		return FALSE;
	// Y) 26 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YAmpStigESCtrl, bHigh ) )
		return FALSE;
	OutputDebugString("Amplifier: Update StigESCtrl finished");
	return TRUE;
}

BOOL CAmplifier::UpdateShiftES( BOOL bHigh )
{
	// X) 23 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_XShiftESCtrl, bHigh ) )
		return FALSE;
	// Y) 30 = OFF
	if( !UpdateStrobe( SCN_ExpanderChip_YShiftESCtrl, bHigh ) )
		return FALSE;
	OutputDebugString("Amplifier: Update ShiftESCtrl finished");
	return TRUE;
}

BOOL CAmplifier::UpdateLowRngMux( short nIndex )
{
	USB_SEM_ControlMessage	usb_hostMSG;
	BOOL blRet = FALSE;
	BYTE byteBitValues = 0;

	switch( nIndex )
	{
	case	1:	// AmpLow1
		// XYLowRngMux) 4
		byteBitValues |= USB_Flag_Bit3;
		break;
	case	2:	// AmpLow2
		// XYLowRngMux) 3
		byteBitValues |= USB_Flag_Bit2;
		break;
	case	3:	// AmpLow3
		// XYLowRngMux) 2
		byteBitValues |= USB_Flag_Bit1;
		break;
	case	4:	// AmpLow4
		// XYLowRngMux) 1
		byteBitValues |= USB_Flag_Bit0;
		break;
	}

	// First write X values:
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_XLowRangeAnalogMultiplexerSel;
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_08;
	usb_hostMSG.bData[2]		= byteBitValues;
	usb_hostMSG.bData[3]		= byteBitValues;
	blRet = PostMSGEx( usb_hostMSG );
	// Now write Y values:
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_YLowRangeAnalogMultiplexerSel;
	blRet = PostMSGEx( usb_hostMSG );

	OutputDebugString("Amplifier: Update LowRngMux finished");
	return TRUE;
}

BOOL CAmplifier::AmpMain()
{
	// Approx mag: 15 to 150
	// Current range: >2.5A to 250mA

	OutputDebugString("Amplifier: Begin AmpMain");
	/////////////////////////////////////////////////////////////////
	// low E/S) 24, 31 = OFF
	UpdateLowAmp( FALSE );
	// GainRly) 17, 1 = OFF
	UpdateGainRly( FALSE );

	ClearAnalogSwitches();

	// MainX) 20, 21, 22 = ON
	UpdateMainX( TRUE );
	// MainY) 27, 28, 29 = ON
	UpdateMainY( TRUE );
	/////////////////////////////////////////////////////////////////
/*
	USB_SEM_ControlMessage	usb_hostMSG;
	/////////////////////////////////////////////////////////////////
	// MainGain 1 (X & Y)
	// MainFB 1 (X & Y)
	BYTE byteBitValues = 0;
//	byteBitValues |= USB_Flag_Bit2;	// FB
	byteBitValues |= USB_Flag_Bit0;	// Gain
	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_XMainGain_FeedbackSetting;	// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_08;			// Serial Comm operand high byte (8 bit write to SCN board)
	usb_hostMSG.bData[2]		= byteBitValues;
	usb_hostMSG.bData[3]		= byteBitValues;
	blRet = PostMSGEx( usb_hostMSG );
	// Now write Y values:
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_YMainGain_FeedbackSetting;	// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
	blRet = PostMSGEx( usb_hostMSG );
	/////////////////////////////////////////////////////////////////
*/
	OutputDebugString("Amplifier: Finish AmpMain");
	OutputDebugString("\r\n");
	return TRUE;
}

BOOL CAmplifier::AmpLow1()
{
	// Approx mag: 150 to 1500
	// Current range: 250mA to 25mA

	OutputDebugString("Amplifier: Begin AmpLow1");
	/////////////////////////////////////////////////////////////////
	// MainX) 20, 21, 22 = OFF
	UpdateMainX( FALSE );
	// MainY) 27, 28, 29 = OFF
	UpdateMainY( FALSE );

	ClearAnalogSwitches();

	/////////////////////////////////////////////////////////////////
	// low E/S) 24, 31 = ON
	UpdateLowAmp( TRUE );
	// GainRly 17, 1 = ON
	UpdateGainRly( TRUE );

	UpdateLowRngMux( 1 );

	OutputDebugString("Amplifier: Finish AmpLow1");
	OutputDebugString("\r\n");
	return TRUE;
}

BOOL CAmplifier::AmpLow2()
{
	// Approx mag: 1500 to 15K
	// Current range: 25mA to 2.5mA

	OutputDebugString("Amplifier: Begin AmpLow2");
	/////////////////////////////////////////////////////////////////
	// MainX) 20, 21, 22 = OFF
	UpdateMainX( FALSE );
	// MainY) 27, 28, 29 = OFF
	UpdateMainY( FALSE );
	// GainRly 17, 1 = OFF
	UpdateGainRly( FALSE );

	ClearAnalogSwitches();

	/////////////////////////////////////////////////////////////////
	// low E/S) 24, 31 = ON
	UpdateLowAmp( TRUE );

	UpdateLowRngMux( 2 );

	OutputDebugString("Amplifier: Finish AmpLow2");
	OutputDebugString("\r\n");
	return TRUE;
}

BOOL CAmplifier::AmpLow3()
{
	// Approx mag: 15K to 150K
	// Current range: 2.5mA to 250uA

	OutputDebugString("Amplifier: Begin AmpLow3");
	/////////////////////////////////////////////////////////////////
	// MainX) 20, 21, 22 = OFF
	UpdateMainX( FALSE );
	// MainY) 27, 28, 29 = OFF
	UpdateMainY( FALSE );
	// GainRly 17, 1 = OFF
	UpdateGainRly( FALSE );

	ClearAnalogSwitches();

	/////////////////////////////////////////////////////////////////
	// low E/S) 24, 31 = ON
	UpdateLowAmp( TRUE );

	UpdateLowRngMux( 3 );

	OutputDebugString("Amplifier: Finish AmpLow3");
	OutputDebugString("\r\n");
	return TRUE;
}

BOOL CAmplifier::AmpLow4()
{
	// Approx mag: >150K
	// Current range: <250uA

	OutputDebugString("Amplifier: Begin AmpLow4");
	/////////////////////////////////////////////////////////////////
	// MainX) 20, 21, 22 = OFF
	UpdateMainX( FALSE );
	// MainY) 27, 28, 29 = OFF
	UpdateMainY( FALSE );
	// GainRly 17, 1 = OFF
	UpdateGainRly( FALSE );

	ClearAnalogSwitches();

	/////////////////////////////////////////////////////////////////
	// low E/S) 24, 31 = ON
	UpdateLowAmp( TRUE );

	UpdateLowRngMux( 4 );

	OutputDebugString("Amplifier: Finish AmpLow4");
	OutputDebugString("\r\n");
	return TRUE;
}

BOOL CAmplifier::ClearAnalogSwitches()
{
	BOOL blRet = FALSE;

	USB_SEM_ControlMessage	usb_hostMSG;
	/////////////////////////////////////////////////////////////////
	// MainGain 1 (X & Y)
	// MainFB 1 (X & Y)
	BYTE byteBitValues = 0;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_XMainGain_FeedbackSetting;	// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_08;			// Serial Comm operand high byte (8 bit write to SCN board)
	usb_hostMSG.bData[2]		= byteBitValues;
	usb_hostMSG.bData[3]		= byteBitValues;
	blRet = PostMSGEx( usb_hostMSG );
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_YMainGain_FeedbackSetting;	// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
	blRet = PostMSGEx( usb_hostMSG );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// XYLowRngMux
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_XLowRangeAnalogMultiplexerSel;
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_08;
	usb_hostMSG.bData[2]		= byteBitValues;
	usb_hostMSG.bData[3]		= byteBitValues;
	blRet = PostMSGEx( usb_hostMSG );
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_YLowRangeAnalogMultiplexerSel;
	blRet = PostMSGEx( usb_hostMSG );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// XYLowRngFB
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_XLowRangeFeedbackSelect;
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_08;
	usb_hostMSG.bData[2]		= byteBitValues;
	usb_hostMSG.bData[3]		= byteBitValues;
	blRet = PostMSGEx( usb_hostMSG );
	// Now write Y values:
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_YLowRangeFeedbackSelect;
	blRet = PostMSGEx( usb_hostMSG );
	/////////////////////////////////////////////////////////////////

	OutputDebugString("Amplifier: Clear Analog Switches finished");
	return TRUE;
}

BOOL CAmplifier::ClearAllChannels() 
{
	// TODO: Add your control notification handler code here
//	for( int i=0; i<31; i++ )
//		UpdateStrobe( SCN_ExpanderChip_Xcros1 +i, FALSE );
	UpdateMainX( FALSE );
	UpdateMainY( FALSE );
	UpdateGainRly( FALSE );
	UpdateLowAmp( FALSE );
	UpdateShortRly( FALSE );
	UpdateStigES( FALSE );
	UpdateShiftES( FALSE );

	ClearAnalogSwitches();
	return TRUE;
}


//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
// 2005-2-2_统调
//各种工作距离下的参数：I_i/I_12 * 100 （乘100是为了取整时保留小数点后两位）
//用于放大倍数计算
const WORD CSemCtrl_Magnifier::m_nWDIndexToParam[] = 
{
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,

	0x4D,	0x4E,	0x4F,	0x50,	0x51,	0x52,	0x53,	0x55,	0x56,	0x58,
	0x59,	0x5B,	0x5C,	0x5E,	0x60,	0x62,	0x64,	0x68,	0x6B,	0x6F,
	0x72,	0x77,	0x7B,	0x7F,	0x84,	0x89,	0x8E,
};
*/

//const SemCtrlFigures DefaultCtrlFigures_Magnifier	= {{0,0,0,0, (DOUBLE)15.0}, {0,0,0,0, (DOUBLE)150000.0}, {0,0,0,0, (DOUBLE)150.0}};

CSemCtrl_Magnifier::CSemCtrl_Magnifier()
{
	//m_scfCtrl = DefaultCtrlFigures_Magnifier;
	m_nDebugMode		= 0;
	m_nRotateLimitAngle = 10;

	///////////////////////////////////////////////////////
	// 2005-2-2_统调 ？
	m_scfCtrl.varMin.dblVal = (DOUBLE)15.0;
	m_scfCtrl.varMax.dblVal = (DOUBLE)150000.0;
	m_scfCtrl.varPos.dblVal = (DOUBLE)250.0;

	double*		pdouble	= new double[16];
	if( CSemBasisCtrl::ReadINI( INI_Ratio, INIIndex_MagK, pdouble ) )
		m_Amp.SetParams( 1, pdouble );

	// Coadjust
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_FactorMin, pdouble ) )
	{
		m_dCoFactorA	= pdouble[0];
		m_dCoFactorB	= pdouble[1];
	}
	else
	{
		m_dCoFactorA	= 0.0;
		m_dCoFactorB	= 0.0;
	}
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 2017.10.24
	// 将切档倍数作为可变参数存储于配置文件中
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_MagChange, pdouble ) )
	{
		m_Amp.SetParams( 2, pdouble );
		m_scfCtrl.varMin.dblVal = (DOUBLE)pdouble[0];
		m_scfCtrl.varMax.dblVal = (DOUBLE)150000.0;
		m_scfCtrl.varPos.dblVal = (DOUBLE)pdouble[1];
	}
	else
		m_Amp.SetParams( -1, pdouble );
	///////////////////////////////////////////////////////
	delete [] pdouble;

	m_pShiftX	= NULL;
	m_pShiftY	= NULL;
	m_pRotate	= NULL;
}

CSemCtrl_Magnifier::~CSemCtrl_Magnifier()
{

}


BOOL CSemCtrl_Magnifier::Bind( CSemCtrl_ScanShiftX*	pShiftX )
{
	//CSemCtrl_ScanShiftX * pShiftXSafety = DYNAMIC_DOWNCAST(CSemCtrl_ScanShiftX, pShiftX);
	
	if ( pShiftX != NULL )
	{
		m_pShiftX	= pShiftX;
	}

	return TRUE;
}

BOOL CSemCtrl_Magnifier::Bind( CSemCtrl_ScanShiftY*	pShiftY )
{
	//CSemCtrl_ScanShiftY * pShiftYSafety = DYNAMIC_DOWNCAST(CSemCtrl_ScanShiftY, pShiftY);
	
	if ( pShiftY != NULL )
	{
		m_pShiftY	= pShiftY;
	}

	return TRUE;
}

BOOL CSemCtrl_Magnifier::Bind( CSemCtrl_ScanRotate*	pRotate )
{
	//CSemCtrl_ScanRotate * pRotateSafety = DYNAMIC_DOWNCAST(CSemCtrl_ScanRotate, pRotate);
	
	if ( pRotate != NULL )
	{
		m_pRotate	= pRotate;
	}

	return TRUE;
}

BOOL CSemCtrl_Magnifier::CoherenceCheck()
{
	BOOL blRet = TRUE;
	
	if ( m_scfCtrl.varMax.dblVal < m_scfCtrl.varMin.dblVal )
	{
		m_scfCtrl.varMax.dblVal = m_scfCtrl.varMin.dblVal;
		blRet = FALSE;
	}

	if ( m_scfCtrl.varPos.dblVal > m_scfCtrl.varMax.dblVal )
	{
		m_scfCtrl.varPos.dblVal = m_scfCtrl.varMax.dblVal;
		blRet = FALSE;
	}

	if ( m_scfCtrl.varPos.dblVal < m_scfCtrl.varMin.dblVal )
	{
		m_scfCtrl.varPos.dblVal = m_scfCtrl.varMin.dblVal;
		blRet = FALSE;
	}

	double	dfMag		= m_scfCtrl.varPos.dblVal;
	double	dfMagMin	= m_scfCtrl.varMin.dblVal;
	long	lStage		= m_Amp.Update(dfMagMin, dfMag);

	return blRet;
}

VARIANT CSemCtrl_Magnifier::Plus( VARIANT verDelt )
{
	m_scfCtrl.varPos.dblVal += verDelt.dblVal;
	
	CoherenceCheck();

	return m_scfCtrl.varPos;
}

VARIANT CSemCtrl_Magnifier::Minus( VARIANT verDelt )
{
	m_scfCtrl.varPos.dblVal -= verDelt.dblVal;

	CoherenceCheck();

	return m_scfCtrl.varPos;
}

const CString& CSemCtrl_Magnifier::FormatText()
{
	VARIANT varPos = GetPos();
	double dfMagnification = varPos.dblVal;

	// Format Magnification Display string
	if( dfMagnification < 10. )
	{
		m_strText.Format( "%1.2f X", dfMagnification );
	}
	else if( dfMagnification < 100. )
	{
		m_strText.Format( "%2.1f X", dfMagnification );
	}
	else if( dfMagnification < 1000. )
	{
		m_strText.Format( "%3.0f X", dfMagnification );
	}
	else if( dfMagnification < 10000. )
	{
		m_strText.Format( "%1.2f KX", dfMagnification / 1000 );
	}
	else if( dfMagnification < 100000. )
	{
		m_strText.Format( "%2.1f KX", dfMagnification / 1000 );
	}
	else //if( dfMagnification < 1000000. )
	{
		m_strText.Format( "%3.0f KX", dfMagnification / 1000 );
	}

	return m_strText;
}

const CString& CSemCtrl_Magnifier::FormatText( VARIANT varPos )
{
	static CString strText;
	double dfMagnification = varPos.dblVal;

	// Format Magnification Display string
	if( dfMagnification < 10. )
	{
		strText.Format( "%1.2f X", dfMagnification );
	}
	else if( dfMagnification < 100. )
	{
		strText.Format( "%2.1f X", dfMagnification );
	}
	else if( dfMagnification < 1000. )
	{
		strText.Format( "%3.0f X", dfMagnification );
	}
	else if( dfMagnification < 10000. )
	{
		strText.Format( "%1.2f KX", dfMagnification / 1000 );
	}
	else if( dfMagnification < 100000. )
	{
		strText.Format( "%2.1f KX", dfMagnification / 1000 );
	}
	else //if( dfMagnification < 1000000. )
	{
		strText.Format( "%3.0f KX", dfMagnification / 1000 );
	}

	return strText;
}

BOOL CSemCtrl_Magnifier::UpdateWDIndex( long lWDIndex )
{
//	static	BOOL boolInit = TRUE;

/*	if ( boolInit )
	{
		boolInit = FALSE;
	}
	else
*/	{
		if ( m_lWDIndex == lWDIndex )
			return FALSE;
		else
			m_lWDIndex = lWDIndex;
	}

	// 2005-2-2_统调
	// double dfMagMin	= 14.9 * ( 1.893 * m_nWDIndexToParam[ m_lWDIndex ] / 100 - 0.893 );
	double dfMagMin	= m_dfWDIndexToMAGmin[ m_lWDIndex ];

	VARIANT varMin, varMax, varPos;

	varMin.dblVal = dfMagMin;
	varMax.dblVal = dfMagMin * 10000;
	varPos.dblVal = m_Amp.GetMagFromAmp( dfMagMin );

	SetRange( varMin, varMax );
	SetPos( varPos );

	return TRUE;
}

BOOL CSemCtrl_Magnifier::Co_UpdatePosByM( double dM )
{
	// Mag = 10 * (M /S) / 2^8;
	// S即为Rate，范围从100到1000
	double dfMagMin = (10 * (dM /1000) / 256) /10;	// 每个WD对应的MagMin是在S即Rate=1000时
	if( dfMagMin < 8 )	// 似乎原来3200在28mm时的最小放大倍数是8.41X
		return FALSE;
	// 15.01.27 由于无法找到扫描线圈驱动电流的范围超出的原因，
	//			从软件上做限制，不允许放大倍数在30KV、12mm情况下小于20倍

	VARIANT varMin, varMax, varPos;

	varMin.dblVal = dfMagMin;
	varMax.dblVal = dfMagMin * 10000;
	varPos.dblVal = m_Amp.GetMagFromAmp( dfMagMin );

	SetRange( varMin, varMax );
	SetPos( varPos );

	return TRUE;
}

BOOL CSemCtrl_Magnifier::Co_UpdatePosByWD( double dWD )
{
	double dfMagMin = 0.0;
	if( m_nType < 2 )
	{
		// 6200(3200): MagMin = 3326 / (WD+100), WD的单位是0.1mm
		dfMagMin = 3326.0 / (dWD+100);
	}
	else
	{
		// 6900 & 6200: MagMin = A / (WD+B), WD的单位是mm
		dfMagMin = m_dCoFactorA / (dWD +m_dCoFactorB);
	}

	VARIANT varMin, varMax, varPos;

	varMin.dblVal = dfMagMin;
	varMax.dblVal = dfMagMin * 10000;
	varPos.dblVal = m_Amp.GetMagFromAmp( dfMagMin );

	SetRange( varMin, varMax );
	SetPos( varPos );

	return TRUE;
}

BOOL CSemCtrl_Magnifier::Flush()
{
//	if( !m_Amp.isReady() )
//		return FALSE;

	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	BOOL bRet = m_Amp.Flush();

	double dStage, dRate, dRate2;
	GetParams(3, dStage );
	GetParams(4, dRate );
	GetParams(15, dRate2 );
	// 2017.10.24
	// 将切档倍数作为可变参数存储于配置文件中
//	if( (dStage < 1) && (dRate < 25.0/15.0) )
	if( (dStage < 1) && (dRate < dRate2/10) )
	{
		// 25x以下禁用扫描旋转
		VARIANT varPos = m_pRotate->GetPos();
/*		if( varPos.lVal != 0 )
		{
			varPos.lVal = 0;
			m_pRotate->SetPos( varPos );
			m_pRotate->Flush();
		}
*/
		if( varPos.lVal > m_nRotateLimitAngle && m_nDebugMode < 1 )
		{
			// 非调试模式下小于25x，则恢复为扫描旋转初始值
			m_pRotate->SetParams( 11, 0 );
		}
	}
	return bRet;
}

void CSemCtrl_Magnifier::SetParams( int nType, double dParam )	// Type:类型；Param:要设置的参数值
{
	if( nType < 10 )
	{
		m_Amp.SetParam( nType, dParam );
		return;
	}

	switch( nType )
	{
	case	11:
		m_dCoFactorA = dParam;
		break;
	case	12:
		m_dCoFactorB = dParam;
		break;

	case	50:		// 调试模式下任何放大倍数不禁用扫描旋转
		m_nDebugMode = (int)dParam;
		break;
	case	51:
		m_nRotateLimitAngle = (int)dParam;
		break;

	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;

	case	101:	// SemType
		m_nType	= (int)dParam;
		break;

	default:
		break;
	}
}

void CSemCtrl_Magnifier::GetParams( int nType, double& dParam )
{
	if( nType < 50 )
		m_Amp.GetParam( nType, dParam );
	else
		dParam = m_nDebugMode;
}