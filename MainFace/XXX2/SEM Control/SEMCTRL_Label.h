// SEMCTRL_Label.h : main header file for the KYKY2800B application
//

#if !defined(_SEMCTRL_LABEL_INCLUDED_)
#define _SEMCTRL_LABEL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////

CString CalculateStageUScale( BOOL	bIsX,			// 是X或Y：1 is X, 0 is Y
							 BOOL	bDir,			// 方向：1 is positive, 0 is negative
							 int	nScalePixels,	// 像素长度
							 int	&nScale );		// 实际移动步数

void SemLabel_Init( int nScreenWidth );



/////////////////////////////////////////////////

#endif // _SEMCTRL_LABEL_INCLUDED_
