// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Basic windows types substitution

#if !defined(NanoWinTypesIncluded)
#define NanoWinTypesIncluded

// Just some basic defines
// -----------------------------------------------------------------------
// Note: All NanoWin32 specific stuff would have NW or NanoWin prefix

#if defined(__GNUC__)
#if (__x86_64__ || __ppc64__)
#define NWCPU64		64
#define NWCPULONG64	64 // width of long type is 64
#elif (__LP64__)
#define NWCPU64		64
#define NWCPULONG64	64 // width of long type is 64
#else
#define NWCPU32		32
#define NWCPULONG32	32 // width of long type is 32
#endif
#elif defined(_MSC_VER)
#if defined(_WIN64) || defined(_M_X64) || defined(_M_ARM64) || defined(_M_AMD64)
#define NWCPU64		64
#define NWCPULONG32	32 // width of long type is 32 // Under MS Long is 32 bits wide even under 64 bit platform
#else
#define NWCPU32		32
#define NWCPULONG32	32 // width of long type is 32
#endif
#endif

#if (defined(LINUX) || defined(__linux__) || defined(__unix__))

#if !defined(LINUX)
#define LINUX
#endif

// The calling convention for functions
// -----------------------------------------------------------------------
// CALLBACK, WINAPI, and APIENTRY are all used to define functions with the __stdcall calling convention. 
// Most functions in the Windows API are declared using WINAPI. 
// You may wish to use CALLBACK for the callback functions that you implement to help identify the function as a callback function.
#define WINAPI    // __stdcall
#define CALLBACK  // __stdcall 
#define APIENTRY  WINAPI

// Function naming style
#if defined(__cplusplus)
#define WINBASEAPI extern "C"
#else
#define WINBASEAPI extern
#endif

// Basic windows types
// -----------------------------------------------------------------------

#include <stdint.h>
#include <wchar.h>

#ifndef FAR
#define FAR                            // Nothing under 32/64 bits (everything is near)
#endif

#ifndef CONST
#define CONST const                    // WinDef.h // Conset supported everywhere
#endif

#define NW_MAKE_P_TYPE(TYPE,NAME)      typedef TYPE *P##NAME;
#define NW_MAKE_LP_TYPE(TYPE,NAME)     typedef TYPE FAR *LP##NAME;
#define NW_MAKE_PLP_TYPES(TYPE,NAME)   NW_MAKE_P_TYPE(TYPE,NAME); NW_MAKE_LP_TYPE(TYPE,NAME);
#define NW_MAKE_PLP_TYPES_BY(NAME)     NW_MAKE_PLP_TYPES(NAME,NAME);

// Define pointer that can be null only (that is, you can pass it only as null)
#define NW_PNULLONLY_TYPE(TYPE)        typedef struct NW_##TYPE##_NULLONLY { void *dummy; } *TYPE;

// Define PTYPE_NULLONLY type version of source PTYPE
#define NW_MAKE_PTYPE_NULLONLY(PTYPE)  NW_PNULLONLY_TYPE(PTYPE##_NULLONLY)

#ifndef VOID
#define VOID                           void      // WinNT.h // Decalred via define, not a typedef (historically)
typedef void                          *PVOID;
typedef void FAR                      *LPVOID;
typedef const void                    *PCVOID;
typedef const void FAR                *LPCVOID;
#endif

// WinDef.h

typedef unsigned char                  BYTE;     NW_MAKE_PLP_TYPES_BY(BYTE);  // 8 bit

typedef uint16_t                       WORD;     NW_MAKE_PLP_TYPES_BY(WORD);   // 16 bit u
typedef int16_t                        SHORT;    NW_MAKE_PLP_TYPES_BY(SHORT);  // 16 bit s
typedef uint16_t                       USHORT;   NW_MAKE_PLP_TYPES_BY(USHORT); // 16 bit u

