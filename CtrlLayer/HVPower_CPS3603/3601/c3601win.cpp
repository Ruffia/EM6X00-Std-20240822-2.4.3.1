////////////////////////////////////////////////////////////////////////////////
// Module    : c3601win.c
// Function  : DLL Startup/Shutdown functions and DLL function pointers.
// Usage     : Linked by all Windows apps that access c3601.DLL functions.
// Author    : Jim Lamberson
// Copyright : (C) 2004 Sensoray
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

// The DLL handle.
static HINSTANCE hlib;

// Instantiate the DLL function pointers.
#define XFUNC3601(RTYP,FNAM,ARGS)       RTYP ( __stdcall *FNAM ) ARGS
#include "c3601win.h"

//////////////////////////////////////////
// Populate the DLL function pointers.
// Returns 0 if pointers successfully initialized.

static int GetFuncPtrs( void )
{
#undef XFUNC3601
#define XFUNC3601(RTYP,FNAM,ARGS)  { if ( (FNAM = ( RTYP ( __stdcall * ) ARGS )GetProcAddress( hlib, #FNAM )) == NULL ) return -1; }
#include "c3601win.h"	// Initialize function pointers; return -1 if failed.

	return 0;			// Indicate success.
}


////////////////////////////////////////////////////////////////////
// Open c2600.DLL and get pointers to exported DLL functions.
// Returns zero if successful, otherwise error code.
// NumMMs is the number of MMs (model 2601 modules) in your system.

int C3601_DLLOpen( char *workdir )
{
    char dllname[256];

    if(workdir)
        sprintf(dllname, "%s\\c3601.dll", workdir);
    else
        sprintf(dllname, "c3601.dll");

	// Dynamically link to c2600.DLL, exit with error if link failed.
	if ( ( hlib = LoadLibrary( dllname ) ) == NULL )
		return ERR_LOAD_DLL;

	// Fill pointers to c2600.DLL functions, exit with error if attempt failed.
	if ( GetFuncPtrs() )
	{
		FreeLibrary( hlib );
		hlib = 0;
		return ERR_FUNCADDR;
	}
    return 0;
}

///////////////////////////////////////////////////////////////////
// Sever dynamic link to c2600.DLL.

void C3601_DLLClose( void )
{
	// Unlink from S2600.DLL.
	if ( hlib )
		FreeLibrary( hlib );
}



