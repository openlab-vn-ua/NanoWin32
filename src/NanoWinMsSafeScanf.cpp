// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe sscanf_s, fscanf_s extensions

#include <stdio.h>
#include <stdarg.h>

#include <sstream>

#include "NanoWinMsSafeScanf.h"

#define NanoWinScanfStartFieldChar         '%'
#define NanoWinScanfSuppressAssignmentChar '*'

namespace
{
  class InputFailure {};
  class OutOfRangeFailure {};

  class InputStream
  {
    public :

    virtual bool eof () = 0;
    virtual int  readChar () = 0;
    virtual void unreadChar (int ch) = 0;
    virtual void skipSpaces () = 0;

    virtual int  scanAny (const char *typeFormat, int fieldWidth, void *value) = 0;

    protected :
    // %[*][width]lld
    enum { FIELD_WIDTH_MAX_WIDTH = 20}; // 20 decimal digits is enough to represent 64-bit value
    enum { MAX_FIELD_SPECIFIER_LEN = 1 + 1 + FIELD_WIDTH_MAX_WIDTH + 3};
  };
  
  class InputStreamString : public InputStream
  {
    public :

    InputStreamString(const char *src)
    {
      str = src;
      pos = 0;
    }

    virtual bool eof()
    {
      return str[pos] == '\0';
    }

    virtual int  readChar()
    {
      char ch = str[pos];

      if (ch != '\0')
      {
        pos++;
      }
      else
      {
        throw InputFailure();
      }

      return ch;
    }

    virtual void unreadChar(int ch)
    {
      if (pos > 0)
      {
        if (str[--pos] != ch)
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
      while (!eof() && isspace(str[pos]))
      {
        pos++;
      }
    }
 
    virtual int scanAny(const char *typeFormat, int fieldWidth, void *value)
    {
      char format[MAX_FIELD_SPECIFIER_LEN * 2 + 1];

      if (fieldWidth > 0)
      {
        sprintf(format,"%%%d%s%%n",fieldWidth,typeFormat);
      }
      else
      {
        sprintf(format,"%%%s%%n",typeFormat);
      }

      int len = 0;
      int fieldCount = sscanf(&str[pos], format, value, &len);

      pos += len;

      if (fieldCount == EOF)
      {
        throw InputFailure();
      }

      return fieldCount;
    }

    private :

    const char *str;
    size_t      pos;
  };

  class InputStreamFile : public InputStream
  {
    public :

    InputStreamFile(FILE *file)
    {
      stream = file;
    }

    virtual bool eof()
    {
      return feof(stream);
    }

    virtual int  readChar()
    {
      int ch = fgetc(stream);

      if (ch == EOF)
      {
        if (ferror(stream))
        {
          throw InputFailure();
        }
      }

      return ch;
    }

    virtual void unreadChar(int ch)
    {
      if (ungetc(ch, stream) == EOF)
      {
        throw InputFailure();
      }
    }

    virtual void skipSpaces()
    {
      int ch;

      do 
      {
        ch = fgetc(stream);
      } while (isspace(ch));

      if (ch == EOF)
      {
        if (ferror(stream))
        {
          throw InputFailure();
        }
      }
      else
      {
        if (ungetc(ch, stream) == EOF)
        {
          throw InputFailure();
        }
      }
    }

    virtual int scanAny(const char *typeFormat, int fieldWidth, void *value)
    {
      char format[MAX_FIELD_SPECIFIER_LEN + 1];

      if (fieldWidth > 0)
      {
        sprintf(format,"%%%d%s",fieldWidth,typeFormat);
      }
      else
      {
        sprintf(format,"%%%s",typeFormat);
      }

      int fieldCount = fscanf(stream, format, value);

      if (fieldCount == EOF)
      {
        throw InputFailure();
      }

     return fieldCount;
    }

    private :

    FILE *stream;
  };

  class Scanner
  {
    public :

    Scanner (InputStream *src) : stream(src) {}

    int readChar()
    {
      return stream->readChar();
    }

    void unreadChar(int ch)
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

    int scanAny(const char *typeFormat, int fieldWidth, void *value)
    {
      return stream->scanAny(typeFormat, fieldWidth, value) == 1;
    }

    bool scanInt(int *value, int fieldWidth = 0)
    {
      return stream->scanAny("d", fieldWidth, value) == 1;
    }

