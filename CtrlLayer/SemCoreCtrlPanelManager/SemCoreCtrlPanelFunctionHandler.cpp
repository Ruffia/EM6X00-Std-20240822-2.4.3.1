#include "StdAfx.h"
#include "SemCoreCtrlPanelFunctionHandler.h"
using namespace nsSemCoreCtrlPanel;
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelCommonMap.h"
#include "Factory.h"

const DWORD	FlagIgnoreFlush = (DWORD)0x04;

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Signal2Brightness,SemControlPanelClassID,SemControlPanelClass_Signal2Brightness)
void CLASS_SemControlPanelClass_Signal2Brightness::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Signal2Brightness.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Signal2Brightness.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );			
}


void CLASS_SemControlPanelClass_Signal2Brightness::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Signal2Brightness.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Signal2Brightness.Flush();
}


void CLASS_SemControlPanelClass_Signal2Brightness::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Signal2Brightness.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Signal2Contrast,SemControlPanelClassID,SemControlPanelClass_Signal2Contrast)
void CLASS_SemControlPanelClass_Signal2Contrast::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Signal2Contrast.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Signal2Contrast.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Signal2Contrast::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Signal2Contrast.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Signal2Contrast.Flush();
}


void CLASS_SemControlPanelClass_Signal2Contrast::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Signal2Contrast.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_YAdjust,SemControlPanelClassID,SemControlPanelClass_YAdjust)
void CLASS_SemControlPanelClass_YAdjust::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_YAdjust.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_YAdjust.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_YAdjust::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_YAdjust.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_YAdjust.Flush();
}


void CLASS_SemControlPanelClass_YAdjust::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_YAdjust.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_DFocus,SemControlPanelClassID,SemControlPanelClass_DFocus)
void CLASS_SemControlPanelClass_DFocus::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_DFocus.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_DFocus.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_DFocus::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_DFocus.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_DFocus.Flush();
}


void CLASS_SemControlPanelClass_DFocus::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_DFocus.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );

}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_DObjectiveLens,SemControlPanelClassID,SemControlPanelClass_DObjectiveLens)
void CLASS_SemControlPanelClass_DObjectiveLens::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_DObjectiveLens.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_DObjectiveLens.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_DObjectiveLens::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_DObjectiveLens.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_DObjectiveLens.Flush();
}


void CLASS_SemControlPanelClass_DObjectiveLens::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_DObjectiveLens.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_RobinsonDetectorSwitch,SemControlPanelClassID,SemControlPanelClass_RobinsonDetectorSwitch)
void CLASS_SemControlPanelClass_RobinsonDetectorSwitch::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_RobinsonDetectorSwitch.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_RobinsonDetectorSwitch.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_RobinsonDetectorSwitch::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_RobinsonDetectorSwitch.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_RobinsonDetectorSwitch.Flush();
}


void CLASS_SemControlPanelClass_RobinsonDetectorSwitch::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_RobinsonDetectorSwitch.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_DFocusSwitch,SemControlPanelClassID,SemControlPanelClass_DFocusSwitch)
void CLASS_SemControlPanelClass_DFocusSwitch::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	Xpos = CSemCoreAssemble::Instance().m_DFocus.GetSwitch();
	Xmin.lVal = 0;
	Xmax.lVal = 1;
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_DFocusSwitch::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_DFocus.SetSwitch( Xpos );
	CSemCoreAssemble::Instance().m_DFocus.Flush();
}


void CLASS_SemControlPanelClass_DFocusSwitch::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_DFocus.FormatSwitchText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_DObjectiveLensSwitch,SemControlPanelClassID,SemControlPanelClass_DObjectiveLensSwitch)
void CLASS_SemControlPanelClass_DObjectiveLensSwitch::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	Xpos = CSemCoreAssemble::Instance().m_DObjectiveLens.GetSwitch();
	Xmin.lVal = 0;
	Xmax.lVal = 1;
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_DObjectiveLensSwitch::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_DObjectiveLens.SetSwitch( Xpos );
	CSemCoreAssemble::Instance().m_DObjectiveLens.Flush();
}


