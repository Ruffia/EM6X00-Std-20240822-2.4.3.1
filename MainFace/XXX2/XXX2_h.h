

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Aug 26 17:46:24 2024
 */
/* Compiler settings for XXX2.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __XXX2_h_h__
#define __XXX2_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IXXX2_FWD_DEFINED__
#define __IXXX2_FWD_DEFINED__
typedef interface IXXX2 IXXX2;

#endif 	/* __IXXX2_FWD_DEFINED__ */


#ifndef __Document_FWD_DEFINED__
#define __Document_FWD_DEFINED__

#ifdef __cplusplus
typedef class Document Document;
#else
typedef struct Document Document;
#endif /* __cplusplus */

#endif 	/* __Document_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __XXX2_LIBRARY_DEFINED__
#define __XXX2_LIBRARY_DEFINED__

/* library XXX2 */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_XXX2,0x57500EEB,0x9C48,0x4B79,0xB9,0x30,0x7A,0x5C,0x57,0x30,0x3F,0xFB);

#ifndef __IXXX2_DISPINTERFACE_DEFINED__
#define __IXXX2_DISPINTERFACE_DEFINED__

/* dispinterface IXXX2 */
/* [uuid] */ 


DEFINE_GUID(DIID_IXXX2,0xB8CDC981,0xF26C,0x4318,0x9E,0x3F,0x25,0x7F,0xAE,0x93,0x84,0x68);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B8CDC981-F26C-4318-9E3F-257FAE938468")
    IXXX2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IXXX2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXXX2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXXX2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXXX2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IXXX2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IXXX2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IXXX2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IXXX2 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IXXX2Vtbl;

    interface IXXX2
    {
        CONST_VTBL struct IXXX2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXXX2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXXX2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXXX2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXXX2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IXXX2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IXXX2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IXXX2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IXXX2_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Document,0x66E4C113,0xAB95,0x4138,0x8B,0xB6,0x1E,0x89,0x0A,0x36,0xE5,0x4F);

#ifdef __cplusplus

class DECLSPEC_UUID("66E4C113-AB95-4138-8BB6-1E890A36E54F")
Document;
#endif
#endif /* __XXX2_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


