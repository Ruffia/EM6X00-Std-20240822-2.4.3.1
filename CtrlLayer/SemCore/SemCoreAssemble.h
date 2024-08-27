// SemCoreAssemble.h: interface for the CSemCoreAssemble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCOREASSEMBLE_H__FBB64732_618D_4ADA_80AD_9CB0C15D8363__INCLUDED_)
#define AFX_SEMCOREASSEMBLE_H__FBB64732_618D_4ADA_80AD_9CB0C15D8363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemCtrl_HV.h"
#include "SemCtrl_WD.h"
#include "SemCtrl_ObjectiveLens.h"
#include "SemCtrl_CondenserLens.h"
#include "SemCtrl_Magnifier.h"
#include "SemCtrl_BeemAlignmentX.h"
#include "SemCtrl_BeemAlignmentY.h"
#include "SemCtrl_ScanShiftX.h"
#include "SemCtrl_ScanShiftY.h"
#include "SemCtrl_Brightness.h"
#include "SemCtrl_Contrast.h"
#include "SemCtrl_StigmatorX.h"
#include "SemCtrl_StigmatorY.h"
#include "SemCtrl_FilamentCurrent.h"
#include "SemCtrl_GunCurrent.h"
#include "SemCtrl_ObjectiveLensReverse.h"
#include "SemCtrl_CondenserLensReverse.h"
#include "SemCtrl_AuxiliaryStigmation.h"
#include "SemCtrl_DCondenserLens.h"
#include "SemCtrl_ScanRotate.h"
#include "SemCtrl_SACP.h"
#include "SemCtrl_Signal2Brightness.h"
#include "SemCtrl_Signal2Contrast.h"
#include "SemCtrl_YAdjust.h"
#include "SemCtrl_DFocus.h"
#include "SemCtrl_DObjectiveLens.h"
#include "SemCtrl_RobinsonDetectorSwitch.h"
#include "SemCtrl_SignalProcesser.h"
#include "SemCtrl_InquirerZ80A.h"
#include "SemCtrl_InquirerZ80B.h"
#include "SemCtrl_GunCurrentAccelerate.h"
#include "SemCtrl_HVAccelerate.h"
#include "SemCtrl_AutoFocus.h"
#include "SemCtrl_AutoStigmator.h"
#include "SemCtrl_RotationScan.h"
#include "SemCtrl_OSE.h"
#include "SemCtrl_ImageInvert.h"
#include "SemCtrl_PC10SE1.h"
#include "SemCtrl_PC10SE6.h"
#include "SemCtrl_Channel2AB.h"
#include "SemCtrl_Channel2C.h"
#include "SemCtrl_Channel2D.h"
#include "SemCtrl_Channel2E.h"
#include "SemCtrl_BseBySeDetectorSwitch.h"
#include "SemCtrl_AdjustParameter.h"

// CPS 3601
#include "SemCtrl_CPS_HV.h"
#include "SemCtrl_CPS_Bias.h"
#include "SemCtrl_CPS_FilamentCurrent.h"
// CPS 3601

// Spellman EBM30N12/771
#include "SemCtrl_SP_HV.h"
#include "SemCtrl_SP_Bias.h"
#include "SemCtrl_SP_Heater.h"
#include "SemCtrl_SP_PMT.h"
// Spellman EBM30N12/771

// MCB
#include "SemCtrl_MCB.h"
// MCB

class AFX_EXT_CLASS CSemCoreAssemble  
{
	// 09.03.23
	CLanguageManagerCtrl*	m_pLanguageMan;

public:
	CSemCtrl_InquirerZ80A			m_InquirerZ80A;
	CSemCtrl_InquirerZ80B			m_InquirerZ80B;

