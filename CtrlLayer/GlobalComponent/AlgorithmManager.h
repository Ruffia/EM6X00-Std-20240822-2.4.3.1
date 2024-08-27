#pragma once
#include "USB_Functions.h"


class AFX_EXT_CLASS CAlgorithmManager  
{
public:
	static CAlgorithmManager& Instance();

	// 拟合直线方程linear equation
	void LeastSquares_LinearEquation( int nCounts, double* pdC, double* pdK, double& dKa, double& dKb );
	void LeastSquares_LinearEquation2( int nCounts, double* pdC, double* pdK, double& dKa, double& dKb );
	// 拟合多项式polynomial
	double LeastSquares_Polynomial_cf(int i,double f);
	void LeastSquares_Polynomial_ColPivot(double a[299][299], int powerAdd1, double b[299][1],double factor[299]);

public:
	template<class T>
	float Avg(T* src,const int nCount)
	{
		float fSum = 0;
		for (int i = 0;i< nCount;i++)
		{
			fSum += src[i];
		}

		return fSum/nCount;
	}

	template<class T>
	void  Adjust(T* src, const int nCount, const float fFactor)
	{
		float fAvg = Avg(src,nCount);
		float* pDest = new float[nCount];
		for (int i = 0; i < nCount;i++)
		{
			pDest[i] = src[i] - fAvg;
		}

		for (int i = 0; i < nCount;i++)
		{
			float fGray = pDest[i]* fFactor + fAvg;
			fGray = fGray < 0.00f?0.00f:fGray;
			fGray = fGray > 255.f?255.f:fGray;
			src[i] = (T)fGray;
		}

		delete[] pDest;
		pDest = NULL;
	}

protected:
	CAlgorithmManager();

};