// These types should be 32 bits wide:
#if __SIZEOF_LONG__ == __SIZEOF_INT__
// Native longs are 32 bits, use native longs
// Note: We use different types for INT and LONG
// This gives some overloaded functions more chances to compile
typedef int32_t                        INT;      // WinDef.h
typedef uint32_t                       UINT;     // WinDef.h
typedef uint32_t                       DWORD;    // IntSafe.h
typedef signed long                    LONG;     // WinNT.h
typedef unsigned long                  ULONG;    // WinNT.h
#define NW_PRINTF_FSTR_INT             "d"
#define NW_PRINTF_FSTR_UINT            "u"
#define NW_PRINTF_FSTR_DWORD           "u"
#define NW_PRINTF_FSTR_LONG            "ld"
#define NW_PRINTF_FSTR_ULONG           "lu"
#else
// Native longs are 64 bits, use portable types
typedef int32_t                        INT;      // WinDef.h
typedef uint32_t                       UINT;     // WinDef.h
typedef uint32_t                       DWORD;    // IntSafe.h
typedef int32_t                        LONG;     // WinNT.h
typedef uint32_t                       ULONG;    // WinNT.h
#define NW_PRINTF_FSTR_INT             "d"
#define NW_PRINTF_FSTR_UINT            "u"
#define NW_PRINTF_FSTR_DWORD           "u"
#define NW_PRINTF_FSTR_LONG            "d"
#define NW_PRINTF_FSTR_ULONG           "u"
#endif

NW_MAKE_PLP_TYPES_BY(INT);
NW_MAKE_PLP_TYPES_BY(UINT);
NW_MAKE_PLP_TYPES_BY(DWORD);
NW_MAKE_PLP_TYPES_BY(LONG);
NW_MAKE_PLP_TYPES_BY(ULONG);

typedef int64_t                        LONGLONG;  NW_MAKE_PLP_TYPES_BY(LONGLONG);  // WinNT.h
typedef uint64_t                       ULONGLONG; NW_MAKE_PLP_TYPES_BY(ULONGLONG); // WinNT.h
typedef uint64_t                       DWORDLONG; NW_MAKE_PLP_TYPES_BY(DWORDLONG); // IntSafe.h

#define NW_PRINTF_FSTR_LONGLONG        "lld"
#define NW_PRINTF_FSTR_ULONGLONG       "llu"

// BaseTsd.h

typedef int8_t                         INT8;     NW_MAKE_PLP_TYPES_BY(INT8);
typedef uint8_t                        UINT8;    NW_MAKE_PLP_TYPES_BY(UINT8);
typedef int16_t                        INT16;    NW_MAKE_PLP_TYPES_BY(INT16);
typedef uint16_t                       UINT16;   NW_MAKE_PLP_TYPES_BY(UINT16);
typedef int32_t                        INT32;    NW_MAKE_PLP_TYPES_BY(INT32);
typedef uint32_t                       UINT32;   NW_MAKE_PLP_TYPES_BY(UINT32);
typedef int64_t                        INT64;    NW_MAKE_PLP_TYPES_BY(INT64);
typedef uint64_t                       UINT64;   NW_MAKE_PLP_TYPES_BY(UINT64);
typedef intptr_t                       INT_PTR;
typedef uintptr_t                      UINT_PTR;

// BaseTsd.h : Exotic

typedef int32_t                        LONG32;   NW_MAKE_PLP_TYPES_BY(LONG32);
typedef uint32_t                       ULONG32;  NW_MAKE_PLP_TYPES_BY(ULONG32);
typedef uint32_t                       DWORD32;  NW_MAKE_PLP_TYPES_BY(DWORD32);
typedef int64_t                        LONG64;   NW_MAKE_PLP_TYPES_BY(LONG64);
typedef uint64_t                       ULONG64;  NW_MAKE_PLP_TYPES_BY(ULONG64);
typedef uint64_t                       DWORD64;  NW_MAKE_PLP_TYPES_BY(DWORD64);

// BaseTsd.h : Event more exotic

typedef intptr_t                       LONG_PTR;
typedef uintptr_t                      ULONG_PTR;
typedef uintptr_t                      DWORD_PTR;

// WinNT.h

typedef char                           CHAR;
typedef unsigned char                  UCHAR;
typedef wchar_t                        WCHAR;

// WinNT.h string types

typedef char                          *PSTR;
typedef char FAR                      *LPSTR;
typedef wchar_t                       *PWSTR;
typedef wchar_t FAR                   *LPWSTR;
typedef const char                    *PCSTR;
typedef const char FAR                *LPCSTR;
typedef const wchar_t                 *PCWSTR;
typedef const wchar_t FAR             *LPCWSTR;

