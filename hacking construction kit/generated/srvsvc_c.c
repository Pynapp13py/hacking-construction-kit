

/* this ALWAYS GENERATED file contains the RPC client stubs */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "srvsvc.h"

#define TYPE_FORMAT_STRING_SIZE   53                                
#define PROC_FORMAT_STRING_SIZE   1117                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _srvsvc_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } srvsvc_MIDL_TYPE_FORMAT_STRING;

typedef struct _srvsvc_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } srvsvc_MIDL_PROC_FORMAT_STRING;

typedef struct _srvsvc_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } srvsvc_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const srvsvc_MIDL_TYPE_FORMAT_STRING srvsvc__MIDL_TypeFormatString;
extern const srvsvc_MIDL_PROC_FORMAT_STRING srvsvc__MIDL_ProcFormatString;
extern const srvsvc_MIDL_EXPR_FORMAT_STRING srvsvc__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: srvsvc, ver. 3.0,
   GUID={0x4b324fc8,0x1670,0x01d3,{0x12,0x78,0x5a,0x47,0xbf,0x6e,0xe1,0x88}} */



static const RPC_CLIENT_INTERFACE srvsvc___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x4b324fc8,0x1670,0x01d3,{0x12,0x78,0x5a,0x47,0xbf,0x6e,0xe1,0x88}},{3,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE srvsvc_v3_0_c_ifspec = (RPC_IF_HANDLE)& srvsvc___RpcClientInterface;

extern const MIDL_STUB_DESC srvsvc_StubDesc;

RPC_BINDING_HANDLE srvsvc__MIDL_AutoBindHandle = 0;


void func0( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&IDL_handle);
    
}


void func1( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[28],
                  ( unsigned char * )&IDL_handle);
    
}


void func2( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[56],
                  ( unsigned char * )&IDL_handle);
    
}


void func3( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[84],
                  ( unsigned char * )&IDL_handle);
    
}


void func4( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[112],
                  ( unsigned char * )&IDL_handle);
    
}


void func5( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[140],
                  ( unsigned char * )&IDL_handle);
    
}


void func6( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[168],
                  ( unsigned char * )&IDL_handle);
    
}


void func7( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[196],
                  ( unsigned char * )&IDL_handle);
    
}


void func8( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[224],
                  ( unsigned char * )&IDL_handle);
    
}


void func9( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[252],
                  ( unsigned char * )&IDL_handle);
    
}


void func0a( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[280],
                  ( unsigned char * )&IDL_handle);
    
}


void func0b( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[308],
                  ( unsigned char * )&IDL_handle);
    
}


void func0c( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[336],
                  ( unsigned char * )&IDL_handle);
    
}


void func0d( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[364],
                  ( unsigned char * )&IDL_handle);
    
}


void func0e( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[392],
                  ( unsigned char * )&IDL_handle);
    
}


void func0f( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[420],
                  ( unsigned char * )&IDL_handle);
    
}


void func10( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[448],
                  ( unsigned char * )&IDL_handle);
    
}


void func11( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[476],
                  ( unsigned char * )&IDL_handle);
    
}


void func12( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[504],
                  ( unsigned char * )&IDL_handle);
    
}


void func13( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[532],
                  ( unsigned char * )&IDL_handle);
    
}


void func14( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[560],
                  ( unsigned char * )&IDL_handle);
    
}


void func15( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[588],
                  ( unsigned char * )&IDL_handle);
    
}


void func16( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[616],
                  ( unsigned char * )&IDL_handle);
    
}


void func17( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[644],
                  ( unsigned char * )&IDL_handle);
    
}


void func18( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[672],
                  ( unsigned char * )&IDL_handle);
    
}


void func19( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[700],
                  ( unsigned char * )&IDL_handle);
    
}


void func1a( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[728],
                  ( unsigned char * )&IDL_handle);
    
}


void func1b( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[756],
                  ( unsigned char * )&IDL_handle);
    
}


