

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SemCoreCtrlPanel_Stand7_h_h__
#define __SemCoreCtrlPanel_Stand7_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSemCoreCtrlPanel_Stand7_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_Stand7_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_Stand7 _DSemCoreCtrlPanel_Stand7;

#endif 	/* ___DSemCoreCtrlPanel_Stand7_FWD_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_Stand7Events_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_Stand7Events_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_Stand7Events _DSemCoreCtrlPanel_Stand7Events;

#endif 	/* ___DSemCoreCtrlPanel_Stand7Events_FWD_DEFINED__ */


#ifndef __SemCoreCtrlPanel_Stand7_FWD_DEFINED__
#define __SemCoreCtrlPanel_Stand7_FWD_DEFINED__

#ifdef __cplusplus
typedef class SemCoreCtrlPanel_Stand7 SemCoreCtrlPanel_Stand7;
#else
typedef struct SemCoreCtrlPanel_Stand7 SemCoreCtrlPanel_Stand7;
#endif /* __cplusplus */

#endif 	/* __SemCoreCtrlPanel_Stand7_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_SemCoreCtrlPanel_Stand7_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_Stand7_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_Stand7_0000_0000_v0_0_s_ifspec;


#ifndef __SEMCORECTRLPANEL_STAND7Lib_LIBRARY_DEFINED__
#define __SEMCORECTRLPANEL_STAND7Lib_LIBRARY_DEFINED__

/* library SEMCORECTRLPANEL_STAND7Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_SEMCORECTRLPANEL_STAND7Lib;

#ifndef ___DSemCoreCtrlPanel_Stand7_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_Stand7_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_Stand7 */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_Stand7;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3C97F6F9-CE50-40E4-ABDB-A44CCA208D0A")
    _DSemCoreCtrlPanel_Stand7 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_Stand7Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_Stand7 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_Stand7 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_Stand7 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_Stand7 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_Stand7 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_Stand7 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_Stand7 * This,
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
    } _DSemCoreCtrlPanel_Stand7Vtbl;

    interface _DSemCoreCtrlPanel_Stand7
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_Stand7Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_Stand7_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_Stand7_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_Stand7_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_Stand7_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_Stand7_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_Stand7_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_Stand7_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_Stand7_DISPINTERFACE_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_Stand7Events_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_Stand7Events_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_Stand7Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_Stand7Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("71A2EEA4-CACC-40C5-AD25-58D11B712063")
    _DSemCoreCtrlPanel_Stand7Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_Stand7EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_Stand7Events * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_Stand7Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_Stand7Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_Stand7Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_Stand7Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_Stand7Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_Stand7Events * This,
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
    } _DSemCoreCtrlPanel_Stand7EventsVtbl;

    interface _DSemCoreCtrlPanel_Stand7Events
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_Stand7EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_Stand7Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_Stand7Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_Stand7Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_Stand7Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_Stand7Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_Stand7Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_Stand7Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_Stand7Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SemCoreCtrlPanel_Stand7;

#ifdef __cplusplus

class DECLSPEC_UUID("A3B6C881-D4E7-401E-8647-D45377334A7B")
SemCoreCtrlPanel_Stand7;
#endif
#endif /* __SEMCORECTRLPANEL_STAND7Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


