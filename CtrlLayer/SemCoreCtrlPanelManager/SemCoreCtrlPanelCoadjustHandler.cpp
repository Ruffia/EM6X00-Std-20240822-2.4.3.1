#include "StdAfx.h"
#include "SemCoreCtrlPanelCoadjustHandler.h"
using namespace nsSemCoreCtrlPanelCoadjust;
#include "SemCtrlPanelManager.h"
#include "SemCoreAssemble.h"
#include "Factory.h"

const DWORD	FlagIgnoreFlush = (DWORD)0x04;

IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_ObjectiveLens,SemControlPanelClassID,SemControlPanelClass_ObjectiveLens)
void CLASS_SemControlPanelClass_ObjectiveLens::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;
	VARIANT Xmin, Xmax, Ymin, Ymax;
	// 物镜统调
	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_WorkDistance;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_WD.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_Magnifier;
	SerialNumberNew.lVal	= -1;
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( Xmin, Xmax );
	Ymin = Xmin;
	Ymax = Xmax;
	pManager->SyncSemCoreCtrlPanelRange( ClassIDNew, SerialNumberNew, Xmin, Xmax, Ymin, Ymax);
	/////////////////////////////////////////////////////////

	XposNew			= CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );
	/////////////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_Magnifier,SemControlPanelClassID,SemControlPanelClass_Magnifier)
void CLASS_SemControlPanelClass_Magnifier::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;
	VARIANT Xmin, Xmax;

	///////////////////////////////////////////////////////////////////////////
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( Xmin, Xmax );
	XposNew			= CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	double dRate	= 0.0;
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 15, dRate );
	double dMode	= 0.0;
	CSemCoreAssemble::Instance().m_Magnifier.GetParams( 50, dMode );
	/////////////////////////////////////////////////////////

	VARIANT	EnableNew;
	ClassIDNew.lVal		= SemControlPanelClass_ScanRotate;
	SerialNumberNew.lVal= -1;
	if( XposNew.dblVal /Xmin.dblVal < dRate/10.0 )
	{
		XposNew = CSemCoreAssemble::Instance().m_ScanRotate.GetPos();
		FlagNew.lVal		= FlagCtrlX;
		//			XposNew.lVal		= 0;
		YposNew				= XposNew;
		pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, YposNew, FlagNew );
		if( dMode < 1 )	// 调试模式下任何放大倍数不禁用扫描旋转
		{
			EnableNew.lVal		= 0;
			FlagNew.lVal		= 3;
			pManager->EnableSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, EnableNew, FlagNew );
		}
	}
	else
	{
		EnableNew.lVal		= 1;
		FlagNew.lVal		= 3;
		pManager->EnableSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, EnableNew, FlagNew );
	}
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_HV,SemControlPanelClassID,SemControlPanelClass_HV)
void CLASS_SemControlPanelClass_HV::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;

		// 高压统调
	ClassIDNew.lVal			= SemControlPanelClass_HVAccelerate;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_HVAccelerate.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );	

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_CondenserLens;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_ObjectiveLens;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	// 高压统调时，只有物镜、聚光镜、扫描波参数变，工作距离和放大倍数不应有所改变
/*		///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_WorkDistance;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_WD.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_Magnifier;
	SerialNumberNew.lVal	= -1;
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( Xmin, Xmax );
	Ymin = Xmin;
	Ymax = Xmax;
	SyncSemCoreCtrlPanelRange( ClassIDNew, SerialNumberNew, Xmin, Xmax, Ymin, Ymax);
	/////////////////////////////////////////////////////////

	XposNew			= CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );
*/		/////////////////////////////////////////////////////////
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_GunCurrent,SemControlPanelClassID,SemControlPanelClass_GunCurrent)
void CLASS_SemControlPanelClass_GunCurrent::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;

	ClassIDNew.lVal			= SemControlPanelClass_GunCurrentAccelerate;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_GunCurrentAccelerate.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;

	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_CPS_HV,SemControlPanelClassID,SemControlPanelClass_CPS_HV)
void CLASS_SemControlPanelClass_CPS_HV::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;

	ClassIDNew.lVal			= SemControlPanelClass_HVAccelerate;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_HVAccelerate.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );	

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_CondenserLens;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_ObjectiveLens;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	// 高压统调时，只有物镜、聚光镜、扫描波参数变，工作距离和放大倍数不应有所改变
/*		///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_WorkDistance;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_WD.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_Magnifier;
	SerialNumberNew.lVal	= -1;
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( Xmin, Xmax );
	Ymin = Xmin;
	Ymax = Xmax;
	pManager->SyncSemCoreCtrlPanelRange( ClassIDNew, SerialNumberNew, Xmin, Xmax, Ymin, Ymax);
	/////////////////////////////////////////////////////////

	XposNew			= CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );
*/		/////////////////////////////////////////////////////////

}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_CPS_Bias,SemControlPanelClassID,SemControlPanelClass_CPS_Bias)
void CLASS_SemControlPanelClass_CPS_Bias::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;

	ClassIDNew.lVal			= SemControlPanelClass_CPS_Bias;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_CPS_Bias.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;

	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );	
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_CPS_FilamentCurrent,SemControlPanelClassID,SemControlPanelClass_CPS_FilamentCurrent)
void CLASS_SemControlPanelClass_CPS_FilamentCurrent::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;

	ClassIDNew.lVal			= SemControlPanelClass_FilamentCurrent;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;

	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );	
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelCoadjustHandler,CLASS_SemControlPanelClass_SP_HV,SemControlPanelClassID,SemControlPanelClass_SP_HV)
void CLASS_SemControlPanelClass_SP_HV::Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	VARIANT ClassIDNew;
	VARIANT SerialNumberNew;
	VARIANT XposNew, YposNew, FlagNew;

	ClassIDNew.lVal			= SemControlPanelClass_HVAccelerate;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_HVAccelerate.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );	

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_CondenserLens;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	///////////////////////////////////////////////////////////////////////////
	ClassIDNew.lVal			= SemControlPanelClass_ObjectiveLens;
	SerialNumberNew.lVal	= -1;
	XposNew			= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
	YposNew			= XposNew;
	FlagNew.lVal	= FlagCtrlX;
	pManager->SyncSemCoreCtrlPanel( ClassIDNew, SerialNumberNew, XposNew, Ypos, FlagNew );

	// 高压统调时，只有物镜、聚光镜、扫描波参数变，工作距离和放大倍数不应有所改变
}