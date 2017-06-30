// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe sscanf_s, fscanf_s extensions

#include <stdio.h>
#include <stdarg.h>

#include <sstream>

#include "NanoWinMsSafeScanf.h"

#define invoke_err_handler(etext,earg,errcode) // TODO: call handler here
#define return_after_err_handler(etext,earg,errcode) return(errcode)

namespace
{
  class InputFailure {};
  class OutOfRangeFailure {};

  class CharTraitsChar
  {
    public :

    typedef char char_type;
    typedef int  char_result_type;

    static constexpr char_type        null = '\0';
    static constexpr char_result_type eof  = EOF;

    static bool isspace (char_result_type ch) { return ::isspace(ch); };
    static int  snprintf(char_type *str, size_t size, const char_type *format, ...)
    {
      va_list args;

      va_start(args,format);

      int result = vsnprintf(str,size,format,args);

      va_end(args);

      return result;
    }

    static int sscanf(const char_type *str, const char_type *format, ...)
    {
      va_list args;

      va_start(args,format);

      int result = vsscanf(str,format,args);

      va_end(args);

      return result;
    }

    static char_result_type fgetc(FILE *stream)
    {
      return ::fgetc(stream);
    }

    static char_result_type ungetc(char_result_type ch, FILE *stream)
    {
      return ::ungetc(ch,stream);
    }

    static int fscanf(FILE *stream, const char_type *format, ...)
    {
      va_list args;

      va_start(args,format);

      int result = vfscanf(stream,format,args);

      va_end(args);

      return result;
    }

    static constexpr char Char_d = 'd';
    static constexpr char Char_i = 'i';
    static constexpr char Char_u = 'u';
    static constexpr char Char_o = 'o';
    static constexpr char Char_x = 'x';
    static constexpr char Char_X = 'X';
    static constexpr char Char_f = 'f';
    static constexpr char Char_F = 'F';
    static constexpr char Char_e = 'e';
    static constexpr char Char_E = 'E';
    static constexpr char Char_g = 'g';
    static constexpr char Char_G = 'G';
    static constexpr char Char_h = 'h';
    static constexpr char Char_l = 'l';
    static constexpr char Char_s = 's';
    static constexpr char Char_c = 'c';
    static constexpr char Char_n = 'n';

    static constexpr char StartFieldChar = '%';
    static constexpr char SuppressAssignmentChar = '*';

    static constexpr const char *Str_d = "d";
    static constexpr const char *Str_i = "i";
    static constexpr const char *Str_u = "u";
    static constexpr const char *Str_o = "o";
    static constexpr const char *Str_x = "x";
    static constexpr const char *Str_X = "X";
    static constexpr const char *Str_f = "f";
    static constexpr const char *Str_F = "F";
    static constexpr const char *Str_e = "e";
    static constexpr const char *Str_E = "E";
    static constexpr const char *Str_g = "g";
    static constexpr const char *Str_G = "G";

    static constexpr const char *Str_hhd = "hhd";
    static constexpr const char *Str_hhi = "hhi";
    static constexpr const char *Str_hhu = "hhu";
    static constexpr const char *Str_hho = "hho";
    static constexpr const char *Str_hhx = "hhx";
    static constexpr const char *Str_hhX = "hhX";

    static constexpr const char *Str_hd = "hd";
    static constexpr const char *Str_hi = "hi";
    static constexpr const char *Str_hu = "hu";
    static constexpr const char *Str_ho = "ho";
    static constexpr const char *Str_hx = "hx";
    static constexpr const char *Str_hX = "hX";

    static constexpr const char *Str_lld = "lld";
    static constexpr const char *Str_lli = "lli";
    static constexpr const char *Str_llu = "llu";
    static constexpr const char *Str_llo = "llo";
    static constexpr const char *Str_llx = "llx";
    static constexpr const char *Str_llX = "llX";

