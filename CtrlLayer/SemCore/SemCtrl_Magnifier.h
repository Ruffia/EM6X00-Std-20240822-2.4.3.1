// SemCtrl_Magnifier.h: interface for the CSemCtrl_Magnifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_MAGNIFIER_H__152A74BF_E981_421F_A854_A3C8F6078AC4__INCLUDED_)
#define AFX_SEMCTRL_MAGNIFIER_H__152A74BF_E981_421F_A854_A3C8F6078AC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mcio.h>
#include "SemBasisCtrl.h"

#include "SemCtrl_ScanShiftX.h"
#include "SemCtrl_ScanShiftY.h"
#include "SemCtrl_ScanRotate.h"

class CAmplifier		// 放大器工作范围 (1, 10000]
{
	double	m_dfRate;	// 放大器倍数 (1, 10]
	long	m_lStage;	// 放大器级数 [0,  3]

//	double	m_dfOldRate;
	long	m_lOldStage;	// 无需频繁设置档位，所以保存上一次的档位

	BOOL	m_bReady;

	WORD	m_wRateXY[10];
	WORD	m_wRateX;
	WORD	m_wRateY;

	double	m_dRateMag[9];

public:	
	CAmplifier();
	~CAmplifier();

	long	Update( double dfMagMin, double dfMag );
	double	GetMagFromAmp( double dfMagMin );

	BOOL	UpdateStrobe( BYTE bID, BOOL bHigh );
	BOOL	UpdateLowAmp( BOOL bHigh );
	BOOL	UpdateGainRly( BOOL bHigh );
	BOOL	UpdateMainX( BOOL bHigh );
	BOOL	UpdateMainY( BOOL bHigh );
	BOOL	UpdateShortRly( BOOL bHigh );
	BOOL	UpdateStigES( BOOL bHigh );
	BOOL	UpdateShiftES( BOOL bHigh );
	BOOL	UpdateLowRngMux( short nIndex );

	BOOL	AmpMain();
	BOOL	AmpLow1();
	BOOL	AmpLow2();
	BOOL	AmpLow3();
	BOOL	AmpLow4();

	BOOL	ClearAllChannels();
	BOOL	ClearAnalogSwitches();

	BOOL	Flush();
	BOOL	PostMSGEx( USB_SEM_ControlMessage msg );

	BOOL	SetParams( int nType, double* pdValue );
	void	SetParam( int nType, double dParam );
	void	GetParam( int nType, double& dRateCtrl );
};

class AFX_EXT_CLASS CSemCtrl_Magnifier : public CSemBasisCtrl  
{
// 2005-2-2_统调
//	const static WORD	m_nWDIndexToParam[ CtrlDataSize_WorkDistance ];	
	long				m_lWDIndex;

	CAmplifier			m_Amp;
	CSemCtrl_ScanShiftX*	m_pShiftX;
	CSemCtrl_ScanShiftY*	m_pShiftY;
	CSemCtrl_ScanRotate*	m_pRotate;

	double					m_dCoFactorA;
	double					m_dCoFactorB;
	int						m_nDebugMode;
	int						m_nRotateLimitAngle;

protected:
	BOOL	UpdateWDIndex( long lWDIndex );
	BOOL	Co_UpdatePosByM( double dM );
	BOOL	Co_UpdatePosByWD( double dWD );

public:
	CSemCtrl_Magnifier();
	virtual ~CSemCtrl_Magnifier();

	BOOL	Bind();
	BOOL	Bind( CSemCtrl_ScanShiftX*	pShiftX );
	BOOL	Bind( CSemCtrl_ScanShiftY*	pShiftY );
	BOOL	Bind( CSemCtrl_ScanRotate*	pRotate );

	virtual	BOOL	CoherenceCheck();

	virtual	VARIANT	Plus( VARIANT varDelt );
	virtual VARIANT	Minus( VARIANT varDelt );

	virtual	const	CString& FormatText();
	virtual	const	CString& FormatText( VARIANT varPos );

	virtual	BOOL	Flush();
	virtual void	SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual void	GetParams( int nType, double& dParam );

	friend	class	CSemCtrl_WD;
};


#endif // !defined(AFX_SEMCTRL_MAGNIFIER_H__152A74BF_E981_421F_A854_A3C8F6078AC4__INCLUDED_)
