// Algorithm_AutoAstigmat.cpp : implementation file
//
// Last Modify : 2009.04.15

#include "stdafx.h"
#include "Algorithm_AutoFocus.h"
#include "FreTrans.h"
#include "CurveFitting.h"
#include <algorithm>
using namespace std;


/////////////////////////////////////////////////////////////////////////////

typedef struct tagMagCtrl{
	WORD	Flag0		: 2;	// 00000000 000000??
	WORD	Flag1		: 2;	// 00000000 0000??00
	WORD	Digit_1		: 4;	// 00000000 ????0000
	WORD	Digit_10	: 4;	// 0000???? 00000000
	WORD	Digit_100	: 4;	// ????0000 00000000
}MagCtrl;

typedef union tagMagCtrlParam
{
	MagCtrl	CTL;
	WORD	nData;
}MagCtrlParam;

typedef struct tagAF_Table{
	long	nFocus;
	double	dfSharpness;
}AF_Table;

/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// coarse focusing
//////////////////////////////////////////////////////////////////
/*
#define			c_nAFTableLength  16
AF_Table		g_AFLookTable[ c_nAFTableLength ];

const long		c_nFocusMAX	=	65535;
const long		c_nFocusMIN	=	0;

const long		c_nFocusStart	=	250*64;
const long		c_nFocusStop	=	750*64;
const long		c_nFocusStep	=   2000;	//(c_nFocusStop - c_nFocusStart) / c_nAFTableLength;
*/
/*
//ZXY 09.04.15
#define			c_nAFTableLength  27					// 28
AF_Table		g_AFLookTable[ c_nAFTableLength ];

const long		c_nFocusMAX	=	65535;
const long		c_nFocusMIN	=	0;

const long		c_nFocusStart	=	250*64;
const long		c_nFocusStop	=	750*64;
const long		c_nFocusStep	=   (c_nFocusStop - c_nFocusStart) / c_nAFTableLength;	//1142;
//ZXY 09.04.15
*/
// 2014.04
#define			c_nAFTableLength  20//27					// 28
AF_Table		g_AFLookTable[ c_nAFTableLength ];

const long		c_nFocusMAX	=	32767;
const long		c_nFocusMIN	=	0;

const long		c_nFocusStart	=	320*32;
const long		c_nFocusStop	=	750*32;
const long		c_nFocusStep	=   (c_nFocusStop - c_nFocusStart) / c_nAFTableLength;	//1142;
// 2014.04

//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// fine focusing
//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Q: "物镜聚焦磁滞归偏移矫正量"？
// A: （分析AF过程记录）（AF_Test_030609_002.doc）
//     - 4 * 细调1单步宽度（4*64）= - 16*64
///////////////////////////////////////////////////////////

//"物镜聚焦磁滞归偏移矫正量"
// BH: A hysteresis loop of four quadrants.

//const long		c_nHysteresisError_Fine1 = - 6*64;
const long		c_nHysteresisError_Fine1 = - 6*32;	// 2014.04

#define			c_nAFTableLength_Fine   8	// 12
#define			c_nAFTableLength_Fine1  12
AF_Table		g_AFLookTable_Fine[ c_nAFTableLength_Fine1 ];

const long		c_nFineAFStep   =   2*c_nFocusStep/c_nAFTableLength_Fine1;		// 3*64;4*64	// ZXY 09.04.15
const long		c_nFineAFStart  = - c_nAFTableLength_Fine1 * c_nFineAFStep / 2 + c_nHysteresisError_Fine1;
const long		c_nFineAFStop   = + c_nAFTableLength_Fine1 * c_nFineAFStep / 2 + c_nHysteresisError_Fine1;

const long		c_nFine2AFStep  =   2* c_nFineAFStep/c_nAFTableLength_Fine;		// 64;	// ZXY 09.04.15
const long		c_nFine2AFStart  = - c_nAFTableLength_Fine * c_nFine2AFStep / 2;
const long		c_nFine2AFStop   = + c_nAFTableLength_Fine * c_nFine2AFStep / 2;

const long		c_nFine3AFStep  =   2* c_nFine2AFStep/c_nAFTableLength_Fine;	// 16;	// ZXY 09.04.15
const long		c_nFine3AFStart  = - c_nAFTableLength_Fine * c_nFine3AFStep / 2;
const long		c_nFine3AFStop   = + c_nAFTableLength_Fine * c_nFine3AFStep / 2;

const long		c_nFine4AFStep  =   2* c_nFine3AFStep/c_nAFTableLength_Fine;	// 4;	// ZXY 09.04.15
const long		c_nFine4AFStart  = - c_nAFTableLength_Fine * c_nFine4AFStep / 2;
const long		c_nFine4AFStop   = + c_nAFTableLength_Fine * c_nFine4AFStep / 2;

const WORD		c_nFineGradeMax = 4;	// 4

//////////////////////////////////////////////////////////////////

const double	c_dblMagnifierAF_Scan	= 500.0;	// 200.0
const double	c_dblMagnifierAF_Fine	= 5000.0;	// 2000.0
const double	c_dblMagnifierAF_Fine1	= 10000.0;		// ZXY 09.04.15 Add

double	g_dblMagnifierAF;
long	g_nFocusStart;
long	g_nFocusStop;
long	g_nFocusStep;
long	g_nAFIndex;

long	g_nAFTableLength_Fine;

BOOL	g_boolFineAF = FALSE;
WORD	g_nFineGrade = 0;
WORD	g_nFineGradeMax;

double	g_dblMagnifierSaved;
double	g_dblMagnifierAF_Fine =	c_dblMagnifierAF_Fine;

// ZXY 09.04.15 Add
long	g_nAFIndexMax;
AF_Table g_AFTableABP1;
AF_Table g_AFTableABP2;

double g_dbSharpnessMax;

const double c_dbSharpnessAB_Scan = 0.0025;
const double c_dbSharpnessAB_Fine = 0.0015;
WORD g_nGrade =0;

double	g_dblMagnifier_Old;
// ZXY 09.04.15 Add End

//HY  2022.12.28 Add
#define			c_nAFTableLength1  10//27
#define			c_nAFTableLength_Fine2   6	// 12

AF_Table		g_AFLookTablehy[ c_nAFTableLength1 ];
AF_Table        g_AFTableABP3;

const long    c_nAutoFocusStep = 4 * 32;
const long    c_nAutoFine1FocusStep = 2 * 32;
const long    c_nAutoFine2FocusStep = 0.8 * 32;
const long    c_nAutoFine3FocusStep =0.8 *0.48 *32;

const double    c_dbSharpnessAB_Scanhy1 = 12 ;
const double    c_dbSharpnessAB_Finehy = 0.5;

long            g_nAutoFocusStep;
long            g_nFocusFine1Step;
long            g_nFocusFine2Step;

BOOL            bDrection = FALSE;

#pragma (comment,"J:/EM6X00-MC6600-粗焦-kappa-细焦/Bin/QXD.lib")
extern "C" void QXD(int&, int&, int*, double&);

/////////////////////////////////////////////////////////////////////////////
// AutoFocus

