// SemCtrl_ObjectiveLens.h: interface for the CSemCtrl_ObjectiveLens class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_OBJECTIVELENS_H__83CE1F7A_55E0_497C_97F7_AE90A4A0E587__INCLUDED_)
#define AFX_SEMCTRL_OBJECTIVELENS_H__83CE1F7A_55E0_497C_97F7_AE90A4A0E587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class CSemCtrl_WD;
class CSemCtrl_ScanRotate;

class AFX_EXT_CLASS CSemCtrl_ObjectiveLens : public CSemBasisCtrl  
{
	CSemCtrl_WD*			m_pWD;
	CSemCtrl_ScanRotate*	m_pRotate;
	BOOL			m_bReady;

	double			m_dRateC;	// ͳ������ϵ��
	double			m_dRateM;	// ӳ�����ϵ�����������Ʒ��Ӧ���ﾵ��ʾֵ��Ӧ���ﾵ��ʾֵ��Χ���м�ֵ
	double			m_dK;		// ֻ�е�dK�����仯ʱ��Ҳ��������λ��ȷʵ�����仯ʱ���Ž����ﾵͳ��
	BOOL			m_bDebug;
	BOOL			m_bReverse;

	BOOL	m_bDFforFF;			// MagС��10000ʱΪ0��Mag��С��10000ʱΪ1����ʾDF����ΪFF
	int		m_nDFforFFBase;		// DF����ΪFFʱ�Ľ�λ����ֵ(�������ļ���ȡ)����DF�ۼƸı����ﵽ��ֵʱ��DF�ۼ������㡢����ֵ�ػ�0x8000��OL����ֵ��1
	long	m_lCtrlDF;			// DF����ΪFFʱ������ֵ

	// ͳ��������K = Ka * OLctrl +Kb
	double	m_dCo_Ka;
	double	m_dCo_Kb;
	double	m_dCo_HV[6];		// 0-5��Ӧ5/10/15/20/25/30mmʱ�ĸ�ѹͳ��ϵ��
	double	m_dCo_Rotate[2];	// 0��1�ֱ��Ӧ�ﾵͳ��ɨ����ת�����Թ�ϵ��K��Bϵ��
	BOOL	m_bCoadjustHVMode, m_bCoadjustRotateMode;

	long	m_lCtrlData[100000];
	long	m_lCtrlCount;

	void	UpdateCoadjust_HV_WD();
	void	UpdateCoadjust_HV_WD_RadioC( double dParam );
	void	UpdateCoadjust_OL_Rotate();

public:
	CSemCtrl_ObjectiveLens();
	virtual ~CSemCtrl_ObjectiveLens();
	
	BOOL	Bind( CSemCtrl_WD*	pWD );
	BOOL	Bind( CSemCtrl_ScanRotate* pRotate );

	virtual	VARIANT SetPos( VARIANT varPos );
	virtual	BOOL Flush();
	virtual	void SetParams( int nType, double dParam );	// Type:���ͣ�Param:Ҫ���õĲ���ֵ
	virtual void GetParams( int nType, double& dParam );
	virtual	const CString& FormatText();
	virtual	const CString& FormatText( VARIANT varPos );
	virtual long  GetPosByValue(const double dValue);

	friend class CSemCtrl_CPS_HV;
};

#endif // !defined(AFX_SEMCTRL_OBJECTIVELENS_H__83CE1F7A_55E0_497C_97F7_AE90A4A0E587__INCLUDED_)
