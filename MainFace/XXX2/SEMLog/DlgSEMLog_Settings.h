#pragma once
#include "afxcmn.h"
#include "Tools/DlgTab.h"

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgSEMLog_Settings : public CDlgTab
{
	DECLARE_DYNAMIC(CDlgSEMLog_Settings)

public:
	CDlgSEMLog_Settings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual void InitPage(); 
	void	Localize(void);

	// �Ի�������
	enum { IDD = IDD_DLG_Tab };

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


