// SemCoreCtrlPanel_Stand5.cpp : Implementation of CSemCoreCtrlPanel_Stand5App and DLL registration.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSemCoreCtrlPanel_Stand5App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xbffb2319, 0xbcb4, 0x4f5f, { 0xb8, 0xdf, 0x5a, 0x77, 0x5f, 0x74, 0xb1, 0xef } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5App::InitInstance - DLL initialization

BOOL CSemCoreCtrlPanel_Stand5App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5App::ExitInstance - DLL termination

int CSemCoreCtrlPanel_Stand5App::ExitInstance()
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
