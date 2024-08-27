// SemCoreAssemble.cpp: implementation of the CSemCoreAssemble class.
//
// Last Modify : 2009.03.23
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCoreAssemble.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCoreAssemble::CSemCoreAssemble()
{
	//m_pMPComm = NULL;

//	未完待续	2002-9-13
	Bind();	// Add for check bind

}

CSemCoreAssemble::~CSemCoreAssemble()
{

}


CSemCoreAssemble& CSemCoreAssemble::Instance()
{
	static CSemCoreAssemble Inst;
	return Inst;
}

BOOL CSemCoreAssemble::Bind()
{
	// ObjectiveLens, WD & Magnifier & ScanRotate Bind
	m_WD.Bind( &m_Magnifier );
	m_ObjectiveLens.Bind( &m_WD );
	m_ObjectiveLens.Bind( &m_ScanRotate );
	/////////////////////////////

	// ShiftX, ShiftY & Magnifier Bind
	/////////////////////////////
	m_Magnifier.Bind( &m_ScanShiftX );
	m_Magnifier.Bind( &m_ScanShiftY );
	/////////////////////////////
	m_Magnifier.Bind( &m_ScanRotate );

	// DFocus, OL Bind
	/////////////////////////////
	m_DFocus.Bind( &m_ObjectiveLens );
	/////////////////////////////

	/////////////////////////////
	// Accelerate
	/////////////////////////////
	m_HV.Bind( &m_HVAccelerate );
	m_GunCurrent.Bind( &m_GunCurrentAccelerate );
	/////////////////////////////
	
	// 新统调系统，全部由软件完成；实验阶段用老HV完成
	/////////////////////////////
	m_HV.Bind( &m_CondenserLens );
	m_HV.Bind( &m_ObjectiveLens );
	/////////////////////////////

	/////////////////////////////
	// Auto
	/////////////////////////////
	m_AutoFocus.Bind( &m_ObjectiveLens );

	m_AutoStigmator.Bind( &m_StigmatorX );
	m_AutoStigmator.Bind( &m_StigmatorY );
	/////////////////////////////

	/////////////////////////////
	// CPS 3601
	/////////////////////////////
	m_CPS_HV.Bind( &m_HVAccelerate );
	m_CPS_HV.Bind( &m_CondenserLens );
	m_CPS_HV.Bind( &m_ObjectiveLens );
	/////////////////////////////

	/////////////////////////////
	// Spellman EBM30N12/771
	/////////////////////////////
	m_SP_HV.Bind( &m_HVAccelerate );
	m_SP_HV.Bind( &m_CondenserLens );
	m_SP_HV.Bind( &m_ObjectiveLens );

	return TRUE;
}

// 09.03.23
BOOL CSemCoreAssemble::BindLanguageManager( CLanguageManagerCtrl* pLanguageManCtrl )
{
	m_pLanguageMan = pLanguageManCtrl;

	m_CondenserLensReverse.BindLanguageManager(m_pLanguageMan);
	m_DCondenserLens.BindLanguageManager(m_pLanguageMan);
	m_DObjectiveLens.BindLanguageManager(m_pLanguageMan);
	m_DFocus.BindLanguageManager(m_pLanguageMan);
	m_ObjectiveLensReverse.BindLanguageManager(m_pLanguageMan);
	m_AuxiliaryStigmation.BindLanguageManager(m_pLanguageMan);
	m_ImageInvert.BindLanguageManager(m_pLanguageMan);
	m_RobinsonDetectorSwitch.BindLanguageManager(m_pLanguageMan);
	m_RotationScan.BindLanguageManager(m_pLanguageMan);
	m_SignalProcesser.BindLanguageManager(m_pLanguageMan);
	m_SACP.BindLanguageManager(m_pLanguageMan);
	m_Channel2AB.BindLanguageManager(m_pLanguageMan);
	m_Channel2C.BindLanguageManager(m_pLanguageMan);
	m_Channel2D.BindLanguageManager(m_pLanguageMan);
	m_Channel2E.BindLanguageManager(m_pLanguageMan);
	m_BseBySeDetectorSwitch.BindLanguageManager(m_pLanguageMan);

	return TRUE;
}