/*
double WINAPI CtrlToMag( WORD wCtrl )
{
	double			dfMag = 0;
	MagCtrlParam	MAG;

	MAG.nData = wCtrl;

	dfMag += MAG.CTL.Digit_100 * 100;
	dfMag += MAG.CTL.Digit_10 * 10;
	dfMag += MAG.CTL.Digit_1;

	if ( MAG.CTL.Flag1 == 2 )		dfMag /= 100;
	else if ( MAG.CTL.Flag1 == 1 )	dfMag /= 10;

	if ( MAG.CTL.Flag0 == 2 )		dfMag *= 1000;

	return dfMag;
}

WORD WINAPI MagToCtrl( double dfMag )
{
	MagCtrlParam	MAG;
	long			nMagTemp;

	if ( dfMag >= 999000.0 )
	{
		MAG.CTL.Flag0		= 2;
		MAG.CTL.Flag1		= 0;

		MAG.CTL.Digit_100	= 9;
		MAG.CTL.Digit_10	= 9;
		MAG.CTL.Digit_1		= 9;
	}
	else if ( dfMag > 100000.0 )
	{
		nMagTemp = dfMag / 1000;
		MAG.CTL.Flag0		= 2;
		MAG.CTL.Flag1		= 0;

		MAG.CTL.Digit_100	=  nMagTemp / 100;
		nMagTemp			-= MAG.CTL.Digit_100 * 100;
		MAG.CTL.Digit_10	=  nMagTemp / 10;
		nMagTemp			-= MAG.CTL.Digit_10 * 10;
		MAG.CTL.Digit_1		=  nMagTemp;
	}
	else if ( dfMag > 10000.0 )
	{
		nMagTemp = dfMag / 100;
		MAG.CTL.Flag0		= 2;
		MAG.CTL.Flag1		= 1;

		MAG.CTL.Digit_100	=  nMagTemp / 100;
		nMagTemp			-= MAG.CTL.Digit_100 * 100;
		MAG.CTL.Digit_10	=  nMagTemp / 10;
		nMagTemp			-= MAG.CTL.Digit_10 * 10;
		MAG.CTL.Digit_1		=  nMagTemp;
	}
	else if ( dfMag > 1000.0 )
	{
		nMagTemp = dfMag / 10;
		MAG.CTL.Flag0		= 2;
		MAG.CTL.Flag1		= 2;

		MAG.CTL.Digit_100	=  nMagTemp / 100;
		nMagTemp			-= MAG.CTL.Digit_100 * 100;
		MAG.CTL.Digit_10	=  nMagTemp / 10;
		nMagTemp			-= MAG.CTL.Digit_10 * 10;
		MAG.CTL.Digit_1		=  nMagTemp;
	}
	else if ( dfMag > 100.0 )
	{
		nMagTemp = dfMag;
		MAG.CTL.Flag0		= 0;
		MAG.CTL.Flag1		= 0;

		MAG.CTL.Digit_100	=  nMagTemp / 100;
		nMagTemp			-= MAG.CTL.Digit_100 * 100;
		MAG.CTL.Digit_10	=  nMagTemp / 10;
		nMagTemp			-= MAG.CTL.Digit_10 * 10;
		MAG.CTL.Digit_1		=  nMagTemp;
	}
	else if ( dfMag > 10.0 )
	{
		nMagTemp = dfMag * 10;
		MAG.CTL.Flag0		= 0;
		MAG.CTL.Flag1		= 1;

		MAG.CTL.Digit_100	=  nMagTemp / 100;
		nMagTemp			-= MAG.CTL.Digit_100 * 100;
		MAG.CTL.Digit_10	=  nMagTemp / 10;
		nMagTemp			-= MAG.CTL.Digit_10 * 10;
		MAG.CTL.Digit_1		=  nMagTemp;
	}
	else
	{
		nMagTemp = dfMag * 100;
		MAG.CTL.Flag0		= 0;
		MAG.CTL.Flag1		= 2;

		MAG.CTL.Digit_100	=  nMagTemp / 100;
		nMagTemp			-= MAG.CTL.Digit_100 * 100;
		MAG.CTL.Digit_10	=  nMagTemp / 10;
		nMagTemp			-= MAG.CTL.Digit_10 * 10;
		MAG.CTL.Digit_1		=  nMagTemp;
	}

	return MAG.nData;
}
*/

void WINAPI AutoFocus_Initialize(
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	g_dblMagnifierSaved = dblMagnifier;

	g_boolFineAF	= FALSE;
	g_nFineGrade	= 0;

	g_dblMagnifierAF	= c_dblMagnifierAF_Scan;
	g_nFocusStart		= c_nFocusStart;
	g_nFocusStop		= c_nFocusStop;
	g_nFocusStep		= c_nFocusStep;
	g_nAFIndex			= 0;

	//////////////////////////////////////////////
	// 2003-7-21 Leo
	//////////////////////////////////////////////
	
	WORD nGrade		= 0;
	if ( dblMagnifier < 200 )			nGrade		= 3;
	else if ( dblMagnifier < 2000 )		nGrade		= 2;
	else if ( dblMagnifier < 10000 )	nGrade		= 1;
	else								nGrade		= 0;

	switch ( nGrade )
	{
	case 3: // 14.9  X
		g_dblMagnifierAF_Fine =	dblMagnifier;
		g_nFineGradeMax = c_nFineGradeMax - 3;
		
		break;

	case 2: // 149   X
		g_dblMagnifierAF_Fine =	dblMagnifier;
		g_nFineGradeMax = c_nFineGradeMax - 2;
		break;

	case 1: // 1.49 KX
		g_dblMagnifierAF_Fine =	c_dblMagnifierAF_Fine;
		g_nFineGradeMax = c_nFineGradeMax - 1;
		break;

	case 0: // 14.9 KX
		g_dblMagnifierAF_Fine =	c_dblMagnifierAF_Fine;
		g_nFineGradeMax = c_nFineGradeMax;
		break;

	default: 
		g_dblMagnifierAF_Fine =	c_dblMagnifierAF_Fine;
		g_nFineGradeMax = c_nFineGradeMax;
		break;
	}
	//////////////////////////////////////////////

	for (long i=0; i<c_nAFTableLength; i++)
	{
		g_AFLookTable[i].dfSharpness = 0;
		lFinalLens = g_nFocusStart + g_nFocusStep * i;
		
		if (lFinalLens > c_nFocusMAX ) lFinalLens = c_nFocusMAX;
		if (lFinalLens < c_nFocusMIN ) lFinalLens = c_nFocusMIN;
		g_AFLookTable[i].nFocus = lFinalLens;
	}

	lFinalLens		= g_AFLookTable[0].nFocus;			//初始化物镜电流从250-750 28份
	dblMagnifier	= g_dblMagnifierAF;					//初始化放大倍数变为200X
}

//2022.12.28 hy 
void WINAPI AutoFocus_Initialize1(
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	g_dblMagnifierSaved = dblMagnifier;
	g_boolFineAF	= FALSE;
	g_nFineGrade	= 0;
	g_dblMagnifierAF	= dblMagnifier;
	g_nFocusStart		= lFinalLens ;
	g_nAutoFocusStep	= c_nAutoFocusStep;
    g_nAFIndex			= 0;

	for (long i=0; i<c_nAFTableLength1; i++)
	{
		if(i<3)
		{
			g_AFLookTablehy[i].dfSharpness = 0;
			lFinalLens = g_nFocusStart + g_nAutoFocusStep*i;
			lFinalLens = min(lFinalLens,c_nFocusMAX);
			lFinalLens = max(lFinalLens,c_nFocusMIN);
			g_AFLookTablehy[i].nFocus = lFinalLens;
		}
		else
		{
			g_AFLookTablehy[i].dfSharpness = 0;
			g_AFLookTablehy[i].nFocus = 0;
		}	
	}
	lFinalLens		= g_AFLookTablehy[0].nFocus;			//初始化物镜电流从250-750 28份
	dblMagnifier	= g_dblMagnifierAF;					//初始化放大倍数变为200X

}
//2022.12.28 hy 
void WINAPI AutoFocus_Initialize2(
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	g_dblMagnifierSaved = dblMagnifier;

	g_boolFineAF	= FALSE;
	g_nFineGrade	= 0;

	g_dblMagnifierAF	= c_dblMagnifierAF_Scan;
	g_nFocusStart		= c_nFocusStart;
	g_nFocusStop		= c_nFocusStop;
	g_nFocusStep		= c_nFocusStep;
	g_nAFIndex			= 0;

	///////////////////////////////////////////////////////
	// 2009.04.15_自动聚焦算法优化
	// 按放大倍数所处的不同区间分为0-3共4个阶段:
	//		c_dblMagnifierAF_Scan|	c_dblMagnifierAF_Fine|	c_dblMagnifierAF_Fine1|
	//	阶段					3|						2|						1|		0
	//	精选次数				1|						2|						3|		4

	if ( g_dblMagnifierSaved <= c_dblMagnifierAF_Scan )
		g_nGrade		= 3;
	else if ( g_dblMagnifierSaved <= c_dblMagnifierAF_Fine )
		g_nGrade		= 2;
	else if ( g_dblMagnifierSaved <= c_dblMagnifierAF_Fine1 )
		g_nGrade		= 1;
	else
		g_nGrade		= 0;

	// 精选最大循环次数；可能根据测试结果用case语句重新设置最大循环次数
	g_nFineGradeMax = c_nFineGradeMax - g_nGrade;
 
	for (long i=0; i<c_nAFTableLength; i++)
	{
		g_AFLookTable[i].dfSharpness = 0;
		lFinalLens = g_nFocusStart + g_nFocusStep * i;
		
		lFinalLens = min(lFinalLens,c_nFocusMAX);
		lFinalLens = max(lFinalLens,c_nFocusMIN);
		g_AFLookTable[i].nFocus = lFinalLens;
	}

	lFinalLens		= g_AFLookTable[0].nFocus;			//初始化物镜电流从250-750 28份
	dblMagnifier	= g_dblMagnifierAF;					//初始化放大倍数变为200X

	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"AutoFocus_Initialize2 -----g_dblMagnifierSaved = %f,g_nFocusStart = %ld,g_nFocusStop = %d,g_nFocusStep = %d,g_nAFIndex = %d,g_nGrade = %d,g_nFineGradeMax = %d \n", 
		g_dblMagnifierSaved,g_nFocusStart,g_nFocusStop,g_nFocusStep,g_nAFIndex,g_nGrade,g_nFineGradeMax );
	OutputDebugString(sz);
}

