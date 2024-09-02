
// AutoImageDlg.h : 头文件
//

#pragma once
#include <map>
using namespace std;
class CFillButton;


// CAutoImageDlg 对话框
class CAutoImageDlg : public CDialog
{
// 构造
public:
	CAutoImageDlg(CWnd* pParent = NULL);	// 标准构造函数

	virtual ~CAutoImageDlg();

// 对话框数据
	enum { IDD = IDD_DLG_AutoImage };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	
	void _CreateLayout();

	CFillButton* _CreateButton(const CRect& rectButton,int nButtonSeq);

	void _DrawCircle();

	bool _IsRectInCircle(CPoint circleCenter, int circleRadius, CPoint rectCenter, int rectWidth, int rectHeight);

	//改变上一次点击按钮的颜色
	void _ChangeCliekedButtonColor(UINT uID);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClick(UINT uID);
	DECLARE_MESSAGE_MAP()

private:
	//所有Button集合
	map<int,CFillButton*> m_mapID2Button;

	//已被点击过的Button
	map<int,CFillButton*> m_mapID2ButtonClicked;

	//最后一次点击的Button ID
	int  m_nLastClickedButtonID;
};
