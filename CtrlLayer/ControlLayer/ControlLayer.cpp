// ControlLayer.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include <math.h>
#include "ControlLayer.h"
#include "SemCtrl.h"
#include "SemDataType.h"
#include "SemCtrlPanelManager.h"

#include "GlobalParamSetting.h"
#include "AppOption.h"
#include "DebugMessageDispatcher.h"
#include "BoardComm.h"
#include "Delayer.h"
#include "GraphBuffManager.h"
#include "CFGFileManager.h"
#include "Scenario.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CControlLayer& CControlLayer::Instance()
{
	static CControlLayer Inst;
	return Inst;
}

void  CControlLayer::Release()
{
	if (m_pScenario)
	{
		delete m_pScenario;m_pScenario = NULL;
	}

	if( m_AT.pAutoTestPower != NULL )
		delete [] m_AT.pAutoTestPower;
	if( m_AT.pAutoTestPowerExp != NULL )
		delete [] m_AT.pAutoTestPowerExp;
	if( m_AT.pAutoTestPowerErr != NULL )
		delete [] m_AT.pAutoTestPowerErr;
	if( m_AT.pPower_OutofRange != NULL )
		delete [] m_AT.pPower_OutofRange;
	if( m_AT.pAutoTestVAC != NULL )
		delete [] m_AT.pAutoTestVAC;
	for(int i=0; i<4; i++ )
	{
		if( m_AT.pAutoTestLNS[i] != NULL )
			delete [] m_AT.pAutoTestLNS[i];
		if( m_AT.pAutoTestSCN[i] != NULL )
			delete [] m_AT.pAutoTestSCN[i];
		if( m_AT.pAutoTestMCI[i] != NULL )
			delete [] m_AT.pAutoTestMCI[i];
		if( m_AT.pAutoTestSPR[i] != NULL )
			delete [] m_AT.pAutoTestSPR[i];
	}
	for(int i=0; i<4; i++ )
	{
		if( m_Cali.pCaliLNS[i] != NULL )
			delete [] m_Cali.pCaliLNS[i];
		if( m_Cali.pCaliSCN[i] != NULL )
			delete [] m_Cali.pCaliSCN[i];
		if( m_Cali.pCaliMCI[i] != NULL )
			delete [] m_Cali.pCaliMCI[i];
		if( m_Cali.pCaliSPR[i] != NULL )
			delete [] m_Cali.pCaliSPR[i];
	}

	m_bHasBSD	= FALSE;
	m_nPAType	= 0;

	for( int i=0; i<12; i++ )
		m_nDACTCvalue[i] = 2048;
}


CControlLayer::CControlLayer()
{
	m_pScenario = new CScenario;

	m_nAdjustorManualSort	= SORT_MANUAL_NULL;	// 手动调节器类型为无
	CSemCtrlPanelManager::Instance().Bind();
	// DF,FF
	m_bDFforFF		= TRUE;

	_InitAutoTest();
	_InitCalibration();
	// Board VAC
	_InitNet();
	_InitBrdMCI();
}

void CControlLayer::MCB_Adjust_Ratio( double dMag )
{
	double dfMCBRatioLog10 = 0.0;
	if( dMag < 10000 )
	{
		// log10(SlowDown) = A * log10(Mag) + B
		// OffsetOLens = OffsetScreenX * dfSlowDown;
		// Mag = 10000			SlowDown =  0.1
		// Mag =     1			SlowDown =  100
		//     ==> A = -0.75   B = 2
		dfMCBRatioLog10 = 2 - 0.75 * log10(dMag);
	}
	else if( dMag < 100000 )
	{
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


void CControlLayer::SyncFineMag()
{
	double dblVal, dblVal2, dblStage;
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 1, dblVal );
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 2, dblVal2 );
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 3, dblStage );

	CDebugMessageDispatcher::Instance().SyncFineMag(dblVal,dblVal2, dblStage);
}


void CControlLayer::Co_UpdateScan()
{
	CDebugMessageDispatcher::Instance().Co_UpdateScan();
}


void CControlLayer::MCI_Det03SB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos;
	m_DL.diag[Diag_Offset_0 + nChn].nCurPos = nPos;
	m_BrdMCI.nOffset[nChn] = 2048 -nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (nChn << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det03StoreDAC, TRUE, wValue );
}

void CControlLayer::MCI_Det45SB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos;
	m_DL.diag[Diag_Offset_4 + nChn].nCurPos = nPos;
	if( nChn < 2 )
		m_BrdMCI.nOffset[nChn +4] = 2048 -nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (nChn << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
}

void CControlLayer::MCI_DetGainSB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue = nPos;
	m_BrdMCI.nGain[nChn] = nPos;

	WORD wValue = (WORD)( (nChn << 8) | (BYTE)nValue );
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );
}

void CControlLayer::MCI_BWselSB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue = nPos;
	m_BrdMCI.nFilter[nChn] = nPos;

	// filter pin in operand low byte
	// detector number in operand high byte
	WORD wValue = (WORD)( (nChn << 8) | (BYTE)nValue );
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
}

void CControlLayer::MCI_UpdateSpotX( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	m_DL.diag[Diag_Pos_X].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)nPos );
}

void CControlLayer::MCI_UpdateSpotY( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	m_DL.diag[Diag_Pos_Y].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)nPos );
}


void CControlLayer::SCN_XstigDACSB_Change( int nPos )
{
	m_DL.diag[Diag_Stig_X].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal	= 1;
		Xpos.lVal	= nPos;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CControlLayer::SCN_YstigDACSB_Change( int nPos )
{
	m_DL.diag[Diag_Stig_Y].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal	= 2;
		Ypos.lVal	= nPos;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CControlLayer::SCN_XshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Shift_X].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
		USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}

void CControlLayer::SCN_YshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Shift_Y].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
		USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}

void CControlLayer::SCN_XxoverDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_xover_X].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
		USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}

