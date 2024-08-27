

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:47:06 2024
 */
/* Compiler settings for SemCoreCtrlPanel_Stand7.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND7Lib,0x234F608C,0xAD09,0x420A,0x81,0x7D,0x91,0x13,0xEE,0x87,0x79,0xF5);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand7,0x3C97F6F9,0xCE50,0x40E4,0xAB,0xDB,0xA4,0x4C,0xCA,0x20,0x8D,0x0A);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand7Events,0x71A2EEA4,0xCACC,0x40C5,0xAD,0x25,0x58,0xD1,0x1B,0x71,0x20,0x63);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand7,0xA3B6C881,0xD4E7,0x401E,0x86,0x47,0xD4,0x53,0x77,0x33,0x4A,0x7B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



