// SemBasisCtrl.h: interface for the CSemBasisCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMBASISCTRL_H__5852F1F0_364F_43F7_8312_E05C1AEB960B__INCLUDED_)
#define AFX_SEMBASISCTRL_H__5852F1F0_364F_43F7_8312_E05C1AEB960B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "SemDataType.H"

const WORD BIT_0	= 1;
const WORD BIT_1	= 2;
const WORD BIT_2	= 4;
const WORD BIT_3	= 8;
const WORD BIT_4	= 16;
const WORD BIT_5	= 32;
const WORD BIT_6	= 64;
const WORD BIT_7	= 128;

const double dblPai = 3.1415926;

//{{0,0,0,0, (long)Min}, {0,0,0,0, (long)Max}, {0,0,0,0, (long)Pos}};
//{{0,0,0,0, (long)0}, {0,0,0,0, (long)0}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_HV		= {{0,0,0,0, (long)0}, {0,0,0,0, (long)120}, {0,0,0,0, (long)0}};
//const long CtrlDataSize_HV	= 121;		// 2005-2-2_统调
//const SemCtrlFigures DefaultCtrlFigures_HVAccelerate	= {{0,0,0,0, (DOUBLE)0}, {0,0,0,0, (DOUBLE)30}, {0,0,0,0, (DOUBLE)0}};

