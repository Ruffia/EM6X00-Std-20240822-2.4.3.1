// DlgImpPanelDebug2.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgImpPanelDebug2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug2 dialog

void CDlgImpPanelDebug2::OnReadVACextSwitchStates() 
{
	// TODO: Add your control notification handler code here
	VAC_ReadVACextSwitchStates();
}

void CDlgImpPanelDebug2::VAC_ReadVACextSwitchStates() 
{
	// Before each read, must first select which switch is to be read via the I/O Extender chip (addresses 0x19,0x1A, 0x1B)
	// Open and Closed ("Clsd") Results are sent to label array: VACextSwStatesLabel(index)

	// Example below: Select switch #4 by setting select bits to 1/0/0 (requires 3 back-to-back 8 bit writes to IOX)
	BYTE bOp1, bOp2, bOp3, bOp4;
	BYTE bSwSelect2, bSwSelect1, bSwSelect0;
	int nNumWords, nBoardData, nDataType, nSwNum, i;
	for( nSwNum=0; nSwNum<8; nSwNum++ )
	{
		// 复位开关状态
		theApp.m_BrdVAC.bStates[nSwNum] = FALSE;
		GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( "unknown" );

		// 设置3个开关位
		bSwSelect2	= nSwNum & 0x4;
		bSwSelect1	= nSwNum & 0x2;
		bSwSelect0	= nSwNum & 0x1;

		bOp1		= USB_CID_VAC_ExpanderChip;	// Circuit address of I/O Expander 0x72
		bOp2		= USB_TM_VAC_Write_08;
		bOp3		= 0x0;
		if( bSwSelect2 )
			bOp4	= 0x19 | 0x80;	// Strobe select line 2@0x19 to a high state
		else
			bOp4	= 0x19 | 0x40;	// Strobe select line 2@0x19 to a low state
		theApp.m_SemCtrl.USB_CommToBoards2(	bOp1, bOp2, bOp3, bOp4 );

		if( bSwSelect1 )
			bOp4	= 0x1A | 0x80;	// Strobe select line 1@0x1A to a high state
		else
			bOp4	= 0x1A | 0x40;	// Strobe select line 1@0x1A to a low state
		theApp.m_SemCtrl.USB_CommToBoards2(	bOp1, bOp2, bOp3, bOp4 );

		if( bSwSelect0 )
			bOp4	= 0x1B | 0x80;	// Strobe select line 0@0x1B to a high state
		else
			bOp4	= 0x1B | 0x40;	// Strobe select line 0@0x1B to a low state
		theApp.m_SemCtrl.USB_CommToBoards2(	bOp1, bOp2, bOp3, bOp4 );

		// 读取选定开关的状态
		unsigned long lBytesTransferred = 0;
		// Allocate on the heap
//		unsigned short* pnFrameData = new unsigned short[ lReadSize +256 ];
//		ZeroMemory(pnFrameData, sizeof(pnFrameData));

		if( theApp.m_SemCtrl.USB_LoopbackAndReadData( USB_TTTT_Switches, ReadBack_DataSize, theApp.m_pnReadCommonData, &lBytesTransferred, -1 ) )
		{
			// 分析数据
			theApp.m_nRBwdCnt = 0;
			BOOL bFound = FALSE;
			for( unsigned long i=1; i<=theApp.m_pnReadCommonData[0]; i++ )
			{
//				str.Format( "%04X, ", pnFrameData[i] );
//				strMsg += str;
//				strList.Format( "@ %ld (hex %04X)", pnFrameData[i], pnFrameData[i] );

				// 直到找到0xD22D才开始计数，前边的数据忽略
				if( theApp.m_pnReadCommonData[i] == 0xD22D )
					bFound = TRUE;
				if( !bFound )
					continue;

				theApp.m_nRBwdCnt++;
				theApp.m_nBoardReadDataBuf[ theApp.m_nRBwdCnt*2 ] = theApp.m_pnReadCommonData[i];	// Numeric value (0-FFFF range)
				switch( theApp.m_pnReadCommonData[i] )
				{
				case	0xFFFF:
					theApp.m_nBoardReadDataBuf[ theApp.m_nRBwdCnt*2 +1 ] = 9;
					break;
				case	0xD22D:
					theApp.m_nBoardReadDataBuf[ theApp.m_nRBwdCnt*2 +1 ] = 4;
					break;
				default:
					theApp.m_nBoardReadDataBuf[ theApp.m_nRBwdCnt*2 +1 ] = 1;
					break;
				}
			}
			theApp.m_nBoardReadDataBuf[0] = theApp.m_nRBwdCnt;	// # of valid readback words contained in the array
		}
//		delete [] pnFrameData;

		// BoardReadDatBuf(256, 2) '2nd element is type: "FFFF"=9, marker "D22D"=4, Board Readback Data=1, (0,0)=count
		nNumWords = theApp.m_nBoardReadDataBuf[0];	// # of words read from board stored in Read Subroutine
		for( i=1; i<=nNumWords; i++ )		// For I = 1 To NumWords
		{
			nBoardData	= theApp.m_nBoardReadDataBuf[i*2+1];
			nDataType	= theApp.m_nBoardReadDataBuf[i*2+2];
			CString str;
			if( nDataType == 1 )	// Board readback data - use it
			{
				if( nBoardData & 1 )
				{
					theApp.m_BrdVAC.bStates[nSwNum] = TRUE;
					str.Format( "S %d:OPEN", nSwNum );
					GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( str );
				}
				else
				{
					theApp.m_BrdVAC.bStates[nSwNum] = FALSE;
					str.Format( "S %d:CLSD", nSwNum );
					GetDlgItem( IDC_STATIC_VACextSwStatesLabel0 +nSwNum )->SetWindowText( str );
				}
			}
		}
	}
}

