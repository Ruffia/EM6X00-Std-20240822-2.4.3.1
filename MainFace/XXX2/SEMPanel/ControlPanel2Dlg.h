
// ControlPanel2Dlg.h : ͷ�ļ�
//

#pragma once
#include <map>
using namespace std;
#include "LibUSB_Function.h"

class InputEdit;
class CCommandButton;

// CControlPanel2Dlg �Ի���
class CControlPanel2Dlg : public CDialog
{
// ����
public:
	CControlPanel2Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CControlPanel2Dlg();

// �Ի�������
	enum { IDD = IDD_CONTROLPANEL2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	//��ʾ64�������͵��ֽ�
	InputEdit*    m_pEdit[64];
	byte          m_DataSend[64];
	byte          m_DataRecv[64];
	//
	CStatic*      m_pStatic[8]; 
	//�����С����
	CFont*        m_pFont;
	//���ݽ��տؼ�
	CEdit*        m_pRecvEdit;
	//libusb ������
	static const string  m_strFunction[8];
	static const string  m_strButtomCaption[8];

    map<int,CCommandButton*> m_mapID2Button;

	//����������libusb ���ݽṹ
	libusb_device_handle* usb_handle;
	usb_dev udev;
	libusb_device_descriptor udev_desc;
};
