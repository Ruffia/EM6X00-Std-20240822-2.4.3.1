#pragma once
#include "PanelDataProcessorBase.h"

class PanelDataProcessor_SwitchCode : public IPanelDataProcessor
{
public:
	//����ǰ��ı�ֵ
	virtual long _CalcChangeData();
};