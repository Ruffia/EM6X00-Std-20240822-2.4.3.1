#if !defined(AFX_COMMVINCE_H__F31C10C1_FE82_47FE_9916_2DD6EEB5BEB8__INCLUDED_)
#define AFX_COMMVINCE_H__F31C10C1_FE82_47FE_9916_2DD6EEB5BEB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommVince.h : header file
//

#include "Comm_Vince.h"

/////////////////////////////////////////////////////////////////////////////
// CCommVince window

class AFX_EXT_CLASS CCommVince
{
// Construction
public:
	CCommVince();
	~CCommVince();

// Attributes
public:

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Open( short port );
	BOOL	Close();
	BOOL	GetConnectState();

	void	SetCfgParams( short nIndex, short nCID, float fValue );
	void	Move( short nIndex, short nCID, long lValue );
	void	SetOrg( short nCID );
	void	MotorEn( short nCID, BOOL bEn );
	void	MotorStop( short nCID, short nValue = 0 );
	void	BackZero( short nCID, BOOL bStart );

	int		GetPosition( short nCID );
	float	GetSpeed( short nIndex, short nCID );
	Vince_Param_Status	GetStatus( short nCID );

	void	LoadDefault( short nCID );

	void	GetHome( short nIndex, unsigned char ucAxis );
	void	GetLimit( short nIndex, unsigned char ucAxis );
	void	GetSys( short nIndex, unsigned char ucAxis );

	void	SetSpeed( short nIndex, unsigned char ucAxis, float fSpeed );
	void	SetHome( short nIndex, unsigned char ucAxis, unsigned char ucMode );
	void	SetLimit( short nIndex, unsigned char ucAxis, unsigned char ucAble, float fLimit );
	void	SetSys( short nIndex, unsigned char ucAxis, unsigned char ucSys, float fSys );

//	void	BackZero( unsigned char ucAxis, unsigned char ucMode );
	void	MoveR( unsigned char ucAxis, long lSteps, BOOL bBacklash = TRUE );
	void	MoveA( unsigned char ucAxis, long lSteps, BOOL bBacklash = TRUE );
	void	GoOrigion( unsigned char ucAxis );
	void	GoHome( unsigned char ucAxis );
	void	GoAxisHome( unsigned char ucX, unsigned char ucY, unsigned char ucZ, unsigned char ucT );
	void	GoPosition( unsigned char ucAxis, char cState, char cMode, char cDir, float fCount );
	void	StopMove( unsigned char ucAxis );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMVINCE_H__F31C10C1_FE82_47FE_9916_2DD6EEB5BEB8__INCLUDED_)