typedef float                          FLOAT;    NW_MAKE_PLP_TYPES_BY(FLOAT);   // WinDef.h

// Boolean types

typedef int                            BOOL;     NW_MAKE_PLP_TYPES_BY(BOOL);    // WinDef.h
typedef BYTE                           BOOLEAN;  NW_MAKE_PLP_TYPES_BY(BOOLEAN); // WinNT.h 

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

// size_t

typedef size_t                         SIZE_T;   NW_MAKE_PLP_TYPES_BY(SIZE_T);  // BaseTsd.h
typedef intptr_t                       SSIZE_T;  NW_MAKE_PLP_TYPES_BY(SSIZE_T); // BaseTsd.h // (signed version of size_t, can overflow)

//  WinNT.h: The limits

#define MINCHAR                 (-128)
#define MAXCHAR                 (0x7f)
#define MINSHORT                (-32768)
#define MAXSHORT                (0x7fff)
#define MINLONG                 (0x80000000) // ?
#define MAXLONG                 (0x7fffffff)
#define MAXBYTE                 (0xff)
#define MAXWORD                 (0xffff)
#define MAXDWORD                (0xffffffff)

// BaseTsd.h (?)

#define MAXUINT8                (0xff)
#define MAXUINT16               (0xffff)
#define MAXUINT32               (0xffffffff)
#define MININT8                 MINCHAR
#define MAXINT8                 MAXCHAR
#define MININT16                MINSHORT
#define MAXINT16                MAXSHORT
#define MININT32                MINLONG   // same 32 bit size
#define MAXINT32                MAXLONG   // same 32 bit size

#define MAXUINT                 MAXUINT32
#define MININT                  MININT32
#define MAXINT                  MAXINT32

// Handle types
// -----------------------------------------------------------------------

// A handle to an object (most of objects use handles dervied from it [but not all])
typedef PVOID                          HANDLE;

#define NW_HANDLE_SPEC_VALUE(VALUE)    ((HANDLE)((intptr_t)(VALUE)))
#define INVALID_HANDLE_VALUE           NW_HANDLE_SPEC_VALUE(-1) // Note: Under win32 defibed as "-1", so it should differ from NULL

// Dll-related types
// -----------------------------------------------------------------------

// A handle to an instance.
// [Win32]: This is the base address of the module in memory.
// [NanoWin]: This is the address of some module-related structure.
// HMODULE and HINSTANCE are the same today, but represented different things in 16-bit Windows.
typedef HANDLE                         HINSTANCE; 

// A handle to a module.
// [Win32]: The is the base address of the module in memory.
// [NanoWin]: This is the address of some module-related structure.
// HMODULE and HINSTANCE are the same today, but represented different things in 16-bit Windows.
typedef HINSTANCE                      HMODULE;

// Address of procedure (code address)
typedef void*  FAR                     FARPROC;

// Win32 Interface compiller
// -----------------------------------------------------------------------

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

// The Microsoft source-code annotation language (SAL) subset
// -----------------------------------------------------------------------
// Used by code analysys tools
// Good practice to annotate at least pointer parameters

#define _In_          // Input to called function
#define _Inout_       // Input to called function, and output to caller
#define _Out_         // Output to caller
#define _Outptr_      // Output of pointer to caller [pointer-to-pointer]

#define _In_opt_      // Input to called function [optional]
#define _Inout_opt_   // Input to called function, and output to caller  [optional]
#define _Out_opt_     // Output to caller  [optional]
#define _Outptr_opt_  // Output of pointer to caller [pointer-to-pointer] [optional]

#define _Success_(x)  // Success criteria for function (to enforce _Out_* annotations)

// Some other annotations

#define _In_z_
#define _Out_writes_z_(n)

// Some exotic generic types
// -----------------------------------------------------------------------

typedef union _LARGE_INTEGER
{
  struct 
  {
    DWORD LowPart;
    LONG  HighPart;
  };
  struct
  {
    DWORD LowPart;
    LONG  HighPart;
  } u;
  LONGLONG QuadPart;
} LARGE_INTEGER;

