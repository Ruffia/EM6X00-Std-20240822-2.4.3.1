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

	double			m_dRateC;	// 统调比例系数
	double			m_dRateM;	// 映射比例系数，将标高样品对应的物镜显示值对应到物镜显示值范围的中间值
	double			m_dK;		// 只有当dK发生变化时，也即滚动条位置确实发生变化时，才进行物镜统调
	BOOL			m_bDebug;
	BOOL			m_bReverse;

	BOOL	m_bDFforFF;			// Mag小于10000时为0；Mag不小于10000时为1，表示DF复用为FF
	int		m_nDFforFFBase;		// DF复用为FF时的进位设置值(从配置文件读取)，当DF累计改变量达到该值时，DF累计量清零、控制值重回0x8000，OL控制值变1
	long	m_lCtrlDF;			// DF复用为FF时的设置值

	// 统调参数，K = Ka * OLctrl +Kb
	double	m_dCo_Ka;
	double	m_dCo_Kb;
	double	m_dCo_HV[6];		// 0-5对应5/10/15/20/25/30mm时的高压统调系数
	double	m_dCo_Rotate[2];	// 0和1分别对应物镜统调扫描旋转的线性关系的K和B系数
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
	virtual	void SetParams( int nType, double dParam );	// Type:类型；Param:要设置的参数值
	virtual void GetParams( int nType, double& dParam );
	virtual	const CString& FormatText();
	virtual	const CString& FormatText( VARIANT varPos );
	virtual long  GetPosByValue(const double dValue);

	friend class CSemCtrl_CPS_HV;
};

#endif // !defined(AFX_SEMCTRL_OBJECTIVELENS_H__83CE1F7A_55E0_497C_97F7_AE90A4A0E587__INCLUDED_)
