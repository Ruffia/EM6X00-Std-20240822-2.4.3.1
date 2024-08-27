// Algorithm_AutoAstigmat.cpp : implementation file
//
// Last modify : 2008.12.17

#include "stdafx.h"
#include "Algorithm_AutoAstigmat.h"
#include "FreTrans.h"

//////////////////////////////////////////////////////////////////
// fine focusing
//////////////////////////////////////////////////////////////////
typedef struct tagAF_Table{
	long	nFocus;
	double	dfSharpness;
}AF_Table;

#define			c_nAFTableLength_Fine  12
AF_Table		g_AFLookTable_Fine[ c_nAFTableLength_Fine ];

const long		c_nFocusMAX	=	32767;//65535;
const long		c_nFocusMIN	=	0;

const long		c_nFine4AFStep  =      4;
const long		c_nFine4AFStart  = - c_nAFTableLength_Fine * c_nFine4AFStep / 2;
const long		c_nFine4AFStop   = + c_nAFTableLength_Fine * c_nFine4AFStep / 2;

long	g_nFocusStart;
long	g_nFocusStop;
long	g_nFocusStep;
long	g_nAFIndex;

//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Coarse astigmating
//////////////////////////////////////////////////////////////////

typedef struct tagAS_Table{
	long	nStX;
	long	nStY;
	double	dfSharpness;
}AS_Table;

#define			c_nASTableLength  16
AS_Table		g_ASLookTable[ c_nASTableLength ];

const long		c_nStigmatorMAX		=	4095;	//255;
const long		c_nStigmatorMIN		=	0;		//0;
const long		c_nAS_CoarseStep	=	1024;	//64;
const long		c_nAS_FineStep		=	256;	//16;
const long		c_nAS_Fine2Step		=	64;		//4;
const long		c_nAS_Fine3Step		=	8;		//1;
// 2008.12.17中间变换值
const long		c_nStigmatorCValue	=	2047;	//127;
// 2008.12.17

long			g_nAS_Milestone		=	0;
long			g_nAS_Index			=	0;

//////////////////////////////////////////////////////////////////


void AutoFocus_Fine_Initialize( long& lFinalLens )
{
	///////////////////////////////////////
	g_nFocusStart	= lFinalLens + c_nFine4AFStart;
	g_nFocusStop	= lFinalLens + c_nFine4AFStop;
	g_nFocusStep	= c_nFine4AFStep;
	g_nAFIndex		= 0;
	///////////////////////////////////////

	///////////////////////////////////////
	for (long i=0; i<c_nAFTableLength_Fine; i++)
	{
		g_AFLookTable_Fine[i].dfSharpness = 0;
		lFinalLens = g_nFocusStart + g_nFocusStep * i;
		
		if (lFinalLens > c_nFocusMAX ) lFinalLens = c_nFocusMAX;
		if (lFinalLens < c_nFocusMIN ) lFinalLens = c_nFocusMIN;
		g_AFLookTable_Fine[i].nFocus = lFinalLens;
	}

	lFinalLens	= g_AFLookTable_Fine[0].nFocus;
}

/////////////////////////////////////////////////////////////////////////////