    static constexpr const char *Str_ld = "ld";
    static constexpr const char *Str_li = "li";
    static constexpr const char *Str_lu = "lu";
    static constexpr const char *Str_lo = "lo";
    static constexpr const char *Str_lx = "lx";
    static constexpr const char *Str_lX = "lX";
    static constexpr const char *Str_le = "le";
    static constexpr const char *Str_lE = "lE";
    static constexpr const char *Str_lf = "lf";
    static constexpr const char *Str_lF = "lF";
    static constexpr const char *Str_lg = "lg";
    static constexpr const char *Str_lG = "lG";

    static constexpr const char *ScanAnyWithWidthFormatStr = "%%%d%s%%n";
    static constexpr const char *ScanAnyFormatStr          = "%%%s%%n";

    static constexpr const char *ScanAnyWithWidthFileFormatStr = "%%%d%s";
    static constexpr const char *ScanAnyFileFormatStr          = "%%%s";
  };

  class CharTraitsWideChar
  {
    public :

    typedef wchar_t char_type;
    typedef wint_t  char_result_type;

    static constexpr char_type        null = L'\0';
    static constexpr char_result_type eof  = WEOF;

    static bool isspace (char_result_type ch) { return iswspace(ch); };

    static int  snprintf(char_type *str, size_t size, const char_type *format, ...)
    {
      va_list args;

      va_start(args,format);

      int result = vswprintf(str,size,format,args); // strictly speaking vswprintf is not analog of vsnprintf, since vswprintf returns -1 on buffer overflow, but this not critical here

      va_end(args);

      return result;
    }

    static int sscanf(const char_type *str, const char_type *format, ...)
    {
      va_list args;

      va_start(args,format);

      int result = vswscanf(str,format,args);

      va_end(args);

      return result;
    }

    static char_result_type fgetc(FILE *stream)
    {
      return fgetwc(stream);
    }

    static char_result_type ungetc(char_result_type ch, FILE *stream)
    {
      return ungetwc(ch,stream);
    }

    static int fscanf(FILE *stream, const char_type *format, ...)
    {
      va_list args;

      va_start(args,format);

      int result = vfwscanf(stream,format,args);

      va_end(args);

      return result;
    }

    static constexpr wchar_t Char_d = L'd';
    static constexpr wchar_t Char_i = L'i';
    static constexpr wchar_t Char_u = L'u';
    static constexpr wchar_t Char_o = L'o';
    static constexpr wchar_t Char_x = L'x';
    static constexpr wchar_t Char_X = L'X';
    static constexpr wchar_t Char_f = L'f';
    static constexpr wchar_t Char_F = L'F';
    static constexpr wchar_t Char_e = L'e';
    static constexpr wchar_t Char_E = L'E';
    static constexpr wchar_t Char_g = L'g';
    static constexpr wchar_t Char_G = L'G';
    static constexpr wchar_t Char_h = L'h';
    static constexpr wchar_t Char_l = L'l';
    static constexpr wchar_t Char_s = L's';
    static constexpr wchar_t Char_c = L'c';
    static constexpr wchar_t Char_n = L'n';

    static constexpr wchar_t StartFieldChar = L'%';
    static constexpr wchar_t SuppressAssignmentChar = L'*';

    static constexpr const wchar_t *Str_d = L"d";
    static constexpr const wchar_t *Str_i = L"i";
    static constexpr const wchar_t *Str_u = L"u";
    static constexpr const wchar_t *Str_o = L"o";
    static constexpr const wchar_t *Str_x = L"x";
    static constexpr const wchar_t *Str_X = L"X";
    static constexpr const wchar_t *Str_f = L"f";
    static constexpr const wchar_t *Str_F = L"F";
    static constexpr const wchar_t *Str_e = L"e";
    static constexpr const wchar_t *Str_E = L"E";
    static constexpr const wchar_t *Str_g = L"g";
    static constexpr const wchar_t *Str_G = L"G";

