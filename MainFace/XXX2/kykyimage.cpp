// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "kykyimage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IKYKYImage properties

/////////////////////////////////////////////////////////////////////////////
// IKYKYImage operations

long IKYKYImage::OpenImageFromFile(LPCTSTR lpszFilePathName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePathName);
	return result;
}

long IKYKYImage::OpenImageFromUntitledCxImage(OLE_HANDLE* pitem)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pitem);
	return result;
}
