#pragma once
#include "USB_Functions.h"
#include "SemCtrl.h"

class CSemVirtualMCICard;


//修改时间：  2020-08-25
//修改人：  周宏
//修改原因：重构，将原先在 CXXX2App 中，但是可以不依赖theApp 的功能从CXXX2App 中独立出来
//功能： App 全局函数管理器

class CGlobalFunctionManager  
{

public:
	static CGlobalFunctionManager& Instance();

	void  UpdateScreenMag(CSemVirtualMCICard ImageCard, double dMag, BOOL bSync = FALSE );

protected:
	CGlobalFunctionManager();

};