void CLASS_SemControlPanelClass_DObjectiveLensSwitch::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_DObjectiveLens.FormatSwitchText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_ObjectiveLens,SemControlPanelClassID,SemControlPanelClass_ObjectiveLens)
void CLASS_SemControlPanelClass_ObjectiveLens::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_ObjectiveLens.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_ObjectiveLens::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_ObjectiveLens.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_ObjectiveLens.Flush();
}



void CLASS_SemControlPanelClass_ObjectiveLens::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_WorkDistance,SemControlPanelClassID,SemControlPanelClass_WorkDistance)
void CLASS_SemControlPanelClass_WorkDistance::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_WD.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_WD.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_WorkDistance::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	//m_CSemCoreAssemble::Instance().m_WD.SetPos( Xpos );
}


void CLASS_SemControlPanelClass_WorkDistance::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_WD.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Magnifier,SemControlPanelClassID,SemControlPanelClass_Magnifier)
void CLASS_SemControlPanelClass_Magnifier::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Magnifier.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Magnifier::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Magnifier.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Magnifier.Flush();
}


void CLASS_SemControlPanelClass_Magnifier::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Magnifier.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_CondenserLens,SemControlPanelClassID,SemControlPanelClass_CondenserLens)
void CLASS_SemControlPanelClass_CondenserLens::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_CondenserLens.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_CondenserLens::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_CondenserLens.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_CondenserLens.Flush();
}



void CLASS_SemControlPanelClass_CondenserLens::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_CondenserLens.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_FilamentCurrent,SemControlPanelClassID,SemControlPanelClass_FilamentCurrent)
void CLASS_SemControlPanelClass_FilamentCurrent::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_FilamentCurrent.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_FilamentCurrent.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_FilamentCurrent::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_FilamentCurrent.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_FilamentCurrent.Flush();
}


void CLASS_SemControlPanelClass_FilamentCurrent::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_FilamentCurrent.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_GunCurrent,SemControlPanelClassID,SemControlPanelClass_GunCurrent)
void CLASS_SemControlPanelClass_GunCurrent::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_GunCurrent.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_GunCurrent.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_GunCurrent::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_GunCurrent.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_GunCurrent.Flush();
}


void CLASS_SemControlPanelClass_GunCurrent::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_GunCurrent.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_GunCurrentAccelerate,SemControlPanelClassID,SemControlPanelClass_GunCurrentAccelerate)
void CLASS_SemControlPanelClass_GunCurrentAccelerate::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_GunCurrentAccelerate.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_GunCurrentAccelerate.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_GunCurrentAccelerate::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{

}


void CLASS_SemControlPanelClass_GunCurrentAccelerate::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_GunCurrentAccelerate.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_ObjectiveLensReverse,SemControlPanelClassID,SemControlPanelClass_ObjectiveLensReverse)
void CLASS_SemControlPanelClass_ObjectiveLensReverse::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_ObjectiveLensReverse.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_ObjectiveLensReverse.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_ObjectiveLensReverse::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_ObjectiveLensReverse.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_ObjectiveLensReverse.Flush();
}


void CLASS_SemControlPanelClass_ObjectiveLensReverse::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_ObjectiveLensReverse.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_CondenserLensReverse,SemControlPanelClassID,SemControlPanelClass_CondenserLensReverse)
void CLASS_SemControlPanelClass_CondenserLensReverse::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_CondenserLensReverse.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_CondenserLensReverse.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_CondenserLensReverse::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_CondenserLensReverse.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_CondenserLensReverse.Flush();
}


void CLASS_SemControlPanelClass_CondenserLensReverse::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_CondenserLensReverse.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_DCondenserLens,SemControlPanelClassID,SemControlPanelClass_DCondenserLens)
void CLASS_SemControlPanelClass_DCondenserLens::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_DCondenserLens.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_DCondenserLens.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_DCondenserLens::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_DCondenserLens.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_DCondenserLens.Flush();
}


