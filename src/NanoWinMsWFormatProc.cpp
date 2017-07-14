// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible printf format processor support
// (fixes differences between processing %s,%c in "wide" printf functions between MSVC and GLIBC)
// This file defines proccessor to update format line (it does not remap functions)

#if defined(__GNUC__)

#include "NanoWinMsWFormatProc.h"
#include <wctype.h>

// Format translation proccessor
// -----------------------------------------

static bool is_format_flag(wchar_t ch)
{
  return ch == L'-' || ch == L'+' || ch == L' ' || ch == L'#' || ch == L'\'' || ch == L'0';
}

static bool is_format_digit(wchar_t ch)
{
  return iswdigit(ch);
}

static bool is_format_width(wchar_t ch)
{
  return is_format_digit(ch);
}

static bool is_format_precision(wchar_t ch)
{
  return is_format_digit(ch);
}

static bool is_format_type_modifier(wchar_t ch)
{
  return ch == L'h' || ch == L'l' || ch == L'L' || ch == L'w' || ch == L'I';
}

static void process_field(wchar_t **ddst, const wchar_t **ssrc)
{
  wchar_t       *dst = *ddst;
  const wchar_t *src = *ssrc;

  *dst++ = *src++; // copy 'percent' char

  while (is_format_flag(*src))
  {
    *dst++ = *src++;
  }

  while (is_format_width(*src))
  {
    *dst++ = *src++;
  }

  if (*src == L'.')
  {
    *dst++ = *src++;

    while (is_format_precision(*src))
    {
      *dst++ = *src++;
    }
  }

  bool has_type_modifier = false;

  // Note, that MS supports also 'I32' and 'I64' type modifiers, which are not
  // directly supported here, but since we're not interested in integer fields
  // it doesn't change results of this function

  if (*src == L'w')
  {
    src++;

    if (*src == L's' || *src == L'S' || *src == L'c' || *src == L'C')
    {
      *dst++ = L'l';
    }
    else
    {
      *dst++ = L'w'; // unsupported combination, so just leave it "as-is"
    }

    has_type_modifier = true;
  }
  else
  {
    while (is_format_type_modifier(*src))
    {
      *dst++ = *src++;

      has_type_modifier = true;
    }
  }

  if (!has_type_modifier)
  {
    if      (*src == L's')
    {
      src++;
      *dst++ = L'S';
    }
    else if (*src == L'S')
    {
      src++;
      *dst++ = L's';
    }
    else if (*src == L'c')
    {
      src++;
      *dst++ = L'C';
    }
    else if (*src == L'C')
    {
      src++;
      *dst++ = L'c';
    }
    else if (*src != L'\0')
    {
      *dst++ = *src++;
    }
  }
  else
  {
    if (*src != L'\0')
    {
      *dst++ = *src++;
    }
  }

  *ssrc = src;
  *ddst = dst;
}

NW_EXTERN_C void NanoWinMsWFormatProcMs2Unix(wchar_t *destFormat, const wchar_t *srcFormat)
{
  const wchar_t *src = srcFormat;
  wchar_t       *dst = destFormat;

  while (*src != L'\0')
  {
    if (*src == L'%')
    {
      process_field(&dst, &src);
    }
    else
    {
      *dst++ = *src++;
    }
  }

  *dst = L'\0';
}

// Function redefinition tools
// -----------------------------------------

static thread_local NanoWin::WFormatLine NanoWinMsWFormatProcMs2UnixCGate(0);

NW_EXTERN_C const wchar_t *NanoWinMsWFormatProcMs2UnixCFormatGate  (const wchar_t *src_format)
{
  NanoWinMsWFormatProcMs2UnixCGate.reset(NanoWinMsWFormatProcMs2UnixRequiredLength(src_format));
  NanoWinMsWFormatProcMs2Unix(NanoWinMsWFormatProcMs2UnixCGate.data(), src_format);
  return(NanoWinMsWFormatProcMs2UnixCGate.data());
}

NW_EXTERN_C intptr_t       NanoWinMsWFormatProcMs2UnixCResultFree (intptr_t       the_result)
{
  NanoWinMsWFormatProcMs2UnixCGate.reset(0);
  return(the_result);
}

#endif // __GNUC__
