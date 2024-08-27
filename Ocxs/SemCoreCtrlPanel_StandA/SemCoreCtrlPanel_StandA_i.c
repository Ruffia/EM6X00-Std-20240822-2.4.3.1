

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:47:12 2024
 */
/* Compiler settings for SemCoreCtrlPanel_StandA.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STANDALib,0x7FB0B592,0xEE80,0x45D1,0x85,0xBC,0x28,0x80,0xD0,0x7E,0xC0,0xBB);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_StandA,0xAB976132,0x08C6,0x4087,0x9A,0x62,0x9B,0xA1,0x82,0x34,0x50,0x69);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_StandAEvents,0x7077C5B3,0xBCD5,0x44AF,0xA3,0x1E,0x9B,0x84,0x39,0x7A,0x6B,0x8D);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_StandA,0x6C185BD2,0x30F0,0x4524,0xA9,0x9C,0x1B,0x6C,0xEE,0xD4,0xED,0x38);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



