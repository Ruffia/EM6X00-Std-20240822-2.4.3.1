#pragma once
#include "USB_Functions.h"

class CSemCtrlInitializer
{
public:
	static CSemCtrlInitializer& Instance();
	
	void	AutoInitSemCtrl_CPS();
	void	AutoInitSemCtrl_MCI();
	void	AutoInitSemCtrl_SPR();
	void	AutoInitSemCtrl_LNS();
	void	AutoInitSemCtrl_SCN( BOOL bPC9Reset = FALSE );
	// 05.06.22 加入参数以判断是否是PC9复位

	void	AutoSaveSemCtrl();		// 2004-6-2_自动保存并更新控制量

	//从注册表中读取上次退出软件之前保存的设置
	DWORD   ReadSettingFromRegistry(const CString& sSettingSection, const CString& sRegEntry, const DWORD dDefaultValue = 0);

protected:
	CSemCtrlInitializer();


};