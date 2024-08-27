#pragma once
#include "afxcmn.h"
#include "DataSet.h"
#include "afxdtctl.h"
#include "Tools/DlgGridPage.h"

class CField;
class CFieldDesc;

#if _MSC_VER > 1000
#pragma once
#endif 

//������¼��ѯ�����ùؼ���
class CDlgSEMLog_OperationQueryKeyWordSettings : public CDlgGridPage
{
	DECLARE_DYNAMIC(CDlgSEMLog_OperationQueryKeyWordSettings)

public:
	CDlgSEMLog_OperationQueryKeyWordSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSEMLog_OperationQueryKeyWordSettings();
	void Localize();

// �Ի�������
	enum { IDD = IDD_DLG_SEMLog_OperationKeyWord };

	void SaveSettings();

protected:
	virtual BOOL OnInitDialog();

	//�ƶ�Grid
	void _MoveGrid();

	//��ƿؼ�����
	void _DesignLayout();

	//��������
	void _LoadData(const string& sTableName);

	//���ݹ���������ʾ����
	void ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs );

protected:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
afx_msg void OnBnClickedButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CDataSet m_ds;
	string   m_sKeyWord;
};
