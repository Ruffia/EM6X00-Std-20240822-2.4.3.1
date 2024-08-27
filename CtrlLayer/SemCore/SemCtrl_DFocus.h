// SemCtrl_DFocus.h: interface for the CSemCtrl_DFocus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_DFOCUS_H__AD1BE65E_9E04_4348_AAFA_E3CA06932009__INCLUDED_)
#define AFX_SEMCTRL_DFOCUS_H__AD1BE65E_9E04_4348_AAFA_E3CA06932009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "SemCtrl_ObjectiveLens.h"

class AFX_EXT_CLASS CSemCtrl_DFocus : public CSemBasisCtrl  
{
	CSemCtrl_ObjectiveLens*	m_pOL;
	long	m_lSwitch;
	CString	m_strSwitchText;

	double	m_dScanHeight;	// ����ɨ�跶Χ
	int		m_nValley;		// Valley�ĸ���

	BYTE	m_bPolarity_Atten;
	WORD	m_wusbCtl_Step;

	int		m_nDFforFFBase;	// DF����ΪFFʱ�Ľ�λ����ֵ(�������ļ���ȡ)����DF�ۼƸı����ﵽ��ֵʱ��DF�ۼ������㡢����ֵ�ػ�0x8000��OL����ֵ��1
	int		m_nFFTotal;
	BOOL	m_bDFforFF;

public:
	CSemCtrl_DFocus();
	virtual ~CSemCtrl_DFocus();

	BOOL Bind( CSemCtrl_ObjectiveLens*	pOL );

	VARIANT GetSwitch( )
		{ 	VARIANT varSwitch;
			varSwitch.lVal = m_lSwitch;
			return varSwitch;	};
	void SetSwitch( VARIANT varSwitch );
//		{ m_lSwitch = varSwitch.lVal; };
	const CString& FormatSwitchText();
	const CString& FormatSwitchText( VARIANT varSwitch );

	virtual BOOL Flush();
	virtual void SetParams( int nType, double dParam );	// Type:���ͣ�Param:Ҫ���õĲ���ֵ
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_DFOCUS_H__AD1BE65E_9E04_4348_AAFA_E3CA06932009__INCLUDED_)
