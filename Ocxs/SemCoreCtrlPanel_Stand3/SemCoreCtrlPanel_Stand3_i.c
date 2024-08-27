

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:47:04 2024
 */
/* Compiler settings for SemCoreCtrlPanel_Stand3.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND3Lib,0x03AAF95F,0xF2FE,0x41C9,0x9D,0xDB,0x4F,0x63,0xF1,0x7F,0x55,0xE3);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand3,0x94F7D89B,0x2BC5,0x4F43,0xA7,0xEE,0x7D,0x7A,0xA6,0xA9,0x8C,0x55);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand3Events,0x7CE808FA,0x2C8C,0x4C35,0xB6,0xE2,0xA6,0xE7,0x31,0x3C,0xDD,0xE7);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand3,0xA7FDF08D,0x50E0,0x4E4E,0xB1,0xC5,0x21,0xC9,0x23,0xB9,0x85,0x04);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



