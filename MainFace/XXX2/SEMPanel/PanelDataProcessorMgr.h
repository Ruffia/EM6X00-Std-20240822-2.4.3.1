#pragma once
#include <map>
using namespace std;

class IPanelDataProcessor;

//手控盒数据处理器管理机
class CPanelDataProcessorMgr
{
protected:
	CPanelDataProcessorMgr(void){};
	~CPanelDataProcessorMgr(void){};

public:
	static CPanelDataProcessorMgr& Instance();

	//释放处理器集合
	void Release();

	//根据16进制码找到对应的数据处理器
	IPanelDataProcessor* GetProcessor(int nHexCode);

private:
	map<int,IPanelDataProcessor*>  m_mapFunctionCode2Processor;
};