void CControlLayer::SCN_SyncXOverPos( int nPosX, int nPosY ) 
{
	m_DL.diag[Diag_xover_X].nCurPos = nPosX -2048;
	m_DL.diag[Diag_xover_Y].nCurPos = nPosY -2048;
}

void CControlLayer::SCN_YxoverDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_xover_Y].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
		USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}


void CControlLayer::LNS_L1DACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	m_DL.diag[Diag_CL].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_CondenserLens;
		Flag.lVal	= 1;
		Xpos.lVal	= nPos;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CControlLayer::LNS_L2SDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	m_DL.diag[Diag_OL].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// ObjectiveLens
	{
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Flag.lVal	= 1;
		Xpos.dblVal	= nPos;
		Xpos.lVal	= nPos;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CControlLayer::LNS_L2DDACSB_Change( int nPos )
{
}

void CControlLayer::LNS_XalDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 32768;
	m_DL.diag[Diag_Align_X].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x42 as low byte (XalDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_XAlignment,		// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
		USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte

	CDebugMessageDispatcher::Instance().UpdateBeemAlignmentX(nPos,0);
}

void CControlLayer::LNS_YalDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 32768;
	m_DL.diag[Diag_Align_Y].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x52 as low byte (YalDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_YAlignment,	// Serial Comm operand low byte (Y alignment DAC circuit address)
		USB_TM_LNS_Write_16,	// Serial Comm operand high byte (16 bit write to LNS board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte

	CDebugMessageDispatcher::Instance().UpdateBeemAlignmentY(nPos,0);
}

void CControlLayer::LNS_WobDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Wob].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_WobblerSetting,	// Serial Comm operand low byte (Y alignment DAC circuit address)
		USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
		byteLo,		// Serial Comm operand low byte
		byteHi );	// Serial Comm operand high byte
}


void CControlLayer::Diag_Init( int nID, BOOL bSet )
{
	switch( nID )
	{
	case	Diag_Offset_0:
		MCI_Det03SB_Change( m_DL.diag[nID].nMin, 0 );
		break;
	case	Diag_Offset_1:
		MCI_Det03SB_Change( m_DL.diag[nID].nMin, 1 );
		break;
	case	Diag_Offset_2:
		MCI_Det03SB_Change( m_DL.diag[nID].nMin, 2 );
		break;
	case	Diag_Offset_3:
		MCI_Det03SB_Change( m_DL.diag[nID].nMin, 3 );
		break;
	case	Diag_Offset_4:
		MCI_Det45SB_Change( m_DL.diag[nID].nMin, 0 );
		break;
	case	Diag_Offset_5:
		MCI_Det45SB_Change( m_DL.diag[nID].nMin, 1 );
		break;
	case	Diag_FineMag_X:
		MCI_Det45SB_Change( m_DL.diag[nID].nMin, 2 );
		break;
	case	Diag_FineMag_Y:
		MCI_Det45SB_Change( m_DL.diag[nID].nMin, 3 );
		break;
	case	Diag_Pos_X:
		break;
	case	Diag_Pos_Y:
		break;

	case	Diag_Stig_X:
		SCN_XstigDACSB_Change( m_DL.diag[nID].nMin -2048);
		break;
	case	Diag_Stig_Y:
		SCN_YstigDACSB_Change( m_DL.diag[nID].nMin -2048);
		break;
	case	Diag_Shift_X:
		SCN_XshiftDACSB_Change( m_DL.diag[nID].nMin -2048 );
		break;
	case	Diag_Shift_Y:
		SCN_YshiftDACSB_Change( m_DL.diag[nID].nMin -2048 );
		break;
	case	Diag_xover_X:
		SCN_XxoverDACSB_Change( m_DL.diag[nID].nMin -2048 );
		break;
	case	Diag_xover_Y:
		SCN_YxoverDACSB_Change( m_DL.diag[nID].nMin -2048 );
		break;

	case	Diag_CL:
		LNS_L1DACSB_Change( m_DL.diag[nID].nMin -32768);
		break;
	case	Diag_OL:
		LNS_L2SDACSB_Change( m_DL.diag[nID].nMin -32768);
		break;
	case	Diag_Align_X:
		LNS_XalDACSB_Change( m_DL.diag[nID].nMin -32768 );
		break;
	case	Diag_Align_Y:
		LNS_YalDACSB_Change( m_DL.diag[nID].nMin -32768 );
		break;
	case	Diag_Wob:
		LNS_WobDACSB_Change( m_DL.diag[nID].nMin  -2048);
		break;

	default:
		break;
	}
}

CString CControlLayer::Diag_SCN_LNS()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";

	if( m_DL.diag[Diag_Stig_X].nCheck )
	{
		nIndex = Diag_Stig_X;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			SCN_XstigDACSB_Change( nPos);
		}
	}
	if( m_DL.diag[Diag_Stig_Y].nCheck )
	{
		nIndex = Diag_Stig_Y;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			SCN_YstigDACSB_Change( nPos);
		}
	}
	if( m_DL.diag[Diag_Shift_X].nCheck )
	{
		nIndex = Diag_Shift_X;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			SCN_XshiftDACSB_Change( nPos );
		}
	}
	if( m_DL.diag[Diag_Shift_Y].nCheck )
	{
		nIndex = Diag_Shift_Y;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[Diag_Shift_Y].nFinish = 1;
			}
			SCN_YshiftDACSB_Change( nPos );
		}
	}
	if( m_DL.diag[Diag_xover_X].nCheck )
	{
		nIndex = Diag_xover_X;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			SCN_XxoverDACSB_Change( nPos );
		}
	}
	if( m_DL.diag[Diag_xover_Y].nCheck )
	{
		nIndex = Diag_xover_Y;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			SCN_YxoverDACSB_Change( nPos );
		}
	}

	if( m_DL.diag[Diag_CL].nCheck )
	{
		nIndex = Diag_CL;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -32768;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			LNS_L1DACSB_Change( nPos);
		}
	}
	if( m_DL.diag[Diag_OL].nCheck )
	{
		nIndex = Diag_OL;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -32768;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			LNS_L2SDACSB_Change( nPos );
		}
	}
	if( m_DL.diag[Diag_Align_X].nCheck )
	{
		nIndex = Diag_Align_X;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -32768;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			LNS_XalDACSB_Change( nPos );
		}
	}
	if( m_DL.diag[Diag_Align_Y].nCheck )
	{
		nIndex = Diag_Align_Y;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -32768;
				//				m_DL.diag[Diag_Align_Y].nFinish = 1;
			}
			LNS_YalDACSB_Change( nPos );
		}
	}
	if( m_DL.diag[Diag_Wob].nCheck )
	{
		nIndex = Diag_Wob;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = m_DL.diag[nIndex].nMin -2048;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			LNS_WobDACSB_Change( nPos );
		}
	}

	return strRes;
}