void AutoFocus_Fine_Initialize(
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	g_dblMagnifierAF		= c_dblMagnifierAF_Fine;
	g_nAFTableLength_Fine	= c_nAFTableLength_Fine;

	///////////////////////////////////////
	if ( g_nFineGrade == 0 )
	{
		g_nFocusStart	= g_nFocusStart + c_nFocusStep * g_nAFIndex + c_nFineAFStart;
		g_nFocusStop	= g_nFocusStart + c_nFocusStep * g_nAFIndex + c_nFineAFStop;
		g_nFocusStep	= c_nFineAFStep;
		g_nAFIndex		= 0;
		g_nAFTableLength_Fine = c_nAFTableLength_Fine1;
	}
	else if ( g_nFineGrade == 1 )
	{
		g_nFocusStart	= g_nFocusStart + c_nFineAFStep * g_nAFIndex + c_nFine2AFStart;
		g_nFocusStop	= g_nFocusStart + c_nFineAFStep * g_nAFIndex + c_nFine2AFStop;
		g_nFocusStep	= c_nFine2AFStep;
		g_nAFIndex		= 0;
		g_dblMagnifierAF	= g_dblMagnifierSaved;//取回原设定倍数
	}
	else if ( g_nFineGrade == 2 )
	{
		g_nFocusStart	= g_nFocusStart + c_nFine2AFStep * g_nAFIndex + c_nFine3AFStart;
		g_nFocusStop	= g_nFocusStart + c_nFine2AFStep * g_nAFIndex + c_nFine3AFStop;
		g_nFocusStep	= c_nFine3AFStep;
		g_nAFIndex		= 0;
		g_dblMagnifierAF	= g_dblMagnifierSaved;
	}
	else if ( g_nFineGrade == 3 )
	{
		g_nFocusStart	= g_nFocusStart + c_nFine3AFStep * g_nAFIndex + c_nFine4AFStart;
		g_nFocusStop	= g_nFocusStart + c_nFine3AFStep * g_nAFIndex + c_nFine4AFStop;
		g_nFocusStep	= c_nFine4AFStep;
		g_nAFIndex		= 0;
		g_dblMagnifierAF	= g_dblMagnifierSaved;
	}
	g_nFineGrade ++;
	///////////////////////////////////////

	///////////////////////////////////////
	for (long i=0; i<g_nAFTableLength_Fine; i++)
	{
		g_AFLookTable_Fine[i].dfSharpness = 0;
		lFinalLens = g_nFocusStart + g_nFocusStep * i;
		
		if (lFinalLens > c_nFocusMAX ) lFinalLens = c_nFocusMAX;
		if (lFinalLens < c_nFocusMIN ) lFinalLens = c_nFocusMIN;
		g_AFLookTable_Fine[i].nFocus = lFinalLens;
	}

	lFinalLens		= g_AFLookTable_Fine[0].nFocus;
	dblMagnifier	= g_dblMagnifierAF;
}

void WINAPI AutoFocus_Fine_Initialize1(
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	g_dblMagnifierAF		= dblMagnifier;
	g_nAFTableLength_Fine	= c_nAFTableLength_Fine2;
	
	if ( g_nFineGrade == 0 )							//设置step和放大倍数
	{
		g_nFocusFine1Step	= c_nAutoFine1FocusStep;				//步进值
		g_nAFIndex		= 0;
		
		if(dblMagnifier <= 3000)
			g_nFocusFine2Step	= c_nAutoFine2FocusStep;
		else
			g_nFocusFine2Step	= c_nAutoFine3FocusStep;
	}
	else if ( g_nFineGrade == 1 )
	{
		g_nFocusFine1Step	= g_nFocusFine2Step;
		g_nAFIndex		= 0;
	}
	g_nFineGrade ++;
	g_nFocusStart	= g_AFTableABP2.nFocus;	
	
	for (long i=0; i<g_nAFTableLength_Fine; i++)
	{
		if(i<3)
		{
			g_AFLookTable_Fine[i].dfSharpness = 0;
			lFinalLens = g_nFocusStart + g_nFocusFine1Step * (i-1);
		
			if (lFinalLens > c_nFocusMAX ) lFinalLens = c_nFocusMAX;
			if (lFinalLens < c_nFocusMIN ) lFinalLens = c_nFocusMIN;
			g_AFLookTable_Fine[i].nFocus = lFinalLens;
		}
		else
		{
			g_AFLookTable_Fine[i].dfSharpness = 0;
			lFinalLens = 0;
			if (lFinalLens > c_nFocusMAX ) lFinalLens = c_nFocusMAX;
			if (lFinalLens < c_nFocusMIN ) lFinalLens = c_nFocusMIN;
			g_AFLookTable_Fine[i].nFocus = lFinalLens;
		}
		
	}
	
	lFinalLens		= g_AFLookTable_Fine[g_nAFIndex].nFocus;
	dblMagnifier	= g_dblMagnifierAF;
}
void WINAPI AutoFocus_Fine_Initialize2(
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	g_dblMagnifierAF		= c_dblMagnifierAF_Fine;
	g_nAFTableLength_Fine	= c_nAFTableLength_Fine;

	//======2009.04.15_自动聚焦算法优化====================
	// 设定细调步进
	if ( g_nFineGrade == 0 )							//设置step和放大倍数
	{
		g_nFocusStep	= c_nFineAFStep;				//步进值
		g_nAFIndex		= 0;
		g_nAFTableLength_Fine = c_nAFTableLength_Fine1;	//段值
	}
	else if ( g_nFineGrade == 1 )
	{
		g_nFocusStep	= c_nFine2AFStep;
		g_nAFIndex		= 0;
		if( g_dblMagnifierSaved < c_dblMagnifierAF_Fine1)
		{
			g_dblMagnifierAF	= g_dblMagnifierSaved;	//取回原设定倍数
		}
		else
		{
			g_dblMagnifierAF = c_dblMagnifierAF_Fine1;
		}
	}
	else if ( g_nFineGrade == 2 )
	{
		g_nFocusStep	= c_nFine3AFStep;
		g_nAFIndex		= 0;
		g_dblMagnifierAF	= g_dblMagnifierSaved;
	}
	else if ( g_nFineGrade == 3 )
	{
		g_nFocusStep	= c_nFine4AFStep;
		g_nAFIndex		= 0;
		g_dblMagnifierAF	= g_dblMagnifierSaved;
	}
	g_nFineGrade ++;									//精选计数值

	///////////////////////////////////////////////////////
	g_nFocusStart	= g_AFTableABP1.nFocus;				//设置初始区间
	g_nFocusStop	= g_AFTableABP2.nFocus;

	///////////////////////////////////////
	for (long i=0; i<g_nAFTableLength_Fine; i++)
	{
		g_AFLookTable_Fine[i].dfSharpness = 0;
		lFinalLens = g_nFocusStart + g_nFocusStep * i;
		
		if (lFinalLens > c_nFocusMAX ) lFinalLens = c_nFocusMAX;
		if (lFinalLens < c_nFocusMIN ) lFinalLens = c_nFocusMIN;
		g_AFLookTable_Fine[i].nFocus = lFinalLens;
	}
	
	if(g_dblMagnifierAF==g_dblMagnifier_Old)			//去除重复扫描情况
	{
		g_AFLookTable_Fine[0].nFocus = g_AFTableABP1.nFocus;
		g_AFLookTable_Fine[0].dfSharpness = g_AFTableABP1.dfSharpness;
//		g_AFLookTable_Fine[g_nAFTableLength_Fine-1].nFocus =g_AFTableABP2.nFocus;
//		g_AFLookTable_Fine[g_nAFTableLength_Fine-1].dfSharpness=g_AFTableABP2.dfSharpness;

		g_nAFIndex = 1;
	}
	lFinalLens		= g_AFLookTable_Fine[g_nAFIndex].nFocus;
	dblMagnifier	= g_dblMagnifierAF;
}

