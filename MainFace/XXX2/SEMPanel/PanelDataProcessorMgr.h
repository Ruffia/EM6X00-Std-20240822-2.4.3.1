#pragma once
#include <map>
using namespace std;

class IPanelDataProcessor;

//�ֿغ����ݴ����������
class CPanelDataProcessorMgr
{
protected:
	CPanelDataProcessorMgr(void){};
	~CPanelDataProcessorMgr(void){};

public:
	static CPanelDataProcessorMgr& Instance();

	//�ͷŴ���������
	void Release();

	//����16�������ҵ���Ӧ�����ݴ�����
	IPanelDataProcessor* GetProcessor(int nHexCode);

private:
	map<int,IPanelDataProcessor*>  m_mapFunctionCode2Processor;
};