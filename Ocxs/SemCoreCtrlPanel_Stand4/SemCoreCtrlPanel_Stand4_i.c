

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Thu Aug 12 11:05:04 2021
 */
/* Compiler settings for SemCoreCtrlPanel_Stand4.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND4Lib,0xBB85E22F,0xB880,0x45DE,0x8F,0x8A,0xB0,0x3F,0xEB,0x79,0x5B,0xA0);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand4,0x4513F07B,0xC2B1,0x4D1C,0x84,0x69,0xDC,0x06,0xA7,0x3F,0xB7,0x5F);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand4Events,0xA4D9D351,0x701F,0x4A82,0xBF,0xCB,0xD4,0xD9,0x0E,0xC7,0x74,0x6F);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand4,0x1FB180C2,0xD2C8,0x4029,0xA6,0x4C,0x26,0xFD,0x6A,0xA0,0xE2,0xAF);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



