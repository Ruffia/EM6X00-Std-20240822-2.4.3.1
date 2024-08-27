#include "stdafx.h"
#include "PanelModeMgr.h"

CPanelModeMgr::CPanelModeMgr()
{
	MagMode = MagnificationMode_Large;
	FocusMode =  FocusControlMode_Large;
	FocusModeBK = FocusControlMode_Large;
	m_nShiftCondeserSwitchMode = 0x01;
	m_nStigmatorApertureSwitchMode = 0x01;
	m_eShiftCondeserSwitchSatusX = ShiftCondeserSwitchStatus::ShiftCondeserSwitchNormal;
	m_eShiftCondeserSwitchSatusY = ShiftCondeserSwitchStatus::ShiftCondeserSwitchNormal;
	m_eStigmatorSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;
	m_eStigmatorSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;
	m_eApertureSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;
	m_eApertureSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;
	m_eElectronBeemTiltSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;
	m_eElectronBeemTiltSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;
	m_nScanSpeedMode = 1;
}

CPanelModeMgr& CPanelModeMgr::Instance()
{
	static CPanelModeMgr mgr;
	return mgr;
}

