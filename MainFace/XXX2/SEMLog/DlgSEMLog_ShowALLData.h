#pragma once
#include "afxcmn.h"
#include "Tools/DlgGridPage.h"
#include "DataSet.h"
#include "afxdtctl.h"
#include "Tools/AsynProcessorWithProgress.h"

class CField;
class CFieldDesc;
class CCheckComboBox;

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgSEMLog_ShowALLData : public CDlgGridPage,public CJobProxy
{
	DECLARE_DYNAMIC(CDlgSEMLog_ShowALLData)

public:
	CDlgSEMLog_ShowALLData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSEMLog_ShowALLData();
	void	Localize(void);

	virtual void DoJob(AsynProcessorWithProgress* pProgress);

// �Ի�������
	enum { IDD = IDD_DLG_SEMLog_ShowALLData };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	void _AddItem2MultiSelectCheckBox();

	//������ѡ������
	void _CreateCheckCombox();

	//�ƶ�Grid
	void _MoveGrid();

	//��ƿؼ�����
	void _DesignLayout();

	//��UI�ϵ�ʱ��ת����ʱ���(����),�������ݿ��ѯ
	__int64 _GetTimeStamp(bool bBeginDateTime);

	//��ȡ�����ֶ�
	//strFieldSelect,ui�Ϲ�ѡ���ֶ�����
	//vSelectFieldID, ��������������ֶ�id
	void _GetFieldID4Filter(const string strFieldSelect, vector<string> &vSelectFieldID );

	//����ҳ������
	//nType = 0, ���أ�nType = 1, ǰһҳ��nType = 1, ��һҳ��
	void _LoadPageData(const int nType);

	//��ȡ��Ԫ������
	void GetCellData( CDataSet* pDs,vector<CFieldDesc*>& vColumn,int row, int col, CString &str );

	virtual void OnVirtualMode();


protected:
	afx_msg void OnBnClickedButtonPreviouspage();
	afx_msg void OnBnClickedButtonNextpage();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnLoadPageData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	//��ѡ��
	CCheckComboBox*	m_pMultiSelectCombo;
	//ʱ��ؼ�
	CDateTimeCtrl m_DayBegin;
	CDateTimeCtrl m_TimeBegin;
	CDateTimeCtrl m_DayEnd;
	CDateTimeCtrl m_TimeEnd;

	//�洢�����б�ѡ���ֶε���Ϣ
	vector<CFieldDesc*>       m_vCompareFiled;

	CComboBox m_comboField;
	CComboBox m_comboCompare;
	CFont*    m_pFont;    
	double    m_dCompareValue;
	CString   m_strQueryByCompare;

public:
	CDataSet m_ds;
	CString m_strPageNO;
	int  m_nCurrentPage;  //��ǰҳ��
	int  m_nTotalPage;    //��ҳ��
	const int  m_nRecordCountPerPage;
	AsynProcessorWithProgress m_AsynProcessor;

	//���ݹ���������ʾ����
	map<string,CFieldDesc*>   m_mapFieldID2Desc;
};