BOOL WINAPI Analyze_Image( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	double&		dfTotal )
{
	BOOL	boolRet;
	double  dflWalsh = 0;

	boolRet = SEM_DIBWalsh1( (LPSTR)lpImageBuff, sizeImage.cx, sizeImage.cy, sizeImage.cx / 4, sizeImage.cy / 4, dfSharpness, dflWalsh );

	double	dfParamX = 0.0;
	double	dfParamY = 0.0;
	boolRet = SEM_Grads( (LPSTR)lpImageBuff, sizeImage.cx, sizeImage.cy, dfParamX, dfParamY, dfTotal );

//	dfTotal = dflWalsh;

	return boolRet;
}

BOOL WINAPI Analyze_Image1( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp)
{
	HINSTANCE m_hDll = LoadLibrary("QXD.dll");
	if(NULL == m_hDll)
	{
		AfxMessageBox("Load dll failed!");
		return FALSE;
	}


	typedef void(*Addqxd)(int&, int&, int*, double&);
	Addqxd m_Add = (Addqxd)GetProcAddress(m_hDll,"QXD");
	if(m_Add == NULL)
	{
		AfxMessageBox("Find Add function failed!");
		return FALSE;
	}
	else
	{
		int Nx = sizeImage.cx, Ny = sizeImage.cy;
		int* GRAY;
		GRAY = (int*)calloc(Nx*Ny, sizeof(int));
		for (int i = 0; i < Nx; i++)
		{
			for (int j = 0; j < Ny; j++)
				{	    
					GRAY[j+i*Nx] = *((unsigned char*)lpImageBuff + Nx * (Ny - 1 - j) + i);                
				}	    
		}
		QXD(Nx,Ny,GRAY,dfSharp);
		FreeLibrary(m_hDll);
			
		return TRUE;
	}
}
BOOL Scan_SearchPeek()
{
	double	dfSharpMax = -1;
	long	i, mMaxIndex = 0;

	BEELINE BaseLine;
	DFPoint AFPoint[c_nAFTableLength];

	for (i=0; i<c_nAFTableLength; i++)
	{
		AFPoint[i].x = (long double)g_AFLookTable[i].nFocus;
		AFPoint[i].y = (long double)g_AFLookTable[i].dfSharpness;
	}

	if ( LineFitting( &BaseLine, AFPoint, c_nAFTableLength ) )
	{
		for (i=0; i<c_nAFTableLength; i++)
		{
			// f(x) = F(x) - { a * x + c }
			g_AFLookTable[i].dfSharpness -= (BaseLine.a * AFPoint[i].x + BaseLine.c);
		}
	}

	mMaxIndex = c_nAFTableLength / 2 - 1;
	for (i=0; i<c_nAFTableLength; i++)
	{
		if( dfSharpMax < g_AFLookTable[ i ].dfSharpness )
		{
			dfSharpMax = g_AFLookTable[ i ].dfSharpness;
			mMaxIndex = i;
		}
	}

	if ( mMaxIndex != 0 && mMaxIndex != c_nAFTableLength-1 )
	{
		g_nAFIndex = mMaxIndex;
		
		return TRUE;
	}

	return FALSE;
}

BOOL Scan_SearchPeekN()
{
	double	dfSharpMax = -1;
	long	i, mMaxIndex = 0;
	BEELINE BaseLine;
	DFPoint AFPoint[c_nAFTableLength1];
	for (i=0; i<c_nAFTableLength1; i++)
	{
		AFPoint[i].x = (long double)g_AFLookTablehy[i].nFocus;
		AFPoint[i].y = (long double)g_AFLookTablehy[i].dfSharpness;
	}
	if ( LineFitting( &BaseLine, AFPoint, c_nAFTableLength1 ) )
	{
		for (i=0; i<c_nAFTableLength1; i++)
		{
			// f(x) = F(x) - { a * x + c }
			g_AFLookTablehy[i].dfSharpness -= (BaseLine.a * AFPoint[i].x + BaseLine.c);
		}
	}
	mMaxIndex = c_nAFTableLength1 / 2 - 1;
	for (i=0; i<c_nAFTableLength1; i++)
	{
		if( dfSharpMax < g_AFLookTablehy[ i ].dfSharpness )
		{
			dfSharpMax = g_AFLookTablehy[ i ].dfSharpness;
			mMaxIndex = i;
		}
	}
	if ( mMaxIndex != 0 && mMaxIndex != c_nAFTableLength1-1 )
	{
		g_nAFIndex = mMaxIndex;
		
		return TRUE;
	}

	return FALSE;
}


UINT WINAPI AutoFocus_Analyze_Scan( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	double	dfSharpness = 0;
	double	dfTotal = 1;
	BOOL	boolRet;

	///////////////////////////////////////
	dblMagnifier = g_dblMagnifierAF;
	///////////////////////////////////////

	///////////////////////////////////////
	boolRet = Analyze_Image( (LPSTR)lpImageBuff, sizeImage, dfSharpness, dfTotal );

	if ( !boolRet )
		return AutoFocus_Analyze_Failure;

	dfSharp = dfSharpness / dfTotal;
	g_AFLookTable[ g_nAFIndex ].dfSharpness = dfSharp;
	///////////////////////////////////////

	if ( g_nAFIndex	< c_nAFTableLength-1 )
	{
		///////////////////////////////////////
		g_nAFIndex ++;
		lFinalLens = g_AFLookTable[ g_nAFIndex ].nFocus;
		///////////////////////////////////////
		
		return AutoFocus_Analyze_Continue;
	}
	else
	{
		if ( Scan_SearchPeek() )
		{
			lFinalLens = g_AFLookTable[ g_nAFIndex ].nFocus;
			return AutoFocus_Analyze_Completed;
		}
	}

	return AutoFocus_Analyze_Failure;
}

