#pragma once

#define Wafer_Inch6_Radius       6*2.54*10000/2  //6英寸晶圆半径(μm)
#define Fill_RowCount            26             //晶圆Fill 最大行数
#define Fill_ColumnCount         22             //晶圆Fill 最大列数

//Wafer比例尺，用于计算Fill区对应的样品台坐标
class CWaferRatioRuler
{
public:
	CWaferRatioRuler();

protected:
	void _Calculate();

public:
	//晶圆半径(μm)
	int m_nWaferRadius;

	//X 方向的比率
	float m_fXRatio;

	//Y 方向的比率
	float m_fYRatio;
};