void srvsvc_NetRemoteTOD( 
    /* [in] */ handle_t IDL_handle,
    /* [string][unique][in] */ wchar_t *server_unc,
    /* [ref][out] */ srvsvc_NetRemoteTODInfo **info)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[784],
                  ( unsigned char * )&IDL_handle);
    
}


void func1d( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[824],
                  ( unsigned char * )&IDL_handle);
    
}


void func1e( 
    /* [in] */ handle_t IDL_handle)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[852],
                  ( unsigned char * )&IDL_handle);
    
}


void srvsvc_NetprPathCanonicalize( 
    /* [in] */ handle_t IDL_handle,
    /* [string][unique][in] */ wchar_t *element_607,
    /* [string][in] */ wchar_t *element_608,
    /* [size_is][out] */ unsigned char element_609[  ],
    /* [in] */ long element_610,
    /* [string][in] */ wchar_t *element_611,
    /* [out][in] */ long *element_612,
    /* [in] */ long element_613)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[880],
                  ( unsigned char * )&IDL_handle);
    
}


int func20( 
    /* [in] */ handle_t IDL_handle)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[950],
                  ( unsigned char * )&IDL_handle);
    return ( int  )_RetVal.Simple;
    
}


int func21( 
    /* [in] */ handle_t IDL_handle)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[984],
                  ( unsigned char * )&IDL_handle);
    return ( int  )_RetVal.Simple;
    
}


int func22( 
    /* [in] */ handle_t IDL_handle)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[1018],
                  ( unsigned char * )&IDL_handle);
    return ( int  )_RetVal.Simple;
    
}


int func23( 
    /* [in] */ handle_t IDL_handle,
    /* [string][unique][in] */ wchar_t *arg_1,
    /* [string][in] */ wchar_t *arg_2,
    /* [string][in] */ wchar_t *arg_3,
    /* [in] */ long arg_4,
    /* [in] */ long arg_5)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&srvsvc_StubDesc,
                  (PFORMAT_STRING) &srvsvc__MIDL_ProcFormatString.Format[1052],
                  ( unsigned char * )&IDL_handle);
    return ( int  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const srvsvc_MIDL_PROC_FORMAT_STRING srvsvc__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure func0 */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x0 ),	/* 0 */
/* 18 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 20 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func1 */


	/* Parameter IDL_handle */