//2022.12.28 HY
UINT WINAPI AutoFocus_Analyze_Scan1( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier,
	long&		lFinalLensold)
{
	double	dfSharpness = 0;
	BOOL	boolRet;
	dblMagnifier = g_dblMagnifierAF;
	boolRet = Analyze_Image1( (LPSTR)lpImageBuff, sizeImage,dfSharp );
	if ( !boolRet )
		return AutoFocus_Analyze_Failure;
	g_AFLookTablehy[ g_nAFIndex ].dfSharpness = dfSharp;
	
 	if(g_nAFIndex < 2)
	{
		if(g_nAFIndex < 1)
		{
			g_AFTableABP1.nFocus = g_AFLookTablehy[ g_nAFIndex ].nFocus;
			g_AFTableABP1.dfSharpness = g_AFLookTablehy[ g_nAFIndex ].dfSharpness;
		}
		else
		{
			g_AFTableABP2.nFocus = g_AFLookTablehy[ g_nAFIndex ].nFocus;
			g_AFTableABP2.dfSharpness = g_AFLookTablehy[ g_nAFIndex ].dfSharpness;
		}
		g_nAFIndex ++;
		lFinalLens = g_AFLookTablehy[ g_nAFIndex ].nFocus;
		return AutoFocus_Analyze_Continue;
	}
	if(bDrection)
	{
		g_AFTableABP1.nFocus = g_AFLookTablehy[ g_nAFIndex ].nFocus;
		g_AFTableABP1.dfSharpness = dfSharp;
	}
	else
	{
		g_AFTableABP3.nFocus = g_AFLookTablehy[ g_nAFIndex ].nFocus;
		g_AFTableABP3.dfSharpness = dfSharp;
	}
	if ( g_nAFIndex	< c_nAFTableLength1  )
	{
		if(((g_AFTableABP2.dfSharpness -g_AFTableABP1.dfSharpness)> c_dbSharpnessAB_Scanhy1)  && ((g_AFTableABP3.dfSharpness - g_AFTableABP2.dfSharpness) > c_dbSharpnessAB_Scanhy1))
		{
			g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
			g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;

			g_AFTableABP2.nFocus = g_AFTableABP3.nFocus;
			g_AFTableABP2.dfSharpness = g_AFTableABP3.dfSharpness;

			lFinalLens = g_AFTableABP2.nFocus + g_nAutoFocusStep;
			g_nAFIndex ++;
			g_AFLookTablehy[ g_nAFIndex ].nFocus = lFinalLens;
			bDrection = FALSE;
			return AutoFocus_Analyze_Continue;
		}
		else if(((g_AFTableABP1.dfSharpness -g_AFTableABP2.dfSharpness)> c_dbSharpnessAB_Scanhy1)  && ((g_AFTableABP2.dfSharpness - g_AFTableABP3.dfSharpness) > c_dbSharpnessAB_Scanhy1))
		{
			g_AFTableABP3.nFocus = g_AFTableABP2.nFocus;
			g_AFTableABP3.dfSharpness = g_AFTableABP2.dfSharpness;

			g_AFTableABP2.nFocus = g_AFTableABP1.nFocus;;
			g_AFTableABP2.dfSharpness =g_AFTableABP1.dfSharpness;

			lFinalLens = g_AFTableABP2.nFocus - g_nAutoFocusStep;
			g_nAFIndex ++;
			g_AFLookTablehy[ g_nAFIndex ].nFocus = lFinalLens;
			bDrection = TRUE;
			return AutoFocus_Analyze_Continue;
		}
		else if(((g_AFTableABP2.dfSharpness -g_AFTableABP1.dfSharpness)> c_dbSharpnessAB_Scanhy1)  && ((g_AFTableABP2.dfSharpness - g_AFTableABP3.dfSharpness) > c_dbSharpnessAB_Scanhy1))
		{
			bDrection = FALSE;
			return AutoFocus_Analyze_Completed;
		}
		else
		{
			if((g_AFTableABP1.dfSharpness -g_AFTableABP2.dfSharpness)> c_dbSharpnessAB_Scanhy1 && (g_AFTableABP1.dfSharpness -g_AFTableABP3.dfSharpness)> c_dbSharpnessAB_Scanhy1)
			{
				g_AFTableABP3.nFocus = g_AFTableABP2.nFocus;
				g_AFTableABP3.dfSharpness = g_AFTableABP2.dfSharpness;

				g_AFTableABP2.nFocus = g_AFTableABP1.nFocus;
				g_AFTableABP2.dfSharpness = g_AFTableABP1.dfSharpness;

				lFinalLens = g_AFTableABP2.nFocus - g_nAutoFocusStep;
				g_nAFIndex ++;
				g_AFLookTablehy[ g_nAFIndex ].nFocus = lFinalLens;
				bDrection = TRUE;
				return AutoFocus_Analyze_Continue;
			}
			else
			{
				g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
				g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;

				g_AFTableABP2.nFocus = g_AFTableABP3.nFocus;
				g_AFTableABP2.dfSharpness = g_AFTableABP3.dfSharpness;

				lFinalLens = g_AFTableABP2.nFocus + g_nAutoFocusStep;
				g_nAFIndex ++;
				g_AFLookTablehy[ g_nAFIndex ].nFocus = lFinalLens;
				bDrection = FALSE;
				return AutoFocus_Analyze_Continue;
			}
		}
	}
	else
	{
		Scan_SearchPeekN();
		if(g_nAFIndex  == c_nAFTableLength1)
		{
			g_nAFIndex = 9;
			g_AFTableABP3.nFocus = g_AFLookTablehy[ g_nAFIndex ].nFocus + g_nAutoFocusStep;
			g_AFTableABP3.dfSharpness = 0;
			g_AFTableABP1.nFocus = g_AFLookTablehy[ g_nAFIndex -1].nFocus;
			g_AFTableABP1.dfSharpness =  g_AFLookTablehy[ g_nAFIndex -1].dfSharpness;

		}	
		else if(g_nAFIndex  == 0)
		{
			g_AFTableABP1.nFocus = g_AFLookTablehy[ g_nAFIndex ].nFocus - g_nAutoFocusStep;
			g_AFTableABP1.dfSharpness = 0;
			g_AFTableABP3.nFocus = g_AFLookTablehy[ g_nAFIndex +1].nFocus ;
			g_AFTableABP3.dfSharpness = g_AFLookTablehy[ g_nAFIndex +1].dfSharpness;
		}
		else
		{
			g_AFTableABP1.nFocus = g_AFLookTablehy[ g_nAFIndex -1].nFocus ;
			g_AFTableABP1.dfSharpness =  g_AFLookTablehy[ g_nAFIndex -1].dfSharpness;
			g_AFTableABP3.nFocus = g_AFLookTablehy[ g_nAFIndex +1].nFocus;
			g_AFTableABP3.dfSharpness = g_AFLookTablehy[ g_nAFIndex +1].dfSharpness;
		}
		g_AFTableABP2.nFocus = g_AFLookTablehy[ g_nAFIndex].nFocus;
		g_AFTableABP2.dfSharpness = g_AFLookTablehy[ g_nAFIndex ].dfSharpness;
		g_dblMagnifier_Old = dblMagnifier;
		bDrection = FALSE;
		return AutoFocus_Analyze_Completed;
	}
	return AutoFocus_Analyze_Failure;
}
//2022.12.28 HY
UINT WINAPI AutoFocus_Analyze_Scan2( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	double	dfSharpness = 0;
	double	dfTotal = 1;
	BOOL	boolRet;
//	BOOL	boolAFind;									// ZYX 09.04.15

	dblMagnifier = g_dblMagnifierAF;

	boolRet = Analyze_Image( (LPSTR)lpImageBuff, sizeImage,dfSharpness, dfTotal );

	if ( !boolRet )
		return AutoFocus_Analyze_Failure;

	dfSharp = dfSharpness / dfTotal;
	g_AFLookTable[ g_nAFIndex ].dfSharpness = dfSharp;

	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"AutoFocus_Analyze_Scan2 ----- g_nAFIndex = %d, g_AFLookTable[ %d ].dfSharpness = %f \n", g_nAFIndex, g_nAFIndex,dfSharp);
	OutputDebugString(sz);

	//======2009.04.15_自动聚焦算法优化====================
	if(g_nAFIndex < 2)
	{
		if(g_nAFIndex < 1)
		{
			g_AFTableABP1.nFocus = g_AFLookTable[ g_nAFIndex ].nFocus;
			g_AFTableABP1.dfSharpness = g_AFLookTable[ g_nAFIndex ].dfSharpness;
		}
		else
		{
			g_AFTableABP2.nFocus = g_AFLookTable[ g_nAFIndex ].nFocus;
			g_AFTableABP2.dfSharpness = g_AFLookTable[ g_nAFIndex ].dfSharpness;
		}

		g_nAFIndex ++;
		lFinalLens = g_AFLookTable[ g_nAFIndex ].nFocus;

		return AutoFocus_Analyze_Continue;
	}