NW_MAKE_PLP_TYPES_BY(LARGE_INTEGER);

typedef double DOUBLE; // WTypesbase.h (not explicitely documented as Win32 type)

// MIDL data types
// -----------------------------------------------------------------------
// Other MIDL base types / predefined types (rpcndr.h)

typedef unsigned char byte;
typedef unsigned char boolean;

// To expotic, hope nobody used it directly
// typedef byte          cs_byte; 
// #define hyper         long long // __int64
// #define MIDL_uhyper   unsigned long long // unsigned __int64

// errno_t
// -----------------------------------------------------------------------
// Looks like may be defined implicitely by windows.h?

#if !defined(NW_ERRNO_T_DEFINED)
#define NW_ERRNO_T_DEFINED
typedef int errno_t; // Not defined by GNU C (standard assumed errno is int always)
#endif

// NULLONLY Version of standard types
// -----------------------------------------------------------------------
// Used to restrict implemented param subset

NW_MAKE_PTYPE_NULLONLY(PSTR);
NW_MAKE_PTYPE_NULLONLY(LPSTR);
NW_MAKE_PTYPE_NULLONLY(PWSTR);
NW_MAKE_PTYPE_NULLONLY(LPWSTR);
NW_MAKE_PTYPE_NULLONLY(PCSTR);
NW_MAKE_PTYPE_NULLONLY(LPCSTR);
NW_MAKE_PTYPE_NULLONLY(PCWSTR);
NW_MAKE_PTYPE_NULLONLY(LPCWSTR);

// Other usefull stuff
// -----------------------------------------------------------------------

#define NW_WIDE_STR(s)                 L##s
#define NW_FORCE_COMPILE_ERROR         ....|....<....>....|.... // This will force compile error
// More macroses like BUILD_BUG() used on linux
#define NW_BUILD_BUG()                 NW_FORCE_COMPILE_ERROR

// Some aux macroses (maybe later we have to add refix to them?)
// -----------------------------------------------------------------------

// Security attribures
typedef void SECURITY_ATTRIBUTES; // Not supported as var
typedef SECURITY_ATTRIBUTES FAR  *LPSECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES      *PSECURITY_ATTRIBUTES;

// Locale-related definitions
typedef DWORD LCID;
#define LOCALE_USER_DEFAULT   (0x0400)
#define LOCALE_SYSTEM_DEFAULT (0x0800)

#define LANG_NEUTRAL          (0x00)
#define SUBLANG_DEFAULT       (0x01)

#define MAKELANGID(lang,sublang)  (((WORD)(sublang) << 10) | ((WORD)(lang)))


// Constants for sync. objects
// -----------------------------------------------------------------------

#define MAXIMUM_WAIT_OBJECTS      (64)

#define WAIT_OBJECT_0             (0x0)  // STATUS_WAIT+0
#define WAIT_ABANDONED_0          (0x80)
#define WAIT_FAILED               (0xFFFFFFFF)
#define WAIT_TIMEOUT              (0x102)

#define INFINITE                  (0xFFFFFFFF)

// Other misc stuff
// -----------------------------------------------------------------------

#ifndef NW_EXTERN_C
#if defined(__cplusplus)
#define NW_EXTERN_C extern "C"
#else
#define NW_EXTERN_C extern
#endif
#endif

#ifndef NW_EXTERN
#define NW_EXTERN  extern // for Cpp functions
#endif

#ifndef NW_EXTERN_C_BEGIN
#ifdef __cplusplus
#define NW_EXTERN_C_BEGIN  extern "C" {
#define NW_EXTERN_C_END    }
#else
#define NW_EXTERN_C_BEGIN  // None
#define NW_EXTERN_C_END    // None
#endif
#endif

#endif // linux

// Usefull macroses for all platforms
#define NW_NOCOPY_CONSTRUCTOR(ClassName) private: ClassName(const ClassName &src) { }
#define NW_NOASSIGN_OPERATOR(ClassName)  private: void operator= (const ClassName &src) { }

#define NW_UNREFERENCED_PARAMETER(p)     if ((&(p)) != NULL) { }

#endif // ...Included