BOOL CDlgImpPanelDebug2::VAC_JudgeLogic(void)
{
	BOOL bOpen = FALSE;
	VAC_ReadVACextSwitchStates();
	VAC_TCsStatus_Check(4);

	BOOL bOldState, bUpdate;

	// 有阀动作，就设置更新标志
	// 有更新标志，就重新判断一下各阀状态
	do 
	{
		// 清楚更新标志
		bUpdate = FALSE;

		bOldState = theApp.m_BrdVAC.bStates[State_V2];
		bOpen = VAC_JudgeV2();
		if( bOldState != bOpen)
		{
			VAC_UpdateValveSwitchANDLEDs( RelayEnAddr_Vf, IDC_STATIC_Lamp_Vacuum_Vf, (BOOL)bOpen );
			bUpdate = TRUE;
		}

		bOldState = theApp.m_BrdVAC.bStates[State_V3];
		bOpen = VAC_JudgeV3();
		if( bOldState != bOpen )
		{
			VAC_UpdateValveSwitchANDLEDs( RelayEnAddr_Vr, IDC_STATIC_Lamp_Vacuum_Vr, (BOOL)bOpen );
			bUpdate = TRUE;
		}

		bOldState = theApp.m_BrdVAC.bStates[State_V4];
		bOpen = VAC_JudgeV4();
		if( bOldState != bOpen )
		{
			VAC_UpdateValveSwitchANDLEDs( RelayEnAddr_Vc, IDC_STATIC_Lamp_Vacuum_Vc, (BOOL)bOpen );
			bUpdate = TRUE;
		}

		bOldState = theApp.m_BrdVAC.bStates[State_V5];
		bOpen = VAC_JudgeV5();
		if( bOldState != bOpen )
		{
			VAC_UpdateValveSwitchANDLEDs( RelayEnAddr_M_TMP, IDC_STATIC_Lamp_Vacuum_M_TMP, (BOOL)bOpen );
			bUpdate = TRUE;
		}

		bOldState = theApp.m_BrdVAC.bStates[State_V6];
		bOpen = VAC_JudgeV6();
		if( bOldState != bOpen )
		{
			VAC_UpdateValveSwitchANDLEDs( RelayEnAddr_Va, IDC_STATIC_Lamp_Vacuum_Va, (BOOL)bOpen );
			bUpdate = TRUE;
		}
	} while (bUpdate);

	return TRUE;
}

BOOL CDlgImpPanelDebug2::VAC_JudgeV2(void)
{
	BOOL bOpen = FALSE;

	// CR49
	// StandBy open, or V1 open, or TC2 low
	if( theApp.m_BrdVAC.bStates[State_StandBy] || theApp.m_BrdVAC.bStates[State_V1] || !VAC_TCStatus_Judge(2) )
	{
		// V5 close, and V3 close, and TC1 high
		if( !theApp.m_BrdVAC.bStates[State_V5] && !theApp.m_BrdVAC.bStates[State_V3] && VAC_TCStatus_Judge(1) )
			bOpen = TRUE;
	}

	// CR50
	// TC2 low, and V3 close, and V2 open
	if( !VAC_TCStatus_Judge(2) && !theApp.m_BrdVAC.bStates[State_V3] && theApp.m_BrdVAC.bStates[State_V2] )
		bOpen = TRUE;

	// CR51
	// TC1 high, and V5 open
	if( VAC_TCStatus_Judge(1) && theApp.m_BrdVAC.bStates[State_V5] )
		bOpen = TRUE;

	// CR52
	// TC1 high, and V6 open
	if( VAC_TCStatus_Judge(1) && theApp.m_BrdVAC.bStates[State_V6] )
		bOpen = TRUE;

	return bOpen;
}

