

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Thu Aug 12 11:05:12 2021
 */
/* Compiler settings for SemCoreCtrlPanel_Stand5.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND5Lib,0xBFFB2319,0xBCB4,0x4F5F,0xB8,0xDF,0x5A,0x77,0x5F,0x74,0xB1,0xEF);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand5,0x23E28CB6,0x7AC0,0x4401,0x83,0x3D,0x3A,0xDE,0xD9,0xBF,0xF7,0x99);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand5Events,0x61B3BB3D,0x9913,0x4370,0x86,0x97,0x70,0x4B,0x1B,0xF7,0x7B,0x93);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand5,0x96E0ED82,0xF866,0x44F0,0x9B,0xEB,0xE0,0x70,0x19,0x45,0xFC,0x1C);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



