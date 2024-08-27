#pragma once

//放大倍数调节模式
enum MagnificationControlMode
{
	MagnificationMode_Large = 0,   //粗调
	MagnificationMode_Small = 1,   //细调
};


//聚焦模式
enum FocusControlMode
{
	FocusControlMode_Large = 0,   //粗调-----〉对应于界面上的细调
	FocusControlMode_Small = 1,   //细调-----〉对应于界面上的微调
	FocusControlMode_ScreenFocus = 2,   //屏幕聚焦调节
};


//按键切换模式
enum StigApertureMode
{
	StigmatorMode = 0x01,       //消像散模式
	ApertureMode,               //对中模式(手控盒面板上的标记)
	FilamentMode,               //灯丝电流模式    
};

//状态
enum ShiftCondeserSwitchStatus
{
	ShiftCondeserSwitchNormal = 0,
	ShiftCondeserSwitching,
};

//状态
enum StigmatorApertureSwitchStatus
{
	StigmatorApertureSwitchNormal = 0,
	StigmatorApertureSwitching,
};

//手控盒工作模式管理器
class CPanelModeMgr
{
protected:
	CPanelModeMgr();
	~CPanelModeMgr(){};

public:
	static CPanelModeMgr& Instance();
	void RestoreFocusMode()
	{
		FocusMode = FocusModeBK;
	}

public:
	MagnificationControlMode MagMode;
	FocusControlMode         FocusMode;                                 //当前聚焦模式
	FocusControlMode         FocusModeBK;                               //之前的聚焦模式(在原有Large，Smalll两种模式的基础上，加入了ScrrenMode),
	                                                                    //当从ScreenMode 模式中退出时，希望还原到原有模式        
	int  m_nShiftCondeserSwitchMode;                                    //电位移，聚光镜切换模式(取值1，2)
	int  m_nStigmatorApertureSwitchMode;                                //消像散，电对中切换模式(取值1，2，3)
	int  m_nScanSpeedMode;                                              //扫描速度模式(取值1，2，3，4)
	ShiftCondeserSwitchStatus      m_eShiftCondeserSwitchSatusX;   
	ShiftCondeserSwitchStatus      m_eShiftCondeserSwitchSatusY;  
	StigmatorApertureSwitchStatus  m_eStigmatorSwitchSatusX;             //消像散X
	StigmatorApertureSwitchStatus  m_eStigmatorSwitchSatusY;             //消像散Y
	StigmatorApertureSwitchStatus  m_eApertureSwitchSatusX;              //电对中X
	StigmatorApertureSwitchStatus  m_eApertureSwitchSatusY;              //电对中Y
	StigmatorApertureSwitchStatus  m_eElectronBeemTiltSwitchSatusX;      //电子束倾斜X
	StigmatorApertureSwitchStatus  m_eElectronBeemTiltSwitchSatusY;      //电子束倾斜Y
};