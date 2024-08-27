// ScanView.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2011.07.19

/////////////////////////////////////////////////////////////////////////////
// 07.07---07.08 加入任意方向标尺
// 09.11.18 左键已按下时再按下右键，消像散值会使物镜值错乱。OnRButtonDown
// 09.11.18 增加单击测试类型GraphHitTestNULL，
//			将整个扫描区域内外的单击测试类型区别开。GraphHitTest
// 09.11.18 若当前没有被选中的单个项目，且并非处于多文件选择状态，
//			则遍历项目列表中的每一个项目，将所有item都设为非选中显示
// 09.11.18 对OnLButtonDown,OnMouseMove,OnRButtonDown中的
//			m_rcDragRectLast,SetCapture(),SemCtrlMouse_Init的执行条件
//			及m_bCatch的设定做了修正
// 09.11.19 将所有LoadString处均改为GetResString
// 09.12.02 修改OnDraw，确保图像区域以外的部分能够FillRect
// 09.12.04 整理任意方向移动标尺代码
// 09.12.10 增加调试状态功能
// 10.07.02 增加优先存储格式
// 11.07.19 按不同机型设置不同的IoRead循环次数

// 12.02.15 增加单双探测器标识
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "ScanView.h"
#include "ScanChildFrm.h"
#include "MainFrm.h"
#include "SheetSetup.h"

#include "ImageDoc.h"
#include "FilmPhotoData.h"
#include "ImageStandStyle.h"
#include "SEMCTRL_Label.h"
#include "SemCtrl_Mouse.h"
#include "SemCtrlDlgEdit.h"
#include "DlgPrompt.h"
#include "DlgInput.h"

#include "DlgProgress.h"
#include "Algorithm_AutoBC.h"
#include "Algorithm_AutoFocus.h"
#include "Algorithm_AutoAstigmat.h"
#include "Algorithm_AutoFilament.h"
#include <mmsystem.h>		// system timer support

#include "LineTracker.h"	// 07.07---07.08加入任意方向标尺

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//速度指示
#define	STATUS_SCAN_SPEED		0	//扫描速度
#define	STATUS_PHOTO_SPEED		1	//照相速度
//照相相关
#define	CAMERA_SNAP				1	//快照
#define	CAMERA_SIMULATE			2	//模拟照相
#define	CAMERA_DIGITAL			3	//数字照相
#define	CAMERA_PREVIEW			4	//照相预览
#define	CAMERA_SERNUM			5	//照相编号

#define CX_BORDER   1
#define CY_BORDER   1

extern BYTE g_cFilmPhotoData[];

///////////////////////////////////////////////////
// ZOOM
///////////////////////////////////////////////////
/*
//const int g_nZoomTable[]	= {25,50,75,100,200,400};
const int g_nZoomTable[]	= {25,50,75,100};
const int g_nZoomNormal		= 3;
const int g_nZoomMax		= sizeof(g_nZoomTable) / sizeof(int) - 1;
const int g_nZoomMin		= 0;
*/
double	g_dScaleScanTable[]			= { 0.1, 0.2, 0.25, 0.50, 0.75, 1.0, 1.5, 2, 2.5, 3, 3.5, 4 };
int		g_nScaleScanIndexMax		= sizeof(g_dScaleScanTable) / sizeof(double) - 1;
int		g_nScaleScanIndexMin		= 0;
int		g_nScaleScanIndexFit		= 5;
int		g_nZoomNormal				= 5;

int		g_nScaleScanIndex			= g_nZoomNormal;
///////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 2005-1-17_液晶屏幕XY比例问题
const COLORREF gc_crScreenColor = RGB(0, 0, 0);

const long HandleAreaW = 16;
const long HandleAreaH = 12;
const long HandleAreaW2 = 20;
const long HandleAreaH2 = 20;

// 09.11.18增加单击测试类型GraphHitTestNULL，总类型由11项变为12项
HCURSOR g_hCursors[12];
extern BYTE g_cFilmPhotoData[];

// Special mapping modes just for CScanView implementation
#define MM_NONE             0
#define MM_SCALETOFIT       (-1)
// standard GDI mapping modes are > 0

//--Auto Functions---------------------------------------//
const UINT nAutoFunctionTimerID			= 100;
const UINT nAutoBCTimerElapse			= 500;

const UINT nAutoFocusTimerElapse		= 700; // ms
const UINT nAFTotalTime					= 45; // s

const UINT nAutoAstigmatTimerElapse		= 500; // ms
const UINT nASTotalTime					= 45;  // s
//--Auto Functions---------------------------------------//
//090327 ADD ZYX
#define DEBUG_Info_Sharpness
#define DEBUG_Info_ABC

const UINT nAutoTimerDate				= 110;

//CScanView* gpScanView = NULL;

CSize GetImageSize()
{
	CSize szImage = theApp.m_ImageCard.GetResolution();
/*	
	switch ( theApp.m_ImageCard.GetResolution() )
	{
// 	case	RESO_512_384:
// 		szImage = g_sizeRESO_512_384;
// 		break;
// 	case	RESO_1024_768:
// 		szImage = g_sizeRESO_1024_768;
// 		break;
// 	case	RESO_2048_1536:
// 		szImage = g_sizeRESO_2048_1536;
// 		break;	

	case	USB_RESO_16_16:		// 16
		szImage = g_sizeUSB_RESO_512_512;
		break;
	case	USB_RESO_32_32:		// 32
		szImage = g_sizeUSB_RESO_512_512;
		break;
	case	USB_RESO_64_64:		// 64
		szImage = g_sizeUSB_RESO_512_512;
		break;
	case	USB_RESO_128_128:	// 128
		szImage = g_sizeUSB_RESO_512_512;
		break;
	case	USB_RESO_256_256:	// 256
		szImage = g_sizeUSB_RESO_512_512;
		break;
	case	USB_RESO_512_512:	// 512
		szImage = g_sizeUSB_RESO_512_512;
		break;
	case	USB_RESO_1024_1024:	// 1024
		szImage = g_sizeUSB_RESO_1024_1024;
		break;
	case	USB_RESO_2048_2048:	// 2048
		szImage = g_sizeUSB_RESO_2048_2048;
		break;
	case	USB_RESO_4096_4096:	// 4096
		szImage = g_sizeUSB_RESO_4096_4096;
		break;
	case	USB_RESO_8192_8192:	// 8192
		szImage = g_sizeUSB_RESO_8192_8192;
	default:
		szImage = g_sizeUSB_RESO_1024_1024;
		break;
	}
*/
	return szImage;
}







/////////////////////////////////////////////////////////////////////////////
// MCB Start
/////////////////////////////////////////////////////////////////////////////

extern	CString	MCB_FindHIDInterface( int nVID, int nPID, int nUsagePage );
extern	HANDLE	MCB_OpenForRead( CString strFileName );
extern	HANDLE	MCB_OpenForWrite( CString strFileName );
extern	void	MCB_ErrorExit( CString strReason );

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

CString		g_strMCB_DeviceName	= _T("");				// The system name for the remote button device
BOOL		g_bMCB_Openned		= FALSE;				// Is the ButtonsAndLights device operational?

HANDLE		g_hMCB_Write		= NULL;					// Needed to write to a system device
HANDLE		g_hMCB_Read			= NULL;					// Needed to read a system device
HANDLE		g_hMCB_ReadEvent	= NULL;					// Needed to do overlapped ( asynchronous ) IO
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



CString MCB_OpenHIDDevice()
{
	// I know this is a HID device with a specific VID and PID.
	// I also search for the UsagePage that defines the data reports
	// I return the system name for the device or NULL if the device is not plugged in
	CString strSystemName = _T("");
	// check for examples 1 and 4
// 	strSystemName = MCB_FindHIDInterface( 0x4242, 0xEE01, 0xFF00 );
// 	if( strSystemName == "" )
// 		strSystemName = MCB_FindHIDInterface( 0x4242, 0xEE04, 0xFF00 );		// Set in USB_descr.asm
	strSystemName = MCB_FindHIDInterface( 0x04D8, 0x003F, 0xFF00 );
	if( strSystemName == "" )
		strSystemName = MCB_FindHIDInterface( 0x04D8, 0x003F, 0xFF00 );		// Set in USB_descr.asm

	return strSystemName;
}

BOOL MCB_InitHIDDevice( CString strDeviceName )
{
	///////////////////////////////////////////////////////////////////////////
	// I do asyncnronous reads so I need an object to wait on
	g_hMCB_ReadEvent = CreateEvent(0, FALSE, FALSE, 0);
	if( g_hMCB_ReadEvent == NULL )
		MCB_ErrorExit( "Could not create ReadEvent" );

	g_MCB_overlapRead.Offset		= 0;
	g_MCB_overlapRead.OffsetHigh	= 0;
	g_MCB_overlapRead.hEvent		= g_hMCB_ReadEvent;
	///////////////////////////////////////////////////////////////////////////

	g_nMCB_PollCounter				= 100;	// Force an initial search for the IO device
	// 设置时间戳初始值
	g_nMCB_CountStamp				= 0;

    if (strDeviceName == "")
        return FALSE;
    else
	{
		g_hMCB_Read		= MCB_OpenForRead(strDeviceName);
		g_hMCB_Write	= MCB_OpenForWrite(strDeviceName);
		g_bMCB_Openned	= TRUE;
	}

	// 接收数据缓冲区清零
	memset( g_byteMCB_ReadReport, 0, sizeof(g_byteMCB_ReadReport) );

	// 最先接收时间戳，设置标志位
	m_bMCB_ExpectLowByte	= FALSE;
	m_bMCB_ExpectHighByte	= FALSE;

	return TRUE;
}

void MCB_CloseHIDDevice()
{
	if( g_hMCB_Read != NULL )
	{
		CloseHandle( g_hMCB_Read );
		g_hMCB_Read = NULL;
	}
	if( g_hMCB_Write != NULL )
	{
		CloseHandle( g_hMCB_Write );
		g_hMCB_Write = NULL;
	}

	ResetEvent( g_hMCB_ReadEvent );

	g_bMCB_Openned = FALSE;
}

DWORD MCB_StartRead( BYTE* pBuf, int nCount )
{
	// Also called by (n)mS polling loop (via timer)
	// Note that this HID report has a 0 byte as the first byte that is not sent on the bus
	// Windows uses this as a ReportID
	// 在每次读/写数据前都要先接收/发送1字节的PID标志，所以每次读/写数据的时候都要多一个字节。
	// 例如：每次读/写的是64字节数据，但是在这64字节之前必须放1字节的PID数据，所以就是65字节。
	// 一般这个PID字节的值为0.
	DWORD dwBytesRead = 0;
	// Data arrives TO HOST via USB HERE
	BOOL bSuccess = ReadFile( g_hMCB_Read, pBuf, nCount, &dwBytesRead, &g_MCB_overlapRead );

	return dwBytesRead;
}

void MCB_SendReport( BYTE* pBuf, int nCount )
{
	// Note that this HID report has a 0 byte as the first byte that is not sent on the bus
	// Windows uses this as a ReportID
	DWORD dwBytesWritten;
	// Rubber meets the road HERE
	BOOL bSuccess = WriteFile( g_hMCB_Write, pBuf, nCount, &dwBytesWritten, NULL );
	if( !bSuccess || (dwBytesWritten != (DWORD)nCount) )
		MCB_ErrorExit( "Error in SendReport" );
}

int MCB_ReceiveCountStamp( BYTE bData )
{
	int nRet = 1;
	if( (bData & 0x01) != 0x01 )
	{
		nRet = -1;
		return nRet;
	}

	int nCountStamp = bData >> 1;		// 右移1位
	if( nCountStamp != g_nMCB_CountStamp )
	{
		g_nMCB_CountStamp = nCountStamp;
		nRet = 0;
	}

	g_nMCB_CountStamp += 1;
	if( g_nMCB_CountStamp == 0x80 )
		g_nMCB_CountStamp = 0;

	m_bMCB_ExpectHighByte = TRUE;
	return nRet;
}

BOOL MCB_CheckHighByte( BYTE bData )
{
	BOOL bRet = FALSE;

/*	switch( bData )
	{
	case	0x04:
	case	0x07:
	case	0x14:
	case	0x17:
	case	0x28:
	case	0x2B:
	case	0x34:
	case	0x37:
	case	0x44:
	case	0x47:
	case	0x58:
	case	0x5B:
	case	0x68:
	case	0x6B:
	case	0x78:
	case	0x7B:
	case	0x84:
	case	0x87:
		bRet = TRUE;
		break;
	}
*/
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

BOOL MCB_ReceiveHighByte( BYTE bData )
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

	g_nMCB_SwPos = (bData & 0x0C) /4;
	if( (g_nMCB_SwPos == 1) || (g_nMCB_SwPos == 2) )	// 1 means fB, 2 means fA
		bSwPosGood = TRUE;
	else
		bSwPosGood = FALSE;

	g_nMCB_TicksHi = ( bData & 0x03 );
	if( (g_nMCB_TicksHi == 3) || (g_nMCB_TicksHi == 0) )
		bRotationGood = TRUE;
	else
		bRotationGood = FALSE;

	if( bChanGood && bSwPosGood && bRotationGood )
		bHiByteGood = TRUE;
	else
		bHiByteGood = FALSE;

	if( bHiByteGood )
	{
		g_nMCB_Channel = nChanNow;

		m_bMCB_ExpectLowByte	= TRUE;		// Finished processed what appeared to be a high byte, so expect low byte next
		m_bMCB_ExpectHighByte	= FALSE;
	}

	return bHiByteGood;
}

BOOL MCB_ReceiveLowByte( BYTE bData )
{
	BOOL bRet = TRUE;
	// Data byte is LOW, so append 8 bit TicksLo to 2 bit TicksHi value for new encoder position
	int	nMCB_TicksLo	= bData;
	int	nMCB_Ticks		= g_nMCB_TicksHi *256 +nMCB_TicksLo;
	if( nMCB_Ticks > 0x39F )	// > 0x39F, so CCW
	{
		g_nMCB_TicksABS	= 1024 - nMCB_Ticks;
		g_nMCB_RotDir	= -1;
	}
	else						// CW Rotation
	{
		g_nMCB_TicksABS	= nMCB_Ticks;
		g_nMCB_RotDir	= 1;
	}
	if( g_nMCB_TicksABS > 0x40 )		// so big value
		bRet = FALSE;

	m_bMCB_ExpectLowByte	= FALSE;		// Since low byte was just processed, expect count stamp next //high byte next
	m_bMCB_ExpectHighByte	= FALSE;

	return bRet;
}
/////////////////////////////////////////////////////////////////////////////
// MCB End
/////////////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////////
// CScanView

IMPLEMENT_DYNCREATE(CScanView, CScrollView)

CScanView::CScanView()
{
//LMH//////////////////////////////////
	// Init everything to zero
	AFX_ZERO_INIT_OBJECT(CWnd);
	m_nMapMode = MM_NONE;

//	SetScrollSizes(MM_TEXT, CSize(0, 0));
	// 2005-1-17_液晶屏幕XY比例问题
	VERIFY(m_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));	// 09.12.02改回diagcross
//	VERIFY(m_brHatch.CreateSolidBrush( gc_crScreenColor ));
	m_bmpBackground.LoadBitmap( MAKEINTRESOURCE(IDB_6K_BITMAP_Wallpaper) );

	//////////////////////////////////////////////////////
	g_hCursors[0] = LoadCursor(NULL, IDC_ARROW);
	g_hCursors[1] = LoadCursor(NULL, IDC_SIZEALL);
	g_hCursors[2] = LoadCursor(NULL, IDC_SIZEWE);
	g_hCursors[3] = LoadCursor(NULL, IDC_SIZEWE);
	g_hCursors[4] = LoadCursor(NULL, IDC_SIZENS);
	g_hCursors[5] = LoadCursor(NULL, IDC_SIZENS);
	g_hCursors[6] = LoadCursor(NULL, IDC_SIZENESW);
	g_hCursors[7] = LoadCursor(NULL, IDC_SIZENWSE);
	g_hCursors[8] = LoadCursor(NULL, IDC_SIZENESW);
	g_hCursors[9] = LoadCursor(NULL, IDC_SIZENWSE);
	g_hCursors[10] = LoadCursor(NULL, IDC_SIZENS);
	// 09.11.18增加单击测试类型GraphHitTestNULL
	g_hCursors[11] = LoadCursor(NULL, IDC_ARROW);
	//////////////////////////////////////////////////////

	m_bCatch = FALSE;

	m_bUScaleHitTest		= FALSE;
	m_bOscillographHitTest	= FALSE;
//LMH//////////////////////////////////

	m_nAdjustorSort			= SORT_OBJECTIVE_LENS;	//调节器类型为对比度调节
	m_nCondenserSort		= 0;					//聚光镜调节类型为粗调
	m_nObjectiveSort		= 0;					//物镜调节类型为粗调

	m_bAbc					= FALSE;	//不进行字符编辑
	m_bLabel				= TRUE;		//显示底标
	m_bUScale				= FALSE;	//隐藏活动标尺
	m_bDate					= FALSE;	//隐藏日期
	m_bNote					= FALSE;	//隐藏备注
	m_bWobbler				= FALSE;	//物镜摇摆关
	m_bLean					= FALSE;	//倾斜补偿关
	m_bDynFocus				= FALSE;	//动态聚焦关
	m_bSacp					= FALSE;	//通道花样关
	m_bRobinson				= FALSE;	//Robinson探头关
	m_bOscillograph			= FALSE;	//隐藏示波器
	m_bFrameRate			= FALSE;	//隐藏标准框

	m_bCondRev				= FALSE;	//聚光镜反转关
	m_bObjectiveRev			= FALSE;	//物镜反转关
	m_bAuxStig				= FALSE;	//辅助消像散关

	m_bPreview				= FALSE;	//非照相预览
	m_bZoomFit				= FALSE;
	m_bLive					= FALSE;	//快速扫描
	m_bSplit				= FALSE;	//非分割像
	m_bSplitZoom			= FALSE;	//分割像不放大显示
	m_bOnlyDet2				= FALSE;	//显示第一探测器图像

	m_bOpenImageAfterSnap	= FALSE;	//快照后不打开图像窗口

///////////////////////////////////////////////////////////
	m_boolScreenFull		= FALSE;

	m_bStopPhoto			= FALSE;	// 2004-5-12_照相编号自动增加
	m_bStopScan				= TRUE;		// 2005-1-17_0.0KV问题

	///////////////////////////////////////////////////////////
	m_usbFunction		= USB_SF_RasterScan;
	m_nusbScanResoIndex	= 4;
	m_nusbPixRate		= 2;					// 1us
	m_nusbPixRateIndex	= 1;					// 扫描速度为1档
	m_nusbPhotoPixRate	= 128;					// 64us
	m_nusbPhotoPixRateIndex	= 4;//11;			// pix clk = 20us, actual dwell time = 80us
//	m_usbParam			= ;
	m_nusbScanStoreMode	= 1;					// XYZ存储方式
	m_nusbScanDetMode	= 0;					// 12.02.15 单探测器模式
	m_nCLgrade			= -1;
	///////////////////////////////////////////////////////////

//	USB_GetResoSize( m_nusbScanResoIndex );
	m_dResoRate = 1.0;
	m_sizeReso = CSize(1024,768);

	///////////////////////////////////////////////////////////
	m_nLineScanParam = m_sizeReso.cy / 2;
	
	m_ptPointScanParam.x = m_sizeReso.cx / 2;
	m_ptPointScanParam.y = m_sizeReso.cy / 2;

	//	1024	512		256
	//	 768	384		192
	m_rcAreaScanParam.left		= 256;
	m_rcAreaScanParam.top		= 192;
	m_rcAreaScanParam.right		= 256 + 512;
	m_rcAreaScanParam.bottom	= 192 + 384;

	//	1024	512		256
	//	 768	384		192
	m_rcStaticAreaScanParam.left	= 256;
	m_rcStaticAreaScanParam.top		= 192;
	m_rcStaticAreaScanParam.right	= 256 + 512;
	m_rcStaticAreaScanParam.bottom	= 192 + 384;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoBCAreaScanParam.left	= (1024 - 512) / 2;
	m_rcAutoBCAreaScanParam.top		= (768 - 384) / 2;
	m_rcAutoBCAreaScanParam.right	= 512 + m_rcAutoBCAreaScanParam.left;
	m_rcAutoBCAreaScanParam.bottom	= 384 + m_rcAutoBCAreaScanParam.top;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoFocusAreaScanParam.left		= (1024 - 256) / 2;
	m_rcAutoFocusAreaScanParam.top		= (768 - 256) / 2;
	m_rcAutoFocusAreaScanParam.right	= 256 + m_rcAutoFocusAreaScanParam.left;
	m_rcAutoFocusAreaScanParam.bottom	= 256 + m_rcAutoFocusAreaScanParam.top;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoAstigmatAreaScanParam.left		= (1024 - 256) / 2;
	m_rcAutoAstigmatAreaScanParam.top		= (768 - 256) / 2;
	m_rcAutoAstigmatAreaScanParam.right		= 256 + m_rcAutoAstigmatAreaScanParam.left;
	m_rcAutoAstigmatAreaScanParam.bottom	= 256 + m_rcAutoAstigmatAreaScanParam.top;

	//	1024	512		256
	//	 768	384		192
	m_rcDualMagParam.left	= 128;
	m_rcDualMagParam.top	= 192;
	m_rcDualMagParam.right	= 128 + 256;
	m_rcDualMagParam.bottom	= 192 + 384;
	///////////////////////////////////////////////////////////

//--Auto Functions---------------------------------------//
	m_nAutoFunctionTimer	= 0;
	m_pAutoProgressDlg		= NULL;
	m_bImageBuff			= new BYTE[256*256];//bAutoBUFF;
//--Auto Functions---------------------------------------//

//	gpScanView = this;

	// 07.04.26显示图像小样
	m_pSelItem			= NULL;
	m_plistImageItem	= new ListImageItem;
	m_bCtrlDown			= FALSE;				//设置Ctrl键按下标志08.06.27
	m_bIsMultiSel		= FALSE;				//设置多文件选择标志08.06.27

	// 07.07---07.08加入任意方向标尺
	m_ptMobileUScale	= CPoint(100,400);
	m_ptMobileUScale2	= CPoint(400,400);

	// 16.06.12 多移动标尺实验
	for( int i=0; i<MobileUScaleCounts; i++ )
	{
//		m_nMobileUScalePixelNumArray[i]	= ;
		m_ptMobileUScaleArray[i]		= CPoint( -1, -1 );
		m_ptMobileUScaleArray2[i]		= CPoint( -1, -1 );
		m_bUScaleHitTestArray[i]		= FALSE;
	}
	m_nMobileUScaleCounts	= 0;
	// 16.06.12 多移动标尺实验

	m_bDebugAccel		= FALSE;				// 09.12.10 增加调试状态功能

	USB_SetScanRectParam();

	// 12.06.21 自动台
	m_bStageOrigin			= FALSE;
	m_bStageOriginHitTest	= FALSE;
	m_bStageTrack			= FALSE;
	m_bShiftDown			= FALSE;
	m_ptStagePointParam.x	= m_sizeReso.cx / 2;
	m_ptStagePointParam.y	= m_sizeReso.cy / 2;
	m_ptStageTrackParam.x	= m_sizeReso.cx / 2;
	m_ptStageTrackParam.y	= m_sizeReso.cy / 2;

	// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
	m_bDebugCoadjust		= FALSE;
	m_bDrawCircle			= FALSE;

	///////////////////////////////////
	// 15.09 远程功能
	m_commCmdThd		= NULL;
	m_commSendImageThd	= NULL;
	m_pcommServer		= NULL;
	m_pcommSendImgData	= NULL;
	m_bCommListenStarted= FALSE;
	m_bCommSendImgData	= FALSE;
	///////////////////////////////////
}

void CScanView::USB_GetResoSize( int nScanResolution )
{
	m_sizeReso.cx = m_nResos[m_nResoX[m_nusbScanResoIndex]];
	if( m_dResoRate < 0 )
		m_sizeReso.cy = m_nResos[m_nResoY[m_nusbScanResoIndex]];
	else
		m_sizeReso.cy = (long)(m_nResos[m_nResoX[m_nusbScanResoIndex]] *m_dResoRate);
}

void CScanView::USB_SetScanRectParam()
{
	///////////////////////////////////////////////////////////
	// usb param
	m_usbParam.RasterScan.dwStartX	= 0;
	m_usbParam.RasterScan.dwStartY	= 0;
	m_usbParam.RasterScan.dwStopX	= m_sizeReso.cx;
	m_usbParam.RasterScan.dwStopY	= m_sizeReso.cy;
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	m_nLineScanParam = m_sizeReso.cy / 2;
	
	m_ptPointScanParam.x = m_sizeReso.cx / 2;
	m_ptPointScanParam.y = m_sizeReso.cy / 2;

	//	1024	512		256
	//	 768	384		192
	m_rcAreaScanParam.left		= m_sizeReso.cx * 5 / 16;
	m_rcAreaScanParam.top		= m_sizeReso.cy * 5 / 16;
	m_rcAreaScanParam.right		= m_sizeReso.cx * 11 / 16;
	m_rcAreaScanParam.bottom	= m_sizeReso.cy * 11 / 16;

	//	1024	512		256
	//	 768	384		192
	m_rcStaticAreaScanParam.left	= m_sizeReso.cx /2 -128;
	if( m_rcStaticAreaScanParam.left < 0 )
		m_rcStaticAreaScanParam.left = 0;
	m_rcStaticAreaScanParam.top		= m_sizeReso.cy /2 -128;
	if( m_rcStaticAreaScanParam.top < 0 )
		m_rcStaticAreaScanParam.top = 0;
	m_rcStaticAreaScanParam.right	= m_sizeReso.cx /2 +128;
	if( m_rcStaticAreaScanParam.right > m_sizeReso.cx )
		m_rcStaticAreaScanParam.right = m_sizeReso.cx;
	m_rcStaticAreaScanParam.bottom	= m_sizeReso.cy /2 +128;
	if( m_rcStaticAreaScanParam.bottom > m_sizeReso.cy )
		m_rcStaticAreaScanParam.bottom = m_sizeReso.cy;
/*
	//	1024	512		256
	//	 768	384		192
	m_rcAutoBCAreaScanParam.left	= m_sizeReso.cx / 4;
	m_rcAutoBCAreaScanParam.top		= m_sizeReso.cy / 4;
	m_rcAutoBCAreaScanParam.right	= m_sizeReso.cx * 3 / 4;
	m_rcAutoBCAreaScanParam.bottom	= m_sizeReso.cy * 3 / 4;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoFocusAreaScanParam.left		= m_sizeReso.cx / 4;
	m_rcAutoFocusAreaScanParam.top		= m_sizeReso.cy / 4;
	m_rcAutoFocusAreaScanParam.right	= m_sizeReso.cx * 3 / 4;
	m_rcAutoFocusAreaScanParam.bottom	= m_sizeReso.cy * 3 / 4;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoAstigmatAreaScanParam.left		= m_sizeReso.cx / 4;
	m_rcAutoAstigmatAreaScanParam.top		= m_sizeReso.cy / 4;
	m_rcAutoAstigmatAreaScanParam.right		= m_sizeReso.cx * 3 / 4;
	m_rcAutoAstigmatAreaScanParam.bottom	= m_sizeReso.cy * 3 / 4;
*/
	m_rcAutoBCAreaScanParam			= m_rcStaticAreaScanParam;
	m_rcAutoFocusAreaScanParam		= m_rcStaticAreaScanParam;
	m_rcAutoAstigmatAreaScanParam	= m_rcStaticAreaScanParam;

	//	1024	512		256
	//	 768	384		192
	m_rcDualMagParam.left	= m_sizeReso.cx / 8;
	m_rcDualMagParam.top	= m_sizeReso.cy / 4;
	m_rcDualMagParam.right	= m_sizeReso.cx * 3 / 8;
	m_rcDualMagParam.bottom	= m_sizeReso.cy * 3 / 4;
	///////////////////////////////////////////////////////////
}

CScanView::~CScanView()
{
//LMH//////////////////////////////////
	m_brHatch.DeleteObject();	// 2005-1-17_液晶屏幕XY比例问题
//LMH//////////////////////////////////

//--Auto Functions---------------------------------------//
	delete [] m_bImageBuff;

	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer	= 0;
	}
//--Auto Functions---------------------------------------//

	//07.04.26显示图像小样
	POSITION pos;
	while( (pos=m_plistImageItem->GetHeadPosition()) )
	{
		CImageItem* pItem = m_plistImageItem->GetAt( pos );
		m_plistImageItem->RemoveHead();
		delete pItem;
	}
	delete m_plistImageItem;
}


BEGIN_MESSAGE_MAP(CScanView, CScrollView)
	//{{AFX_MSG_MAP(CScanView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ADJUSTOR_CONDENSER_LENS_REVERSE, OnAdjustorCondenserLensReverse)
	ON_UPDATE_COMMAND_UI(ID_ADJUSTOR_CONDENSER_LENS_REVERSE, OnUpdateAdjustorCondenserLensReverse)
	ON_COMMAND(ID_ADJUSTOR_OBJECTIVE_LENS_REVERSE, OnAdjustorObjectiveLensReverse)
	ON_UPDATE_COMMAND_UI(ID_ADJUSTOR_OBJECTIVE_LENS_REVERSE, OnUpdateAdjustorObjectiveLensReverse)
	ON_COMMAND(ID_ADJUSTOR_STIGMATOR_ASSISTANT, OnStigmatorAssistant)
	ON_UPDATE_COMMAND_UI(ID_ADJUSTOR_STIGMATOR_ASSISTANT, OnUpdateStigmatorAssistant)
	ON_COMMAND(ID_PHOTO_SNAPSHOT, OnPhotoSnapshot)
	ON_COMMAND(ID_PHOTO_PREVIEW, OnPhotoPreview)
	ON_COMMAND(ID_AUTO_BRIGHT_CONTRAST, OnAutoBrightContrast)
	ON_COMMAND(ID_AUTO_FOCUS, OnAutoFocus)
	ON_COMMAND(ID_AUTO_STIGMATOR, OnAutoStigmator)
	ON_COMMAND(ID_AUTO_FILAMENT, OnAutoFilament)
	ON_COMMAND(ID_OTHERS_ABC, OnAbc)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_ABC, OnUpdateAbc)
	ON_COMMAND(ID_OTHERS_LABEL, OnLabel)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_LABEL, OnUpdateLabel)
	ON_COMMAND(ID_OTHERS_RULER, OnRuler)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_RULER, OnUpdateRuler)
	ON_COMMAND(ID_OTHERS_DATE, OnDate)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_DATE, OnUpdateDate)
	ON_COMMAND(ID_OTHERS_NOTE, OnNote)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_NOTE, OnUpdateNote)
	ON_COMMAND(ID_ACCESSORIES_VIDEO_PRO, OnVideoPro)
	ON_COMMAND(ID_ACCESSORIES_BSE_DET, OnRobinsonDetector)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_BSE_DET, OnUpdateRobinsonDetector)
	ON_COMMAND(ID_ACCESSORIES_WOBBLER, OnWobbler)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_WOBBLER, OnUpdateWobbler)
	ON_COMMAND(ID_ACCESSORIES_SACP, OnSACP)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_SACP, OnUpdateSACP)
	ON_COMMAND(ID_ACCESSORIES_LEAN_COMPENSATE, OnLeanCompensate)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_LEAN_COMPENSATE, OnUpdateLeanCompensate)
	ON_COMMAND(ID_ACCESSORIES_DYNAMIC_FOCUS, OnDynamicFocus)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_DYNAMIC_FOCUS, OnUpdateDynamicFocus)
	ON_COMMAND(ID_ACCESSORIES_OSCILLOGRAPH, OnOscillograph)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_OSCILLOGRAPH, OnUpdateOscillograph)
	ON_COMMAND(ID_ACCESSORIES_STAND_IMAGE, OnStandImage)
	ON_COMMAND(ID_OPTIONS_SETUP, OnOptionsSetup)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewZoomOut)
	ON_COMMAND(ID_VIEW_ACTUAL_SIZE_SCAN, OnViewActualSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTUAL_SIZE_SCAN, OnUpdateViewActualSize)
	ON_COMMAND(ID_VIEW_ZOOM_FIT, OnViewZoomFit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_FIT, OnUpdateViewZoomFit)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_PHOTO_PREVIEW, OnUpdatePhotoPreview)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_ACCESSORIES_FRAME_RATE, OnFrameRate)
	ON_UPDATE_COMMAND_UI(ID_ACCESSORIES_FRAME_RATE, OnUpdateFrameRate)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_CHANGESTATE_DELETE, OnChangestateDelete)
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDC_BUTTON_Diagnose, OnBUTTONDiagnose)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_Diagnose, OnUpdateBUTTONDiagnose)
	ON_COMMAND(IDC_BUTTON_Diagnose_Setup, OnBUTTONDiagnoseSetup)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_Diagnose_Setup, OnUpdateBUTTONDiagnoseSetup)
	ON_WM_SETFOCUS()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_SCAN_TYPE_SPLIT, OnScanTypeSplit)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_SPLIT, OnUpdateScanTypeSplit)
	ON_COMMAND(ID_SCAN_TYPE_SPLIT_ZOOM, OnScanTypeSplitZoom)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_SPLIT_ZOOM, OnUpdateScanTypeSplitZoom)
	ON_COMMAND(ID_SCAN_TYPE_DET2, OnScanTypeDet2)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_DET2, OnUpdateScanTypeDet2)
	ON_COMMAND(ID_Stage_Coordinate_origin, OnStageCoordinateorigin)
	ON_UPDATE_COMMAND_UI(ID_Stage_Coordinate_origin, OnUpdateStageCoordinateorigin)
	ON_COMMAND(ID_OPTIONS_ListenStart, OnOptionsListenStart)
	ON_COMMAND(ID_OPTIONS_ListenStop, OnOptionsListenStop)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ListenStart, OnUpdateOptionsListenStart)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ListenStop, OnUpdateOptionsListenStop)
	ON_COMMAND(ID_SCAN_TYPE_LIVE, OnScanTypeLive)
	ON_UPDATE_COMMAND_UI(ID_SCAN_TYPE_LIVE, OnUpdateScanTypeLive)
	ON_COMMAND(ID_OTHERS_RULER_Add, OnRulerAdd)
	ON_UPDATE_COMMAND_UI(ID_OTHERS_RULER_Add, OnUpdateRulerAdd)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_ADJUSTOR_BEAM_ALIGNMENT, ID_ADJUSTOR_STIGMATOR, OnAdjustorSortChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ADJUSTOR_BEAM_ALIGNMENT, ID_ADJUSTOR_STIGMATOR, OnUpdateAdjustorSortChanged)
	ON_COMMAND_RANGE(ID_ADJUSTOR_MANUAL_ObjectiveLens, ID_ADJUSTOR_MANUAL_NULL, OnAdjustorManualSortChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ADJUSTOR_MANUAL_ObjectiveLens, ID_ADJUSTOR_MANUAL_NULL, OnUpdateAdjustorManualSortChanged)
	ON_COMMAND_RANGE(ID_SCAN_TYPE_FREEZE, ID_SCAN_TYPE_CAMERA_SIMULATE, OnScanTypeChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_TYPE_FREEZE, ID_SCAN_TYPE_CAMERA_SIMULATE, OnUpdateScanTypeChanged)
	ON_COMMAND_RANGE(ID_SCAN_SPEED_0, ID_SCAN_SPEED_9, OnScanSpeedChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_SPEED_0, ID_SCAN_SPEED_9, OnUpdateScanSpeedChanged)
	ON_COMMAND_RANGE(ID_SCAN_RESOLUTION_1, ID_SCAN_RESOLUTION_9, OnScanResolutionChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_RESOLUTION_1, ID_SCAN_RESOLUTION_9, OnUpdateScanResolutionChanged)
	ON_COMMAND_RANGE(ID_SCAN_STORE_ZZZ, ID_SCAN_STORE_XYZ, OnScanStoreModeChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCAN_STORE_ZZZ, ID_SCAN_STORE_XYZ, OnUpdateScanStoreModeChanged)
	ON_COMMAND_RANGE(ID_Condenser_Fast_1, ID_Condenser_Fast_9, OnCondenserFastChanged)
	ON_UPDATE_COMMAND_UI_RANGE(ID_Condenser_Fast_1, ID_Condenser_Fast_5, OnUpdateCondenserFastChanged)

	ON_MESSAGE(WM_PARAM_CHANGED, OnParamChanged)
	ON_MESSAGE(WM_USER_VAC_MSG, OnSMvacChanged)
	ON_MESSAGE(WM_USER_KYKYComm_Recv, OnKYKYComm_ReplyAsk)
	ON_MESSAGE(WM_USER_KYKYComm_Thread_Send, OnKYKYComm_Connect)
	ON_MESSAGE(WM_USER_KYKYComm_Thread_Send_Connect, OnKYKYComm_ConnectThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanView drawing

void CScanView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();

	//2009.12.02修改两个CRect的边界值取值方法，确保图像区域以外的部分能够FillRect
	// 本次修改前代码副本
	/*------------------------------------------------- 
	if ( m_szTotalLog.cx != 0  )
	{
		CRect rcBounds;
		GetClientRect( (LPRECT)rcBounds );
//		CSize szTotalSize = GetTotalSize();

		CRect rc_VERT;
		CRect rc_HORZ;

		rc_HORZ.left	= rcBounds.left;
		rc_HORZ.right	= rcBounds.right;
		rc_HORZ.bottom	= rcBounds.bottom;
		rc_HORZ.top		=
			(m_szTotalLog.cy < rcBounds.bottom) ? m_szTotalLog.cy : rcBounds.bottom;

		rc_VERT.top		= rcBounds.top;
		rc_VERT.bottom	= rcBounds.bottom;
		rc_VERT.right	= rcBounds.right;
		rc_VERT.left	=
			(m_szTotalLog.cx < rcBounds.right) ? m_szTotalLog.cx : rcBounds.right;

		if ( !rc_HORZ.IsRectEmpty() )
			pDC->FillRect(rc_HORZ, &m_brHatch);		// 2005-1-17_液晶屏幕XY比例问题
	//		CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)) );

		if ( !rc_VERT.IsRectEmpty() )
			pDC->FillRect(rc_VERT, &m_brHatch);		// 2005-1-17_液晶屏幕XY比例问题
	//		CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)) );
	}

	CRect rect, rcClient;
	GetClientRect( &rect );
	//填充背景图片
	CBrush brush;
//	brush.CreatePatternBrush( &m_bmpBackground );
	brush.CreateHatchBrush( HS_DIAGCROSS, RGB(191, 191, 191) );
	CBrush* pOldbrush = pDC->SelectObject( &brush );
	pDC->FillRect( rect, &brush );
	pDC->SelectObject( pOldbrush );
	-------------------------------------------------*/

	// 本次修改后代码
	//-------------------------------------------------
	CRect rcBounds;
	GetClientRect( (LPRECT)rcBounds );

/*	if ( m_szTotalLog.cx != 0  )
	{
		CRect rc_VERT;
		CRect rc_HORZ;

		rc_HORZ.left	= rcBounds.left;
		rc_HORZ.right	= rcBounds.right;
		rc_HORZ.top		=
			(m_szTotalLog.cy < rcBounds.bottom) ? m_szTotalLog.cy : rcBounds.bottom;
		rc_HORZ.bottom	=
			(m_szTotalLog.cy > rcBounds.bottom) ? m_szTotalLog.cy : rcBounds.bottom;

		rc_VERT.top		= rcBounds.top;
		rc_VERT.bottom	= rcBounds.bottom;
		rc_VERT.left	=
			(m_szTotalLog.cx < rcBounds.right) ? m_szTotalLog.cx : rcBounds.right;
		rc_VERT.right	=
			(m_szTotalLog.cx > rcBounds.right) ? m_szTotalLog.cx : rcBounds.right;

		if ( !rc_HORZ.IsRectEmpty() )
			pDC->FillRect(rc_HORZ, &m_brHatch);		// 2005-1-17_液晶屏幕XY比例问题

		if ( !rc_VERT.IsRectEmpty() )
			pDC->FillRect(rc_VERT, &m_brHatch);		// 2005-1-17_液晶屏幕XY比例问题
	}
	else
	{
		pDC->FillRect(rcBounds, &m_brHatch);		// 2005-1-17_液晶屏幕XY比例问题
	}
*/	//-------------------------------------------------
/*
	//07.04.26显示图像小样试验
	POSITION pos = m_plistImageItem->GetHeadPosition();
	if( !pos )
		return;

	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		pitem->Draw( pDC, pitem->m_rect, FALSE );
	}*/
}

void CScanView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( theApp.m_Stage.nHasAutoStage > 0 )
	{
		CPoint pt1 = m_ptStagePointParam;
		CPoint pt2 = ScreenToGraph( point );
		CPoint pt3 = ScreenToGraph( pt1 );
		if( theApp.m_Stage.nHasAutoStage < 2 && theApp.m_Stage.xy.bReady )	// && m_bStageOrigin )
		{
			// 像素距离
			int nLengthX = abs( pt1.x - pt2.x );
			int nLengthY = abs( pt1.y - pt2.y );
			// 实际步数
			int nScaleX = 0;
			int nScaleY = 0;
			if( theApp.m_Stage.xy.bRev )
			{
				CalculateStageUScale( FALSE, (pt1.x<pt2.x) ? TRUE : FALSE, nLengthX, nScaleX );
				CalculateStageUScale( TRUE, (pt1.y<pt2.y) ? TRUE : FALSE, nLengthY, nScaleY );
			}
			else
			{
				CalculateStageUScale( TRUE, (pt1.x<pt2.x) ? TRUE : FALSE, nLengthX, nScaleX );
				CalculateStageUScale( FALSE, (pt1.y<pt2.y) ? TRUE : FALSE, nLengthY, nScaleY );
			}
			
			char szBuffer[64];
			CString str;
			str.Format( "SP %ld,%ld", theApp.m_Stage.xy.nStageSPX, theApp.m_Stage.xy.nStageSPY );
			theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
//			str.Format( "SHXY;PR %ld,%ld;BGXY", (pt1.y<pt2.y) ? -nScaleY : nScaleY,
//												(pt1.x<pt2.x) ? nScaleX : -nScaleX );
//			theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			theApp.Stage_XY_Command2( "PR", nScaleX, nScaleY, (pt1.x<pt2.x) ? FALSE : TRUE, (pt1.y<pt2.y) ? FALSE : TRUE, TRUE );
		}
		else
		{
			StageMC600_MoveToCenter( pt1.x, pt1.y, pt2.x, pt2.y );
		}
		return;
	}
	//获得被选中的项目指针
	m_pSelItem = GetSelImageItem( point );
	if( m_pSelItem )
	{
		CString strName = m_pSelItem->m_strFileName;
		int nPos = strName.Find( '\\' );
		while( nPos != -1 )
		{
			strName = strName.Right( strName.GetLength() - nPos - 1 );
			nPos = strName.Find( '\\' );
		}
//		((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild( strName );
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CScanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == 16 )			// 按下Shift键，加四方向键，可移动自动台
	{
		m_bShiftDown = TRUE;
		char szBuffer[64];
		CString str;
		str.Format( "SP %ld,%ld", theApp.m_Stage.xy.nStageSPX, theApp.m_Stage.xy.nStageSPY );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else if( nChar == 17 )		//是否按下了Ctrl键，用于多个items的选择08.06.27
		m_bCtrlDown = TRUE;
	else if( nChar == 27 )		// 按下ESC，停止自动台移动
	{
		AutoStageXY_FI_Finish( FALSE );
		theApp.m_DMCWin.Command( "ST" );
	}

	if( m_bShiftDown && theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.xy.bReady )
	{
		// 12.06.21 自动台：Shift+四个方向键
		ManuallyAdjustStage( nChar );
	}
	else if( m_bDebugAccel )	// 09.12.10 增加调试状态功能
	{
		// 调试状态下，四个方向键用于消像散的调节
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		VARIANT varMinX, varMaxX, varMinY, varMaxY;

		theApp.m_SemCtrl.m_SemCore.m_StigmatorX.GetRange( varMinX, varMaxX );
		theApp.m_SemCtrl.m_SemCore.m_StigmatorY.GetRange( varMinY, varMaxY );
		ClassID.lVal		= SemControlPanelClass_Stigmator;
		SerialNumber.lVal	= -1;

		int nKey = -1;
		switch( nChar )
		{
		case	VK_LEFT:
			Xpos.lVal = theApp.m_SemCtrl.m_SemCore.m_StigmatorX.GetPos().lVal;
			Xpos.lVal -= 1;
			Flag.lVal = 1;
			nKey = 0;
			break;
		case	VK_RIGHT:
			Xpos.lVal = theApp.m_SemCtrl.m_SemCore.m_StigmatorX.GetPos().lVal;
			Xpos.lVal += 1;
			Flag.lVal = 1;
			nKey = 1;
			break;
		case	VK_UP:
			Ypos.lVal = theApp.m_SemCtrl.m_SemCore.m_StigmatorY.GetPos().lVal;
			Ypos.lVal += 1;
			Flag.lVal = 2;
			nKey = 2;
			break;
		case	VK_DOWN:
			Ypos.lVal = theApp.m_SemCtrl.m_SemCore.m_StigmatorY.GetPos().lVal;
			Ypos.lVal -= 1;
			Flag.lVal = 2;
			nKey = 3;
			break;
		case	192:
			nKey = 10;
			break;
		}

		if( nKey < 10 && nKey > 0 )
		{
			if( Xpos.lVal < varMinX.lVal )
				Xpos.lVal = varMinX.lVal;
			if( Xpos.lVal > varMaxX.lVal )
				Xpos.lVal = varMaxX.lVal;
			if( Ypos.lVal < varMinY.lVal )
				Ypos.lVal = varMinY.lVal;
			if( Ypos.lVal > varMaxY.lVal )
				Ypos.lVal = varMaxY.lVal;

			theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		else if( nKey > 9 )
			SyncCondenserLensReverse( !m_bCondRev );
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CScanView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == 17 )	//是否抬起了Ctrl键，用于多个items的选择08.06.27
		m_bCtrlDown = FALSE;

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CScanView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( bActivate )
		UpdateItemRect( FALSE );
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//关闭当前被选择的图像文件08.06.27
void CScanView::OnChangestateDelete() 
{
	// TODO: Add your command handler code here
	//获得项目列表头位置
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		//获取项目指针
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		//若只有一个项目被选中，且就是该项目
		if( m_pSelItem && !m_bIsMultiSel && (m_pSelItem == pitem) )
		{
			CString strName = pitem->m_strFileName;
			int nPos = strName.Find( '\\' );
			while( nPos != -1 )
			{
				strName = strName.Right( strName.GetLength() - nPos - 1 );
				nPos = strName.Find( '\\' );
			}
//			((CMainFrame *)theApp.GetMainWnd())->DestroyMDIChild( strName );
			//设置被选中项目指针为空
			m_pSelItem = NULL;
			//还原多文件选择标志
			m_bIsMultiSel = FALSE;
			Invalidate( FALSE );
			return;
		}
		else if( m_bIsMultiSel && pitem->m_bSel )
		{//若选择了多个项目，关闭每一个项目
			CString strName = pitem->m_strFileName;
			int nPos = strName.Find( '\\' );
			while( nPos != -1 )
			{
				strName = strName.Right( strName.GetLength() - nPos - 1 );
				nPos = strName.Find( '\\' );
			}
//			((CMainFrame *)theApp.GetMainWnd())->DestroyMDIChild( strName );
		}
/*		if( pitem->m_bSel )
		{
			CString strName = pitem->m_strFileName;
			int nPos = strName.Find( '\\' );
			while( nPos != -1 )
			{
				strName = strName.Right( strName.GetLength() - nPos - 1 );
				nPos = strName.Find( '\\' );
			}
			((CMainFrame *)theApp.GetMainWnd())->DestroyMDIChild( strName );
		}
*/	}
	//还原多文件选择标志
	m_bIsMultiSel = FALSE;
	Invalidate( FALSE );
}

void CScanView::UpdateItemRect( BOOL bPrompt )
{
	POSITION pos = m_plistImageItem->GetHeadPosition();
	if( !pos )
		return;

	//获取客户窗口大小
	CRect rect, rcClient;
	GetClientRect( &rcClient );
//	CPaintDC dc(this); // device context for painting
	if( m_szTotalLog.cx == 0 )
		rect.left = rcClient.Height()/3*4 + 10;
	else
		rect.left	= m_szTotalLog.cx + 4;
	rect.right	= rcClient.Width() - GetSystemMetrics( SM_CXVSCROLL );
	rect.top	= rcClient.top;
	rect.bottom	= rcClient.bottom;
	if( rect.Width() <= 0 || rect.Height() <= 0 )
	{
		while( pos )
		{
			CImageItem* pitem = m_plistImageItem->GetNext( pos );
			CRect rc = CRect( 0, 0, 0, 0 );
			pitem->m_rect = rc;
		}
		return;
	}
	
	CSize sz;
	int count = m_plistImageItem->GetCount();
	if( count <= 9 )
		sz.cx = 2;
	else if( count>9 && count<=20 )
		sz.cx = 3;
	else if( count>20 && count<=30 )
		sz.cx = 4;
	else if( count>30 && count<=42 )
		sz.cx = 5;
	sz.cy = ((double)count/sz.cx > count/sz.cx) ? count/sz.cx+1 : count/sz.cx;
	if( sz.cy < 4 )
		sz.cy = 4;

	int nWidth = rect.Width() / sz.cx;
	int nHeight = rect.Height() / sz.cy;
	if( nWidth/4*3 < nHeight )
		nHeight = nWidth/4*3;
	else if( nWidth/4*3 > nHeight )
		nWidth = nHeight/3*4;

	int x = rect.left;
	int y = rect.top;
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		//调整画图区域
		if( x+nWidth > rect.right )
		{
			x = rect.left;
			y += nHeight;//(nHeight+4);
		}
		CRect rc = CRect( x, y, x+nWidth-4, y+nHeight-4 );//y+nHeight-4 );
		pitem->m_rect = rc;
		x += nWidth;
	}
	InvalidateRect( NULL );
	if( bPrompt && count > 16 )
	{
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = GetResString( IDS_SD_PROMPT_TOOMANY_IMAGE );
		dlg.DoModal();
	}
}

//获得被选中的项目指针
CImageItem* CScanView::GetSelImageItem(CPoint &point)
{
	//获得单击点的实际位置
	CPoint offpoint = point;
	offpoint.Offset( GetScrollPos( SB_HORZ ), GetScrollPos( SB_VERT ));

	CImageItem* pSelItem = NULL;
	//遍历项目列表中的每一个项目，若单击点在某一项目的矩形区内，返回该项目；
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		if( pitem->m_rect.PtInRect( offpoint ) )
		{
//			pitem->m_bSel = TRUE;
			pSelItem = pitem;//return pitem;
		}
	}
	//若无项目被选中，返回空指针
	return pSelItem;
}

void CScanView::DeleteItem( CString strTitle )
{
	//获得项目列表头位置
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		//保存老位置
		POSITION oldpos = pos;
		//获取项目指针
		CImageItem* pitem = m_plistImageItem->GetNext( pos );

		CString strName = pitem->m_strFileName;
		int nPos = strName.Find( '\\' );
		while( nPos != -1 )
		{
			strName = strName.Right( strName.GetLength() - nPos - 1 );
			nPos = strName.Find( '\\' );
		}

		if( strName == strTitle )
		{
			//如果有同名临时文件，删除之
			pitem->DeleteTempFile( strTitle );
			//从项目列表中删除该项目
			m_plistImageItem->RemoveAt( oldpos );
			delete pitem;
			//设置被选中项目指针为空
			m_pSelItem = NULL;
			UpdateItemRect( FALSE );
			Invalidate( FALSE );
			return;
		}
	}
}

void CScanView::ChangeItemName( CString strOldName, CString strNewName )
{
	//遍历项目列表中的每一个项目，若单击点在某一项目的矩形区内，返回该项目；
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );

		CString strName = pitem->m_strFileName;
		int nPos = strName.Find( '\\' );
		while( nPos != -1 )
		{
			strName = strName.Right( strName.GetLength() - nPos - 1 );
			nPos = strName.Find( '\\' );
		}

		if( strName == strOldName )
		{
			pitem->m_strFileName = strNewName;
			return;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CScanView diagnostics

#ifdef _DEBUG
void CScanView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScanView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CScanDoc* CScanView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanDoc)));
	return (CScanDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScanView message handlers

BOOL CScanView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~WS_OVERLAPPED;
	cs.style &= ~WS_SYSMENU;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_CAPTION;
	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~WS_DLGFRAME;
	cs.dwExStyle &= ~WS_EX_DLGMODALFRAME;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle &= ~WS_EX_WINDOWEDGE;
	cs.dwExStyle &= ~WS_EX_STATICEDGE;

	return CScrollView::PreCreateWindow(cs);
}

int CScanView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Init SEM Control
//	if ( !theApp.m_SemCtrl.Init(&(theApp.m_ImageCard)) )
	int nReady = theApp.m_SemCtrl.USB_Init(&(theApp.m_ImageCard));
	if( nReady > -1 )
	{
		CString str;
		if( (nReady & 0x01) == 0x01 )
		{
			str.Format( "%s%s", GetResString(IDS_SD_PROMPT_VAC_31), GetResString(IDS_SD_PROMPT_VAC_37) );
			theApp.DisplayMsg( str );
		}
		if( (nReady & 0x02) == 0x02 )
		{
			str.Format( "%s%s", GetResString(IDS_SD_PROMPT_VAC_32), GetResString(IDS_SD_PROMPT_VAC_37) );
			theApp.DisplayMsg( str );
		}
		if( (nReady & 0x04) == 0x04 )
		{
			str.Format( "%s%s", GetResString(IDS_SD_PROMPT_VAC_33), GetResString(IDS_SD_PROMPT_VAC_37) );
			theApp.DisplayMsg( str );
		}
		if( (nReady & 0x08) == 0x08 )
		{
			str.Format( "%s%s", GetResString(IDS_SD_PROMPT_VAC_34), GetResString(IDS_SD_PROMPT_VAC_37) );
			theApp.DisplayMsg( str );
		}
		if( (nReady & 0x10) == 0x10 )
			theApp.DisplayMsg( "DI16A config error!" );
		if( (nReady & 0x20) == 0x20 )
			theApp.DisplayMsg( "DI16A power not on!" );
		if( (nReady & 0x40) == 0x40 )
			theApp.DisplayMsg( "DI16A set clocks error!" );
		if( (nReady & 0x80) == 0x80 )
			theApp.DisplayMsg( "DI16A could not open stream!" );
	}

	theApp.m_SemCtrl.m_SemScan.Bind( g_cFilmPhotoData );

	CMainFrame* frame = (CMainFrame *)theApp.GetMainWnd();
	///////////////////////////////////
	// MCB
	/* 1.使用单独线程处理MCB消息，主程序空闲时不断查询+定时查询
//	BOOL bMCBReady = theApp.m_SemCtrl.MCB_Init();
//	frame->SetTimers( nMCB_SemMessageTimerID, bMCBReady );
	/* 1. End*/

	// 2. 直接在主线程ScanView中处理MCB消息
	// App::AutoInitSemCtrl之后再启动定时器
	g_strMCB_DeviceName = MCB_OpenHIDDevice();
	MCB_InitHIDDevice( g_strMCB_DeviceName );
	nMCB_Timer				= 0;
//	nMCB_SemMessageTimer = SetTimer( nMCB_TimerID, nMCB_TimerElapse, NULL );
	/* 2. End*/

	/* 3. 在主程序中使用单独线程处理MCB消息*/
	// 见App::AutoInitSemCtrl，所有参数初始化后，启动专门线程
	/* 3. End*/
	///////////////////////////////////

	// Init ZOOM
//	SetScrollSizes( MM_TEXT, g_sizeUSB_RESO_2048_2048 );
//	m_szTotalLog = g_sizeUSB_RESO_2048_2048;
	SetScrollSizes( MM_TEXT, m_sizeReso );
	m_szTotalLog = m_sizeReso;

	///////////////////////////////////////////////////////
	// 读取各参数
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	///////////////////////////////////////////////////////
	// 读颜色值
	int i;
	long* pnColor = new long[10];
	if( theApp.ReadInOneLine(path, "Options", "Colors", LONGSTYLE, 10, pnColor) == 10 )
	{
		for( i=0; i<10; i++ )
		{
			theApp.m_sysOpt.color[i]	= pnColor[i];
			theApp.m_ImageCard.SetColor( i, pnColor[i] );
		}
	}
	delete [] pnColor;
	char szBuf[20];
	theApp.m_sysOpt.nLineWidth = 3;
	if(GetPrivateProfileString( "Options", "LineWidth", "3", szBuf, 10, path) != 0 )
		theApp.m_sysOpt.nLineWidth = atoi( szBuf );
	theApp.m_ImageCard.SetParams( 10, theApp.m_sysOpt.nLineWidth );
	///////////////////////////////////////////////////////
	// 读字体参数
	CString* pstrName = new CString[3];
	if( theApp.ReadInOneLine(path, "Options", "FontNames", STRSTYLE, 3, pstrName) == 3 )
	{
		for( i=0; i<3; i++ )
		{
			theApp.m_sysOpt.fName[i]	= pstrName[i];
			theApp.m_ImageCard.SetFontName( i, pstrName[i] );
		}
	}
	delete [] pstrName;
	int* pnSize = new int[3];
	if( theApp.ReadInOneLine(path, "Options", "FontSizes", INTSTYLE, 3, pnSize) == 3 )
	{
		for( i=0; i<3; i++ )
		{
			theApp.m_sysOpt.fSize[i]	= pnSize[i];
			theApp.m_ImageCard.SetFontSize( i, pnSize[i] );
		}
	}
	delete [] pnSize;
	///////////////////////////////////////////////////////
	// 显示参数
	theApp.m_sysOpt.nShowParams[SysOpt_showLabelTrans]	= 1;
	theApp.m_sysOpt.nShowParams[SysOpt_showScrMag]		= 1;
	theApp.m_sysOpt.nShowParams[SysOpt_showDate]		= 0;
	theApp.m_sysOpt.nShowParams[SysOpt_showNote]		= 0;
	theApp.m_sysOpt.nShowParams[SysOpt_showCCD]			= 1;
	int* pnShow = new int[5];
	if( theApp.ReadInOneLine(path, "Options", "ShowParams", INTSTYLE, 5, pnShow) == 5 )
	{
		for( i=0; i<5; i++ )
			theApp.m_sysOpt.nShowParams[i]	= pnShow[i];
	}
	delete [] pnShow;
	theApp.m_ImageCard.SetTransparent( theApp.m_sysOpt.nShowParams[SysOpt_showLabelTrans] );
	m_bDate = theApp.m_sysOpt.nShowParams[SysOpt_showDate];
	m_bNote = theApp.m_sysOpt.nShowParams[SysOpt_showNote];
	///////////////////////////////////////////////////////
	// 其它系统参数
	CString strLo1, strLo2, strLo3, strLo4;
	if(GetPrivateProfileString( "Options2", "Lo1", "EM", szBuf, 10, path) != 0 )
		strLo1 = szBuf;
	if(GetPrivateProfileString( "Options2", "Lo2", "62", szBuf, 10, path) != 0 )
		strLo2 = szBuf;
	if(GetPrivateProfileString( "Options2", "Lo3", "00", szBuf, 10, path) != 0 )
		strLo3 = szBuf;
	if(GetPrivateProfileString( "Options2", "Lo4", "", szBuf, 10, path) != 0 )
		strLo4 = szBuf;
	theApp.m_strLogoDisp.Format( "KYKY-%s%s%s%s", strLo1, strLo2, strLo3, strLo4 );
	///////////////////////////////////////////////////////
	// 底标参数
	if( ReadFooterSettings( path ) < 0 )
		SetFooterSettingsDefault();
	for( i=0; i<11; i++ )
	{
		theApp.m_ImageCard.UpdateFooter( i, theApp.m_footerOpt.opt[i].bVisible, theApp.m_footerOpt.opt[i].ptPos );
		theApp.m_ImageCard.UpdateFooterText( i, theApp.m_footerOpt.opt[i].strDisp );
	}
	// 19.06.19 扫描区域显示像素
	m_nDispPixel = 1024;
	if(GetPrivateProfileString( "Hardware", "DispP", "1024", szBuf, 10, path) != 0 )
		m_nDispPixel = atoi(szBuf);

	m_scanParamLive.nScan_Type		= USB_ST_RasterScan;
	m_scanParamLive.nScan_Speed		= 1;	// 500ns
	m_scanParamLive.nSync_Speed		= m_nusbPhotoPixRate;
	m_scanParamLive.nScan_ResoX		= 512;	// 512*384
	m_scanParamLive.nScan_ResoY		= 384;
	m_scanParamLive.nScan_left		= 0;
	m_scanParamLive.nScan_top		= 0;
	m_scanParamLive.nScan_right		= 512;
	m_scanParamLive.nScan_bottom	= 384;
	m_scanParamLive.nSplit			= 0;
	m_scanParamLive.nDet2			= 0;
	m_scanParamLive.nStoreMode		= 1;

	theApp.m_ImageCard.SetZoom( 1.0 );
//	theApp.m_ImageCard.SetResolution(RESO_1024_768);
	theApp.m_ImageCard.SetResolution(m_sizeReso);
	theApp.m_ImageCard.Reset( this );
	theApp.m_ImageCard.ShowLabel( m_bLabel );
	theApp.m_ImageCard.SetParams( 4, 0 );	// 默认显示偶数通道图像数据
	theApp.m_SemCtrl.m_SemScan.USB_SetParams( SET_PARAMS_NeedMon, 0 );

	//======2006.11.22_3200与3900采集屏显示兼容============
	// 修改标示：2006.11.22_3200与3900采集屏显示兼容
	// 修改日期：2006.11.22
	// 修改人  ：J.Y.Ma
	// 修改原因：合并程序后，使各主程序均能调用同一动态链接库

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	-----------------------------------------------------*/
	
	// 本次修改后代码
	//-----------------------------------------------------
	theApp.m_ImageCard.SetScanScreenMode( TRUE );
	theApp.m_ImageCard.SetProductLogo( theApp.m_strLogoDisp );
	//-----------------------------------------------------
	//======2006.11.22_3200与3900采集屏显示兼容============

	///////////////////////////////////////////////////////
	// VAC State Machine
	theApp.m_SemCtrl.VAC_Init( &(theApp.m_SMvac) );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 分配多个内存段，并通知SemScan和VirtualMCICard
	for( i=0; i<SharedImageBuffCounts; i++ )
	{
		theApp.m_pSharedImageBuff[i] = new unsigned short[LOOP_COUNTS * 40960];
		theApp.m_SemCtrl.m_SemScan.USB_SetSharedImageBuff( i, theApp.m_pSharedImageBuff[i] );
		theApp.m_ImageCard.SetSharedImageBuff( i, theApp.m_pSharedImageBuff[i] );
	}
	// 按最快100ms一个数据计算，1秒10个，1分钟600个，1小时36000个，10小时360000个
	for( i=0; i<GraphDataBuffGroups; i++ )
	{
		theApp.m_pGraphDataBuff[i] = new double[GraphDataBuffCounts];
		ZeroMemory( theApp.m_pGraphDataBuff[i], sizeof(double)*GraphDataBuffCounts );
	}
	for( i=0; i<GraphDataBuffGroups; i++ )
	{
		theApp.m_pDiffGraphDataBuff[i] = new double[GraphDataBuffCounts];
		ZeroMemory( theApp.m_pDiffGraphDataBuff[i], sizeof(double)*GraphDataBuffCounts );
	}
	///////////////////////////////////////////////////////

	return 0;
}

int CScanView::ReadFooterSettings( char* path )
{
	for( int i=0; i<11; i++)
		theApp.m_footerOpt.opt[i].bVisible	= FALSE;

	char szBuf[20], szBuf1[255], szBuf2[255], szBuf3[50];
	int nLines = 0;
	if(GetPrivateProfileString( "FooterSettings", "Lines", "", szBuf, 10, path) != 0 )
		nLines = atoi(szBuf);
	if( nLines < 1 )
		return -1;
	int nPos = 0;
	int i= 0;
	for( i=0; i<nLines; i++ )
	{
		sprintf(szBuf, "MacroLine%d", i+1);
		if(GetPrivateProfileString( "FooterSettings", szBuf, "", szBuf1, 255, path) != 0 )
		{
			strcpy(szBuf2, szBuf1);
			while(strchr(szBuf2, '}') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, '}')+1);	// szBuf1中是该行剩余字符串
				*strchr(szBuf2, '}') = '\0';			// szBuf2中是字符串{HV;x=0
				strcpy(szBuf3, szBuf2+1);				// szBuf3中是当前字符串HV;x=0
				strcpy(szBuf2, strrchr(szBuf3, '=')+1);	// szBuf2中是当前字符串的位置
				nPos = atoi(szBuf2);
				*strchr(szBuf3, ';') = '\0';			// szBuf3中是带标识的当前字符串的内容
				strcpy(szBuf2, szBuf3+1);				// szBuf2中是当前字符串的内容
				switch(szBuf3[0])
				{
				case	't':	// Time
					theApp.m_footerOpt.opt[0].bVisible	= TRUE;
					theApp.m_footerOpt.opt[0].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[0].ptPos.y	= i+1;
					ReadFooter_TimeFormat(szBuf2);
					break;
				case	'h':	// HV
					theApp.m_footerOpt.opt[1].bVisible	= TRUE;
					theApp.m_footerOpt.opt[1].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[1].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[1].strDisp	= szBuf2;
					break;
				case	'w':	// WD
					theApp.m_footerOpt.opt[2].bVisible	= TRUE;
					theApp.m_footerOpt.opt[2].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[2].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[2].strDisp	= szBuf2;
					break;
				case	'm':	// Mag
					theApp.m_footerOpt.opt[3].bVisible	= TRUE;
					theApp.m_footerOpt.opt[3].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[3].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[3].strDisp	= szBuf2;
					break;
				case	'd':	// DET
					theApp.m_footerOpt.opt[4].bVisible	= TRUE;
					theApp.m_footerOpt.opt[4].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[4].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[4].strDisp	= szBuf2;
					break;
				case	'l':	// Logo
					theApp.m_footerOpt.opt[5].bVisible	= TRUE;
					theApp.m_footerOpt.opt[5].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[5].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[5].strDisp	= szBuf2;
					break;
				case	'u':	// uBar
					theApp.m_footerOpt.opt[6].bVisible	= TRUE;
					theApp.m_footerOpt.opt[6].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[6].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[6].strDisp	= szBuf2;
					break;
				case	'n':	// Note
					theApp.m_footerOpt.opt[7].bVisible	= TRUE;
					theApp.m_footerOpt.opt[7].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[7].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[7].strDisp	= szBuf2;
					break;
				case	'v':	// Vac
					theApp.m_footerOpt.opt[8].bVisible	= TRUE;
					theApp.m_footerOpt.opt[8].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[8].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[8].strDisp	= szBuf2;
					break;
				case	'e':	// EMI
					theApp.m_footerOpt.opt[9].bVisible	= TRUE;
					theApp.m_footerOpt.opt[9].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[9].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[9].strDisp	= szBuf2;
					break;
				case	's':	// Speed
					theApp.m_footerOpt.opt[10].bVisible	= TRUE;
					theApp.m_footerOpt.opt[10].ptPos.x	= nPos;
					theApp.m_footerOpt.opt[10].ptPos.y	= i+1;
					theApp.m_footerOpt.opt[10].strDisp	= szBuf2;
					break;
				}
				strcpy(szBuf2, szBuf1);
			}
		}
	}
	return 0;
}

void CScanView::ReadFooter_TimeFormat( char* buf )
{
	// buf = "Time=Y/m/d H/M"
	char szBuf[100], szBuf1[20], szPer, ch;
	strcpy( szBuf1, buf );
	strcpy(szBuf, strchr(szBuf1, '=')+1);	// szBuf中是日期格式字符串

	szPer = '%';
	sprintf( szBuf1, "" );
	int len = strlen( szBuf );
	int index = 0;
	for( int i=0; i<len; i++ )
	{
		ch = szBuf[i];
		if(ch >='a' && ch<='z' ||  ch >='A' && ch<='Z')
		{
			szBuf1[index] = szPer;
			index++;
		}
		szBuf1[index] = ch;
		index++;
	}
	szBuf1[index] = '\0';
	theApp.m_footerOpt.opt[0].strDisp = szBuf1;
}

void CScanView::SetFooterSettingsDefault()
{
	// Time
	theApp.m_footerOpt.opt[0].bVisible	= TRUE;
	theApp.m_footerOpt.opt[0].ptPos.x	= 75;
	theApp.m_footerOpt.opt[0].ptPos.y	= 1;
	theApp.m_footerOpt.opt[0].strDisp	= "%Y.%m.%d %H:%M";
	// HV
	theApp.m_footerOpt.opt[1].bVisible	= TRUE;
	theApp.m_footerOpt.opt[1].ptPos.x	= 2;
	theApp.m_footerOpt.opt[1].ptPos.y	= 1;
	theApp.m_footerOpt.opt[1].strDisp	= "HV";
	// WD
	theApp.m_footerOpt.opt[2].bVisible	= TRUE;
	theApp.m_footerOpt.opt[2].ptPos.x	= 2;
	theApp.m_footerOpt.opt[2].ptPos.y	= 2;
	theApp.m_footerOpt.opt[2].strDisp	= "WD";
	// Mag
	theApp.m_footerOpt.opt[3].bVisible	= TRUE;
	theApp.m_footerOpt.opt[3].ptPos.x	= 25;
	theApp.m_footerOpt.opt[3].ptPos.y	= 1;
	theApp.m_footerOpt.opt[3].strDisp	= "MAG";
	// DET
	theApp.m_footerOpt.opt[4].bVisible	= TRUE;
	theApp.m_footerOpt.opt[4].ptPos.x	= 25;
	theApp.m_footerOpt.opt[4].ptPos.y	= 2;
	theApp.m_footerOpt.opt[4].strDisp	= "DET";
	// Logo
	theApp.m_footerOpt.opt[5].bVisible	= TRUE;
	theApp.m_footerOpt.opt[5].ptPos.x	= 50;
	theApp.m_footerOpt.opt[5].ptPos.y	= 2;
	theApp.m_footerOpt.opt[5].strDisp	= "Logo";
	// uBar
	theApp.m_footerOpt.opt[6].bVisible	= TRUE;
	theApp.m_footerOpt.opt[6].ptPos.x	= 50;
	theApp.m_footerOpt.opt[6].ptPos.y	= 1;
	theApp.m_footerOpt.opt[6].strDisp	= "uBar";
	// Note
	theApp.m_footerOpt.opt[7].bVisible	= TRUE;
	theApp.m_footerOpt.opt[7].ptPos.x	= 75;
	theApp.m_footerOpt.opt[7].ptPos.y	= 2;
	theApp.m_footerOpt.opt[7].strDisp	= "Note";
	// Vac
	theApp.m_footerOpt.opt[8].bVisible	= FALSE;
	theApp.m_footerOpt.opt[8].ptPos.x	= 0;
	theApp.m_footerOpt.opt[8].ptPos.y	= 0;
	theApp.m_footerOpt.opt[8].strDisp	= "Vac";
	// EMI
	theApp.m_footerOpt.opt[9].bVisible	= FALSE;
	theApp.m_footerOpt.opt[9].ptPos.x	= 0;
	theApp.m_footerOpt.opt[9].ptPos.y	= 0;
	theApp.m_footerOpt.opt[9].strDisp	= "EMI";
	// Speed
	theApp.m_footerOpt.opt[10].bVisible	= FALSE;
	theApp.m_footerOpt.opt[10].ptPos.x	= 0;
	theApp.m_footerOpt.opt[10].ptPos.y	= 0;
	theApp.m_footerOpt.opt[10].strDisp	= "Speed";
}

void CScanView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: calculate the total size of this view
	m_szTotalLog.cx = m_szTotalLog.cy = 100;
	SetScrollSizes(MM_TEXT, m_szTotalLog);

	SemLabel_Init( m_sizeReso.cx );
//	USB_ChangeScan( USB_ST_RasterScan );
	UpdateWindowPos();
//	OnViewZoomFit();
}

void CScanView::Localize(void)
{
	LocalizeMenu();
}

void CScanView::LocalizeMenu(void)
{
	m_wLanguageID = theApp.m_wLanguageID;

	// 获取menu.mu的路径
	TCHAR path[255];
	::GetModuleFileName(0, path, 255);
	LPTSTR pszFileName = _tcsrchr(path, '\\') + 1;
	*pszFileName = '\0';
	strcat( path, g_csMenuFileName );

	TCHAR szBuf[20];

	// 获取根菜单数目
	TCHAR szBufRootSection[20];
	int nMenuRootCount = 0;
	sprintf(szBufRootSection, "Menu_Scan_Root");

	if(GetPrivateProfileString(szBufRootSection, "Count", "0", szBuf, 255, path) != 0)
		nMenuRootCount = atoi(szBuf);
	else
		return;
	if( nMenuRootCount == 0 )
		return;

	int i;
	// 分配菜单资源
	CMenu menuNew;
	menuNew.CreateMenu();
	CMenu* pmenu = new CMenu[nMenuRootCount];
	for( i=0; i<nMenuRootCount; i++ )
		VERIFY( pmenu[i].CreatePopupMenu() );

	// 获取根菜单IDS
	CStringArray strRoot;
	strRoot.SetSize( nMenuRootCount, 20 );
	TCHAR szBufRootKey[20];
	for( i=0; i<nMenuRootCount; i++ )
	{
		sprintf(szBufRootKey, "menu%d", i);
		if(GetPrivateProfileString(szBufRootSection, szBufRootKey, "0", szBuf, 255, path) != 0)
			strRoot[i].Format( "%s", szBuf );
	}

	// 依次获取子菜单
	int nMenuSubCount = 0;
	UINT nID = 0;
	for( i=0; i<nMenuRootCount; i++ )
		theApp.MyAppendMenu( &(pmenu[i]), atoi(strRoot[i]), path );

	CString str;
	for( i=0; i<nMenuRootCount; i++ )
	{
		nID = atoi( strRoot[i] );
		menuNew.AppendMenu(MF_POPUP, (UINT) pmenu[i].m_hMenu, GetResString(nID));
	}

	CScanDoc* pdoc = GetDocument();
	pdoc->m_DefaultMenu = menuNew.GetSafeHmenu();
	if (pdoc->m_DefaultMenu == NULL)
		return;
/*	CMDIFrameWnd* frame = ((CMDIChildWnd *) GetParent())->GetMDIFrame();
	CMenu* wmenu = CMenu::FromHandle(frame->GetWindowMenuPopup((CMenu::FromHandle(pdoc->m_DefaultMenu))->GetSafeHmenu()));
	frame->MDISetMenu(CMenu::FromHandle(pdoc->m_DefaultMenu), wmenu);
	frame->DrawMenuBar();
*/
	CMainFrame* frame = (CMainFrame *)theApp.GetMainWnd();
	frame->SetMenu( CMenu::FromHandle(pdoc->m_DefaultMenu) );
	frame->DrawMenuBar();

	// delete and detach
	for( i=0; i<nMenuRootCount; i++ )
		pmenu[i].Detach();
	delete [] pmenu;
	menuNew.Detach();
}


BOOL CScanView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect( &rc );
	pDC->FillRect( rc, &CBrush( RGB(0,0,0) ) );
	return TRUE;//CScrollView::OnEraseBkgnd(pDC);
}

void CScanView::OnDestroy() 
{
	///////////////////////////////////
	// 15.09 远程功能
	OnOptionsListenStop();
	///////////////////////////////////

	USB_ChangeScan( USB_ST_Stop );
	theApp.DisplayMsg("ImageCard Close");
	theApp.m_ImageCard.Close();

	///////////////////////////////////
	// MCB
	MCB_CloseHIDDevice();
	if( nMCB_Timer != 0 )
	{
		KillTimer( nMCB_Timer );
		nMCB_Timer = 0;
	}
	///////////////////////////////////

	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
//	theApp.m_SemCtrl.Close();	// 退出时重复调用，屏蔽
}

void CScanView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptPosSave				= ScreenToGraph( point );
	m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );

	//======2004-5-13_屏幕鼠标控制电镜=====================
	// 修改标示：2004-5-13_屏幕鼠标控制电镜
	// 修改日期：2004年5月13日
	// 修改人：罗明华
	// 修改原因：添加屏幕鼠标操作电镜控制。

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	if ( m_nGraphHitTestSave != GraphHitTest_Normal )
	{
		m_bCatch = TRUE;
		m_rcDragRectLast = CRect(0,0,0,0);
		SetCapture();
	}
	-----------------------------------------------------*/

	// 本次修改后代码
	//-----------------------------------------------------
	//SemCtrlMouse_Init
	if ( m_bAbc != TRUE )
	{
		if ( m_nGraphHitTestSave == GraphHitTest_Normal )
		{
			switch( m_nAdjustorSort )				//根据调节器类型设置光标
			{
			case	SORT_OBJECTIVE_LENS:
				SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_OBJECTIVE_LENS ) ) );
				break;
			case	SORT_STIGMATOR:
				SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_STIGMATOR ) ) );
				break;
			default:
				SetCursor( g_hCursors[0] );//AfxGetApp()->LoadCursor( IDC_ARROW ) );
				break;
			}
			m_rcDragRectLast = CRect(0,0,0,0);
			SetCapture();

			SemCtrlMouse_Init( m_ptPosSave, m_nAdjustorSort );
		}
		if( m_nGraphHitTestSave != GraphHitTest_NULL )
			m_bCatch = TRUE;
	}
	//-----------------------------------------------------
	//======2004-5-13_屏幕鼠标控制电镜=====================

/****************************************************************************
*	08.12.18  对于一般情况(单击鼠标)，或按下Ctrl键后单击，来选择文件的情况  *
*****************************************************************************/
	//获得被选中的项目指针
	m_pSelItem = GetSelImageItem( point );
	//若当前没有被选中的单个项目，且并非处于多文件选择状态，则返回
	if( !m_pSelItem && !m_bIsMultiSel )
	{
		//09.11.18遍历项目列表中的每一个项目，将所有item都设为非选中显示
		CDC* pDC = GetDC();
		POSITION pos = m_plistImageItem->GetHeadPosition();
		while( pos )
		{
			CImageItem* pitem = m_plistImageItem->GetNext( pos );
			if( pitem->m_bSel == TRUE )
			{
				pitem->m_bSel = FALSE;
				pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );
			}
		}
		ReleaseDC( pDC );

		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	CDC* pDC = GetDC();	//08.12.18
	//获得单击点的实际位置
	CPoint offpoint = point;
	offpoint.Offset( GetScrollPos( SB_HORZ ), GetScrollPos( SB_VERT ));
	//还原被选中项目指针和多文件选择标志
	m_pSelItem = NULL;
	m_bIsMultiSel = FALSE;
	//遍历项目列表中的每一个项目
	POSITION pos = m_plistImageItem->GetHeadPosition();
	while( pos )
	{
		CImageItem* pitem = m_plistImageItem->GetNext( pos );
		//08.12.18
/*
		//对于一般情况，先将所有item都设为非选中显示08.06.27
		if( !m_bCtrlDown )
			pitem->m_bSel = FALSE;
*/
		//08.12.18
		if( pitem->m_rect.PtInRect( offpoint ) )
		{
			//对于一般情况，设置项目的被选中标志、文档中的被选中项目指针08.06.27
			if( !m_bCtrlDown )
			{
				if( !pitem->m_bSel )
				{
					pitem->m_bSel = TRUE;
					pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );//08.12.18
				}
				m_pSelItem = pitem;
			}
			//对于按下Ctrl的情况
			else
			{
				pitem->m_bSel = !(pitem->m_bSel);//可在选中与非选中之间切换
				pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );//08.12.18
			}
			//若项目被选中，设置多文件选择标志08.06.27
			if( pitem->m_bSel && m_bCtrlDown )
				m_bIsMultiSel = TRUE;
		}
		else//08.12.18
		{
			//对于一般情况
			if( !m_bCtrlDown && pitem->m_bSel )
			{
				pitem->m_bSel = FALSE;
				pitem->Draw( pDC, pitem->m_rect, pitem->m_bSel );
			}
		}
	}
	ReleaseDC( pDC );//08.12.18

	CScrollView::OnLButtonDown(nFlags, point);
}

void CScanView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptPosSave				= ScreenToGraph( point );
	m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );

	int i, j;
	// 16.06.12 多移动标尺实验
	// 右击则删除选中测量线
	for( i=0; i<MobileUScaleCounts; i++ )
	{
		if( m_bUScaleHitTestArray[i] )
			break;
	}
	if( i != MobileUScaleCounts )
	{
		m_bUScaleHitTestArray[i]	= FALSE;
		CString csMobileUScaleText;
		// 移动数组
		for( j=i; j<MobileUScaleCounts -1; j++ )
		{
			m_ptMobileUScaleArray[j]		= m_ptMobileUScaleArray[j+1];
			m_ptMobileUScaleArray2[j]		= m_ptMobileUScaleArray2[j+1];
			m_nMobileUScalePixelNumArray[j]	= m_nMobileUScalePixelNumArray[j+1];
			theApp.m_ImageCard.SetMobileUScalePosArray( j, m_ptMobileUScaleArray[j], m_ptMobileUScaleArray2[j], RGB(255, 0, 0) );
			csMobileUScaleText = CalculateMobileUScale( m_nMobileUScalePixelNumArray[j] );
			theApp.m_ImageCard.UpdateUScaleArray(j, m_nMobileUScalePixelNumArray[j], csMobileUScaleText );
		}
		// 数组最后一个值复位
		m_ptMobileUScaleArray[MobileUScaleCounts -1]		= CPoint(-1, -1);
		m_ptMobileUScaleArray2[MobileUScaleCounts -1]		= CPoint(-1, -1);
		m_nMobileUScalePixelNumArray[MobileUScaleCounts -1]	= 0;
		theApp.m_ImageCard.SetMobileUScalePosArray( MobileUScaleCounts -1, m_ptMobileUScaleArray[MobileUScaleCounts -1], m_ptMobileUScaleArray2[MobileUScaleCounts -1], RGB(255, 0, 0) );
		csMobileUScaleText = CalculateMobileUScale( m_nMobileUScalePixelNumArray[MobileUScaleCounts -1] );
		theApp.m_ImageCard.UpdateUScaleArray(MobileUScaleCounts -1, m_nMobileUScalePixelNumArray[MobileUScaleCounts -1], csMobileUScaleText );
		// 更新数量
		m_nMobileUScaleCounts--;
		if( m_nMobileUScaleCounts < 0 )
			m_nMobileUScaleCounts = 0;
	}
	// 16.06.12 多移动标尺实验

	//======2004-5-13_屏幕鼠标控制电镜=====================
	//SemCtrlMouse_Init
	if( m_bAbc != TRUE )
	{
		if( m_nGraphHitTestSave == GraphHitTest_Normal )
		{
			SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_STIGMATOR ) ) );
			m_rcDragRectLast = CRect(0,0,0,0);
			SetCapture();

			SemCtrlMouse_Init( m_ptPosSave, SORT_STIGMATOR );
		}
		if( m_nGraphHitTestSave != GraphHitTest_NULL )
			m_bCatch = TRUE;
	}
	//-----------------------------------------------------
	//======2004-5-13_屏幕鼠标控制电镜=====================

}

CPoint CScanView::ScreenToGraph( const CPoint point )
{
	CPoint ptPos = point;

	////////////////////////////////////////////
	// Get Logic Pos (Screen)
	////////////////////////////////////////////
	ptPos.x += GetScrollPos( SB_HORZ );
	ptPos.y += GetScrollPos( SB_VERT );
	////////////////////////////////////////////

	////////////////////////////////////////////
	// ZOOM to Graph (Graph)
	////////////////////////////////////////////
	double dScale = theApp.m_ImageCard.GetZoom();
	if ( dScale > 0 )
	{
		ptPos.x = (int)(ptPos.x / dScale + 0.5);
		ptPos.y = (int)(ptPos.y / dScale + 0.5);
	}
	else
	{
		ptPos.x = (int)(ptPos.x / g_dScaleScanTable[g_nScaleScanIndex] + 0.5);
		ptPos.y = (int)(ptPos.y / g_dScaleScanTable[g_nScaleScanIndex] + 0.5);
	}

	return ptPos;
}

UINT CScanView::USB_GraphHitTest( const CPoint ptGraphPos )
{
	CRect	rcFrame;
	CRect	rcFrame2;
	CRect	rcFrame3;

	BOOL	boolLeft	= FALSE;
	BOOL	boolRight	= FALSE;
	BOOL	boolUp		= FALSE;
	BOOL	boolDown	= FALSE;

	//////////////////////////////////////////////////////////////////
	if ( m_bOscillograph )
	{
		theApp.m_ImageCard.GetOscillographParam( m_ptOscillograph );

		rcFrame3.left	= m_ptOscillograph.x;
		rcFrame3.right	= m_ptOscillograph.x + g_szUSB_FrameOscillograph.cx;
		rcFrame3.top	= m_ptOscillograph.y;
		rcFrame3.bottom	= m_ptOscillograph.y + g_szUSB_FrameOscillograph.cy;

		if ( rcFrame3.PtInRect( ptGraphPos ) )
		{
			m_bOscillographHitTest = TRUE;
			return GraphHitTest_In;
		}
	}
	m_bOscillographHitTest = FALSE;
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	if ( m_bUScale )
	{
		theApp.m_ImageCard.GetMobileUScaleParamEx(
			m_nMobileUScalePixelNum, m_ptMobileUScale, m_ptMobileUScale2 );
		CRect trackerRect( m_ptMobileUScale, m_ptMobileUScale2 );
		CLineTracker tracker( &trackerRect, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
		int nHitTest = tracker.HitTest( ptGraphPos );
		switch( nHitTest )
		{
		case	0:
			m_bUScaleHitTest = TRUE;
			return GraphHitTest_W;
		case	2:
			m_bUScaleHitTest = TRUE;
			return GraphHitTest_E;
		case	8:
			m_bUScaleHitTest = TRUE;
			return GraphHitTest_In;
		}

		int i;
		// 16.06.12 多移动标尺实验
		for( i=0; i<MobileUScaleCounts; i++ )
			m_bUScaleHitTestArray[i] = FALSE;
		int nPixelNum;
		CPoint pt, pt2;
		for( i=0; i<MobileUScaleCounts; i++ )
		{
			theApp.m_ImageCard.GetMobileUScaleParamArray( i, nPixelNum, pt, pt2 );
			CRect trackerRc( pt, pt2 );
			CLineTracker tracker2( &trackerRc, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
			nHitTest = tracker2.HitTest( ptGraphPos );
			switch( nHitTest )
			{
			case	0:
				m_bUScaleHitTestArray[i] = TRUE;
				return GraphHitTest_W;
			case	2:
				m_bUScaleHitTestArray[i] = TRUE;
				return GraphHitTest_E;
			case	8:
				m_bUScaleHitTestArray[i] = TRUE;
				return GraphHitTest_In;
			}
		}
		// 16.06.12 多移动标尺实验
	}
	m_bUScaleHitTest = FALSE;
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_bStageOriginHitTest = FALSE;
	if( m_bStageOrigin )
	{
		rcFrame = m_rcStageDragRectLast;
		if( ptGraphPos.x < 1024 && ptGraphPos.y < 768 )
		{
			if ( ( abs(rcFrame.top +20 - ptGraphPos.y) < HandleAreaH2 )
				&& ( abs(rcFrame.left +20 - ptGraphPos.x) < HandleAreaW2 ) )
			{
				m_bStageOriginHitTest = TRUE;
				return GraphHitTest_In;
			}
//			else
//				return GraphHitTest_Normal;
		}
//		else
//			return GraphHitTest_NULL;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	int nusbScanType= m_nusbScanType;
	int	nWidth		= m_sizeReso.cx;
	int nHeight		= m_sizeReso.cy;

	switch ( nusbScanType )
	{
	case USB_ST_SpotScan:			// 点扫描
	case USB_ST_LineScan_HORZ:		// 水平线扫描
	case USB_ST_LineScan_VERT:		// 垂直线扫描
	case USB_ST_AreaScan:			// 选区扫描
		rcFrame.left		= m_usbParam.RasterScan.dwStartX;
		rcFrame.right		= m_usbParam.RasterScan.dwStopX;
		rcFrame.top			= m_usbParam.RasterScan.dwStartY;
		rcFrame.bottom		= m_usbParam.RasterScan.dwStopY;
		break;

	case USB_ST_Stop:				// 冻结图象扫描
	case USB_ST_RasterScan:			// 常规扫描
	case USB_ST_AutoBCArea:			// 固定选区扫描	AutoBC
	case USB_ST_AutoFocusArea:		// 固定选区扫描 AutoFocus
	case USB_ST_AutoAstigmatArea:	// 固定选区扫描 AutoAstigmat
	case USB_ST_AnalogPhoto:		// 模拟照相
	default:
		// 09.11.18将整个扫描区域内外的单击测试类型区别开
		// 本次修改前代码副本
		/*------------------------------------------------- 
		return GraphHitTest_Normal;
		-------------------------------------------------*/

		// 本次修改后代码
		//-------------------------------------------------
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
			return GraphHitTest_Normal;
		else
			return GraphHitTest_NULL;
		//-------------------------------------------------
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	if ( nusbScanType == USB_ST_SpotScan )
	{
		// 09.11.18将整个扫描区域内外的单击测试类型区别开
		// 本次修改前代码副本
		/*------------------------------------------------- 
		if ( ( abs(rcFrame.top - ptGraphPos.y) < HandleAreaH )
			&& ( abs(rcFrame.left - ptGraphPos.x) < HandleAreaW ) )
			return GraphHitTest_In;
		else
			return GraphHitTest_Normal;
		-------------------------------------------------*/

		// 本次修改后代码
		//-------------------------------------------------
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		{
			if ( ( abs(rcFrame.top - ptGraphPos.y) < HandleAreaH )
				&& ( abs(rcFrame.left - ptGraphPos.x) < HandleAreaW ) )
				return GraphHitTest_In;
			else
				return GraphHitTest_Normal;
		}
		else
			return GraphHitTest_NULL;
		//-------------------------------------------------
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	if ( nusbScanType == USB_ST_LineScan_HORZ )
	{
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		{
			if( abs(rcFrame.top - ptGraphPos.y) < HandleAreaH)
				return GraphHitTest_NS;
			else
				return GraphHitTest_Normal;
		}
		else
			return GraphHitTest_NULL;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	if ( nusbScanType == USB_ST_LineScan_VERT )
	{
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		{
			if( abs(rcFrame.left - ptGraphPos.x) < HandleAreaH)
				return GraphHitTest_E;
			else
				return GraphHitTest_Normal;
		}
		else
			return GraphHitTest_NULL;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	if ( rcFrame.PtInRect( ptGraphPos ) )
		return GraphHitTest_In;

	////////////////////////////////////////////////
	boolLeft	= FALSE;
	boolRight	= FALSE;
	boolUp		= FALSE;
	boolDown	= FALSE;
	////////////////////////////////////////////////

	//======2004-5-13_屏幕鼠标控制电镜=====================
	// 修改标示：2005.04.18 光标变化
	// 修改日期：2005.04.18
	// 修改人：J.Y.Ma
	// 修改原因：原来的鼠标是否在区域内的判断规则不准确

	// 本次修改前代码副本
	/*----------------------------------------------------- 
/*	if ( ( ptGraphPos.x <= rcFrame.left )
		&& (rcFrame.left - ptGraphPos.x)<HandleAreaW )
		boolLeft = TRUE;

	if ( ( ptGraphPos.x >= rcFrame.right )
		&& (ptGraphPos.x - rcFrame.right)<HandleAreaW )
		boolRight = TRUE;

	if ( ( ptGraphPos.y <= rcFrame.top )
		&& (rcFrame.top - ptGraphPos.y)<HandleAreaH )
		boolUp = TRUE;

	if ( ( ptGraphPos.y >= rcFrame.bottom )
		&& (ptGraphPos.y - rcFrame.bottom)<HandleAreaH )
		boolDown = TRUE;
	-----------------------------------------------------*/

	// 本次修改后代码
	//-----------------------------------------------------
	if ( abs(rcFrame.left - ptGraphPos.x)<HandleAreaW
		&& (ptGraphPos.y >= rcFrame.top)
		&& (ptGraphPos.y <= rcFrame.bottom) )
		boolLeft = TRUE;

	if ( abs(ptGraphPos.x - rcFrame.right)<HandleAreaW
		&& (ptGraphPos.y >= rcFrame.top)
		&& (ptGraphPos.y <= rcFrame.bottom) )
		boolRight = TRUE;

	if ( abs(rcFrame.top - ptGraphPos.y)<HandleAreaH
		&& (ptGraphPos.x >= rcFrame.left)
		&& (ptGraphPos.x <= rcFrame.right) )
		boolUp = TRUE;

	if ( abs(ptGraphPos.y - rcFrame.bottom)<HandleAreaH
		&& (ptGraphPos.x >= rcFrame.left)
		&& (ptGraphPos.x <= rcFrame.right) )
		boolDown = TRUE;

	////////////////////////////////////////
/*	if ( nusbScanType == USB_ST_DualMagnification )
	{
		// 09.11.18将整个扫描区域内外的单击测试类型区别开
		if(boolRight)
			return GraphHitTest_E;
		if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
			return GraphHitTest_Normal;
		return GraphHitTest_NULL;
	}*/
	////////////////////////////////////////

	if(boolLeft && boolUp)
		return GraphHitTest_SW;

	if(boolLeft && boolDown)
		return GraphHitTest_NW;

	if(boolRight && boolUp)
		return GraphHitTest_NE;

	if(boolRight && boolDown)
		return GraphHitTest_SE;

	if(boolLeft)
		return GraphHitTest_W;

	if(boolRight)
		return GraphHitTest_E;

	if(boolUp)
		return GraphHitTest_N;
	
	if(boolDown)
		return GraphHitTest_S;
	//////////////////////////////////////////////////////////////////

	// 09.11.18将整个扫描区域内外的单击测试类型区别开
	// 本次修改前代码副本
	/*------------------------------------------------- 
	return GraphHitTest_Normal;
	-------------------------------------------------*/

	// 本次修改后代码
	//-------------------------------------------------
	if( ptGraphPos.x < nWidth && ptGraphPos.y < nHeight )
		return GraphHitTest_Normal;
	return GraphHitTest_NULL;
	//-------------------------------------------------
}

void CScanView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if ( m_bCatch )
	{
/*		if ( (nFlags & MK_LBUTTON) != MK_LBUTTON )		//09.03.20
		{
			m_bCatch = FALSE;
			ReleaseCapture();
			UpdateScanParam( point );
			return;
		}//SemCtrlMouse_Close
*/
		//=====2004-5-13_屏幕鼠标控制电镜=================
		//SemCtrlMouse_Update
		if( (nFlags & MK_LBUTTON) == MK_LBUTTON )		//09.03.20
		{
			if ( m_nGraphHitTestSave == GraphHitTest_Normal )
			{
				switch( m_nAdjustorSort )				//根据调节器类型设置光标
				{
				case	SORT_BEAM_ALIGNMENT:
				case	SORT_SCAN_SHIFT:
				case	SORT_BEAM_ROTATE:
				case	SORT_CONDENSER_LENS:
				case	SORT_OBJECTIVE_LENS:
				case	SORT_BRIGHT:
				case	SORT_CONTRAST:
				case	SORT_STIGMATOR:
					SemCtrlMouse_Update( ScreenToGraph( point ), m_nAdjustorSort );
					return;
					break;
				}
			}
		}
		else if( (nFlags & MK_RBUTTON) == MK_RBUTTON )	//09.03.20
		{
			if ( m_nGraphHitTestSave == GraphHitTest_Normal )
			{
				SemCtrlMouse_Update( ScreenToGraph( point ), SORT_STIGMATOR );
				return;
			}
		}
		else if( (nFlags & MK_MBUTTON) == MK_MBUTTON )
		{
			// 按下中键且拖动，在样品台跟踪模式下，调整跟踪参数
			if( m_bStageTrack && 
				( (theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.xy.bReady)
				|| (theApp.m_Stage.nHasAutoStage > 1 && StageMC600_IsReady()) ) )
				SetStageTrackParam( point );
			return;
		}
		USB_SetScreenMouseCtrl( point );
	}
	else
	{
		if ( (nFlags & MK_LBUTTON) == MK_LBUTTON )
		{
			m_ptPosSave				= ScreenToGraph( point );
			m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );

			//======2004-5-13_屏幕鼠标控制电镜=====================
			// 修改标示：2004-5-13_屏幕鼠标控制电镜
			// 修改日期：2004年5月13日
			// 修改人：罗明华
			// 修改原因：添加屏幕鼠标操作电镜控制。

			// 本次修改前代码副本
			/*----------------------------------------------------- 
			if ( m_nGraphHitTestSave != GraphHitTest_Normal )
			{
				m_bCatch = TRUE;
				m_rcDragRectLast = CRect(0,0,0,0);
				SetCapture();
			}
			-----------------------------------------------------*/

			// 本次修改后代码
			//-----------------------------------------------------
			//SemCtrlMouse_Init
			if ( m_bAbc != TRUE )
			{
				if ( m_nGraphHitTestSave == GraphHitTest_Normal )
				{
					SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_OBJECTIVE_LENS ) ) );
					m_rcDragRectLast = CRect(0,0,0,0);
					SetCapture();

					SemCtrlMouse_Init( m_ptPosSave, m_nAdjustorSort );
				}
				if ( m_nGraphHitTestSave != GraphHitTest_NULL )
					m_bCatch = TRUE;
			}
			//-----------------------------------------------------
			//======2004-5-13_屏幕鼠标控制电镜=====================
		}
		else if( (nFlags & MK_RBUTTON) == MK_RBUTTON )	//09.03.20
		{
			m_ptPosSave				= ScreenToGraph( point );
			m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );
			//SemCtrlMouse_Init
			if ( m_bAbc != TRUE )
			{
				if ( m_nGraphHitTestSave == GraphHitTest_Normal )
				{
					SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE( IDC_CURSOR_STIGMATOR ) ) );
					m_rcDragRectLast = CRect(0,0,0,0);
					SetCapture();

					SemCtrlMouse_Init( m_ptPosSave, SORT_STIGMATOR );
				}
				if ( m_nGraphHitTestSave != GraphHitTest_NULL )
					m_bCatch = TRUE;
			}
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CScanView::USB_SetScreenMouseCtrl( CPoint point )
{
	//=====2004-5-13_屏幕鼠标控制电镜=================

	if ( m_bOscillographHitTest )
	{
		SetOscillographParam(point);
		return;
	}

	if ( m_bUScaleHitTest )
	{
		SetUScaleParam(point);
		return;
	}
	
	if( m_bStageOriginHitTest )
	{
		SetStagePointParam( point );
		return;
	}

	// 16.06.12 多移动标尺实验
	int i;
	for( i=0; i<MobileUScaleCounts; i++ )
	{
		if( m_bUScaleHitTestArray[i] )
			SetUScaleParamArray( i, point );
	}
	// 16.06.12 多移动标尺实验

	int nScanType = m_nusbScanType;
	/////////////////////////////////////////////

	if ( nScanType == USB_ST_SpotScan )
	{
		SetPointScanParam(point);
	}
	else if ( nScanType == USB_ST_LineScan_HORZ
		|| nScanType == USB_ST_LineScan_VERT )
	{
		SetLineScanParam(point);
	}
	else if ( nScanType == USB_ST_AreaScan )
	{
		SetActiveAreaParam(point);
	}
/*	else if ( nScanType == USB_ST_DualMagnification )
	{
		SetDualMagParam(point);		
	}*/
	/////////////////////////////////////////////
}

double& GetThirdSignificandWeight( double dblValue, int nPower = 1, BOOL boolIncrease = TRUE )
{
	static double dblSignificandWeight;

	if ( boolIncrease )
	{
		if (dblValue < 10.0)
			dblSignificandWeight = 0.01 * nPower;
		else if (dblValue < 100.0)
			dblSignificandWeight = 0.1 * nPower;
		else if (dblValue < 1000.0)
			dblSignificandWeight = 1.0 * nPower;
		else if (dblValue < 10000.0)
			dblSignificandWeight = 10.0 * nPower;
		else if (dblValue < 100000.0)
			dblSignificandWeight = 100.0 * nPower;
		else
			dblSignificandWeight = 1000.0 * nPower;
	}
	else
	{
		if (dblValue < 10.0 + 0.1 * nPower)
		{
			if (dblValue >= 10.0)
				dblSignificandWeight = dblValue - 9.99;
			else
				dblSignificandWeight = 0.01 * nPower;
		}
		else if (dblValue < 100.0 + 1.0 * nPower)
		{
			if (dblValue >= 100.0)
				dblSignificandWeight = dblValue - 99.9;
			else
				dblSignificandWeight = 0.1 * nPower;
		}
		else if (dblValue < 1000.0 + 10.0 * nPower)
		{
			if (dblValue >= 1000.0)
				dblSignificandWeight = dblValue - 999.0;
			else
				dblSignificandWeight = 1.0 * nPower;
		}
		else if (dblValue < 10000.0 + 100.0 * nPower)
		{
			if (dblValue >= 10000.0)
				dblSignificandWeight = dblValue - 9990.0;
			else
				dblSignificandWeight = 10.0 * nPower;
		}
		else if (dblValue < 100000.0 + 1000.0 * nPower)
		{
			if (dblValue >= 100000.0)
				dblSignificandWeight = dblValue - 99900.0;
			else
				dblSignificandWeight = 100.0 * nPower;
		}
		else
			dblSignificandWeight = 1000.0 * nPower;
	}

	return dblSignificandWeight;
}

// 2009.03.20 鼠标滚轮消息响应
BOOL CScanView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag, varData;
	ClassID.lVal = SemControlPanelClass_Magnifier;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	double sblStep;

	varData = theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos();
	sblStep = varData.dblVal;
	if( zDelta > 0 )		// Zoom In
	{
		sblStep = GetThirdSignificandWeight( varData.dblVal, 10 );
		varData.dblVal += sblStep;
	}
	else if( zDelta < 0 )	// Zoom Out
	{
		sblStep = GetThirdSignificandWeight( varData.dblVal, 10, FALSE );
		varData.dblVal -= sblStep;
	}
	Xpos = varData;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	theApp.DisplayMsg("Mouse wheel");
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CScanView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		m_bCatch = FALSE;
		ReleaseCapture();
		UpdateScanParam( point );
		return;
	}
	
	GraphTextEdit( point );

	CScrollView::OnLButtonUp(nFlags, point);
}

//09.03.20
void CScanView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		m_bCatch = FALSE;
		ReleaseCapture();
		UpdateScanParam( point );
		return;
	}

	CScrollView::OnRButtonUp(nFlags, point);
}

void CScanView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if( theApp.m_Stage.nHasAutoStage < 1 || (!theApp.m_Stage.xy.bReady && !StageMC600_IsReady()) )
	{
		CMenu menuManual;
		// Create a new menu for the application window.
		VERIFY(menuManual.CreatePopupMenu());

// 		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_ObjectiveLens, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_ObjectiveLens));
// 		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_ObjectiveLens2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_ObjectiveLens2));
// 		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_CondenserLens, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_CondenserLens));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_CondenserLens2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_CondenserLens2));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorX, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorX));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorX2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorX2));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorY, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorY));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_StigmatorY2, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_StigmatorY2));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_ScanShiftX, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_ScanShiftX));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_ScanShiftY, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_ScanShiftY));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_BeamAlignmentX, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_BeamAlignmentX));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_BeamAlignmentY, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_BeamAlignmentY));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_Brightness, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_Brightness));
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_Contrast, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_Contrast));
		menuManual.AppendMenu(MF_SEPARATOR);
		menuManual.AppendMenu(MF_STRING, ID_ADJUSTOR_MANUAL_NULL, (LPCTSTR)GetResString(IDS_MENUITEM_ADJUSTOR_MANUAL_NULL));

		menuManual.CheckMenuItem( ID_ADJUSTOR_MANUAL_ObjectiveLens +theApp.m_nAdjustorManualSort,
			MF_BYCOMMAND | MF_CHECKED );

		CPoint temppoint = point;
		ClientToScreen( &temppoint );
		menuManual.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			temppoint.x, temppoint.y, this, NULL);
		CScrollView::OnMButtonDown(nFlags, point);
		return;
	}

	m_bStageTrack	= TRUE;
	m_bCatch		= TRUE;
	SetCapture();

	m_ptPosSave				= ScreenToGraph( point );
	m_nGraphHitTestSave		= USB_GraphHitTest( m_ptPosSave );
	m_ptStageTrackParam		= m_ptPosSave;

	// 画一个标识，准备进入跟踪模式
	CPoint ptPoint	= m_ptStageTrackParam;
	if( ptPoint.x < nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.x = nSTAGE_TRACE_POINT_RADIUS;
	if( ptPoint.x > m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.x = m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS;
	if( ptPoint.y < nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.y = nSTAGE_TRACE_POINT_RADIUS;
	if( ptPoint.y > m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS )
		ptPoint.y = m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS;
	theApp.m_ImageCard.GraphDrawCircle( ptPoint, m_ptPosSave, FALSE, nSTAGE_TRACE_POINT_RADIUS, RGB(0, 255, 0) );

	// 设置初始跟踪步距
	double dMag = theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
	if( theApp.m_Stage.xy.bReady )
	{
		if( dMag < 1000 )
		{
			m_nStageTrackStepX = 50;
			m_nStageTrackStepY = 50;
		}
		else if( dMag < 5000 )
		{
			m_nStageTrackStepX = 20;
			m_nStageTrackStepY = 20;
		}
		else
		{
			m_nStageTrackStepX = 10;
			m_nStageTrackStepY = 10;
		}

		char szBuffer[64];
		CString str;
		// 跟踪
		str.Format( "JG %ld,%ld;BGXY", m_nStageTrackStepX, m_nStageTrackStepY );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		// 开启跟踪保护
		SetTimer( nAutoStageTrack, nAutoStageXY_FIElapse, NULL );
	}
	else
	{
		if( dMag < 1000 )
		{
			m_nStageTrackStepX = 50;
			m_nStageTrackStepY = 50;
		}
		else if( dMag < 5000 )
		{
			m_nStageTrackStepX = 20;
			m_nStageTrackStepY = 20;
		}
		else
		{
			m_nStageTrackStepX = 10;
			m_nStageTrackStepY = 10;
		}
		m_nStageTrackStepX = 0;
		m_nStageTrackStepY = 0;
		StageMC600_SetTrackParam( m_nStageTrackStepX, m_nStageTrackStepY );
	}

	CScrollView::OnMButtonDown(nFlags, point);
}

void CScanView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		if( theApp.m_Stage.nHasAutoStage < 2 && theApp.m_Stage.xy.bReady )
		{
			// 停止样品台移动
			theApp.m_DMCWin.Command( "ST" );
			theApp.DisplayMsg( "ST in OnMButtonUp" );
			KillTimer( nAutoStageTrack );
		}
		else if( theApp.m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
		{
			StageMC600_StopTrack();
			theApp.DisplayMsg( "Stop in OnMButtonUp" );
		}
		m_bCatch = FALSE;
		ReleaseCapture();
		m_bStageTrack = FALSE;

		// 擦除标识，退出跟踪模式
		CPoint ptPoint	= m_ptStageTrackParam;
		if( ptPoint.x < nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.x = nSTAGE_TRACE_POINT_RADIUS;
		if( ptPoint.x > m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.x = m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS;
		if( ptPoint.y < nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.y = nSTAGE_TRACE_POINT_RADIUS;
		if( ptPoint.y > m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS )
			ptPoint.y = m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS;
		CRect rect(ptPoint.x-nSTAGE_TRACE_POINT_RADIUS,ptPoint.y-nSTAGE_TRACE_POINT_RADIUS,
			ptPoint.x+nSTAGE_TRACE_POINT_RADIUS,ptPoint.y+nSTAGE_TRACE_POINT_RADIUS);
		theApp.m_ImageCard.GraphDrawCircle( ptPoint, m_ptPosSave );
		theApp.m_ImageCard.GraphDrawCircle( ptPoint, m_ptPosSave, FALSE );
		theApp.m_ImageCard.GraphClear( rect, RGB(0,0,0) );
		return;
	}
	CScrollView::OnMButtonUp(nFlags, point);
}

void CScanView::UpdateScanParam( CPoint point )
{
	if ( m_bOscillographHitTest )
		return;

	if ( m_bUScaleHitTest )
		return;

	if ( m_bStageOriginHitTest )
	{
		m_ptStagePointParam = m_rcStageDragRectLast.TopLeft();
		m_ptStagePointParam.Offset(20,20);
		return;
	}

	if ( m_rcDragRectLast == CRect(0,0,0,0) )
	{
		return;
	}

	int nScanType = m_nusbScanType;
	/////////////////////////////////////////////
	
	if ( nScanType == USB_ST_SpotScan )
	{
		m_ptPointScanParam = m_rcDragRectLast.TopLeft();
		//======2006.09.04_点扫描点的画法=================
		// 修改标示：2006.09.04_点扫描点的画法
		// 修改日期：2006.09.04
		// 修改人  ：J.Y.Ma
		// 修改原因：点扫描点过小，且颜色不够醒目，不易寻找

		// 本次修改前代码副本
		/*----------------------------------------------------- 
		m_ptPointScanParam.Offset(2,2);
		-----------------------------------------------------*/

		// 本次修改后代码
		//-----------------------------------------------------
		m_ptPointScanParam.Offset(6,6);
		//======2006.09.04_点扫描点的画法=================
	}
	else if ( nScanType == USB_ST_LineScan_HORZ )
	{
		m_nLineScanParam = m_rcDragRectLast.top;
	}
	else if ( nScanType == USB_ST_LineScan_VERT )
	{
		m_nLineScanParam = m_rcDragRectLast.left;
	}
	else if ( nScanType == USB_ST_AreaScan )
	{
		m_rcAreaScanParam = m_rcDragRectLast;
	}
/*	else if ( nScanType == USB_ST_DualMagnification )
	{
		m_rcDualMagParam = m_rcDragRectLast;
	}*/
	USB_ChangeScan( nScanType );
}

void CScanView::SetPointScanParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////

	CPoint ptPoint	= m_ptPointScanParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	ptPoint.x += nOffsetX;
	ptPoint.y += nOffsetY;

	//======2006.09.04_点扫描点的画法=================
	// 修改标示：2006.09.04_点扫描点的画法
	// 修改日期：2006.09.04
	// 修改人  ：J.Y.Ma
	// 修改原因：点扫描点过小，且颜色不够醒目，不易寻找

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	CRect rect(ptPoint.x-2,ptPoint.y-2,ptPoint.x+3,ptPoint.y+3);
	theApp.m_ImageCard.GraphClear( m_rcDragRectLast );
	theApp.m_ImageCard.GraphClear( rect, RGB(255,0,0) );
	-----------------------------------------------------*/

	// 本次修改后代码
	//-----------------------------------------------------
	CRect rect(ptPoint.x-6,ptPoint.y-6,ptPoint.x+8,ptPoint.y+8);
	theApp.m_ImageCard.GraphClear( m_rcDragRectLast );
	theApp.m_ImageCard.GraphClear( rect, RGB(0,0,0) );

	ptPos0.x = m_rcDragRectLast.left+6;
	ptPos0.y = m_rcDragRectLast.top+6;
	ptPos1 = ptPos0;
	ptPos0.x -= 6;
	ptPos1.x += 8;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1 );

	ptPos0.x = m_rcDragRectLast.left+6;
	ptPos0.y = m_rcDragRectLast.top+6;
	ptPos1 = ptPos0;
	ptPos0.y -= 6;
	ptPos1.y += 8;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1 );

	/////////////////////////////////////////////////////////////
	ptPos0.x = rect.left+6;
	ptPos0.y = rect.top+6;
	ptPos1 = ptPos0;
	ptPos0.x -= 6;
	ptPos1.x += 8;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

	ptPos0.y += 1;
	ptPos1.y += 1;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );
	/////////////////////////////////////////////////////////////
	ptPos0.x = rect.left+6;
	ptPos0.y = rect.top+6;
	ptPos1 = ptPos0;

	ptPos0.y -= 6;
	ptPos1.y += 8;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

	ptPos0.x += 1;
	ptPos1.x += 1;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );
	//======2006.09.04_点扫描点的画法=================

	if( theApp.m_dwOpr > 0 && ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL )
	{
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotX( ptPoint.x, FALSE );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotY( ptPoint.y, FALSE );
	}
	m_rcDragRectLast = rect;
}

void CScanView::SetLineScanParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////

	CRect rect;
	int nScanType = m_nusbScanType;
	if ( nScanType == USB_ST_LineScan_HORZ )
	{
		long nLineScanY		= m_nLineScanParam;
		long nLineOffset	= ptPos1.y - ptPos0.y;
		long nLineScanY_New = nLineScanY + nLineOffset;
		rect = CRect(0,nLineScanY_New,m_sizeReso.cx,nLineScanY_New+1);
		if( theApp.m_dwOpr > 0 && ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL )
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotY( nLineScanY_New, FALSE );
	}
	else
	{
		long nLineScanX		= m_nLineScanParam;
		long nLineOffset	= ptPos1.x - ptPos0.x;
		long nLineScanX_New = nLineScanX + nLineOffset;
		rect = CRect(nLineScanX_New, 0, nLineScanX_New+1, m_sizeReso.cy);
		if( theApp.m_dwOpr > 0 && ((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL )
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelMCI.UpdateSpotX( nLineScanX_New, FALSE );
	}

	theApp.m_ImageCard.GraphClear( m_rcDragRectLast );
	theApp.m_ImageCard.GraphClear( rect, RGB(255,0,0) );
	m_rcDragRectLast = rect;

	//CSize	size(1024,1);
	//GetDC()->DrawDragRect(rect, size, m_rcDragRectLast, m_szDragSizeLast);
	//m_szDragSizeLast = size;
	//m_rcDragRectLast = rect;
}

void CScanView::SetLineScanParamManually( BOOL bHORZ, int nPos )
{
	CRect rect;
	if ( bHORZ )
		rect = CRect(0, nPos, m_sizeReso.cx, nPos+1);
	else
		rect = CRect(nPos, 0, nPos+1, m_sizeReso.cy);

	theApp.m_ImageCard.GraphClear( m_rcDragRectLast );
	theApp.m_ImageCard.GraphClear( rect, RGB(255,0,0) );
	m_rcDragRectLast = rect;
	m_nLineScanParam = nPos;
}

void CScanView::SetActiveAreaParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////

	const long AreaMinW = 96;
	const long AreaMinH = 96;
	CRect rcArea	= m_rcAreaScanParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	CRect rcAreaNew = rcArea;
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		if ( (rcAreaNew.left + nOffsetX) < 0 )
			nOffsetX = - rcAreaNew.left;
		if ( (rcAreaNew.right + nOffsetX) > m_sizeReso.cx )
			nOffsetX = m_sizeReso.cx - rcAreaNew.right;

		if ( (rcAreaNew.top + nOffsetY) < 0 )
			nOffsetY = - rcAreaNew.top;
		if ( (rcAreaNew.bottom + nOffsetY) > m_sizeReso.cy )
			nOffsetY = m_sizeReso.cy - rcAreaNew.bottom;

		rcAreaNew.OffsetRect(nOffsetX, nOffsetY);
		break;

	case GraphHitTest_E:
		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}
		break;

	case GraphHitTest_W:
		rcAreaNew.left += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.left = rcAreaNew.right - AreaMinW;
		}
		break;

	case GraphHitTest_N:
		rcAreaNew.top += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.top = rcAreaNew.bottom - AreaMinH;
		}
		break;

	case GraphHitTest_S:
		rcAreaNew.bottom += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.bottom = rcAreaNew.top + AreaMinH;
		}
		break;

	case GraphHitTest_NE:
		rcAreaNew.top += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.top = rcAreaNew.bottom - AreaMinH;
		}
		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}
		break;

	case GraphHitTest_SE:
		rcAreaNew.bottom += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.bottom = rcAreaNew.top + AreaMinH;
		}
		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}
		break;

	case GraphHitTest_SW:
		rcAreaNew.top += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.top = rcAreaNew.bottom - AreaMinH;
		}
		rcAreaNew.left += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.left = rcAreaNew.right - AreaMinW;
		}
		break;

	case GraphHitTest_NW:
		rcAreaNew.bottom += nOffsetY;
		if ( rcAreaNew.Height() < AreaMinH )
		{
			rcAreaNew.bottom = rcAreaNew.top + AreaMinH;
		}
		rcAreaNew.left += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.left = rcAreaNew.right - AreaMinW;
		}
		break;

	default:
		break;
	}

	theApp.m_ImageCard.GraphDrawRect( m_rcDragRectLast );
	theApp.m_ImageCard.GraphDrawRect( rcAreaNew, RGB(255,0,0) );
	m_rcDragRectLast = rcAreaNew;
}

void CScanView::SetDualMagParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////

	long AreaMinW	= m_sizeReso.cx /8;
	long AreaMinH	= m_sizeReso.cx /8;
	CRect rcArea	= m_rcDualMagParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;
	int	nWidth		= m_sizeReso.cx /2;
	int nHeight		= m_sizeReso.cy;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	CRect rcAreaNew = rcArea;
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		if ( (rcAreaNew.left + nOffsetX) < 0 )
			nOffsetX = - rcAreaNew.left;
		if ( (rcAreaNew.right + nOffsetX) > nWidth )
			nOffsetX = nWidth - rcAreaNew.right;

		if ( (rcAreaNew.top + nOffsetY) < 0 )
			nOffsetY = - rcAreaNew.top;
		if ( (rcAreaNew.bottom + nOffsetY) > nHeight )
			nOffsetY = nHeight - rcAreaNew.bottom;

		rcAreaNew.OffsetRect(nOffsetX, nOffsetY);
		break;

	case GraphHitTest_E:
		if ( (rcAreaNew.right + nOffsetX) > nWidth )
			nOffsetX = nWidth - rcAreaNew.right;

		rcAreaNew.right += nOffsetX;
		if ( rcAreaNew.Width() < AreaMinW )
		{
			rcAreaNew.right = rcAreaNew.left + AreaMinW;
		}

		rcAreaNew.bottom = rcAreaNew.top + rcAreaNew.Width() *2;
		if ( rcAreaNew.bottom > nHeight )
		{
			rcAreaNew.bottom = nHeight;
			rcAreaNew.right = rcAreaNew.left + rcAreaNew.Height() / 2;
		}
		break;

	default:
		break;
	}

	theApp.m_ImageCard.GraphDrawRect( m_rcDragRectLast );
	theApp.m_ImageCard.GraphDrawRect( rcAreaNew, RGB(255,0,0) );
	m_rcDragRectLast = rcAreaNew;
}

void CScanView::SetUScaleParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////
	
	// 07.07---07.08加入任意方向标尺
	// 本次修改前代码副本
	/*------------------------------------------------- 
	const long UScaleMin = 32;	//64;
	const long UScaleMax = 1024;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint ptPosNew = m_ptMobileUScale;
	int	nLength	= m_nMobileUScalePixelNum;

//	nOffsetX = nOffsetX / 4 * 4;
//	nOffsetY = nOffsetY / 4 * 4;

	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		ptPosNew.x += nOffsetX;
		ptPosNew.y += nOffsetY;
		break;

	case GraphHitTest_W:
		if ( (nLength - nOffsetX) < UScaleMin )
		{
			nOffsetX = -(UScaleMin - nLength);
		}
		else if ( (nLength - nOffsetX) > UScaleMax )
		{
			nOffsetX = -(UScaleMax - nLength);
		}
		ptPosNew.x += nOffsetX;
		nLength -= nOffsetX;
		break;

	case GraphHitTest_E:
		if ( (nLength + nOffsetX) < UScaleMin )
		{
			nOffsetX = UScaleMin - nLength;
		}
		else if ( (nLength + nOffsetX) > UScaleMax )
		{
			nOffsetX = UScaleMax - nLength;
		}
		nLength += nOffsetX;
		break;

	default:
		break;
	}

	theApp.m_ImageCard.SetMobileUScalePos( ptPosNew );
	if ( nLength != m_nMobileUScalePixelNum )
	{
		//05.08.16移动标尺的刷新
		theApp.m_csMobileUScaleText = CalculateMobileUScale( nLength );
		theApp.m_ImageCard.UpdateUScale(nLength, theApp.m_csMobileUScaleText);
	}
	-------------------------------------------------*/

	// 本次修改后代码
	//-------------------------------------------------
	int	nLength		= 0;//sqrt( pow( (m_ptMobileUScale.x-m_ptMobileUScale2.x), 2) + pow( (m_ptMobileUScale.y-m_ptMobileUScale2.y), 2 ) ) +0.5;

	AdjustPos( ptPos1 );

	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint pt1 = m_ptMobileUScale;
	CPoint pt2 = m_ptMobileUScale2;
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		pt1.x += nOffsetX;
		pt1.y += nOffsetY;
		pt2.x += nOffsetX;
		pt2.y += nOffsetY;
		AdjustUScaleLength( pt1, pt2, TRUE );
		break;

	case GraphHitTest_W:
		pt1 = ptPos1;
		AdjustUScaleLength( pt1, pt2 );
		break;

	case GraphHitTest_E:
		pt2 = ptPos1;
		AdjustUScaleLength( pt2, pt1 );
		break;

	default:
		break;
	}
	AdjustPos( pt1 );
	AdjustPos( pt2 );

	nLength = (int)(sqrt( pow((double) (pt1.x - pt2.x), 2) + pow((double)(pt1.y - pt2.y), 2 ) ) +0.5);
	if( /*m_bStageOrigin && */m_bDebugCoadjust )
	{
		// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
		// 根据标尺长度，将标尺移到以参考点为中心、以标尺长度为边长的正方形的下底边
		pt1.x	= m_sizeReso.cx /2 -nLength /2;
		pt2.x	= pt1.x +nLength;
		pt1.y	= m_sizeReso.cy /2 +nLength /2;
		pt2.y	= pt1.y;
	}
	m_ptMobileUScale = pt1;
	m_ptMobileUScale2 = pt2;

	theApp.m_ImageCard.SetMobileUScalePosEx( m_ptMobileUScale, m_ptMobileUScale2, RGB(255, 0, 0) );
	//05.08.16移动标尺的刷新
	theApp.m_csMobileUScaleText = CalculateMobileUScale( (int)(nLength));// *1024.0 /m_sizeReso.cx) );
	theApp.m_ImageCard.UpdateUScale(nLength, theApp.m_csMobileUScaleText);
	m_ptPosSave = ptPos1;
	//-------------------------------------------------

	// 2005-1-17_放大倍数XY联合校正
	///////////////////////////////////////////////////////////
	CPoint ptPosNew = m_ptMobileUScale;
	CRect rcAreaNew;
	rcAreaNew.left		= ptPosNew.x;
	rcAreaNew.top		= ptPosNew.y - nLength - 1;
	rcAreaNew.right		= ptPosNew.x + nLength + 1;
	rcAreaNew.bottom	= ptPosNew.y;

	if ( theApp.m_boolShowUScaleXY )
	{
		theApp.m_ImageCard.GraphDrawRectEx( m_bDrawCircle, m_rcUScaleAreaLast );
		theApp.m_ImageCard.GraphDrawRectEx( m_bDrawCircle, rcAreaNew, RGB(0,255,0) );
		m_boolUScaleAreaIsDraw = TRUE;
	}
	else if ( m_boolUScaleAreaIsDraw )
	{
		theApp.m_ImageCard.GraphDrawRect( m_rcUScaleAreaLast );
		m_boolUScaleAreaIsDraw = FALSE;
	}
	m_rcUScaleAreaLast = rcAreaNew;
}

// 16.06.12 多移动标尺实验
void CScanView::SetUScaleParamArray(int nIndex, CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////
	
	AdjustPos( ptPos1 );

	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint pt1 = m_ptMobileUScaleArray[nIndex];
	CPoint pt2 = m_ptMobileUScaleArray2[nIndex];
	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		pt1.x += nOffsetX;
		pt1.y += nOffsetY;
		pt2.x += nOffsetX;
		pt2.y += nOffsetY;
		AdjustUScaleLength( pt1, pt2, TRUE );
		break;

	case GraphHitTest_W:
		pt1 = ptPos1;
		AdjustUScaleLength( pt1, pt2 );
		break;

	case GraphHitTest_E:
		pt2 = ptPos1;
		AdjustUScaleLength( pt2, pt1 );
		break;

	default:
		break;
	}
	AdjustPos( pt1 );
	AdjustPos( pt2 );

	m_nMobileUScalePixelNumArray[nIndex] = (int)(sqrt( pow( (double)(pt1.x - pt2.x), 2) + pow( (double)(pt1.y - pt2.y), 2 ) ) +0.5);
	m_ptMobileUScaleArray[nIndex] = pt1;
	m_ptMobileUScaleArray2[nIndex] = pt2;

	theApp.m_ImageCard.SetMobileUScalePosArray( nIndex, m_ptMobileUScaleArray[nIndex], m_ptMobileUScaleArray2[nIndex], RGB(255, 0, 0) );
	CString csMobileUScaleText = CalculateMobileUScale( m_nMobileUScalePixelNumArray[nIndex] );
	theApp.m_ImageCard.UpdateUScaleArray(nIndex, m_nMobileUScalePixelNumArray[nIndex], csMobileUScaleText);
	m_ptPosSave = ptPos1;
//	m_rcUScaleAreaLast = rcAreaNew;
}
// 16.06.12 多移动标尺实验

void CScanView::SetUScaleParamByDefine( double dScale )
{
	// 由欲标注的尺寸得到标尺像素长度
	int nScalePixels = (int)( CalculateMobileUScale( dScale ) *m_sizeReso.cx /1024.0 );
	if( nScalePixels < 16 )
		return;
	if( m_ptMobileUScale.x + nScalePixels > m_sizeReso.cx )
		return;
	// 以m_ptMobileUScale为起点的向右的水平线,得到m_ptMobileUScale2的坐标
	m_ptMobileUScale2.x = m_ptMobileUScale.x + nScalePixels;
	m_ptMobileUScale2.y = m_ptMobileUScale.y;
	// 反送到显示层刷新标尺
	theApp.m_ImageCard.SetMobileUScalePosEx( m_ptMobileUScale, m_ptMobileUScale2, RGB(255, 0, 0) );
	theApp.m_csMobileUScaleText = CalculateMobileUScale( (int)(nScalePixels *1024.0 / m_sizeReso.cx) );
	theApp.m_ImageCard.UpdateUScale(nScalePixels, theApp.m_csMobileUScaleText);
}

void CScanView::AdjustPos( CPoint& ptPos )
{
	if( ptPos.x < 0 )
		ptPos.x = 0;
	if( ptPos.y < 0 )
		ptPos.y = 0;
	if( ptPos.x > m_sizeReso.cx )
		ptPos.x = m_sizeReso.cx;
	if( ptPos.y > m_sizeReso.cy )
		ptPos.y = m_sizeReso.cy;
}

void CScanView::AdjustUScaleLength( CPoint& ptStart, CPoint& ptEnd, BOOL bTranslation )
{
	CPoint pt1 = ptStart;	//动的点
	CPoint pt2 = ptEnd;		//不动的点

	double k, b;//斜率与截距
	int nLength = 0;

	const long UScaleMin = 16;
	int	nWidth		= m_sizeReso.cx;
	int nHeight		= m_sizeReso.cy;

	AdjustPos( pt1 );
	AdjustPos( pt2 );

	if( pt1.x == pt2.x )		//垂直线，无斜率
	{
		nLength = abs( pt1.y - pt2.y );
		if( nLength < UScaleMin )
		{
			if( bTranslation )	//平移时
			{
				if( pt1.y < pt2.y && pt1.y == 0 )
					pt2.y = pt1.y + UScaleMin;
				else if( pt1.y < pt2.y && pt2.y == nHeight )
					pt1.y = pt2.y - UScaleMin;
				else if( pt1.y >= pt2.y && pt2.y == 0 )
					pt1.y = pt2.y + UScaleMin;
				else if( pt1.y >= pt2.y && pt1.y == nHeight )
					pt2.y = pt1.y - UScaleMin;
			}
			else
			{
				if( pt1.y > pt2.y )
					pt1.y = pt2.y + UScaleMin;
				else
					pt1.y = pt2.y - UScaleMin;
			}
		}
		else if( nLength > nHeight )
		{
			if( pt1.y > pt2.y )
				pt1.y = nHeight;
			else
				pt1.y = 0;
		}
	}
	else if( pt1.y == pt2.y )	//水平线
	{
		nLength = abs( pt1.x - pt2.x );
		if( nLength < UScaleMin )
		{
			if( bTranslation )	//平移时
			{
				if( pt1.x < pt2.x && pt1.x == 0 )
					pt2.x = pt1.x + UScaleMin;
				else if( pt1.x < pt2.x && pt2.x == nWidth )
					pt1.x = pt2.x - UScaleMin;
				else if( pt1.x >= pt2.x && pt2.x == 0 )
					pt1.x = pt2.x + UScaleMin;
				else if( pt1.x >= pt2.x && pt1.x == nWidth )
					pt2.x = pt1.x - UScaleMin;
/*				if( pt1.x > pt2.x )
					pt2.x = pt1.x - UScaleMin;
				else
					pt2.x = pt1.x + UScaleMin;
*/			}
			else
			{
				if( pt1.x > pt2.x )
					pt1.x = pt2.x + UScaleMin;
				else
					pt1.x = pt2.x - UScaleMin;
			}
		}
		else if( nLength > nWidth )
		{
			if( pt1.x > pt2.x )
				pt1.x = nWidth;
			else
				pt1.x = 0;
		}
	}
	else
	{
		nLength = (int)(sqrt( pow((double) (pt1.x-pt2.x), 2) + pow( (double)(pt1.y-pt2.y), 2 ) ) +0.5);
		if( nLength < UScaleMin )
		{
			k = 1. * (pt1.y - pt2.y) / (pt1.x - pt2.x);
			b = pt1.y - k * pt1.x;
			if( (pt1.x > pt2.x)  && (pt1.y > pt2.y) && (k < 1) )
			{
				pt1.x = pt2.x + UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x > pt2.x) && (pt1.y > pt2.y) && (k == 1) )
			{
				pt1.x = (long)(pt2.x + UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x > pt2.x) && (pt1.y > pt2.y) && (k > 1) )
			{
				pt1.y = pt2.y + UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
			else if( (pt1.x > pt2.x) && (pt1.y < pt2.y) && (k < -1) )
			{
				pt1.y = pt2.y - UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
			else if( (pt1.x > pt2.x) && (pt1.y < pt2.y) && (k == -1) )
			{
				pt1.x = (long)(pt2.x + UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x > pt2.x) && (pt1.y < pt2.y) && (k > -1) )
			{
				pt1.x = pt2.x + UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y > pt2.y) && (k < -1) )
			{
				pt1.y = pt2.y + UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
			else if( (pt1.x < pt2.x) && (pt1.y > pt2.y) && (k == -1) )
			{
				pt1.x = (long)(pt2.x - UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y > pt2.y) && (k > -1) )
			{
				pt1.x = pt2.x - UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y < pt2.y) && (k < 1) )
			{
				pt1.x = pt2.x - UScaleMin;
				pt1.y = (long)(k * pt1.x + b);
			}
			else if( (pt1.x < pt2.x) && (pt1.y < pt2.y) && (k == 1) )
			{
				pt1.x = (long)(pt2.x - UScaleMin / sqrt((double)2));
				pt1.y = (long)(k * pt1.x + b);
			}
			else
			{
				pt1.y = pt2.y - UScaleMin;
				pt1.x = (long)((pt1.y - b) / k);
			}
		}
	}
	ptStart = pt1;
	ptEnd	= pt2;
}

// 2005-1-17_放大倍数XY联合校正
void CScanView::UpdateUScaleXY()
{
	if ( m_bUScale )
	{
		if ( theApp.m_boolShowUScaleXY )
		{
			if( m_rcUScaleAreaLast.IsRectEmpty() )
				SetUScaleParam( CPoint(100, 400) );
			theApp.m_ImageCard.GraphDrawRectEx( m_bDrawCircle, m_rcUScaleAreaLast, RGB(0,255,0) );
			m_boolUScaleAreaIsDraw = TRUE;
		}
		else
		{
			theApp.m_ImageCard.GraphDrawRectEx( m_bDrawCircle, m_rcUScaleAreaLast );
			m_boolUScaleAreaIsDraw = FALSE;
		}
	}
	else if ( m_boolUScaleAreaIsDraw )
	{
		theApp.m_ImageCard.GraphDrawRect( m_rcUScaleAreaLast );
		m_boolUScaleAreaIsDraw = FALSE;
	}
}

void CScanView::SetOscillographParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////
	
	long nOffsetY	= ptPos1.y - ptPos0.y;

	CPoint ptPosNew = m_ptOscillograph;

	switch ( m_nGraphHitTestSave )
	{
	case GraphHitTest_In:
		ptPosNew.y += nOffsetY;
		break;

	default:
		break;
	}
	//08.04.24保证示波器不出扫描区域
	if( ptPosNew.y < 0 )
		ptPosNew.y = 0;
	if( ptPosNew.y > m_sizeReso.cy-g_szUSB_FrameOscillograph.cy )
		ptPosNew.y = m_sizeReso.cy-g_szUSB_FrameOscillograph.cy;
	//08.04.24保证示波器不出扫描区域

	theApp.m_ImageCard.SetOscillographPos( ptPosNew );
}

void CScanView::GraphTextEdit( CPoint point )
{
	///////////////////////////////////////////////////////
	static BOOL boolBusy = FALSE;
	static CSemCtrlDlgEdit	dlg;

	if ( m_bAbc )
	{
		point.y -= 8;
		CPoint ptPos = ScreenToGraph( point );
		////////////////////////////////////////////
		// Grid Pos (Screen)
		////////////////////////////////////////////
		ptPos.x = (ptPos.x / 16) * 16;
		ptPos.y = (ptPos.y / 32) * 32;
		////////////////////////////////////////////

		if( (ptPos.x > m_sizeReso.cx *3/4) && (ptPos.y > m_sizeReso.cy -64) )
		{
			CDlgInput input;
			input.m_nTitleIndex = IDS_SD_PROMPT_Input_Note;
			if( input.DoModal() == IDOK )
				theApp.m_ImageCard.UpdateNote( input.m_strInput );
		}
		else
		{
			dlg.m_ptPos = ptPos;
			if ( boolBusy )
			{
				dlg.ClearTextBuff();
			}
			else
			{
				boolBusy = TRUE;
				////////////////////

				dlg.m_csText.Empty();
				dlg.DoModal();

				// Modify 2003-6-27 Leo
				m_bAbc = FALSE;
//				m_pScanDlg->UpdateData( FALSE );

				////////////////////
				boolBusy = FALSE;
			}
		}
	}
	///////////////////////////////////////////////////////
}

BOOL CScanView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	static HCURSOR hCursor_IBEAM = LoadCursor(NULL, IDC_IBEAM);

	if ( m_bCatch )
	{
		return TRUE;
	}

	if ( nHitTest != HTCLIENT )
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	POINT point;
	GetCursorPos( &point );
	ScreenToClient( &point );

	CPoint ptPos = ScreenToGraph( point );
	int nGraphHitTest = USB_GraphHitTest( ptPos );

	if ( m_bAbc )
	{
		if ( nGraphHitTest == GraphHitTest_Normal )
		{
			if ( GetCursor() != hCursor_IBEAM )
				SetCursor( hCursor_IBEAM );

			return TRUE;
		}
	}

	if ( GetCursor() != g_hCursors[nGraphHitTest] )
		SetCursor( g_hCursors[nGraphHitTest] );

	return TRUE;
//	return CWnd::OnSetCursor(pWnd, nHitTest, message);
//	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

//扫描速度档改变
void CScanView::OnScanSpeedChanged(UINT nID)
{
	m_nusbPixRateIndex = nID - ID_SCAN_SPEED_0;
	NotifyPixRate( m_nPixRateIndex[m_nusbPixRateIndex] );
}

void CScanView::OnUpdateScanSpeedChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nusbPixRateIndex == pCmdUI->m_nID -ID_SCAN_SPEED_0 );
	pCmdUI->Enable( !m_bLive );
}

//通报扫描速度改变
void CScanView::NotifyPixRate( unsigned short nNewVal )
{
	m_nusbPixRate = nNewVal;

	// 扫描速度改变时，要通知DFocus重新计算Valley个数
	theApp.m_SemCtrl.m_SemCore.m_DFocus.SetParams( 16, m_nusbPixRate );

	CString str;
	str.Format( "Press select speed to %d us/s", m_nusbPixRate /2 );
	theApp.DisplayMsg(str);

	if ( m_nusbScanType != USB_ST_Stop )
	if ( m_nusbScanType != USB_ST_AnalogPhoto )
		USB_ChangeScan( m_nusbScanType );
}

void CScanView::OnScanResolutionChanged(UINT nID)
{
	NotifyScanResolution( nID - ID_SCAN_RESOLUTION_1 );
}

void CScanView::OnUpdateScanResolutionChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nusbScanResoIndex == pCmdUI->m_nID -ID_SCAN_RESOLUTION_1 );
	pCmdUI->Enable( !m_bLive );
}

void CScanView::NotifyScanResolution( UINT uNewVal, BOOL bForce )
{
	if( !bForce && m_nusbScanResoIndex == uNewVal )
		return;

	if ( m_nusbScanType == USB_ST_AnalogPhoto )
		return;

	// 先停止采集
	int nusbScanType = m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );

	m_nusbScanResoIndex = uNewVal;
	USB_GetResoSize( m_nusbScanResoIndex );

	UpdateWindowPos(m_bSplit);
//	UpdateScale();

	// 16.06.12 多移动标尺实验
	m_nMobileUScaleCounts = 0;

	StageMC600_SetResolution( m_sizeReso );
	theApp.m_ImageCard.SetResolution( m_sizeReso );
	theApp.m_ImageCard.SetSplit(m_bSplit);
	theApp.m_ImageCard.SetLiveMode( m_bLive );
	theApp.m_ImageCard.Reset( this );
	theApp.m_ImageCard.SetProductLogo( theApp.m_strLogoDisp );
	theApp.m_ImageCard.ShowLabel( m_bLabel );
	theApp.m_ImageCard.ShowTime( m_bDate );
	SyncDate( m_bDate );
	KillTimer( nAutoTimerDate );
	if( m_bDate )
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	theApp.m_ImageCard.ShowNote( m_bNote );
	theApp.m_ImageCard.ShowMobileUScale( m_bUScale );
	theApp.m_ImageCard.ShowOscillograph( m_bOscillograph );

	UpdatePhotoSn(theApp.m_nPhotoSerialNumber);

	UpdateScale();
	OnViewZoomFit();

	// 设置扫描区域参数
	USB_SetScanRectParam();
	CString str;
	str.Format( "Press select resolution to %d x %d", m_sizeReso.cx, m_sizeReso.cy );
	theApp.DisplayMsg(str);
	// 按照当前的方式重新采集
	USB_ChangeScan( nusbScanType );
	// 更新一下放大倍数相关参数
	VARIANT ClassID, SerialNumber, Flag, Xpos, Ypos; 
	ClassID.lVal	= SemControlPanelClass_Magnifier;
	SerialNumber.lVal = -1;
	Flag.lVal		= 1;
	Xpos.dblVal		= theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
	theApp.SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 自动台移动参考点
	m_ptStagePointParam.x	= m_sizeReso.cx /2;
	m_ptStagePointParam.y	= m_sizeReso.cy /2;
	m_ptPosSave = m_ptStagePointParam;	// 屏幕显示区域始终是1024*768大小，所以中心固定不变
	SetStagePointParam( CPoint(512,384) );

	// 15.09 远程功能
	if (m_bCommSendImgData)
	{
		KYKYComm_SimulImage();
	}
}

void CScanView::UpdateWindowPos( BOOL bDual )
{
	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	int nScreenX = GetSystemMetrics( SM_CXSCREEN );
	int nLeft, nTop;
	if( bDual )
	{
		if( m_bSplitZoom )
		{
			int nWidth = (nScreenX -20) /32 *32;
			nLeft	= ( nScreenX -nWidth ) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nTop	= 10;
			pfrm->SetWindowPos(NULL, nLeft, nTop, nWidth +4, (int)(1.0*m_sizeReso.cy /m_sizeReso.cx *(nWidth/2)) +4, SWP_NOZORDER );
		}
		else
		{
/*			if( theApp.m_dwOpr > 0 )
			{
				nLeft	= 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, 1024 +4, 1.0*m_sizeReso.cy /m_sizeReso.cx *512 +4, SWP_NOZORDER );
			}
			else
*/			{
				nLeft	= (nScreenX -450 -1440) /2;
				if( nLeft < 0 )
					nLeft = 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, 1440 +4, (int)(1.0*m_sizeReso.cy /m_sizeReso.cx *720) +4, SWP_NOZORDER );
			}
		}
	}
	else
	{
/*		if( theApp.m_dwOpr > 0 )
		{
			nLeft	= 10;
			nTop	= 10;
// 			if( m_sizeReso.cx > 800 || m_sizeReso.cy > 800 )
// 				pfrm->SetWindowPos(NULL, nLeft, nTop, 800 +4, 800 +4, SWP_NOZORDER );
// 			else
// 				pfrm->SetWindowPos(NULL, nLeft, nTop, m_sizeReso.cx +4, m_sizeReso.cy +4, SWP_NOZORDER );
			pfrm->SetWindowPos(NULL, nLeft, nTop, 1024 +4, 1.0*m_sizeReso.cy /m_sizeReso.cx *1024 +4, SWP_NOZORDER );
		}
		else
*/		{
			int nX = m_sizeReso.cx;
//			if( m_sizeReso.cx > 1024 || m_sizeReso.cy > 800 )
				nX = m_nDispPixel;	// 1024;
			nLeft	= (nScreenX -450 -nX) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nTop	= 10;
//			if( m_sizeReso.cx > 1024 || m_sizeReso.cy > 800 )
				pfrm->SetWindowPos(NULL, nLeft, nTop, nX +4, (int)(1.0*m_sizeReso.cy /m_sizeReso.cx *nX) +4, SWP_NOZORDER );
// 			else
// 				pfrm->SetWindowPos(NULL, nLeft, nTop, m_sizeReso.cx +4, m_sizeReso.cy +4, SWP_NOZORDER );
/*			int nX, nY, nWidth, nHeight;
			if( m_sizeReso.cx > 800 || m_sizeReso.cy > 800 )
			{
				nX		= 800;
				nWidth	= 800 +4;
				nHeight	= 800 +4;
			}
			else
			{
				nX		= m_sizeReso.cx;
				nWidth	= m_sizeReso.cx +4;
				nHeight	= m_sizeReso.cy +4;
			}
			nLeft	= (nScreenX -450 -nX) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nY = 0;
			CMainFrame* pfrmMain = (CMainFrame *)theApp.GetMainWnd();
			if( (pfrmMain->m_wndImpPanelThumb.m_hWnd != NULL) && pfrmMain->m_bPanelThumb )
			{
				CRect rectCtrl;
				pfrmMain->m_wndImpPanelThumb.GetClientRect( &rectCtrl );
				nY = rectCtrl.Height() +28;
			}
			else if( (pfrmMain->m_wndImpPanelLog.m_hWnd != NULL) && pfrmMain->m_bPanelLog )
			{
				CRect rectCtrl;
				pfrmMain->m_wndImpPanelLog.GetClientRect( &rectCtrl );
				nY = rectCtrl.Height() +28;
			}
			int nScreenCaption = GetSystemMetrics( SM_CYCAPTION );
			int nScreenMenu = GetSystemMetrics( SM_CYMENU );
			CRect rect;
			pfrmMain->GetClientRect( &rect );
			nTop = (rect.Height() -2*nScreenCaption -nScreenMenu -nY -nHeight) /2;
			if( nTop < 0 )
				nTop = 0;
			pfrm->SetWindowPos(NULL, nLeft, nTop, nWidth, nHeight, SWP_NOZORDER );
*/		}
	}
}

void CScanView::OnScanStoreModeChanged(UINT nID)
{
	if( m_nusbScanStoreMode == nID - ID_SCAN_STORE_ZZZ )
		return;
	
	// 先停止采集
	int nusbScanType = m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );
	
	m_nusbScanStoreMode = nID - ID_SCAN_STORE_ZZZ;
	
	// 按照当前的方式重新采集
	USB_ChangeScan( nusbScanType );
}

void CScanView::OnUpdateScanStoreModeChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nusbScanStoreMode == pCmdUI->m_nID -ID_SCAN_STORE_ZZZ );
}

void CScanView::OnCondenserFastChanged(UINT nID)
{
	if( m_nCLgrade == nID - ID_Condenser_Fast_1 )
		return;
	m_nCLgrade = nID - ID_Condenser_Fast_1;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal	= -1;
	Flag.lVal			= 1;

	// CondenserLens
	ClassID.lVal	= SemControlPanelClass_CondenserLens;
//	Xpos.lVal		= m_lCLgrade[m_nCLgrade*3];
	Xpos.lVal		= m_lCLfast[m_nCLgrade];
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 做几次聚光镜反转
	ClassID.lVal = SemControlPanelClass_CondenserLens;
	SerialNumber.lVal = -1;
	BOOL bCondRev = !m_bCondRev;
	int i;
	for( i=0; i<6; i++ )
	{
		theApp.m_SemCtrl.m_SemCore.m_CondenserLens.SetParams( SET_PARAMS_Reverse, bCondRev );	// 反转
		Xpos = theApp.m_SemCtrl.m_SemCore.m_CondenserLens.GetPos();
		theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		if( bCondRev )
			theApp.DisplayMsg("Press CL reverse: ON");
		else
			theApp.DisplayMsg("Press CL reverse: OFF");
		bCondRev = !bCondRev;
		Sleep(500);
	}

	StartAutoBC2();
/*
	// Brightness
	ClassID.lVal	= SemControlPanelClass_Brightness;
	Xpos.lVal		= m_lCLgrade[m_nCLgrade*3 +1];
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	if( theApp.m_blUnlock_HV || theApp.m_blOverhaulState )
	{
		// Contrast
		ClassID.lVal	= SemControlPanelClass_Contrast;
		Xpos.lVal		= m_lCLgrade[m_nCLgrade*3 +2];
		theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}*/
}

void CScanView::OnUpdateCondenserFastChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nCLgrade == pCmdUI->m_nID -ID_Condenser_Fast_1 );
}

void CScanView::ResetImageCard()
{
	// 先停止采集
	int nusbScanType = m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );
	Sleep(1000);

	theApp.m_ImageCard.Reset( this );
	theApp.m_ImageCard.SetProductLogo( theApp.m_strLogoDisp );
	theApp.m_ImageCard.ShowLabel( m_bLabel );
	theApp.m_ImageCard.ShowMobileUScale( m_bUScale );
	theApp.m_ImageCard.ShowOscillograph( m_bOscillograph );

	// 按照当前的方式重新采集
	USB_ChangeScan( nusbScanType );
	UpdatePhotoSn(theApp.m_nPhotoSerialNumber);
	UpdateData(FALSE);
}

void CScanView::StopPhoto()
{
	USB_ChangeScan( USB_ST_Stop );

	//======2004-5-12_照相编号自动增加=====================
	// 本次修改前代码副本
	/*----------------------------------------------------- 
	UpdatePhotoSn(theApp.m_nPhotoSerialNumber + 1);
	-----------------------------------------------------*/
	
	// 本次修改后代码
	//-----------------------------------------------------
	m_bStopPhoto = TRUE;
	//-----------------------------------------------------
	//======2004-5-12_照相编号自动增加=====================


	//======2004-6-2_自动更新日期==========================
	// 修改标示：2004-6-2_自动更新日期
	// 修改日期：2004年6月2日
	// 修改人：罗鸣华
	// 修改原因：同步照相，完成后自动生成文件。

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	-----------------------------------------------------*/
	
	// 本次修改后代码
	//-----------------------------------------------------
	OnPhotoSnapshot();
	//-----------------------------------------------------
	//======2004-6-2_自动更新日期==========================

	//照相后仍将扫描窗口作为当前窗口05.05.25
//	((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild();

	// 照相后重新启动日期刷新定时器05.08.12
	if( m_bDate )
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	// 停止照相后，将当前通道组的Filter恢复
	WORD wValue = (WORD)( (theApp.m_BrdMCI.nDetPair << 8) | (BYTE)(theApp.m_BrdMCI.nFilter[theApp.m_BrdMCI.nDetPair]) );
//	theApp.m_SemCtrl.USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );

	// 判断一下是否要MCI复位通讯
	unsigned short nRegister1 = 0;
	unsigned short nRegister2 = 0;
	unsigned short nRegister3 = 0;
	unsigned short nRegister4 = 0;
	theApp.m_SemCtrl.Serial_ReadbackData( 1, USB_CID_SCN_ReadBoardID, &nRegister1 );
	theApp.m_SemCtrl.Serial_ReadbackData( 2, USB_CID_LNS_ReadBoardID, &nRegister2 );
	theApp.m_SemCtrl.Serial_ReadbackData( 3, USB_CID_VAC_ReadBoardID, &nRegister3 );
	theApp.m_SemCtrl.Serial_ReadbackData( 4, USB_CID_SPR_ReadBoardID, &nRegister4 );

	if( (nRegister1 != 0xA1) || (nRegister2 != 0xA2) || (nRegister3 != 0xA3) || (nRegister4 != 0xA4) )
	{
		CString str, strTemp;
		if( nRegister1 != 0xA1 )
			str += "1/ ";
		if( nRegister2 != 0xA2 )
			str += "2/ ";
		if( nRegister3 != 0xA3 )
			str += "3/ ";
		if( nRegister4 != 0xA4 )
			str += "4 ";
		str += "have errors! ";
		str += "Reset SubBoards!";
		theApp.DisplayMsg(str);
		theApp.m_SemCtrl.USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
	}
}

void CScanView::USB_ChangeScan( int nusbScanType )
{
	static int nusbScanTypeSave = USB_ST_RasterScan;

	// 2005-1-17_0.0KV问题
	if ( (nusbScanTypeSave == USB_ST_Stop) && (nusbScanType != USB_ST_Stop) )
		theApp.RefreshLabel();

	// 2005-1-17_0.0KV问题
	if ( nusbScanType == USB_ST_Stop )
		m_bStopScan = TRUE;
	else
		m_bStopScan = FALSE;

	///////////////////////////////////////////////////////////
	USB_ScanFunction		Function	= USB_SF_RasterScan;	// 工作方式
	unsigned short			nPixRate	= m_nusbPixRate;
	///////////////////////////////////////////////////////////
	USB_ScanParam		Param;				// 扫描参数
	Param.RasterScan.dwStartX	= 0;
	Param.RasterScan.dwStartY	= 0;
	Param.RasterScan.dwStopX	= m_sizeReso.cx;
	Param.RasterScan.dwStopY	= m_sizeReso.cy;
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// ResetImageCard()
	///////////////////////////////////////////////////////////
	if ( nusbScanType == -1 )
	{
		nusbScanType	= m_nusbScanType;

		Function	= m_usbFunction;		// 工作方式
		Param		= m_usbParam;			// 扫描参数
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	switch (nusbScanType)
	{
	/////////////////////////////////////////////////////
	case USB_ST_Stop:					// 冻结图象扫描
		theApp.m_SemCtrl.m_SemScan.USB_ScanStop();
		theApp.DisplayMsg( "Change Scan: scan stop" );
		break;

	/////////////////////////////////////////////////////
	case USB_ST_RasterScan:				// 常规扫描
		Function		= USB_SF_RasterScan;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= 0;
		Param.RasterScan.dwStartY	= 0;
		Param.RasterScan.dwStopX	= m_sizeReso.cx;
		Param.RasterScan.dwStopY	= m_sizeReso.cy;
		break;
	
	/////////////////////////////////////////////////////
	case USB_ST_AreaScan:				// 活动选区扫描
		Function		= USB_SF_AreaScan;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= m_rcAreaScanParam.left;
		Param.RasterScan.dwStartY	= m_rcAreaScanParam.top;
		Param.RasterScan.dwStopX	= m_rcAreaScanParam.right;
		Param.RasterScan.dwStopY	= m_rcAreaScanParam.bottom;
		break;

	/////////////////////////////////////////////////////
	case USB_ST_LineScan_HORZ:			// 水平线扫描
		Function		= USB_SF_LineScan_HORZ;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= 0;
		Param.RasterScan.dwStopX	= m_sizeReso.cx;
		Param.RasterScan.dwStartY	= m_nLineScanParam;
		Param.RasterScan.dwStopY	= m_nLineScanParam + 1;
		break;

	case USB_ST_LineScan_VERT:			// 垂直线扫描
		Function		= USB_SF_LineScan_VERT;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= m_nLineScanParam;
		Param.RasterScan.dwStopX	= m_nLineScanParam + 1;
		Param.RasterScan.dwStartY	= 0;
		Param.RasterScan.dwStopY	= m_sizeReso.cy;
		break;

	/////////////////////////////////////////////////////
	case USB_ST_SpotScan:				// 点扫描
		Function		= USB_SF_SpotScan;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= m_ptPointScanParam.x;
		Param.RasterScan.dwStopX	= m_ptPointScanParam.x + 1;
		Param.RasterScan.dwStartY	= m_ptPointScanParam.y;
		Param.RasterScan.dwStopY	= m_ptPointScanParam.y + 1;
		break;

	/////////////////////////////////////////////////////
	case USB_ST_AnalogPhoto:			// 模拟照相
		m_bStageOrigin	= FALSE;
		theApp.m_ImageCard.GraphClear( m_rcStageDragRectLast );
		Function		= USB_SF_AnalogPhoto;
		nPixRate		= m_nusbPhotoPixRate;
//		nPixRate		= m_nusbPixRate;
		{
			// 照相时将当前通道组的Filter设置为5
			WORD wValue = (WORD)( (theApp.m_BrdMCI.nDetPair << 8) | 0x05 );
//			theApp.m_SemCtrl.USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
		}

//		theApp.m_SemCtrl.m_SemScan.USB_ScanStop();	// 2003-8-29
		MakeFilmPhotoDataByGraphLay();

		//照相时停止日期刷新定时器05.08.12
		KillTimer( nAutoTimerDate );
		break;

	/////////////////////////////////////////////////////
	case USB_ST_AutoBCArea:				// 固定选区扫描	AutoBC
		Function		= USB_SF_RasterScan;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= m_rcAutoBCAreaScanParam.left;
		Param.RasterScan.dwStartY	= m_rcAutoBCAreaScanParam.top;
		Param.RasterScan.dwStopX	= m_rcAutoBCAreaScanParam.right;
		Param.RasterScan.dwStopY	= m_rcAutoBCAreaScanParam.bottom;
		break;

	/////////////////////////////////////////////////////
	case USB_ST_AutoFocusArea:			// 固定选区扫描	AutoFocus
		Function		= USB_SF_RasterScan;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= m_rcAutoFocusAreaScanParam.left;
		Param.RasterScan.dwStartY	= m_rcAutoFocusAreaScanParam.top;
		Param.RasterScan.dwStopX	= m_rcAutoFocusAreaScanParam.right;
		Param.RasterScan.dwStopY	= m_rcAutoFocusAreaScanParam.bottom;
		break;

	/////////////////////////////////////////////////////
	case USB_ST_AutoAstigmatArea:		// 固定选区扫描 AutoAstigmat
		Function		= USB_SF_RasterScan;
		nPixRate		= m_nusbPixRate;
		
		Param.RasterScan.dwStartX	= m_rcAutoAstigmatAreaScanParam.left;
		Param.RasterScan.dwStartY	= m_rcAutoAstigmatAreaScanParam.top;
		Param.RasterScan.dwStopX	= m_rcAutoAstigmatAreaScanParam.right;
		Param.RasterScan.dwStopY	= m_rcAutoAstigmatAreaScanParam.bottom;
		break;

	default :
		nusbScanType = USB_ST_Stop;		// 冻结图象扫描
		theApp.m_SemCtrl.m_SemScan.USB_ScanStop();
		break;
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	if( /*nusbScanType == USB_ST_AnalogPhoto ||*/ m_bPreview )
		// 恢复实际放大倍数
		theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal, TRUE );
	else
		theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal );
	if ( (USB_ST_Stop < nusbScanType) && (nusbScanType <= USB_ST_Max) )
	{
		theApp.m_SemCtrl.m_SemScan.USB_SetScanFunctionEx(
			Function,			// 工作方式
			m_sizeReso,			// 分辨率
			nPixRate,			// 像素率 = 点停留时间/4，必须为整数
			Param,				// 扫描参数
			theApp.m_dHV );

/*		// 扫描方式改变，动态聚焦参数也需改变
		varVal = theApp.m_SemCtrl.m_SemCore.m_DFocus.GetSwitch();
		if( varVal.lVal == 1 || varVal.lVal == 2 )	// 正在使用动态聚焦
		{
			if( nusbScanType == USB_SF_RasterScan || nusbScanType == USB_ST_AreaScan )	// 只在常规扫描，区域扫描时使用
				theApp.m_SemCtrl.m_SemCore.m_DFocus.SetParams( 13, Param.RasterScan.dwStopY - Param.RasterScan.dwStartY );
		}
*/	}

	///////////////////////////////////////////////
	// 保存参数 for ResetImageCard()
	///////////////////////////////////////////////
	nusbScanTypeSave	= nusbScanType;
	m_usbFunction		= Function;			// 工作方式
	m_usbParam			= Param;			// 扫描参数
	///////////////////////////////////////////////

	if ( m_nusbScanType != nusbScanType )
		m_nusbScanType = nusbScanType;

	if( m_bStageOrigin )
	{
		m_ptPosSave = CPoint(0,0);
		SetStagePointParam( CPoint(0,0) );
	}
}
// 2005-1-17_0.0KV问题
BOOL CScanView::IsStopScan()
{
	return m_bStopScan;
}

BOOL CScanView::IsPhoto()
{
	if ( m_nusbScanType == USB_ST_AnalogPhoto)
		return TRUE;
	
	return FALSE;
}

//-----------------------------------------------------
//======修改日期：2004年5月12日星期三==================


//扫描模式改变
void CScanView::OnScanTypeChanged(UINT nID)
{
	// TODO: Add your control notification handler code here
	unsigned short nusbScanTypeOld = m_nusbScanType;

	BeginWaitCursor(); // display the hourglass cursor
	m_nusbScanType = nID - ID_SCAN_TYPE_FREEZE;
	//======2004-5-12_照相编号自动增加====================
	if ( m_bStopPhoto == TRUE )
	{
		m_bStopPhoto = FALSE;
		UpdatePhotoSn(theApp.m_nPhotoSerialNumber + 1);
	}

	CString str = "";
	switch( m_nusbScanType )
	{
	case	USB_ST_Stop:
		str = "Press select scan type to Freeze";
		break;
	case	USB_ST_RasterScan:
		str.Format( "Press select scan type to Normal Scan, %dx%d, speed = %d us/s", m_sizeReso.cx, m_sizeReso.cy, m_nusbPixRate /2 );
		break;
	case	USB_ST_AreaScan:
		str.Format( "Press select scan type to Area Scan, (%d,%d,%d,%d), speed = %d us/s",
					m_rcAreaScanParam.left, m_rcAreaScanParam.top, m_rcAreaScanParam.right, m_rcAreaScanParam.bottom, m_nusbPixRate /2 );
		break;
	case	USB_ST_LineScan_HORZ:
		str.Format( "Press select scan type to LineScan HORZ, speed = %d us/s", m_nusbPixRate /2 );
		break;
	case	USB_ST_LineScan_VERT:
		str.Format( "Press select scan type to LineScan VERT, speed = %d us/s", m_nusbPixRate /2 );
		break;
	case	USB_ST_SpotScan:
		str.Format( "Press select scan type to Spot, speed = %d us/s", m_nusbPixRate /2 );
		break;
	case	USB_ST_AnalogPhoto:
		str.Format( "Press select scan type to Sync, %dx%d, speed = %d us/s", m_sizeReso.cx, m_sizeReso.cy, m_nusbPhotoPixRate /2 );
		break;
	}
	theApp.DisplayMsg(str);

	if( m_nusbScanType != USB_ST_Stop )
	{
		if( m_bDate )
			SetTimer( nAutoTimerDate, 1000*30, NULL );
		theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal );
	}
	if( (nusbScanTypeOld == USB_ST_AnalogPhoto)
		&& (m_nusbScanType != USB_ST_AnalogPhoto) )
	{
		// 将当前通道组的Filter恢复
		WORD wValue = (WORD)( (theApp.m_BrdMCI.nDetPair << 8) | (BYTE)(theApp.m_BrdMCI.nFilter[theApp.m_BrdMCI.nDetPair]) );
//		theApp.m_SemCtrl.USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
	}

	USB_ChangeScan( m_nusbScanType );
	EndWaitCursor(); // remove the hourglass cursor
}

void CScanView::OnUpdateScanTypeChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nusbScanType == pCmdUI->m_nID -ID_SCAN_TYPE_FREEZE );
	pCmdUI->Enable( !m_bLive );
}

void CScanView::OnScanTypeLive() 
{
	// TODO: Add your command handler code here
	NotifyScanLive( !m_bLive );
}

void CScanView::OnUpdateScanTypeLive(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bLive );
}

void CScanView::NotifyScanLive( BOOL bLive )
{
	if( m_bLive == bLive )
		return;
	m_bLive = bLive;
	// 图像分辨率变为512*384，但扫描区域大小为1024*768，即放大2倍显示
	// 扫描速度设置为500ns

	if( m_bLive )
	{
		// 先备份扫描参数
//		m_scanParamBak = theApp.m_Param.scan;
		m_scanParamBak.nScan_Type	= m_nusbScanType;
		m_scanParamBak.nScan_Speed	= m_nusbPixRate;
		m_scanParamBak.nSync_Speed	= m_nusbPhotoPixRate;
		m_scanParamBak.nScan_ResoX	= m_sizeReso.cx;
		m_scanParamBak.nScan_ResoY	= m_sizeReso.cy;
		m_scanParamBak.nSplit		= m_bSplit;
		m_scanParamBak.nDet2		= m_bOnlyDet2;
		// 停止采集
		USB_ChangeScan( USB_ST_Stop );
		// 扫描参数设置为快速扫描参数
// 		theApp.m_Param.scan	= m_scanParamLive;
// 		theApp.m_Param.scan.nSplit	= m_bSplit;
// 		theApp.m_Param.scan.nDet2	= m_bOnlyDet2;
		m_nusbScanType		= m_scanParamLive.nScan_Type;
		m_nusbPixRate		= m_scanParamLive.nScan_Speed;
		m_nusbPhotoPixRate	= m_scanParamLive.nSync_Speed;
		m_sizeReso.cx		= m_scanParamLive.nScan_ResoX;
		m_sizeReso.cy		= m_scanParamLive.nScan_ResoY;
		// 改变扫描分辨率
//		UpdateWindowPos(m_bSplit);
		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
		int nScreenX = GetSystemMetrics( SM_CXSCREEN );
		int nLeft, nTop;
		if( m_bSplit )
		{
			if( m_bSplitZoom )
			{
				int nWidth = (nScreenX -20) /32 *32;
				nLeft	= ( nScreenX -nWidth ) /2;
				if( nLeft < 0 )
					nLeft = 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, nWidth +4, (int)(1.0*m_sizeReso.cy /m_sizeReso.cx *(nWidth/2)) +4, SWP_NOZORDER );
			}
			else
			{
				nLeft	= (nScreenX -450 -1440) /2;
				if( nLeft < 0 )
					nLeft = 10;
				nTop	= 10;
				pfrm->SetWindowPos(NULL, nLeft, nTop, 1440 +4, (int)(1.0*m_sizeReso.cy /m_sizeReso.cx *720) +4, SWP_NOZORDER );
			}
		}
		else
		{
			int nX = m_sizeReso.cx *2;
			nLeft	= (nScreenX -450 -nX) /2;
			if( nLeft < 0 )
				nLeft = 10;
			nTop	= 10;
			pfrm->SetWindowPos(NULL, nLeft, nTop, m_sizeReso.cx*2 +4, m_sizeReso.cy*2 +4, SWP_NOZORDER );
		}

		StageMC600_SetResolution( m_sizeReso );
		theApp.m_ImageCard.SetResolution( m_sizeReso );
		theApp.m_ImageCard.SetSplit(m_bSplit);
		theApp.m_ImageCard.SetLiveMode( m_bLive );
		theApp.m_ImageCard.Reset( this );
		theApp.m_ImageCard.SetProductLogo( theApp.m_strLogoDisp );
		theApp.m_ImageCard.ShowLabel( m_bLabel );
		theApp.m_ImageCard.ShowMobileUScale( m_bUScale );
		theApp.m_ImageCard.ShowOscillograph( m_bOscillograph );

		CString strPhotoSN;
		strPhotoSN.Format("SN:%04d", theApp.m_nPhotoSerialNumber);
		theApp.m_ImageCard.UpdatePhotoSN( strPhotoSN );

		UpdateScale();
		OnViewZoomFit();

		// 设置扫描区域参数
		USB_SetScanRectParam();
		CString str;
		str.Format( "Press select resolution to %d x %d, Live mode", m_sizeReso.cx, m_sizeReso.cy );
		theApp.DisplayMsg(str);
		// 自动台移动参考点
		m_ptStagePointParam.x	= m_sizeReso.cx /2;
		m_ptStagePointParam.y	= m_sizeReso.cy /2;
		m_ptPosSave = m_ptStagePointParam;	// 屏幕显示区域始终是1024*768大小，所以中心固定不变
		SetStagePointParam( CPoint(512,384) );
		// 发起采集
		USB_ChangeScan( m_nusbScanType );
		// 更新一下放大倍数相关参数
		VARIANT ClassID, SerialNumber, Flag, Xpos, Ypos; 
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		SerialNumber.lVal = -1;
		Flag.lVal		= 1;
		Xpos.dblVal		= theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
		theApp.SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
	else
	{
		USB_ChangeScan( USB_ST_Stop );
//		theApp.m_Param.scan	= m_scanParamBak;
		m_nusbScanType		= m_scanParamBak.nScan_Type;
		m_nusbPixRate		= m_scanParamBak.nScan_Speed;
		m_nusbPhotoPixRate	= m_scanParamBak.nSync_Speed;
		m_sizeReso.cx		= m_scanParamBak.nScan_ResoX;
		m_sizeReso.cy		= m_scanParamBak.nScan_ResoY;
		NotifyScanResolution( m_nusbScanResoIndex, TRUE );
		USB_ChangeScan( m_nusbScanType );
		// 更新一下放大倍数相关参数
		VARIANT ClassID, SerialNumber, Flag, Xpos, Ypos; 
		ClassID.lVal	= SemControlPanelClass_Magnifier;
		SerialNumber.lVal = -1;
		Flag.lVal		= 1;
		Xpos.dblVal		= theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
		theApp.SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );
	}
}

void CScanView::OnScanTypeSplit() 
{
	// TODO: Add your command handler code here
	NotifyScanSplit( !m_bSplit, m_bSplitZoom );
}

void CScanView::NotifyScanSplit( BOOL bSplit, BOOL bSplitZoom )
{
	if( (m_bSplit == bSplit) && (m_bSplitZoom == bSplitZoom) )
		return;

	m_bSplit = bSplit;
	m_bSplitZoom = bSplitZoom;
	if( !m_bSplit )
		m_bSplitZoom = FALSE;

	int nScanType = m_nusbScanType;
	USB_ChangeScan( USB_ST_Stop );
	Sleep(5);
	// 设置窗口大小
	UpdateWindowPos( m_bSplit );
	if( m_bSplit )
	{
		theApp.m_ImageCard.SetFunction( USB_ST_Max );	// Split
		if( m_bSplitZoom )
			theApp.DisplayMsg("Press split: split & zoom");
		else
			theApp.DisplayMsg("Press split: split");
	}
	else
	{
		theApp.m_ImageCard.SetFunction( m_nusbScanType );
		theApp.DisplayMsg("Press split: normal");
	}
	theApp.m_ImageCard.SetSplit( m_bSplit );
	// Reset 图像卡
	theApp.m_ImageCard.Reset( this );
	theApp.m_ImageCard.SetProductLogo( theApp.m_strLogoDisp );
	theApp.m_ImageCard.ShowLabel( m_bLabel );
	theApp.m_ImageCard.ShowTime( m_bDate );
	SyncDate( m_bDate );
	KillTimer( nAutoTimerDate );
	if( m_bDate )
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	theApp.m_ImageCard.ShowNote( m_bNote );
	theApp.m_ImageCard.ShowMobileUScale( m_bUScale );
	theApp.m_ImageCard.ShowOscillograph( m_bOscillograph );

	UpdateScale();
	theApp.m_SemCtrl.m_SemScan.USB_SetParams( 31, m_bSplit );
	CheckCCDSwitch();
	USB_ChangeScan( nScanType );

/*	if( theApp.m_dwOpr > 0 )
	{
		if( (((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL)
			&& (((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.m_hWnd != NULL) )
		{
			if( m_bSplit )
				((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.m_ctrlSignal2Brightness.ShowWindow( SW_SHOW );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.m_ctrlSignal2Brightness.ShowWindow( SW_HIDE );

			if( m_bSplitZoom )
				((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.ShowWindow(SW_HIDE);
			else
				((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.ShowWindow(SW_SHOW);
			((CMainFrame *)theApp.GetMainWnd())->RecalcLayout();
		}
	}
	else
*/	{
		if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
		{
			if( m_bSplit )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_ctrlSignal2Brightness.ShowWindow( SW_SHOW );
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_ctrlSignal2Brightness.ShowWindow( SW_HIDE );

			if( m_bSplitZoom )
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ShowWindow(SW_HIDE);
			else
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ShowWindow(SW_SHOW);
		}
	}
}

void CScanView::CheckCCDSwitch()
{
	if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
	{
		// 17.09 CCD在看背散射像的时候不工作（J12要关）
		if( m_bSplit || m_bSplitZoom )
		{
			if( theApp.m_BrdMCI.nChnIndex[0] > 0 || theApp.m_BrdMCI.nChnIndex[1] > 0 )
			{
				// 分割像
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( FALSE );
			}
//			else
//				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( TRUE );
		}
		else
		{
			// 仅看其中一通道像
			if( m_bOnlyDet2 )
			{
				// 仅看第二通道，且第二通道显示的是背散射像或混合像
				if( theApp.m_BrdMCI.nChnIndex[1] > 0 )
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( FALSE );
				else
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( TRUE );
			}
			else
			{
				// 普通看像（看第一通道），且第一通道显示的是背散射像或混合像
				if( theApp.m_BrdMCI.nChnIndex[0] > 0 )
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( FALSE );
				else
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.SyncCCDSwitch( TRUE );
			}
		}
	}
}

void CScanView::OnUpdateScanTypeSplit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bSplit );
}

void CScanView::OnScanTypeSplitZoom() 
{
	if( !m_bSplitZoom )
		NotifyScanSplit( TRUE, !m_bSplitZoom );
	else
		NotifyScanSplit( m_bSplit, !m_bSplitZoom );
}

void CScanView::OnUpdateScanTypeSplitZoom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bSplitZoom );
}

void CScanView::OnScanTypeDet2() 
{
	m_bOnlyDet2 = !m_bOnlyDet2;
	theApp.CheckDet2nd( m_bOnlyDet2 );
	CheckCCDSwitch();
	if( m_bOnlyDet2 )
		theApp.DisplayMsg("Press scan type det 2: select 1,3,5 chn");
	else
		theApp.DisplayMsg("Press scan type det 2: select 0,2,4 chn");
}

void CScanView::OnUpdateScanTypeDet2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bOnlyDet2 );
	pCmdUI->Enable( !(m_bSplit || m_bSplitZoom) );
}

void CScanView::UpdatePhotoSn(int nSN)
{
	if ( nSN > 9999 ) nSN = 0;

	if( theApp.m_nPhotoSerialNumber != nSN )
	{
		theApp.m_nPhotoSerialNumber = nSN;
	
		theApp.WriteProfileInt( g_csSettingSection, 
		g_csRegEntry_nPhotoSerialNumber, nSN );
	}

	CString m_csPhotoSN;
	m_csPhotoSN.Format("SN:%04d", nSN);
	theApp.m_ImageCard.UpdatePhotoSN( m_csPhotoSN );
}

//调节器类型改变
void CScanView::OnAdjustorSortChanged(UINT nID)
{
	SyncAdjustorSort( nID );
}

void CScanView::OnUpdateAdjustorSortChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_nAdjustorSort == pCmdUI->m_nID -ID_ADJUSTOR_BEAM_ALIGNMENT );
}

//同步调节器类型改变
void CScanView::SyncAdjustorSort( UINT nNewVal )
{
	m_nAdjustorSort = nNewVal - ID_ADJUSTOR_BEAM_ALIGNMENT;
	if( (((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_hWnd != NULL)
		&& (((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.m_hWnd != NULL) )
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PageDebug2.UpdateAdjustor( m_nAdjustorSort );
	if( (((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL) )
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.UpdateAdjustor( m_nAdjustorSort );
}

//手动调节器类型改变
void CScanView::OnAdjustorManualSortChanged(UINT nID)
{
	SyncAdjustorManualSort( nID );
}

void CScanView::OnUpdateAdjustorManualSortChanged(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( theApp.m_nAdjustorManualSort == pCmdUI->m_nID -ID_ADJUSTOR_MANUAL_ObjectiveLens );
}

//同步手动调节器类型改变
void CScanView::SyncAdjustorManualSort( UINT nNewVal )
{
	theApp.m_nAdjustorManualSort = nNewVal -ID_ADJUSTOR_MANUAL_ObjectiveLens;
	return;
	UINT nID = 10000;
	switch( theApp.m_nAdjustorManualSort )
	{
	case	SORT_MANUAL_OBJECTIVE_FINE:
	case	SORT_MANUAL_OBJECTIVE_CORSE:
		nID = ID_ADJUSTOR_OBJECTIVE_LENS;
		break;
	case	SORT_MANUAL_CONDENSER_FINE:
	case	SORT_MANUAL_CONDENSER_CORSE:
		nID = ID_ADJUSTOR_CONDENSER_LENS;
		break;
	case	SORT_MANUAL_STIGMATOR_X:
	case	SORT_MANUAL_STIGMATOR_Y:
	case	SORT_MANUAL_STIGMATOR_X2:
	case	SORT_MANUAL_STIGMATOR_Y2:
		nID = ID_ADJUSTOR_STIGMATOR;
		break;
	case	SORT_MANUAL_BEAM_DISPLACEMENT_X:
	case	SORT_MANUAL_BEAM_DISPLACEMENT_Y:
		nID = ID_ADJUSTOR_BEAM_SHIFT;
		break;
	case	SORT_MANUAL_BEAM_ALIGNMENT_X:
	case	SORT_MANUAL_BEAM_ALIGNMENT_Y:
		nID = ID_ADJUSTOR_BEAM_ALIGNMENT;
		break;
	case	SORT_MANUAL_BRIGHT:
		nID = ID_ADJUSTOR_BRIGHTNESS;
		break;
	case	SORT_MANUAL_CONTRAST:
		nID = ID_ADJUSTOR_CONTRASTNESS;
		break;
	case	SORT_MANUAL_NULL:
		nID = ID_ADJUSTOR_STIGMATOR_ASSISTANT;	// Temp
		break;
	}
	SyncAdjustorSort( nID );
}

//聚光镜反转开关改变
void CScanView::OnAdjustorCondenserLensReverse() 
{
	SyncCondenserLensReverse( !m_bCondRev );
}

void CScanView::OnUpdateAdjustorCondenserLensReverse(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bCondRev );
}

void CScanView::SyncCondenserLensReverse( BOOL bNewVal )
{
	m_bCondRev = bNewVal;

	theApp.m_SemCtrl.m_SemCore.m_CondenserLens.SetParams( SET_PARAMS_Reverse, m_bCondRev );	// 反转

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_CondenserLens;
	SerialNumber.lVal = -1;
	Xpos = theApp.m_SemCtrl.m_SemCore.m_CondenserLens.GetPos();
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	if( m_bCondRev )
		theApp.DisplayMsg("Press CL reverse: ON");
	else
		theApp.DisplayMsg("Press CL reverse: OFF");
}

//物镜反转开关改变
void CScanView::OnAdjustorObjectiveLensReverse() 
{
	SyncObjectiveLensReverse( !m_bObjectiveRev );
}

void CScanView::OnUpdateAdjustorObjectiveLensReverse(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bObjectiveRev );
}

void CScanView::SyncObjectiveLensReverse( BOOL bNewVal )
{
	m_bObjectiveRev = bNewVal;

	theApp.m_SemCtrl.m_SemCore.m_ObjectiveLens.SetParams( SET_PARAMS_Reverse, m_bObjectiveRev );	// 反转

	if( m_bObjectiveRev )
		theApp.DisplayMsg("Press OL reverse: ON");
	else
		theApp.DisplayMsg("Press OL reverse: OFF");
}

//辅助消像散开关改变
void CScanView::OnStigmatorAssistant() 
{
	SyncAuxiliaryStigmation( !m_bAuxStig );
}

void CScanView::OnUpdateStigmatorAssistant(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bAuxStig );
}

void CScanView::SyncAuxiliaryStigmation( BOOL bNewVal )
{
	m_bAuxStig = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_AuxiliaryStigmation;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)m_bAuxStig;
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	((CButton*)(pfrm->m_MainPanel.m_PagePanelAdjust.GetDlgItem( ID_ADJUSTOR_STIGMATOR_ASSISTANT ) ))->SetCheck( bNewVal );
}

//四舍五入函数
extern int RoundFunc( double dNum );

//快照
void CScanView::OnPhotoSnapshot() 
{
/*	CImageDoc *NewDoc=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	BeginWaitCursor();
	if (NewDoc)	{
//		CThumbImageItem* pImageItem = new CThumbImageItem();

		/////////////////////////////////////////////////////
		double dfMag = theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
		double dblHV = theApp.m_SemCtrl.m_SemCore.m_HVAccelerate.GetPos().dblVal;
		NewDoc->m_nMag	= RoundFunc( dfMag );	//09.06.05
		NewDoc->m_nHV	= RoundFunc( dblHV );	//09.06.05
//		pImageItem->m_nMag	= RoundFunc( dfMag );
//		pImageItem->m_nHV	= RoundFunc( dblHV );
		/////////////////////////////////////////////////////

		/////////////////////////////////////////////////////
		// PixelsPerMeter = DPI * 10000.0 / 254.0;
		// DPI = PixelsPerMeter * 254.0 / 10000.0;
		// --------------------------------------------------
		// PixelsPerMeter = Pixels / PhotoSize (m)
		// PixelsPerMeter = Pixels * 1000 / PhotoSize (mm)
		// --------------------------------------------------
		// PhotoWidth  = 120 mm		<==>	ScreenWidth  = 1024 Pixel
		// PhotoHeight =  90 mm		<==>	ScreenHeight =  768 Pixel
		// --------------------------------------------------
		// DPI = Pixels * 25.4 / PhotoSize (mm);
		// --------------------------------------------------
		// DPI_X = 25.4 * 1024 / 120;
		// DPI_Y = 25.4 *  768 /  90;
		const double dblDPI_X = 25.4 * 1024 / 120;
		const double dblDPI_Y = 25.4 *  768 /  90;
		/////////////////////////////////////////////////////

		/////////////////////////////////////////////////////
		CBitmap bmpGraphLay;
		if ( theApp.m_ImageCard.GetGraphBmp(bmpGraphLay) )
		{
			CxImage *newima = new CxImage();
			newima->CreateFromHBITMAP( (HBITMAP)bmpGraphLay );
			newima->SetXDPI(dblDPI_X);
			newima->SetYDPI(dblDPI_Y);
			NewDoc->GraphLayer = newima;

			/////////////////////////////////////////////
			bmpGraphLay.DeleteObject();
			/////////////////////////////////////////////
		}
		/////////////////////////////////////////////////////

		/////////////////////////////////////////////////////
		HANDLE hImageLay = NULL;
		hImageLay = theApp.m_ImageCard.GetDib();
		if (hImageLay != NULL)
		{
			CxImage *newima = new CxImage();
			newima->CreateFromHANDLE(hImageLay);
			newima->SetXDPI(dblDPI_X);
			newima->SetYDPI(dblDPI_Y);
			NewDoc->ImageLayer = newima;
//			pImageItem->m_pImageLayer->Copy( *newima );

			/////////////////////////////////////////////
			if( hImageLay != NULL )
			{
				GlobalUnlock( (HGLOBAL)hImageLay );
				GlobalFree( (HGLOBAL)hImageLay );
				hImageLay = NULL;
			}
			/////////////////////////////////////////////
		}
		/////////////////////////////////////////////////////

		/////////////////////////////////////////////////////
		CBitmap bmpScreen;
		if ( theApp.m_ImageCard.CopyScreenBmp(bmpScreen) )
		{
			CxImage *newima = new CxImage();
			newima->CreateFromHBITMAP( (HBITMAP)bmpScreen );
			newima->SetXDPI(dblDPI_X);
			newima->SetYDPI(dblDPI_Y);
			NewDoc->image = newima;
//			pImageItem->m_pImageSrc->Copy( *newima );

			/////////////////////////////////////////////
			bmpScreen.DeleteObject();
			/////////////////////////////////////////////
		}
		/////////////////////////////////////////////////////

		/////////////////////////////////////////////////////
		NewDoc->SetModifiedFlag();
		NewDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		NewDoc->UpdateScale();	// 05.06.23调整缩放尺寸
		NewDoc->UpdateStatusBar();
		NewDoc->SaveTempFile();	// 10.02.08保存临时文件
		/////////////////////////////////////////////////////

		//07.04.26显示图像小样
		if( !m_bOpenImageAfterSnap )	//照相后仍将扫描窗口作为当前窗口
			((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild();

// 		CImageItem* pImageItem = new CImageItem();
// 		pImageItem->m_pIImage->Copy( *NewDoc->GetImage() );
// 		pImageItem->m_strFileName = NewDoc->GetTitle();
// 		m_plistImageItem->AddTail( pImageItem );
// 		UpdateItemRect();
// 
// 		CxImage* pImage = new CxImage();
// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		pfrm->m_wndImpPanelThumb.AddItem( NewDoc->GetImage(), NewDoc->GetTitle() );
// 		((CMainFrame *)theApp.GetMainWnd())->m_MainPanel.m_PagePanelThumb.AddItem( NewDoc->GetImage(), NewDoc->GetTitle() );

//		if( pImageItem->GetImage() != NULL )
			((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.AddItem( NewDoc->GetImage(), NewDoc->GetTitle() );
	}	
	EndWaitCursor();
	return;
*/
	BeginWaitCursor();
	CThumbImageItem* pImageItem = new CThumbImageItem();

	/////////////////////////////////////////////////////
	double dfMag = theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
	double dblHV = theApp.m_SemCtrl.m_SemCore.m_HVAccelerate.GetPos().dblVal;

	pImageItem->m_nMag	= RoundFunc( dfMag );	//09.06.05
	pImageItem->m_nHV	= RoundFunc( dblHV );	//09.06.05
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	// PixelsPerMeter = DPI * 10000.0 / 254.0;
	// DPI = PixelsPerMeter * 254.0 / 10000.0;
	// --------------------------------------------------
	// PixelsPerMeter = Pixels / PhotoSize (m)
	// PixelsPerMeter = Pixels * 1000 / PhotoSize (mm)
	// --------------------------------------------------
	// PhotoWidth  = 120 mm		<==>	ScreenWidth  = 1024 Pixel
	// PhotoHeight =  90 mm		<==>	ScreenHeight =  768 Pixel
	// --------------------------------------------------
	// DPI = Pixels * 25.4 / PhotoSize (mm);
	// --------------------------------------------------
	// DPI_X = 25.4 * 1024 / 127;	// 5寸照片尺寸为12.7cm x 8.9cm
	// DPI_Y = 25.4 *  768 /  89;
	const double dblDPI = 25.4 * m_sizeReso.cx / 127;
	/////////////////////////////////////////////////////

/*	/////////////////////////////////////////////////////
	CBitmap bmpGraphLay;
	if ( theApp.m_ImageCard.GetGraphBmp(bmpGraphLay) )
	{
		CxImage *newima = new CxImage();
		newima->CreateFromHBITMAP( (HBITMAP)bmpGraphLay );
		newima->SetXDPI(dblDPI_X);
		newima->SetYDPI(dblDPI_Y);
		pImageItem->m_pGraphLayer = newima;

		/////////////////////////////////////////////
		bmpGraphLay.DeleteObject();
		/////////////////////////////////////////////
	}
	/////////////////////////////////////////////////////
*/
	/////////////////////////////////////////////////////
	HANDLE hImageLay = NULL;
	hImageLay = theApp.m_ImageCard.GetDib();
	if (hImageLay != NULL)
	{
		CxImage *newima = new CxImage();
		newima->CreateFromHANDLE(hImageLay);
		newima->SetXDPI((long)dblDPI);
		newima->SetYDPI((long)dblDPI);
		pImageItem->m_pImageLayer = newima;

		/////////////////////////////////////////////
		if( hImageLay != NULL )
		{
			GlobalUnlock( (HGLOBAL)hImageLay );
			GlobalFree( (HGLOBAL)hImageLay );
			hImageLay = NULL;
		}
		/////////////////////////////////////////////
	}
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	CBitmap bmpScreen;
	if ( theApp.m_ImageCard.CopyScreenBmp(bmpScreen) )
	{
		CxImage *newima = new CxImage();
		newima->CreateFromHBITMAP( (HBITMAP)bmpScreen );
		newima->SetXDPI((long)dblDPI);
		newima->SetYDPI((long)dblDPI);
		pImageItem->m_pImageSrc = newima;

		/////////////////////////////////////////////
		bmpScreen.DeleteObject();
		/////////////////////////////////////////////
	}
	/////////////////////////////////////////////////////

//	pImageItem->m_pIImage->Copy( *NewDoc->GetImage() );
//	pImageItem->m_strFileName = NewDoc->GetTitle();
//	pImageItem->SaveTempFile();
//	m_plistImageItem->AddTail( pImageItem );
//	UpdateItemRect();

	if( pImageItem->GetImage() != NULL )
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.AddItem( pImageItem );

	EndWaitCursor();
	theApp.DisplayMsg("Press photo snapshot");
}

//照相预览
void CScanView::OnPhotoPreview() 
{
	m_bPreview = !m_bPreview;
	if( m_bPreview )
	{
		theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal, TRUE );
		m_bZoomFit = TRUE;
		ScreenZoom();
		theApp.m_ImageCard.SetPhotoPreview( TRUE );
		theApp.DisplayMsg("Press photo preview: ON");
	}
	else
	{
		ScreenZoom();
		theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal );
		theApp.m_ImageCard.SetPhotoPreview( FALSE );
		theApp.DisplayMsg("Press photo preview: OFF");
	}
	return;
/*	///////////////////////////////////////////////////////
	// SET FIT ZOOM MODE
	///////////////////////////////////////////////////////
	if ( !m_bZoomFit )
	{
		CSize szImage(0,0);
		double dScale;
		CString csTemp;

		///////////////////////////////////////////////
		dScale = 0;
		
		theApp.m_ImageCard.SetZoom( dScale );
		m_SemScreen.SetScrollSizes( MM_TEXT, szImage );
		m_SemScreen.InvalidateRect(NULL);
		///////////////////////////////////////////////

		///////////////////////////////////////////////
		csTemp = _T("Auto Fit");
		SetDlgItemText(IDC_STATIC_ZOOM, csTemp);
		///////////////////////////////////////////////

		m_bZoomFit = TRUE;
	}
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// SET SCREEN MODE
	///////////////////////////////////////////////////////
	if ( m_boolScreenFull )
	{
		m_SemScreen.ModifyStyle(0, WS_CAPTION);
		m_SemScreen.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		m_SemScreen.ShowWindow( SW_RESTORE );

		m_boolScreenFull = FALSE;
	}
	else if ( m_SemScreen.IsIconic() )
	{
		m_SemScreen.ShowWindow( SW_RESTORE );
	}
	else if ( m_SemScreen.IsZoomed() )
	{
		m_SemScreen.ShowWindow( SW_RESTORE );
	}

	///////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	// SET SCREEN SIZE
	///////////////////////////////////////////////////////
	CRect rcRect(0,0,0,0);

	///////////////////////////////////////////////////////
	CDC* pDC = m_SemScreen.GetDC();
	double dblPixelsPerMM_HORZ = 
		(double)pDC->GetDeviceCaps(HORZRES) / pDC->GetDeviceCaps(HORZSIZE);
	double dblPixelsPerMM_VERT = 
		(double)pDC->GetDeviceCaps(VERTRES) / pDC->GetDeviceCaps(VERTSIZE);

	rcRect.right	= (int)(SEM_dfStandImageWidth * dblPixelsPerMM_HORZ + 0.5);
	rcRect.bottom	= (int)(SEM_dfStandImageHeight * dblPixelsPerMM_VERT + 0.5);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	rcRect.right += GetSystemMetrics( SM_CXBORDER ) * 2;
	rcRect.bottom += GetSystemMetrics( SM_CYBORDER ) * 2;
	rcRect.bottom += GetSystemMetrics( SM_CYCAPTION );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	CPoint ptOffset(10,10);
	ptOffset.x	= (int)(ptOffset.x * dblPixelsPerMM_HORZ + 0.5);
	ptOffset.y	= (int)(ptOffset.y * dblPixelsPerMM_VERT + 0.5);

	rcRect.OffsetRect( ptOffset );
	///////////////////////////////////////////////////////

	m_SemScreen.SetWindowPos(&wndTopMost, 
		rcRect.left,rcRect.top, rcRect.Width(),rcRect.Height(), 0 );
	///////////////////////////////////////////////////////
*/	////////////////////////////////////////////////////////////////////////////
}

void CScanView::OnUpdatePhotoPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bPreview );
}

//设置字符编辑开关
void CScanView::OnAbc() 
{
	m_bAbc = !m_bAbc;
	if( m_bAbc )
		theApp.DisplayMsg("Press ABC: ON");
	else
		theApp.DisplayMsg("Press ABC: OFF");
}

void CScanView::OnUpdateAbc(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bAbc );
}

//设置底标显示开关
void CScanView::OnLabel() 
{
	SyncLabel( !m_bLabel );
}

void CScanView::OnUpdateLabel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bLabel );
}

void CScanView::SyncLabel( BOOL bNewVal )
{
	if( m_bLabel == bNewVal )
		return;
	m_bLabel = bNewVal;
	theApp.m_ImageCard.ShowLabel( bNewVal );

	if( m_bLabel )
		theApp.DisplayMsg("Press Label: ON");
	else
		theApp.DisplayMsg("Press Label: OFF");
}

//设置移动标尺显示开关
void CScanView::OnRuler() 
{
	m_bUScale = !m_bUScale;
	
	theApp.m_ImageCard.ShowMobileUScale( m_bUScale );

	if( m_bUScale )
		theApp.DisplayMsg("Press ruler: ON");
	else
		theApp.DisplayMsg("Press ruler: OFF");
	// 2005-1-17_放大倍数XY联合校正
	///////////////////////////////////////////////////////////
	UpdateUScaleXY();
	InvalidateRect(NULL);
}

void CScanView::OnUpdateRuler(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bUScale );
}

// 16.06.12 多移动标尺实验
void CScanView::OnRulerAdd() 
{
	// TODO: Add your command handler code here
	m_ptMobileUScaleArray[m_nMobileUScaleCounts]		= CPoint(0,50);
	m_ptMobileUScaleArray2[m_nMobileUScaleCounts]		= CPoint(50,50);
	m_nMobileUScalePixelNumArray[m_nMobileUScaleCounts]	= 50;
	theApp.m_ImageCard.SetMobileUScalePosArray( m_nMobileUScaleCounts, m_ptMobileUScaleArray[m_nMobileUScaleCounts], m_ptMobileUScaleArray2[m_nMobileUScaleCounts], RGB(255, 0, 0) );
	CString csMobileUScaleText = CalculateMobileUScale(50);
	theApp.m_ImageCard.UpdateUScaleArray(m_nMobileUScaleCounts, 50, csMobileUScaleText);
	m_nMobileUScaleCounts++;
}

void CScanView::OnUpdateRulerAdd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_nMobileUScaleCounts < MobileUScaleCounts );
}
// 16.06.12 多移动标尺实验

//设置日期显示开关
void CScanView::OnDate() 
{
	SyncDate( !m_bDate );
	if( m_bDate )
	{
		theApp.DisplayMsg("Press date: ON");
		SetTimer( nAutoTimerDate, 1000*30, NULL );
	}
	else
	{
		KillTimer( nAutoTimerDate );
		theApp.DisplayMsg("Press date: OFF");
	}
}

void CScanView::OnUpdateDate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bDate );
}

void CScanView::SyncDate( BOOL bNewVal )
{
	m_bDate = bNewVal;
/*	static CSize szLastTextSize(0,0);
	CSize szLabel = theApp.m_ImageCard.GetSizes( 2 );
	const  CPoint ptPos = CPoint(szLabel.cx/16, m_sizeReso.cy -szLabel.cy -32);

	CRect rcClear;
	rcClear.left	= ptPos.x -1;
	rcClear.right	= ptPos.x + szLastTextSize.cx +1;
	rcClear.top		= ptPos.y;
	rcClear.bottom	= ptPos.y + szLastTextSize.cy;
	theApp.m_ImageCard.GraphClear( rcClear );

	if (bNewVal == TRUE)
	{
		CTime timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
		szLastTextSize = theApp.m_ImageCard.GraphTextOut( ptPos, csCurrentTime );
	}*/
	theApp.m_ImageCard.ShowTime( bNewVal );
	if (bNewVal == TRUE)
	{
		CTime timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime;
		if( theApp.m_footerOpt.opt[0].strDisp.IsEmpty() )
			csCurrentTime = timeCurrent.Format("%Y.%m.%d %H:%M");
		else
			csCurrentTime = timeCurrent.Format(theApp.m_footerOpt.opt[0].strDisp);
		theApp.m_ImageCard.UpdateTime( csCurrentTime );
	}
}

//设置备注显示开关
void CScanView::OnNote() 
{
//	SyncDate( !m_bDate );
	m_bNote = !m_bNote;
	if( m_bNote )
	{
		theApp.DisplayMsg("Press note: ON");
	}
	else
	{
		theApp.DisplayMsg("Press note: OFF");
	}
	theApp.m_ImageCard.ShowNote( m_bNote );
}

void CScanView::OnUpdateNote(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bNote );
}

///////////////////////////////////////////////////////////
// 附件

void CScanView::OnVideoPro() 
{
	//打开视频处理器对话框
//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	pfrm->m_MainPanel.SelchangePage( 1 );
}

//设置物镜摇摆开关
void CScanView::OnWobbler() 
{
	SyncWobbler( !m_bWobbler );
	NotifyWobbler( m_bWobbler );
}

void CScanView::OnUpdateWobbler(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bWobbler );
}

void CScanView::NotifyWobbler( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Flag;
	ClassID.lVal = SemControlPanelClass_DObjectiveLensSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;

//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	pfrm->m_MainPanel.m_PagePanelAccessory.EnableCtrl( ClassID.lVal, bNewVal );
}

void CScanView::SyncWobbler( BOOL bNewVal )
{
	m_bWobbler = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_DObjectiveLensSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//设置倾斜补偿开关
void CScanView::OnLeanCompensate() 
{
	SyncLean( !m_bLean );
	NotifyLean( m_bLean );
}

void CScanView::OnUpdateLeanCompensate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bLean );
}

void CScanView::NotifyLean( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Flag;
	ClassID.lVal = SemControlPanelClass_YAdjust;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;

//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	pfrm->m_MainPanel.m_PagePanelAccessory.EnableCtrl( ClassID.lVal, bNewVal );
}

void CScanView::SyncLean( BOOL bNewVal )
{
	m_bLean = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_YAdjust;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//设置动态聚焦开关
void CScanView::OnDynamicFocus() 
{
	SyncDynFocus( !m_bDynFocus );
	NotifyDynFocus( m_bDynFocus );
}

void CScanView::OnUpdateDynamicFocus(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bDynFocus );
}

void CScanView::NotifyDynFocus( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_DFocusSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	pfrm->m_MainPanel.m_PagePanelAccessory.EnableCtrl( ClassID.lVal, bNewVal );
}

void CScanView::SyncDynFocus( BOOL bNewVal )
{
	m_bDynFocus = bNewVal;
}

//设置通道花样开关
void CScanView::OnSACP() 
{
	if( theApp.m_blSACPState )				// 09.01.15 S.A.C.P.功能默认状态为不启用
	{
		SyncSacp( !m_bSacp );
		NotifySacp( m_bSacp );
	}
}

void CScanView::OnUpdateSACP(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( theApp.m_blSACPState );	// 09.01.15 S.A.C.P.功能默认状态为不启用
	pCmdUI->SetCheck( m_bSacp );
}

void CScanView::NotifySacp( BOOL bNewVal )
{
	bNewVal;
}

void CScanView::SyncSacp( BOOL bNewVal )
{
	m_bSacp = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_SACP;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//Robinson探头
void CScanView::OnRobinsonDetector() 
{
	SyncRobinsonDetector( !m_bRobinson );
	NotifyRobinsonDetector( m_bRobinson );
}

void CScanView::OnUpdateRobinsonDetector(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bRobinson );
}

void CScanView::NotifyRobinsonDetector( BOOL bNewVal )
{
	VARIANT ClassID, SerialNumber, Xpos, Flag;
	ClassID.lVal = SemControlPanelClass_RobinsonDetectorSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;

//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
//	pfrm->m_MainPanel.m_PagePanelAccessory.EnableCtrl( ClassID.lVal, bNewVal );
}

void CScanView::SyncRobinsonDetector( BOOL bNewVal )
{
	m_bRobinson = bNewVal;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	ClassID.lVal = SemControlPanelClass_RobinsonDetectorSwitch;
	SerialNumber.lVal = -1;
	Xpos.lVal = (long)bNewVal;
	Flag.lVal = 1;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

//设置示波器开关
void CScanView::OnOscillograph() 
{
	SyncOscillograph( !m_bOscillograph );
}

void CScanView::OnUpdateOscillograph(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bOscillograph );
}

void CScanView::SyncOscillograph( BOOL bNewVal )
{
	m_bOscillograph = bNewVal;

	theApp.m_ImageCard.ShowOscillograph( m_bOscillograph );

	if( m_bOscillograph )
		theApp.DisplayMsg("Press OSC: ON");
	else
		theApp.DisplayMsg("Press OSC: OFF");
}

//标准框显示开关
void CScanView::OnFrameRate() 
{
	// TODO: Add your command handler code here
	SyncFrameRate( !m_bFrameRate );
}

void CScanView::OnUpdateFrameRate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bFrameRate );
}

void CScanView::SyncFrameRate( BOOL bNewVal )
{
	m_bFrameRate = bNewVal;
//	theApp.m_ImageCard.ShowFrameRate( m_bFrameRate );

	// 2005-1-17_放大倍数XY联合校正
	///////////////////////////////////////////////////////////
	theApp.m_boolShowUScaleXY = m_bFrameRate; 
	UpdateUScaleXY();

	if( m_bFrameRate )
		theApp.DisplayMsg("Press frame: ON");
	else
		theApp.DisplayMsg("Press frame: OFF");
}

//标准图样
void CScanView::OnStandImage() 
{
	StandImage( 1 );
}

void CScanView::StandImage( int nDelt )
{
	// TODO: Add your command handler code here
	static int		nStyle = 0;

	nStyle += nDelt;
	switch ( nStyle )
	{
	case 0:
		ImageStandStyle0();
		break;

	case 1:
		ImageStandStyle1();
		break;
	
	case 2:
		ImageStandStyle2();
		break;
	
	case 3:
		ImageStandStyle3();
		break;

	case 4:
		ImageStandStyle4();
		break;

	case 5:
		ImageStandStyle5();
		break;

	case 6:
		ImageStandStyle6();
		break;

	case 7:
		ImageStandStyle7();
		break;

	case 8:
		ImageStandStyle8();
		break;

	case 9:
		ImageStandStyle9();
		break;

	case 10:
		ImageStandStyle10();
		break;

	case 11:
		ImageStandStyle11();
		break;

	case 12:
		ImageStandStyle12();
		break;

	case 13:
		ImageStandStyle13();
		break;

	default:
		if ( nStyle < 0 )
		{
			nStyle = 13;
			ImageStandStyle13();
		}
		else
		{
			nStyle = 0;
			ImageStandStyle0();
		}
		break;
	}
}

void CScanView::OnOptionsSetup() 
{
	// 打开设置对话框，包括聚光镜、物镜步距设置、反转开关设置；
	//					  鼠标速度调节设置；
	//					  扫描档数对应实际扫描速度设置等
	// 所有不太常用的设置均可在该对话框中添加
	// 扫描速度档
	CSheetSetup sheetSetup( IDS_SD_TITLE_PARAM_SETUP );
	CPageScanPhoto* pPageScanPhoto = (CPageScanPhoto*)sheetSetup.GetPage(0);
	pPageScanPhoto->m_nPixRateIndex1 = m_nPixRateIndex[1] /2;
	pPageScanPhoto->m_nPixRateIndex2 = m_nPixRateIndex[2] /2;
	pPageScanPhoto->m_nPixRateIndex3 = m_nPixRateIndex[3] /2;
	pPageScanPhoto->m_nPixRateIndex4 = m_nPixRateIndex[4] /2;
	// 照相速度
	switch( m_nusbPhotoPixRate )
	{
	case	64:		// 32us
		pPageScanPhoto->m_nCameraPixRateIndex = 0;
		break;
	case	128:	// 64us
		pPageScanPhoto->m_nCameraPixRateIndex = 1;
		break;
	case	256:	// 128us
		pPageScanPhoto->m_nCameraPixRateIndex = 2;
		break;
	case	512:	// 256us
		pPageScanPhoto->m_nCameraPixRateIndex = 3;
		break;
	}
	// 照相编号
	pPageScanPhoto->m_nSN = theApp.m_nPhotoSerialNumber;
	// 通道参数
	pPageScanPhoto->m_strChnName1	= theApp.m_BrdMCI.strChnName[0];
	pPageScanPhoto->m_strChnName2	= theApp.m_BrdMCI.strChnName[1];

	// 其它
	CPageLangOther* pPageLangOther = (CPageLangOther*)sheetSetup.GetPage(1);
	if( m_wLanguageID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED) )
		pPageLangOther->m_nLanguage = 0;
	else
		pPageLangOther->m_nLanguage = 1;
	// 打开图像窗口标志
	pPageLangOther->m_bOpenImageAfterSnap = m_bOpenImageAfterSnap;
	// 开机自动存储所有日志
	pPageLangOther->m_bLogAll = theApp.GetProfileInt( g_csSettingSection, 
								g_csRegEntry_nLogAll, 1 );
	// 显示屏幕放大倍数
	pPageLangOther->m_bShowScreenMag = theApp.m_sysOpt.nShowParams[SysOpt_showScrMag];
	// 显示日期
	pPageLangOther->m_bShowDate = theApp.m_sysOpt.nShowParams[SysOpt_showDate];
	// 显示备注
	pPageLangOther->m_bShowNote = theApp.m_sysOpt.nShowParams[SysOpt_showNote];
	// 显示CCD Camera窗口
	pPageLangOther->m_bShowCCD = theApp.m_sysOpt.nShowParams[SysOpt_showCCD];
	// 优先存储格式10.07.02
	pPageLangOther->m_nPriorSaveType = theApp.GetProfileInt( g_csSettingSection, 
								g_csRegEntry_nPriorSaveType, 2 );

	sheetSetup.DoModal();
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CScanView::OnParamChanged(WPARAM wParam, LPARAM lParam)
{
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CString str;
	if( lParam == 0 )
	{
		CPageScanPhoto* pPageScanPhoto = (CPageScanPhoto*)wParam;
		// 扫描速度
		m_nPixRateIndex[1] = (pPageScanPhoto->m_nPixRateIndex1 == 0 ? 1 : pPageScanPhoto->m_nPixRateIndex1 *2);
		m_nPixRateIndex[2] = (pPageScanPhoto->m_nPixRateIndex2 == 0 ? 1 : pPageScanPhoto->m_nPixRateIndex2 *2);
		m_nPixRateIndex[3] = (pPageScanPhoto->m_nPixRateIndex3 == 0 ? 1 : pPageScanPhoto->m_nPixRateIndex3 *2);
		m_nPixRateIndex[4] = (pPageScanPhoto->m_nPixRateIndex4 == 0 ? 1 : pPageScanPhoto->m_nPixRateIndex4 *2);
		// 将扫描速度写入配置文件
		theApp.SaveInOneLine(path, "Options", "PixRate", INTSTYLE, 9, m_nPixRateIndex );
		// 强制刷新
		if( m_nusbPixRate != m_nPixRateIndex[m_nusbPixRateIndex] )
			NotifyPixRate( m_nPixRateIndex[m_nusbPixRateIndex] );

		// 照相速度
		switch( pPageScanPhoto->m_nCameraPixRateIndex )
		{
		case	0:
			m_nusbPhotoPixRate = 64;	// 32us
			break;
		case	1:
			m_nusbPhotoPixRate = 128;	// 64us
			break;
		case	2:
			m_nusbPhotoPixRate = 256;	// 128us
			break;
		case	3:
			m_nusbPhotoPixRate = 512;	// 256us
			break;
		}
		// 将照相速度写入配置文件
		CString str;
		str.Format( "%d", m_nusbPhotoPixRate );
		WritePrivateProfileString( "Options", "PhotoPixRate", str, path );

		// 照相字符亮度及字符背底亮度
		UpdatePhotoSn( pPageScanPhoto->m_nSN );

		// 分辨率
		m_nResoX[4] = pPageScanPhoto->m_nResoX1 +4;
		m_nResoX[5] = pPageScanPhoto->m_nResoX2 +4;
		m_nResoX[6] = pPageScanPhoto->m_nResoX3 +4;
		m_nResoX[7] = pPageScanPhoto->m_nResoX4 +4;
		m_nResoY[4] = pPageScanPhoto->m_nResoY1 +4;
		m_nResoY[5] = pPageScanPhoto->m_nResoY2 +4;
		m_nResoY[6] = pPageScanPhoto->m_nResoY3 +4;
		m_nResoY[7] = pPageScanPhoto->m_nResoY4 +4;
		// 将分辨率写入配置文件
		theApp.SaveInOneLine( path, "Options", "ResoX", INTSTYLE, 8, m_nResoX );
		theApp.SaveInOneLine( path, "Options", "ResoY", INTSTYLE, 8, m_nResoY );
 		// 强制刷新
		if( m_dResoRate < 0 )
		{
			if( (m_sizeReso.cx != m_nResos[m_nResoX[m_nusbScanResoIndex]])
				|| (m_sizeReso.cy != m_nResos[m_nResoY[m_nusbScanResoIndex]]) )
				NotifyScanResolution( m_nusbScanResoIndex, TRUE );
		}
		else
		{
			if( (m_sizeReso.cx != m_nResos[m_nResoX[m_nusbScanResoIndex]])
				|| (m_sizeReso.cy != m_nResos[m_nResoX[m_nusbScanResoIndex]] *m_dResoRate) )
 				NotifyScanResolution( m_nusbScanResoIndex, TRUE );
		}

		// 聚光镜快速设置
		long lCL[4];
		double dDisplay = (double)1.0/32.0;
		lCL[0]	= (long)(pPageScanPhoto->m_dCL1 / dDisplay);
		lCL[1]	= (long)(pPageScanPhoto->m_dCL2 / dDisplay);
		lCL[2]	= (long)(pPageScanPhoto->m_dCL3 / dDisplay);
		lCL[3]	= (long)(pPageScanPhoto->m_dCL4 / dDisplay);
		theApp.SaveInOneLine( path, "Options", "CLFast", LONGSTYLE, 4, lCL );
		// 通道参数
		theApp.m_BrdMCI.strChnName[0]	= pPageScanPhoto->m_strChnName1;
		theApp.m_BrdMCI.strChnName[1]	= pPageScanPhoto->m_strChnName2;
		if( (theApp.m_BrdMCI.nChnIndex[0] != pPageScanPhoto->m_nChn1)
			|| (theApp.m_BrdMCI.nChnIndex[1] != pPageScanPhoto->m_nChn2) )
		{
			theApp.m_BrdMCI.nChnIndex[0]	= pPageScanPhoto->m_nChn1;
			theApp.m_BrdMCI.nChnIndex[1]	= pPageScanPhoto->m_nChn2;
			// 先停止扫描，更新完参数后再重新发起扫描
			if ( m_nusbScanType != USB_ST_Stop )
			{
				int nScanType = m_nusbScanType;
				USB_ChangeScan( USB_ST_Stop );
				theApp.m_ImageCard.SetParams( 11, theApp.m_BrdMCI.nChnIndex[0] );
				theApp.m_ImageCard.SetParams( 12, theApp.m_BrdMCI.nChnIndex[1] );
				CheckCCDSwitch();
				USB_ChangeScan( nScanType );
			}
			else
			{
				theApp.m_ImageCard.SetParams( 11, theApp.m_BrdMCI.nChnIndex[0] );
				theApp.m_ImageCard.SetParams( 12, theApp.m_BrdMCI.nChnIndex[1] );
				CheckCCDSwitch();
			}
		}
		str.Format( "%s,%s", theApp.m_BrdMCI.strChnName[0], theApp.m_BrdMCI.strChnName[1] );
		WritePrivateProfileString( "Options", "ChnNames", str, path );
		str.Format( "%d,%d", theApp.m_BrdMCI.nChnIndex[0], theApp.m_BrdMCI.nChnIndex[1] );
		WritePrivateProfileString( "Options", "ChnIndex", str, path );
	}
	else if( lParam == 1 )
	{
		CPageLangOther* pPageLangOther = (CPageLangOther*)wParam;
		// 语言
		if( pPageLangOther->m_nLanguage == 0 )
		{
			WORD lid = MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
			if( theApp.m_wLanguageID != lid )
			{
				theApp.m_wLanguageID = lid;
//				::SetThreadLocale( MAKELCID( theApp.m_wLanguageID, SORT_DEFAULT ) );
				theApp.m_LanguageManagerCtrl.SetLanguageID( theApp.m_wLanguageID );
				theApp.Localize();
				theApp.WriteProfileInt( g_csSettingSection, 
							g_csRegEntry_nLanguage, theApp.m_wLanguageID );
			}
		}
		else if( pPageLangOther->m_nLanguage == 1 )
		{
			WORD lid = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
			if( theApp.m_wLanguageID != lid )
			{
				theApp.m_wLanguageID = lid;
//				::SetThreadLocale( MAKELCID( theApp.m_wLanguageID, SORT_DEFAULT ) );
				theApp.m_LanguageManagerCtrl.SetLanguageID( theApp.m_wLanguageID );
				theApp.Localize();
				theApp.WriteProfileInt( g_csSettingSection, 
							g_csRegEntry_nLanguage, theApp.m_wLanguageID );
			}
		}
		// 打开图像窗口标志
		m_bOpenImageAfterSnap = pPageLangOther->m_bOpenImageAfterSnap;
		// 开机自动存储所有日志
		theApp.WriteProfileInt( g_csSettingSection,
								g_csRegEntry_nLogAll, pPageLangOther->m_bLogAll );
		// 显示屏幕放大倍数
		int nShowScreenMag = theApp.m_sysOpt.nShowParams[SysOpt_showScrMag];
		theApp.m_sysOpt.nShowParams[SysOpt_showScrMag] = pPageLangOther->m_bShowScreenMag;
		if( nShowScreenMag != theApp.m_sysOpt.nShowParams[SysOpt_showScrMag] )
			theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal );
		// 显示日期
		theApp.m_sysOpt.nShowParams[SysOpt_showDate] = pPageLangOther->m_bShowDate;
		if( theApp.m_sysOpt.nShowParams[SysOpt_showDate] != m_bDate )
			OnDate();
		// 显示备注
		theApp.m_sysOpt.nShowParams[SysOpt_showNote] = pPageLangOther->m_bShowNote;
		if( theApp.m_sysOpt.nShowParams[SysOpt_showNote] != m_bNote )
			OnNote();
		// 显示CCD Camera窗口
		theApp.m_sysOpt.nShowParams[SysOpt_showCCD] = pPageLangOther->m_bShowCCD;
		str.Format( "%d,%d,%d,%d,%d", theApp.m_sysOpt.nShowParams[SysOpt_showLabelTrans],
										theApp.m_sysOpt.nShowParams[SysOpt_showScrMag],
										theApp.m_sysOpt.nShowParams[SysOpt_showDate],
										theApp.m_sysOpt.nShowParams[SysOpt_showNote],
										theApp.m_sysOpt.nShowParams[SysOpt_showCCD] );
		WritePrivateProfileString( "Options", "ShowParams", str, path );
		// 优先存储格式10.07.02
		theApp.m_nPriorSaveType = pPageLangOther->m_nPriorSaveType;
		theApp.WriteProfileInt( g_csSettingSection,
								g_csRegEntry_nPriorSaveType, theApp.m_nPriorSaveType );
		// 端口设置
		theApp.m_CPS3603s.nPort = pPageLangOther->m_nPortCPS;
		theApp.m_cfgVacCCG.nPort = pPageLangOther->m_nPortCCG +1;
		str.Format( "%d,%d", theApp.m_CPS3603s.nPort +1, theApp.m_cfgVacCCG.nPort );
		WritePrivateProfileString( "Options", "CommPorts", str, path );
	}
	else if( lParam == 2 )
	{
		CPageSys* pPageSys = (CPageSys*)wParam;
		// 颜色
		int i;
		for( i=0; i<10; i++ )
		{
			theApp.m_sysOpt.color[i]	= pPageSys->m_sysOpt.color[i];
			theApp.m_ImageCard.SetColor( i, pPageSys->m_sysOpt.color[i] );
		}
		theApp.SaveInOneLine( path, "Options", "Colors", LONGSTYLE, 10, theApp.m_sysOpt.color );
		// 背景透明
		theApp.m_sysOpt.nShowParams[SysOpt_showLabelTrans]	= pPageSys->m_sysOpt.nShowParams[SysOpt_showLabelTrans];
		theApp.m_ImageCard.SetTransparent( theApp.m_sysOpt.nShowParams[SysOpt_showLabelTrans] );
		str.Format( "%d,%d,%d,%d,%d", theApp.m_sysOpt.nShowParams[SysOpt_showLabelTrans],
										theApp.m_sysOpt.nShowParams[SysOpt_showScrMag],
										theApp.m_sysOpt.nShowParams[SysOpt_showDate],
										theApp.m_sysOpt.nShowParams[SysOpt_showNote],
										theApp.m_sysOpt.nShowParams[SysOpt_showCCD] );
		WritePrivateProfileString( "Options", "ShowParams", str, path );
		// 线宽
		theApp.m_sysOpt.nLineWidth		= pPageSys->m_sysOpt.nLineWidth;
		theApp.m_ImageCard.SetParams( 10, theApp.m_sysOpt.nLineWidth );
		str.Format( "%d", theApp.m_sysOpt.nLineWidth );
		WritePrivateProfileString( "Options", "LineWidth", str, path );
		// 字体
		for( i=0; i<3; i++ )
		{
			theApp.m_sysOpt.fName[i]	= pPageSys->m_sysOpt.fName[i];
			theApp.m_ImageCard.SetFontName( i, pPageSys->m_sysOpt.fName[i] );
		}
		str.Format( "%s,%s,%s", theApp.m_sysOpt.fName[0], theApp.m_sysOpt.fName[1], theApp.m_sysOpt.fName[2] );
		WritePrivateProfileString( "Options", "FontNames", str, path );
		// 字体大小
		for( i=0; i<3; i++ )
		{
			theApp.m_sysOpt.fSize[i]	= pPageSys->m_sysOpt.fSize[i];
			theApp.m_ImageCard.SetFontSize( i, pPageSys->m_sysOpt.fSize[i] );
		}
		theApp.SaveInOneLine( path, "Options", "FontSizes", INTSTYLE, 3, theApp.m_sysOpt.fSize );
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////

void CScanView::OnOptionsListenStart() 
{
	// TODO: Add your command handler code here
	ListenStart();
}

void CScanView::ListenStart()
{
	m_pcommServer = new CTCPServer(this->m_hWnd);
	m_pcommSendImgData = new CTCPServer(this->m_hWnd);
	m_pcommServer->SetPort(KYKYComm_ListenPort1);
	m_pcommSendImgData->SetPort(KYKYComm_ListenPort2);

	if (m_pcommServer->StartListen())
	{
		if (m_pcommSendImgData->StartListen())
		{
			m_bCommListenStarted = TRUE;
		}
	}
}

void CScanView::OnUpdateOptionsListenStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( !m_bCommListenStarted );
}

void CScanView::OnOptionsListenStop() 
{
	// TODO: Add your command handler code here
	KYKYComm_EndConnectThread();
	if( m_pcommServer != NULL )
	{
		delete m_pcommServer;
		m_pcommServer = NULL;
	}
	if (m_pcommSendImgData != NULL)
	{
		delete m_pcommSendImgData;
		m_pcommSendImgData = NULL;
	}
	m_bCommListenStarted = FALSE;
}

void CScanView::OnUpdateOptionsListenStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_bCommListenStarted );
}

LRESULT CScanView::OnKYKYComm_ConnectThread(WPARAM wParam, LPARAM lParam)
{
/*	if (m_commCmdThd == NULL)
	{
		m_commCmdThd	= (CWinThread*)lParam;	// 命令线程先建立
		theApp.DisplayMsg( "connect thread 1" );
	}
	else
	{
		m_commSendImageThd = (CWinThread*)lParam;	// 图像数据线程后建立		
		theApp.DisplayMsg( "connect thread 2" );
	}	
*/	m_commCmdThd	= (CWinThread*)lParam;	// 命令线程先建立
	theApp.DisplayMsg( "connect other program" );
	return 0;	
}

LRESULT CScanView::OnKYKYComm_Connect(WPARAM wParam, LPARAM lParam)
{
	if( !m_commCmdThd )
		return 0;
	BYTE brecvCommand = BYTE(wParam);
	switch (brecvCommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
		if( theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.xy.bReady )
		{
			// 因光栅尺参数是计算位置参数的基础，所以先传递
//			KYKYComm_StageSendGrating();
			m_szCommStageGrating.cx = theApp.m_Stage.xy.nGratingX;
			m_szCommStageGrating.cy = theApp.m_Stage.xy.nGratingY;
		}
		break;
	}
	return 0;	
}

LRESULT CScanView::OnKYKYComm_ReplyAsk(WPARAM wParam,LPARAM lParam)
{
	if( !m_commCmdThd )
		return 1;

	BYTE brecvCommand = BYTE(wParam);
	switch (brecvCommand)
	{
	case	KYKYComm_CMD_Sys_Disconnect:
		KYKYComm_EndConnectThread();
		break;

	case	KYKYComm_CMD_Func_AutoBC:
		FuncAutoBrightContrast();
		break;

	case	KYKYComm_CMD_ScanCtrl_Freeze:
	case	KYKYComm_CMD_ScanCtrl_Normal:
	case	KYKYComm_CMD_ScanCtrl_ActiveArea:
	case	KYKYComm_CMD_ScanCtrl_Line_HORZ:
	case	KYKYComm_CMD_ScanCtrl_Line_VERT:
	case	KYKYComm_CMD_ScanCtrl_Spot:
	case	KYKYComm_CMD_ScanCtrl_Sync:
		{
			int nIndex = brecvCommand -KYKYComm_CMD_ScanCtrl_Freeze;
			int nCommand = ID_SCAN_TYPE_FREEZE;
			nCommand += nIndex;
			OnScanTypeChanged(nCommand);
		}
		break;

	case	KYKYComm_CMD_ScanCtrl_Speed1:
	case	KYKYComm_CMD_ScanCtrl_Speed2:
	case	KYKYComm_CMD_ScanCtrl_Speed3:
	case	KYKYComm_CMD_ScanCtrl_Speed4:
		{
			int nIndex = brecvCommand - KYKYComm_CMD_ScanCtrl_Speed1;
			int nCommand = ID_SCAN_SPEED_1;
			nCommand += nIndex;
			OnScanSpeedChanged(nCommand);
			TRACE("SPEED %d\r\n",nIndex+1);
		}		
		break;

	case	KYKYComm_CMD_ScanCtrl_Reso1:
	case	KYKYComm_CMD_ScanCtrl_Reso2:
	case	KYKYComm_CMD_ScanCtrl_Reso3:
	case	KYKYComm_CMD_ScanCtrl_Reso4:
		{
			int nIndex = brecvCommand - KYKYComm_CMD_ScanCtrl_Reso1;
			int nCommand = ID_SCAN_RESOLUTION_5;
			nCommand += nIndex;		
			OnScanResolutionChanged(nCommand);
		}
		break;

	case	KYKYComm_CMD_Sys_GetVersion:
		{
//			DWORD versionNumber;
//			versionNumber = MAKEWORD(1,1);   //(low,high) (sofware,hardware)
			WORD versionNumber;
			versionNumber = theApp.GetVersionInfo();
			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, wParam, versionNumber);
		}
		break;

	case	KYKYComm_CMD_Sys_GetSEMType:
		KYKYComm_SendSEMType();
		break;

	case	KYKYComm_CMD_Get_Magnification:
		KYKYComm_SendMag();
		break;

	case	KYKYComm_CMD_Set_Magnification:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Magnifier;
			Flag.lVal		= 1;
			Xpos.dblVal		= msg->fValue;
			theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		KYKYComm_SendSRotAngInit();
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		KYKYComm_SendSRotAng();
		break;
	case	KYKYComm_CMD_Set_ScanRotAngle:
		{
			KYKYCommMsg_SendDataC1i* msg = (KYKYCommMsg_SendDataC1i*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_ScanRotate;
			Flag.lVal		= 1;
			Xpos.lVal		= msg->nValue -m_nCommScanRotAngInit;
			theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Get_WorkDistance:
		KYKYComm_SendWD();
		break;

	case	KYKYComm_CMD_Get_Brightness:
		KYKYComm_SendBrt();
		break;

	case	KYKYComm_CMD_Get_Brightness2:
		KYKYComm_SendBrt2();
		break;

	case	KYKYComm_CMD_Set_Brightness:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Brightness;
			Flag.lVal		= 1;
			Xpos.lVal		= (int)(msg->fValue *2048 /100);
			theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Set_Brightness2:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Signal2Brightness;
			Flag.lVal		= 1;
			Xpos.lVal		= (int)(msg->fValue *2048 /100);
			theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;
		
	case	KYKYComm_CMD_Get_Contrast:
		KYKYComm_SendCst();
		break;

	case	KYKYComm_CMD_Set_Contrast:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			Flag.lVal		= 1;
			if( theApp.m_dwHVtype < 1 )
			{
				ClassID.lVal	= SemControlPanelClass_Contrast;
				Xpos.lVal		= (int)(msg->fValue *255 /100);
				theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			}
			else
			{
				ClassID.lVal = SemControlPanelClass_SP_PMT;
				Xpos.lVal = (int)(msg->fValue *400 /100);
				theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
				CString strX = theApp.m_SemCtrl.m_SemCore.m_SP_PMT.FormatText();
				if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_ctrlContrast.SyncText( strX, strX, Flag );
				theApp.m_Spellman.SetParam( 35, Xpos.lVal );
			}
		}
		break;

	case	KYKYComm_CMD_Get_Stigmators:
		KYKYComm_SendStigmator();
		break;
	case	KYKYComm_CMD_Set_Stigmators:
		{
			KYKYCommMsg_SendDataC2i* msg = (KYKYCommMsg_SendDataC2i*)lParam;
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal	= SemControlPanelClass_Stigmator;
			Flag.lVal		= 3;
			if( msg->szSize.cx < 2048 )
				Xpos.lVal		= msg->szSize.cx;
			if( msg->szSize.cy < 2048 )
				Ypos.lVal		= msg->szSize.cy;
			theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		}
		break;

	case	KYKYComm_CMD_Get_VacStatus:
		KYKYComm_SendVacStatus();
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		KYKYComm_SendGunStatus();
		break;

	case	KYKYComm_CMD_Get_HighVoltageType:
		KYKYComm_SendHVType();
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		KYKYComm_SendHV();
		break;
	case	KYKYComm_CMD_Set_HighVoltage:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				if( msg->fValue < 10000 )
					Xpos.lVal	= (LONG)(msg->fValue /100);
				else
					Xpos.lVal	= (LONG)(msg->fValue /1000 -10 +100);
				if( theApp.m_dwHVtype < 1 )
				{
					ClassID.lVal	= SemControlPanelClass_CPS_HV;
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
				}
				else
				{
					ClassID.lVal	= SemControlPanelClass_SP_HV;
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeHighVoltage_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE, TRUE );
				}
			}
		}
		break;

	case	KYKYComm_CMD_Get_Fila:
		KYKYComm_SendFilament();
		break;
	case	KYKYComm_CMD_Set_Fila:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if(((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_hWnd != NULL)
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				Xpos.lVal			= (LONG)(msg->fValue *100);
				if( theApp.m_dwHVtype < 1 )
				{
					ClassID.lVal	= SemControlPanelClass_CPS_FilamentCurrent;
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
				}
				else
				{
					ClassID.lVal	= SemControlPanelClass_SP_Heater;
					((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeFilament_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
				}
			}
		}
		break;

	case	KYKYComm_CMD_Get_Bias:
		KYKYComm_SendBias();
		break;
	case	KYKYComm_CMD_Set_Bias:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if( theApp.m_dwHVtype < 1 )
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				Xpos.lVal			= (LONG)(msg->fValue);
				ClassID.lVal		= SemControlPanelClass_CPS_Bias;
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBias_3603( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
			}
		}
		break;

	case	KYKYComm_CMD_Set_BeamCurrent:
		{
			KYKYCommMsg_SendDataC1f* msg = (KYKYCommMsg_SendDataC1f*)lParam;
			if( theApp.m_dwHVtype > 0 )
			{
				VARIANT ClassID, SerialNumber, Xpos, Flag;
				SerialNumber.lVal	= -1;
				Flag.lVal			= -1;
				Xpos.lVal			= (LONG)(msg->fValue);
				ClassID.lVal		= SemControlPanelClass_SP_Bias;
				((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.ChangeBeam_771( ClassID, SerialNumber, Xpos, Xpos, Flag, TRUE, TRUE );
			}
		}
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		KYKYComm_SendBeamCurrent();
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		KYKYComm_StageSendStatus();
		break;

	case	KYKYComm_CMD_Stage_GetGrating:
		KYKYComm_StageSendGrating();
		break;

	case	KYKYComm_CMD_Stage_GetRange:
	case	KYKYComm_CMD_Stage_GetRange5:
		KYKYComm_StageSendRange();
		break;

	case	KYKYComm_CMD_Stage_GetPosXY:
	case	KYKYComm_CMD_Stage_GetPos5:
		KYKYComm_StageSendPosition();
		break;

	case	KYKYComm_CMD_Stage_SetPosXY:
		{
// 			KYKYCommMsg_SendDataC2i* msg = (KYKYCommMsg_SendDataC2i*)lParam;
// 			theApp.m_Stage.xy.ptCurPosAb.x = msg->szSize.cx;
// 			theApp.m_Stage.xy.ptCurPosAb.y = msg->szSize.cy;
			KYKYCommMsg_SendDataC2f* msg = (KYKYCommMsg_SendDataC2f*)lParam;
			theApp.m_Stage.xy.ptCurPosAb.x = (int)(msg->fValue1 *1000 *1000 /m_szCommStageGrating.cx);
			theApp.m_Stage.xy.ptCurPosAb.y = (int)(msg->fValue2 *1000 *1000 /m_szCommStageGrating.cy);
			if( theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.xy.bReady )
			{
				char szBuffer[64];
				CString str;
				str.Format( "Set Position from third, pos = %d, %d", theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
				theApp.DisplayMsg( str );
				str.Format( "SHXY;PA %ld,%ld;BGXY", theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
				theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
				theApp.DisplayMsg( str );
			}
			else
			{
				CString str;
				str.Format( "Set Position from third. Has NO auto-stage or Stage is NOT ready" );
				theApp.DisplayMsg( str );
			}
		}
		break;

	case	KYKYComm_CMD_Stage_SetPos5:
		{
			KYKYCommMsg_SendDataC5f* msg = (KYKYCommMsg_SendDataC5f*)lParam;
			if( theApp.m_Stage.nHasAutoStage > 0  && StageMC600_IsReady() )
			{
				StageMC600_Move4Axis( msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4 );
				CString str;
				str.Format( "Set Position from third, pos = %.2f, %.2f, %.2f, %.1f", msg->Values.fValue1, msg->Values.fValue2, msg->Values.fValue3, msg->Values.fValue4 );
				theApp.DisplayMsg( str );
			}
			else
			{
				CString str;
				str.Format( "Set Position from third. Has NO auto-stage or Stage is NOT ready" );
				theApp.DisplayMsg( str );
			}
		}
		break;

	case	KYKYComm_CMD_Stage_Stop:
		{
			if( theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.xy.bReady )
			{
				theApp.m_DMCWin.Command( "ST" );
				Sleep( 100 );
				theApp.m_DMCWin.Command( "MO" );
				theApp.DisplayMsg( "Stop stage from third" );
			}
			else if( theApp.m_Stage.nHasAutoStage > 0  && StageMC600_IsReady() )
				StageMC600_StopMove();
		}
		break;

	case	KYKYComm_CMD_Stage_GoHome:
		{
			if( theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.xy.bReady )
				theApp.Stage_XY_FindIndex();
			else if( theApp.m_Stage.nHasAutoStage > 0  && StageMC600_IsReady() )
				StageMC600_GoHome();
		}
		break;
	}

	if ( !m_commSendImageThd )
		return 1;

	switch (brecvCommand )
	{
	case	KYKYComm_CMD_Image_Sync:
			m_bCommSendImgData = TRUE;
			KYKYComm_SimulImage();
			break;
	case	KYKYComm_CMD_Image_Stop:
			m_bCommBatchSendEnd = TRUE;
			m_bCommSendImgData = FALSE;
			break;
	}

	return 0;
}

void CScanView::KYKYComm_EndConnectThread()
{
	m_bCommBatchSendEnd = TRUE;
	if (m_commCmdThd != NULL)
	{
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Disconnect, 0);
		m_commCmdThd->PostThreadMessage(WM_QUIT, 0, 0);
		m_commCmdThd = NULL;	
		theApp.DisplayMsg( "Disconnect thread" );
	}
/*	if (m_commSendImageThd != NULL)
	{
		m_commSendImageThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Disconnect, 0);
		m_commSendImageThd->PostThreadMessage(WM_QUIT, 0, 0);
		m_commSendImageThd = NULL;
		theApp.DisplayMsg( "Disconnect thread 2" );
	}*/
}

void CScanView::KYKYComm_Snap()
{
	
}
void CScanView::KYKYComm_SimulImage()
{
	m_bCommBatchSendEnd = TRUE;

	KYKYComm_SendReso();
	Sleep(5);
	m_bCommBatchSendEnd = FALSE;

	DWORD id;
	HANDLE h = CreateThread(NULL, 0, KYKYComm_BatchSendImageData, this, 0, &id);
	CloseHandle(h);	
}

void CScanView::KYKYComm_SendReso()
{
	if (m_commCmdThd == NULL)
		return;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_ScanCtrl_Reso, (LPARAM)(&m_sizeReso));
	TRACE("imageReso = (%d, %d)\r\n", m_sizeReso.cx, m_sizeReso.cy);
}

void CScanView::KYKYComm_SendSEMType()
{
	if (m_commCmdThd == NULL)
		return;
	int nPos = theApp.m_strLogo.Find( "6" );
	CString strType = theApp.m_strLogo.Right( theApp.m_strLogo.GetLength() - nPos );
	m_nCommSEMType = atoi(strType);
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_GetSEMType, (LPARAM)(&m_nCommSEMType));
	
	CString str;
	str.Format( "Send SEM type = %s", strType );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendMag()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommMag = theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Magnification, (LPARAM)(&m_dCommMag));

	CString str;
	str.Format( "Send magnification = %.1f", m_dCommMag );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendSRotAngInit()
{
	if (m_commCmdThd == NULL)
		return;
	double dParam = 0.0;
	theApp.m_SemCtrl.m_SemCore.m_ScanRotate.GetParams( 1, dParam );
	m_nCommScanRotAngInit = (int)dParam;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_ScanRotAngleInit, (LPARAM)(&m_nCommScanRotAngInit));

	CString str;
	str.Format( "Send scan rotate angle init = %d", m_nCommScanRotAngInit );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendSRotAng()
{
	if (m_commCmdThd == NULL)
		return;
	double dParam = 0.0;
	theApp.m_SemCtrl.m_SemCore.m_ScanRotate.GetParams( 2, dParam );
	m_nCommScanRotAng = (int)dParam;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_ScanRotAngle, (LPARAM)(&m_nCommScanRotAng));

	CString str;
	str.Format( "Send scan rotate angle actual = %d", m_nCommScanRotAng );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendWD()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommWD = theApp.m_SemCtrl.m_SemCore.m_WD.GetPos().dblVal;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_WorkDistance, (LPARAM)(&m_dCommWD));

	CString str;
	str.Format( "Send WD = %.1f", m_dCommWD );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendBrt()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommBrt = theApp.m_SemCtrl.m_SemCore.m_Brightness.GetPos().lVal *100.0 /2048;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Brightness, (LPARAM)(&m_dCommBrt));

	CString str;
	str.Format( "Send brightness = %.2f", m_dCommBrt );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendBrt2()
{
	if (m_commCmdThd == NULL)
		return;
	m_dCommBrt2 = theApp.m_SemCtrl.m_SemCore.m_Signal2Brightness.GetPos().lVal *100.0 /2048;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Brightness2, (LPARAM)(&m_dCommBrt2));

	CString str;
	str.Format( "Send brightness 2 = %.2f", m_dCommBrt2 );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendCst()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_dwHVtype < 1 )
		m_dCommCst = theApp.m_SemCtrl.m_SemCore.m_Contrast.GetPos().lVal *100.0 /255;
	else
		m_dCommCst = theApp.m_SemCtrl.m_SemCore.m_SP_PMT.GetPos().lVal *100.0 /400;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Contrast, (LPARAM)(&m_dCommCst));

	CString str;
	str.Format( "Send contrast = %.3f", m_dCommCst );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendStigmator()
{
	if (m_commCmdThd == NULL)
		return;
	m_szCommStigmator.cx = theApp.m_SemCtrl.m_SemCore.m_StigmatorX.GetPos().lVal;
	m_szCommStigmator.cy = theApp.m_SemCtrl.m_SemCore.m_StigmatorY.GetPos().lVal;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Stigmators, (LPARAM)(&m_szCommStigmator));

	CString str;
	str.Format( "Send Stigmator = (%d, %d)", m_szCommStigmator.cx, m_szCommStigmator.cy );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendVacStatus()
{
	if (m_commCmdThd == NULL)
		return;
	m_nCommStatusVac = 0;
	if( theApp.m_VacTCAmp.nState == 2 )	// 真空已好，允许开V1阀
		m_nCommStatusVac = 1;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_VacStatus, (LPARAM)(&m_nCommStatusVac));

	CString str;
	str.Format( "Send Vac status = %d", m_nCommStatusVac );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendGunStatus()
{
	if (m_commCmdThd == NULL)
		return;
	m_nCommStatusGun = 0;
	if( theApp.m_dwHVtype < 1 )	// 3603
	{
		if( theApp.m_blUnlock_HV )
		{
			int nCheck1 = ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnHV.GetCheck();
			int nCheck2 = ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnBias.GetCheck();
			int nCheck3 = ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelAll2.m_btnFilament.GetCheck();
			if( nCheck1 && nCheck2 && nCheck3 )
				m_nCommStatusGun = 1;
		}
	}
	else
	{
		if( theApp.m_blUnlock_HV )
		{
			if( theApp.m_Spellmans.status.bstatusSwitch == 0x1F )
				m_nCommStatusGun = 1;
		}
	}
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_GunStatus, (LPARAM)(&m_nCommStatusGun));

	CString str;
	str.Format( "Send Gun status = %d", m_nCommStatusGun );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendHVType()
{
	if (m_commCmdThd == NULL)
		return;
	m_nCommHVType = theApp.m_dwHVtype;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_HighVoltageType, (LPARAM)(&m_nCommHVType));

	CString str;
	str.Format( "Send HV type = %d", m_nCommHVType );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendHV()
{
	if (m_commCmdThd == NULL)
		return;
//	m_dCommHV = theApp.m_dHV *1000;
	if( theApp.m_dwHVtype < 1 )
		m_dCommHV = theApp.m_CPS3603.dVoutA;
	else
		m_dCommHV = theApp.m_Spellmans.pM3.dVE;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_HighVoltage, (LPARAM)(&m_dCommHV));

	CString str;
	str.Format( "Send HV = %.1f KV", m_dCommHV /1000.0 );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendFilament()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_dwHVtype < 1 )
		m_dCommFila = theApp.m_CPS3603.dIoutF;
	else
		m_dCommFila = theApp.m_Spellmans.pM3.dIH;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Fila, (LPARAM)(&m_dCommFila));

	CString str;
	str.Format( "Send Fila = %.3f A", m_dCommFila );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_SendBias()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_dwHVtype < 1 )	// 3603
	{
		m_dCommBias = theApp.m_CPS3603.dVoutE;
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Bias, (LPARAM)(&m_dCommBias));
		CString str;
		str.Format( "Send Bias = %.3f V", m_dCommBias );
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SendBeamCurrent()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_dwHVtype < 1 )
		m_dCommBeamCurrent = theApp.m_CPS3603.dIoutA * 1e6;
	else
		m_dCommBeamCurrent = theApp.m_Spellmans.pM.dII;
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_BeamCurrent, (LPARAM)(&m_dCommBeamCurrent));

	CString str;
	str.Format( "Send Beam current = %.2f uA", m_dCommBeamCurrent /1000.0 );
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_StageSendStatus()
{
	if (m_commCmdThd == NULL)
		return;

	m_btCommStageStatus = 0x00;
	if( theApp.m_Stage.nHasAutoStage > 0 )		// 有自动台
	{
		if( theApp.m_Stage.nHasAutoStage == 1 )
		{
			// 2轴
			m_szCommStageGrating.cx = theApp.m_Stage.xy.nGratingX;
			m_szCommStageGrating.cy = theApp.m_Stage.xy.nGratingY;
			m_btCommStageStatus |= 0x01;			// 2轴
			char szBuffer[64];
			CString str;
			str.Format( "MG_BGX" );
			theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			int nIsXMoving = atoi(szBuffer);
			str.Format( "MG_BGY" );
			theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			int nIsYMoving = atoi(szBuffer);
			
			if( theApp.m_Stage.xy.bReady )			// XY自动台连接正常
				m_btCommStageStatus |= 0x04;
			if( nIsXMoving > 0 || nIsYMoving > 0 )	// XY自动台正在运动中
				m_btCommStageStatus |= 0x08;
// 			if( theApp.m_Stage.xy.bRev )			// XY对调
// 				m_btCommStageStatus |= 0x10;
// 			if( theApp.m_Stage.xy.sXLD == "+" )		// X反向：向左为+
// 				m_btCommStageStatus |= 0x20;
// 			if( theApp.m_Stage.xy.sYLD == "+" )		// Y反向：向上为+
// 				m_btCommStageStatus |= 0x40;
		}
		else if( theApp.m_Stage.nHasAutoStage == 2 )
		{
			// 4轴
			m_btCommStageStatus |= 0x02;			// 4轴
			BOOL bReady = StageMC600_IsReady();		// 自动台连接正常
			if( bReady )
				m_btCommStageStatus |= 0x04;
			BOOL bIsMoving = StageMC600_IsMoving();	// 自动台正在运动中
			if( bIsMoving )
				m_btCommStageStatus |= 0x08;
		}
		else
		{
			// 5轴
			m_btCommStageStatus |= 0x03;			// 5轴
		}
	}
	m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetStatus, (LPARAM)(&m_btCommStageStatus));

	CString str;
	str.Format( "Send Stage status = 0x%02X", m_btCommStageStatus);
	theApp.DisplayMsg( str );
}

void CScanView::KYKYComm_StageSendGrating()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_Stage.nHasAutoStage == 1 )		// 有XY自动台
	{
		m_szCommStageGrating.cx = theApp.m_Stage.xy.nGratingX;
		m_szCommStageGrating.cy = theApp.m_Stage.xy.nGratingY;
		m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetGrating, (LPARAM)(&m_szCommStageGrating));

		CString str;
		str.Format( "Send Stage grating = (%d, %d)", m_szCommStageGrating.cx, m_szCommStageGrating.cy);
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_StageSendRange()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_Stage.nHasAutoStage > 0 )		// 有自动台
	{
		CString str;
		if( theApp.m_Stage.nHasAutoStage == 1 )
		{
			// 2轴
// 			m_szCommStageRange.cx = theApp.m_Stage.xy.nRangeX;
// 			m_szCommStageRange.cy = theApp.m_Stage.xy.nRangeY;
//			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetRange, (LPARAM)(&m_szCommStageRange));
//			str.Format( "Send Stage range = (%d, %d)", m_szCommStageRange.cx, m_szCommStageRange.cy);
			m_2fStageRange.fValue1 = 1.0 *theApp.m_Stage.xy.nRangeX;
			m_2fStageRange.fValue2 = 1.0 *theApp.m_Stage.xy.nRangeY;
			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetRange, (LPARAM)(&m_2fStageRange));
			str.Format( "Send Stage range = (%.2f, %.2f)", m_2fStageRange.fValue1, m_2fStageRange.fValue2);
		}
		else if( theApp.m_Stage.nHasAutoStage == 2 )
		{
			// 4轴
			float fX1, fX2, fY1, fY2, fZ1, fZ2, fR1, fR2;
			if( StageMC600_IsReady() )
			{
				StageMC600_GetParam( 0, fX1, fY1, fZ1, fR1 );	// pos
				StageMC600_GetParam( 1, fX2, fY2, fZ2, fR2 );	// neg
				m_5fStageRange5.fValue1 = fX1 -fX2;
				m_5fStageRange5.fValue2 = fY1 -fY2;
				m_5fStageRange5.fValue3 = fZ1 -fZ2;
				m_5fStageRange5.fValue4 = fR1 -fR2;
				m_5fStageRange5.fValue5 = 0.0;
			}
			else
			{
				m_5fStageRange5.fValue1 = 0.0;
				m_5fStageRange5.fValue2 = 0.0;
				m_5fStageRange5.fValue3 = 0.0;
				m_5fStageRange5.fValue4 = 0.0;
				m_5fStageRange5.fValue5 = 0.0;
			}
			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetRange5, (LPARAM)(&m_5fStageRange5));
			str.Format( "Send Stage range = (%.2f, %.2f, %.2f, %.1f)", m_5fStageRange5.fValue1, m_5fStageRange5.fValue2, m_5fStageRange5.fValue3, m_5fStageRange5.fValue4 );
		}
		else
		{
			// 5轴
		}
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_StageSendPosition()
{
	if (m_commCmdThd == NULL)
		return;
	if( theApp.m_Stage.nHasAutoStage > 0 )		// 有自动台
	{
		CString str;
		if( theApp.m_Stage.nHasAutoStage == 1 )
		{
			// 2轴
			// 得到当前位置
 			theApp.Stage_XY_GetPosition( theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
// 			m_szCommStagePosition.cx = theApp.m_Stage.xy.ptCurPosAb.x;
// 			m_szCommStagePosition.cy = theApp.m_Stage.xy.ptCurPosAb.y;
// 			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetPosXY, (LPARAM)(&m_szCommStagePosition));
// 			str.Format( "Send Stage position = (%d, %d)", m_szCommStagePosition.cx, m_szCommStagePosition.cy);
			m_2fStagePos.fValue1 = 1.0 *theApp.m_Stage.xy.ptCurPosAb.x *m_szCommStageGrating.cx /1000 /1000;
			m_2fStagePos.fValue2 = 1.0 *theApp.m_Stage.xy.ptCurPosAb.y *m_szCommStageGrating.cy /1000 /1000;
			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetPosXY, (LPARAM)(&m_2fStagePos));
			str.Format( "Send Stage position = (%.2f, %.2f)", m_2fStagePos.fValue1, m_2fStagePos.fValue2);
		}
		else if( theApp.m_Stage.nHasAutoStage == 2 )
		{
			// 4轴
			float fX, fY, fZ, fR;
			StageMC600_GetParam( 2, fX, fY, fZ, fR );	// position
			m_5fStagePos5.fValue1 = fX;
			m_5fStagePos5.fValue2 = fY;
			m_5fStagePos5.fValue3 = fZ;
			m_5fStagePos5.fValue4 = fR;
			m_5fStagePos5.fValue5 = 0.0;
			m_commCmdThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Stage_GetPos5, (LPARAM)(&m_5fStagePos5));
			str.Format( "Send Stage position = (%.2f, %.2f, %.2f, %.1f)", m_5fStagePos5.fValue1, m_5fStagePos5.fValue2, m_5fStagePos5.fValue3, m_5fStagePos5.fValue4 );
		}
		else
		{
			// 5轴
		}
		theApp.DisplayMsg( str );
	}
}

void CScanView::KYKYComm_SetSendRow(int nCount)
{
	m_nCommRectLen = nCount;
}

DWORD WINAPI CScanView::KYKYComm_BatchSendImageData(LPVOID lparam)
{
	CScanView * pScan = (CScanView*)lparam;	
	int nDestRowStart, nDestRowEnd;
	int nRectLen = 4;	//pScan->m_nCommRectLen;     // 每次发送的行数
	theApp.m_ImageCard.GetCurrentY(nDestRowStart, nDestRowEnd);
	if (nDestRowStart <= nRectLen)
	{
		pScan->m_commmsgSendData.rcDest.bottom = 0;
	}
	else
	{
		int nbottom = nDestRowStart - nDestRowStart % nRectLen;
		pScan->m_commmsgSendData.rcDest.bottom = nbottom;
	}

	while (!pScan->m_bCommBatchSendEnd)
	{
		if (pScan->m_commmsgSendData.rcDest.bottom == pScan->m_sizeReso.cy)
			pScan->m_commmsgSendData.rcDest.bottom = 0;
// 		TRACE("m_commmsgSendData.rcDest = %d,%d,%d,%d\r\n",m_commmsgSendData.rcDest.left,m_commmsgSendData.rcDest.top,m_commmsgSendData.rcDest.right,m_commmsgSendData.rcDest.bottom);

		int nLastBottom = pScan->m_commmsgSendData.rcDest.bottom;
		CString s2;
		theApp.m_ImageCard.GetCurrentY(nDestRowStart, nDestRowEnd);
		s2.Format("nDestRowStart=%d, nDestRowEnd=%d\r\n",nDestRowStart,nDestRowEnd);

		int nGap = nDestRowEnd - nDestRowStart;
		if (nGap > 3)   //4,5
		{
			nRectLen = 16;
		}
		else if (nGap > 1)    //2,3
		{
			nRectLen = 8;
		}
		else if (nGap >0)     //1
		{
			nRectLen = 4;
		}
		// if nGap = 0;,nRectLen not change
//		TRACE("nRectLen = %d\r\n",nRectLen);
		//如果当前最新扫描开始行小于等于上次发送的最后一行，则等待
		if ((nDestRowStart - nLastBottom  <= nRectLen) && (nLastBottom <= nDestRowStart))
		{
			continue;
		}
		TRACE(s2);

		//在远程传送图片过程中，不允许扫描方式或者区域改变

		//每次发送 nRectLen 行数据
		pScan->m_commmsgSendData.rcDest = CRect(0, nLastBottom, pScan->m_sizeReso.cx, nLastBottom +nRectLen);
		//监测是否是低端数据
		if (pScan->m_commmsgSendData.rcDest.bottom >= pScan->m_sizeReso.cy)
		{
			pScan->m_commmsgSendData.rcDest.bottom = pScan->m_sizeReso.cy;
			TRACE("发送低端数据:");
		}

		TRACE("m_commmsgSendData.rcDest = %d,%d,%d,%d\r\n", pScan->m_commmsgSendData.rcDest.left, pScan->m_commmsgSendData.rcDest.top, pScan->m_commmsgSendData.rcDest.right, pScan->m_commmsgSendData.rcDest.bottom);

		int nSize = pScan->m_commmsgSendData.rcDest.Width() * pScan->m_commmsgSendData.rcDest.Height();

		BOOL bRet = theApp.m_ImageCard.Read( (LPVOID)pScan->m_commmsgSendData.bImageBuff, nSize, pScan->m_commmsgSendData.rcDest );

		LPARAM LParam;
		LParam = (LPARAM)&(pScan->m_commmsgSendData);

		pScan->m_commSendImageThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_Image_Sync, LParam);
		Sleep(5);   //CSocket.send的时间  同一台电脑时设置为5，不同电脑设置为10
	}

	return 0;	
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CScanView::OnSMvacChanged(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format( "VAC state: 0x%04X, 0x%04X", wParam, lParam );
	theApp.DisplayMsg( str );
	return 0;
}
//////////////////////////////////////////////////////////////////////////////




void CScanView::OnViewZoomIn() 
{
	if (++g_nScaleScanIndex >= g_nScaleScanIndexMax )
		g_nScaleScanIndex = g_nScaleScanIndexMax;

	if( g_nScaleScanIndex == g_nScaleScanIndexFit )
		m_bZoomFit = TRUE;
	else
		m_bZoomFit = FALSE;

//	m_bPreview = FALSE;
	theApp.m_ImageCard.SetPhotoPreview( FALSE );
	///////////////////////////////////////////////

	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.DisplayMsg("Press view zoom in");
}

void CScanView::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
	if( g_nScaleScanIndex == g_nScaleScanIndexMax )//&& !m_bZoomFit )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( !m_bPreview );
}

void CScanView::OnViewZoomOut() 
{
	if (--g_nScaleScanIndex < g_nScaleScanIndexMin )
		g_nScaleScanIndex = g_nScaleScanIndexMin;

	if( g_nScaleScanIndex == g_nScaleScanIndexFit )
		m_bZoomFit = TRUE;
	else
		m_bZoomFit = FALSE;

//	m_bPreview = FALSE;
	theApp.m_ImageCard.SetPhotoPreview( FALSE );

	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.DisplayMsg("Press view zoom out");
}

void CScanView::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
	if( g_nScaleScanIndex == g_nScaleScanIndexMin && !m_bZoomFit )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( !m_bPreview );
}

void CScanView::OnViewActualSize() 
{
	theApp.DisplayMsg("Press view actual size");
	///////////////////////////////////////////////
//	g_nScaleScanIndex = g_nZoomNormal;

	for( int i=g_nScaleScanIndexMin; i<=g_nScaleScanIndexMax; i++ )
	{
		if( g_dScaleScanTable[i] == 1.0 )
		{
			g_nScaleScanIndex	= i;
			g_nZoomNormal		= i;
			break;
		}
	}

	if( g_nScaleScanIndex == g_nScaleScanIndexFit )
		m_bZoomFit = TRUE;
	else
		m_bZoomFit = FALSE;

	m_bPreview = FALSE;
	theApp.m_ImageCard.SetPhotoPreview( FALSE );
	///////////////////////////////////////////////

//	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.m_ImageCard.SetZoom( 1.0 );
	CSize szImage = GetImageSize();
	SetScrollSizes( MM_TEXT, szImage );
	m_szTotalLog = szImage;
	UpdateItemRect( FALSE );
//	InvalidateRect(NULL);
	theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal );
}

void CScanView::OnUpdateViewActualSize(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( theApp.m_ImageCard.GetZoom() == 1.0 );
	pCmdUI->Enable( !m_bPreview );
}

void CScanView::OnViewZoomFit() 
{
//	UpdateScale();
	g_nScaleScanIndex = g_nScaleScanIndexFit;
	m_bZoomFit = TRUE;
//	m_bPreview = FALSE;
	theApp.m_ImageCard.SetPhotoPreview( FALSE );
	ScreenZoom();	// 2005-1-17_液晶屏幕XY比例问题
	theApp.DisplayMsg("Press view zoom fit");
}

void CScanView::OnUpdateViewZoomFit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bZoomFit );
	pCmdUI->Enable( !m_bPreview );
}

// 2005-1-17_液晶屏幕XY比例问题
void CScanView::ScreenZoom() 
{
//	CScanChildFrame* pfrm = (CScanChildFrame*)GetParentFrame();
//	if( pfrm->m_wndScan.m_hWnd == NULL )
//		return;

	CSize szImage, szClient;
	double dScale;
	CString csTemp;
	CRect	rcClient;

	if (( m_boolScreenFull == TRUE ) && ( m_bZoomFit == TRUE ))
	{
		szImage = CSize(0, 0);
		SetScrollSizes( MM_TEXT, szImage );
		GetClientRect( &rcClient );
		szClient = rcClient.Size();

		szImage = GetImageSize();
		if ( szClient.cy == 0 )
		{
			dScale = 0;
		}
		else if ( szClient.cx * 3 <  szClient.cy * 4 )
		{
			dScale = (double)szClient.cx / szImage.cx;
		}
		else
		{
			dScale = (double)szClient.cy / szImage.cy;
		}

		szImage.cx = (long)(szImage.cx *dScale);
		szImage.cy = (long)(szImage.cy *dScale);
		theApp.m_ImageCard.SetZoom( dScale );
		SetScrollSizes( MM_TEXT, szImage );
//		InvalidateRect(NULL);

		for (int i=g_nScaleScanIndexMin; i<=g_nScaleScanIndexMax; i++)
		{
			if ( dScale <= g_dScaleScanTable[i] )
			{
				g_nScaleScanIndex = i;		
				break;
			}
		}
	}
	else if ( m_bZoomFit == TRUE )
	{
		szImage = CSize(0,0);
		
		theApp.m_ImageCard.SetZoom( 0 );
		SetScrollSizes( MM_TEXT, szImage );
//		InvalidateRect(NULL);
	}
	else
	{
		szImage = GetImageSize();
		if( m_bSplit )
			szImage.cx *= 2;
		dScale = g_dScaleScanTable[g_nScaleScanIndex];
		theApp.m_ImageCard.SetZoom( dScale );

		szImage.cx = (long)(szImage.cx *dScale);
		szImage.cy = (long)(szImage.cy *dScale);
		SetScrollSizes( MM_TEXT, szImage );
//		InvalidateRect(NULL);
	}
//	pfrm->m_wndScan.GetClientRect( &m_rcScan );
	m_szTotalLog = szImage;
	UpdateItemRect();

	InvalidateRect(NULL);

//	SetDlgItemText(IDC_STATIC_ZOOM, csTemp);
//	UpdateData(FALSE);
	theApp.UpdateScreenMag( theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal );
}



///////////////////////////////////////////////////////////
// Auto Functions

void CScanView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static BOOL	boolBusy = FALSE;

	if ( !boolBusy )
		boolBusy = TRUE;
	else
		return;

	if( nIDEvent == nAutoTimerDate )
		SyncDate( m_bDate );
	else if( nIDEvent == 31 )
	{
		if( !StageMC600_IsMoving() )
		{
			KillTimer(31);
			AutoStage_Clear_Finish( TRUE, TRUE );
		}
	}
	else if( nIDEvent == Diag_Total )
	{
		CString str, strTemp;
		str.Format( "%04d\t", theApp.m_DL.other.lTimes++ );

		strTemp = Diag_Logging2();
		str += strTemp;
		strTemp = Diag_SCN_LNS();
		str += strTemp;
		strTemp = Diag_VAC();
		str += strTemp;
		strTemp = Diag_SPR();
		str += strTemp;
		strTemp = Diag_MCI();
		str += strTemp;

		unsigned long cbRet;
		char Buf[4096];
		sprintf( Buf, "%s\n", str );
		WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
	}
	///////////////////////////////////////
	else if( nIDEvent == nAutoFunctionTimerID )
	{
		switch (m_nAutoFuction)
		{
		case	AutoBC:
			AutoBCProcess2();
			break;
			
		case	AutoFocus:
			AutoFocusProcess();
			break;
			
		case	AutoAstigmat:
			AutoAstigmatProcess();
			break;
			
		case	AutoFilament:
			AutoFilamentProcess();
			break;
		}
	}
	else if( nIDEvent == nAutoStageXY_FI11 )
	{
		if( theApp.m_Stage.xy.nMtd < 1 )
			AutoStageXY_FI_XQ();
		else if( theApp.m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process11();
		else
			AutoStageXY_FI2_Process11();
	}
	else if( nIDEvent == nAutoStageXY_FI12 )
	{
		if( theApp.m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process12();
		else
			AutoStageXY_FI2_Process12();
	}
	else if( nIDEvent == nAutoStageXY_FI21 )
	{
		if( theApp.m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process21();
		else
			AutoStageXY_FI2_Process21();
	}
	else if( nIDEvent == nAutoStageXY_FI22 )
	{
		if( theApp.m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process22();
		else
			AutoStageXY_FI2_Process22();
	}
	else if( nIDEvent == nAutoStageXY_FI31 )
	{
		if( theApp.m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process31();
		else
			AutoStageXY_FI2_Process31();
	}
	else if( nIDEvent == nAutoStageXY_FI32 )
	{
		if( theApp.m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process32();
		else
			AutoStageXY_FI2_Process32();
	}
	else if( nIDEvent == nAutoStageXY_FI4 )
		AutoStageXY_FI_Process4();
	else if( nIDEvent == nAutoStageTrack )
		AutoStageXY_FI_TrackSecurity();
	else if( nIDEvent == nAutoStageClearXY )
		AutoStage_Clear_XY();
	///////////////////////////////////////
	else if( nIDEvent == nMCB_TimerID )
	{
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
									theApp.MCB_Ctrl_Flush_WP( g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS );
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
							theApp.MCB_Ctrl_Flush_WP( g_nMCB_Channel, g_nMCB_SwPos, g_nMCB_RotDir, g_nMCB_TicksABS );
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
	}

	boolBusy = FALSE;

//	CScrollView::OnTimer(nIDEvent);
}

//进行自动灯丝调节
void CScanView::OnAutoFilament() 
{
	FuncAutoFilament();
}

void CScanView::FuncAutoFilament()
{
	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Filament );
	dlg.m_iRangeTo	= 100;//50;		// 200 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_FILAMENT2 );

	StartAutoFilament();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoFilament();
}

void AutoFilament_Setup(long lFila)
{
	VARIANT Xpos, Ypos, Flag;
	CString strX, strY;

	Flag.ulVal	= (DWORD)0x01;
	Xpos.lVal = lFila;

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( theApp.m_dwHVtype < 1 )
	{
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );

		strX.Format( "%.2f A", Xpos.lVal *0.01 );
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncText( strX, strY, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );

		theApp.m_HVPower.SetParam( 12, Xpos.lVal *10 );
	}
	else
	{
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncPos( Xpos, Ypos, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncPos( Xpos, Ypos, Flag );

		strX.Format( "%.2f A", Xpos.lVal *0.01 );
		if( pfrm->m_wndImpPanelAll2.m_hWnd != NULL )
			pfrm->m_wndImpPanelAll2.m_ctrlHeater.SyncText( strX, strY, Flag );
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.m_PageDebug2.m_ctrlCPS_FilamentCurrent.SyncText( strX, strY, Flag );

		theApp.m_Spellman.SetParam( 34, Xpos.lVal *10 );
	}
}

void CScanView::StartAutoFilament()
{
	////////////////////////////////////////////////////////////////////
	// 保存现场
	////////////////////////////////////////////////////////////////////
	m_nusbScanTypeSaved	= m_nusbScanType;
	m_nusbPixRateSaved	= m_nusbPixRate;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 读取参数值
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 4;
	int*		pCheck	= new int[4];
	CString		str;
	sprintf(stabuff, "Auto");
	sprintf(staitembuff, "Fila");
	theApp.ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	int nMin	= pCheck[0];
	int nMax	= pCheck[1];
	int	nStep	= pCheck[2];
	int	nDebug	= pCheck[3];
	delete [] pCheck;
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	// 设定灯丝初始尝试值
	AutoFila_Initialize( nMin, nMax, nStep, nDebug, m_lFilaSaved );
	AutoFilament_Setup(m_lFilaSaved);
	theApp.DisplayMsg( "AL-Start" );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定当前扫描
	m_nusbPixRate	= 16;	// USB_SS_16us
	USB_ChangeScan( USB_ST_AutoBCArea );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 进入自动灯丝过程
	////////////////////////////////////////////////////////////////////
	m_nLoop = 0;
	m_nAutoFuction		= AutoFilament;
	m_bAutoFilaPause	= FALSE;
	theApp.m_ImageCard.FramGrayHistogramReset();
	theApp.m_ImageCard.FramGrayHistogramStart();
	if( theApp.m_dwHVtype < 1 )
	{
		theApp.m_HVPower.SetParam( 35, TRUE );	// 设置寻找饱和点标志位
		m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, 700/*nAutoBCTimerElapse*/, NULL );
	}
	else
		m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, 700/*nAutoBCTimerElapse*/, NULL );
	////////////////////////////////////////////////////////////////////
}

void CScanView::StopAutoFilament()
{
	////////////////////////////////////////////////////////////////////
	// 恢复灯丝
	////////////////////////////////////////////////////////////////////
//	AutoFilament_Setup(m_lFilaSaved);
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 停止图象分析
	////////////////////////////////////////////////////////////////////
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	theApp.m_ImageCard.FramGrayHistogramStop();
	m_nAutoFuction = AutoNULL;
	////////////////////////////////////////////////////////////////////

	if( theApp.m_dwHVtype < 1 )
		theApp.m_HVPower.SetParam( 35, FALSE );	// 设置寻找饱和点标志位

	////////////////////////////////////////////////////////////////////
	// 恢复扫描
	m_nusbPixRate = m_nusbPixRateSaved;
	USB_ChangeScan( m_nusbScanTypeSaved );
	////////////////////////////////////////////////////////////////////
}

void CScanView::AutoFilamentOK( long lFila )
{
	m_lFilaSaved = lFila;
	AutoFilament_Setup(m_lFilaSaved);

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoFilamentProcess()
{
	if ( !theApp.m_ImageCard.IsReadyFramGrayHistogram() )
		return;

	DWORD	dwTotalPixel;
	dwTotalPixel = theApp.m_ImageCard.CopyFramGrayHistogramBuff( m_dwFramGrayHistogramBuff );
	theApp.m_ImageCard.FramGrayHistogramReset();
	theApp.m_ImageCard.FramGrayHistogramStart();

	////////////////////////////////////////////////////////////////////
	// 分析灰度直方图，尝试新的亮度、对比度
	long lFilaSaved = m_lFilaSaved;
	UINT nRet = AutoFila_Analyze_GrayHistogram( m_bAutoFilaPause, dwTotalPixel,
		m_dwFramGrayHistogramBuff, m_lFilaSaved );
	////////////////////////////////////////////////////////////////////

	if( theApp.m_dwOpr > 0 )
	{
		////////////////////////////////////////////////////////////////////
		// Test 调试信息
		////////////////////////////////////////////////////////////////////
		CString		csMsg;

		csMsg.Format("%02d F=%3d FS=%3d P=%6d L=%5.1f Min=%5.1f",
			m_nLoop++, lFilaSaved, m_lFilaSaved,
			dwTotalPixel, dwTotalPixel/256.0, dwTotalPixel * dblPeakMinParam);
		theApp.DisplayMsg( csMsg );
		////////////////////////////////////////////////////////////////////
	}
	
	////////////////////////////////////////////////////////////////////
	if ( nRet == AutoFila_Analyze_Continue )
	{
		if( theApp.m_dwHVtype > 0 )
		{
			if( theApp.m_dwOpr > 0 )
			{
				CString str;
				str.Format( "In AutoFilamentProcess, S.dIB = %.3f, M.dII = %.3f", theApp.m_Spellmans.pS.dIB, theApp.m_Spellmans.pM.dII );
				theApp.DisplayMsg( str );
			}
			if( theApp.m_Spellmans.pS.dIB > 0.001 && (theApp.m_Spellmans.pM.dII -theApp.m_Spellmans.pS.dIB) / theApp.m_Spellmans.pS.dIB < 0.3 )
			{
				AutoFilament_Setup(m_lFilaSaved);
				m_bAutoFilaPause = FALSE;
			}
			else
				m_bAutoFilaPause = TRUE;
		}
		else
			AutoFilament_Setup(m_lFilaSaved);
	}
	else
	{
		////////////////////////////////////////////////////////////
		// 退出自动灯丝
		////////////////////////////////////////////////////////////
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		theApp.m_ImageCard.FramGrayHistogramStop();
		m_nAutoFuction = AutoNULL;
		////////////////////////////////////////////////////////////

		if( theApp.m_dwHVtype < 1 )
			theApp.m_HVPower.SetParam( 35, FALSE );	// 设置寻找饱和点标志位

		if ( nRet == AutoFila_Analyze_Completed )
		{
			AutoFilamentOK( m_lFilaSaved );
			theApp.DisplayMsg( "AL-Complete" );
		}
		else
		{
			AutoFilamentOK( lFilaSaved );
			theApp.DisplayMsg( "AL-Finish" );
		}
	}
	////////////////////////////////////////////////////////////////////
}

//进行自动亮度/对比度调节
void CScanView::OnAutoBrightContrast() 
{
	FuncAutoBrightContrast();
}

void CScanView::FuncAutoBrightContrast()
{
	CDlgProgress	dlg;

	dlg.m_csTitle = GetResString( IDS_SD_TITLE_AUTO_BrtCont );
	dlg.m_iRangeTo		= 100;//50;		// 200 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_BC );

	// 11.01.28 改善自动亮度对比度功能
//	StartAutoBC();
	StartAutoBC2();
	// 11.01.28 改善自动亮度对比度功能

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	// 11.01.28 改善自动亮度对比度功能
//	StopAutoBC();
	StopAutoBC2();
	// 11.01.28 改善自动亮度对比度功能
}

void AutoBC_Setup(long lBrightness, long lContrast)
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	ClassID.lVal = SemControlPanelClass_Brightness;
	Flag.lVal = 1;
	Xpos.lVal = lBrightness;

	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );

	ClassID.lVal = SemControlPanelClass_Contrast;
	Flag.lVal = 1;
	Xpos.lVal = lContrast;
	
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}

void CScanView::StartAutoBC()
{
	////////////////////////////////////////////////////////////////////
	// 保存现场
	////////////////////////////////////////////////////////////////////
	m_nusbScanTypeSaved	= m_nusbScanType;
	m_nusbPixRateSaved	= m_nusbPixRate;

	m_lBrightnessSaved	= theApp.m_SemCtrl.m_SemCore.m_Brightness.GetPos().lVal;
	m_lContrastSaved	= theApp.m_SemCtrl.m_SemCore.m_Contrast.GetPos().lVal;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定亮度、对比度初始尝试值
	long	lBrightness;
	long	lContrast;
	AutoBC_Initialize( lBrightness, lContrast );

	AutoBC_Setup(lBrightness, lContrast);
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定当前扫描
	m_nusbPixRate	= 16;	// USB_SS_5us
	USB_ChangeScan( USB_ST_AutoBCArea );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 进入自动亮度、对比度过程
	////////////////////////////////////////////////////////////////////
	m_nLoop = 0;
	m_nAutoFuction = AutoBC;
	theApp.m_ImageCard.FramGrayHistogramReset();
	theApp.m_ImageCard.FramGrayHistogramStart();
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, 100/*nAutoBCTimerElapse*/, NULL );	
	////////////////////////////////////////////////////////////////////
}

void CScanView::StopAutoBC()
{
	////////////////////////////////////////////////////////////////////
	// 恢复亮度、对比度
	////////////////////////////////////////////////////////////////////
	AutoBC_Setup(m_lBrightnessSaved, m_lContrastSaved);
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 停止图象分析
	////////////////////////////////////////////////////////////////////
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	theApp.m_ImageCard.FramGrayHistogramStop();
	m_nAutoFuction = AutoNULL;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 恢复扫描
	m_nusbPixRate = m_nusbPixRateSaved;
	USB_ChangeScan( m_nusbScanTypeSaved );
	////////////////////////////////////////////////////////////////////
}

void CScanView::AutoBCOK( long lBrightness, long lContrast )
{
	m_lBrightnessSaved = lBrightness;
	m_lContrastSaved = lContrast;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoBCProcess()
{
	if ( !theApp.m_ImageCard.IsReadyFramGrayHistogram() )
		return;

	DWORD	dwTotalPixel;
	dwTotalPixel = theApp.m_ImageCard.CopyFramGrayHistogramBuff( m_dwFramGrayHistogramBuff );
	theApp.m_ImageCard.FramGrayHistogramReset();
	theApp.m_ImageCard.FramGrayHistogramStart();

	////////////////////////////////////////////////////////////////////
	// 分析灰度直方图，尝试新的亮度、对比度
	long	lContrast;
	long	lBrightness;
	UINT nRet = AutoBC_Analyze_GrayHistogram( dwTotalPixel,
		m_dwFramGrayHistogramBuff, lBrightness, lContrast );
	////////////////////////////////////////////////////////////////////

#ifdef DEBUG_Info_ABC
	////////////////////////////////////////////////////////////////////
	// Test 调试信息
	////////////////////////////////////////////////////////////////////
	CString		csMsg;

	csMsg.Format("%d B0=%3d B=%3d C=%3d P=%6d L=%5.1f PeakMin=%5.1f",
		m_nLoop++, m_lBrightnessSaved, lBrightness, lContrast,
		dwTotalPixel, dwTotalPixel/256.0, dwTotalPixel * dblPeakMinParam);
//	((CMainFrame *)theApp.GetMainWnd())->
//		m_MainConsole.m_ctrlMsg.SetWindowText( csMsg );
		theApp.DisplayMsg( csMsg );
	////////////////////////////////////////////////////////////////////
#endif //DEBUG_Info_ABC
	
	////////////////////////////////////////////////////////////////////
	if ( nRet == AutoBC_Analyze_Continue )
	{
		AutoBC_Setup(lBrightness, lContrast);
	}
	else
	{
		////////////////////////////////////////////////////////////
		// 退出自动亮度、对比度
		////////////////////////////////////////////////////////////
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		theApp.m_ImageCard.FramGrayHistogramStop();
		m_nAutoFuction = AutoNULL;
		////////////////////////////////////////////////////////////

		if ( nRet == AutoBC_Analyze_Completed )
			AutoBCOK( lBrightness, lContrast );
		else
			AutoBCOK( m_lBrightnessSaved, m_lContrastSaved );
	}
	////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理

void CScanView::StartAutoBC2()
{
	////////////////////////////////////////////////////////////////////
	// 保存现场
	////////////////////////////////////////////////////////////////////
	m_nusbScanTypeSaved	= m_nusbScanType;
	m_nusbPixRateSaved	= m_nusbPixRate;

	m_lBrightnessSaved		= theApp.m_SemCtrl.m_SemCore.m_Brightness.GetPos().lVal;
	m_lContrastSaved		= theApp.m_SemCtrl.m_SemCore.m_Contrast.GetPos().lVal;
	long lCondenserLens		= theApp.m_SemCtrl.m_SemCore.m_CondenserLens.GetPos().lVal;
	m_lObjectiveLensSaved	= theApp.m_SemCtrl.m_SemCore.m_ObjectiveLens.GetPos().lVal;
	////////////////////////////////////////////////////////////////////

	CString		csMsg;
	csMsg.Format("AutoBC2 Start! ScanType=%3d ScanSpeed=%3d B=%6.2d, C=%6.2d, CX=[0x%04X] %5.2f, F=[0x%04X] %5.2f ",
		m_nusbScanTypeSaved,m_nusbPixRateSaved,m_lBrightnessSaved, m_lContrastSaved, lCondenserLens,lCondenserLens/OL_TO_LED*2, m_lObjectiveLensSaved,m_lObjectiveLensSaved/OL_TO_LED*2);
	theApp.DisplayMsg( csMsg );

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	if( ( lCondenserLens / OL_TO_LED *2 > 750 )
		|| ( lCondenserLens / OL_TO_LED *2 < 350 ) )
	{
		////////////////////////////////////////////////////////////////////
		// 设置聚光镜聚焦
		////////////////////////////////////////////////////////////////////
		ClassID.lVal	= SemControlPanelClass_CondenserLens;
		Flag.lVal		= 1;
		Xpos.lVal		= (long)(500*OL_TO_LED /2);

		theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		////////////////////////////////////////////////////////////////////
// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		if( pfrm->m_bIsDebugCreated )
		{
			CString csMsg;
			csMsg.Format("Current CondenserLens(%5.2f) out of [400,800], turn CondenserLens to 550.00 ",
				lCondenserLens/OL_TO_LED*2);
			theApp.DisplayMsg( csMsg );
		}
	}
	if( (m_lObjectiveLensSaved / OL_TO_LED *2 > 716 )
		|| ( m_lObjectiveLensSaved / OL_TO_LED *2 < 394 ) )//根据统调表
	{
		////////////////////////////////////////////////////////////////////
		// 设置物镜聚焦
		////////////////////////////////////////////////////////////////////
		ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
		Flag.lVal		= 1;
		Xpos.dblVal		= 500*OL_TO_LED/2;
		Xpos.lVal		= (long)(Xpos.dblVal);

		theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		////////////////////////////////////////////////////////////////////
// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		if( pfrm->m_bIsDebugCreated )
		{
			CString csMsg;
			csMsg.Format("Current ObjectiveLens(%5.2f) out of [394,716], turn ObjectiveLens to 550.00 ",
				m_lObjectiveLensSaved/OL_TO_LED*2);
			theApp.DisplayMsg( csMsg );
		}
	}

	////////////////////////////////////////////////////////////////////
	// 设定亮度、对比度初始尝试值
	// B: -12.41(0xB7F, GetPos().lVal = -127)
	// C: 62.745(0x5F00, GetPos().lVal = 160)
	long	lBrightness	= -360;
	long	lContrast	= 120;
	// 读取参数值
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char		stabuff[20];
	char		staitembuff[20];
	int			nTotal	= 8;
	int*		pCheck	= new int[8];
	CString		str;
	sprintf(stabuff, "OneKEY");
	sprintf(staitembuff, "param");
	theApp.ReadInOneLine(path, stabuff, staitembuff, INTSTYLE, nTotal, pCheck);
	lBrightness	= pCheck[3] -10;
	lContrast	= pCheck[4] -20;
	delete [] pCheck;
	AutoBC_Initialize3( lBrightness, lContrast );

	AutoBC_Setup(lBrightness, lContrast);
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定当前扫描
	m_nusbPixRate	= 8;	// USB_SS_4us
	USB_ChangeScan( USB_ST_AutoBCArea );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 进入自动亮度、对比度BEST过程
	////////////////////////////////////////////////////////////////////
	m_nLoop				= 0;
	m_nAutoFuction		= AutoBC;
	theApp.m_ImageCard.FramGrayHistogramReset();
	theApp.m_ImageCard.FramGrayHistogramStart();
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoBCTimerElapse, NULL );	
	////////////////////////////////////////////////////////////////////
}

void CScanView::AutoBCProcess2()
{
	if ( !theApp.m_ImageCard.IsReadyFramGrayHistogram() )
		return;

	////////////////////////////////////////////////////////////////////
	DWORD	dwTotalPixel;
	dwTotalPixel = theApp.m_ImageCard.CopyFramGrayHistogramBuff( m_dwFramGrayHistogramBuff );
	theApp.m_ImageCard.FramGrayHistogramReset();
	theApp.m_ImageCard.FramGrayHistogramStart();

	////////////////////////////////////////////////////////////////////
	// 分析灰度直方图，尝试新的亮度、对比度
	long	lContrast;
	long	lBrightness;
	UINT nRet = AutoBC_Analyze_Image2( dwTotalPixel,
		m_dwFramGrayHistogramBuff, lBrightness, lContrast );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	if ( nRet == AutoBC_Analyze_Continue )
	{
		AutoBC_Setup(lBrightness, lContrast);

// 		CScanChildFrame* pfrm = (CScanChildFrame*)GetParent();
// 		if( pfrm->m_bIsDebugCreated )
		{
			CString		csMsg;
			csMsg.Format("ABC-%02d, B0=%03d, B=%03d, C=%03d, P=%06d, L=%05.1f, PeakMin=%05.1f",
				m_nLoop++, m_lBrightnessSaved, lBrightness, lContrast,
				dwTotalPixel, dwTotalPixel/256.0, dwTotalPixel * dblPeakMinParam);
			theApp.DisplayMsg( csMsg );
		}
	}
	else
	{
		////////////////////////////////////////////////////////////
		// 退出自动亮度、对比度
		////////////////////////////////////////////////////////////
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		theApp.m_ImageCard.FramGrayHistogramStop();
		m_nAutoFuction = AutoNULL;
		////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////
		// 恢复扫描
		m_nusbPixRate = m_nusbPixRateSaved;
		USB_ChangeScan( m_nusbScanTypeSaved );
		////////////////////////////////////////////////////////////////////

		CString		csMsg;
		if ( nRet == AutoBC_Analyze_Completed )
		{
			AutoBCOK( lBrightness, lContrast );
			csMsg.Format("ABC-Complete! Current B=%5d, C=%5d",m_lBrightnessSaved, m_lContrastSaved);
		}
		else
		{
			AutoBCOK( m_lBrightnessSaved, m_lContrastSaved );
			csMsg.Format("ABC-Finish! Saved B=%5d, C=%5d",m_lBrightnessSaved, m_lContrastSaved);
		}
		theApp.DisplayMsg( csMsg );
	}
	////////////////////////////////////////////////////////////////////
}

void CScanView::StopAutoBC2()
{
	////////////////////////////////////////////////////////////////////
	// 恢复亮度、对比度
	////////////////////////////////////////////////////////////////////
	AutoBC_Setup(m_lBrightnessSaved, m_lContrastSaved);
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 停止图象分析
	////////////////////////////////////////////////////////////////////
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	theApp.m_ImageCard.FramGrayHistogramStop();
	m_nAutoFuction = AutoNULL;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 恢复扫描
	m_nusbPixRate = m_nusbPixRateSaved;
	USB_ChangeScan( m_nusbScanTypeSaved );
	////////////////////////////////////////////////////////////////////
}
// 11.01.28 改善自动亮度对比度功能，ZYX修改，MJY整理
///////////////////////////////////////////////////////////////////////////////


//进行自动聚焦调节
void CScanView::OnAutoFocus() 
{
	FuncAutoFocus();
}

void CScanView::FuncAutoFocus()
{
	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Focus );
	dlg.m_iRangeTo	= nAFTotalTime;		// 100 s
	dlg.m_csMessage = GetResString( IDS_STRING_AUTO_FOCUS );

	StartAutoFocus();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoFocus();
}

void CScanView::AutoFocus_Setup(long lObjectiveLens, double dblMagnifier )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	////////////////////////////////////////////////////////////////////
	m_lLastlObjectiveLens	= lObjectiveLens;

	m_dbLastlMagnifierSaved =dblMagnifier;

	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设置物镜聚焦
	////////////////////////////////////////////////////////////////////
	ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
	Flag.lVal		= 1;
	Xpos.dblVal		= lObjectiveLens;
	Xpos.lVal		= lObjectiveLens;

	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设置放大倍数
	////////////////////////////////////////////////////////////////////
	ClassID.lVal	= SemControlPanelClass_Magnifier;
	Flag.lVal		= 1;
	Xpos.dblVal		= dblMagnifier;

	theApp.SemCoreCtrlPanelMessageProc(	ClassID, SerialNumber, Xpos, Ypos, Flag );
	////////////////////////////////////////////////////////////////////
}

void CScanView::StartAutoFocus()
{
	////////////////////////////////////////////////////////////////////
	// 保存现场
	////////////////////////////////////////////////////////////////////
	m_nusbScanTypeSaved	= m_nusbScanType;
	m_nusbPixRateSaved	= m_nusbPixRate;

	m_lObjectiveLensSaved	= theApp.m_SemCtrl.m_SemCore.m_ObjectiveLens.GetPos().lVal;
	m_dblMagnifierSaved		= theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	if( theApp.m_dwOpr > 0 )
	{
		// Test 调试信息
		////////////////////////////////////////////////////////////////////
		CString		csMsg;
		csMsg.Format("AF-Start, F =[0x%04X] %5.1f, M = %5.1f",
			m_lObjectiveLensSaved, m_lObjectiveLensSaved/OL_TO_LED*2, m_dblMagnifierSaved );
		theApp.DisplayMsg( csMsg );
		////////////////////////////////////////////////////////////////////
	}
	else
		theApp.DisplayMsg( "AF-Start" );

	////////////////////////////////////////////////////////////////////
	// 设定初始尝试值
	long 		lObjectiveLens	= m_lObjectiveLensSaved;
	double		dblMagnifier	= m_dblMagnifierSaved;
	AutoFocus_Initialize2( lObjectiveLens, dblMagnifier );

	AutoFocus_Setup( lObjectiveLens, dblMagnifier );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定当前扫描
	m_nusbPixRate = 16;	// USB_SS_8us
	USB_ChangeScan( USB_ST_AutoFocusArea );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 进入自动聚焦过程
	////////////////////////////////////////////////////////////////////
	m_nLoop = 0;
	m_nAutoFuction = AutoFocus;
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoFocusTimerElapse, NULL );
	////////////////////////////////////////////////////////////////////
}

void CScanView::StopAutoFocus()
{
	////////////////////////////////////////////////////////////////////
	// 恢复物镜、放大器
	////////////////////////////////////////////////////////////////////
	AutoFocus_Setup( m_lObjectiveLensSaved, m_dblMagnifierSaved );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 停止图象分析
	////////////////////////////////////////////////////////////////////
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	m_nAutoFuction = AutoNULL;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 恢复扫描
	m_nusbPixRate = m_nusbPixRateSaved;
	USB_ChangeScan( m_nusbScanTypeSaved );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 结束自动功能
	////////////////////////////////////////////////////////////////////
}

void CScanView::AutoFocusOK( long lObjectiveLens )
{
	m_lObjectiveLensSaved = lObjectiveLens;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoFocusProcess()
{
	DWORD	ReadTickCount, AnalyzeTickCount, SetupTickCount;
	DWORD	thisTickCount, lastTickCount;

	lastTickCount = timeGetTime();
	////////////////////////////////////////////////////////////////////
	// 复制图象
	DWORD	dwTotalPixel;
	CRect	rcDest = m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	dwTotalPixel = theApp.m_ImageCard.Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );
	////////////////////////////////////////////////////////////////////
	thisTickCount = timeGetTime();
	ReadTickCount = thisTickCount - lastTickCount;

	lastTickCount = timeGetTime();
	////////////////////////////////////////////////////////////////////
	// 分析图象，尝试新的聚焦值、放大倍数
	long 		lObjectiveLens_Old	= m_lLastlObjectiveLens;
	long 		lObjectiveLens		= m_lObjectiveLensSaved;
	double		dblMagnifier		= m_dblMagnifierSaved;
	//zyx add 090407
	double		dblMagnifier_Old	= m_dbLastlMagnifierSaved;
	double		dfSharpness = -1;
	UINT nRet = AutoFocus_Analyze_Image2(
		(LPVOID)m_bImageBuff, rcDest.Size(),
		dfSharpness, lObjectiveLens, dblMagnifier );
	////////////////////////////////////////////////////////////////////
	thisTickCount = timeGetTime();
	AnalyzeTickCount = thisTickCount - lastTickCount;

	////////////////////////////////////////////////////////////////////
	// Setup lObjectiveLens, dfMagnifier
	if ( nRet == AutoFocus_Analyze_Continue )
	{
		lastTickCount = timeGetTime();
		/////////////////////////////////////////

		AutoFocus_Setup( lObjectiveLens, dblMagnifier );

		/////////////////////////////////////////
		thisTickCount = timeGetTime();
		SetupTickCount = thisTickCount - lastTickCount;

		if( theApp.m_dwOpr > 0 )
		{
			////////////////////////////////////////////////////////////////////
			// Test 调试信息
			////////////////////////////////////////////////////////////////////
			CString		csMsg;
			csMsg.Format("AF-%2d,time[ R=%3d,A=%3d,S=%3d ],[%5.2f, %.6f, %5.1f]",
				 m_nLoop++, ReadTickCount, AnalyzeTickCount, SetupTickCount,
				lObjectiveLens_Old/OL_TO_LED*2, dfSharpness*1000, dblMagnifier_Old );
			theApp.DisplayMsg( csMsg );
			////////////////////////////////////////////////////////////////////
		}
	}
	else
	{
		////////////////////////////////////////////////////////////
		// 退出自动聚焦
		////////////////////////////////////////////////////////////
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		m_nAutoFuction = AutoNULL;
		////////////////////////////////////////////////////////////

		if ( nRet == AutoFocus_Analyze_Completed )
		{
			if( theApp.m_dwOpr > 0 )
			{
				////////////////////////////////////////////////////////////////////
				// Test 调试信息
				////////////////////////////////////////////////////////////////////
				CString		csMsg;
				csMsg.Format("AF-Complete, F = %5.2f,[%5.2f,%.6f, %5.1f]", lObjectiveLens/OL_TO_LED*2,lObjectiveLens_Old/OL_TO_LED*2,dfSharpness*1000, dblMagnifier_Old );
				theApp.DisplayMsg( csMsg );
				TRACE( csMsg );
				////////////////////////////////////////////////////////////////////
			}
			else
				theApp.DisplayMsg( "AF-Complete" );
			AutoFocusOK( lObjectiveLens );
		}
		else
		{
			if( theApp.m_dwOpr > 0 )
			{
				////////////////////////////////////////////////////////////////////
				// Test 调试信息
				////////////////////////////////////////////////////////////////////
				CString		csMsg;
				csMsg.Format("AF-Finish, F = %5.1f", m_lObjectiveLensSaved/OL_TO_LED*2 );
				theApp.DisplayMsg( csMsg );
				TRACE( csMsg );
				////////////////////////////////////////////////////////////////////
			}
			else
				theApp.DisplayMsg( "AF-Finish" );
			AutoFocusOK( m_lObjectiveLensSaved );
		}
	}
	////////////////////////////////////////////////////////////////////
}

void CScanView::AFTest()
{
	////////////////////////////////////////////////////////////////////
	// 复制图象
	DWORD	dwTotalPixel;
	CRect	rcDest = m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	dwTotalPixel = theApp.m_ImageCard.Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 分析图象
	double		dfSharpness = -1;
	double		dfParamX	= -1;
	double		dfParamY	= -1;
	double		dfTotal		= -1;
	Analyze_Image( (LPVOID)m_bImageBuff, rcDest.Size(),
		 dfSharpness, dfParamX, dfParamY, dfTotal );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// Test 调试信息
	////////////////////////////////////////////////////////////////////
	CString		csMsg;
	csMsg.Format("AF, %5.1f, %5.1f, %5.1f, %5.1f, %5.1f",
		dfSharpness / dfTotal * 1000,
		dfSharpness, dfParamX, dfParamY, dfTotal );

//	((CMainFrame *)theApp.GetMainWnd())->
//		m_MainConsole.m_ctrlMsg.SetWindowText( csMsg );
	
	theApp.DisplayMsg( csMsg );
	////////////////////////////////////////////////////////////////////
}


//进行自动消像散调节
void CScanView::OnAutoStigmator() 
{
	FuncAutoStigmator();
}

void CScanView::FuncAutoStigmator()
{
	CDlgProgress	dlg;

	dlg.m_csTitle	= GetResString( IDS_SD_TITLE_AUTO_Stigmator );
	dlg.m_iRangeTo	= nASTotalTime;		// 200 s
	dlg.m_csMessage	= GetResString( IDS_STRING_AUTO_STIGMATOR );

	StartAutoAstigmat();

	m_pAutoProgressDlg = &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg = NULL;

	StopAutoAstigmat();
}


void AutoAstigmat_Setup(long lObjectiveLens, double dblMagnifier, long lStigmatorX, long lStigmatorY )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	////////////////////////////////////////////////////////////////////
	// 设置物镜聚焦
	////////////////////////////////////////////////////////////////////
	ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
	Flag.lVal		= 1;
	Xpos.dblVal		= lObjectiveLens;
	Xpos.lVal		= lObjectiveLens;

	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设置消像散器
	////////////////////////////////////////////////////////////////////
	ClassID.lVal	= SemControlPanelClass_Stigmator;
	Flag.lVal		= 3;
	Xpos.lVal		= lStigmatorX;
	Ypos.lVal		= lStigmatorY;

	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	////////////////////////////////////////////////////////////////////
}

void CScanView::StartAutoAstigmat()
{
	////////////////////////////////////////////////////////////////////
	// 保存现场
	////////////////////////////////////////////////////////////////////
	m_nusbScanTypeSaved	= m_nusbScanType;
	m_nusbPixRateSaved	= m_nusbPixRate;

	m_lStigmatorXSaved		= theApp.m_SemCtrl.m_SemCore.m_StigmatorX.GetPos().lVal;
	m_lStigmatorYSaved		= theApp.m_SemCtrl.m_SemCore.m_StigmatorY.GetPos().lVal;
	m_lObjectiveLensSaved	= theApp.m_SemCtrl.m_SemCore.m_ObjectiveLens.GetPos().lVal;
	m_dblMagnifierSaved		= theApp.m_SemCtrl.m_SemCore.m_Magnifier.GetPos().dblVal;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定初始尝试值
	long	lObjectiveLens	= m_lObjectiveLensSaved;
	double	dblMagnifier	= m_dblMagnifierSaved;
	long	lStigmatorX		= m_lStigmatorXSaved;
	long	lStigmatorY		= m_lStigmatorYSaved;

	AutoAstigmat_Initialize( lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );

	AutoAstigmat_Setup( lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 设定当前扫描
	m_nusbPixRate = 8;	// USB_SS_4us
	USB_ChangeScan( USB_ST_AutoAstigmatArea );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 进入自动消像散过程
	////////////////////////////////////////////////////////////////////
	m_nLoop = 0;
	m_nAutoFuction = AutoAstigmat;
	m_nAutoFunctionTimer = SetTimer( nAutoFunctionTimerID, nAutoAstigmatTimerElapse, NULL );
	////////////////////////////////////////////////////////////////////
}

void CScanView::StopAutoAstigmat()
{
	////////////////////////////////////////////////////////////////////
	// 恢复物镜、消像散器
	////////////////////////////////////////////////////////////////////
	AutoAstigmat_Setup( m_lObjectiveLensSaved, m_dblMagnifierSaved, m_lStigmatorXSaved, m_lStigmatorYSaved );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 停止图象分析
	////////////////////////////////////////////////////////////////////
	if ( m_nAutoFunctionTimer != 0 )
	{
		KillTimer( m_nAutoFunctionTimer );
		m_nAutoFunctionTimer = 0;
	}
	m_nAutoFuction = AutoNULL;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 恢复扫描
	m_nusbPixRate = m_nusbPixRateSaved;
	USB_ChangeScan( m_nusbScanTypeSaved );
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 结束自动功能
	////////////////////////////////////////////////////////////////////
}

void CScanView::AutoAstigmatOK( long lObjectiveLens, long lStigmatorX, long lStigmatorY )
{
	m_lObjectiveLensSaved	= lObjectiveLens;
	m_lStigmatorXSaved	= lStigmatorX;
	m_lStigmatorYSaved	= lStigmatorY;

	if ( m_pAutoProgressDlg != NULL )
		m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
}

void CScanView::AutoAstigmatProcess()
{
	DWORD	ReadTickCount, AnalyzeTickCount;
	DWORD	thisTickCount, lastTickCount;


	lastTickCount = timeGetTime();
	////////////////////////////////////////////////////////////////////
	// 复制图象
	////////////////////////////////////////////////////////////////////
	DWORD	dwTotalPixel;
	CRect	rcDest = m_rcAutoAstigmatAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	dwTotalPixel = theApp.m_ImageCard.Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );
	////////////////////////////////////////////////////////////////////
	thisTickCount = timeGetTime();
	ReadTickCount = thisTickCount - lastTickCount;


	lastTickCount = timeGetTime();
	////////////////////////////////////////////////////////////////////
	// 分析图象，尝试新控制值
	////////////////////////////////////////////////////////////////////
	long	lObjectiveLens	= m_lObjectiveLensSaved;
	double	dblMagnifier	= m_dblMagnifierSaved;
	long	lStigmatorX		= m_lStigmatorXSaved;
	long	lStigmatorY		= m_lStigmatorYSaved;
	double	dfSharpness		= -1;

	UINT nRet = AutoAstigmat_Analyze_Image(	
		(LPVOID)m_bImageBuff, rcDest.Size(), dfSharpness,
		lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );
	////////////////////////////////////////////////////////////////////
	thisTickCount = timeGetTime();
	AnalyzeTickCount = thisTickCount - lastTickCount;

	if( theApp.m_dwOpr > 0 )
	{
		////////////////////////////////////////////////////////////////////
		// Test 调试信息
		////////////////////////////////////////////////////////////////////
		CString		csMsg;
		csMsg.Format("AS %2d, %5.1f, time[ R=%3d,A=%3d ]",
			 m_nLoop++, dfSharpness*1000, ReadTickCount, AnalyzeTickCount );
		theApp.DisplayMsg( csMsg );
		////////////////////////////////////////////////////////////////////
	}

	////////////////////////////////////////////////////////////////////
	// AutoAstigmat_Setup( lObjectiveLens, nMagCtrl, lStigmatorX, lStigmatorY )
	////////////////////////////////////////////////////////////////////
	if ( nRet == AutoAstigmat_Analyze_Continue )
	{
		AutoAstigmat_Setup( lObjectiveLens, dblMagnifier, lStigmatorX, lStigmatorY );
	}
	else
	{
		////////////////////////////////////////////////////////////
		// 退出自动消像散
		////////////////////////////////////////////////////////////
		if ( m_nAutoFunctionTimer != 0 )
		{
			KillTimer( m_nAutoFunctionTimer );
			m_nAutoFunctionTimer = 0;
		}
		m_nAutoFuction = AutoNULL;
		////////////////////////////////////////////////////////////

		if ( nRet == AutoAstigmat_Analyze_Completed )
		{
			AutoAstigmatOK( lObjectiveLens, lStigmatorX, lStigmatorY );

			////////////////////////////////////////////////////////////////////
			// Test 调试信息
			////////////////////////////////////////////////////////////////////
			CString		csMsg;
			csMsg = _T("AS Complete");
			theApp.DisplayMsg( csMsg );
			////////////////////////////////////////////////////////////////////
		}
		else
		{
			AutoAstigmatOK( m_lObjectiveLensSaved, m_lStigmatorXSaved, m_lStigmatorYSaved );

			////////////////////////////////////////////////////////////////////
			// Test 调试信息
			////////////////////////////////////////////////////////////////////
			CString		csMsg;
			csMsg = _T("AS Finish");
			theApp.DisplayMsg( csMsg );
			////////////////////////////////////////////////////////////////////
		}
	}
	////////////////////////////////////////////////////////////////////
}











void CScanView::GetFitScreenClientRect( CRect& rcScreenClientRect )
{
	CRect rc;
	GetClientRect( (LPRECT)rc );
//	if( m_bSplit )
//		rc.right = rc.left +rc.Width() /2;
	rcScreenClientRect = rc;
}

void CScanView::UpdateScale()
{
	CRect rcScreenClientRect;
	GetFitScreenClientRect( rcScreenClientRect );

//	double dScale = (double)rcScreenClientRect.Width() / g_szFrameGraph.cx;
	double dScale = (double)rcScreenClientRect.Width() / m_sizeReso.cx;
	if( m_bSplit )
		dScale /= 2;
	if( dScale == 0.0 )
		return;

	double	dScaleScanTable[]		= { 0.1, 0.2, 0.25, 0.50, 0.75, 1.0, 1.5, 2, 2.5, 3, 3.5, 4 };
	int		nScaleScanIndexMax		= sizeof(g_dScaleScanTable) / sizeof(double) - 1;
	int		nScaleScanIndexMin		= 0;
	g_nScaleScanIndexFit			= 0;
	if( dScale < 1.0 )
	{
		int i = 0;
		for( i=nScaleScanIndexMin; i<=nScaleScanIndexMax; i++ )
		{
			if( (dScale < dScaleScanTable[i]) && (g_nScaleScanIndexFit == 0) )
			{
				g_dScaleScanTable[i+1] = dScaleScanTable[i];
				g_dScaleScanTable[i] = dScale;
				g_nScaleScanIndexFit = i;
			}
			else if( (dScaleScanTable[i] <= 1.0) && (g_nScaleScanIndexFit == 0) )
				g_dScaleScanTable[i] = dScaleScanTable[i];
			else if( (dScaleScanTable[i] <= 1.0) && (g_nScaleScanIndexFit != 0) )
				g_dScaleScanTable[i+1] = dScaleScanTable[i];
			else
				break;
		}
		g_nScaleScanIndexMax = i;
	}
	else
	{
		int i = 0;
		for( i=nScaleScanIndexMin; i<=nScaleScanIndexMax; i++ )
		{
			if( dScale < dScaleScanTable[i] )
			{
				g_dScaleScanTable[i] = dScale;
				g_nScaleScanIndexMax = i;
				g_nScaleScanIndexFit = i;
				break;
			}
		}
		if( i == nScaleScanIndexMax +1 )
		{
			g_nScaleScanIndexMax = i-1;
			g_nScaleScanIndexFit = i-1;
		}
	}

	if( m_bZoomFit )
		g_nScaleScanIndex = g_nScaleScanIndexFit;
//	else
//		g_nScaleScanIndex = g_nScaleScanIndexMax;
}

void CScanView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	// 扫描区域设为正方形
/*	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm != NULL && pfrm->m_pwndSplitter->m_hWnd != NULL )
	{
		CRect rect;
		GetClientRect( &rect );
		pfrm->m_pwndSplitter->SetColumnInfo(0, rect.Height(), 100);
		pfrm->m_pwndSplitter->RecalcLayout();
	}
*/
	UpdateItemRect( FALSE );
	UpdateScale();
	
	// TODO: Add your message handler code here
/*	if (m_nMapMode == MM_SCALETOFIT)
	{
		// force recalculation of scale to fit parameters
		SetScaleToFitSize(m_szTotalLog);
	}
	else
	{
		// UpdateBars() handles locking out recursion
		UpdateBars();
	}*/
}

/////////////////////////////////////////////////////////////////////////////
// Set mode and scaling/scrolling sizes

void CScanView::SetScaleToFitSize(SIZE sizeTotal)
{
	// Note: It is possible to set sizeTotal members to negative values to
	//  effectively invert either the X or Y axis.

	ASSERT(m_hWnd != NULL);
	m_nMapMode = MM_SCALETOFIT;     // special internal value
	m_szTotalLog = sizeTotal;

	// reset and turn any scroll bars off
	if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL)))
	{
		SetScrollPos(SB_HORZ, 0);
		SetScrollPos(SB_VERT, 0);
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
	}

	CRect rectT;
	GetClientRect(rectT);
	m_szTotalDev = rectT.Size();

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate
		UpdateBars();
		Invalidate(TRUE);
	}
}

void CScanView::UpdateBars()
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_szTotalDev.cx >= 0 && m_szTotalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	CSize sizeClient;
	CSize sizeSb;

	GetClientRect(&rectClient);

	if (bCalcClient)
	{
		// get client rect
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// no room for scroll bars (common for zero sized elements)
			CRect rect;
			GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// if entire client area is not invisible, assume we have
				//  control over our scrollbars
				EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// let parent window determine the "client" rect
		GetScrollBarSizes(sizeSb);
		sizeClient.cx = rectClient.right - rectClient.left;
		sizeClient.cy = rectClient.bottom - rectClient.top;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if (needSb.cx)
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_szTotalDev.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_szTotalDev.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}

void CScanView::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as CScanView is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, ptDev.y);
	ScrollWindow(xOrig - ptDev.x, yOrig - ptDev.y);
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void CScanView::GetScrollBarState(CSize sizeClient, CSize& needSb,
	CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_szTotalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behaviour

void CScanView::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = GetSystemMetrics(SM_CXVSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cx -= CX_BORDER;
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = GetSystemMetrics(SM_CYHSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cy -= CY_BORDER;
	}
}

BOOL CScanView::GetTrueClientSize(CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

CPoint CScanView::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);

	if (m_bCenter)
	{
		CRect rect;
		GetClientRect(&rect);

		// if client area is larger than total device size,
		// the scroll positions are overridden to place origin such that
		// output is centered in the window
		// GetDeviceScrollPosition() must reflect this

		if (m_szTotalDev.cx < rect.Width())
			pt.x = -((rect.Width() - m_szTotalDev.cx) / 2);
		if (m_szTotalDev.cy < rect.Height())
			pt.y = -((rect.Height() - m_szTotalDev.cy) / 2);
	}

	return pt;
}


void CScanView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm )
	{
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.ShowWindow( SW_SHOW );
		if( pfrm->m_wndScanCtrlBar.m_hWnd != NULL )
			pfrm->ShowControlBar(&(pfrm->m_wndScanCtrlBar), TRUE, FALSE);
		if( pfrm->m_HistoBar.m_hWnd != NULL )
			pfrm->ShowControlBar(&(pfrm->m_HistoBar), FALSE, FALSE);
		pfrm->RecalcLayout();
	}
}






///////////////////////////////////////////////////////////////////////////////
//
// 自动台相关操作
//
///////////////////////////////////////////////////////////////////////////////

void CScanView::OnStageCoordinateorigin() 
{
	// TODO: Add your command handler code here
	m_bStageOrigin = !m_bStageOrigin;
	if( m_bStageOrigin )
	{
		m_ptPosSave = CPoint(0,0);
		SetStagePointParam( CPoint(0,0) );
	}
	else
		theApp.m_ImageCard.GraphClear( m_rcStageDragRectLast );
}

void CScanView::OnUpdateStageCoordinateorigin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
	// 所以不光是自动台移动参考点，还复用为校准时的屏幕中心参考点
//	pCmdUI->Enable( theApp.m_Stage.nHasAutoStage > 0 && theApp.m_Stage.bAutoStageReady );
	pCmdUI->SetCheck( m_bStageOrigin );
}

void CScanView::SetStagePointParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	/////////////////////////////////////////////

	CPoint ptPoint	= m_ptStagePointParam;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos1.y - ptPos0.y;

	nOffsetX = nOffsetX / 4 * 4;
	nOffsetY = nOffsetY / 4 * 4;

	ptPoint.x += nOffsetX;
	ptPoint.y += nOffsetY;

	if( ptPoint.x < 20 )
		ptPoint.x = 20;
	if( ptPoint.x > m_sizeReso.cx -20 )
		ptPoint.x = m_sizeReso.cx -20;
	if( ptPoint.y < 20 )
		ptPoint.y = 20;
	if( ptPoint.y > m_sizeReso.cy -20 )
		ptPoint.y = m_sizeReso.cy -20;
	CRect rect(ptPoint.x-20,ptPoint.y-20,ptPoint.x+20,ptPoint.y+20);
	theApp.m_ImageCard.GraphClear( m_rcStageDragRectLast );
	theApp.m_ImageCard.GraphClear( rect, RGB(0,0,0) );

	if( !m_bStageOrigin )
		return;
/*
	ptPos0.x = m_rcStageDragRectLast.left+20;
	ptPos0.y = m_rcStageDragRectLast.top+20;
	ptPos1 = ptPos0;
	ptPos0.x -= 20;
	ptPos1.x += 20;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1 );

	ptPos0.x = m_rcStageDragRectLast.left+20;
	ptPos0.y = m_rcStageDragRectLast.top+20;
	ptPos1 = ptPos0;
	ptPos0.y -= 20;
	ptPos1.y += 20;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1 );
*/
	if( m_bStageOrigin && m_bDebugCoadjust )
	{
		// 15.11 校准crossover时使屏幕中心显示参考点，且标准框是以参考点为中心的正方形
		// DebugCoadjust且显示参考点的情况下，参考点固定在中心不许移动
		m_ptStagePointParam.x	= m_sizeReso.cx /2;
		m_ptStagePointParam.y	= m_sizeReso.cy /2;
		rect.left	= m_ptStagePointParam.x -20;
		rect.right	= m_ptStagePointParam.x +20;
		rect.top	= m_ptStagePointParam.y -20;
		rect.bottom	= m_ptStagePointParam.y +20;
	}
	/////////////////////////////////////////////////////////////
	ptPos0.x = rect.left+20;
	ptPos0.y = rect.top+20;
	ptPos1 = ptPos0;
	ptPos0.x -= 20;
	ptPos1.x += 20;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );

	ptPos0.y += 1;
	ptPos1.y += 1;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );
	/////////////////////////////////////////////////////////////
	ptPos0.x = rect.left+20;
	ptPos0.y = rect.top+20;
	ptPos1 = ptPos0;

	ptPos0.y -= 20;
	ptPos1.y += 20;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );

	ptPos0.x += 1;
	ptPos1.x += 1;
	theApp.m_ImageCard.GraphDrawLine( ptPos0, ptPos1, RGB(255,255,0) );
	//======2006.09.04_点扫描点的画法=================

	m_rcStageDragRectLast = rect;
}

void CScanView::SetStageTrackParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = ScreenToGraph( point );
	CPoint ptPoint	= m_ptStageTrackParam;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// 画线
	BOOL bSameX, bSameY;
	bSameX = bSameY = FALSE;
	if( ptPos1.x == ptPoint.x )
		bSameX = TRUE;
	if( ptPos1.y == ptPoint.y )
		bSameY = TRUE;
	theApp.m_ImageCard.GraphDrawCircle( ptPoint, ptPos0 );
//	theApp.m_ImageCard.GraphClear( CRect(ptPos0.x-1, ptPos0.y-1, ptPos0.x+1, ptPos0.y+1) );
	theApp.m_ImageCard.GraphDrawCircle( ptPoint, ptPos1, TRUE, nSTAGE_TRACE_POINT_RADIUS, RGB(0, 255, 0) );
	/////////////////////////////////////////////

	if( theApp.m_Stage.nHasAutoStage < 2 )
	{
		double dScaleX, dScaleY;
		/////////////////////////////////////////////
		// 调整X步距及方向
		if( (ptPos0.x == ptPoint.x) || (ptPos1.x == ptPoint.x) )
			dScaleX = 1.0;
		else
			dScaleX = 1.0 * abs(ptPos1.x - ptPoint.x) / abs(ptPos0.x - ptPoint.x);
		m_nStageTrackStepX = (int)(abs(m_nStageTrackStepX) *dScaleX);
		/////////////////////////////////////////////

		/////////////////////////////////////////////
		// 调整Y步距及方向
		if( (ptPos0.y == ptPoint.y) || (ptPos1.y == ptPoint.y) )
			dScaleY = 1.0;
		else
			dScaleY = 1.0 * abs(ptPos1.y - ptPoint.y) / abs(ptPos0.y - ptPoint.y);
		m_nStageTrackStepY = (int)(abs(m_nStageTrackStepY) *dScaleY);
		/////////////////////////////////////////////

		// 保存位置
		m_ptPosSave = ptPos1;

		// 设置跟踪参数
			theApp.Stage_XY_Command2( "JG", bSameX ? 0 : m_nStageTrackStepX, bSameY ? 0 : m_nStageTrackStepY,
									(ptPos1.x<ptPoint.x) ? FALSE : TRUE, (ptPos1.y<ptPoint.y) ? FALSE : TRUE, FALSE );
	}
	else
	{
		m_nStageTrackStepX = abs(ptPos1.x - ptPoint.x) *2;
		m_nStageTrackStepY = abs(ptPos1.y - ptPoint.y) *2;
		// 保存位置
		m_ptPosSave = ptPos1;
		StageMC600_SetTrackParam( (ptPos1.x<ptPoint.x) ? 0 -m_nStageTrackStepX : m_nStageTrackStepX,
									(ptPos1.y<ptPoint.y) ? 0 -m_nStageTrackStepY : m_nStageTrackStepY );
	}
}

void CScanView::ManuallyAdjustStage( UINT nChar )
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm == NULL
		|| pfrm->m_dlgStage.m_hWnd == NULL )
		return;

	int nLength, nScale;	// 像素距离, 实际步数
//	char szBuffer[64];
	CString str;

	switch( nChar )
	{
	case	VK_LEFT:
		nLength = m_sizeReso.cx /4;
		nScale = 0;
		if( theApp.m_Stage.xy.bRev)
			CalculateStageUScale( FALSE, FALSE, nLength, nScale );
		else
			CalculateStageUScale( TRUE, FALSE, nLength, nScale );
// 		theApp.Stage_XY_Command( "PR", nScale, TRUE, FALSE );
		pfrm->m_dlgStage.StageMoveX( -1*nScale, TRUE, FALSE );
		break;
	case	VK_RIGHT:
		nLength = m_sizeReso.cx /4;
		nScale = 0;
		if( theApp.m_Stage.xy.bRev )
			CalculateStageUScale( FALSE, TRUE, nLength, nScale );
		else
			CalculateStageUScale( TRUE, TRUE, nLength, nScale );
// 		theApp.Stage_XY_Command( "PR", nScale, TRUE, TRUE );
		pfrm->m_dlgStage.StageMoveX( nScale, TRUE, TRUE );
		break;
	case	VK_UP:
		nLength = m_sizeReso.cy /4;
		nScale = 0;
		if( theApp.m_Stage.xy.bRev)
			CalculateStageUScale( TRUE, FALSE, nLength, nScale );
		else
			CalculateStageUScale( FALSE, FALSE, nLength, nScale );
//		theApp.Stage_XY_Command( "PR", nScale, FALSE, FALSE );
		pfrm->m_dlgStage.StageMoveY( nScale, TRUE, FALSE );
		break;
	case	VK_DOWN:
		nLength = m_sizeReso.cy /4;
		nScale = 0;
		if( theApp.m_Stage.xy.bRev )
			CalculateStageUScale( TRUE, TRUE, nLength, nScale );
		else
			CalculateStageUScale( FALSE, TRUE, nLength, nScale );
// 		theApp.Stage_XY_Command( "PR", nScale, FALSE, TRUE );
		pfrm->m_dlgStage.StageMoveY( -1*nScale, TRUE, TRUE );
		break;
	}
}

void CScanView::AutoStageXY_FI_Init()
{
	CDlgProgress		dlg;
	dlg.m_csTitle		= GetResString( IDS_Stage_SD_TITLE_AUTO_FindIndex );
	dlg.m_iRangeTo		= 60;
	dlg.m_csMessage		= GetResString( IDS_Stage_STRING_AUTO_FindIndex );

	m_bStageFinishX		= FALSE;
	m_bStageFinishY		= FALSE;

	char szBuffer[64];
	CString str;
	if( theApp.m_Stage.xy.nMtd < 1 )
	{
		m_lastStage = GetTickCount();
		// 调用上电复位程序
		str.Format( "XQ #AUTO" );
	}
	else
	{
		// 找中心原点
		// 第一步是撞到行程开关，先走Y方向
		str.Format( "SHY;JG %s;BGY", theApp.m_Stage.xy.sCmdFI1 );
	}
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	theApp.DisplayMsg( str );
	SetTimer( nAutoStageXY_FI11, nAutoStageXY_FIElapse, NULL );

	m_pAutoProgressDlg	= &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg	= NULL;

	AutoStageXY_FI_Finish( FALSE );
}

void CScanView::AutoStageXY_FI_XQ()
{
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	if(  (abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 10)
		&& (abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 10) )
	{
		DWORD thisTickCount = GetTickCount();
		if( thisTickCount -m_lastStage > 5000 )	// 3s没有动
			AutoStageXY_FI_Finish( TRUE );
	}
	else
	{
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;
		m_lastStage = GetTickCount();
	}
}

void CScanView::AutoStageXY_FI_Process11()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	theApp.m_DMCWin.Command( (char*)((const char*)theApp.m_Stage.xy.sCmdLY), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI11 );	// 停止第一步Y方向撞行程开关
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 21" );
		Sleep( 1000 );

		// 第二步，Y反方向向中间走，走至离中心约2mm
		str.Format( "JG %s", theApp.m_Stage.xy.sCmdFI21 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

		str.Format( "SHY;PR %s;BGY", theApp.m_Stage.xy.sCmdFI22 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI21, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process12()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	theApp.m_DMCWin.Command( (char*)((const char*)theApp.m_Stage.xy.sCmdLX), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;

	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI12 );	// 停止第一步撞行程开关
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 22" );
		Sleep( 1000 );

		// 第二步，反方向向中间走，走至离中心约2mm
		str.Format( "JG %s", theApp.m_Stage.xy.sCmdFI21 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

		str.Format( "SHX;PR %s;BGX", theApp.m_Stage.xy.sCmdFI22 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI22, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process21()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI21 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishY && abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		theApp.m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishY )
	{
		// Y方向已到达离中心约2mm处
		KillTimer( nAutoStageXY_FI21 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 31" );
		Sleep( 1000 );

		// 第三步，使用较低的速度寻找零点Index
		str.Format( "SHY;JG %s;FIY;BGY", theApp.m_Stage.xy.sCmdFI3 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI31, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process22()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI21 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if( !m_bStageFinishX && abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		theApp.m_Stage.xy.ptCurPosAb.x = lPosX;

	if( m_bStageFinishX )
	{
		// X方向已到达离中心约2mm处
		KillTimer( nAutoStageXY_FI22 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 32" );
		Sleep( 1000 );

		// 第三步，使用较低的速度寻找零点Index
		str.Format( "SHX;JG %s;FIX;BGX", theApp.m_Stage.xy.sCmdFI3 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI32, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process31()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI3 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if( abs(lPosY) < 10 )
	{
		KillTimer( nAutoStageXY_FI31 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 12" );
		Sleep( 1000 );

		// X方向撞行程开关
		str.Format( "SHX;JG %s;BGX", theApp.m_Stage.xy.sCmdFI1 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI12, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI_Process32()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI3 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if( abs(lPosX) < 10 )
	{
		KillTimer( nAutoStageXY_FI32 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the end of process 32" );
		Sleep( 1000 );
		Sleep( 1000 );

		// 第四步，校准
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
		{
			BOOL bVisible = FALSE;
			if( pfrm->m_dlgStage.m_hWnd != NULL )
				bVisible = ((pfrm->m_dlgStage.GetDlgItem(IDC_Stage_Calibrate)->GetStyle() & WS_VISIBLE) != 0);
			if( bVisible )
			{
				// 调试时，只找到Index就停止
				AutoStageXY_FI_Finish( TRUE );
				return;
			}
		}
		// 运行软件第一次找Index时，再走绝对距离为校准值
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");

		theApp.m_Stage.xy.ptCurPosAb.x = 0;
		theApp.m_Stage.xy.ptCurPosAb.y = 0;
		char	szBuf1[50], szBuf2[50], szBufRootSection[20];
		sprintf(szBufRootSection, "StagePos");
		if(GetPrivateProfileString(szBufRootSection, "Cali", "0", szBuf1, 255, path) != 0)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				theApp.m_Stage.xy.ptCurPosAb.x = atol(szBuf2);
				theApp.m_Stage.xy.ptCurPosAb.y = atol(szBuf1);
			}
		}
		if( theApp.m_Stage.xy.ptCurPosAb.x != 0 || theApp.m_Stage.xy.ptCurPosAb.y != 0 )
		{
			str.Format( "SHXY;PA %ld,%ld;BGXY", theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
			theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			theApp.DisplayMsg( str );
			SetTimer( nAutoStageXY_FI4, nAutoStageXY_FIElapse, NULL );
		}
		else
			AutoStageXY_FI_Finish( TRUE );
	}
}

void CScanView::AutoStageXY_FI2_Process11()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	theApp.m_DMCWin.Command( (char*)((const char*)theApp.m_Stage.xy.sCmdLY), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI11 );	// 停止第一步Y方向撞行程开关
		Sleep( 100 );

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 21" );
		Sleep( 500 );
		theApp.m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 21: to edge" );
		Sleep( 500 );

		// 定义原点
		theApp.m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= 0;

		// 第二步，Y反方向撞行程开关
		str.Format( "SHY;JG %s;BGY", theApp.m_Stage.xy.sCmdFI21 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI21, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process12()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	theApp.m_DMCWin.Command( (char*)((const char*)theApp.m_Stage.xy.sCmdLX), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;

	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI12 );	// 停止第一步X方向撞行程开关
		Sleep( 100 );

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 22" );
		Sleep( 500 );
		theApp.m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 22: to edge" );
		Sleep( 500 );
		// 定义原点
		theApp.m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= 0;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		// 第二步，X反方向撞行程开关
		str.Format( "SHX;JG %s;BGX", theApp.m_Stage.xy.sCmdFI21 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI22, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process21()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI21 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	theApp.m_DMCWin.Command( (char*)((const char*)theApp.m_Stage.xy.sCmdLY2), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;
	
	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI21 );	// 停止第二步Y反方向撞行程开关
		Sleep( 100 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 31" );
		Sleep( 500 );
		theApp.m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 31: return to center" );
		Sleep( 500 );

		// 第三步，向中心走到Y整个行程的一半
		str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHY;PAY=%ld;BGY", lPosY /2 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI31, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process22()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI21 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// 查询标志位
	theApp.m_DMCWin.Command( (char*)((const char*)theApp.m_Stage.xy.sCmdLX2), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// 得到当前位置
	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );
	// 如果位置在1秒内变化小于10，则人为设置标志位（等同于直接撞到行程开关）
	if(  abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
	
	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI22 );	// 停止第二步X反方向撞行程开关
		Sleep( 100 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 32" );
		Sleep( 500 );
		theApp.m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 32: return to center" );
		Sleep( 500 );

		// 第三步，向中心走到X整个行程的一半
		str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHX;PAX=%ld;BGX", lPosX /2 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI32, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process31()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishY && abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		theApp.m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishY )
	{
		KillTimer( nAutoStageXY_FI31 );	// 已到达中心处
		Sleep( 500 );
		// 保存现在的位置
		theApp.m_Stage.xy.ptCurPosAb.x	= lPosX;
		theApp.m_Stage.xy.ptCurPosAb.y	= lPosY;

		theApp.m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( "ST at the beginning of process 12" );
		Sleep( 500 );
		theApp.m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY at the beginning of process 12: to edge" );
		Sleep( 500 );

		// X方向撞行程开关
		CString str;
		str.Format( "SHX;JG %s;BGX", theApp.m_Stage.xy.sCmdFI1 );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageXY_FI12, nAutoStageXY_FIElapse, NULL );
	}
}

void CScanView::AutoStageXY_FI2_Process32()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", theApp.m_Stage.xy.sCmdFI1 );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if( !m_bStageFinishX && abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		theApp.m_Stage.xy.ptCurPosAb.x = lPosX;

	if( m_bStageFinishX )
	{
		KillTimer( nAutoStageXY_FI32 );	// 已到达中心处
		Sleep( 1000 );
		// 定义原点
		theApp.m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );

		// 第四步，校准
		CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
		if( pfrm )
		{
			BOOL bVisible = FALSE;
			if( pfrm->m_dlgStage.m_hWnd != NULL )
				bVisible = ((pfrm->m_dlgStage.GetDlgItem(IDC_Stage_Calibrate)->GetStyle() & WS_VISIBLE) != 0);
			if( bVisible )
			{
				// 调试时，只找到Index就停止
				AutoStageXY_FI_Finish( TRUE );
				return;
			}
		}
		// 运行软件第一次找Index时，再走绝对距离为校准值
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");

		theApp.m_Stage.xy.ptCurPosAb.x = 0;
		theApp.m_Stage.xy.ptCurPosAb.y = 0;
		char	szBuf1[50], szBuf2[50], szBufRootSection[20];
		sprintf(szBufRootSection, "StagePos");
		if(GetPrivateProfileString(szBufRootSection, "Cali", "0", szBuf1, 255, path) != 0)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				theApp.m_Stage.xy.ptCurPosAb.x = atol(szBuf2);
				theApp.m_Stage.xy.ptCurPosAb.y = atol(szBuf1);
			}
		}
		if( theApp.m_Stage.xy.ptCurPosAb.x != 0 || theApp.m_Stage.xy.ptCurPosAb.y != 0 )
		{
			str.Format( "SHXY;PA %ld,%ld;BGXY", theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
			theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			theApp.DisplayMsg( str );
			SetTimer( nAutoStageXY_FI4, nAutoStageXY_FIElapse, NULL );
		}
		else
			AutoStageXY_FI_Finish( TRUE );
	}
}

void CScanView::AutoStageXY_FI_Process4()
{
	long nPosX, nPosY;
	theApp.Stage_XY_GetPosition( nPosX, nPosY );

	long nDiffX	= theApp.m_Stage.xy.ptCurPosAb.x - nPosX;
	long nDiffY	= theApp.m_Stage.xy.ptCurPosAb.y - nPosY;
	if( -10 < nDiffX && nDiffX < 10 && -10 < nDiffY && nDiffY < 10 )
	{
		// 定义原点
		theApp.m_DMCWin.Command( "DP 0,0" );
		theApp.DisplayMsg( "DP 0,0" );
		// 软件限位
		char szBuffer[64];
		CString str;
		str.Format( "%s", theApp.m_Stage.xy.sCmdLimitX );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		str.Format( "%s", theApp.m_Stage.xy.sCmdLimitY );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		AutoStageXY_FI_Finish( TRUE );
	}
}

void CScanView::AutoStageXY_FI_Finish( BOOL bAutoFinish )
{
	KillTimer( nAutoStageXY_FI11 );
	KillTimer( nAutoStageXY_FI12 );
	KillTimer( nAutoStageXY_FI21 );
	KillTimer( nAutoStageXY_FI22 );
	KillTimer( nAutoStageXY_FI31 );
	KillTimer( nAutoStageXY_FI32 );
	KillTimer( nAutoStageXY_FI4 );

	if( bAutoFinish )
	{
		// 自动找到原点，完成退出
		if ( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}

	theApp.m_DMCWin.Command( "ST" );
	theApp.DisplayMsg( "ST in finish" );
	Sleep( 100 );
	theApp.m_DMCWin.Command( "MO" );
	theApp.DisplayMsg( "MO in finish" );
	theApp.m_DMCWin.Command( "SHXY" );
	theApp.DisplayMsg( "SHXY in finish" );
	Sleep( 100 );
	theApp.Stage_XY_GetPosition( theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
}

void CScanView::AutoStageXY_FI_TrackSecurity()
{
	// 获取自动台X/Y当前位置脉冲数
	theApp.Stage_XY_GetPosition( theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
	// 得到目前的实际位置mm
	BOOL bExceed = FALSE;
	int nActualX, nActualY;
	if( theApp.m_Stage.xy.bRev )
	{
		nActualX = theApp.m_Stage.xy.ptCurPosAb.y;
		// 绝对坐标
		if( nActualX > (1.0 *theApp.m_Stage.xy.nRangeY /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingY				// 1450000
			|| nActualX < -(1.0 *theApp.m_Stage.xy.nRangeY /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingY )			// -1450000
			bExceed = TRUE;
	}
	else
	{
		nActualX = theApp.m_Stage.xy.ptCurPosAb.x;
		// 绝对坐标
		if( nActualX > (1.0 *theApp.m_Stage.xy.nRangeX /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingX				// 1450000
			|| nActualX < -(1.0 *theApp.m_Stage.xy.nRangeX /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingX )			// -1450000 )
			bExceed = TRUE;
	}
	if( theApp.m_Stage.xy.bRev )
	{
		nActualY = theApp.m_Stage.xy.ptCurPosAb.x;
		// 绝对坐标
		if( nActualY > (1.0 *theApp.m_Stage.xy.nRangeX /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingX				// 1450000
			|| nActualY < -(1.0 *theApp.m_Stage.xy.nRangeX /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingX )			// -1450000 )
			bExceed = TRUE;
	}
	else
	{
		nActualY = theApp.m_Stage.xy.ptCurPosAb.y;
		// 绝对坐标
		if( nActualY > (1.0 *theApp.m_Stage.xy.nRangeY /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingY				// 1450000
			|| nActualY < -(1.0 *theApp.m_Stage.xy.nRangeY /2 -0.1)*1000000 /theApp.m_Stage.xy.nGratingY )			// -1450000
			bExceed = TRUE;
	}
	if( bExceed )
	{
		KillTimer( nAutoStageTrack );
		theApp.m_DMCWin.Command( "ST" );
		theApp.DisplayMsg( "ST in AutoStageXY_FI_TrackSecurity" );
		Sleep( 100 );
		theApp.m_DMCWin.Command( "MO" );
		theApp.DisplayMsg( "MO in AutoStageXY_FI_TrackSecurity" );
		theApp.m_DMCWin.Command( "SHXY" );
		theApp.DisplayMsg( "SHXY in AutoStageXY_FI_TrackSecurity" );
		Sleep( 100 );
		theApp.Stage_XY_GetPosition( theApp.m_Stage.xy.ptCurPosAb.x, theApp.m_Stage.xy.ptCurPosAb.y );
		AfxMessageBox( "超出行程范围！" );
		if ( m_bCatch )
		{
			m_bCatch = FALSE;
			ReleaseCapture();
			m_bStageTrack = FALSE;

			// 擦除标识，退出跟踪模式
			CPoint ptPoint	= m_ptStageTrackParam;
			if( ptPoint.x < nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.x = nSTAGE_TRACE_POINT_RADIUS;
			if( ptPoint.x > m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.x = m_sizeReso.cx - nSTAGE_TRACE_POINT_RADIUS;
			if( ptPoint.y < nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.y = nSTAGE_TRACE_POINT_RADIUS;
			if( ptPoint.y > m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS )
				ptPoint.y = m_sizeReso.cy - nSTAGE_TRACE_POINT_RADIUS;
			CRect rect(ptPoint.x-nSTAGE_TRACE_POINT_RADIUS,ptPoint.y-nSTAGE_TRACE_POINT_RADIUS,
				ptPoint.x+nSTAGE_TRACE_POINT_RADIUS,ptPoint.y+nSTAGE_TRACE_POINT_RADIUS);
			theApp.m_ImageCard.GraphDrawCircle( ptPoint, m_ptPosSave );
			theApp.m_ImageCard.GraphDrawCircle( ptPoint, m_ptPosSave, FALSE );
			theApp.m_ImageCard.GraphClear( rect, RGB(0,0,0) );
		}
	}
}

void CScanView::AutoStage_Clear( BOOL bZRT )
{
	if( !bZRT && theApp.m_Stage.nHasAutoStage == 1 && theApp.m_Stage.xy.bReady )
	{
		CDlgProgress		dlg;
		dlg.m_csTitle		= "Axis X/Y clearing";
		dlg.m_iRangeTo		= 20;
		dlg.m_csMessage		= "Axis X/Y is clearing now, please wait...";

		m_bStageFinishX		= FALSE;
		m_bStageFinishY		= FALSE;

		// 第一步先XY向归零
		char szBuffer[64];
		CString str;
		str.Format( "SP 2000,2000" );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHXY;PA 0,0;BGXY" );
		theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		theApp.DisplayMsg( str );
		SetTimer( nAutoStageClearXY, nAutoStageXY_FIElapse, NULL );

		m_pAutoProgressDlg	= &dlg;
		dlg.DoModal();
		m_pAutoProgressDlg	= NULL;

		AutoStage_Clear_Finish( FALSE, FALSE );
	}
	else if( theApp.m_Stage.nHasAutoStage > 1 && StageMC600_IsReady() )
	{
		CDlgProgress		dlg;
		dlg.m_csTitle		= "Axis move";
		dlg.m_iRangeTo		= 100;
		dlg.m_csMessage		= "Axis X/YZ/R is moving now, please wait...";

		StageMC600_OnVentExit();
		SetTimer(31, 250, NULL);

		m_pAutoProgressDlg	= &dlg;
		dlg.DoModal();
		m_pAutoProgressDlg	= NULL;

		AutoStage_Clear_Finish( FALSE, FALSE );
	}
}

void CScanView::AutoStage_Clear_XY()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP 5000,5000" );
	theApp.m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	theApp.Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishX && abs(lPosX - theApp.m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		theApp.m_Stage.xy.ptCurPosAb.x = lPosX;

	if(  !m_bStageFinishY && abs(lPosY - theApp.m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		theApp.m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishX && m_bStageFinishY )
	{
		// XY方向已归零
		KillTimer( nAutoStageClearXY );
		Sleep( 100 );
		AutoStage_Clear_Finish( FALSE, TRUE );
	}
}

void CScanView::AutoStage_Clear_Finish( BOOL bStep, BOOL bAutoFinish )
{
	if( !bStep )
		KillTimer( nAutoStageClearXY );
	if( bAutoFinish )
	{
		// XYT向完成清零
		if( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}
}
