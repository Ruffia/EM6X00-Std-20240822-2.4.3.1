#pragma once
#include "USB_Functions.h"
#include "SemCtrl.h"

class CSemVirtualMCICard;


//�޸�ʱ�䣺  2020-08-25
//�޸��ˣ�  �ܺ�
//�޸�ԭ���ع�����ԭ���� CXXX2App �У����ǿ��Բ�����theApp �Ĺ��ܴ�CXXX2App �ж�������
//���ܣ� App ȫ�ֺ���������

class CGlobalFunctionManager  
{

public:
	static CGlobalFunctionManager& Instance();

	void  UpdateScreenMag(CSemVirtualMCICard ImageCard, double dMag, BOOL bSync = FALSE );

protected:
	CGlobalFunctionManager();

};
