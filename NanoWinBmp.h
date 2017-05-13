// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Bmp functions

#if !defined(NanoWinBmpIncluded)
#define NanoWinBmpIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

#if defined(__GNUC__)
#define NW_PACKED_ATTR   __attribute__((packed)) 
#else
#define NW_PACKED_ATTR   // nothing
#endif

// Win32 Bitmap structures
// -------------------------------------------------

typedef struct tagBITMAPFILEHEADER
{
  WORD  bfType;        // Magic identifier
  DWORD bfSize;        // File size in bytes
  WORD  bfReserved1;   //
  WORD  bfReserved2;   // 
  DWORD bfOffBits;     // Offset to image data, bytes
} NW_PACKED_ATTR BITMAPFILEHEADER; NW_MAKE_PLP_TYPES_BY(BITMAPFILEHEADER);

#define NW_BM_TYPE (0x4D42) // NanoWin: This works only for little endian

typedef struct tagRGBQUAD
{
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
}  NW_PACKED_ATTR RGBQUAD; // NW_MAKE_PLP_TYPES_BY(RGBQUAD);

// V3 header

typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} NW_PACKED_ATTR BITMAPINFOHEADER; NW_MAKE_PLP_TYPES_BY(BITMAPINFOHEADER);

// Constants

#define BI_RGB 0  // biCompression

// V4 header

typedef UINT32 FXPT2DOT30; // [...] FXPT2DOT30 [...] which means that they are interpreted as fixed-point values with a 2-bit integer part and a 30-bit fractional part.

typedef struct tagCIEXYZ
{
  FXPT2DOT30 ciexyzX;
  FXPT2DOT30 ciexyzY;
  FXPT2DOT30 ciexyzZ;
} NW_PACKED_ATTR CIEXYZ;  NW_MAKE_PLP_TYPES_BY(CIEXYZ);

typedef struct tagCIEXYZTRIPLE
{
  CIEXYZ ciexyzRed;
  CIEXYZ ciexyzGreen;
  CIEXYZ ciexyzBlue;
} NW_PACKED_ATTR CIEXYZTRIPLE;  NW_MAKE_PLP_TYPES_BY(CIEXYZTRIPLE);

typedef struct
{
  DWORD        bV4Size;
  LONG         bV4Width;
  LONG         bV4Height;
  WORD         bV4Planes;
  WORD         bV4BitCount;
  DWORD        bV4V4Compression;
  DWORD        bV4SizeImage;
  LONG         bV4XPelsPerMeter;
  LONG         bV4YPelsPerMeter;
  DWORD        bV4ClrUsed;
  DWORD        bV4ClrImportant;
  // V4 fields
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4CSType;
  CIEXYZTRIPLE bV4Endpoints;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;
} NW_PACKED_ATTR BITMAPV4HEADER;  NW_MAKE_PLP_TYPES_BY(BITMAPV4HEADER);

// V5 header

typedef struct 
{
  DWORD        bV5Size;
  LONG         bV5Width;
  LONG         bV5Height;
  WORD         bV5Planes;
  WORD         bV5BitCount;
  DWORD        bV5Compression;
  DWORD        bV5SizeImage;
  LONG         bV5XPelsPerMeter;
  LONG         bV5YPelsPerMeter;
  DWORD        bV5ClrUsed;
  DWORD        bV5ClrImportant;
  // V4 fields
  DWORD        bV5RedMask;
  DWORD        bV5GreenMask;
  DWORD        bV5BlueMask;
  DWORD        bV5AlphaMask;
  DWORD        bV5CSType;
  CIEXYZTRIPLE bV5Endpoints;
  DWORD        bV5GammaRed;
  DWORD        bV5GammaGreen;
  DWORD        bV5GammaBlue;
  // V5 fields
  DWORD        bV5Intent;
  DWORD        bV5ProfileData;
  DWORD        bV5ProfileSize;
  DWORD        bV5Reserved;
} NW_PACKED_ATTR BITMAPV5HEADER; NW_MAKE_PLP_TYPES_BY(BITMAPV5HEADER);

// MS COLORREF type and related macroses
// -------------------------------------------------

typedef DWORD COLORREF; NW_MAKE_PLP_TYPES_BY(COLORREF);

#define RGB(r,g,b)       ( (DWORD) ( (((DWORD)((BYTE)(r)))) | (((DWORD)((BYTE)(g))) << 8) | (((DWORD)((BYTE)(b))) << 16) ) )

#define GetRValue(rgb)   ((BYTE) (rgb))
#define GetGValue(rgb)   ((BYTE) (((WORD) (rgb)) >> 8))
#define GetBValue(rgb)   ((BYTE) ((rgb) >> 16))

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
