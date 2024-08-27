#if !defined(AFX_VIRTUALDISPLAY_H__0E6EAC37_3949_4A3A_B6C0_0EC4CA0BF2F2__INCLUDED_)
#define AFX_VITUALDISPLAY_H__0E6EAC37_3949_4A3A_B6C0_0EC4CA0BF2F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualDisplay.h : header file
//

///////////////////////////////////////////////////////
#ifndef _INC_DIBAPI

// Handle to a DIB
DECLARE_HANDLE(HDIB);

#endif // _INC_DIBAPI
///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CVirtualDisplay window

class CDisplay;
class AFX_EXT_CLASS CVirtualDisplay
{
	CDisplay*	m_pDisplay;
	BOOL		m_bReady;

	double		m_dblZoom;				// The map of Image to screen
	CSize		m_sizeReso;				// Resolution
	int			m_nusbFunction;			// Scan Type

// Construction
public:
	CVirtualDisplay();
	virtual ~CVirtualDisplay();

// Attributes
public:

// Operations
public:
	BOOL	Close( );
	BOOL	Reset( CWnd* pWnd );
	BOOL	IsShouldReset();
	BOOL	isReady(){ return m_bReady; }

	HDIB	GetDib();

	void	SetResolution(CSize szReso);
	void	SetFunction( int nFunction );
	void	GetCurrentY( int &nY1, int &nY2 );
	
	double	GetZoom();
	void	SetZoom(double dblZoom);

	void	SetDestFrame( CRect rcDestFrame );
	CSize	GetSizes( int nIndex );

	BOOL	WriteCommImageData( LPVOID lpBuff, long lBuffSize, CRect& rcFrameWindow );
	BOOL	Read( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALDISPLAY_H__0E6EAC37_3949_4A3A_B6C0_0EC4CA0BF2F2__INCLUDED_)
