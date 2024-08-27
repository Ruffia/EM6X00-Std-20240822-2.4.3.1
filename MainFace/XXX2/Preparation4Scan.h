#pragma once
#include "ScanControlStructure.h"
#include "USB_Functions.h"

class CScanView;

namespace Prepare4Scan
{
	class IPrepare4Scan
	{
	public:
		IPrepare4Scan(){};

		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate) = 0;
	};


	// 冻结图象扫描
	class CLASS_USB_ST_Stop: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param, USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 常规扫描
	class CLASS_USB_ST_RasterScan: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 活动选区扫描
	class CLASS_USB_ST_AreaScan: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 水平线扫描
	class CLASS_USB_ST_LineScan_HORZ: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 垂直线扫描
	class CLASS_USB_ST_LineScan_VERT: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 点扫描
	class CLASS_USB_ST_SpotScan: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 模拟照相
	class CLASS_USB_ST_AnalogPhoto: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	
	// 固定选区扫描	AutoBC
	class CLASS_USB_ST_AutoBCArea: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};


	// 固定选区扫描	AutoFocus
	class CLASS_USB_ST_AutoFocusArea: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// 固定选区扫描 AutoAstigmat
	class CLASS_USB_ST_AutoAstigmatArea: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

}

