
// CoadjustParam.h: interface for the CoadjustParam.cpp.
// 2005-2-2_统调
//////////////////////////////////////////////////////////////////////

#if !defined(CoadjustParam_H_INCLUDED)
#define CoadjustParam_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const CString HVCoadjustParamFilename = _T("Coadjust_HV.txt");
const CString WDCoadjustParamFilename = _T("Coadjust_WD.txt");

const long	CtrlDataSizeMax_HV	= 256;	// 最大控制步数
extern WORD	m_wHVCtlData[];
extern WORD	m_wHVAdjustData[];

const long	CtrlDataSizeMax_WD	= 256;	// 最大控制步数
extern WORD	m_nObjectiveLensToWDIndex[];

//用于显示工作距离
//WorkDistance <--> ObjectiveLens electric-current : WorkDistance ( Unit : mm )
extern WORD		m_nWDIndexToWD[];
//用于检索（特定工作距离下）最小放大倍数
extern double	m_dfWDIndexToMAGmin[];

//bool LoadSemHVAdjustParam(const CString& filename, 
//	const WORD nLengthMax, WORD &nLength,
//	WORD wHVData[CtrlDataSizeMax_HV], WORD wHVAdjustData[CtrlDataSizeMax_HV] );

DWORD LoadSemHVAdjustParam(const CString& filename, 
	const WORD nLengthMax,
	WORD wHVData[CtrlDataSizeMax_HV], WORD wHVAdjustData[CtrlDataSizeMax_HV] );

bool LoadSemWDAdjustParam(const CString& filename, 
	const WORD nLengthMax, WORD &nLength, 
	WORD wObjectiveLensToWDIndex[], WORD wWDIndexToWD[], double	dfWDIndexToMAGmin[] );

#endif // !defined(CoadjustParam_H_INCLUDED)
