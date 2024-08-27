

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:46:59 2024
 */
/* Compiler settings for SemCoreCtrlPanel_Stand1.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND1Lib,0xC376E3F2,0xB7C8,0x46A8,0x84,0x63,0x88,0x48,0xB7,0x39,0x2A,0xC9);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand1,0x997ED4B7,0xAEFA,0x46E4,0x95,0x59,0x7E,0x8B,0xEA,0xD2,0xCD,0x4B);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand1Events,0xC696CDD4,0x4806,0x437D,0x9E,0x74,0x85,0x4A,0x3D,0x8D,0x96,0x44);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand1,0xA34817AD,0x1937,0x4D50,0xAB,0xC0,0x07,0x20,0x90,0xD2,0xD1,0xBF);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



