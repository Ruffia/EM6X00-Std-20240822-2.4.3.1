
#include "stdafx.h"
#include "PublicFunction.h"

/////////////////////////////////////////////////////////////////

void ChangeCtrlStyle( CWnd *pCtrlWnd, long lStyle, BOOL bSetBit )
{
	long    lStyleOld;
	CRect   rect;

	lStyleOld = GetWindowLong( pCtrlWnd->GetSafeHwnd(), GWL_STYLE );
	if ( bSetBit )
		lStyleOld |= lStyle;
	else
		lStyleOld &= ~lStyle;
	SetWindowLong( pCtrlWnd->GetSafeHwnd(), GWL_STYLE, lStyleOld );
	pCtrlWnd->GetWindowRect(&rect);
	pCtrlWnd->InvalidateRect(&rect);
}

BOOL IsAvoidableScrollCode(	UINT nSBCode )
{
	BOOL	blRet = TRUE;

	switch ( nSBCode )
	{
	case SB_LINEUP:			// 0
	//case SB_LINELEFT:		// 0
	case SB_LINEDOWN:		// 1
	//case SB_LINERIGHT:	// 1
	case SB_PAGEUP:			// 2
	//case SB_PAGELEFT:		// 2
	case SB_PAGEDOWN:		// 3
	//case SB_PAGERIGHT:	// 3
	case SB_THUMBTRACK:		// 5
	case SB_THUMBPOSITION:	// 4
		blRet = FALSE;
		break;

//	case SB_THUMBPOSITION:	// 4
	case SB_TOP:			// 6
	//case SB_LEFT:			// 6
	case SB_BOTTOM:			// 7
	//case SB_RIGHT:		// 7
	case SB_ENDSCROLL:		// 8
		break;

	default:
		break;
	}

	return blRet;
}