BOOL CDlgImpPanelDebug2::VAC_JudgeV3(void)
{
	BOOL bOpen = FALSE;

	// V1,V2,V5 close,
	// Chamber,GUN,StandBy close
	// TC2 high
	// TC3 low or TC4 low
	if( !theApp.m_BrdVAC.bStates[State_V1] && !theApp.m_BrdVAC.bStates[State_V2] && !theApp.m_BrdVAC.bStates[State_V5]
		&& !theApp.m_BrdVAC.bStates[State_ChamberVent] && !theApp.m_BrdVAC.bStates[State_GunVent] && !theApp.m_BrdVAC.bStates[State_StandBy]
		&& VAC_TCStatus_Judge(2)
		&& ( !VAC_TCStatus_Judge(3) || !VAC_TCStatus_Judge(4) ) )
		bOpen = TRUE;

	return bOpen;
}

BOOL CDlgImpPanelDebug2::VAC_JudgeV4(void)
{
	BOOL bOpen = FALSE;

	// GUN open
	if( theApp.m_BrdVAC.bStates[State_GunVent] )
		bOpen = TRUE;

	// Chamber,GUN,StandBy close
	// V3 open, V5 close
	// TC3 high or V4 open
	// TC2 high, TC4 low
	if( !theApp.m_BrdVAC.bStates[State_ChamberVent] && !theApp.m_BrdVAC.bStates[State_GunVent] && !theApp.m_BrdVAC.bStates[State_StandBy]
		&& theApp.m_BrdVAC.bStates[State_V3] && !theApp.m_BrdVAC.bStates[State_V5]
		&& ( theApp.m_BrdVAC.bStates[State_V4] || VAC_TCStatus_Judge(3) )
		&& VAC_TCStatus_Judge(2) && !VAC_TCStatus_Judge(4) )
		bOpen = TRUE;

	// Chamber,GUN,StandBy close
	// V5 open, V3 close
	// TC2,3,4 high
	if( !theApp.m_BrdVAC.bStates[State_ChamberVent] && !theApp.m_BrdVAC.bStates[State_GunVent] && !theApp.m_BrdVAC.bStates[State_StandBy]
		&& theApp.m_BrdVAC.bStates[State_V5] && !theApp.m_BrdVAC.bStates[State_V3]
		&& VAC_TCStatus_Judge(2) && VAC_TCStatus_Judge(3) && VAC_TCStatus_Judge(4) )
		bOpen = TRUE;

	return bOpen;
}

BOOL CDlgImpPanelDebug2::VAC_JudgeV5(void)
{
	BOOL bOpen = FALSE;

	// Chamber,GUN,StandBy close
	// V3 close
	// V1 close or V5 open
	// TC2,3,4 high
	if( !theApp.m_BrdVAC.bStates[State_ChamberVent] && !theApp.m_BrdVAC.bStates[State_GunVent] && !theApp.m_BrdVAC.bStates[State_StandBy]
		&& !theApp.m_BrdVAC.bStates[State_V3]
		&& ( !theApp.m_BrdVAC.bStates[State_V1] || theApp.m_BrdVAC.bStates[State_V5] )
		&& VAC_TCStatus_Judge(2) && VAC_TCStatus_Judge(3) && VAC_TCStatus_Judge(4) )
		bOpen = TRUE;

	return bOpen;
}

BOOL CDlgImpPanelDebug2::VAC_JudgeV6(void)
{
	BOOL bOpen = FALSE;

	// StandBy close
	// V1,V3,V5 close
	// Chamber open or GUN open
	// delay 3s
	if( !theApp.m_BrdVAC.bStates[State_StandBy]
		&& !theApp.m_BrdVAC.bStates[State_V1] && !theApp.m_BrdVAC.bStates[State_V3] && !theApp.m_BrdVAC.bStates[State_V5]
		&& ( theApp.m_BrdVAC.bStates[State_ChamberVent] || theApp.m_BrdVAC.bStates[State_GunVent] ) )
		bOpen = TRUE;

	return bOpen;
}
