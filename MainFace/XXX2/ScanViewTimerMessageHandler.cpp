#include "StdAfx.h"
#include "ScanViewTimerMessageHandler.h"
using namespace ScanViewTimerMessageHandler;
#include "ScanView.h"
#include "Factory.h"
#include "StageXYManager.h"
#include "MCBControler.h"
#include "ControlLayer.h"


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_110,int,110)
void CLASS_ScanViewTimerMessageHandler_110::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	pScanView->SyncDate( pScanView->m_bDate );
}


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_31,int,31)
void CLASS_ScanViewTimerMessageHandler_31::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( !StageMC600_IsMoving() )
	{
		pScanView->KillTimer(31);
		pScanView->AutoStage_Clear_Finish( TRUE, TRUE );
	}
}


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_37,int,37)
void CLASS_ScanViewTimerMessageHandler_37::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	CString str, strTemp;
	str.Format( "%04d\t", CControlLayer::Instance().m_DL.other.lTimes++ );

	strTemp = pScanView->Diag_Logging2();
	str += strTemp;
	strTemp = pScanView->Diag_SCN_LNS();
	str += strTemp;
	strTemp = pScanView->Diag_VAC();
	str += strTemp;
	strTemp = pScanView->Diag_SPR();
	str += strTemp;
	strTemp = pScanView->Diag_MCI();
	str += strTemp;

	unsigned long cbRet;
	char Buf[4096];
	sprintf( Buf, "%s\n", str );
	WriteFile( CControlLayer::Instance().m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
}



IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_100,int,100)
void CLASS_ScanViewTimerMessageHandler_100::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	switch (pScanView->m_nAutoFuction)
	{
	case	AutoBC:
		pScanView->AutoBCProcess2();
		break;

	case	AutoFocus:
		pScanView->AutoFocusProcess();
		break;

	case	AutoAstigmat:
		pScanView->AutoAstigmatProcess();
		break;

	case	AutoFilament:
		pScanView->AutoFilamentProcess();
		break;
	}
}



IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_131,int,131)
void CLASS_ScanViewTimerMessageHandler_131::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 1 )
		pScanView->AutoStageXY_FI_XQ();
	else if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
		pScanView->AutoStageXY_FI_Process11();
	else
		pScanView->AutoStageXY_FI2_Process11();
}



IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_132,int,132)
void CLASS_ScanViewTimerMessageHandler_132::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
		pScanView->AutoStageXY_FI_Process12();
	else
		pScanView->AutoStageXY_FI2_Process12();
}


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_133,int,133)
void CLASS_ScanViewTimerMessageHandler_133::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
		pScanView->AutoStageXY_FI_Process21();
	else
		pScanView->AutoStageXY_FI2_Process21();
}


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_134,int,134)
void CLASS_ScanViewTimerMessageHandler_134::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
		pScanView->AutoStageXY_FI_Process22();
	else
		pScanView->AutoStageXY_FI2_Process22();
};


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_135,int,135)
void CLASS_ScanViewTimerMessageHandler_135::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
		pScanView->AutoStageXY_FI_Process31();
	else
		pScanView->AutoStageXY_FI2_Process31();
};



IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_136,int,136)
void CLASS_ScanViewTimerMessageHandler_136::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	if( CStageXYManager::Instance().m_Stage.xy.nMtd < 2 )
		pScanView->AutoStageXY_FI_Process32();
	else
		pScanView->AutoStageXY_FI2_Process32();
};



IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_137,int,137)
void CLASS_ScanViewTimerMessageHandler_137::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	pScanView->AutoStageXY_FI_Process4();
};


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_138,int,138)
void CLASS_ScanViewTimerMessageHandler_138::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	pScanView->AutoStageXY_FI_TrackSecurity();
};


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_150,int,150)
void CLASS_ScanViewTimerMessageHandler_150::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;
	pScanView->AutoStage_Clear_XY();
};


extern int g_nMCB_PollCounter;
extern BOOL g_bMCB_Openned;
extern void MCB_CloseHIDDevice();
extern CString g_strMCB_DeviceName;
extern CString MCB_OpenHIDDevice();
extern HANDLE g_hMCB_Read;
extern HANDLE MCB_OpenForRead( CString strFileName );
extern HANDLE g_hMCB_Write;
extern HANDLE MCB_OpenForWrite( CString strFileName );
extern DWORD MCB_StartRead( BYTE* pBuf, int nCount );
extern BYTE g_byteMCB_ReadReport[3];
extern HANDLE g_hMCB_ReadEvent;
extern BYTE g_byteMCB_Port2RBdata;
extern BOOL m_bMCB_ExpectHighByte;
extern BOOL m_bMCB_ExpectLowByte;
extern int MCB_ReceiveCountStamp( BYTE bData );
extern int MCB_ReceiveHighByte( BYTE bData );
extern BYTE g_byteMCB_Port2RBdataBak;
extern BOOL MCB_ReceiveLowByte( BYTE bData );
extern int g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS;


