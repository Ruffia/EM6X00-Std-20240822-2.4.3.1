

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:47:03 2024
 */
/* Compiler settings for SemCoreCtrlPanel_Stand2.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_SEMCORECTRLPANEL_STAND2Lib,0x9CAA635E,0x69EC,0x4684,0xBE,0x58,0xA1,0xA8,0x6A,0x30,0x1C,0xF3);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand2,0x1A6D957E,0xBEFD,0x4EE8,0xA2,0xBA,0x7B,0x21,0x06,0x25,0xA2,0xCE);


MIDL_DEFINE_GUID(IID, DIID__DSemCoreCtrlPanel_Stand2Events,0x40B3CEE8,0x606F,0x47B0,0xAF,0x16,0x04,0xBB,0xE0,0xE4,0x86,0xE9);


MIDL_DEFINE_GUID(CLSID, CLSID_SemCoreCtrlPanel_Stand2,0x2F96E150,0x5951,0x4817,0x8C,0xEC,0xAC,0x17,0x29,0x56,0x91,0x25);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