    static constexpr const wchar_t *Str_hhd = L"hhd";
    static constexpr const wchar_t *Str_hhi = L"hhi";
    static constexpr const wchar_t *Str_hhu = L"hhu";
    static constexpr const wchar_t *Str_hho = L"hho";
    static constexpr const wchar_t *Str_hhx = L"hhx";
    static constexpr const wchar_t *Str_hhX = L"hhX";

    static constexpr const wchar_t *Str_hd = L"hd";
    static constexpr const wchar_t *Str_hi = L"hi";
    static constexpr const wchar_t *Str_hu = L"hu";
    static constexpr const wchar_t *Str_ho = L"ho";
    static constexpr const wchar_t *Str_hx = L"hx";
    static constexpr const wchar_t *Str_hX = L"hX";

    static constexpr const wchar_t *Str_lld = L"lld";
    static constexpr const wchar_t *Str_lli = L"lli";
    static constexpr const wchar_t *Str_llu = L"llu";
    static constexpr const wchar_t *Str_llo = L"llo";
    static constexpr const wchar_t *Str_llx = L"llx";
    static constexpr const wchar_t *Str_llX = L"llX";

    static constexpr const wchar_t *Str_ld = L"ld";
    static constexpr const wchar_t *Str_li = L"li";
    static constexpr const wchar_t *Str_lu = L"lu";
    static constexpr const wchar_t *Str_lo = L"lo";
    static constexpr const wchar_t *Str_lx = L"lx";
    static constexpr const wchar_t *Str_lX = L"lX";
    static constexpr const wchar_t *Str_le = L"le";
    static constexpr const wchar_t *Str_lE = L"lE";
    static constexpr const wchar_t *Str_lf = L"lf";
    static constexpr const wchar_t *Str_lF = L"lF";
    static constexpr const wchar_t *Str_lg = L"lg";
    static constexpr const wchar_t *Str_lG = L"lG";

    static constexpr const wchar_t *ScanAnyWithWidthFormatStr     = L"%%%d%ls%%n";
    static constexpr const wchar_t *ScanAnyFormatStr              = L"%%%ls%%n";
    static constexpr const wchar_t *ScanAnyWithWidthFileFormatStr = L"%%%d%ls";
    static constexpr const wchar_t *ScanAnyFileFormatStr          = L"%%%ls";
  };
 
  template<typename CharTraits>
  class InputStream
  {
    public :

    typedef typename CharTraits::char_type         char_type;
    typedef typename CharTraits::char_result_type  char_result_type;

    virtual bool eof () = 0;
    virtual char_result_type readChar () = 0;
    virtual void unreadChar (char_result_type ch) = 0;
    virtual void skipSpaces () = 0;
    virtual off64_t getReadCharCount () = 0;

    virtual int  scanAny (const char_type *typeFormat, unsigned int fieldWidth, void *value) = 0;

    protected :
    // %[*][width]lld
    enum { FIELD_WIDTH_MAX_WIDTH = 20}; // 20 decimal digits is enough to represent 64-bit value
    enum { MAX_FIELD_SPECIFIER_LEN = 1 + 1 + FIELD_WIDTH_MAX_WIDTH + 3};
  };
  
  template<typename CharTraits>
  class InputStreamString : public InputStream<CharTraits>
  {
    public :

    typedef typename CharTraits::char_type         char_type;
    typedef typename CharTraits::char_result_type  char_result_type;

    InputStreamString(const char_type *src)
    {
      str = src;
      pos = 0;
    }

    virtual bool eof()
    {
      return str[pos] == CharTraits::null;
    }

    virtual char_result_type readChar()
    {
      char_result_type ch = str[pos];

      if (ch != CharTraits::null)
      {
        pos++;
      }
      else
      {
        throw InputFailure();
      }

      return ch;
    }

    virtual void unreadChar(char_result_type ch)
    {
      if (pos > 0)
      {
        if (str[--pos] != (char_type)ch)
        {
          throw InputFailure();
        }
      }
      else
      {
        throw InputFailure();
      }
    }

