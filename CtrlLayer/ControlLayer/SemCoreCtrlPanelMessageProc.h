#pragma once
#include <string>
class CLanguageManagerCtrl;

namespace SemCoreCtrlPanelMessageProcessor
{
	class ISemCoreCtrlPanelMessageProc
	{
	public:
		ISemCoreCtrlPanelMessageProc(){};
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos) = 0;
	};


	class ImageReverse: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class DFocus: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class DFocusSwitch: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class DObjectiveLens: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class DObjectiveLensSwitch: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class RobinsonDetectorSwitch: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class Signal2Brightness: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class Signal2Contrast: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class YAdjust: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class ObjectiveLens: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class WorkDistance: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class Magnifier: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class CondenserLens: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class CPS_FilamentCurrent: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class GunCurrent: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class ObjectiveLensReverse: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class CondenserLensReverse: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class DCondenserLens: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class ScanRotate: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class SACP: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class CPS_HV: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class SP_HV: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class HV: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class BeemAlignment: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class ScanShift: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class Stigmator: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class AuxiliaryStigmation: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class Brightness: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
		void _PerformAddtionalTask();
	};


	class Contrast: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class SP_PMT: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};


	class Enum_NULL: public ISemCoreCtrlPanelMessageProc
	{
	public:
		virtual void DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos);
	};

}

