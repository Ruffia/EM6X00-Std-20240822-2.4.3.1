// SemCtrl_Mouse.cpp : implementation file
//======2004-5-13_屏幕鼠标控制电镜=====================
// 修改标示：2004-5-13_屏幕鼠标控制电镜
// 修改日期：2004年5月13日星期四
// 修改人：罗明华
// 修改原因：添加屏幕鼠标操作电镜控制。

// Last Modify : 2009.12.02
/////////////////////////////////////////////////////////////////////////////
// 09.12.02左键已按下时再按下右键，消像散值会使物镜值错乱。
//			增加全局变量g_uAdjustorSort，并在SemCtrlMouse_Init中设置之，
//			在SemCtrlMouse_Update中调用之
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"

#include "SemCtrl_Mouse.h"
#include <math.h>
#include "ControlLayer.h"
#include "GlobalParamSetting.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "../SEMPanel/PanelModeMgr.h"
#include "SemCtrl_global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPoint	g_ptPosSave = CPoint(0,0);
long	g_lAdjustorX = 0;
long	g_lAdjustorY = 0;
long	g_lAdjustorOldX = 0;
long	g_lAdjustorOldY = 0;
double	g_dfSlowDownlog10 = 0.0;
VARIANT	g_varMinX, g_varMaxX, g_varMinY, g_varMaxY;
double	g_dAdjOL = 0.0;
double	g_dAdjOldOL = 0.0;

UINT	g_uAdjustorSort;


//四舍五入函数
int RoundFunc( double dNum )
{
	int nMin = (int)dNum;
	if( dNum < 0 )
		return ( (fabs(dNum-nMin) < 0.5) ? nMin : (nMin - 1) );
	else
		return ( (dNum-nMin < 0.5) ? nMin : (nMin + 1) );
}