CString CControlLayer::Diag_MCI()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";

	if( m_DL.diag[Diag_Offset_0].nCheck )
	{
		nIndex = Diag_Offset_0;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det03SB_Change( nPos, 0 );
		}
	}
	if( m_DL.diag[Diag_Offset_1].nCheck )
	{
		nIndex = Diag_Offset_1;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det03SB_Change( nPos, 1 );
		}
	}
	if( m_DL.diag[Diag_Offset_2].nCheck )
	{
		nIndex = Diag_Offset_2;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det03SB_Change( nPos, 2 );
		}
	}
	if( m_DL.diag[Diag_Offset_3].nCheck )
	{
		nIndex = Diag_Offset_3;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det03SB_Change( nPos, 3 );
		}
	}
	if( m_DL.diag[Diag_Offset_4].nCheck )
	{
		nIndex = Diag_Offset_4;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det45SB_Change( nPos, 0 );
		}
	}
	if( m_DL.diag[Diag_Offset_5].nCheck )
	{
		nIndex = Diag_Offset_5;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det45SB_Change( nPos, 1 );
		}
	}
	if( m_DL.diag[Diag_FineMag_X].nCheck )
	{
		nIndex = Diag_FineMag_X;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det45SB_Change( nPos, 2 );
		}
	}
	if( m_DL.diag[Diag_FineMag_Y].nCheck )
	{
		nIndex = Diag_FineMag_Y;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_Det45SB_Change( nPos, 3 );
		}
	}
	if( m_DL.diag[Diag_Pos_X].nCheck )
	{
		nIndex = Diag_Pos_X;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_UpdateSpotX( nPos, TRUE );
		}
	}
	if( m_DL.diag[Diag_Pos_Y].nCheck )
	{
		nIndex = Diag_Pos_Y;
		int nPos = m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos );
		strRes += str;
		if( m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += m_DL.diag[nIndex].nStep;
			if( nPos > m_DL.diag[nIndex].nMax +1)
			{
				nPos = m_DL.diag[nIndex].nMin;
				//				m_DL.diag[nIndex].nFinish = 1;
			}
			MCI_UpdateSpotY( nPos, TRUE );
		}
	}

	return strRes;
}


