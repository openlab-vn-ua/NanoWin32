// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Variant ole auto object // OAIdl.h // VARIANT

#if !defined(NanoWinOAVariantIncluded)
#define NanoWinOAVariantIncluded

#include "NanoWinTypes.h"

#if defined LINUX

// VARIANT OAIdl.h
// -----------------------------------------

typedef unsigned short VARTYPE;

enum VARENUM
{
	VT_EMPTY = 0,
	VT_NULL = 1,
	VT_I2 = 2,
	VT_I4 = 3,
	VT_R4 = 4,
	VT_R8 = 5,
	VT_CY = 6,
	VT_DATE = 7,
	VT_BSTR = 8,
	VT_DISPATCH = 9,
	VT_ERROR = 10,
	VT_BOOL = 11,
	VT_VARIANT = 12,
	VT_UNKNOWN = 13,
	VT_DECIMAL = 14,
	VT_I1 = 16,
	VT_UI1 = 17,
	VT_UI2 = 18,
	VT_UI4 = 19,
	VT_I8 = 20,
	VT_UI8 = 21,
	VT_INT = 22,
	VT_UINT = 23,
	VT_VOID = 24,
	VT_HRESULT = 25,
	VT_PTR = 26,
	VT_SAFEARRAY = 27,
	VT_CARRAY = 28,
	VT_USERDEFINED = 29,
	VT_LPSTR = 30,
	VT_LPWSTR = 31,
	VT_RECORD = 36,
	VT_INT_PTR = 37,
	VT_UINT_PTR = 38,
	VT_FILETIME = 64,
	VT_BLOB = 65,
	VT_STREAM = 66,
	VT_STORAGE = 67,
	VT_STREAMED_OBJECT = 68,
	VT_STORED_OBJECT = 69,
	VT_BLOB_OBJECT = 70,
	VT_CF = 71,
	VT_CLSID = 72,
	VT_VERSIONED_STREAM = 73,
	VT_BSTR_BLOB = 0xfff,
	VT_VECTOR = 0x1000,
	VT_ARRAY = 0x2000,
	VT_BYREF = 0x4000,
	VT_RESERVED = 0x8000,
	VT_ILLEGAL = 0xffff,
	VT_ILLEGALMASKED = 0xfff,
	VT_TYPEMASK = 0xfff
};

#if defined(VARIANT_USE_NAMED_UNION)
#define __VARIANT_NAME_1 n1
#define __VARIANT_NAME_2 n2
#define __VARIANT_NAME_3 n3
#define __VARIANT_NAME_4 brecVal
#else
#define __tagVARIANT
#define __VARIANT_NAME_1
#define __VARIANT_NAME_2
#define __VARIANT_NAME_3
#define __tagBRECORD
#define __VARIANT_NAME_4
#endif

// Variant sub types
// -----------------------------------------------

// 0 == FALSE, -1 == TRUE
typedef short VARIANT_BOOL;
#define _VARIANT_BOOL VARIANT_BOOL
#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

// COM interfgaces (not supported)
typedef void *IUnknown;
typedef void *IDispatch;
typedef void *IRecordInfo;

typedef void  SAFEARRAY; // OAIdl.h // not supported

// BSTR
typedef WCHAR OLECHAR;
typedef /* [wire_marshal] */ OLECHAR *BSTR; // Null-terminated

// Complex Data types

typedef struct tagDEC // Wire mashal view
{
	USHORT wReserved;
	BYTE scale;
	BYTE sign;
	ULONG Hi32;
	ULONGLONG Lo64;
} 	DECIMAL;

typedef struct tagCY // wire mashal view
{
	LONGLONG int64;
} 	CY;

// Simple Data types

typedef double DATE;

typedef double DOUBLE;

typedef DWORD SCODE; // Outlook // WTypesbase.h

typedef /* [wire_marshal] */ struct tagVARIANT VARIANT; // foreward declaration
struct tagVARIANT
{
	// <Fix> to allow compilation the same source
	#define bool _bool
	union
	{
		struct __tagVARIANT
		{
			VARTYPE vt;
			WORD wReserved1;
			WORD wReserved2;
			WORD wReserved3;
			union
			{
				LONGLONG llVal;
				LONG lVal;
				BYTE bVal;
				SHORT iVal;
				FLOAT fltVal;
				DOUBLE dblVal;
				VARIANT_BOOL boolVal;
				_VARIANT_BOOL bool;
				SCODE scode;
				CY cyVal;
				DATE date;
				BSTR bstrVal;
				IUnknown *punkVal;
				IDispatch *pdispVal;
				SAFEARRAY *parray;
				BYTE *pbVal;
				SHORT *piVal;
				LONG *plVal;
				LONGLONG *pllVal;
				FLOAT *pfltVal;
				DOUBLE *pdblVal;
				VARIANT_BOOL *pboolVal;
				_VARIANT_BOOL *pbool;
				SCODE *pscode;
				CY *pcyVal;
				DATE *pdate;
				BSTR *pbstrVal;
				IUnknown **ppunkVal;
				IDispatch **ppdispVal;
				SAFEARRAY **pparray;
				VARIANT *pvarVal;
				PVOID byref;
				CHAR cVal;
				USHORT uiVal;
				ULONG ulVal;
				ULONGLONG ullVal;
				INT intVal;
				UINT uintVal;
				DECIMAL *pdecVal;
				CHAR *pcVal;
				USHORT *puiVal;
				ULONG *pulVal;
				ULONGLONG *pullVal;
				INT *pintVal;
				UINT *puintVal;
				struct __tagBRECORD
				{
					PVOID pvRecord;
					IRecordInfo *pRecInfo;
				} 	__VARIANT_NAME_4;
			} 	__VARIANT_NAME_3;
		} 	__VARIANT_NAME_2;
		DECIMAL decVal;
	} 	__VARIANT_NAME_1;
	// </fix>
	#undef bool
};
NW_MAKE_PLP_TYPES_BY(VARIANT);

typedef VARIANT VARIANTARG;
NW_MAKE_PLP_TYPES_BY(VARIANTARG);

#ifdef __cplusplus
#define REFVARIANT const VARIANT &
#else
#define REFVARIANT const VARIANT * // __MIDL_CONST
#endif

#ifdef __cplusplus // safe templates

// Init variant structure with VT_EMPTY
inline void VariantInit(VARIANTARG *pvarg)
{
  if (pvarg == NULL) { return; } // Bug trap
  memset(pvarg, 0, sizeof(*pvarg)); // Just in case (strictly speaking not need)
  pvarg->vt = VT_EMPTY;
}

// Release vairant structure after use [Not implemented]
// (Simple cases may be supported later)
//extern void VariantClear(VARIANTARG *pvarg);

#endif

NW_EXTERN_C_BEGIN

// Move functions here later

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
