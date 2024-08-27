// SemCtrl_Mouse.cpp : implementation file
//======2004-5-13_��Ļ�����Ƶ羵=====================
// �޸ı�ʾ��2004-5-13_��Ļ�����Ƶ羵
// �޸����ڣ�2004��5��13��������
// �޸��ˣ�������
// �޸�ԭ�������Ļ�������羵���ơ�

// Last Modify : 2009.12.02
/////////////////////////////////////////////////////////////////////////////
// 09.12.02����Ѱ���ʱ�ٰ����Ҽ�������ɢֵ��ʹ�ﾵֵ���ҡ�
//			����ȫ�ֱ���g_uAdjustorSort������SemCtrlMouse_Init������֮��
//			��SemCtrlMouse_Update�е���֮
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


//�������뺯��
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
	switch( uAdjustorSort )				//���ݵ������������ù��
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
			g_dfSlowDownlog10 = -2.3;	// SlowDownԼ����0.005����200�����ص�ı�һ��������
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
			// Mag = 100000			SlowDown =  0.005	= 10*(-2.3)	Լ����
			//     ==> A = -1.3   B = 4.2
			g_dfSlowDownlog10 = 4.2 - 1.3 * log10(CGlobalParamSetting::Instance().g_dfMag);
		}
		else
			g_dfSlowDownlog10 = -2.3;	// SlowDownԼ����0.005����200�����ص�ı�һ��������
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
			// Mag = 50000			SlowDown =  0.25	= 10*(-0.6)����1�����ظı�0.25����������256������64��������
			// Mag =  2000			SlowDown =  0.5		= 10*(-0.3)����1�����ظı�0.5����������256������128��������
			//     ==> A = -0.214   B = 0.407
			/////////////////////////////////////////////
			g_dfSlowDownlog10 = 0.407 - 0.214 * log10(CGlobalParamSetting::Instance().g_dfMag);
		}
		else if( CGlobalParamSetting::Instance().g_dfMag < 100000 )
		{
			/////////////////////////////////////////////
			// log10(SlowDown) = A * log10(Mag) + B
			// OffsetOLens = OffsetScreenX * dfSlowDown;
			// Mag = 50000			SlowDown =  0.25	= 10*(-0.6)����1�����ظı�0.25����������256������64��������
			// Mag = 100000			SlowDown =  0.125	= 10*(-0.9)����1�����ظı�0.125����������256������32��������
			//     ==> A = -1   B = 4.1
			g_dfSlowDownlog10 = 4.1 - log10(CGlobalParamSetting::Instance().g_dfMag);
		}
 		else
 			g_dfSlowDownlog10 = -0.9;	// 10*(-0.9) = 0.125����Լ8�����ظı�1��������
		break;
	default:
		g_lAdjustorX = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
		CSemCoreAssemble::Instance().m_ObjectiveLens.GetRange( g_varMinX, g_varMaxX );
		g_dfSlowDownlog10 = 2 - 0.75 * log10(CGlobalParamSetting::Instance().g_dfMag);
		break;
	}
	// 2004-5-14_�߱��������Ͽ���
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
	SerialNumber.lVal = -1;				// �㲥�޸����пؼ���ʾ
	switch( g_uAdjustorSort )			//���ݵ������������ù��
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
			Flag.lVal = 1;			// ��������Ϣ
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			g_lAdjustorOldX	= Xpos.lVal;
		}
		if( g_lAdjustorOldY != Ypos.lVal )
		{
			Flag.lVal = 2;			// ��������Ϣ
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			g_lAdjustorOldY	= Ypos.lVal;
		}
	}
	else
	{
		if( g_dAdjOldOL != Xpos.dblVal )
		{
			Flag.lVal = 1;			// ��������Ϣ
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