CString CControlLayer::Diag_Logging() 
{
	CString str, strRes;
	strRes = "";

	if( m_DL.other.lTimes > GraphDataBuffCounts )
		return strRes;

	int i, j=0;
	int nTotal = 16;

	// Gauge Comparators
	if( m_DL.other.bGaugeCompr )
	{
		str = CBoardComm::Instance().ReadComprs_Logging();
		strRes += str;
	}


	// LNS set value and comp switch
	if( m_DL.log.bLogParam[Logging_CL] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_CL].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_OL] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_OL].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Align_X] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_Align_X].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Align_Y] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_Align_Y].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Wob] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_Wob].nCurPos +2048);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_CL] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_CL -5]);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_OL] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_OL -5]);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_Align_X] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_Align_X -5]);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_Align_Y] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_Align_Y -5]);
		strRes += str;
	}

	double dValue = 0.0;
	int k=0;
	// SCN
	CBoardComm::Instance().ChangedADCselectOB( 0 );
	for( i=0; i<nTotal; i++ )
	{
		if( m_DL.log.bLog[j*16 +i] )
		{
			CBoardComm::Instance().ChangedADCsChSelect( i, m_DL.adc.chn );
			if( m_DL.other.bDebug )
			{
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn,TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn,TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
			}
			else
			{
				str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue, m_DL.adc, m_DL.log );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}


	j++;
	nTotal = 8;
	// LNS
	CBoardComm::Instance().ChangedADCselectOB( 1 );
	for( i=0; i<nTotal; i++ )
	{
		if( m_DL.log.bLog[j*16 +i] )
		{
			CBoardComm::Instance().ChangedADCsChSelect( i,m_DL.adc.chn );
			if( m_DL.other.bDebug )
			{
				// 13.05.09 测试时保留每一次读的值，而非读两次只保留第二次的值
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue,m_DL.adc.chn );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, m_DL.adc.chn, TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, m_DL.adc.chn);
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
				// 13.05.09 测试时保留每一次读的值，而非读两次只保留第二次的值
			}
			else
			{
				str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, m_DL.adc, m_DL.log );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}


	j++;
	nTotal = 16;
	// VAC0
	CBoardComm::Instance().ChangedADCselectOB( 2 );
	for( i=0; i<nTotal; i++ )
	{
		if( m_DL.log.bLog[j*16 +i] )
		{
			CBoardComm::Instance().ChangedADCsChSelect( i,m_DL.adc.chn );
			if( m_DL.other.bDebug )
			{
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue,m_DL.adc.chn, TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn, TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
			}
			else
			{
				str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue,m_DL.adc, m_DL.log );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}

	j++;
	// VAC1
	CBoardComm::Instance().ChangedADCselectOB( 3 );
	for( i=0; i<nTotal; i++ )
	{
		if( m_DL.log.bLog[j*16 +i] )
		{
			CBoardComm::Instance().ChangedADCsChSelect( i, m_DL.adc.chn );
			if( m_DL.other.bDebug )
			{
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue,m_DL.adc.chn,TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn, TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
			}
			else
			{
				str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue,m_DL.adc, m_DL.log );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}


	j++;
	// SPR0
	CBoardComm::Instance().ChangedADCselectOB( 4 );
	for( i=0; i<nTotal; i++ )
	{
		if( m_DL.log.bLog[j*16 +i] )
		{
			CBoardComm::Instance().ChangedADCsChSelect( i, m_DL.adc.chn );
			if( m_DL.other.bDebug )
			{
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn,TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn,TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
			}
			else
			{
				str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue,m_DL.adc,m_DL.log );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}


	j++;
	// SPR1
	CBoardComm::Instance().ChangedADCselectOB( 5 );
	for( i=0; i<nTotal; i++ )
	{
		if( m_DL.log.bLog[j*16 +i] )
		{
			CBoardComm::Instance().ChangedADCsChSelect( i , m_DL.adc.chn);
			if( m_DL.other.bDebug )
			{
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn, TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, m_DL.adc.chn, TRUE );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
				// 13.05.09 调试模式下保留每一次读的值，而非读两次只保留第二次的值
			}
			else
			{
				str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue, m_DL.adc, m_DL.log );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == j*16 +i) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}

	return strRes;
}


CString CControlLayer::Diag_Logging2() 
{
	CString str, strRes;
	strRes = "";
	int i, j=0;
	int nTotal = 16;

	// LNS set value and comp switch
	if( m_DL.log.bLogParam[Logging_CL] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_CL].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_OL] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_OL].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Align_X] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_Align_X].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Align_Y] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_Align_Y].nCurPos +32768);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Wob] )
	{
		str.Format( "%04X\t", m_DL.diag[Diag_Wob].nCurPos +2048);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_CL] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_CL -5]);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_OL] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_OL -5]);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_Align_X] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_Align_X -5]);
		strRes += str;
	}
	if( m_DL.log.bLogParam[Logging_Comp_Align_Y] )
	{
		str.Format( "%d\t", m_DL.log.nCheckComp[Logging_Comp_Align_Y -5]);
		strRes += str;
	}

	// 发送所有需回读的命令
	nTotal = m_DL.log2.nCounts;	// 有多少需要从FIFO回读的参数
	int nSendReadCmdCounts = 0;			// 为从FIFO回读所有参数，总共发送了多少次回读命令
	for( i=0; i<nTotal; i++ )
	{
		logParam lP = m_DL.log2.logParams[i];
		switch( lP.nType )
		{
		case	2:
			{
				CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
				CDelayer::Instance().Delay( 100 );
				nSendReadCmdCounts++;
			}
			break;

		case	3:
			{
				CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
				CDelayer::Instance().Delay( 100 );
				nSendReadCmdCounts++;
			}
			break;

		default:
			{
				if( lP.bType )
				{
					// 2448，发送2次读命令
					CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
					CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
					CDelayer::Instance().Delay( 100 );
					CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
					CDelayer::Instance().Delay( 100 );
					nSendReadCmdCounts += 3;
				}
				else
				{
					// 1859，发送3次读命令
					CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
					CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
					CDelayer::Instance().Delay( 5 );
					CBoardComm::Instance().OnlySendReadSelectedADCcmd( lP );
					CDelayer::Instance().Delay( 5 );
					nSendReadCmdCounts += 3;
				}
			}
			break;
		}
	}
	CDelayer::Instance().Delay( 50 );

	// 读一次FIFO
	long lReadSize = 96*10 +128;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	DWORD lRet = CSemCtrl::Instance().USB_ReadBoardData( lReadSize, pnFrameData, &lBytesTransferred );

	if( (lRet != 0) || (lBytesTransferred < 1) )
	{
		delete [] pnFrameData;
		return strRes;
	}

	// 先看一看0xD22D的个数是否符合发出命令的个数
	nTotal = lBytesTransferred/2;
	int nRecvReadCounts = 0;	// 总共收到多少组回读值，每组6个数据，每个0xD22D后面跟着一组回读值
	for( i=0; i<nTotal; i++ )
	{
		if( pnFrameData[i] == 0xD22D )
			nRecvReadCounts++;
	}
	if( nRecvReadCounts != nSendReadCmdCounts )
	{
		delete [] pnFrameData;
		return strRes;
	}

	// 将有效的0xD22D后边的数据取出
	// 每个有效数据由两个16位数据组成
	int *pnValidData;
	int nValidCount = 0;	// 总共要分析记录多少组数据
	if( m_DL.other.bDebug )
	{
		pnValidData = new int[nRecvReadCounts *2];
		ZeroMemory(pnValidData, sizeof(pnValidData));
		for( i=0; i<nTotal; i++ )
		{
			if( pnFrameData[i] == 0xD22D )
			{
				// 13.05.09 调试模式下保留每一次读的值
				pnValidData[nValidCount *2]		= pnFrameData[i+1];
				pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
				nValidCount++;
				i += 2;
			}
		}
	}
	else
	{
		pnValidData = new int[m_DL.log2.nCounts *2];
		ZeroMemory(pnValidData, sizeof(pnValidData));
		int nD22Dcount	= 0;	// 非调试模式下，用于记录已忽略的无效数据的个数
		for( i=0; i<nTotal; i++ )
		{
			if( pnFrameData[i] == 0xD22D )
			{
				nD22Dcount++;
				if( m_DL.log2.logParams[nValidCount].nType == 2 )		// gauge compr
				{
					nD22Dcount = 0;
					pnValidData[nValidCount *2]		= pnFrameData[i+1];
					pnValidData[nValidCount *2 +1]	= pnFrameData[i+1];
					nValidCount++;
					i += 1;
				}
				else if( m_DL.log2.logParams[nValidCount].nType == 3 )	// Ext. switch
				{
					nD22Dcount = 0;
					pnValidData[nValidCount *2]		= pnFrameData[i+1];
					pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
					nValidCount++;
					i += 2;
				}
				else
				{
					if( m_DL.log2.logParams[nValidCount].bType && nD22Dcount == 3 )
					{
						nD22Dcount = 0;
						pnValidData[nValidCount *2]		= pnFrameData[i+1];
						pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
						nValidCount++;
						i += 2;
					}
					else if( !(m_DL.log2.logParams[nValidCount].bType) && nD22Dcount == 3 )
					{
						nD22Dcount = 0;
						pnValidData[nValidCount *2]		= pnFrameData[i+1];
						pnValidData[nValidCount *2 +1]	= pnFrameData[i+1];
						nValidCount++;
						i += 1;
					}
				}
			}
		}
	}

	// 分析数据并记录
	int k=0;
	double dValue = 0.0;
	if( m_DL.other.bDebug )
	{
		int m = 0;
		nTotal = nValidCount;
		for( i=0; i<nTotal; i++ )
		{
			logParam lP = m_DL.log2.logParams[m];
			m++;
			if( lP.nType == 2 )
			{
				str.Format( "%04X\t", pnValidData[i*2] );
				strRes += str;
			}
			else if( lP.nType == 3 )
			{
				str = "";
				strRes += str;
			}
			else
			{	
				str = CBoardComm::Instance().ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue,m_DL.adc.chn );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts] = dValue;
				i++;
				str = CBoardComm::Instance().ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue, m_DL.adc.chn );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +1] = dValue;
				i++;
				str = CBoardComm::Instance().ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue, m_DL.adc.chn  );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lLoggingDataCounts +2] = dValue;
					k++;
				}
			}
		}
	}
	else
	{
		nTotal = m_DL.log2.nCounts;
		for( i=0; i<nTotal; i++ )
		{
			logParam lP = m_DL.log2.logParams[i];
			if( lP.nType == 2 )
			{
				str.Format( "%04X\t", pnValidData[i*2] );
				strRes += str;
			}
			else if( lP.nType == 3 )
			{
				str = "";
				strRes += str;
			}
			else
			{
				str = CBoardComm::Instance().ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue, m_DL.adc.chn );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
				{
					CGraphBuffMgr::Instance().m_pGraphDataBuff[k][m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}

	delete [] pnFrameData;
	delete [] pnValidData;

	return strRes;
}

void CControlLayer::CheckDet2nd( int nCheck )
{
	CSemVirtualMCICard::Instance().SetParams(4, nCheck);
	CSemVirtualMCICard::Instance().SetParams(SET_PARAMS_DetectorMode, nCheck);

	CString str;
	str.Format( "Check Det 2nd: %d", nCheck );
	CDebugMessageDispatcher::Instance().DisplayMsg(str);
	m_BrdMCI.bDet2nd = nCheck;
	// Brightness
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_Det2nd, m_BrdMCI.bDet2nd );
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	ClassID.lVal = SemControlPanelClass_Brightness;
	Flag.lVal = 1;
	Xpos.lVal = m_BrdMCI.nOffset[ 2 * m_BrdMCI.nDetPair + m_BrdMCI.bDet2nd ];
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}


void CControlLayer::MCI_BWselSB_Change( int nPos ) 
{
	int nValue = nPos;
	CString str = "";
	m_BrdMCI.nFilter[ m_BrdMCI.nDetPair ] = nPos;

	WORD wValue = (BYTE)nValue;				// filter pin in operand low byte
	wValue |= (((BYTE)m_BrdMCI.nDetPair) << 8 );	// detector number in operand high byte
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );

	str.Format( "Press select filter: %X", nValue );
	CDebugMessageDispatcher::Instance().DisplayMsg(str);
}


