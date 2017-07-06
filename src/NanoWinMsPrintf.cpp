// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible printf format support (removes differences between processing
// %s,%c in "wide" printf functions between MSVC and GLIBC)

#if defined(__GNUC__)

#if !defined(NanoWinUseNativePrintfFormat)

#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static auto NanoWinMsPrintfNativeVSWPrintf = &vswprintf;
static auto NanoWinMsPrintfNativeVWPrintf  = &vwprintf;
static auto NanoWinMsPrintfNativeVFWPrintf = &vfwprintf;

#include <wctype.h>
#include <NanoWinMsPrintf.h>

namespace
{
  template<typename T, size_t initialSize = 256>
  class NanoWinPreAllocatedBuffer
  {
    public:

    NanoWinPreAllocatedBuffer (size_t size)
    {
      if (size <= initialSize)
      {
        buffer = preAllocatedBuffer;
      }
      else
      {
        buffer = new T[size];
      }
    }

    ~NanoWinPreAllocatedBuffer()
    {
      if (buffer != preAllocatedBuffer)
      {
        delete[] buffer;
      }
    }

    NanoWinPreAllocatedBuffer (const NanoWinPreAllocatedBuffer &src) = delete;
    NanoWinPreAllocatedBuffer (NanoWinPreAllocatedBuffer &&src) = delete;

    NanoWinPreAllocatedBuffer& operator = (const NanoWinPreAllocatedBuffer &src) = delete;
    NanoWinPreAllocatedBuffer& operator = (NanoWinPreAllocatedBuffer &&src) = delete;

    T       *data()       { return buffer; }
    const T *data() const { return buffer; }

    private:

    T  preAllocatedBuffer[initialSize];
    T *buffer;
  };
}


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

NW_EXTERN_C int NanoWinMsSWPrintf(wchar_t *buffer, size_t count, const wchar_t *format, ...)
{
  va_list argptr;

  va_start(argptr,format);

  int result = NanoWinMsVSWPrintf(buffer,count,format,argptr);

  va_end(argptr);

  return result;
}

NW_EXTERN_C int NanoWinMsVSWPrintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr)
{
  int result;

  try
  {
    NanoWinPreAllocatedBuffer<wchar_t> nativeFormat(wcslen(format) + 1);

    NanoWinMsPrintfWFormatMs2Unix(nativeFormat.data(),format);

    result = NanoWinMsPrintfNativeVSWPrintf(buffer,count,nativeFormat.data(),argptr);
  }
  catch (...)
  {
    result = -1;
    errno  = ENOMEM;
  }

  return result;
}

NW_EXTERN_C int NanoWinMsWPrintf(const wchar_t *format, ...)
{
  va_list argptr;

  va_start(argptr,format);

  int result = NanoWinMsVWPrintf(format,argptr);

  va_end(argptr);

  return result;
}

NW_EXTERN_C int NanoWinMsVWPrintf(const wchar_t *format, va_list argptr)
{
  int result;

  try
  {
    NanoWinPreAllocatedBuffer<wchar_t> nativeFormat(wcslen(format) + 1);

    NanoWinMsPrintfWFormatMs2Unix(nativeFormat.data(),format);

    result = NanoWinMsPrintfNativeVWPrintf(nativeFormat.data(),argptr);
  }
  catch (...)
  {
    result = -1;
    errno  = ENOMEM;
  }

  return result;
}

NW_EXTERN_C int NanoWinMsFWPrintf(FILE *stream, const wchar_t *format, ...)
{
  va_list argptr;

  va_start(argptr,format);

  int result = NanoWinMsVFWPrintf(stream,format,argptr);

  va_end(argptr);

  return result;
}

NW_EXTERN_C int NanoWinMsVFWPrintf(FILE *stream, const wchar_t *format, va_list argptr)
{
  int result;

  try
  {
    NanoWinPreAllocatedBuffer<wchar_t> nativeFormat(wcslen(format) + 1);

    NanoWinMsPrintfWFormatMs2Unix(nativeFormat.data(),format);

    result = NanoWinMsPrintfNativeVFWPrintf(stream,nativeFormat.data(),argptr);
  }
  catch (...)
  {
    result = -1;
    errno  = ENOMEM;
  }

  return result;
}

#endif // NanoWinUseNativePrintfFormat

#endif // __GNUC__
