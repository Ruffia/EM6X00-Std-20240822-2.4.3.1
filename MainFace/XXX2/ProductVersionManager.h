#pragma once
#include <afxtempl.h>
#include "LanguageManagerCtrl.h"

//修改时间：  2020-07-21
//修改人：  周宏
//修改原因：重构，将版本管理功能从CXXX2App 中独立出来，
//          减少CXXX2App 的复杂度
//功能： 版本 验证/管理 模块

class CProductVersionManager  
{

public:
	static CProductVersionManager& Instance();
	
	void Initialize(CLanguageManagerCtrl& LanguageCtrl);

	void	Trial_Encryption( char* cTimeSrc, char* cTimeCvt );
	BOOL	Trial_Decryption( char* cTimeCvt, char* cTimeSrc );
	BOOL	Trial_GetFirstTime();
	void	Trial_SaveTime( BOOL bRegister );
	int		Trial_CheckValid( char* cTrialCvt, char* cTimeCvt );
	int		m_nTrialTime;
	BOOL	m_bTrial_NormalUse;	// 是否可以正常使用
	/////////////////////////////////////////////
	WORD	GetVersionInfo();

protected:
	CProductVersionManager();

private:
	CLanguageManagerCtrl* m_pLanguageManagerCtrl;

};