void AutoAstigmat_Coarse_Initialize(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	int		i, j, l;
	int		nX0, nY0;

	///////////////////////////////////////////////////////////
	l	= 0;
	nX0	= c_nAS_CoarseStep / 2 -1;
	nY0 = c_nAS_CoarseStep / 2 -1;
	///////////////////////////////////////////////////////////
	for (i=0, j=0; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_CoarseStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_CoarseStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=1; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_CoarseStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_CoarseStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=0, j=2; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_CoarseStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_CoarseStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=3; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_CoarseStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_CoarseStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// 2008.12.17
	// 由于g_ASLookTable[0-15]的值均在0-255范围
	// 而实际向下位机所送之值在SemCore中重复做了变换
	// 因此在此处做调整
	///////////////////////////////////////////////////////////
	for( i=0; i<16; i++ )
	{
		g_ASLookTable[ i ].nStX -= c_nStigmatorCValue;
		g_ASLookTable[ i ].nStY -= c_nStigmatorCValue;
	}
	// 2008.12.17
	///////////////////////////////////////////////////////////

	g_nAS_Milestone		= 0;
	g_nAS_Index			= 0;

	lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
	lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
}

void AutoAstigmat_Fine_Initialize(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	int		i, j, l;
	int		nX0, nY0;

	///////////////////////////////////////////////////////////
	l	= 0;
	nX0	= c_nAS_FineStep / 2 -1;
	nY0 = c_nAS_FineStep / 2 -1;
	// 2008.12.17
//	nX0	+= lStigmatorX - c_nAS_FineStep * 2;
//	nY0 += lStigmatorY - c_nAS_FineStep * 2;
	nX0	+= lStigmatorX + c_nStigmatorCValue - c_nAS_FineStep * 2;
	nY0 += lStigmatorY + c_nStigmatorCValue - c_nAS_FineStep * 2;
	///////////////////////////////////////////////////////////
	for (i=0, j=0; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_FineStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_FineStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=1; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_FineStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_FineStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=0, j=2; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_FineStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_FineStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=3; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_FineStep * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_FineStep * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// 2008.12.17
	// 由于g_ASLookTable[0-15]的值均在0-255范围
	// 而实际向下位机所送之值在SemCore中重复做了变换
	// 因此在此处做调整
	///////////////////////////////////////////////////////////
	for( i=0; i<16; i++ )
	{
		g_ASLookTable[ i ].nStX -= c_nStigmatorCValue;
		g_ASLookTable[ i ].nStY -= c_nStigmatorCValue;
	}
	// 2008.12.17
	///////////////////////////////////////////////////////////

	g_nAS_Milestone		= 1;
	g_nAS_Index			= 0;

	lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
	lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
}

void AutoAstigmat_Fine2_Initialize(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	int		i, j, l;
	int		nX0, nY0;

	///////////////////////////////////////////////////////////
	l	= 0;
	nX0	= c_nAS_Fine2Step / 2 -1;
	nY0 = c_nAS_Fine2Step / 2 -1;
	// 2008.12.17
//	nX0	+= lStigmatorX - c_nAS_Fine2Step * 2;
//	nY0 += lStigmatorY - c_nAS_Fine2Step * 2;
	nX0	+= lStigmatorX + c_nStigmatorCValue - c_nAS_Fine2Step * 2;
	nY0 += lStigmatorY + c_nStigmatorCValue - c_nAS_Fine2Step * 2;
	///////////////////////////////////////////////////////////
	for (i=0, j=0; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine2Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine2Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=1; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine2Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine2Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=0, j=2; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine2Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine2Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=3; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine2Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine2Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// 2008.12.17
	// 由于g_ASLookTable[0-15]的值均在0-255范围
	// 而实际向下位机所送之值在SemCore中重复做了变换
	// 因此在此处做调整
	///////////////////////////////////////////////////////////
	for( i=0; i<16; i++ )
	{
		g_ASLookTable[ i ].nStX -= c_nStigmatorCValue;
		g_ASLookTable[ i ].nStY -= c_nStigmatorCValue;
	}
	// 2008.12.17
	///////////////////////////////////////////////////////////

	g_nAS_Milestone		= 2;
	g_nAS_Index			= 0;

	lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
	lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
}

void AutoAstigmat_Fine3_Initialize(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	int		i, j, l;
	int		nX0, nY0;

	///////////////////////////////////////////////////////////
	l	= 0;
	// 2008.12.17
//	nX0	= lStigmatorX - c_nAS_Fine3Step * 2;
//	nY0 = lStigmatorY - c_nAS_Fine3Step * 2;
	nX0	= lStigmatorX + c_nStigmatorCValue - c_nAS_Fine3Step * 2;
	nY0 = lStigmatorY + c_nStigmatorCValue - c_nAS_Fine3Step * 2;
	///////////////////////////////////////////////////////////
	for (i=0, j=0; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine3Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine3Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=1; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine3Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine3Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=0, j=2; i<=3; i++)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine3Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine3Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	for (i=3, j=3; i>=0; i--)
	{
		g_ASLookTable[ l ].nStX = nX0 + c_nAS_Fine3Step * i;
		g_ASLookTable[ l ].nStY = nY0 + c_nAS_Fine3Step * j;
		g_ASLookTable[ l++ ].dfSharpness = 0;
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// 2008.12.17
	// 由于g_ASLookTable[0-15]的值均在0-255范围
	// 而实际向下位机所送之值在SemCore中重复做了变换
	// 因此在此处做调整
	///////////////////////////////////////////////////////////
	for( i=0; i<16; i++ )
	{
		g_ASLookTable[ i ].nStX -= c_nStigmatorCValue;
		g_ASLookTable[ i ].nStY -= c_nStigmatorCValue;
	}
	// 2008.12.17
	///////////////////////////////////////////////////////////

	g_nAS_Milestone		= 3;
	g_nAS_Index			= 0;

	lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
	lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
}

void WINAPI AutoAstigmat_Initialize(
	long& 		lFinalLens,
	double&		dblMagnifier,
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	AutoAstigmat_Coarse_Initialize(lStigmatorX, lStigmatorY);

	return;
}


BOOL Image_Sharpness( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness )
{
	BOOL		boolRet;
	double		dfParamX	= 0;
	double		dfParamY	= 0;
	double		dfTotal		= 1;
	double		dfWalsh		= 0;

	///////////////////////////////////////
	boolRet = SEM_DIBWalsh1( (LPSTR)lpImageBuff, sizeImage.cx, sizeImage.cy,
		sizeImage.cx / 4, sizeImage.cy / 4,	dfSharpness, dfWalsh );

	///////////////////////////////////////
	boolRet = SEM_Grads( (LPSTR)lpImageBuff, sizeImage.cx, sizeImage.cy,
		dfParamX, dfParamY, dfTotal );

	dfSharpness = dfSharpness / dfTotal;
	
	return boolRet;
}

BOOL Max_Sharp(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	int		i, nMax;
	double	dfSharp, dfSharpMax;

	dfSharpMax = -1;
	nMax = c_nASTableLength / 2 - 1;
	///////////////////////////////////////////////////
	for (i=0; i<c_nASTableLength; i++)
	{
		dfSharp = g_ASLookTable[ i ].dfSharpness;
		if (dfSharpMax < dfSharp)
		{
			dfSharpMax	= dfSharp;
			nMax		= i;
		}
	}
	///////////////////////////////////////////////////
	
	lStigmatorX = g_ASLookTable[ nMax ].nStX;
	lStigmatorY = g_ASLookTable[ nMax ].nStY;

	return TRUE;
}

BOOL ExpectationXY_P2(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	int		i;
	double	dfP2, dfP2Sum = 0;
	double	dfExpectationX = 0;
	double	dfExpectationY = 0;

	///////////////////////////////////////////////////
	for (i=0; i<c_nASTableLength; i++)
	{
		dfP2	 = g_ASLookTable[ i ].dfSharpness;
		dfP2	*= dfP2;
		dfP2Sum += dfP2;

		g_ASLookTable[ i ].dfSharpness = dfP2;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	for (i=0; i<c_nASTableLength; i++)
	{
		dfP2 = g_ASLookTable[ i ].dfSharpness;
		dfExpectationX += g_ASLookTable[ i ].nStX * dfP2;
	}
	dfExpectationX /= dfP2Sum;
	///////////////////////////////////////////////////
	
	///////////////////////////////////////////////////
	for (i=0; i<c_nASTableLength; i++)
	{
		dfP2 = g_ASLookTable[ i ].dfSharpness;
		dfExpectationY += g_ASLookTable[ i ].nStY * dfP2;
	}
	dfExpectationY /= dfP2Sum;
	///////////////////////////////////////////////////

	lStigmatorX = (long)dfExpectationX;
	lStigmatorY = (long)dfExpectationY;

	return TRUE;
}

BOOL CoarseStigmator(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	//return ExpectationXY_P2(lStigmatorX, lStigmatorY);

	return Max_Sharp(lStigmatorX, lStigmatorY);
}

BOOL FineStigmator(
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	//return ExpectationXY_P2(lStigmatorX, lStigmatorY);

	return Max_Sharp(lStigmatorX, lStigmatorY);
}

UINT WINAPI AutoAstigmat_Analyze_Image( 
	LPVOID		lpImageBuff,
	CSize		sizeImage,
	double&		dfSharpness,
	long& 		lFinalLens,
	double&		dblMagnifier,
	long&		lStigmatorX,
	long&		lStigmatorY )
{
	UINT nRet = AutoAstigmat_Analyze_Failure;

	if ( ! Image_Sharpness( lpImageBuff, sizeImage, dfSharpness ) )
		return nRet;
	g_ASLookTable[ g_nAS_Index ].dfSharpness = dfSharpness;

	if ( g_nAS_Milestone == 0 )		// Coarse
	{
		nRet = AutoAstigmat_Analyze_Continue;

		g_nAS_Index ++;
		if ( g_nAS_Index < c_nASTableLength	)
		{
			lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
			lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
		}
		else
		{
			CoarseStigmator( lStigmatorX, lStigmatorY );
			AutoAstigmat_Fine_Initialize( lStigmatorX, lStigmatorY );
		}
	}
	// 2008.12.17
//	if ( g_nAS_Milestone == 1 )		// Fine
	else if ( g_nAS_Milestone == 1 )		// Fine
	// 2008.12.17
	{
		nRet = AutoAstigmat_Analyze_Continue;

		g_nAS_Index ++;
		if ( g_nAS_Index < c_nASTableLength	)
		{
			lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
			lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
		}
		else
		{
			FineStigmator( lStigmatorX, lStigmatorY );
			AutoAstigmat_Fine2_Initialize( lStigmatorX, lStigmatorY );
		}
	}
	else if ( g_nAS_Milestone == 2 )		// Fine2
	{
		nRet = AutoAstigmat_Analyze_Continue;

		g_nAS_Index ++;
		if ( g_nAS_Index < c_nASTableLength	)
		{
			lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
			lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
		}
		else
		{
			FineStigmator( lStigmatorX, lStigmatorY );
			//nRet = AutoAstigmat_Analyze_Completed;
			AutoAstigmat_Fine3_Initialize( lStigmatorX, lStigmatorY );
		}
	}
	else if ( g_nAS_Milestone == 3 )		// Fine3
	{
		nRet = AutoAstigmat_Analyze_Continue;

		g_nAS_Index ++;
		if ( g_nAS_Index < c_nASTableLength	)
		{
			lStigmatorX	= g_ASLookTable[ g_nAS_Index ].nStX;
			lStigmatorY	= g_ASLookTable[ g_nAS_Index ].nStY;
		}
		else
		{
			FineStigmator( lStigmatorX, lStigmatorY );
			nRet = AutoAstigmat_Analyze_Completed;
		}
	}

	return nRet;
}
