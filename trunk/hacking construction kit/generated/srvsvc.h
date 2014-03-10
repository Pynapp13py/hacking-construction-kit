

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Nov 24 15:16:53 2013
 */
/* Compiler settings for srvsvc.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __srvsvc_h__
#define __srvsvc_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 

RPC_BINDING_HANDLE srvsvc__MIDL_AutoBindHandle;

#ifndef __srvsvc_INTERFACE_DEFINED__
#define __srvsvc_INTERFACE_DEFINED__

/* interface srvsvc */
/* [version][uuid] */ 

void func0( 
    /* [in] */ handle_t IDL_handle);

void func1( 
    /* [in] */ handle_t IDL_handle);

void func2( 
    /* [in] */ handle_t IDL_handle);

void func3( 
    /* [in] */ handle_t IDL_handle);

void func4( 
    /* [in] */ handle_t IDL_handle);

void func5( 
    /* [in] */ handle_t IDL_handle);

void func6( 
    /* [in] */ handle_t IDL_handle);

void func7( 
    /* [in] */ handle_t IDL_handle);

void func8( 
    /* [in] */ handle_t IDL_handle);

void func9( 
    /* [in] */ handle_t IDL_handle);

void func0a( 
    /* [in] */ handle_t IDL_handle);

void func0b( 
    /* [in] */ handle_t IDL_handle);

void func0c( 
    /* [in] */ handle_t IDL_handle);

void func0d( 
    /* [in] */ handle_t IDL_handle);

void func0e( 
    /* [in] */ handle_t IDL_handle);

void func0f( 
    /* [in] */ handle_t IDL_handle);

void func10( 
    /* [in] */ handle_t IDL_handle);

void func11( 
    /* [in] */ handle_t IDL_handle);

void func12( 
    /* [in] */ handle_t IDL_handle);

void func13( 
    /* [in] */ handle_t IDL_handle);

void func14( 
    /* [in] */ handle_t IDL_handle);

void func15( 
    /* [in] */ handle_t IDL_handle);

void func16( 
    /* [in] */ handle_t IDL_handle);

void func17( 
    /* [in] */ handle_t IDL_handle);

void func18( 
    /* [in] */ handle_t IDL_handle);

void func19( 
    /* [in] */ handle_t IDL_handle);

void func1a( 
    /* [in] */ handle_t IDL_handle);

void func1b( 
    /* [in] */ handle_t IDL_handle);

typedef /* [public][public] */ struct __MIDL_srvsvc_0001
    {
    int elapsed;
    int msecs;
    int hours;
    int mins;
    int secs;
    int hunds;
    int timezone;
    int tinterval;
    int day;
    int month;
    int year;
    int weekday;
    } 	srvsvc_NetRemoteTODInfo;

void srvsvc_NetRemoteTOD( 
    /* [in] */ handle_t IDL_handle,
    /* [string][unique][in] */ wchar_t *server_unc,
    /* [ref][out] */ srvsvc_NetRemoteTODInfo **info);

void func1d( 
    /* [in] */ handle_t IDL_handle);

void func1e( 
    /* [in] */ handle_t IDL_handle);

void srvsvc_NetprPathCanonicalize( 
    /* [in] */ handle_t IDL_handle,
    /* [string][unique][in] */ wchar_t *element_607,
    /* [string][in] */ wchar_t *element_608,
    /* [size_is][out] */ unsigned char element_609[  ],
    /* [in] */ long element_610,
    /* [string][in] */ wchar_t *element_611,
    /* [out][in] */ long *element_612,
    /* [in] */ long element_613);

int func20( 
    /* [in] */ handle_t IDL_handle);

int func21( 
    /* [in] */ handle_t IDL_handle);

int func22( 
    /* [in] */ handle_t IDL_handle);

int func23( 
    /* [in] */ handle_t IDL_handle,
    /* [string][unique][in] */ wchar_t *arg_1,
    /* [string][in] */ wchar_t *arg_2,
    /* [string][in] */ wchar_t *arg_3,
    /* [in] */ long arg_4,
    /* [in] */ long arg_5);



extern RPC_IF_HANDLE srvsvc_v3_0_c_ifspec;
extern RPC_IF_HANDLE srvsvc_v3_0_s_ifspec;
#endif /* __srvsvc_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


