

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Thu Aug 12 11:05:18 2021
 */
/* Compiler settings for SemCoreCtrlPanel_Stand8.odl:
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


#ifndef __SemCoreCtrlPanel_Stand8_h_h__
#define __SemCoreCtrlPanel_Stand8_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSemCoreCtrlPanel_Stand8_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_Stand8_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_Stand8 _DSemCoreCtrlPanel_Stand8;

#endif 	/* ___DSemCoreCtrlPanel_Stand8_FWD_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_Stand8Events_FWD_DEFINED__
#define ___DSemCoreCtrlPanel_Stand8Events_FWD_DEFINED__
typedef interface _DSemCoreCtrlPanel_Stand8Events _DSemCoreCtrlPanel_Stand8Events;

#endif 	/* ___DSemCoreCtrlPanel_Stand8Events_FWD_DEFINED__ */


#ifndef __SemCoreCtrlPanel_Stand8_FWD_DEFINED__
#define __SemCoreCtrlPanel_Stand8_FWD_DEFINED__

#ifdef __cplusplus
typedef class SemCoreCtrlPanel_Stand8 SemCoreCtrlPanel_Stand8;
#else
typedef struct SemCoreCtrlPanel_Stand8 SemCoreCtrlPanel_Stand8;
#endif /* __cplusplus */

#endif 	/* __SemCoreCtrlPanel_Stand8_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_SemCoreCtrlPanel_Stand8_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_Stand8_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SemCoreCtrlPanel_Stand8_0000_0000_v0_0_s_ifspec;


#ifndef __SEMCORECTRLPANEL_STAND8Lib_LIBRARY_DEFINED__
#define __SEMCORECTRLPANEL_STAND8Lib_LIBRARY_DEFINED__

/* library SEMCORECTRLPANEL_STAND8Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_SEMCORECTRLPANEL_STAND8Lib;

#ifndef ___DSemCoreCtrlPanel_Stand8_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_Stand8_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_Stand8 */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_Stand8;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("00BA85F2-CBFD-49C6-AAA6-C29B81F07F38")
    _DSemCoreCtrlPanel_Stand8 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_Stand8Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_Stand8 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_Stand8 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_Stand8 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_Stand8 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_Stand8 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_Stand8 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_Stand8 * This,
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
    } _DSemCoreCtrlPanel_Stand8Vtbl;

    interface _DSemCoreCtrlPanel_Stand8
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_Stand8Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_Stand8_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_Stand8_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_Stand8_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_Stand8_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_Stand8_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_Stand8_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_Stand8_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_Stand8_DISPINTERFACE_DEFINED__ */


#ifndef ___DSemCoreCtrlPanel_Stand8Events_DISPINTERFACE_DEFINED__
#define ___DSemCoreCtrlPanel_Stand8Events_DISPINTERFACE_DEFINED__

/* dispinterface _DSemCoreCtrlPanel_Stand8Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DSemCoreCtrlPanel_Stand8Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E6A338F7-C0A3-4982-A8F4-072CAE0FFFE3")
    _DSemCoreCtrlPanel_Stand8Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSemCoreCtrlPanel_Stand8EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSemCoreCtrlPanel_Stand8Events * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSemCoreCtrlPanel_Stand8Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSemCoreCtrlPanel_Stand8Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSemCoreCtrlPanel_Stand8Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSemCoreCtrlPanel_Stand8Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSemCoreCtrlPanel_Stand8Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSemCoreCtrlPanel_Stand8Events * This,
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
    } _DSemCoreCtrlPanel_Stand8EventsVtbl;

    interface _DSemCoreCtrlPanel_Stand8Events
    {
        CONST_VTBL struct _DSemCoreCtrlPanel_Stand8EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSemCoreCtrlPanel_Stand8Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSemCoreCtrlPanel_Stand8Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSemCoreCtrlPanel_Stand8Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSemCoreCtrlPanel_Stand8Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSemCoreCtrlPanel_Stand8Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSemCoreCtrlPanel_Stand8Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSemCoreCtrlPanel_Stand8Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSemCoreCtrlPanel_Stand8Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SemCoreCtrlPanel_Stand8;

#ifdef __cplusplus

class DECLSPEC_UUID("F515798A-B3D0-4B57-B2FE-89EBB37F3A69")
SemCoreCtrlPanel_Stand8;
#endif
#endif /* __SEMCORECTRLPANEL_STAND8Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


