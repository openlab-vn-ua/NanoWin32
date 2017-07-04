// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible printf format support (removes differences between processing
// %s,%c in "wide" printf functions between MSVC and GLIBC)

#include <wctype.h>
#include <NanoWinMsPrintf.h>

#if defined(__GNUC__)

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
  return ch == L'h' || ch == L'l' || ch == L'H' || ch == L'L';
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

  while (is_format_type_modifier(*src))
  {
    *dst++ = *src++;

    has_type_modifier = true;
  }

  if (!has_type_modifier)
  {
    if (*src == L's')
    {
      src++;
      *dst++ = L'S';
    }
    else if (*src == L'c')
    {
      src++;
      *dst++ = L'C';
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

NW_EXTERN_C void NanoWinMsPrintfWFormatMs2Unix(wchar_t *destFormat, const wchar_t *srcFormat)
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

#endif

