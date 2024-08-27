#include "stdafx.h"
#include "AlgorithmManager.h"
#include <math.h>


CAlgorithmManager& CAlgorithmManager::Instance()
{
	static CAlgorithmManager Inst;
	return Inst;
}


CAlgorithmManager::CAlgorithmManager()
{	

}



void CAlgorithmManager::LeastSquares_LinearEquation( int nCounts, double* pdC, double* pdK, double& dKa, double& dKb )
{
	double l, m, n, p;
	l = 0.0;
	m = 0.0;
	n = 0.0;
	p = 0.0;

	for( int i=0; i<nCounts; i++ )
	{
		l += pdC[i];
		m += pdK[i];
		n += pdC[i]*pdK[i];
		p += pdC[i]*pdC[i];
	}
	dKa = ( nCounts *n -l *m ) / ( nCounts *p -l *l );
	dKb = ( p *m -n *l ) / ( nCounts *p -l *l );
}

void CAlgorithmManager::LeastSquares_LinearEquation2( int nCounts, double* pdC, double* pdK, double& dKa, double& dKb )
{
	double midx, midy;
	double tempMu, tempZi;

	// 求平均值
	midx = 0.0;
	midy = 0.0;
	for(int i=0; i<nCounts; i++ )
	{
		midx += ( 1.0 *pdC[i] /nCounts);
		midy += ( 1.0 *pdK[i] /nCounts);
	}

	// 求斜率
	tempMu	= 0.0;
	tempZi	= 0.0;
	for(int i=0; i<nCounts; i++ )
	{
		tempMu	+= ( (pdC[i] -midx) * (pdC[i] -midx) );
		tempZi	+= ( (pdC[i] -midx) * (pdK[i] -midy) );
	}
	if( tempMu == 0 )
		printf( "Fail!\n" );
	dKa = tempZi /tempMu;

	// 求截距
	dKb = midy -dKa *midx;
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 拟合多项式polynomial
double CAlgorithmManager::LeastSquares_Polynomial_cf(int i,double f)
{
	double g=1.0;
	while(i--)
		g*=f;
	return g;
}

void CAlgorithmManager::LeastSquares_Polynomial_ColPivot(double a[299][299], int powerAdd1, double b[299][1],double factor[299])
{
	double max,m,temp;
	int k,q=0,i,j,I;
	for(k=0;k<powerAdd1 && q==0;k++)
	{
		max=a[k][k];
		for(i=k+1;i<powerAdd1;i++)
			if(fabs(max)<fabs(a[i][k]))
			{
				max=a[i][k];
				I=i;
			}

			if(max==0)
				q=1;
			else
			{
				if(I!=k)
				{
					temp=b[I][0];
					b[I][0]=b[k][0];
					b[k][0]=temp;

					for(j=k;j<powerAdd1;j++)
					{
						temp=a[I][j];
						a[I][j]=a[k][j];
						a[k][j]=temp;
					}
				}
				for(i=k+1;i<powerAdd1;i++)
				{
					m=a[i][k]/a[k][k];
					b[i][0]=b[i][0]-b[k][0]*m;

					for(j=0;j<powerAdd1;j++)
						a[i][j]=a[i][j]-a[k][j]*m;
				}
			}
	}
	for(i=powerAdd1-1;i>=0;i--)
	{
		for(j=powerAdd1-1;j>=i+1;j--)
			b[i][0]=b[i][0]-a[i][j]*factor[j];
		factor[i]=b[i][0]/a[i][i];
	}
} 