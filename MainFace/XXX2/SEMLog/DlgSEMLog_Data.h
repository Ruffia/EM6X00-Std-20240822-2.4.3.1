#pragma once
#include "afxcmn.h"
#include "Tools/DlgTab.h"

class CDlgSEMLog_Data : public CDlgTab
{
	DECLARE_DYNAMIC(CDlgSEMLog_Data)

public:
	CDlgSEMLog_Data(CWnd* pParent = NULL);   // ��׼���캯��

	virtual void InitPage(); 

// �Ի�������
	enum { IDD = IDD_DLG_Tab };

protected:

	DECLARE_MESSAGE_MAP()
};
