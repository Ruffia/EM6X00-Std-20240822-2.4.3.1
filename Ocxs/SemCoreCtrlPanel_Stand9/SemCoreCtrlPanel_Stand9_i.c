

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:47:10 2024
 */
/* Compiler settings for SemCoreCtrlPanel_Stand9.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND9Lib,0x238D91B7,0x3A56,0x49CB,0xAA,0xDF,0x6F,0x3D,0xF7,0x42,0x1A,0xBF);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand9,0x7783BC3E,0x6657,0x4717,0x9B,0xFF,0x60,0xFA,0x2D,0xFB,0xB4,0x1C);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand9Events,0x78D88A88,0xB2D0,0x401B,0xAD,0x5C,0x48,0x0D,0x1A,0x83,0x27,0x60);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand9,0xE6FFA1C0,0x7787,0x41D9,0x8B,0xA6,0xB4,0xC5,0x12,0xAF,0x09,0xE3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



