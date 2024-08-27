#pragma once
#include <string>
class CSemCtrlPanelManager;

namespace nsSemCoreCtrlPanelCoadjust
{
	class ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		ISemCoreCtrlPanelCoadjustHandler(){};
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) = 0;
	};

	
	class CLASS_SemControlPanelClass_ObjectiveLens: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Magnifier: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_HV: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_GunCurrent: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CPS_HV: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CPS_Bias: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CPS_FilamentCurrent: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_SP_HV: public ISemCoreCtrlPanelCoadjustHandler
	{
	public:
		virtual void Adjust(CSemCtrlPanelManager* pManager,const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	};

}