IMPLEMENT_FACTORY(IScanViewTimerMessageHandler,CLASS_ScanViewTimerMessageHandler_121,int,121)
void CLASS_ScanViewTimerMessageHandler_121::HandleMessage(CScanView* pScanView,BOOL& boolBusy)
{
	if(!pScanView) return;

	CString str;

	g_nMCB_PollCounter++;
	if( g_nMCB_PollCounter > 300 )		// (based on 10mS poll_interval setting)
	{
		// I check for the IO device attach or removal every 3 seconds
		// I do this by closing the device and reopenning it
		g_nMCB_PollCounter = 0;
		if( g_bMCB_Openned )
			MCB_CloseHIDDevice();
		g_strMCB_DeviceName = MCB_OpenHIDDevice();
		if( g_strMCB_DeviceName != "" )
		{
			g_hMCB_Read		= MCB_OpenForRead( g_strMCB_DeviceName );
			g_hMCB_Write	= MCB_OpenForWrite( g_strMCB_DeviceName );
			g_bMCB_Openned	= TRUE;
		}
	}	// if( nPollCounter > 300 )

	if( g_hMCB_Read )
	{
		// Start an asynchronous read, only wait up to 80msec
		DWORD dwBytesRead = MCB_StartRead( &g_byteMCB_ReadReport[0], 2 );	// Read = Data from device TO host
		long lResult = WaitForSingleObject( g_hMCB_ReadEvent, 5 );	// 80 = 80mS wait for non-response
		ResetEvent( g_hMCB_ReadEvent );
		if( lResult == WAIT_TIMEOUT )
			lResult = CancelIo( g_hMCB_Read );				// Read was not successful, so cancel the request
		else
		{
			// Read was successful and a change of button state was reported
			g_byteMCB_Port2RBdata = g_byteMCB_ReadReport[1];				// As of 6-21-10, ONLY element 1 has data, which is 2nd byte of 2.

			if( g_byteMCB_Port2RBdata >= 0 )
			{
				// TEMP!!!!  SUSPEND REPORT IF DATA if it's zero
				// Change this condition to something smarter, and segregate status report info vs. encoder+sw data
				// Map data from MCB PLD to corresponding form components:
				// First, identify whether data is status or a change of switch or encoder position(s):

				if( theApp.m_dwOpr > 0 )
				{
					str.Format( "MCB Message: 0x%02X", g_byteMCB_Port2RBdata );
					theApp.DisplayMsg( str );
				}

				if( !m_bMCB_ExpectHighByte && !m_bMCB_ExpectLowByte )
				{
					// Time Stamp ( count stamp )
					int nRet = MCB_ReceiveCountStamp( g_byteMCB_Port2RBdata );
					if( nRet < 0 )
					{
						// not count stamp
						if( theApp.m_dwOpr > 0 )
							theApp.DisplayMsg( "EER: not count stamp" );
						// try high byte
						nRet = MCB_ReceiveHighByte( g_byteMCB_Port2RBdata );
						if( nRet == 0 )
						{
							if( theApp.m_dwOpr > 0 )
							{
								theApp.DisplayMsg( "and not hi, abandon" );
								theApp.DisplayMsg( " " );
							}
							m_bMCB_ExpectHighByte	= FALSE;
							m_bMCB_ExpectLowByte	= FALSE;
							boolBusy = FALSE;
							return;
						}
						if( theApp.m_dwOpr > 0 )
							theApp.DisplayMsg( "EER: it is high" );
					}
					else
						g_byteMCB_Port2RBdataBak = g_byteMCB_Port2RBdata;	// if it is stamp, backup it
				}
				else if( !m_bMCB_ExpectLowByte )
				{
					BOOL bRet = MCB_ReceiveHighByte( g_byteMCB_Port2RBdata );
					if( !bRet )
					{
						if( theApp.m_dwOpr > 0 )
							theApp.DisplayMsg( "EER: not high byte" );
						// not high byte, maybe it is low byte
						// so the backup data maybe high byte
						bRet = MCB_ReceiveHighByte( g_byteMCB_Port2RBdataBak );
						if( bRet )
						{
							if( MCB_ReceiveLowByte( g_byteMCB_Port2RBdata ) )
							{
								CMCBControler::Instance().MCB_Ctrl_Flush_WP( g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS, theApp.m_blUnlock_HV, theApp.m_blOverhaulState );
								if( theApp.m_dwOpr > 0 )
								{
									theApp.DisplayMsg( "EER: the backup is hi, HI & LO are both OK" );
									theApp.DisplayMsg( " " );
								}
							}
							else
							{
								// if the backup data is not high byte, abandon
								if( theApp.m_dwOpr > 0 )
								{
									theApp.DisplayMsg( "EER: Neither backup or now is high byte, abandon" );
									theApp.DisplayMsg( " " );
								}
								m_bMCB_ExpectHighByte	= FALSE;
								m_bMCB_ExpectLowByte	= FALSE;

							}
						}
					}
					boolBusy = FALSE;
					return;
				}	// (If NOT expecting low byte)
				else
				{
					if( MCB_ReceiveLowByte( g_byteMCB_Port2RBdata ) )
					{
						CMCBControler::Instance().MCB_Ctrl_Flush_WP( g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS, theApp.m_blUnlock_HV, theApp.m_blOverhaulState );
						if( theApp.m_dwOpr > 0 )
							theApp.DisplayMsg( " " );
					}
					else
					{
						if( theApp.m_dwOpr > 0 )
						{
							theApp.DisplayMsg( "EER: not low byte, try count stamp" );
							theApp.DisplayMsg( " " );
						}
						MCB_ReceiveCountStamp( g_byteMCB_Port2RBdata );
					}
				}
			}	// End of "If(Port2RBdata >= 0)" condition
		}	// End of "If USB Read Was Successful"
	}
	boolBusy = FALSE;
};
