#pragma once

#define Wafer_Inch6_Radius       6*2.54*10000/2  //6Ӣ�羧Բ�뾶(��m)
#define Fill_RowCount            26             //��ԲFill �������
#define Fill_ColumnCount         22             //��ԲFill �������

//Wafer�����ߣ����ڼ���Fill����Ӧ����Ʒ̨����
class CWaferRatioRuler
{
public:
	CWaferRatioRuler();

protected:
	void _Calculate();

public:
	//��Բ�뾶(��m)
	int m_nWaferRadius;

	//X ����ı���
	float m_fXRatio;

	//Y ����ı���
	float m_fYRatio;
};