void CLASS_SemControlPanelClass_DCondenserLens::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_DCondenserLens.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_ScanRotate,SemControlPanelClassID,SemControlPanelClass_ScanRotate)
void CLASS_SemControlPanelClass_ScanRotate::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_ScanRotate.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_ScanRotate.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_ScanRotate::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_ScanRotate.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_ScanRotate.Flush();
}


void CLASS_SemControlPanelClass_ScanRotate::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_ScanRotate.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_SACP,SemControlPanelClassID,SemControlPanelClass_SACP)
void CLASS_SemControlPanelClass_SACP::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_SACP.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_SACP.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_SACP::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_SACP.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_SACP.Flush();
}


void CLASS_SemControlPanelClass_SACP::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SACP.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_HV,SemControlPanelClassID,SemControlPanelClass_HV)
void CLASS_SemControlPanelClass_HV::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_HV.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_HV.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_HV::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_HV.SetPos( Xpos );
	//		CSemCoreAssemble::Instance().m_HV.Flush();
}


void CLASS_SemControlPanelClass_HV::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_HV.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_HVAccelerate,SemControlPanelClassID,SemControlPanelClass_HVAccelerate)
void CLASS_SemControlPanelClass_HVAccelerate::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_HVAccelerate.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_HVAccelerate.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_HVAccelerate::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{

}


void CLASS_SemControlPanelClass_HVAccelerate::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_HVAccelerate.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_BeemAlignment,SemControlPanelClassID,SemControlPanelClass_BeemAlignment)
void CLASS_SemControlPanelClass_BeemAlignment::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax, Ypos;
	CSemCoreAssemble::Instance().m_BeemAlignmentX.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
	CSemCoreAssemble::Instance().m_BeemAlignmentY.GetRange( Ymin, Ymax );
	Ypos = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();

	Flag.ulVal = FlagCtrlX | FlagCtrlY;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_BeemAlignment::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
	{
		CSemCoreAssemble::Instance().m_BeemAlignmentX.SetPos( Xpos );
		CSemCoreAssemble::Instance().m_BeemAlignmentX.Flush();
	}
	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
	{
		CSemCoreAssemble::Instance().m_BeemAlignmentY.SetPos( Ypos );
		CSemCoreAssemble::Instance().m_BeemAlignmentY.Flush();
	}
}


void CLASS_SemControlPanelClass_BeemAlignment::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = CSemCoreAssemble::Instance().m_BeemAlignmentX.FormatText();
	csY = CSemCoreAssemble::Instance().m_BeemAlignmentY.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_ScanShift,SemControlPanelClassID,SemControlPanelClass_ScanShift)
void CLASS_SemControlPanelClass_ScanShift::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax, Ypos;
	CSemCoreAssemble::Instance().m_ScanShiftX.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
	CSemCoreAssemble::Instance().m_ScanShiftY.GetRange( Ymin, Ymax );
	Ypos = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();

	Flag.ulVal = FlagCtrlX | FlagCtrlY;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_ScanShift::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
	{
		CSemCoreAssemble::Instance().m_ScanShiftX.SetPos( Xpos );
		CSemCoreAssemble::Instance().m_ScanShiftX.Flush();
	}
	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
	{
		CSemCoreAssemble::Instance().m_ScanShiftY.SetPos( Ypos );
		CSemCoreAssemble::Instance().m_ScanShiftY.Flush();
	}
}


void CLASS_SemControlPanelClass_ScanShift::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = CSemCoreAssemble::Instance().m_ScanShiftX.FormatText();
	csY = CSemCoreAssemble::Instance().m_ScanShiftY.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Stigmator,SemControlPanelClassID,SemControlPanelClass_Stigmator)
