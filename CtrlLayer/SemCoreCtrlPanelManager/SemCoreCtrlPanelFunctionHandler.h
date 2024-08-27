#pragma once
#include <string>
class CSemCoreAssemble;
class CSemCoreCtrlPanelCommonMap;

namespace nsSemCoreCtrlPanel
{
	//该接口表示每一个被 CSemCoreAssemble 管理的控件都有三种基本操作，即SyncPos，SetPos，SyncText
	class ISemCoreCtrlPanelHandler
	{
	public:
		ISemCoreCtrlPanelHandler(){};
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap)
		{};

        virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
		{};

		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag)
		{};
	};


	class CLASS_SemControlPanelClass_Signal2Brightness: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Signal2Contrast: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_YAdjust: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_DFocus: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_DObjectiveLens: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_RobinsonDetectorSwitch: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_DFocusSwitch: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_DObjectiveLensSwitch: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_ObjectiveLens: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_WorkDistance: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_Magnifier: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CondenserLens: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_FilamentCurrent: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_GunCurrent: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_GunCurrentAccelerate: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_ObjectiveLensReverse: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_CondenserLensReverse: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_DCondenserLens: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};



	class CLASS_SemControlPanelClass_ScanRotate: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_SACP: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_HV: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_HVAccelerate: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_BeemAlignment: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_ScanShift: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Stigmator: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_AuxiliaryStigmation: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Brightness: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Contrast: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Channel2AB: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Channel2C: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Channel2D: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_Channel2E: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_BseBySeDetectorSwitch: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CPS_HV: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CPS_Bias: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_CPS_FilamentCurrent: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_SP_HV: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_SP_Bias: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_SP_Heater: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_SP_PMT: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};


	class CLASS_SemControlPanelClass_ImageAlgo_AdjustmentParameter: public ISemCoreCtrlPanelHandler
	{
	public:
		virtual void SyncPos(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap);
		virtual void SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
		virtual void SyncText(CSemCoreCtrlPanelCommonMap*  pSemCoreCtrlPanelMap,const VARIANT FAR& Flag);
	};

}

