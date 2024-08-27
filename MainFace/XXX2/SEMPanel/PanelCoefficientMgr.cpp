#include "stdafx.h"
#include "PanelCoefficientMgr.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
using namespace pugi;
#include "PanelDataProcessorMgr.h"
#include "PanelDataProcessorBase.h"

FunctionCoefficient::FunctionCoefficient(int nID,string sName,float fValue):
	m_nFunctionID(nID),m_strName(sName),m_fDivisor(fValue)
{}


FunctionCoefficient::FunctionCoefficient()
{
	m_nFunctionID = 0x00;
	m_strName = "";
	m_fDivisor = 1.f;
}

CPanelCoefficientMgr::CPanelCoefficientMgr():
	m_fLargeFocusMode(1.f),m_fSmallFocusMode(1.f)
{

}

CPanelCoefficientMgr& CPanelCoefficientMgr::Instance()
{
	static CPanelCoefficientMgr mgr;
	return mgr;
}


bool CPanelCoefficientMgr::LoadSettings()
{
	char exeFullPath[255] = {0};
	GetModuleFileNameA(NULL,exeFullPath,MAX_PATH);
	char path[255] = {0};
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';

	sprintf_s(path,255,"%s\\%s",path,"ControlPanelSettings.xml");

	string sFilename(path);

	DWORD dwErr = 0;
	xml_document doc;
	if (!doc.load_file(sFilename.c_str())) { //加载xml文件
		dwErr = GetLastError();
		return false;
	}

	xml_node root = doc.child("root");  //根节点
	for (xml_node node = root.first_child();node != root.last_child(); node = node.next_sibling())
	{
		const int nID = node.attribute("FunctionID").as_int(0);
		const string sName = node.attribute("Name").as_string("");
		const float fValue = node.attribute("Coefficient").as_double(0.0);
		FunctionCoefficient obj(nID,sName,fValue);
		m_ID2Coefficient[nID] = obj;

		//加载完数据之后，为对应的处理器设置灵敏系数
		IPanelDataProcessor* Processor = CPanelDataProcessorMgr::Instance().GetProcessor(nID);
		if (Processor)
		{
			Processor->SetDivisor(fValue);
		}		
	}

	xml_node node = root.last_child();
	{
		const int nID = node.attribute("FunctionID").as_int(0);
		const string sName = node.attribute("Name").as_string("");
		const float fValue = node.attribute("Coefficient").as_double(0.0);
		FunctionCoefficient obj(nID,sName,fValue);
		m_ID2Coefficient[nID] = obj;
		IPanelDataProcessor* Processor = CPanelDataProcessorMgr::Instance().GetProcessor(nID);
		if (Processor)
		{
			Processor->SetDivisor(fValue);
		}	

		//加载聚焦粗调，细调的调节值
		const float fLargeValue = node.attribute("LargeMode").as_double(0.0);
		m_fLargeFocusMode = fLargeValue;
		const float fSmallValue = node.attribute("SmallMode").as_double(0.0);
		m_fSmallFocusMode = fSmallValue;
	}

	return true;
}


float CPanelCoefficientMgr::GetDivisor(const int nFunctionCode)
{
	map<int,FunctionCoefficient>::iterator it = m_ID2Coefficient.find(nFunctionCode);
	if (m_ID2Coefficient.end() == it)
	{
		return 1.f;
	}

	FunctionCoefficient& obj = it->second;
	return obj.m_fDivisor;
}