void CControlLayer::MCI_DetGain2SB_Change( int nPos ) 
{
	int nValue = nPos;
	CString str = "";
	m_BrdMCI.nGain[ 2*m_BrdMCI.nDetPair +1 ] = nPos;

	WORD wValue = (BYTE)nValue;
	if( m_BrdMCI.nDetPair == 0 )
		wValue |= ( (WORD)(pow((double)2, m_BrdMCI.nDetPair)) << 8);
	else
		wValue |= ( (WORD)(pow((double)2, m_BrdMCI.nDetPair) +1) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );

	str.Format( "Press select gain 2: %X", nValue );
	CDebugMessageDispatcher::Instance().DisplayMsg(str);
}


void CControlLayer::MCI_DetGainSB_Change( int nPos ) 
{
	int nValue = nPos;
	CString str = "";
	m_BrdMCI.nGain[ 2*m_BrdMCI.nDetPair ] = nPos;

	WORD wValue = (BYTE)nValue;
	if( m_BrdMCI.nDetPair != 0 )
		wValue |= ( (WORD)(pow((double)2, m_BrdMCI.nDetPair)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );

	str.Format( "Press select gain: %X", nValue );
	CDebugMessageDispatcher::Instance().DisplayMsg(str);
}


void CControlLayer::MCI_Det45SBY_Change( int nPos ) 
{
	CString str = "";
	int nValue = nPos;
	BYTE byteHi, byteLo;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (0x03 << 6)) << 8);	// Y FineMag
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );

	str.Format( "Press select FineMagY: %X", nValue );
	CDebugMessageDispatcher::Instance().DisplayMsg(str);
}


void CControlLayer::MCI_Det45SB_Change( int nPos ) 
{
	CString str = "";
	int nValue = nPos;
	BYTE byteHi, byteLo;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (0x02 << 6)) << 8);	// X FineMag
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );

	str.Format( "Press select FineMagX: %X", nValue );
	CDebugMessageDispatcher::Instance().DisplayMsg(str);
}

void CControlLayer::MCI_Sync_BrightnessParam() 
{
	// Brightness
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_DetPair, m_BrdMCI.nDetPair );
	CSemCoreAssemble::Instance().m_Brightness.SetParams( SET_PARAMS_Det2nd, m_BrdMCI.bDet2nd );
	CSemCoreAssemble::Instance().m_Signal2Brightness.SetParams( SET_PARAMS_DetPair, m_BrdMCI.nDetPair );
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	ClassID.lVal = SemControlPanelClass_Brightness;
	Xpos.lVal = m_BrdMCI.nOffset[ 2 * m_BrdMCI.nDetPair +m_BrdMCI.bDet2nd ];
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	ClassID.lVal = SemControlPanelClass_Signal2Brightness;
	Xpos.lVal = m_BrdMCI.nOffset[ 2 * m_BrdMCI.nDetPair +1 ];
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}


