#include "stdafx.h"
#include "PanelDataProcessor.h"
#include "PanelCoefficientMgr.h"
#include <math.h>
#include "Util.h"


IPanelDataProcessor::IPanelDataProcessor():m_byteFunctionCode(0x00),m_fDivisor(2.f),m_nExec(0)
{
	m_hWnd = NULL;
	ZeroMemory(m_dataPrevious,CmdLen);
	ZeroMemory(m_dataCurrent,CmdLen);
}

void IPanelDataProcessor::SetCurrentData(BYTE dataCurrent[CmdLen])
{
	memcpy(m_dataPrevious,m_dataCurrent,CmdLen);
	memcpy(m_dataCurrent,dataCurrent,CmdLen);
}


long IPanelDataProcessor::_CalcChangeData()
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
		return 0.f;
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


void IPanelDataProcessor::Notify()
{
	const long lValue = _CalcChangeData();
	if (abs(lValue) < 0.01f){
		return;
	}

	_SyncPos(lValue/m_fDivisor);
}