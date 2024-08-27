

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Thu Aug 12 11:05:18 2021
 */
/* Compiler settings for SemCoreCtrlPanel_Stand8.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND8Lib,0xB3C00C8D,0xEC88,0x4A15,0x8E,0xE2,0x56,0x11,0x9F,0xAE,0xF0,0xB4);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand8,0x00BA85F2,0xCBFD,0x49C6,0xAA,0xA6,0xC2,0x9B,0x81,0xF0,0x7F,0x38);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand8Events,0xE6A338F7,0xC0A3,0x4982,0xA8,0xF4,0x07,0x2C,0xAE,0x0F,0xFF,0xE3);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand8,0xF515798A,0xB3D0,0x4B57,0xB2,0xFE,0x89,0xEB,0xB3,0x7F,0x3A,0x69);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