BOOL CControlLayer::SCN_UpdateStrobe( BYTE bID, BOOL bHigh )
{
	BYTE bytePos = bID;
	if( bHigh )
		bytePos |= 0x80;
	else
		bytePos |= 0x40;

	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip,	// Low byte = ckt addr. of SCN I/O Expander
		USB_TM_SCN_Write_08,		// Serial Comm operand high byte (&H11 = 8 bit write to SCN board)
		bytePos,
		bytePos );
	return TRUE;
}

BOOL CControlLayer::SCN_ClearAnalogSwitches()
{
	// MainGain 1 (X & Y)
	// MainFB 1 (X & Y)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XMainGain_FeedbackSetting,
		USB_TM_SCN_Write_08, 0x0, 0x0 );
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YMainGain_FeedbackSetting,
		USB_TM_SCN_Write_08, 0x0, 0x0 );

	// XYLowRngMux
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08, 0x0, 0x0 );
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08, 0x0, 0x0 );

	// XYLowRngFB
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XLowRangeFeedbackSelect,
		USB_TM_SCN_Write_08, 0x0, 0x0 );
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YLowRangeFeedbackSelect,
		USB_TM_SCN_Write_08, 0x0, 0x0 );

	return TRUE;
}

BOOL CControlLayer::SCN_AmpMain()
{
	// Approx mag: 15 to 250
	// Current range: >2.5A to 250mA
	CDebugMessageDispatcher::Instance().DisplayMsg("Press select AmpMain: 15x - 250x");

	// X) 24 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, FALSE ) )
		return FALSE;
	// Y) 31 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;

	SCN_ClearAnalogSwitches();

	// X) 20, 21, 22 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, TRUE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, TRUE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, TRUE ) )
		return FALSE;

	// Y) 27, 28, 29 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, TRUE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, TRUE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, TRUE ) )
		return FALSE;

	return TRUE;
}

BOOL CControlLayer::SCN_AmpLow1()
{
	// Approx mag: 250 to 2500
	// Current range: 250mA to 25mA
	CDebugMessageDispatcher::Instance().DisplayMsg("Press select AmpLow1: 250x - 2500x");
	// X) 20, 21, 22 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	SCN_ClearAnalogSwitches();

	// Xlow E/S) 24 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;
	// Ylow E/S) 31 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;

	// XGainRly 17 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, TRUE ) )
		return FALSE;
	// YGainRly 1 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, TRUE ) )
		return FALSE;

	BYTE byteBitValues = 0;
	// XYLowRngMux) 4
	byteBitValues |= USB_Flag_Bit3;
	// Write via MCI to SCN board.  First write 0x11 (8 bit write to Board 1), low byte = 0x24 (X gain/FB ctl.)
	// First write X values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XMainGain_FeedbackSetting,	// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
		USB_TM_SCN_Write_08,			// Serial Comm operand high byte (8 bit write to SCN board)
		byteBitValues,
		byteBitValues );
	// Now write Y values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YMainGain_FeedbackSetting,	// Serial Comm operand low byte (X Gain/Feedback ctrl. circuit address)
		USB_TM_SCN_Write_08,			// Serial Comm operand high byte (8 bit write to SCN board)
		byteBitValues,
		byteBitValues );

	return TRUE;
}

BOOL CControlLayer::SCN_AmpLow2()
{
	// Approx mag: 2500 to 25K
	// Current range: 25mA to 2.5mA
	CDebugMessageDispatcher::Instance().DisplayMsg("Press select AmpLow2: 2,500x - 25,000x");
	// X) 20, 21, 22 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;

	SCN_ClearAnalogSwitches();

	// Xlow E/S) 24 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;

	// Ylow E/S) 31 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;

	BYTE byteBitValues = 0;
	// XYLowRngMux) 3
	byteBitValues |= USB_Flag_Bit2;
	// First write X values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08,
		byteBitValues,
		byteBitValues );
	// Now write Y values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08,
		byteBitValues,
		byteBitValues );

	return TRUE;
}

BOOL CControlLayer::SCN_AmpLow3()
{
	// Approx mag: 25K to 60K
	// Current range: 2.5mA to 250uA
	CDebugMessageDispatcher::Instance().DisplayMsg("Press select AmpLow3: 25,000x - 60,000x");
	// X) 20, 21, 22 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;

	SCN_ClearAnalogSwitches();

	// Xlow E/S) 24 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;

	// Ylow E/S) 31 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;

	BYTE byteBitValues = 0;
	// XYLowRngMux 2
	byteBitValues |= USB_Flag_Bit1;
	// First write X values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08,
		byteBitValues,
		byteBitValues );
	// Now write Y values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08,
		byteBitValues,
		byteBitValues );

	return TRUE;
}


BOOL CControlLayer::SCN_AmpLow4()
{
	// Approx mag: >150K
	// Current range: <250uA
	CDebugMessageDispatcher::Instance().DisplayMsg("Press select AmpLow4: > 60,000x");

	// X) 20, 21, 22 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XMainAmpESCtrl, FALSE ) )
		return FALSE;

	// Y) 27, 28, 29 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainRlyCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainCathCtrl, FALSE ) )
		return FALSE;
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YMainAmpESCtrl, FALSE ) )
		return FALSE;

	// XGainRly 17 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XGainRlyCtrl, FALSE ) )
		return FALSE;
	// YGainRly 1 = OFF
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YGainRlyCtrl, FALSE ) )
		return FALSE;

	SCN_ClearAnalogSwitches();

	// Xlow E/S) 24 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_XLowAmpESCtrl, TRUE ) )
		return FALSE;

	// Ylow E/S) 31 = ON
	if( !SCN_UpdateStrobe( SCN_ExpanderChip_YLowAmpESCtrl, TRUE ) )
		return FALSE;

	BYTE byteBitValues = 0;
	// XYLowRngMux 1
	byteBitValues |= USB_Flag_Bit0;
	// First write X values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_XLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08,
		byteBitValues,
		byteBitValues );
	// Now write Y values:
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_YLowRangeAnalogMultiplexerSel,
		USB_TM_SCN_Write_08,
		byteBitValues,
		byteBitValues );

	return TRUE;
}


