
// AutoImageDlg.h : ͷ�ļ�
//

#pragma once
#include <map>
using namespace std;
class CFillButton;


// CAutoImageDlg �Ի���
class CAutoImageDlg : public CDialog
{
// ����
public:
	CAutoImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

	virtual ~CAutoImageDlg();

// �Ի�������
	enum { IDD = IDD_DLG_AutoImage };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	
	void _CreateLayout();

	CFillButton* _CreateButton(const CRect& rectButton,int nButtonSeq);

	void _DrawCircle();

	bool _IsRectInCircle(CPoint circleCenter, int circleRadius, CPoint rectCenter, int rectWidth, int rectHeight);

	//�ı���һ�ε����ť����ɫ
	void _ChangeCliekedButtonColor(UINT uID);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClick(UINT uID);
	DECLARE_MESSAGE_MAP()

private:
	//����Button����
	map<int,CFillButton*> m_mapID2Button;

	//�ѱ��������Button
	map<int,CFillButton*> m_mapID2ButtonClicked;

	//���һ�ε����Button ID
	int  m_nLastClickedButtonID;
};