/* 28 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 30 */	NdrFcLong( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x1 ),	/* 1 */
/* 36 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 38 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 40 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 42 */	NdrFcShort( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 48 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func2 */


	/* Parameter IDL_handle */

/* 56 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 58 */	NdrFcLong( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x2 ),	/* 2 */
/* 64 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 66 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 76 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func3 */


	/* Parameter IDL_handle */

/* 84 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x3 ),	/* 3 */
/* 92 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 94 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 96 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 102 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 104 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 110 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func4 */


	/* Parameter IDL_handle */

/* 112 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 114 */	NdrFcLong( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x4 ),	/* 4 */
/* 120 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 122 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 124 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 132 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func5 */


	/* Parameter IDL_handle */

/* 140 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 146 */	NdrFcShort( 0x5 ),	/* 5 */
/* 148 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 150 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 152 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func6 */


	/* Parameter IDL_handle */

/* 168 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x6 ),	/* 6 */
/* 176 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 178 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 180 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 186 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 188 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func7 */


	/* Parameter IDL_handle */

/* 196 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 198 */	NdrFcLong( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x7 ),	/* 7 */
/* 204 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 206 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 208 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 216 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func8 */


	/* Parameter IDL_handle */

/* 224 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 230 */	NdrFcShort( 0x8 ),	/* 8 */
/* 232 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 234 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 242 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 244 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func9 */


	/* Parameter IDL_handle */

/* 252 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0x9 ),	/* 9 */
/* 260 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 262 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 264 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 270 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 272 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 278 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func0a */


	/* Parameter IDL_handle */

/* 280 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 282 */	NdrFcLong( 0x0 ),	/* 0 */
/* 286 */	NdrFcShort( 0xa ),	/* 10 */
/* 288 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 290 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 292 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 298 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 300 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func0b */


	/* Parameter IDL_handle */

/* 308 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0xb ),	/* 11 */
/* 316 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 318 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 320 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 328 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 330 */	NdrFcShort( 0x0 ),	/* 0 */
/* 332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 334 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func0c */


	/* Parameter IDL_handle */

/* 336 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0xc ),	/* 12 */
/* 344 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 346 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 348 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 354 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 356 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func0d */


	/* Parameter IDL_handle */

/* 364 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 366 */	NdrFcLong( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0xd ),	/* 13 */
/* 372 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 374 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 376 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 384 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func0e */


	/* Parameter IDL_handle */

/* 392 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 394 */	NdrFcLong( 0x0 ),	/* 0 */
/* 398 */	NdrFcShort( 0xe ),	/* 14 */
/* 400 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 402 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 404 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 410 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 412 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func0f */


	/* Parameter IDL_handle */

/* 420 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 422 */	NdrFcLong( 0x0 ),	/* 0 */
/* 426 */	NdrFcShort( 0xf ),	/* 15 */
/* 428 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 430 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 432 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 440 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 446 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func10 */


	/* Parameter IDL_handle */

/* 448 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 450 */	NdrFcLong( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x10 ),	/* 16 */
/* 456 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 458 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 460 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 466 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 468 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func11 */


	/* Parameter IDL_handle */

/* 476 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 478 */	NdrFcLong( 0x0 ),	/* 0 */
/* 482 */	NdrFcShort( 0x11 ),	/* 17 */
/* 484 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 486 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 488 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 494 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 496 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func12 */


	/* Parameter IDL_handle */

/* 504 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0x12 ),	/* 18 */
/* 512 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 514 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 516 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 524 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func13 */


	/* Parameter IDL_handle */

/* 532 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 534 */	NdrFcLong( 0x0 ),	/* 0 */
/* 538 */	NdrFcShort( 0x13 ),	/* 19 */
/* 540 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 542 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 544 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 552 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func14 */


	/* Parameter IDL_handle */

/* 560 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 562 */	NdrFcLong( 0x0 ),	/* 0 */
/* 566 */	NdrFcShort( 0x14 ),	/* 20 */
/* 568 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 570 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 572 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 580 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func15 */


	/* Parameter IDL_handle */

/* 588 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 590 */	NdrFcLong( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x15 ),	/* 21 */
/* 596 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 598 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 600 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 608 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func16 */


	/* Parameter IDL_handle */

/* 616 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 618 */	NdrFcLong( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x16 ),	/* 22 */
/* 624 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 626 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 628 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 634 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 636 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func17 */


	/* Parameter IDL_handle */

/* 644 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x17 ),	/* 23 */
/* 652 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 654 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 656 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 664 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 670 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func18 */


	/* Parameter IDL_handle */

/* 672 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 674 */	NdrFcLong( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x18 ),	/* 24 */
/* 680 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 682 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 684 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 692 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func19 */


	/* Parameter IDL_handle */

/* 700 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 702 */	NdrFcLong( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x19 ),	/* 25 */
/* 708 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 710 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 712 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 718 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 720 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func1a */


	/* Parameter IDL_handle */

/* 728 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 730 */	NdrFcLong( 0x0 ),	/* 0 */
/* 734 */	NdrFcShort( 0x1a ),	/* 26 */
/* 736 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 738 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 740 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 748 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func1b */


	/* Parameter IDL_handle */

/* 756 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 758 */	NdrFcLong( 0x0 ),	/* 0 */
/* 762 */	NdrFcShort( 0x1b ),	/* 27 */
/* 764 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 766 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 768 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x0 ),	/* 0 */
/* 774 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 776 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure srvsvc_NetRemoteTOD */


	/* Parameter IDL_handle */

/* 784 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 786 */	NdrFcLong( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x1c ),	/* 28 */
/* 792 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 794 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 796 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 800 */	NdrFcShort( 0x68 ),	/* 104 */
/* 802 */	0x42,		/* Oi2 Flags:  clt must size, has ext, */
			0x2,		/* 2 */
/* 804 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 810 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 812 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 814 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 816 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter server_unc */

/* 818 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 820 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 822 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Procedure func1d */


	/* Parameter info */

/* 824 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 830 */	NdrFcShort( 0x1d ),	/* 29 */
/* 832 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 834 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 836 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 844 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 850 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure func1e */


	/* Parameter IDL_handle */

/* 852 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x1e ),	/* 30 */
/* 860 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 862 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 864 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 870 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 872 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 878 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure srvsvc_NetprPathCanonicalize */


	/* Parameter IDL_handle */

/* 880 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 882 */	NdrFcLong( 0x0 ),	/* 0 */
/* 886 */	NdrFcShort( 0x1f ),	/* 31 */
/* 888 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 890 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 892 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 894 */	NdrFcShort( 0x2c ),	/* 44 */
/* 896 */	NdrFcShort( 0x1c ),	/* 28 */
/* 898 */	0x43,		/* Oi2 Flags:  srv must size, clt must size, has ext, */
			0x7,		/* 7 */
/* 900 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 902 */	NdrFcShort( 0x1 ),	/* 1 */
/* 904 */	NdrFcShort( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 908 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 910 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 912 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter element_607 */

/* 914 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 916 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 918 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Parameter element_608 */

/* 920 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 922 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 924 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Parameter element_609 */

/* 926 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 928 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 930 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter element_610 */

/* 932 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 934 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 936 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Parameter element_611 */

/* 938 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 940 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter element_612 */

/* 944 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 946 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure func20 */


	/* Parameter element_613 */

/* 950 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 952 */	NdrFcLong( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x20 ),	/* 32 */
/* 958 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 960 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 962 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 964 */	NdrFcShort( 0x0 ),	/* 0 */
/* 966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 968 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 970 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 976 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 978 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 980 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 982 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure func21 */


	/* Return value */

/* 984 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 986 */	NdrFcLong( 0x0 ),	/* 0 */
/* 990 */	NdrFcShort( 0x21 ),	/* 33 */
/* 992 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 994 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 996 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1000 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1002 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1004 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1010 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 1012 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1014 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1016 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure func22 */


	/* Return value */

/* 1018 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1020 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1024 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1026 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1028 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 1030 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 1032 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1036 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1038 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 1046 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1048 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1050 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure func23 */


	/* Return value */

/* 1052 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 1054 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1058 */	NdrFcShort( 0x23 ),	/* 35 */
/* 1060 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1062 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 1064 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 1066 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1068 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1070 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1072 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1078 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 1080 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1082 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1084 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter arg_1 */

/* 1086 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1088 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1090 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Parameter arg_2 */

/* 1092 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1094 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1096 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Parameter arg_3 */

/* 1098 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1100 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1102 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter arg_4 */

/* 1104 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1106 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter arg_5 */

/* 1110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1112 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const srvsvc_MIDL_TYPE_FORMAT_STRING srvsvc__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/*  8 */	NdrFcShort( 0x2 ),	/* Offset= 2 (10) */
/* 10 */	
			0x12, 0x0,	/* FC_UP */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 16 */	NdrFcShort( 0x30 ),	/* 48 */
/* 18 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 20 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 28 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 30 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 32 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 34 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 36 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 38 */	NdrFcShort( 0x1 ),	/* 1 */
/* 40 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 42 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 44 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 46 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 48 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 50 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const unsigned short srvsvc_FormatStringOffsetTable[] =
    {
    0,
    28,
    56,
    84,
    112,
    140,
    168,
    196,
    224,
    252,
    280,
    308,
    336,
    364,
    392,
    420,
    448,
    476,
    504,
    532,
    560,
    588,
    616,
    644,
    672,
    700,
    728,
    756,
    784,
    824,
    852,
    880,
    950,
    984,
    1018,
    1052
    };


static const MIDL_STUB_DESC srvsvc_StubDesc = 
    {
    (void *)& srvsvc___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &srvsvc__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    srvsvc__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

