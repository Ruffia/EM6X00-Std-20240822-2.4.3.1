#include "StdAfx.h"
#include "PanelDataProcessorSwitchCode.h"
#include "Util.h"
#include "PanelModeMgr.h"

long PanelDataProcessor_SwitchCode::_CalcChangeData()
{
	long dataPrevious =  0;
	long dataCurrent  =  0;
	long lValue = 0;
	if (IsLittleEndian())
	{
		dataPrevious = m_dataPrevious[Data1] * pow((double)16,0) + m_dataPrevious[Data2] * pow((double)16,2);
		dataCurrent =  m_dataCurrent[Data1]  * pow((double)16,0) + m_dataCurrent[Data2]  * pow((double)16,2);
	}
	else
	{
		dataPrevious = m_dataPrevious[Data2] * pow((double)16,0) + m_dataPrevious[Data1] * pow((double)16,2);
		dataCurrent =  m_dataCurrent[Data2]  * pow((double)16,0) + m_dataCurrent[Data1]  * pow((double)16,2);
	}

	//第一次运行时，数据未初始化，所以做丢弃处理
	if (0 == m_nExec)
	{
		m_nExec++;
		return 0;
	}

	CString strDataPrevious = "";
	CString strDataCurrent = "";
	for (int i = 0; i < CmdLen; i++)
	{
		CString strByte = "";
		strByte.Format("%02X  ",m_dataPrevious[i]);
		strDataPrevious +=  strByte;
		strByte = "";
		strByte.Format("%02X  ",m_dataCurrent[i]);
		strDataCurrent += strByte;
	}

	strDataPrevious += "\r\n";
	strDataCurrent += "\r\n";

	//电位移，聚光镜进行功能键切换时，第一次收到的数据作丢弃处理
	if (m_byteFunctionCode ==  0x01 &&  CPanelModeMgr::Instance().m_eShiftCondeserSwitchSatusX == ShiftCondeserSwitchStatus::ShiftCondeserSwitching)
	{
		CPanelModeMgr::Instance().m_eShiftCondeserSwitchSatusX = ShiftCondeserSwitchStatus::ShiftCondeserSwitchNormal;

		char sz[512] = {0};
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
		OutputDebugString(sz);

		return 0;
	}

	//电位移，聚光镜进行功能键切换时，第一次收到的数据作丢弃处理
	if (m_byteFunctionCode ==  0x02 && CPanelModeMgr::Instance().m_eShiftCondeserSwitchSatusY == ShiftCondeserSwitchStatus::ShiftCondeserSwitching)
	{
		CPanelModeMgr::Instance().m_eShiftCondeserSwitchSatusY = ShiftCondeserSwitchStatus::ShiftCondeserSwitchNormal;

		char sz[512] = {0};
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
		OutputDebugString(sz);

		return 0;
	}

	if(CPanelModeMgr::Instance().m_eStigmatorSwitchSatusX != StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal ||
		CPanelModeMgr::Instance().m_eStigmatorSwitchSatusY != StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal ||
		CPanelModeMgr::Instance().m_eApertureSwitchSatusX != StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal ||
		CPanelModeMgr::Instance().m_eApertureSwitchSatusY != StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal ||
		CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusX != StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal ||
		CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusY != StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal)
	{

		if (0x01 == CPanelModeMgr::Instance().m_nStigmatorApertureSwitchMode)
		{
			//电对中，消像散进行功能键切换时，第一次收到的数据作丢弃处理
			if(CPanelModeMgr::Instance().m_eStigmatorSwitchSatusX == StigmatorApertureSwitchStatus::StigmatorApertureSwitching)
			{
				if (0x0B == m_byteFunctionCode )
				{
					CPanelModeMgr::Instance().m_eStigmatorSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;

					char sz[512] = {0};
					ZeroMemory(sz,512);
					sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
					OutputDebugString(sz);

					return 0;
				}
			}

			if(CPanelModeMgr::Instance().m_eStigmatorSwitchSatusY == StigmatorApertureSwitchStatus::StigmatorApertureSwitching)
			{
				if (0x0C == m_byteFunctionCode )
				{
					CPanelModeMgr::Instance().m_eStigmatorSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;

					char sz[512] = {0};
					ZeroMemory(sz,512);
					sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
					OutputDebugString(sz);

					return 0;
				}
			}
		}
		else if (0x02 == CPanelModeMgr::Instance().m_nStigmatorApertureSwitchMode)
		{
			//电对中，消像散进行功能键切换时，第一次收到的数据作丢弃处理
			if(CPanelModeMgr::Instance().m_eApertureSwitchSatusX == StigmatorApertureSwitchStatus::StigmatorApertureSwitching)
			{
				if (0x0D == m_byteFunctionCode )
				{
					CPanelModeMgr::Instance().m_eApertureSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;

					char sz[512] = {0};
					ZeroMemory(sz,512);
					sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
					OutputDebugString(sz);

					return 0;
				}
			}

			if(CPanelModeMgr::Instance().m_eApertureSwitchSatusY == StigmatorApertureSwitchStatus::StigmatorApertureSwitching)
			{
				if (0x0E == m_byteFunctionCode )
				{
					CPanelModeMgr::Instance().m_eApertureSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;

					char sz[512] = {0};
					ZeroMemory(sz,512);
					sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
					OutputDebugString(sz);

					return 0;
				}
			}
		}
		else if (0x03 == CPanelModeMgr::Instance().m_nStigmatorApertureSwitchMode)
		{
			//电对中，消像散进行功能键切换时，第一次收到的数据作丢弃处理
			if(CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusX == StigmatorApertureSwitchStatus::StigmatorApertureSwitching)
			{
				if (0x0F == m_byteFunctionCode )
				{
					CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;

					char sz[512] = {0};
					ZeroMemory(sz,512);
					sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
					OutputDebugString(sz);

					return 0;
				}
			}

			if(CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusY == StigmatorApertureSwitchStatus::StigmatorApertureSwitching)
			{
				if (0x10 == m_byteFunctionCode )
				{
					CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitchNormal;

					char sz[512] = {0};
					ZeroMemory(sz,512);
					sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData() abandond data, %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
					OutputDebugString(sz);

					return 0;
				}
			}
		}
	}




	if (abs(dataCurrent - dataPrevious) >= lRange)
	{
		const int nMax = 0xFFFF;
		const int nMin = 0x0000;
		if (dataCurrent < dataPrevious)
		{
			lValue = nMax - dataPrevious + dataCurrent - nMin;
		}
		else
		{
			lValue = nMax - dataCurrent + dataPrevious - nMin;
		}	
	}
	else
	{
		lValue = dataCurrent - dataPrevious;
	}

	if (abs(lValue) > 0.01f)
	{
		CString strDataPrevious = "";
		CString strDataCurrent = "";
		for (int i = 0; i < CmdLen; i++)
		{
			CString strByte = "";
			strByte.Format("%02X  ",m_dataPrevious[i]);
			strDataPrevious +=  strByte;
			strByte = "";
			strByte.Format("%02X  ",m_dataCurrent[i]);
			strDataCurrent += strByte;
		}

		strDataPrevious += "\r\n";
		strDataCurrent += "\r\n";

		char sz[512] = {0};
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData(), %s  %s  lValue = %d \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent,lValue);
		OutputDebugString(sz);
	}

	return lValue;
}
