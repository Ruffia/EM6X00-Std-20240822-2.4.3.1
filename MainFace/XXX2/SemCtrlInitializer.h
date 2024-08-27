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
	// 05.06.22 ����������ж��Ƿ���PC9��λ

	void	AutoSaveSemCtrl();		// 2004-6-2_�Զ����沢���¿�����

	//��ע����ж�ȡ�ϴ��˳����֮ǰ���������
	DWORD   ReadSettingFromRegistry(const CString& sSettingSection, const CString& sRegEntry, const DWORD dDefaultValue = 0);

protected:
	CSemCtrlInitializer();


};