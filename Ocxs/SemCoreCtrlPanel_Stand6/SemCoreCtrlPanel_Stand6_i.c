

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:46:55 2024
 */
/* Compiler settings for SemCoreCtrlPanel_Stand6.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND6Lib,0xE2411A6A,0xE8CF,0x408F,0xA3,0xB5,0xA2,0x7E,0xB7,0xB6,0x1F,0xB4);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand6,0xB6C04BA3,0x1D44,0x4F69,0x84,0x1E,0x5B,0x6D,0x87,0x73,0x05,0x3C);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand6Events,0x4ADD80A6,0xF40A,0x4C7A,0x90,0x85,0x75,0x5C,0x1C,0xAB,0xB3,0x4F);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand6,0x957FC761,0x96DB,0x40D2,0x97,0x52,0x5A,0x79,0x02,0x22,0xC4,0x98);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



