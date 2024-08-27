// CurveFitting.cpp : implementation file
//

#include "stdafx.h"
#include "CurveFitting.h"


/////////////////////////////////////////////////////////////////////////////
// least square curve fitting

BOOL LineFitting( LPBEELINE lpLine, LPDFPoint lpPoint, WORD wTotalNum, WORD wDirection)
{
    long double			a, b, c;
    long double			Sxx, Syy, Sxy, Sy, Sx;
    long double			x, y, f1, f2;
    WORD		    	i;
    
    if( wTotalNum <= 1 ) return	FALSE;
    
    if( wDirection == Y_Direction )	
    {
	    a = (long double)-1.0;		// x = by + c
    
    	Sy = Sx = (long double)0.0;
	    Sxy = Syy = Sx;
    	f1 = (long double)1.0 / (long double)wTotalNum;
	    for( i=0; i<wTotalNum; i ++ )
    	{
    		x = (long double)lpPoint[i].x;
	    	y = (long double)lpPoint[i].y;
    	
    		Sx += x * f1;
    		Sy += y * f1;
	    	Syy += y * y * f1;
    		Sxy += x * y * f1;
	    }
	
		f2 = Sy * Sy - Syy;
	    b = ( Sx * Sy - Sxy ) / f2;
    	c = ( Sy * Sxy - Sx * Syy ) / f2;
    }
    else // ( wDirection == X_Direction )
    {
	    b = (long double)-1.0;		// y = ax + c
    
    	Sy = Sx = (long double)0.0;
	    Sxy = Sxx = Sx;
    	f1 = (long double)1.0 / (long double)wTotalNum;
	    for( i=0; i<wTotalNum; i ++ )
    	{
    		x = (long double)lpPoint[i].x;
	    	y = (long double)lpPoint[i].y;
    	
    		Sx += x * f1;
    		Sy += y * f1;
	    	Sxx += x * x * f1;
    		Sxy += x * y * f1;
	    }
	
		f2 = Sx * Sx - Sxx;
	    a = ( Sx * Sy - Sxy ) / f2;
    	c = ( Sx * Sxy - Sy * Sxx ) / f2;
    }
    
	lpLine->a = a;
	lpLine->b = b;
	lpLine->c = c;
	
	return TRUE;
}