void SemCtrlMouse_Init(CPoint point, UINT uAdjustorSort)
{
	g_ptPosSave = point;
	switch( uAdjustorSort )				//根据调节器类型设置光标
	{
	case	SORT_BEAM_ALIGNMENT:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos().lVal;
		g_lAdjustorY = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos().lVal;
		CSemCoreAssemble::Instance().m_BeemAlignmentX.GetRange( g_varMinX, g_varMaxX );
		CSemCoreAssemble::Instance().m_BeemAlignmentY.GetRange( g_varMinY, g_varMaxY );
		g_dfSlowDownlog10 = -1.2;
		break;
	case	SORT_SCAN_SHIFT:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos().lVal;
		g_lAdjustorY = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos().lVal;
		CSemCoreAssemble::Instance().m_ScanShiftX.GetRange( g_varMinX, g_varMaxX );
		CSemCoreAssemble::Instance().m_ScanShiftY.GetRange( g_varMinY, g_varMaxY );
		g_dfSlowDownlog10 = -1.2;
		break;
	case	SORT_BEAM_ROTATE:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_ScanRotate.GetPos().lVal;
		g_lAdjustorY = 0;
		CSemCoreAssemble::Instance().m_ScanRotate.GetRange( g_varMinX, g_varMaxX );
		g_dfSlowDownlog10 = -1.2;
		break;
	case	SORT_CONDENSER_LENS:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_CondenserLens.GetPos().lVal;
		g_lAdjustorY = 0;
		CSemCoreAssemble::Instance().m_CondenserLens.GetRange( g_varMinX, g_varMaxX );
		if( CGlobalParamSetting::Instance().g_dfMag < 10000 )
			g_dfSlowDownlog10 = 2 - 0.75 * log10(CGlobalParamSetting::Instance().g_dfMag);
		else if( CGlobalParamSetting::Instance().g_dfMag < 100000 )
			g_dfSlowDownlog10 = 4.2 - 1.3 * log10(CGlobalParamSetting::Instance().g_dfMag);
		else
			g_dfSlowDownlog10 = -2.3;	// SlowDown约等于0.005，即200个像素点改变一个控制量
		break;
	case	SORT_OBJECTIVE_LENS:
		g_dAdjOL	 = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().dblVal;
		if( g_dAdjOL < 0.001 )
		{
			if( g_dAdjOldOL > -1.0 )
			{
				g_dAdjOL = g_dAdjOldOL;
			}
			else
			{
				g_dAdjOL = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
			}
		}

		g_dAdjOldOL	 = g_dAdjOL;
		g_lAdjustorX = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
		g_lAdjustorY = 0;
		CSemCoreAssemble::Instance().m_ObjectiveLens.GetRange( g_varMinX, g_varMaxX );
		if( CGlobalParamSetting::Instance().g_dfMag < 10000 )
		{
			/////////////////////////////////////////////
			// log10(SlowDown) = A * log10(Mag) + B
			// OffsetOLens = OffsetScreenX * dfSlowDown;
			// Mag = 10000			SlowDown =  0.1
			// Mag =     1			SlowDown =  100
			//     ==> A = -0.75   B = 2
			/////////////////////////////////////////////
			g_dfSlowDownlog10 = 2 - 0.75 * log10(CGlobalParamSetting::Instance().g_dfMag);
		}
		else if( CGlobalParamSetting::Instance().g_dfMag < 100000 )
		{
			/////////////////////////////////////////////
			// log10(SlowDown) = A * log10(Mag) + B
			// OffsetOLens = OffsetScreenX * dfSlowDown;
			// Mag = 10000			SlowDown =  0.1		= 10*(-1)
			// Mag = 100000			SlowDown =  0.005	= 10*(-2.3)	约等于
			//     ==> A = -1.3   B = 4.2
			g_dfSlowDownlog10 = 4.2 - 1.3 * log10(CGlobalParamSetting::Instance().g_dfMag);
		}
		else
			g_dfSlowDownlog10 = -2.3;	// SlowDown约等于0.005，即200个像素点改变一个控制量
		break;
	case	SORT_BRIGHT:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_Brightness.GetPos().lVal;
		g_lAdjustorY = 0;
		CSemCoreAssemble::Instance().m_Brightness.GetRange( g_varMinX, g_varMaxX );
		g_dfSlowDownlog10 = -1.2;
		break;
	case	SORT_CONTRAST:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_Contrast.GetPos().lVal;
		g_lAdjustorY = 0;
		CSemCoreAssemble::Instance().m_Contrast.GetRange( g_varMinX, g_varMaxX );
		g_dfSlowDownlog10 = -0.7;
		break;
	case	SORT_STIGMATOR:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_StigmatorX.GetPos().lVal;
		g_lAdjustorY = CSemCoreAssemble::Instance().m_StigmatorY.GetPos().lVal;
		CSemCoreAssemble::Instance().m_StigmatorX.GetRange( g_varMinX, g_varMaxX );
		CSemCoreAssemble::Instance().m_StigmatorY.GetRange( g_varMinY, g_varMaxY );
//		g_dfSlowDownlog10 = -2.18;//-1.2
		if( CGlobalParamSetting::Instance().g_dfMag < 50000 )
		{
			/////////////////////////////////////////////
			// log10(SlowDown) = A * log10(Mag) + B
			// OffsetOLens = OffsetScreenX * dfSlowDown;
			// Mag = 50000			SlowDown =  0.25	= 10*(-0.6)，即1个像素改变0.25个控制量，256个像素64个控制量
			// Mag =  2000			SlowDown =  0.5		= 10*(-0.3)，即1个像素改变0.5个控制量，256个像素128个控制量
			//     ==> A = -0.214   B = 0.407
			/////////////////////////////////////////////
			g_dfSlowDownlog10 = 0.407 - 0.214 * log10(CGlobalParamSetting::Instance().g_dfMag);
		}
		else if( CGlobalParamSetting::Instance().g_dfMag < 100000 )
		{
			/////////////////////////////////////////////
			// log10(SlowDown) = A * log10(Mag) + B
			// OffsetOLens = OffsetScreenX * dfSlowDown;
			// Mag = 50000			SlowDown =  0.25	= 10*(-0.6)，即1个像素改变0.25个控制量，256个像素64个控制量
			// Mag = 100000			SlowDown =  0.125	= 10*(-0.9)，即1个像素改变0.125个控制量，256个像素32个控制量
			//     ==> A = -1   B = 4.1
			g_dfSlowDownlog10 = 4.1 - log10(CGlobalParamSetting::Instance().g_dfMag);
		}
 		else
 			g_dfSlowDownlog10 = -0.9;	// 10*(-0.9) = 0.125，即约8个像素改变1个控制量
		break;
	default:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
		CSemCoreAssemble::Instance().m_ObjectiveLens.GetRange( g_varMinX, g_varMaxX );
		g_dfSlowDownlog10 = 2 - 0.75 * log10(CGlobalParamSetting::Instance().g_dfMag);
		break;
	}
	// 2004-5-14_高倍像操作组合控制
	g_dfSlowDownlog10 *= theApp.m_dfMouseSpeed;
	g_lAdjustorOldX = g_lAdjustorX;
	g_lAdjustorOldY = g_lAdjustorY;

	g_uAdjustorSort = uAdjustorSort;
}