    virtual void skipSpaces()
    {
      while (!eof() && CharTraits::isspace(str[pos]))
      {
        pos++;
      }
    }
 
    virtual int scanAny(const char_type *typeFormat, unsigned int fieldWidth, void *value)
    {
      char_type format[this->MAX_FIELD_SPECIFIER_LEN * 2 + 1];

      if (fieldWidth > 0)
      {
        CharTraits::snprintf(format,sizeof(format) / sizeof(format[0]),CharTraits::ScanAnyWithWidthFormatStr,fieldWidth,typeFormat);
      }
      else
      {
        CharTraits::snprintf(format,sizeof(format) / sizeof(format[0]),CharTraits::ScanAnyFormatStr,typeFormat);
      }

      int len = 0;
      int fieldCount = CharTraits::sscanf(&str[pos], format, value, &len);

      pos += len;

      if ((char_result_type)fieldCount == CharTraits::eof)
      {
        throw InputFailure();
      }

      return fieldCount;
    }

    virtual off64_t getReadCharCount()
    {
      return pos;
    }

    private :

    const char_type *str;
    size_t           pos;
  };

  template<typename CharTraits>
  class InputStreamFile : public InputStream<CharTraits>
  {
    public :

    typedef typename CharTraits::char_type         char_type;
    typedef typename CharTraits::char_result_type  char_result_type;

    InputStreamFile(FILE *file)
    {
      stream = file;

      startOffset = ftello64(file);

      // in case of error set offset to default value
      if (startOffset == -1)
      {
        startOffset = 0;
      }
    }

    virtual bool eof()
    {
      return feof(stream);
    }

    virtual char_result_type  readChar()
    {
      char_result_type ch = CharTraits::fgetc(stream);

      if (ch == CharTraits::eof)
      {
        if (ferror(stream))
        {
          throw InputFailure();
        }
      }

      return ch;
    }

    virtual void unreadChar (char_result_type ch)
    {
      if (CharTraits::ungetc(ch, stream) == CharTraits::eof)
      {
        throw InputFailure();
      }
    }

    virtual void skipSpaces()
    {
      char_result_type ch;

      do 
      {
        ch = CharTraits::fgetc(stream);
      } while (CharTraits::isspace(ch));

      if (ch == CharTraits::eof)
      {
        if (ferror(stream))
        {
          throw InputFailure();
        }
      }
      else
      {
        if (CharTraits::ungetc(ch, stream) == CharTraits::eof)
        {
          throw InputFailure();
        }
      }
    }

    virtual int scanAny(const char_type *typeFormat, unsigned int fieldWidth, void *value)
    {
      char_type format[this->MAX_FIELD_SPECIFIER_LEN + 1];

      if (fieldWidth > 0)
      {
        CharTraits::snprintf(format,sizeof(format)/sizeof(format[0]),CharTraits::ScanAnyWithWidthFileFormatStr,fieldWidth,typeFormat);
      }
      else
      {
        CharTraits::snprintf(format,sizeof(format)/sizeof(format[0]),CharTraits::ScanAnyFileFormatStr,typeFormat);
      }

      int fieldCount = CharTraits::fscanf(stream, format, value);

      if ((char_result_type)fieldCount == CharTraits::eof)
      {
        throw InputFailure();
      }

     return fieldCount;
    }

    virtual off64_t getReadCharCount()
    {
      off64_t currOffset = ftello64(stream);

      return currOffset > startOffset ? currOffset - startOffset : 0;
    }

    private :

    off64_t startOffset;
    FILE   *stream;
  };

  template<typename CharTraits>
  class Scanner
  {
    public :

    typedef typename CharTraits::char_result_type char_result_type;
    typedef typename CharTraits::char_type        char_type;

    Scanner (InputStream<CharTraits> *src) : stream(src) {}

