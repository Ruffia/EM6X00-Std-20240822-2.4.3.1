#pragma once
#include "USB_Functions.h"
#include "MCIO.H"

//修改时间：  2020-09-04
//修改人：  周宏
//修改原因：将原 MCB 控制器相关功能从 ScanView 类中独立出来
//功能： MCB 控制器



class CViewMCBControler  
{
public:
	static CViewMCBControler& Instance();

	CString	MCB_OpenHIDDevice();
	BOOL	MCB_InitHIDDevice( CString strDeviceName );
	void	MCB_CloseHIDDevice();
	DWORD	MCB_StartRead( BYTE* pBuf, int nCount );
	void	MCB_SendReport( BYTE* pBuf, int nCount );
	BOOL	MCB_ReadData( BYTE &bData, BOOL &bLow );
	int		MCB_ReceiveCountStamp( BYTE bData );
	BOOL	MCB_ReceiveHighByte( BYTE bData );
	BOOL	MCB_ReceiveLowByte( BYTE bData );
	BOOL	MCB_CheckHighByte( BYTE bData );

	void    HandleMCBTimerMessage(const DWORD dwOpr, BOOL blUnlock_HV, BOOL blOverhaulState,BOOL& boolBusy);

public:
	CString		g_strMCB_DeviceName;				       // The system name for the remote button device
	BOOL		g_bMCB_Openned;				               // Is the ButtonsAndLights device operational?

	HANDLE		g_hMCB_Write;					           // Needed to write to a system device
	HANDLE		g_hMCB_Read;					           // Needed to read a system device
	HANDLE		g_hMCB_ReadEvent;					       // Needed to do overlapped ( asynchronous ) IO
	OVERLAPPED	g_MCB_overlapRead;

	BYTE		g_byteMCB_ReadReport[3];					// Data read from ButtonsAdnLights device, changed to 8 by DD 6-24-10
	BYTE		g_byteMCB_WriteReport[20];						// Data written to ButtonsAndLights device
	BYTE		g_byteMCB_Port2RBdata, g_byteMCB_Port2RBdataBak;// Comes from ButtonReport data upon detection of Port2 data change

	BOOL		m_bMCB_ExpectLowByte, m_bMCB_ExpectHighByte;// Keeps track of which USB Rx byte is which
	int			g_nMCB_Channel;								// Which channels's data is being sent (Need to decode from data)
	int			g_nMCB_RotDir;								// CCW or CW rotation direction detected?
	int			g_nMCB_SwPos;								// switch position of corresponding channel
	int			g_nMCB_TicksABS, g_nMCB_TicksHi;			// Low 2 bits of High Byte + Low Byte (10 bits total)
	//long		lMCB_USBwriteTotals, lMCB_USBreadTotals;	// For determining whether any Port3 transitions were missed
	int			g_nMCB_PollCounter;							// A local timer
	int			g_nMCB_CountStamp;


protected:
	CViewMCBControler();

};