	CSemCtrl_HV						m_HV;
	CSemCtrl_HVAccelerate			m_HVAccelerate;
	CSemCtrl_WD						m_WD;
	CSemCtrl_ObjectiveLens			m_ObjectiveLens;
	CSemCtrl_CondenserLens			m_CondenserLens;
	CSemCtrl_Magnifier				m_Magnifier;
	CSemCtrl_BeemAlignmentX			m_BeemAlignmentX;
	CSemCtrl_BeemAlignmentY			m_BeemAlignmentY;
	CSemCtrl_ScanShiftX				m_ScanShiftX;
	CSemCtrl_ScanShiftY				m_ScanShiftY;
	CSemCtrl_Brightness				m_Brightness;
	CSemCtrl_Contrast				m_Contrast;
	CSemCtrl_StigmatorX				m_StigmatorX;
	CSemCtrl_StigmatorY				m_StigmatorY;
	CSemCtrl_AuxiliaryStigmation	m_AuxiliaryStigmation;
	CSemCtrl_FilamentCurrent		m_FilamentCurrent;
	CSemCtrl_GunCurrent				m_GunCurrent;
	CSemCtrl_GunCurrentAccelerate	m_GunCurrentAccelerate;
	CSemCtrl_ObjectiveLensReverse	m_ObjectiveLensReverse;
	CSemCtrl_CondenserLensReverse	m_CondenserLensReverse;
	CSemCtrl_DCondenserLens			m_DCondenserLens;
	CSemCtrl_ScanRotate				m_ScanRotate;
	CSemCtrl_SACP					m_SACP;
	CSemCtrl_Signal2Brightness		m_Signal2Brightness;
	CSemCtrl_Signal2Contrast		m_Signal2Contrast;
	CSemCtrl_YAdjust				m_YAdjust;
	CSemCtrl_DFocus					m_DFocus;
	CSemCtrl_DObjectiveLens			m_DObjectiveLens;
	CSemCtrl_RobinsonDetectorSwitch	m_RobinsonDetectorSwitch;
	CSemCtrl_SignalProcesser		m_SignalProcesser;
	CSemCtrl_AutoFocus				m_AutoFocus;
	CSemCtrl_AutoStigmator			m_AutoStigmator;
	CSemCtrl_RotationScan			m_RotationScan;
	CSemCtrl_OSE					m_OSE;
	CSemCtrl_ImageInvert			m_ImageInvert;
	CSemCtrl_PC10SE1				m_PC10SE1;
	CSemCtrl_PC10SE6				m_PC10SE6;
	CSemCtrl_Channel2AB				m_Channel2AB;
	CSemCtrl_Channel2C				m_Channel2C;
	CSemCtrl_Channel2D				m_Channel2D;
	CSemCtrl_Channel2E				m_Channel2E;
	CSemCtrl_BseBySeDetectorSwitch	m_BseBySeDetectorSwitch;
	CSemCtrl_AdjustParameter        m_SemCtrl_AdjustmentParameter;

	// CPS 3601
	CSemCtrl_CPS_HV					m_CPS_HV;
	CSemCtrl_CPS_Bias				m_CPS_Bias;
	CSemCtrl_CPS_FilamentCurrent	m_CPS_FilamentCurrent;
	// CPS 3601

	// Spellman EBM30N12/771
	CSemCtrl_SP_HV					m_SP_HV;
	CSemCtrl_SP_Bias				m_SP_Bias;
	CSemCtrl_SP_Heater				m_SP_Heater;
	CSemCtrl_SP_PMT					m_SP_PMT;
	// Spellman EBM30N12/771

	// MCB
	CSemCtrl_MCB					m_MCB;
	// MCB

protected:
	CSemCoreAssemble();
	~CSemCoreAssemble();

public:
	static CSemCoreAssemble& Instance();

	BOOL	Bind();

	// 09.03.23
	BOOL	BindLanguageManager( CLanguageManagerCtrl* pLanguageManCtrl );
};

#endif // !defined(AFX_SEMCOREASSEMBLE_H__FBB64732_618D_4ADA_80AD_9CB0C15D8363__INCLUDED_)
