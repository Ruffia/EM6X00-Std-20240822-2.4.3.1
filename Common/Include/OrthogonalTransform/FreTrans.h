// FreTrans.h


#ifndef _INC_FreTransAPI
#define _INC_FreTransAPI

#include <complex>
using namespace std;

// º¯ÊýÔ­ÐÍ
AFX_EXT_API VOID WINAPI FFT(complex<double> * TD, complex<double> * FD, int r);
AFX_EXT_API VOID WINAPI IFFT(complex<double> * FD, complex<double> * TD, int r);
AFX_EXT_API VOID WINAPI DCT(double *f, double *F, int power);
AFX_EXT_API VOID WINAPI IDCT(double *F, double *f, int power);
AFX_EXT_API VOID WINAPI WALSH(double *f, double *F, int r);
AFX_EXT_API VOID WINAPI IWALSH(double *F, double *f, int r);

AFX_EXT_API BOOL WINAPI Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
AFX_EXT_API BOOL WINAPI DIBDct(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
AFX_EXT_API BOOL WINAPI DIBWalsh(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
AFX_EXT_API BOOL WINAPI DIBWalsh1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

AFX_EXT_API BOOL WINAPI SEM_DIBWalsh1
(
	LPSTR lpDIBBits,
	LONG lWidth,
	LONG lHeight,
	LONG lWalshWidth,
	LONG lWalshHeight,
	double &dfIntegralWalsh,
	double &dflWalsh );

AFX_EXT_API BOOL SEM_Grads(
	LPSTR lpDIBBits,
	LONG lWidth,
	LONG lHeight,
	double &dfParamX,
	double &dfParamY,
	double &dfTotal );

#endif //!_INC_FreTransAPI