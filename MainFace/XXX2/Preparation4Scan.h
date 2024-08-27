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


	// ����ͼ��ɨ��
	class CLASS_USB_ST_Stop: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param, USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// ����ɨ��
	class CLASS_USB_ST_RasterScan: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// �ѡ��ɨ��
	class CLASS_USB_ST_AreaScan: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// ˮƽ��ɨ��
	class CLASS_USB_ST_LineScan_HORZ: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// ��ֱ��ɨ��
	class CLASS_USB_ST_LineScan_VERT: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// ��ɨ��
	class CLASS_USB_ST_SpotScan: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// ģ������
	class CLASS_USB_ST_AnalogPhoto: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	
	// �̶�ѡ��ɨ��	AutoBC
	class CLASS_USB_ST_AutoBCArea: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};


	// �̶�ѡ��ɨ��	AutoFocus
	class CLASS_USB_ST_AutoFocusArea: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

	// �̶�ѡ��ɨ�� AutoAstigmat
	class CLASS_USB_ST_AutoAstigmatArea: public IPrepare4Scan
	{
	public:
		virtual void Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate);
	};

}

