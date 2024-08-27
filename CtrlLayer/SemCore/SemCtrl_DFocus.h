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

	double	m_dScanHeight;	// 纵向扫描范围
	int		m_nValley;		// Valley的个数

	BYTE	m_bPolarity_Atten;
	WORD	m_wusbCtl_Step;

	int		m_nDFforFFBase;	// DF复用为FF时的进位设置值(从配置文件读取)，当DF累计改变量达到该值时，DF累计量清零、控制值重回0x8000，OL控制值变1
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
	virtual void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_DFOCUS_H__AD1BE65E_9E04_4348_AAFA_E3CA06932009__INCLUDED_)
