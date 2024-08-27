#pragma once
#include "afxcmn.h"

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgSEMLog_DeleteData : public CDialog
{
	DECLARE_DYNAMIC(CDlgSEMLog_DeleteData)

public:
	CDlgSEMLog_DeleteData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSEMLog_DeleteData();

// �Ի�������
	enum { IDD = IDD_DLG_SEMLog_DeleteData };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDelete();
	DECLARE_MESSAGE_MAP()

protected:
	//��UI�ϵ�ʱ��ת����ʱ���(����),�������ݿ��ѯ
	__int64 _GetTimeStamp(bool bBeginDateTime);

protected:
	//ʱ��ؼ�
	CDateTimeCtrl m_DayBegin;
	CDateTimeCtrl m_TimeBegin;
	CDateTimeCtrl m_DayEnd;
	CDateTimeCtrl m_TimeEnd;
};
