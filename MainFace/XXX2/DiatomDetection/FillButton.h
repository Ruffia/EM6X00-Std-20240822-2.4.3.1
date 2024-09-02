#pragma once
#include "CustomButton.h"

class CKYKYFeSEMComm;
class CFillButton : public CCustomButton
{
	DECLARE_DYNAMIC(CFillButton)

public:
	CFillButton();
	virtual ~CFillButton();

	void SetStageClient(CKYKYFeSEMComm*	pClient)
	{
		m_pClient = pClient;
	}

	void SetStagePosition(float fX,float fY)
	{
		m_fXStagePos = fX;
		m_fYStagePos = fY;
	}

	void SetPosition(int nRow,int nColumn)
	{
		m_nRow = nRow;
		m_nCol = nColumn;
	}

	//����Fill�����ڶԻ����е����λ�ã�����������Ʒ̨�ж�Ӧ��λ��
	void CalculateStagePosition(const CPoint& ptWaferCenter);

	void ShowPostion();

	//�ƶ��Զ�̨
	void MoveStage();

public:
	CKYKYFeSEMComm*	m_pClient;

	float m_fXStagePos;   //Fill��Ӧ����Ʒ̨X����
	float m_fYStagePos;   //Fill��Ӧ����Ʒ̨Y����

	int m_nRow;           //���ھ�Բ����
	int m_nCol;           //���ھ�Բ����

	int m_nShowID;        //չʾ�ڽ����ϵ�ID
};