const SemCtrlFigures DefaultCtrlFigures_BeemAlignmentX	= {{0,0,0,0, (long)-16384}, {0,0,0,0, (long)16383}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_BeemAlignmentY	= {{0,0,0,0, (long)-16384}, {0,0,0,0, (long)16383}, {0,0,0,0, (long)0}};

const SemCtrlFigures DefaultCtrlFigures_ScanShiftX	= {{0,0,0,0, (long)-2047}, {0,0,0,0, (long)2048}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_ScanShiftY	= {{0,0,0,0, (long)-2047}, {0,0,0,0, (long)2048}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_StigmatorX	= {{0,0,0,0, (long)-2048}, {0,0,0,0, (long)2047}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_StigmatorY	= {{0,0,0,0, (long)-2048}, {0,0,0,0, (long)2047}, {0,0,0,0, (long)0}};

const SemCtrlFigures DefaultCtrlFigures_Brightness		= {{0,0,0,0, (long)-2047}, {0,0,0,0, (long)2048}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_Contrast		= {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};

const SemCtrlFigures DefaultCtrlFigures_OSE				= {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};


const SemCtrlFigures DefaultCtrlFigures_CondenserLens	= {{0,0,0,0, (LONG)0}, {0,0,0,0, (long)32767}, {0,0,0,0, (long)16384}};
const SemCtrlFigures DefaultCtrlFigures_ObjectiveLens	= {{0,0,0,0, (LONG)0}, {0,0,0,0, (long)32767}, {0,0,0,0, (long)16384}};

const SemCtrlFigures DefaultCtrlFigures_FilamentCurrent = {{0,0,0,0, (long)0}, {0,0,0,0, (long)200}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_FilamentVoltage = {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};

const SemCtrlFigures DefaultCtrlFigures_ERotate			= {{0,0,0,0, (long)0}, {0,0,0,0, (long)3599}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_DFocus			= {{0,0,0,0, (long)0}, {0,0,0,0, (LONG)0x6000}, {0,0,0,0, 0}};	// range: 0x2000 - 0x8000
const SemCtrlFigures DefaultCtrlFigures_DObjectiveLens	= {{0,0,0,0, (long)0}, {0,0,0,0, (long)1023}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_YAdjust			= {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};

const SemCtrlFigures DefaultCtrlFigures_GunCurrent				= {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};

//const SemCtrlFigures DefaultCtrlFigures_Magnifier	= {{0,0,0,0, (DOUBLE)15.0}, {0,0,0,0, (DOUBLE)150000.0}, {0,0,0,0, (DOUBLE)150.0}};

const SemCtrlFigures DefaultCtrlFigures_WorkDistance	= {{0,0,0,0, (long)0}, {0,0,0,0, (long)53}, {0,0,0,0, (long)12}};

const SemCtrlFigures DefaultCtrlFigures_Signal2Brightness		= {{0,0,0,0, (long)-2047}, {0,0,0,0, (long)2048}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_Signal2Contrast			= {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};

const SemCtrlFigures DefaultCtrlFigures_RobinsonDetectorSwitch	= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_AuxiliaryStigmation		= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_ObjectiveLensReverse	= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_CondenserLensReverse	= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_DCondenserLens			= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_SACP					= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_InquirerContent			= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_AutoFocus				= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_AutoStigmator			= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_RotationScan			= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_ImageInvert				= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};

//const SemCtrlFigures DefaultCtrlFigures_SignalProcesser			= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_SignalProcesser			= {{0,0,0,0, (LONG)0}, {0,0,0,0, (LONG)0xFFFF}, {0,0,0,0, (LONG)0}};
const SemCtrlFigures DefaultCtrlFigures_PC10SE1					= {{0,0,0,0, (LONG)0}, {0,0,0,0, (LONG)0xFFFF}, {0,0,0,0, (LONG)0}};
const SemCtrlFigures DefaultCtrlFigures_PC10SE6					= {{0,0,0,0, (LONG)0}, {0,0,0,0, (LONG)0xFFFF}, {0,0,0,0, (LONG)0}};

const SemCtrlFigures DefaultCtrlFigures_Channel2AB				= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_Channel2C				= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_Channel2D				= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_Channel2E				= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};
const SemCtrlFigures DefaultCtrlFigures_BseBySeDetectorSwitch	= {{0,0,0,0, (BOOL)0}, {0,0,0,0, (BOOL)1}, {0,0,0,0, (BOOL)0}};




///////////////////////////////////////////////////////////////////////////////
// CPS 3601
const SemCtrlFigures DefaultCtrlFigures_CPS_HV					= {{0,0,0,0, (long)0}, {0,0,0,0, (long)30000}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_CPS_Bias				= {{0,0,0,0, (long)0}, {0,0,0,0, (long)500}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_CPS_FilamentCurrent		= {{0,0,0,0, (long)0}, {0,0,0,0, (long)300}, {0,0,0,0, (long)0}};
const CString CtrlFigures_RegRootKey = "Software\\KYKY\\CPS";
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Spellman EBM30N12/771
const SemCtrlFigures DefaultCtrlFigures_SP_HV					= {{0,0,0,0, (long)0}, {0,0,0,0, (long)30000}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_SP_Bias					= {{0,0,0,0, (long)0}, {0,0,0,0, (long)400}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_SP_Heater				= {{0,0,0,0, (long)0}, {0,0,0,0, (long)300}, {0,0,0,0, (long)0}};
const SemCtrlFigures DefaultCtrlFigures_SP_PMT					= {{0,0,0,0, (long)0}, {0,0,0,0, (long)400}, {0,0,0,0, (long)0}};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// MCB
const SemCtrlFigures DefaultCtrlFigures_OSE_USB					= {{0,0,0,0, (long)0}, {0,0,0,0, (long)255}, {0,0,0,0, (long)0}};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// INI file name
const CString	m_strINIfileName	= "KYKYcfg.ini";

const int INI_Ratio			= 1;
const int INI_CaliOffset	= 2;
const int INI_Debug			= 3;
const int INI_DFforFF		= 4;
const int INI_Coadjust		= 5;

const int INIIndex_OL		= 1;
const int INIIndex_MagK		= 2;
const int INIIndex_WD		= 3;
const int INIIndex_DF		= 4;
const int INIIndex_Factors	= 5;
const int INIIndex_Factors2	= 6;
const int INIIndex_FactorMin= 7;
const int INIIndex_MagChange= 8;
const int INIIndex_HV		= 9;
const int INIIndex_Rotate	= 10;

const int INIIndex_ShiftX	= 1;
const int INIIndex_ShiftY	= 2;
const int INIIndex_StigX	= 3;
const int INIIndex_StigY	= 4;
const int INIIndex_CrossX	= 5;
const int INIIndex_CrossY	= 6;
const int INIIndex_AlignX	= 11;
const int INIIndex_AlignY	= 12;

const int INIIndex_Start	= 1;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// OL Coadjust
const	long	Co_6200_N	= 1191974834;
const	long	Co_6200_CNT	= 2139881;
const	long	Co_6200_C0	= 17059;
const	long	Co_6200_C1	= 54025;
///////////////////////////////////////////////////////////////////////////////

class CLanguageManagerCtrl;//09.03.23
#include "LanguageManagerCtrl.h"

class AFX_EXT_CLASS CSemBasisCtrl  
{
protected:
	CString				m_strText;

	DWORD				m_lastTickCount;

	//09.03.23
	CLanguageManagerCtrl*	m_pLanguageMan;

public:
	SemCtrlFigures		m_scfCtrl;
	double				m_dCaliOffset;
	int					m_nOpr;
	int					m_nType;

public:
	CSemBasisCtrl();
	virtual ~CSemBasisCtrl();

	BOOL	ReadINI( int nType, int nIndex, double* pdValue );	// 读取配置文件，获取参数

	//09.03.23
	BOOL	BindLanguageManager( CLanguageManagerCtrl* pLanguageManCtrl );

	virtual	BOOL	CoherenceCheck();

	virtual	void	SetRange( VARIANT varMin, VARIANT varMax );

	virtual	void	GetRange( VARIANT &varMin, VARIANT &varMax )
		{varMin = m_scfCtrl.varMin; varMax = m_scfCtrl.varMax;}

	virtual	VARIANT	SetPos( VARIANT varPos );
	virtual	VARIANT	GetPos(){ return m_scfCtrl.varPos; }
	virtual long    GetPosByValue(const double dValue);

	virtual	VARIANT	Plus( VARIANT varDelt );
	virtual	VARIANT	Minus( VARIANT varDelt );

	virtual	const	CString&	GetText() { return m_strText; }
	virtual	const	CString&	FormatText() { return m_strText; }
	virtual	const	CString&	FormatText( VARIANT varPos ) { return m_strText; }

	virtual	BOOL	Flush();/*{ return TRUE; };*/
	virtual void	SetParams( int nType, double dParam ){};	// Type:类型；Param:要设置的参数值
	virtual void	GetParams( int nType, double& dParam ){};
};

#endif // !defined(AFX_SEMBASISCTRL_H__5852F1F0_364F_43F7_8312_E05C1AEB960B__INCLUDED_)