void CControlLayer::ClearAllChannels() 
{
	// Main Amp: 20,21,22,27,28,29
	BYTE bytePos = 0x40 | 0x14;	// 20
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x15;		// 21
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x16;		// 22
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1B;		// 27
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1C;		// 28
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1D;		// 29
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// Low Amp: 24,31
	bytePos = 0x40 | 0x18;		// 24
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1F;		// 31
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x11;		// 17
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x01;		// 1
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// Stig Ctrl switch
	bytePos = 0x40 | 0x13;		// 19
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1A;		// 26
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// ShortRly
	bytePos = 0x40 | 0x12;		// 18
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x19;		// 25
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	// Shift E/S
	bytePos = 0x40 | 0x17;		// 23
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	bytePos = 0x40 | 0x1E;		// 30
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );

	SCN_ClearAnalogSwitches();
}


void CControlLayer::CHECKStigCtrl( int nCheck ) 
{
	if( nCheck )
	{
		BYTE bytePos = 0x80 | 0x13;	// 19
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x80 | 0x1A;		// 26
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x80 | 0x17;		// 23
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x80 | 0x1E;		// 30
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x12;		// 18
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x19;		// 25
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
	else
	{
		BYTE bytePos = 0x40 | 0x13;	// 19
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x1A;		// 26
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x17;		// 23
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x1E;		// 30
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x12;		// 18
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
		CDelayer::Instance().Delay(10);//Sleep( 10 );
		bytePos = 0x40 | 0x19;		// 25
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_ExpanderChip, USB_TM_SCN_Write_08, bytePos, bytePos );
	}
}


void CControlLayer::CHECKFakeMainslock( int nCheck ) 
{
	if( nCheck )
	{
		CSemCtrl::Instance().USB_WriteToBoard( USB_FakeMains_ON, FALSE, 0 );
	}
	else
	{
		CSemCtrl::Instance().USB_WriteToBoard( USB_FakeMains_OFF, FALSE, 0 );
	}
}


void CControlLayer::CHECKMainsLock( int nCheck ) 
{
	if( nCheck )
	{
		CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_ON, FALSE, 0 );
	}
	else
	{
		CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
	}
}


void CControlLayer::MainsLockFrameSync() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_MainsLock_FrameSync, FALSE, 0 );
}


void CControlLayer::CHECKCompCond( int nCheck ) 
{
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_CondenserLens_LoopTest,	// Serial Comm operand low byte (L1 compensation disable ctl.)
		USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
		nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
		nCheck );	
}


void CControlLayer::CHECKCompObj( int nCheck ) 
{
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ObjectiveLens_LoopTest,	// Serial Comm operand low byte (L1 compensation disable ctl.)
		USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
		nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
		nCheck );		// Hi byte operand is what is written during 8 bit write
}


void CControlLayer::CHECKCompAlignX( int nCheck ) 
{
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_XAlignment_LoopTest,	// Serial Comm operand low byte (L1 compensation disable ctl.)
		USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
		nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
		nCheck );		// Hi byte operand is what is written during 8 bit write
}


void CControlLayer::CHECKCompAlignY( int nCheck ) 
{
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_YAlignment_LoopTest,	// Serial Comm operand low byte (L1 compensation disable ctl.)
		USB_TM_LNS_Write_08,				// Serial Comm operand high byte (8 bit write to LNS board)
		nCheck,			// Serial Comm operand low byte (actually n/a as of 5-23-11 for 8 bit write)
		nCheck );		// Hi byte operand is what is written during 8 bit write
}

void CControlLayer::CHECKVideo( int nCheck ) 
{
	if( nCheck )
	{
		CSemCtrl::Instance().USB_StreamOpen( TRUE );	// TRUE for read; FALSE for write.
		CSemCtrl::Instance().USB_VideoON();
	}
	else
	{
		CSemCtrl::Instance().USB_StreamClose();
		CSemCtrl::Instance().USB_VideoOFF();
	}
}


void CControlLayer::CHECKTestLEDs( int nCheck ) 
{
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Test_LED,		// Low byte = ckt addr. of Test LED 16
		USB_TM_SCN_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
		0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
		nCheck );					// Hi byte data is actually low byte of 8 bit write

	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_Test_LED,		// Low byte = ckt addr. of Test LED 16
		USB_TM_LNS_Write_08,		// Serial Comm operand high byte (8 bit write to LNS board)
		0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
		nCheck );					// Hi byte data is actually low byte of 8 bit write

	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_Test_LED,		// Low byte = ckt addr. of Test LED 16
		USB_TM_VAC_Write_08,		// Serial Comm operand high byte (8 bit write to VAC board)
		0x0,						// OP3 = 0 for 8 bit write
		nCheck );					// OP4 contains bit 0 value for LED on/off control

	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SPR_Test_LED,		// Low byte = ckt addr. of Test LED 16
		USB_TM_SPR_Write_08,		// Serial Comm operand high byte (8 bit write to SPR board)
		0x0,						// OP3 = 0 for 8 bit write
		nCheck );					// OP4 contains bit 0 value for LED on/off control
}


void CControlLayer::CHECKDebugModeCoadjust( int nCheck ) 
{
	if( nCheck )
	{
		CDebugMessageDispatcher::Instance().DisplayMsg("Press debug coadjust: debug mode");
		// 调试模式下，禁止使用放大倍数控件
		VARIANT ClassID, SerialNumber, Flag, Enabled;
		SerialNumber.lVal = -1;

		// Magnifier
		{
			ClassID.lVal = SemControlPanelClass_Magnifier;
			Enabled.lVal	= 0;
			Flag.lVal		= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		}

	}
	else
	{
		CDebugMessageDispatcher::Instance().DisplayMsg("Press debug coadjust: normal mode");
		VARIANT ClassID, SerialNumber, Flag, Enabled;
		SerialNumber.lVal = -1;

		// Magnifier
		{
			ClassID.lVal = SemControlPanelClass_Magnifier;
			Enabled.lVal	= 1;
			Flag.lVal		= 3;
			CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		}
	}
}