void CLASS_SemControlPanelClass_Stigmator::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax, Ypos;
	CSemCoreAssemble::Instance().m_StigmatorX.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
	CSemCoreAssemble::Instance().m_StigmatorY.GetRange( Ymin, Ymax );
	Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();

	Flag.ulVal = FlagCtrlX | FlagCtrlY;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Stigmator::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
	{
		CSemCoreAssemble::Instance().m_StigmatorX.SetPos( Xpos );
		CSemCoreAssemble::Instance().m_StigmatorX.Flush();
	}
	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
	{
		CSemCoreAssemble::Instance().m_StigmatorY.SetPos( Ypos );
		CSemCoreAssemble::Instance().m_StigmatorY.Flush();
	}
}


void CLASS_SemControlPanelClass_Stigmator::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = CSemCoreAssemble::Instance().m_StigmatorX.FormatText();
	csY = CSemCoreAssemble::Instance().m_StigmatorY.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_AuxiliaryStigmation,SemControlPanelClassID,SemControlPanelClass_AuxiliaryStigmation)
void CLASS_SemControlPanelClass_AuxiliaryStigmation::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_AuxiliaryStigmation.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_AuxiliaryStigmation.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_AuxiliaryStigmation::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_AuxiliaryStigmation.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_AuxiliaryStigmation.Flush();
}


void CLASS_SemControlPanelClass_AuxiliaryStigmation::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_AuxiliaryStigmation.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Brightness,SemControlPanelClassID,SemControlPanelClass_Brightness)
void CLASS_SemControlPanelClass_Brightness::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Brightness.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Brightness.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Brightness::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Brightness.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Brightness.Flush();
}


void CLASS_SemControlPanelClass_Brightness::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Brightness.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Contrast,SemControlPanelClassID,SemControlPanelClass_Contrast)
void CLASS_SemControlPanelClass_Contrast::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Contrast.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Contrast.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Contrast::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Contrast.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Contrast.Flush();
}


void CLASS_SemControlPanelClass_Contrast::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Contrast.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Channel2AB,SemControlPanelClassID,SemControlPanelClass_Channel2AB)
void CLASS_SemControlPanelClass_Channel2AB::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Channel2AB.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Channel2AB.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Channel2AB::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Channel2AB.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Channel2AB.Flush();
}



void CLASS_SemControlPanelClass_Channel2AB::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Channel2AB.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Channel2C,SemControlPanelClassID,SemControlPanelClass_Channel2C)
void CLASS_SemControlPanelClass_Channel2C::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Channel2C.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Channel2C.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Channel2C::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Channel2C.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Channel2C.Flush();
}


void CLASS_SemControlPanelClass_Channel2C::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Channel2C.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Channel2D,SemControlPanelClassID,SemControlPanelClass_Channel2D)
void CLASS_SemControlPanelClass_Channel2D::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Channel2D.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Channel2D.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Channel2D::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Channel2D.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Channel2D.Flush();
}



void CLASS_SemControlPanelClass_Channel2D::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Channel2D.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_Channel2E,SemControlPanelClassID,SemControlPanelClass_Channel2E)
void CLASS_SemControlPanelClass_Channel2E::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_Channel2E.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_Channel2E.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_Channel2E::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_Channel2E.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_Channel2E.Flush();
}



void CLASS_SemControlPanelClass_Channel2E::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Channel2E.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_BseBySeDetectorSwitch,SemControlPanelClassID,SemControlPanelClass_BseBySeDetectorSwitch)
void CLASS_SemControlPanelClass_BseBySeDetectorSwitch::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_BseBySeDetectorSwitch.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_BseBySeDetectorSwitch.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_BseBySeDetectorSwitch::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_BseBySeDetectorSwitch.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_BseBySeDetectorSwitch.Flush();
}


void CLASS_SemControlPanelClass_BseBySeDetectorSwitch::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_BseBySeDetectorSwitch.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}




IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_CPS_HV,SemControlPanelClassID,SemControlPanelClass_CPS_HV)
void CLASS_SemControlPanelClass_CPS_HV::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	// CPS 3601
	CSemCoreAssemble::Instance().m_CPS_HV.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_CPS_HV.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_CPS_HV::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	// CPS 3601
	CSemCoreAssemble::Instance().m_CPS_HV.SetPos( Xpos );
	//		CSemCoreAssemble::Instance().m_CPS_HV.Flush();
}


