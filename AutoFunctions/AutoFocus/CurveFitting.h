#if !defined(Algorithm_CurveFitting__INCLUDED_)
#define Algorithm_CurveFitting__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveFitting.h : header file
//

typedef struct BEELINE{	// ax + by + c = 0
    long double		 a;
    long double		 b;
    long double		 c;
} BEELINE;
typedef BEELINE     *  PBEELINE;
typedef BEELINE FAR *  LPBEELINE;

typedef struct DFPoint{	// ax + by + c = 0
    long double		 x;
    long double		 y;
} DFPoint;
typedef DFPoint     *  PDFPoint;
typedef DFPoint FAR *  LPDFPoint;

const WORD X_Direction = 0;	// b = -1, ax + by + c = 0 ==> y = ax + c
const WORD Y_Direction = 1;	// a = -1, ax + by + c = 0 ==> x = by + c

BOOL LineFitting( LPBEELINE lpLine, LPDFPoint lpPoint, WORD wTotalNum, WORD wDirection = X_Direction);

#endif // Algorithm_CurveFitting__INCLUDED_