void CControlLayer::SetDetPair(const WORD wData ) 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_DetPairSelection, TRUE, wData );
	m_BrdMCI.nDetPair = (int)wData;
}

void CControlLayer::GetMagnifierParam( double* dRateMag ) 
{
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 10, dRateMag[0] );
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 11, dRateMag[1] );
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 12, dRateMag[2] );
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 13, dRateMag[3] );
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 14, dRateMag[4] );
}

void CControlLayer::CHECKFPGArelayEn11() 
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<100; j++ )
		{
			ClassID.lVal = SemControlPanelClass_ObjectiveLens;
			Flag.lVal = 1;
			Xpos.lVal += 100;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			CDelayer::Instance().Delay(11);
		}
		for(int j=0; j<1000; j++ )
		{
			ClassID.lVal = SemControlPanelClass_ObjectiveLens;
			Flag.lVal = 1;
			Xpos.lVal -= 100;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			CDelayer::Instance().Delay(11);
		}
	}
}


void CControlLayer::SPR_HVOn( BOOL bHVOn ) 
{
	int nWsize, nModID;
	BYTE bCktAddr;
	WORD wData;
	if( bHVOn )
	{
		wData		= 0x89;		// Strobe IOX output HVonA high
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		wData		= 0x8C;		// Strobe IOX output HVonB high
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );

		// J12开后，允许加高压，且设置初始值为0
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
		ClassID.lVal		= SemControlPanelClass_HV;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 1;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

		SerialNumber.lVal	= -1;
		// HV to 0
		ClassID.lVal		= SemControlPanelClass_HV;
		Flag.lVal			= 1;
		Xpos.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
	}
	else
	{
		// J12关时，不允许加高压，且恢复高压值为0
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Enabled, Flag;
		SerialNumber.lVal	= -1;
		// HV to 0
		ClassID.lVal		= SemControlPanelClass_HV;
		Flag.lVal			= 1;
		Xpos.lVal			= 0;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

		ClassID.lVal		= SemControlPanelClass_HV;
		SerialNumber.lVal	= -1;
		Enabled.lVal		= 0;
		Flag.lVal			= 3;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag);

		// 使用3603后，SPR上控制老高压的部分没有用了。暂时保留，等韩勇短接信号
		wData		= 0x49;		// Strobe IOX output HVonA low
		nWsize		= 8;
		nModID		= USB_MID_SPR;
		bCktAddr	= USB_CID_SPR_ExpanderChip;// 0x72
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
		wData		= 0x4C;		// Strobe IOX output HVonB low
		CBoardComm::Instance().BoardWrite0816( nWsize, nModID, bCktAddr, wData );
	}

}


void CControlLayer::SyncDate( BOOL bNewVal ) 
{
	CSemVirtualMCICard::Instance().ShowTime( bNewVal );
	if (bNewVal == TRUE)
	{
		CTime timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime;
		if( CAppOption::Instance().m_footerOpt.opt[0].strDisp.IsEmpty() )
			csCurrentTime = timeCurrent.Format("%Y.%m.%d %H:%M");
		else
			csCurrentTime = timeCurrent.Format(CAppOption::Instance().m_footerOpt.opt[0].strDisp);
		CSemVirtualMCICard::Instance().UpdateTime( csCurrentTime );
	}
}


void CControlLayer::_InitAutoTest() 
{
	// AutoTest
	m_AT.pAutoTestPower	= NULL;
	m_AT.pAutoTestPowerExp= NULL;
	m_AT.pAutoTestPowerErr= NULL;
	m_AT.pPower_OutofRange= NULL;
	m_AT.pAutoTestVAC = NULL;
	for(int i=0; i<4; i++)
	{
		m_AT.pAutoTestLNS[i] = NULL;
		m_AT.pAutoTestSCN[i] = NULL;
		m_AT.pAutoTestMCI[i] = NULL;
		m_AT.pAutoTestSPR[i] = NULL;
	}
}

void CControlLayer::_InitCalibration() 
{
	// Calibration
	for(int i=0; i<4; i++ )
	{
		m_Cali.pCaliLNS[i] = NULL;
		m_Cali.pCaliSCN[i] = NULL;
		m_Cali.pCaliMCI[i] = NULL;
		m_Cali.pCaliSPR[i] = NULL;
	}

	for(int i=0; i<10; i++ )
	{
		m_Cali.dCaliCoX[i]	= 0.0;
		m_Cali.dCaliCoY[i]	= 0.0;
		m_Cali.dCaliCoWD[i]	= 0.0;
		m_Cali.bCaliCoSel[i]	= FALSE;
	}
	m_Cali.nCaliCoCounts	= 0;
}

void CControlLayer::_InitNet() 
{
	// Net Param
	m_Net.AddrLen	= 0;
	m_Net.Ret		= 0;
	m_Net.hThread	= NULL;
	ZeroMemory( &(m_Net.LocalAddr), sizeof( struct sockaddr_in));
}

void CControlLayer::_InitBrdMCI() 
{
	// Board MCI
	int i;
	for( i=0; i<6; i++ )
	{
		m_BrdMCI.nOffset[i]	= 0;
		m_BrdMCI.nGain[i]	= 5;
	}
	for( i=0; i<3; i++ )
		m_BrdMCI.nFilter[i]	= 0;
	m_BrdMCI.nDetPair	= 0;
	m_BrdMCI.bDet2nd	= FALSE;
	for( i=0; i<3; i++ )
	{
		m_BrdMCI.strChnName[i*2]	= "SE";
		m_BrdMCI.strChnName[i*2 +1]	= "BSE";
	}
	m_BrdMCI.nChnIndex[0]	= 0;
	m_BrdMCI.nChnIndex[1]	= 1;
}