    char_result_type readChar()
    {
      return stream->readChar();
    }

    void unreadChar(char_result_type ch)
    {
      return stream->unreadChar(ch);
    }

    bool eof() const
    {
      return stream->eof();
    }

    void skipSpaces()
    {
      stream->skipSpaces();
    }

    int scanAny(const char_type *typeFormat, unsigned int fieldWidth, void *value)
    {
      return stream->scanAny(typeFormat, fieldWidth, value) == 1;
    }

    bool scanUInt(unsigned int *value, unsigned int fieldWidth = 0)
    {
      return stream->scanAny(CharTraits::Str_u, fieldWidth, value) == 1;
    }

    bool skipString(unsigned int fieldWidth)
    {
      unsigned int pos        = 0;
      bool         spaceFound = false;

      skipSpaces();

      while (!eof() && !spaceFound && (fieldWidth == 0 || pos < fieldWidth))
      {
        char_result_type ch = readChar();

        spaceFound = CharTraits::isspace(ch);

        if (spaceFound)
        {
          unreadChar(ch);
        }
        else
        {
          pos++;
        }
      }

      return true;
    }

    bool scanString(char_type *value, unsigned int bufferSize, unsigned int fieldWidth = 0)
    {
      if (bufferSize == 0) throw OutOfRangeFailure();

      skipSpaces();

      unsigned int pos        = 0;
      bool         spaceFound = false;

      while (!eof() && !spaceFound && pos < bufferSize && (fieldWidth == 0 || pos < fieldWidth))
      {
        char_result_type ch = readChar();

        if (ch != CharTraits::eof)
        {
          spaceFound = CharTraits::isspace(ch);

          if (spaceFound)
          {
            unreadChar(ch);
          }
          else
          {
            value[pos++] = (char_type)ch;
          }
        }
      }

      if (pos < bufferSize)
      {
        value[pos] = '\0';

        if (pos == 0)
        {
          throw InputFailure();
        }

        return true;
      }
      else
      {
        value[bufferSize - 1] = '\0';

        throw OutOfRangeFailure();
      }
    }

    bool skipChars(unsigned int fieldWidth)
    {
      unsigned int pos = 0;

      for (pos = 0; pos < fieldWidth && !eof(); pos++)
      {
        readChar();
      }

      return pos == fieldWidth;
    }

    bool scanChars(char_type *value, unsigned int bufferSize, unsigned int fieldWidth)
    {
      if (bufferSize == 0) throw OutOfRangeFailure();

      unsigned int pos = 0;

      while (!eof() && pos < bufferSize && pos < fieldWidth)
      {
        char_result_type ch = readChar();

        value[pos++] = (char_type)ch;
      }

      if (pos >= bufferSize && pos < fieldWidth)
      {
        throw OutOfRangeFailure();
      }

      return true;
    }

    off64_t getReadCharCount()
    {
      return stream->getReadCharCount();
    }

    private :

    InputStream<CharTraits> *stream;
  };
}

#define FORMAT_CASE(formatChar,formatStr,type) \
case (formatChar) : \
{ \
  if (suppressAssignment) \
  { \
    type temp; \
\
    ok = bufferScanner.scanAny(formatStr,maxWidth,&temp); \
  } \
  else \
  { \
    type *valueBuf = va_arg(args,type*); \
 \
    if (valueBuf == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); } \
 \
    ok = bufferScanner.scanAny(formatStr,maxWidth,valueBuf); \
\
    if (ok) \
    { \
      parsedFieldCount++; \
    } \
  } \
} break

#define FORMAT_N_CASE(type) \
case (CharTraits::Char_n) : \
{ \
  if (!suppressAssignment) \
  { \
    type *valueBuf = va_arg(args,type*); \
\
    if (valueBuf == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); } \
\
    *valueBuf = (type)bufferScanner.getReadCharCount(); \
  } \
} break

