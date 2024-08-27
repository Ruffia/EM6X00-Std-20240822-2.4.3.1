#pragma once
#include "PanelDataProcessorBase.h"
#include "PanelDataProcessorSwitchCode.h"

//电位移X
class PanelDataProcessor_ScanShift_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ScanShift_X();
	virtual void _SyncPos(long lValue);
};

//电位移Y
class PanelDataProcessor_ScanShift_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ScanShift_Y();
	virtual void _SyncPos(long lValue);
};


//F1快捷键
class PanelDataProcessor_F1 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_F1();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//F2快捷键
class PanelDataProcessor_F2 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_F2();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//F3快捷键
class PanelDataProcessor_F3 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_F3();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//S1快捷键
class PanelDataProcessor_S1 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_S1();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//S2快捷键
class PanelDataProcessor_S2 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_S2();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//S3快捷键
class PanelDataProcessor_S3 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_S3();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//消像散X
class PanelDataProcessor_Stigmator_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_Stigmator_X();
	virtual void _SyncPos(long lValue);
};

//消像散Y
class PanelDataProcessor_Stigmator_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_Stigmator_Y();
	virtual void _SyncPos(long lValue);
};


//电对中X
class PanelDataProcessor_BeemAlignment_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_BeemAlignment_X();
	virtual void _SyncPos(long lValue);
};

//电对中Y
class PanelDataProcessor_BeemAlignment_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_BeemAlignment_Y();
	virtual void _SyncPos(long lValue);
};


//电子束倾斜X
class PanelDataProcessor_ElectronBeemTilt_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ElectronBeemTilt_X();
	virtual void _SyncPos(long lValue);
};

//电子束倾斜Y
class PanelDataProcessor_ElectronBeemTilt_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ElectronBeemTilt_Y();
	virtual void _SyncPos(long lValue);
};


//放大倍数粗调细调
class PanelDataProcessor_MagnificationMode : public IPanelDataProcessor
{
public:
	PanelDataProcessor_MagnificationMode();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};

//放大倍数调节
class PanelDataProcessor_Magnifier : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Magnifier();
	virtual void _SyncPos(long lValue);
};


//自动亮度对比度
class PanelDataProcessor_AutoBrightContrast : public IPanelDataProcessor
{
public:
	PanelDataProcessor_AutoBrightContrast();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//亮度调节
class PanelDataProcessor_Brightness : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Brightness();
	virtual void _SyncPos(long lValue);
};


//对比度调节
class PanelDataProcessor_Contrast : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Contrast();
	virtual void _SyncPos(long lValue);
};


//聚焦功能粗调细调
class PanelDataProcessor_FocusMode : public IPanelDataProcessor
{
public:
	PanelDataProcessor_FocusMode();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//自动聚焦
class PanelDataProcessor_AutoFocus : public IPanelDataProcessor
{
public:
	PanelDataProcessor_AutoFocus();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//聚焦调节
class PanelDataProcessor_Focus : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Focus();
	virtual void _SyncPos(long lValue);
};


//扫描速度-
class PanelDataProcessor_ScanSpeedMinus : public IPanelDataProcessor
{
public:
	PanelDataProcessor_ScanSpeedMinus();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//扫描速度+
class PanelDataProcessor_ScanSpeedAdd : public IPanelDataProcessor
{
public:
	PanelDataProcessor_ScanSpeedAdd();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//消像散，电对中功能切换
class PanelDataProcessor_StigApertureModeSwitch : public IPanelDataProcessor
{
public:
	PanelDataProcessor_StigApertureModeSwitch();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};



//电位移，聚光镜功能切换
class PanelDataProcessor_ShiftCondeserModeSwitch : public IPanelDataProcessor
{
public:
	PanelDataProcessor_ShiftCondeserModeSwitch();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};