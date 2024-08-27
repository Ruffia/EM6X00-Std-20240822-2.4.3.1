#pragma once
#include <string>
class CScanView;

namespace ScanViewTimerMessageHandler
{
	class IScanViewTimerMessageHandler
	{
	public:
		IScanViewTimerMessageHandler(){};
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy) = 0;
	};


	class CLASS_ScanViewTimerMessageHandler_110: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_31: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_37: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_100: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_131: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_132: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_133: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_134: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_135: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_136: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_137: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_138: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_150: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};


	class CLASS_ScanViewTimerMessageHandler_121: public IScanViewTimerMessageHandler
	{
	public:
		virtual void HandleMessage(CScanView* pScanView,BOOL& boolBusy);
	};
}

