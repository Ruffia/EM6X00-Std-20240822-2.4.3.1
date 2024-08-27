

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SemCoreCtrlPanel_StandA_h_h__
#define __SemCoreCtrlPanel_StandA_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSemCoreCtrlPanel_StandA_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_StandA_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_StandA _DSemCoreCtrlPanel_StandA;

#endif 	/* ___DSemCoreCtrlPanel_StandA_FWD_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_StandAEvents_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_StandAEvents_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_StandAEvents _DSemCoreCtrlPanel_StandAEvents;

#endif 	/* ___DSemCoreCtrlPanel_StandAEvents_FWD_DEFINED__ */


#ifndef __SemCoreCtrlPanel_StandA_FWD_DEFINED__
#define __SemCoreCtrlPanel_StandA_FWD_DEFINED__

#ifdef __cplusplus
typedef class SemCoreCtrlPanel_StandA SemCoreCtrlPanel_StandA;
#else
typedef struct SemCoreCtrlPanel_StandA SemCoreCtrlPanel_StandA;
#endif /* __cplusplus */

#endif 	/* __SemCoreCtrlPanel_StandA_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_SemCoreCtrlPanel_StandA_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_StandA_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_StandA_0000_0000_v0_0_s_ifspec;


#ifndef __SEMCORECTRLPANEL_STANDALib_LIBRARY_DEFINED__
#define __SEMCORECTRLPANEL_STANDALib_LIBRARY_DEFINED__

/* library SEMCORECTRLPANEL_STANDALib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_SEMCORECTRLPANEL_STANDALib;

#ifndef ___DSemCoreCtrlPanel_StandA_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_StandA_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_StandA */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_StandA;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("AB976132-08C6-4087-9A62-9BA182345069")
    _DSemCoreCtrlPanel_StandA : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_StandAVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_StandA * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_StandA * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_StandA * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_StandA * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_StandA * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_StandA * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_StandA * This,
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
    } _DSemCoreCtrlPanel_StandAVtbl;

    interface _DSemCoreCtrlPanel_StandA
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_StandAVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_StandA_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_StandA_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_StandA_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_StandA_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_StandA_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_StandA_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_StandA_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_StandA_DISPINTERFACE_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_StandAEvents_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_StandAEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_StandAEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_StandAEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7077C5B3-BCD5-44AF-A31E-9B84397A6B8D")
    _DSemCoreCtrlPanel_StandAEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_StandAEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_StandAEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_StandAEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_StandAEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_StandAEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_StandAEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_StandAEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_StandAEvents * This,
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
    } _DSemCoreCtrlPanel_StandAEventsVtbl;

    interface _DSemCoreCtrlPanel_StandAEvents
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_StandAEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_StandAEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_StandAEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_StandAEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_StandAEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_StandAEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_StandAEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_StandAEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_StandAEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SemCoreCtrlPanel_StandA;

#ifdef __cplusplus

class DECLSPEC_UUID("6C185BD2-30F0-4524-A99C-1B6CEED4ED38")
SemCoreCtrlPanel_StandA;
#endif
#endif /* __SEMCORECTRLPANEL_STANDALib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


