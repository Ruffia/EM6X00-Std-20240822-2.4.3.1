#pragma once
#include <afxtempl.h>
#include "LanguageManagerCtrl.h"

//�޸�ʱ�䣺  2020-07-21
//�޸��ˣ�  �ܺ�
//�޸�ԭ���ع������汾�����ܴ�CXXX2App �ж���������
//          ����CXXX2App �ĸ��Ӷ�
//���ܣ� �汾 ��֤/���� ģ��

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
	BOOL	m_bTrial_NormalUse;	// �Ƿ��������ʹ��
	/////////////////////////////////////////////
	WORD	GetVersionInfo();

protected:
	CProductVersionManager();

private:
	CLanguageManagerCtrl* m_pLanguageManagerCtrl;

};

