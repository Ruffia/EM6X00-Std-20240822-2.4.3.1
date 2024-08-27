#pragma once
#include "PanelDataProcessorBase.h"

class PanelDataProcessor_SwitchCode : public IPanelDataProcessor
{
public:
	//计算前后改变值
	virtual long _CalcChangeData();
};