    bool skipString(int fieldWidth)
    {
      int  pos        = 0;
      bool spaceFound = false;

      skipSpaces();

      while (!eof() && !spaceFound && (fieldWidth == 0 || pos < fieldWidth))
      {
        int ch = readChar();

        spaceFound = isspace(ch);

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

    bool scanString(char *value, int bufferSize, int fieldWidth = 0)
    {
      if (bufferSize == 0) throw OutOfRangeFailure();

      skipSpaces();

      int  pos        = 0;
      bool spaceFound = false;

      while (!eof() && !spaceFound && pos < bufferSize && (fieldWidth == 0 || pos < fieldWidth))
      {
        int ch = readChar();

        if (ch != EOF)
        {
          spaceFound = isspace(ch);

          if (spaceFound)
          {
            unreadChar(ch);
          }
          else
          {
            value[pos++] = (char)ch;
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

    bool skipChars(int fieldWidth)
    {
      int pos = 0;

      for (int pos = 0; pos < fieldWidth && !eof(); pos++)
      {
        readChar();
      }

      return pos == fieldWidth;
    }

    bool scanChars(char *value, int bufferSize, int fieldWidth)
    {
      if (bufferSize == 0) throw OutOfRangeFailure();

      int pos = 0;

      while (!eof() && pos < bufferSize && pos < fieldWidth)
      {
        int ch = readChar();

        value[pos++] = (char)ch;
      }

      if (pos >= bufferSize && pos < fieldWidth)
      {
        throw OutOfRangeFailure();
      }

      return true;
    }

    private :

    InputStream *stream;
  };

  enum
  {
    NanoWinMsSafeScanfLengthModifierNone,
    NanoWinMsSafeScanfLengthModifierShortShort,
    NanoWinMsSafeScanfLengthModifierShort,
    NanoWinMsSafeScanfLengthModifierLong,
    NanoWinMsSafeScanfLengthModifierLongLong
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
    ok = bufferScanner.scanAny(formatStr,maxWidth,va_arg(args,type*)); \
\
    if (ok) \
    { \
      parsedFieldCount++; \
    } \
  } \
} break


static int stream_scanf_s(InputStream *stream, const char *format, va_list args)
{
  Scanner           bufferScanner(stream);
  InputStreamString formatStream(format);
  Scanner           formatScanner(&formatStream);

  int          parsedFieldCount = 0;
  bool         ok = true;
  bool         inputFailed = false;

  try
  {
    while (ok && !formatScanner.eof())
    {
      int formatChar = formatScanner.readChar();

      if      (formatChar == NanoWinScanfStartFieldChar)
      {
        formatChar = formatScanner.readChar();

        bool suppressAssignment = formatChar == NanoWinScanfSuppressAssignmentChar;

        if (suppressAssignment)
        {
          formatChar = formatScanner.readChar();
        }

        int maxWidth = 0;

        if (isdigit(formatChar))
        {
          formatScanner.unreadChar(formatChar);

          ok = formatScanner.scanInt(&maxWidth);

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
                FORMAT_CASE('d',"hhd",char);
                FORMAT_CASE('i',"hhi",char);
                FORMAT_CASE('u',"hhu",unsigned char);
                FORMAT_CASE('o',"hho",unsigned char);
                FORMAT_CASE('x',"hhx",unsigned char);
                FORMAT_CASE('X',"hhX",unsigned char);

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
                FORMAT_CASE('d',"hd",short);
                FORMAT_CASE('i',"hi",short);
                FORMAT_CASE('u',"hu",unsigned short);
                FORMAT_CASE('o',"ho",unsigned short);
                FORMAT_CASE('x',"hx",unsigned short);
                FORMAT_CASE('X',"hX",unsigned short);

                default :
                {
                  ok = false;
                }
              }
            }
          }
          else if (formatChar == 'l')
          {
            formatChar = formatScanner.readChar();

            if (formatChar == 'l')
            {
              formatChar = formatScanner.readChar();

              switch (formatChar)
              {
                FORMAT_CASE('d',"lld",long long);
                FORMAT_CASE('i',"lli",long long);
                FORMAT_CASE('u',"llu",unsigned long long);
                FORMAT_CASE('o',"llo",unsigned long long);
                FORMAT_CASE('x',"llx",unsigned long long);
                FORMAT_CASE('X',"llX",unsigned long long);

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
                FORMAT_CASE('d',"ld",long);
                FORMAT_CASE('i',"li",long);
                FORMAT_CASE('u',"lu",unsigned long);
                FORMAT_CASE('o',"lo",unsigned long);
                FORMAT_CASE('x',"lx",unsigned long);
                FORMAT_CASE('X',"lX",unsigned long);
                FORMAT_CASE('f',"lf",double);
                FORMAT_CASE('F',"lF",double);
                FORMAT_CASE('e',"le",double);
                FORMAT_CASE('E',"lE",double);
                FORMAT_CASE('g',"lg",double);
                FORMAT_CASE('G',"lG",double);

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
              FORMAT_CASE('d',"d",int);
              FORMAT_CASE('i',"i",int);
              FORMAT_CASE('u',"u",unsigned);
              FORMAT_CASE('o',"o",unsigned);
              FORMAT_CASE('x',"x",unsigned);
              FORMAT_CASE('X',"X",unsigned);
              FORMAT_CASE('f',"f",float);
              FORMAT_CASE('F',"F",float);
              FORMAT_CASE('e',"e",float);
              FORMAT_CASE('E',"E",float);
              FORMAT_CASE('g',"g",float);
              FORMAT_CASE('G',"G",float);

              case ('s') :
              {
                if (suppressAssignment)
                {
                  ok = bufferScanner.skipString(maxWidth);
                }
                else
                {
                  char *destStr = va_arg(args,char*);
                  int   maxSize = va_arg(args,int);

                  ok = bufferScanner.scanString(destStr,maxSize,maxWidth);

                  if (ok)
                  {
                    parsedFieldCount++;
                  }
                }
              } break;

              case ('c') :
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
                  char *destBuff   = va_arg(args,char*);
                  int   bufferSize = va_arg(args,int);

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
      else if (isspace(formatChar))
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
  }

  return inputFailed ? -1 : parsedFieldCount;
}
 
NW_EXTERN_C  int sscanf_s(const char *buffer, const char *format, ...)
{
  InputStreamString stream(buffer);
  va_list           args;

  va_start(args,format);

  int result = stream_scanf_s(&stream,format,args);

  va_end(args);

  return result;
}

NW_EXTERN_C  int fscanf_s(FILE *stream, const char *format, ...)
{
  InputStreamFile   fileStream(stream);
  va_list           args;

  va_start(args,format);

  int result = stream_scanf_s(&fileStream,format,args);

  va_end(args);

  return result;
}
