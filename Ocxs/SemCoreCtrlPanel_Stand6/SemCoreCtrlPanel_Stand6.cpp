// SemCoreCtrlPanel_Stand6.cpp : Implementation of CSemCoreCtrlPanel_Stand6App and DLL registration.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSemCoreCtrlPanel_Stand6App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xe2411a6a, 0xe8cf, 0x408f, { 0xa3, 0xb5, 0xa2, 0x7e, 0xb7, 0xb6, 0x1f, 0xb4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6App::InitInstance - DLL initialization

BOOL CSemCoreCtrlPanel_Stand6App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6App::ExitInstance - DLL termination

int CSemCoreCtrlPanel_Stand6App::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
