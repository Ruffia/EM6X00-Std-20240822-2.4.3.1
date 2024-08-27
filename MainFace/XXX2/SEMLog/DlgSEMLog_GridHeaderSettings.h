#pragma once
#include "afxcmn.h"
#include "Tools/DlgGridPage.h"
#include "DataSet.h"
#include "afxdtctl.h"

class CField;
class CFieldDesc;
class CCheckComboBox;

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgSEMLog_GridHeaderSettings : public CDlgGridPage
{
	DECLARE_DYNAMIC(CDlgSEMLog_GridHeaderSettings)

public:
	CDlgSEMLog_GridHeaderSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSEMLog_GridHeaderSettings();

// �Ի�������
	enum { IDD = IDD_DLG_Grid };

protected:
	virtual BOOL OnInitDialog();

	//�ƶ�Grid
	void _MoveGrid();

	//��ƿؼ�����
	void _DesignLayout();

	//����ҳ������
	void _LoadPageData();

	//���ݹ���������ʾ����
	void ShowData(const vector<tuple<int,string,string>>& vHeader);

	//��ȡ��Ԫ������
	void GetCellData( const vector<tuple<int,string,string>>& vHeader,const int row, const int col, CString &str );

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnLoadPageData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CDataSet m_ds;
};