//		boolAFind=SearchPoint(dfSharp);
	if(g_AFTableABP2.dfSharpness -dfSharp > c_dbSharpnessAB_Scan)	//还需要进一步改进步进条件
	{
		g_AFTableABP2.nFocus = lFinalLens;
		g_AFTableABP2.dfSharpness = dfSharp;

		g_dblMagnifier_Old = dblMagnifier;

		return AutoFocus_Analyze_Completed;
	}

	if ( g_nAFIndex	< c_nAFTableLength -1 )
	{
		g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
		g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;

		g_AFTableABP2.nFocus = g_AFLookTable[ g_nAFIndex ].nFocus;
		g_AFTableABP2.dfSharpness = g_AFLookTable[ g_nAFIndex ].dfSharpness;

		g_nAFIndex ++;
		lFinalLens = g_AFLookTable[ g_nAFIndex ].nFocus;

		return AutoFocus_Analyze_Continue;
	}
	else
	{
		Scan_SearchPeek();
		g_AFTableABP1.nFocus = g_AFLookTable[ g_nAFIndex -1].nFocus;
		g_AFTableABP1.dfSharpness = g_AFLookTable[ g_nAFIndex-1 ].dfSharpness;

		g_AFTableABP2.nFocus = g_AFLookTable[ g_nAFIndex +1].nFocus;
		g_AFTableABP2.dfSharpness = g_AFLookTable[ g_nAFIndex+1 ].dfSharpness;

		g_dblMagnifier_Old = dblMagnifier;

		return AutoFocus_Analyze_Completed;
	}

	//======2009.04.15_自动聚焦算法优化====================

	return AutoFocus_Analyze_Failure;
}



UINT WINAPI AutoFocus_Analyze_Scan3( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	double	dfSharpness = 0;
	double	dfTotal = 1;
	BOOL	boolRet;
	//	BOOL	boolAFind;									// ZYX 09.04.15

	dblMagnifier = g_dblMagnifierAF;

	boolRet = Analyze_Image( (LPSTR)lpImageBuff, sizeImage,dfSharpness, dfTotal );

	if ( !boolRet )
		return AutoFocus_Analyze_Failure;

	dfSharp = dfSharpness / dfTotal;
	g_AFLookTable[ g_nAFIndex ].dfSharpness = dfSharp;

	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"AutoFocus_Analyze_Scan2 ----- g_nAFIndex = %d, g_AFLookTable[ %d ].dfSharpness = %f \n", g_nAFIndex, g_nAFIndex,dfSharp);
	OutputDebugString(sz);

	//======2009.04.15_自动聚焦算法优化====================
	if(g_nAFIndex < 2)
	{
		if(g_nAFIndex < 1)
		{
			g_AFTableABP1.nFocus = g_AFLookTable[ g_nAFIndex ].nFocus;
			g_AFTableABP1.dfSharpness = g_AFLookTable[ g_nAFIndex ].dfSharpness;
		}
		else
		{
			g_AFTableABP2.nFocus = g_AFLookTable[ g_nAFIndex ].nFocus;
			g_AFTableABP2.dfSharpness = g_AFLookTable[ g_nAFIndex ].dfSharpness;
		}

		g_nAFIndex ++;
		lFinalLens = g_AFLookTable[ g_nAFIndex ].nFocus;

		return AutoFocus_Analyze_Continue;
	}

	//		boolAFind=SearchPoint(dfSharp);
	if(g_AFTableABP2.dfSharpness -dfSharp > c_dbSharpnessAB_Scan)	//还需要进一步改进步进条件
	{
		g_AFTableABP2.nFocus = lFinalLens;
		g_AFTableABP2.dfSharpness = dfSharp;

		g_dblMagnifier_Old = dblMagnifier;

		return AutoFocus_Analyze_Completed;
	}

	if ( g_nAFIndex	< c_nAFTableLength -1 )
	{
		g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
		g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;

		g_AFTableABP2.nFocus = g_AFLookTable[ g_nAFIndex ].nFocus;
		g_AFTableABP2.dfSharpness = g_AFLookTable[ g_nAFIndex ].dfSharpness;

		g_nAFIndex ++;
		lFinalLens = g_AFLookTable[ g_nAFIndex ].nFocus;

		return AutoFocus_Analyze_Continue;
	}
	else
	{
		Scan_SearchPeek();
		g_AFTableABP1.nFocus = g_AFLookTable[ g_nAFIndex -1].nFocus;
		g_AFTableABP1.dfSharpness = g_AFLookTable[ g_nAFIndex-1 ].dfSharpness;

		g_AFTableABP2.nFocus = g_AFLookTable[ g_nAFIndex +1].nFocus;
		g_AFTableABP2.dfSharpness = g_AFLookTable[ g_nAFIndex+1 ].dfSharpness;

		g_dblMagnifier_Old = dblMagnifier;

		return AutoFocus_Analyze_Completed;
	}

	//======2009.04.15_自动聚焦算法优化====================

	return AutoFocus_Analyze_Failure;
}


BOOL Fine_SearchPeek()
{
	double	dfSharM, dfSharTemp, dfSharMax, dfSharSum;
	long	nAFIndexMax = 0;
	BOOL	boolAF_OK = FALSE;

	if ( g_nAFIndex	== g_nAFTableLength_Fine - 1 )
	{
		dfSharMax	= 0;
		nAFIndexMax = g_nAFTableLength_Fine / 2 - 1;

		for ( int i=0; i<g_nAFTableLength_Fine; i++ )
		{
			dfSharTemp = g_AFLookTable_Fine[ i ].dfSharpness;

			if ( dfSharMax < dfSharTemp )
			{
				dfSharMax = dfSharTemp;
				nAFIndexMax = i;
			}
		}

		if ( 0 < nAFIndexMax  &&  nAFIndexMax < g_nAFTableLength_Fine - 1 )
		{
			g_nAFIndex	= nAFIndexMax;
			boolAF_OK	= TRUE;
		}
	}

	if ( !boolAF_OK && g_nAFIndex >= 2 )
	{
		dfSharMax	= 0;
		for ( int i=0; i<3; i++ )
		{
			dfSharTemp = g_AFLookTable_Fine[ g_nAFIndex - i ].dfSharpness;

			if ( dfSharMax < dfSharTemp )
				dfSharMax = dfSharTemp;
		}

		dfSharM		= g_AFLookTable_Fine[ g_nAFIndex - 1 ].dfSharpness;

		////////////////////////////////////////////////////////
		dfSharSum	= 0;
		dfSharTemp  = g_AFLookTable_Fine[ g_nAFIndex - 0 ].dfSharpness;
		dfSharSum	+= dfSharTemp;
		dfSharTemp  = g_AFLookTable_Fine[ g_nAFIndex - 2 ].dfSharpness;
		dfSharSum	+= dfSharTemp;
		////////////////////////////////////////////////////////
		
		double	dfDAF = 100 * (dfSharMax*2 - dfSharSum) / dfSharSum;

		if ( ( dfSharM == dfSharMax ) && ( dfDAF > 50 ) )
		{
			g_nAFIndex -= 1;
			boolAF_OK = TRUE;
		}
	}
	
	if ( !boolAF_OK && ( g_nAFIndex	>= 4 ) )
	{
		dfSharMax	= 0;
		for ( int i=0; i<5; i++ )
		{
			dfSharTemp = g_AFLookTable_Fine[ g_nAFIndex - i ].dfSharpness;

			if ( dfSharMax < dfSharTemp )
				dfSharMax = dfSharTemp;
		}

		dfSharM		= g_AFLookTable_Fine[ g_nAFIndex - 2 ].dfSharpness;

		////////////////////////////////////////////////////////
		dfSharSum	= 0;
		dfSharTemp  = g_AFLookTable_Fine[ g_nAFIndex - 0 ].dfSharpness;
		dfSharSum	+= dfSharTemp;
		dfSharTemp  = g_AFLookTable_Fine[ g_nAFIndex - 4 ].dfSharpness;
		dfSharSum	+= dfSharTemp;
		////////////////////////////////////////////////////////
		
		double	dfDAF = 100 * (dfSharMax*2 - dfSharSum) / dfSharSum;

		if ( ( dfSharM == dfSharMax ) && ( dfDAF > 20 ) )
		{
			g_nAFIndex -= 2;
			boolAF_OK = TRUE;
		}
	}

	return boolAF_OK;
}

