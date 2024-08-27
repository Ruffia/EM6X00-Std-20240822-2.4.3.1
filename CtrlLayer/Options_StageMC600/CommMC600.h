#if !defined(AFX_COMMMC600_H__EFEEBC0C_BD08_4F8D_9ECC_E2E4392CCA3B__INCLUDED_)
#define AFX_COMMMC600_H__EFEEBC0C_BD08_4F8D_9ECC_E2E4392CCA3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommMC600.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommMC600 window

class AFX_EXT_CLASS CCommMC600
{
// Construction
public:
	CCommMC600();
	~CCommMC600();

// Attributes
public:

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Open( short port );
	BOOL	Close();
	BOOL	GetConnectState();

	float	GetPosition( unsigned char ucAxis );
	float	GetSpeed( short nIndex, unsigned char ucAxis );
	void	GetHome( short nIndex, unsigned char ucAxis );
	void	GetLimit( short nIndex, unsigned char ucAxis );
	void	GetSys( short nIndex, unsigned char ucAxis );

	void	SetSpeed( short nIndex, unsigned char ucAxis, float fSpeed );
	void	SetHome( short nIndex, unsigned char ucAxis, unsigned char ucMode );
	void	SetLimit( short nIndex, unsigned char ucAxis, unsigned char ucAble, float fLimit );
	void	SetSys( short nIndex, unsigned char ucAxis, unsigned char ucSys, float fSys );

	void	BackZero( unsigned char ucAxis, unsigned char ucMode );
	void	MoveR( unsigned char ucAxis, long lSteps, BOOL bBacklash = TRUE );
	void	MoveA( unsigned char ucAxis, long lSteps, BOOL bBacklash = TRUE );
 	void	GoOrigion( unsigned char ucAxis );
 	void	GoHome( unsigned char ucAxis );
	void	GoAxisHome( unsigned char ucX, unsigned char ucY, unsigned char ucZ, unsigned char ucT );
	void	GoPosition( unsigned char ucAxis, char cState, char cMode, char cDir, float fCount );
	void	StopMove( unsigned char ucAxis );

	void	LoadDefault( BOOL bFactory );

	void	GoInterpolationLine2( unsigned char ucAxis1, float fS1, unsigned char ucAxis2, float fS2 );
	void	GoInterpolationLine3( unsigned char ucAxis1, float fS1, unsigned char ucAxis2, float fS2, unsigned char ucAxis3, float fS3 );

	void	SetBacklashParams( short nIndex, short nDir, int nCount );
	void	CoherenceCheck_Backlash( unsigned char ucAxis, long& lSteps, BOOL bRelative );

	int		GetDir( unsigned char ucAxis );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMMC600_H__EFEEBC0C_BD08_4F8D_9ECC_E2E4392CCA3B__INCLUDED_)
