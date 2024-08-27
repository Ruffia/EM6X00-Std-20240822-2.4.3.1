// SemCtrl_CPS_FilamentCurrent.h: interface for the CSemCtrl_CPS_FilamentCurrent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CPS_FILAMENTCURRENT_H__3A75D539_2220_46CD_BBBB_4764DA02B0EC__INCLUDED_)
#define AFX_SEMCTRL_CPS_FILAMENTCURRENT_H__3A75D539_2220_46CD_BBBB_4764DA02B0EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_CPS_FilamentCurrent : public CSemBasisCtrl
{
	double	m_dIScale_ABS;
	double	m_dIScale_Set;
	double	m_dIOffset_Set;

	double	m_dIScale;
	double	m_dIOffset;
	double	m_dVScale;
	double	m_dVOffset;

	long	m_lUserMode;
	long	m_lEnable;

	long	m_lOldCtlIndex;

public:
	CSemCtrl_CPS_FilamentCurrent();
	virtual ~CSemCtrl_CPS_FilamentCurrent();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
	virtual long    GetPosByValue(const double dValue);
};

#endif // !defined(AFX_SEMCTRL_CPS_FILAMENTCURRENT_H__3A75D539_2220_46CD_BBBB_4764DA02B0EC__INCLUDED_)