BOOL Fine_SearchPeek2()
{
	double	dfSharTemp, dfSharMax;
	long	nAFIndexMax = 0;
	BOOL	boolAF_OK = FALSE;

//	if ( g_nAFIndex	== g_nAFTableLength_Fine - 1 )		// ZYX 09.04.15屏蔽
	{
		dfSharMax	= 0;
		nAFIndexMax = g_nAFTableLength_Fine / 2 - 1;

		for ( int i=0; i<g_nAFTableLength_Fine; i++ )
		{
			dfSharTemp = g_AFLookTable_Fine[ i ].dfSharpness;

			if ( dfSharMax < dfSharTemp )
			{
				dfSharMax = dfSharTemp;
				nAFIndexMax = i;
			}
		}

//		if ( ( 0 < nAFIndexMax ) && ( nAFIndexMax < g_nAFTableLength_Fine - 1 ) )
		{
			// ZYX 09.04.15
//			g_nAFIndex	= nAFIndexMax;
			g_nAFIndexMax	= nAFIndexMax;
			// ZYX 09.04.15
			boolAF_OK	= TRUE;
		}
	}
	return boolAF_OK;
}

UINT WINAPI AutoFocus_Analyze_Fine( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	double	dfSharpness = 0;
	double	dfTotal = 1;
	BOOL	boolRet;
	BOOL	boolAF_OK = FALSE;

	///////////////////////////////////////
	dblMagnifier = g_dblMagnifierAF;
	///////////////////////////////////////

	///////////////////////////////////////
	boolRet = Analyze_Image( (LPSTR)lpImageBuff, sizeImage, dfSharpness, dfTotal );

	if ( !boolRet )
		return AutoFocus_Analyze_Failure;

	dfSharp = dfSharpness / dfTotal;
	g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness = dfSharp;
	///////////////////////////////////////

	boolAF_OK = Fine_SearchPeek( );

	if ( boolAF_OK )
	{
		if ( g_nFineGrade < c_nFineGradeMax )
		{
			AutoFocus_Fine_Initialize( lFinalLens, dblMagnifier );
			return AutoFocus_Analyze_Continue;
		}
		else
		{
			lFinalLens = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;

			// Test 调试信息
			////////////////////////////////////////////////////////////////////
// 			CString		csMsg;
// 			csMsg.Format("AF-Max, F = %5.1f, Sharp = %5.1f", lFinalLens/OL_TO_LED, dfSharMax*1000 );
// 			theApp.DisplayMsg( csMsg );
			////////////////////////////////////////////////////////////////////

			return AutoFocus_Analyze_Completed;
		}
	}
	else if ( g_nAFIndex	< g_nAFTableLength_Fine - 1 )
	{
		///////////////////////////////////////
		g_nAFIndex ++;
		lFinalLens = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
		///////////////////////////////////////
		
		return AutoFocus_Analyze_Continue;
	}

	return AutoFocus_Analyze_Failure;
}

