

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SemCoreCtrlPanel_Stand9_h_h__
#define __SemCoreCtrlPanel_Stand9_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSemCoreCtrlPanel_Stand9_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_Stand9_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_Stand9 _DSemCoreCtrlPanel_Stand9;

#endif 	/* ___DSemCoreCtrlPanel_Stand9_FWD_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_Stand9Events_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_Stand9Events_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_Stand9Events _DSemCoreCtrlPanel_Stand9Events;

#endif 	/* ___DSemCoreCtrlPanel_Stand9Events_FWD_DEFINED__ */


#ifndef __SemCoreCtrlPanel_Stand9_FWD_DEFINED__
#define __SemCoreCtrlPanel_Stand9_FWD_DEFINED__

#ifdef __cplusplus
typedef class SemCoreCtrlPanel_Stand9 SemCoreCtrlPanel_Stand9;
#else
typedef struct SemCoreCtrlPanel_Stand9 SemCoreCtrlPanel_Stand9;
#endif /* __cplusplus */

#endif 	/* __SemCoreCtrlPanel_Stand9_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_SemCoreCtrlPanel_Stand9_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_Stand9_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_Stand9_0000_0000_v0_0_s_ifspec;


#ifndef __SEMCORECTRLPANEL_STAND9Lib_LIBRARY_DEFINED__
#define __SEMCORECTRLPANEL_STAND9Lib_LIBRARY_DEFINED__

/* library SEMCORECTRLPANEL_STAND9Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_SEMCORECTRLPANEL_STAND9Lib;

#ifndef ___DSemCoreCtrlPanel_Stand9_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_Stand9_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_Stand9 */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_Stand9;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7783BC3E-6657-4717-9BFF-60FA2DFBB41C")
    _DSemCoreCtrlPanel_Stand9 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_Stand9Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_Stand9 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_Stand9 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_Stand9 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_Stand9 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_Stand9 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_Stand9 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_Stand9 * This,
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
    } _DSemCoreCtrlPanel_Stand9Vtbl;

    interface _DSemCoreCtrlPanel_Stand9
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_Stand9Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_Stand9_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_Stand9_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_Stand9_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_Stand9_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_Stand9_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_Stand9_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_Stand9_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_Stand9_DISPINTERFACE_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_Stand9Events_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_Stand9Events_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_Stand9Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_Stand9Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("78D88A88-B2D0-401B-AD5C-480D1A832760")
    _DSemCoreCtrlPanel_Stand9Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_Stand9EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_Stand9Events * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_Stand9Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_Stand9Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_Stand9Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_Stand9Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_Stand9Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_Stand9Events * This,
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
    } _DSemCoreCtrlPanel_Stand9EventsVtbl;

    interface _DSemCoreCtrlPanel_Stand9Events
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_Stand9EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_Stand9Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_Stand9Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_Stand9Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_Stand9Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_Stand9Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_Stand9Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_Stand9Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_Stand9Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SemCoreCtrlPanel_Stand9;

#ifdef __cplusplus

class DECLSPEC_UUID("E6FFA1C0-7787-41D9-8BA6-B4C512AF09E3")
SemCoreCtrlPanel_Stand9;
#endif
#endif /* __SEMCORECTRLPANEL_STAND9Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


