#pragma once
#include "USB_Functions.h"


class AFX_EXT_CLASS CTestParameterManager  
{
public:
	static CTestParameterManager& Instance();

public:
	//图像算法测试参数
	//均值
	bool    m_bEnableAlgoAdjust;   //启用均值算法
	float   m_fAlgoAdjust;         //均值算法调节系数

	//自动功能
	DWORD   m_dwSleep_AutoFocus;   //Step 间隔时间(毫秒)

protected:
	CTestParameterManager();

};