UINT WINAPI AutoFocus_Analyze_Fine1( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier)
{

	BOOL	boolAF_OK = FALSE;	
	double	dfSharpness = 0;
	BOOL	boolRet;
	dblMagnifier = g_dblMagnifierAF;
	boolRet = Analyze_Image1( (LPSTR)lpImageBuff, sizeImage,dfSharp );

	if ( !boolRet )
		return AutoFocus_Analyze_Failure;

	g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness = dfSharp;
	
	if(g_nAFIndex < 2)
	{
		if(g_nAFIndex < 1)
		{
			g_AFTableABP1.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
			g_AFTableABP1.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;
		}
		else
		{
			g_AFTableABP2.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
			g_AFTableABP2.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;
		}
		g_nAFIndex ++;
		lFinalLens = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
		return AutoFocus_Analyze_Continue;
	}
	if(bDrection)
	{
		g_AFTableABP1.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
		g_AFTableABP1.dfSharpness = dfSharp;
	}
	else
	{
		g_AFTableABP3.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
		g_AFTableABP3.dfSharpness = dfSharp;
	}
	if ( g_nAFIndex	< g_nAFTableLength_Fine -1 )
	{
		if((g_AFTableABP2.dfSharpness - g_AFTableABP1.dfSharpness)> c_dbSharpnessAB_Finehy && (g_AFTableABP3.dfSharpness - g_AFTableABP2.dfSharpness)> c_dbSharpnessAB_Finehy)
		{
			g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
			g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;

			g_AFTableABP2.nFocus = g_AFTableABP3.nFocus;
			g_AFTableABP2.dfSharpness = g_AFTableABP3.dfSharpness;

			lFinalLens = g_AFTableABP2.nFocus + g_nFocusFine1Step;
			g_nAFIndex ++;
			g_AFLookTable_Fine[ g_nAFIndex ].nFocus = lFinalLens;
			bDrection = FALSE;
			return AutoFocus_Analyze_Continue;
		}

		else if((g_AFTableABP1.dfSharpness - g_AFTableABP2.dfSharpness)> c_dbSharpnessAB_Finehy && (g_AFTableABP2.dfSharpness - g_AFTableABP3.dfSharpness)> c_dbSharpnessAB_Finehy)
		{
			g_AFTableABP3.nFocus = g_AFTableABP2.nFocus;
			g_AFTableABP3.dfSharpness = g_AFTableABP2.dfSharpness;

			g_AFTableABP2.nFocus = g_AFTableABP1.nFocus;;
			g_AFTableABP2.dfSharpness =g_AFTableABP1.dfSharpness;

			lFinalLens =g_AFTableABP2.nFocus - g_nFocusFine1Step;
			g_nAFIndex ++;
			g_AFLookTable_Fine[ g_nAFIndex ].nFocus = lFinalLens;
			bDrection = TRUE;
			return AutoFocus_Analyze_Continue;
		}
		else if((g_AFTableABP2.dfSharpness - g_AFTableABP1.dfSharpness)> c_dbSharpnessAB_Finehy && (g_AFTableABP2.dfSharpness - g_AFTableABP3.dfSharpness)> c_dbSharpnessAB_Finehy)
		{
			if ( g_nFocusFine1Step > g_nFocusFine2Step)
			{
				AutoFocus_Fine_Initialize1( lFinalLens, dblMagnifier );
				bDrection = FALSE;
				return AutoFocus_Analyze_Continue;
			}
			bDrection = FALSE;
			lFinalLens = g_AFTableABP2.nFocus;
			return AutoFocus_Analyze_Completed;
		}
		else
		{
			if((g_AFTableABP1.dfSharpness - g_AFTableABP2.dfSharpness)> c_dbSharpnessAB_Finehy && (g_AFTableABP1.dfSharpness - g_AFTableABP3.dfSharpness)> c_dbSharpnessAB_Finehy)
			{
				g_AFTableABP3.nFocus = g_AFTableABP2.nFocus;
				g_AFTableABP3.dfSharpness = g_AFTableABP2.dfSharpness;

				g_AFTableABP2.nFocus = g_AFTableABP1.nFocus;
				g_AFTableABP2.dfSharpness = g_AFTableABP1.dfSharpness;

				lFinalLens = g_AFTableABP2.nFocus - g_nFocusFine1Step;
				g_nAFIndex ++;
				g_AFLookTable_Fine[ g_nAFIndex ].nFocus = lFinalLens;
				bDrection = TRUE;
				return AutoFocus_Analyze_Continue;
			}
			else 
			{
				g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
				g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;

				g_AFTableABP2.nFocus = g_AFTableABP3.nFocus;
				g_AFTableABP2.dfSharpness = g_AFTableABP3.dfSharpness;

				lFinalLens = g_AFTableABP2.nFocus + g_nFocusFine1Step;
				g_nAFIndex ++;
				g_AFLookTable_Fine[ g_nAFIndex ].nFocus = lFinalLens;
				bDrection = FALSE;
				return AutoFocus_Analyze_Continue;
			}
			
		}
	}
	else
	{
		if ( g_nFocusFine1Step > g_nFocusFine2Step)
		{
			Fine_SearchPeek2();
			g_AFTableABP1.nFocus = g_AFLookTable_Fine[ g_nAFIndex -1].nFocus;
			g_AFTableABP1.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex-1 ].dfSharpness;

			g_AFTableABP2.nFocus = g_AFLookTable_Fine[ g_nAFIndex].nFocus;
			g_AFTableABP2.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;

			g_AFTableABP3.nFocus = g_AFLookTable_Fine[ g_nAFIndex +1].nFocus;
			g_AFTableABP3.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex+1 ].dfSharpness;

			g_dblMagnifier_Old = dblMagnifier;
			AutoFocus_Fine_Initialize1( lFinalLens, dblMagnifier );
			bDrection = FALSE;
			return AutoFocus_Analyze_Continue;

		}
		else
		{
			Fine_SearchPeek2();
			lFinalLens = g_AFLookTable_Fine[ g_nAFIndexMax ].nFocus;
			bDrection = FALSE;
			return AutoFocus_Analyze_Completed;		
		}
	}
	

	return AutoFocus_Analyze_Failure;
}
UINT WINAPI AutoFocus_Analyze_Fine2( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	double	dfSharpness = 0;
	double	dfTotal = 1;
	BOOL	boolRet;
	BOOL	boolAF_OK = FALSE;
//	BOOL	boolAFind = FALSE;							// ZYX 09.04.15

	///////////////////////////////////////
	dblMagnifier = g_dblMagnifierAF;
	///////////////////////////////////////

	///////////////////////////////////////
	boolRet = Analyze_Image( (LPSTR)lpImageBuff, sizeImage, dfSharpness, dfTotal );

	if ( !boolRet )
		return AutoFocus_Analyze_Failure;

	dfSharp = dfSharpness / dfTotal;
	g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness = dfSharp;
	///////////////////////////////////////

	//======2009.04.15_自动聚焦算法优化====================
//	boolAFind=SearchPoint(dfSharp);

	if(g_nAFIndex < 2)
	{
		if(g_nAFIndex < 1)
		{
			g_AFTableABP1.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
			g_AFTableABP1.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;
		}
		else
		{
			g_AFTableABP2.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
			g_AFTableABP2.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;
		}

		g_nAFIndex ++;
		lFinalLens = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;

		return AutoFocus_Analyze_Continue;
	}
	else
	{
		if ( g_AFTableABP2.dfSharpness -dfSharp > c_dbSharpnessAB_Fine)	//已经选好区间
		{
			if ( g_nFineGrade < g_nFineGradeMax )		//进入下一轮细选
			{
				g_AFTableABP2.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
				g_AFTableABP2.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;

				g_dblMagnifier_Old = dblMagnifier;
	
				AutoFocus_Fine_Initialize2( lFinalLens, dblMagnifier );
			
				return AutoFocus_Analyze_Continue;
			}
			else										//选择完毕
			{
				lFinalLens = g_AFLookTable_Fine[ g_nAFIndex-1 ].nFocus;
				return AutoFocus_Analyze_Completed;
			}
		}
		else											//未选好区间
		{
			if ( g_nAFIndex	< g_nAFTableLength_Fine - 1 )//没有全程搜索完毕
			{
				g_AFTableABP1.nFocus = g_AFTableABP2.nFocus;
				g_AFTableABP1.dfSharpness = g_AFTableABP2.dfSharpness;
				
				g_AFTableABP2.nFocus = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
				g_AFTableABP2.dfSharpness = g_AFLookTable_Fine[ g_nAFIndex ].dfSharpness;
				///////////////////////////////////////
				g_nAFIndex ++;
				lFinalLens = g_AFLookTable_Fine[ g_nAFIndex ].nFocus;
				///////////////////////////////////////
		
				return AutoFocus_Analyze_Continue;
			}
			else										//遍历所有点
			{
				if ( g_nFineGrade < g_nFineGradeMax )
				{
					Fine_SearchPeek2();
					g_AFTableABP1.nFocus = g_AFLookTable_Fine[ g_nAFIndexMax -1].nFocus;
					g_AFTableABP1.dfSharpness = g_AFLookTable_Fine[ g_nAFIndexMax-1 ].dfSharpness;
	
					g_AFTableABP2.nFocus = g_AFLookTable_Fine[ g_nAFIndexMax +1].nFocus;
					g_AFTableABP2.dfSharpness = g_AFLookTable_Fine[ g_nAFIndexMax+1 ].dfSharpness;

					g_dblMagnifier_Old = dblMagnifier;
		
					AutoFocus_Fine_Initialize2( lFinalLens, dblMagnifier );
				
					return AutoFocus_Analyze_Continue;
				}
				else
				{
					Fine_SearchPeek2();
					lFinalLens = g_AFLookTable_Fine[ g_nAFIndexMax ].nFocus;
					return AutoFocus_Analyze_Completed;
				}
			}
		}
	}
	//======2009.04.15_自动聚焦算法优化====================

	return AutoFocus_Analyze_Failure;
}

UINT WINAPI AutoFocus_Analyze_Image( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	UINT nRet = AutoFocus_Analyze_Failure;

	if ( !g_boolFineAF )
	{
		nRet = AutoFocus_Analyze_Scan
			( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );

		if ( nRet == AutoFocus_Analyze_Completed )
		{
			g_boolFineAF = TRUE;
			AutoFocus_Fine_Initialize( lFinalLens, dblMagnifier );
			nRet = AutoFocus_Analyze_Continue;
		}
	}
	else
	{
		nRet = AutoFocus_Analyze_Fine( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );
	}
	return nRet;
}

//HY 2022.12.28 
UINT WINAPI AutoFocus_Analyze_Image1( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier,
    long&		lFinalLensold )
{
	UINT nRet = AutoFocus_Analyze_Failure;
	
	if ( !g_boolFineAF )
	{
		nRet = AutoFocus_Analyze_Scan1( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier, lFinalLensold );
		
		if ( nRet == AutoFocus_Analyze_Completed )
		{
			g_boolFineAF = TRUE;
			AutoFocus_Fine_Initialize1( lFinalLens, dblMagnifier );
			nRet = AutoFocus_Analyze_Continue;
		}
	}
	else 
	{
		nRet = AutoFocus_Analyze_Fine1( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );
	}
	return nRet;
}
//HY 2022.12.28
UINT WINAPI AutoFocus_Analyze_Image2( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	UINT nRet = AutoFocus_Analyze_Failure;
	
	if ( !g_boolFineAF )
	{
		nRet = AutoFocus_Analyze_Scan2( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );
		
		if ( nRet == AutoFocus_Analyze_Completed )
		{
			g_boolFineAF = TRUE;
			AutoFocus_Fine_Initialize2( lFinalLens, dblMagnifier );
			nRet = AutoFocus_Analyze_Continue;
		}
	}
	else
	{
		nRet = AutoFocus_Analyze_Fine2( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );
	}
	return nRet;
}


UINT WINAPI AutoFocus_Analyze_Image3( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharp,
	long& 		lFinalLens,
	double&		dblMagnifier )
{
	UINT nRet = AutoFocus_Analyze_Failure;

	if ( !g_boolFineAF )
	{
		nRet = AutoFocus_Analyze_Scan2( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );

		if ( nRet == AutoFocus_Analyze_Completed )
		{
			g_boolFineAF = TRUE;
			AutoFocus_Fine_Initialize2( lFinalLens, dblMagnifier );
			nRet = AutoFocus_Analyze_Continue;
		}
	}
	else
	{
		nRet = AutoFocus_Analyze_Fine2( lpImageBuff, sizeImage, dfSharp, lFinalLens, dblMagnifier );
	}
	return nRet;
}