template<typename CharTraits>
static int stream_scanf_s(InputStream<CharTraits> *stream, const typename CharTraits::char_type *format, va_list args)
{
  #define FN "scanf_s"
  #define SP " "

  Scanner<CharTraits>           bufferScanner(stream);
  InputStreamString<CharTraits> formatStream(format);
  Scanner<CharTraits>           formatScanner(&formatStream);

  int          parsedFieldCount = 0;
  bool         ok = true;
  bool         inputFailed = false;

  try
  {
    while (ok && !formatScanner.eof())
    {
      typename CharTraits::char_result_type formatChar = formatScanner.readChar();

      if      (formatChar == CharTraits::StartFieldChar)
      {
        formatChar = formatScanner.readChar();

        bool suppressAssignment = formatChar == CharTraits::SuppressAssignmentChar;

        if (suppressAssignment)
        {
          formatChar = formatScanner.readChar();
        }

        unsigned int maxWidth = 0;

        if (isdigit(formatChar))
        {
          formatScanner.unreadChar(formatChar);

          ok = formatScanner.scanUInt(&maxWidth);

          if (ok)
          {
            if (maxWidth > 0)
            {
              formatChar = formatScanner.readChar();
            }
            else
            {
              ok = false;
            }
          }
        }

        if (ok)
        {
          if (formatChar == 'h')
          {
            formatChar = formatScanner.readChar();

            if (formatChar == 'h')
            {
              formatChar = formatScanner.readChar();

              switch (formatChar)
              {
                FORMAT_CASE(CharTraits::Char_d,CharTraits::Str_hhd,char);
                FORMAT_CASE(CharTraits::Char_i,CharTraits::Str_hhi,char);
                FORMAT_CASE(CharTraits::Char_u,CharTraits::Str_hhu,unsigned char);
                FORMAT_CASE(CharTraits::Char_o,CharTraits::Str_hho,unsigned char);
                FORMAT_CASE(CharTraits::Char_x,CharTraits::Str_hhx,unsigned char);
                FORMAT_CASE(CharTraits::Char_X,CharTraits::Str_hhX,unsigned char);
                FORMAT_N_CASE(char);

                default :
                {
                  ok = false;
                }
              }
            }
            else
            {
              switch (formatChar)
              {
                FORMAT_CASE(CharTraits::Char_d,CharTraits::Str_hd,short);
                FORMAT_CASE(CharTraits::Char_i,CharTraits::Str_hi,short);
                FORMAT_CASE(CharTraits::Char_u,CharTraits::Str_hu,unsigned short);
                FORMAT_CASE(CharTraits::Char_o,CharTraits::Str_ho,unsigned short);
                FORMAT_CASE(CharTraits::Char_x,CharTraits::Str_hx,unsigned short);
                FORMAT_CASE(CharTraits::Char_X,CharTraits::Str_hX,unsigned short);
                FORMAT_N_CASE(short);

                default :
                {
                  ok = false;
                }
              }
            }
          }
          else if (formatChar == CharTraits::Char_l)
          {
            formatChar = formatScanner.readChar();

            if (formatChar == CharTraits::Char_l)
            {
              formatChar = formatScanner.readChar();

              switch (formatChar)
              {
                FORMAT_CASE(CharTraits::Char_d,CharTraits::Str_lld,long long);
                FORMAT_CASE(CharTraits::Char_i,CharTraits::Str_lli,long long);
                FORMAT_CASE(CharTraits::Char_u,CharTraits::Str_llu,unsigned long long);
                FORMAT_CASE(CharTraits::Char_o,CharTraits::Str_llo,unsigned long long);
                FORMAT_CASE(CharTraits::Char_x,CharTraits::Str_llx,unsigned long long);
                FORMAT_CASE(CharTraits::Char_X,CharTraits::Str_llX,unsigned long long);
                FORMAT_N_CASE(long long);

                default :
                {
                  ok = false;
                }
              }
            }
            else
            {
              switch (formatChar)
              {
                FORMAT_CASE(CharTraits::Char_d,CharTraits::Str_ld,long);
                FORMAT_CASE(CharTraits::Char_i,CharTraits::Str_li,long);
                FORMAT_CASE(CharTraits::Char_u,CharTraits::Str_lu,unsigned long);
                FORMAT_CASE(CharTraits::Char_o,CharTraits::Str_lo,unsigned long);
                FORMAT_CASE(CharTraits::Char_x,CharTraits::Str_lx,unsigned long);
                FORMAT_CASE(CharTraits::Char_X,CharTraits::Str_lX,unsigned long);
                FORMAT_CASE(CharTraits::Char_f,CharTraits::Str_lf,double);
                FORMAT_CASE(CharTraits::Char_F,CharTraits::Str_lF,double);
                FORMAT_CASE(CharTraits::Char_e,CharTraits::Str_le,double);
                FORMAT_CASE(CharTraits::Char_E,CharTraits::Str_lE,double);
                FORMAT_CASE(CharTraits::Char_g,CharTraits::Str_lg,double);
                FORMAT_CASE(CharTraits::Char_G,CharTraits::Str_lG,double);
                FORMAT_N_CASE(long);

                default :
                {
                  ok = false;
                }
              }
            }
          }
          else
          {
            switch (formatChar)
            {
              FORMAT_CASE(CharTraits::Char_d,CharTraits::Str_d,int);
              FORMAT_CASE(CharTraits::Char_i,CharTraits::Str_i,int);
              FORMAT_CASE(CharTraits::Char_u,CharTraits::Str_u,unsigned);
              FORMAT_CASE(CharTraits::Char_o,CharTraits::Str_o,unsigned);
              FORMAT_CASE(CharTraits::Char_x,CharTraits::Str_x,unsigned);
              FORMAT_CASE(CharTraits::Char_X,CharTraits::Str_X,unsigned);
              FORMAT_CASE(CharTraits::Char_f,CharTraits::Str_f,float);
              FORMAT_CASE(CharTraits::Char_F,CharTraits::Str_F,float);
              FORMAT_CASE(CharTraits::Char_e,CharTraits::Str_e,float);
              FORMAT_CASE(CharTraits::Char_E,CharTraits::Str_E,float);
              FORMAT_CASE(CharTraits::Char_g,CharTraits::Str_g,float);
              FORMAT_CASE(CharTraits::Char_G,CharTraits::Str_G,float);
              FORMAT_N_CASE(int);

              case (CharTraits::StartFieldChar) :
              {
                if (bufferScanner.eof() || bufferScanner.readChar() != CharTraits::StartFieldChar)
                {
                  ok = false;
                }
              } break;

              case (CharTraits::Char_s) :
              {
                if (suppressAssignment)
                {
                  ok = bufferScanner.skipString(maxWidth);
                }
                else
                {
                  typename CharTraits::char_type *destStr = va_arg(args,typename CharTraits::char_type*);
                  unsigned int                    maxSize = va_arg(args,unsigned int);

                  if (destStr == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); }

                  ok = bufferScanner.scanString(destStr,maxSize,maxWidth);

                  if (ok)
                  {
                    parsedFieldCount++;
                  }
                }
              } break;

              case (CharTraits::Char_c) :
              {
                if (maxWidth == 0)
                {
                  maxWidth = 1;
                }

                if (suppressAssignment)
                {
                  ok = bufferScanner.skipChars(maxWidth);
                }
                else
                {
                  typename CharTraits::char_type *destBuff   = va_arg(args,typename CharTraits::char_type*);
                  unsigned int                    bufferSize = va_arg(args,unsigned int);

                  if (destBuff == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); }

                  ok = bufferScanner.scanChars(destBuff,bufferSize,maxWidth);

                  if (ok)
                  {
                    parsedFieldCount++;
                  }
                }
              } break;

              default :
              {
                ok = false;
              }
            }
          }
        }
      }
      else if (CharTraits::isspace(formatChar))
      {
        bufferScanner.skipSpaces();
      }
      else
      {
        if (bufferScanner.eof() || bufferScanner.readChar() != formatChar)
        {
          ok = false;
        }
      }
    }
  }
  catch (InputFailure&)
  {
    if (parsedFieldCount == 0)
    {
      inputFailed = true;
    }
  }
  catch (OutOfRangeFailure&)
  {
    inputFailed = true;

    return_after_err_handler(FN SP "buffer is too small",NULL,EOF);
  }

  return inputFailed ? -1 : parsedFieldCount;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int vsscanf_s(const char    *buffer, const char    *format, va_list argptr)
{
  #define FN "vsscanf_s"
  #define SP " "

  if (buffer == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamString<CharTraitsChar> stream(buffer);

  int result = stream_scanf_s<CharTraitsChar>(&stream,format,argptr);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int sscanf_s(const char *buffer, const char *format, ...)
{
  #define FN "sscanf_s"
  #define SP " "

  if (buffer == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamString<CharTraitsChar> stream(buffer);
  va_list                           args;

  va_start(args,format);

  int result = stream_scanf_s<CharTraitsChar>(&stream,format,args);

  va_end(args);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int vfscanf_s(FILE          *stream, const char    *format, va_list argptr)
{
  #define FN "vfscanf_s"
  #define SP " "

  if (stream == NULL) { return_after_err_handler(FN SP "stream is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamFile<CharTraitsChar> fileStream(stream);

  int result = stream_scanf_s<CharTraitsChar>(&fileStream,format,argptr);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int fscanf_s(FILE *stream, const char *format, ...)
{
  #define FN "fscanf_s"
  #define SP " "

  if (stream == NULL) { return_after_err_handler(FN SP "stream is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamFile<CharTraitsChar> fileStream(stream);
  va_list                         args;

  va_start(args,format);

  int result = stream_scanf_s<CharTraitsChar>(&fileStream,format,args);

  va_end(args);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int vswscanf_s(const wchar_t *buffer, const wchar_t *format, va_list argptr)
{
  #define FN "vswscanf_s"
  #define SP " "

  if (buffer == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamString<CharTraitsWideChar> stream(buffer);

  int result = stream_scanf_s<CharTraitsWideChar>(&stream,format,argptr);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int swscanf_s(const wchar_t *buffer, const wchar_t *format, ...)
{
  #define FN "swscanf_s"
  #define SP " "

  if (buffer == NULL) { return_after_err_handler(FN SP "buffer is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamString<CharTraitsWideChar> stream(buffer);
  va_list                               args;

  va_start(args,format);

  int result = stream_scanf_s<CharTraitsWideChar>(&stream,format,args);

  va_end(args);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int vfwscanf_s(FILE          *stream, const wchar_t *format, va_list argptr)
{
  #define FN "vfwscanf_s"
  #define SP " "

  if (stream == NULL) { return_after_err_handler(FN SP "stream is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamFile<CharTraitsWideChar> fileStream(stream);

  int result = stream_scanf_s<CharTraitsWideChar>(&fileStream,format,argptr);

  return result;

  #undef SP
  #undef FN
}

NW_EXTERN_C  int fwscanf_s(FILE          *stream, const wchar_t *format, ...)
{
  #define FN "fwscanf_s"
  #define SP " "

  if (stream == NULL) { return_after_err_handler(FN SP "stream is NULL",NULL,EOF); }
  if (format == NULL) { return_after_err_handler(FN SP "format is NULL",NULL,EOF); }

  InputStreamFile<CharTraitsWideChar> fileStream(stream);
  va_list                             args;

  va_start(args,format);

  int result = stream_scanf_s<CharTraitsWideChar>(&fileStream,format,args);

  va_end(args);

  return result;

  #undef SP
  #undef FN
}