void SemCtrlMouse_Update(CPoint point, UINT uAdjustorSort)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	g_ptPosSave;
	CPoint ptPos1 = point;
	/////////////////////////////////////////////
	double	dfSlowDown = pow( 10, g_dfSlowDownlog10 );
	
//	const long UScaleMin = 64;
//	const long UScaleMax = 1024;
	long nOffsetX	= ptPos1.x - ptPos0.x;
	long nOffsetY	= ptPos0.y - ptPos1.y;

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;				// 广播修改所有控件显示
	switch( g_uAdjustorSort )			//根据调节器类型设置光标
	{
	case	SORT_BEAM_ALIGNMENT:
		ClassID.lVal = SemControlPanelClass_BeemAlignment;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = RoundFunc(g_lAdjustorY + nOffsetY * dfSlowDown);
		break;
	case	SORT_SCAN_SHIFT:
		ClassID.lVal = SemControlPanelClass_ScanShift;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = RoundFunc(g_lAdjustorY + nOffsetY * dfSlowDown);
		break;
	case	SORT_BEAM_ROTATE:
		ClassID.lVal = SemControlPanelClass_ScanRotate;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = 0;
		break;
	case	SORT_CONDENSER_LENS:
		ClassID.lVal = SemControlPanelClass_CondenserLens;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = 0;
		break;
	case	SORT_OBJECTIVE_LENS:
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Xpos.dblVal = g_dAdjOL + nOffsetX * dfSlowDown;
		Xpos.lVal = (long)(Xpos.dblVal);
		Ypos.lVal = 0;
		break;
	case	SORT_BRIGHT:
		ClassID.lVal = SemControlPanelClass_Brightness;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = 0;
		break;
	case	SORT_CONTRAST:
		ClassID.lVal = SemControlPanelClass_Contrast;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = 0;
		break;
	case	SORT_STIGMATOR:
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = RoundFunc(g_lAdjustorY + nOffsetY * dfSlowDown);
		break;
	default:
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Xpos.lVal = RoundFunc(g_lAdjustorX + nOffsetX * dfSlowDown);
		Ypos.lVal = 0;
		break;
	}

	if( Xpos.lVal < g_varMinX.lVal )
		Xpos.lVal = g_varMinX.lVal;
	if( Xpos.lVal > g_varMaxX.lVal )
		Xpos.lVal = g_varMaxX.lVal;
	if( Ypos.lVal < g_varMinY.lVal )
		Ypos.lVal = g_varMinY.lVal;
	if( Ypos.lVal > g_varMaxY.lVal )
		Ypos.lVal = g_varMaxY.lVal;

	if( g_uAdjustorSort != SORT_OBJECTIVE_LENS )
	{
		if( g_lAdjustorOldX != Xpos.lVal )
		{
			Flag.lVal = 1;			// 发控制消息
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			g_lAdjustorOldX	= Xpos.lVal;
		}
		if( g_lAdjustorOldY != Ypos.lVal )
		{
			Flag.lVal = 2;			// 发控制消息
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			g_lAdjustorOldY	= Ypos.lVal;
		}
	}
	else
	{
		if( g_dAdjOldOL != Xpos.dblVal )
		{
			Flag.lVal = 1;			// 发控制消息
			if (CPanelModeMgr::Instance().FocusMode != FocusControlMode_ScreenFocus)
			{
				CPanelModeMgr::Instance().FocusModeBK = CPanelModeMgr::Instance().FocusMode;
				CPanelModeMgr::Instance().FocusMode = FocusControlMode_ScreenFocus;
			}
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			g_dAdjOldOL	= Xpos.dblVal;
		}
	}
}

