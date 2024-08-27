// SemCoreCtrlPanel_Stand8.cpp : Implementation of CSemCoreCtrlPanel_Stand8App and DLL registration.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand8.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSemCoreCtrlPanel_Stand8App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xb3c00c8d, 0xec88, 0x4a15, { 0x8e, 0xe2, 0x56, 0x11, 0x9f, 0xae, 0xf0, 0xb4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8App::InitInstance - DLL initialization

BOOL CSemCoreCtrlPanel_Stand8App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.

		// Change the registry key under which our settings are stored.
		// TODO: You should modify this string to be something appropriate
		// such as the name of your company or organization.
		SetRegistryKey(g_csRegistryKey);

		LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8App::ExitInstance - DLL termination

int CSemCoreCtrlPanel_Stand8App::ExitInstance()
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
