#include "StdAfx.h"
#include "Factory.h"
#include "PanelDataProcessorMgr.h"
#include "PanelDataProcessorBase.h"

CPanelDataProcessorMgr& CPanelDataProcessorMgr::Instance()
{
	static CPanelDataProcessorMgr mgr;
	return mgr;
}

void CPanelDataProcessorMgr::Release()
{
	for (map<int,IPanelDataProcessor*>::iterator it = m_mapFunctionCode2Processor.begin();
		it != m_mapFunctionCode2Processor.end();it++)
	{
		IPanelDataProcessor* Processor = it->second;
		if (Processor)
		{
			delete Processor;
			Processor = NULL;
		}
	}
	m_mapFunctionCode2Processor.clear();

}


IPanelDataProcessor* CPanelDataProcessorMgr::GetProcessor(int nHexCode)
{
	map<int,IPanelDataProcessor*>::iterator it = m_mapFunctionCode2Processor.find(nHexCode);
	if (it != m_mapFunctionCode2Processor.end())
	{
		return it->second;
	}
	else
	{
		IPanelDataProcessor* Processor = Factory<IPanelDataProcessor,int>::Instance().BuildProduct(nHexCode);
		if (Processor)
		{
			m_mapFunctionCode2Processor[nHexCode] = Processor;
		}
		return Processor;
	}
}