void CLASS_SemControlPanelClass_CPS_HV::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	// CPS 3601
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_CPS_Bias,SemControlPanelClassID,SemControlPanelClass_CPS_Bias)
void CLASS_SemControlPanelClass_CPS_Bias::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_CPS_Bias.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_CPS_Bias.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_CPS_Bias::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_CPS_Bias.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_CPS_Bias.Flush();
}


void CLASS_SemControlPanelClass_CPS_Bias::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_CPS_Bias.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_CPS_FilamentCurrent,SemControlPanelClassID,SemControlPanelClass_CPS_FilamentCurrent)
void CLASS_SemControlPanelClass_CPS_FilamentCurrent::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_CPS_FilamentCurrent::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.SetPos( Xpos );
	CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.Flush();
}



void CLASS_SemControlPanelClass_CPS_FilamentCurrent::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_SP_HV,SemControlPanelClassID,SemControlPanelClass_SP_HV)
void CLASS_SemControlPanelClass_SP_HV::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	// Spellman EBM30N12/771
	CSemCoreAssemble::Instance().m_SP_HV.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_SP_HV.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}



void CLASS_SemControlPanelClass_SP_HV::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	// Spellman EBM30N12/771
	CSemCoreAssemble::Instance().m_SP_HV.SetPos( Xpos );
}


void CLASS_SemControlPanelClass_SP_HV::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	// Spellman EBM30N12/771
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SP_HV.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_SP_Bias,SemControlPanelClassID,SemControlPanelClass_SP_Bias)
void CLASS_SemControlPanelClass_SP_Bias::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_SP_Bias.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_SP_Bias.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_SP_Bias::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_SP_Bias.SetPos( Xpos );
}


void CLASS_SemControlPanelClass_SP_Bias::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SP_Bias.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_SP_Heater,SemControlPanelClassID,SemControlPanelClass_SP_Heater)
void CLASS_SemControlPanelClass_SP_Heater::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_SP_Heater.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_SP_Heater.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_SP_Heater::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_SP_Heater.SetPos( Xpos );
}


void CLASS_SemControlPanelClass_SP_Heater::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SP_Heater.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}

IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_SP_PMT,SemControlPanelClassID,SemControlPanelClass_SP_PMT)
void CLASS_SemControlPanelClass_SP_PMT::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_SP_PMT.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_SP_PMT.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_SP_PMT::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_SP_PMT.SetPos( Xpos );
}


void CLASS_SemControlPanelClass_SP_PMT::SyncText(CSemCoreCtrlPanelCommonMap*  pPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
	pPanelMap->SyncText( csX, csY, Flag );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelHandler,CLASS_SemControlPanelClass_ImageAlgo_AdjustmentParameter,SemControlPanelClassID,SemControlPanelClass_ImageAlgo_AdjustmentParameter)
void CLASS_SemControlPanelClass_ImageAlgo_AdjustmentParameter::SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
{
	VARIANT  Xmin, Xmax, Xpos, Flag;
	VARIANT  Ymin, Ymax;
	CSemCoreAssemble::Instance().m_SemCtrl_AdjustmentParameter.GetRange( Xmin, Xmax );
	Xpos = CSemCoreAssemble::Instance().m_SemCtrl_AdjustmentParameter.GetPos();
	Ymin = Xmin;
	Ymax = Xmax;

	Flag.ulVal = FlagCtrlX;

	pSemCoreCtrlPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
	pSemCoreCtrlPanelMap->SyncPos( Xpos, Xpos, Flag );
}


void CLASS_SemControlPanelClass_ImageAlgo_AdjustmentParameter::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCoreAssemble::Instance().m_SemCtrl_AdjustmentParameter.SetPos( Xpos );
}

void CLASS_SemControlPanelClass_ImageAlgo_AdjustmentParameter::SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag)
{
	CString csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SemCtrl_AdjustmentParameter.FormatText();
	pSemCoreCtrlPanelMap->SyncText( csX, csY, Flag );
}
