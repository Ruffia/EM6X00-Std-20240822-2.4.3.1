#pragma once
#include "USB_Functions.h"


class AFX_EXT_CLASS CTestParameterManager  
{
public:
	static CTestParameterManager& Instance();

public:
	//ͼ���㷨���Բ���
	//��ֵ
	bool    m_bEnableAlgoAdjust;   //���þ�ֵ�㷨
	float   m_fAlgoAdjust;         //��ֵ�㷨����ϵ��

	//�Զ�����
	DWORD   m_dwSleep_AutoFocus;   //Step ���ʱ��(����)

protected:
	CTestParameterManager();

};
