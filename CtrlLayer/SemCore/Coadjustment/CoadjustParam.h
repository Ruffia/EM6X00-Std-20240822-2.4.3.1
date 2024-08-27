
// CoadjustParam.h: interface for the CoadjustParam.cpp.
// 2005-2-2_ͳ��
//////////////////////////////////////////////////////////////////////

#if !defined(CoadjustParam_H_INCLUDED)
#define CoadjustParam_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const CString HVCoadjustParamFilename = _T("Coadjust_HV.txt");
const CString WDCoadjustParamFilename = _T("Coadjust_WD.txt");

const long	CtrlDataSizeMax_HV	= 256;	// �����Ʋ���
extern WORD	m_wHVCtlData[];
extern WORD	m_wHVAdjustData[];

const long	CtrlDataSizeMax_WD	= 256;	// �����Ʋ���
extern WORD	m_nObjectiveLensToWDIndex[];

//������ʾ��������
//WorkDistance <--> ObjectiveLens electric-current : WorkDistance ( Unit : mm )
extern WORD		m_nWDIndexToWD[];
//���ڼ������ض����������£���С�Ŵ���
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
