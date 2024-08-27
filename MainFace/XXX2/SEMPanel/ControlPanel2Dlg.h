
// ControlPanel2Dlg.h : 头文件
//

#pragma once
#include <map>
using namespace std;
#include "LibUSB_Function.h"

class InputEdit;
class CCommandButton;

// CControlPanel2Dlg 对话框
class CControlPanel2Dlg : public CDialog
{
// 构造
public:
	CControlPanel2Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CControlPanel2Dlg();

// 对话框数据
	enum { IDD = IDD_CONTROLPANEL2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void _CreateLayout();
	void _CreateFont();
	CCommandButton* _CreateButton(const CRect& rectButton,int nButtonSeq);
	int _InitUSB();
	void _UnInitUSB();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClick(UINT uID);
	DECLARE_MESSAGE_MAP()

protected:
	//显示64个待发送的字节
	InputEdit*    m_pEdit[64];
	byte          m_DataSend[64];
	byte          m_DataRecv[64];
	//
	CStatic*      m_pStatic[8]; 
	//字体大小定制
	CFont*        m_pFont;
	//数据接收控件
	CEdit*        m_pRecvEdit;
	//libusb 函数名
	static const string  m_strFunction[8];
	static const string  m_strButtomCaption[8];

    map<int,CCommandButton*> m_mapID2Button;

	//以下三项是libusb 数据结构
	libusb_device_handle* usb_handle;
	usb_dev udev;
	libusb_device_descriptor udev_desc;
};
