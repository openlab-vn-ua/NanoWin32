#include "NanoWinPrivateProfile.h"
#include "NanoWinError.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <stdlib.h>

#if defined(LINUX)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#define GetProfXMaxLineLength         (1024)

#define GetProfXSectionNameStartChar  ('[')
#define GetProfXSectionNameEndChar    (']')
#define GetProfXKeyValueSeparatorChar ('=')
#define GetProfXValueSingleQuoteChar  ('\'')
#define GetProfXValueDoubleQuoteChar  ('\"')
#define GetProfXDummySectionValueChar (' ')

#define GetProfXUnixLineEndingStrLen  (1)
#define GetProfXDosLineEndingStrLen   (2)

#if defined(LINUX)
#define GetProfXIniFileDefaultPermissions (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)
#endif

#include "NanoWinStrConvert.h"
using namespace NanoWin;

/*
#ifndef FALSE
#define FALSE    (0)
#endif

#ifndef TRUE
#define TRUE     (1)
#endif
*/

static bool IsEmptyChar(char c)
{
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static size_t FindNextNonEmptyChar(const char *str, size_t startPos)
{
  size_t pos;

  for (pos = startPos; str[pos] != '\0' && IsEmptyChar(str[pos]); pos++)
  {
  }

  return pos;
}

static size_t FindFirstNonEmptyChar(const char *str)
{
  return FindNextNonEmptyChar(str, 0);
}

static size_t FindLastNonEmptyChar(const char *str, size_t startPos)
{
  size_t nonEmptyPos = startPos;

  for (size_t pos = startPos; str[pos] != '\0'; pos++)
  {
    if (!IsEmptyChar(str[pos]))
    {
      nonEmptyPos = pos;
    }
  }

  return nonEmptyPos;
}

static bool StrSkipExpectedSubString(const char *line, const char *expected, size_t *pos)
{
  size_t expectedPos = 0;

  while (expected[expectedPos] != '\0' && toupper(line[*pos]) == toupper(expected[expectedPos]))
  {
    (*pos)++;
    expectedPos++;
  }

  return expected[expectedPos] == '\0';
}

static bool IsSectionStartLine(const char *line, const char *sectionName = NULL)
{
  size_t linePos = FindFirstNonEmptyChar(line);

  if (line[linePos] != GetProfXSectionNameStartChar)
  {
    return false;
  }

  if (sectionName != NULL)
  {
    linePos++; // skip '[' char

    if (StrSkipExpectedSubString(line, sectionName, &linePos))
    {
      return line[linePos] == GetProfXSectionNameEndChar;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return true;
  }
}

static bool SeekToSectionStart(FILE *srcFile, const char *sectionName)
{
  char line[GetProfXMaxLineLength + 1];
  bool sectionFound = false;

  while (!sectionFound && fgets(line, sizeof(line), srcFile) != NULL)
  {
    sectionFound = IsSectionStartLine(line, sectionName);
  }

  return sectionFound;
}

static size_t ExtractValueFromLine(char *dstString, size_t dstStringMaxSize, const char *line, size_t startPos)
{
  size_t valueStartPos = FindNextNonEmptyChar(line, startPos);
  size_t valueEndPos = FindLastNonEmptyChar(line, valueStartPos);

  char firstChar = line[valueStartPos];
  char lastChar = line[valueEndPos];

  // after the following code quotes (if present) are stripped and valueEndPos points to the char after last non-empty char
  if (firstChar == lastChar &&
    (firstChar == GetProfXValueSingleQuoteChar || firstChar == GetProfXValueDoubleQuoteChar) &&
    valueEndPos > valueStartPos)
  {
    valueStartPos++;
  }
  else
  {
    if (lastChar != '\0')
    {
      valueEndPos++;
    }
  }

  size_t valueLen = valueEndPos - valueStartPos;

  if (dstStringMaxSize > 0)
  {
    size_t maxAllowedLen = dstStringMaxSize - 1;

    if (valueLen > maxAllowedLen)
    {
      valueLen = maxAllowedLen;
    }

    memcpy(dstString, &line[valueStartPos], valueLen);

    dstString[valueLen] = '\0';
  }
  else
  {
    valueLen = 0;
  }

  return valueLen;
}

static size_t CopyDefaultProfileStringValue(char *dstString, size_t dstStringMaxSize, const char *srcString)
{
  size_t resultLen = 0;

  if (dstStringMaxSize > 0)
  {
    if (srcString == NULL)
    {
      dstString[0] = '\0';
    }
    else
    {
      size_t srcStringLen = strlen(srcString);
      size_t maxAllowedLen = dstStringMaxSize - 1;

      if (srcStringLen > maxAllowedLen)
      {
        srcStringLen = maxAllowedLen;
      }

      memcpy(dstString, srcString, srcStringLen);
      dstString[srcStringLen] = '\0';

      resultLen = srcStringLen;
    }
  }

  return resultLen;
}

static bool ReadStringForKey(FILE *srcFile, const char *key, char *dstString, size_t dstStringMaxSize, size_t *valueLen)
{
  char line[GetProfXMaxLineLength + 1];
  bool keyFound = false;
  bool sectionFinished = false;

  *valueLen = 0;

  while (!sectionFinished && !keyFound && fgets(line, sizeof(line), srcFile) != NULL)
  {
    size_t pos = FindFirstNonEmptyChar(line);

    if (line[pos] == GetProfXSectionNameStartChar)
    {
      sectionFinished = true;
    }
    else
    {
      keyFound = StrSkipExpectedSubString(line, key, &pos);

      if (keyFound)
      {
        pos = FindNextNonEmptyChar(line, pos);

        if (line[pos] == GetProfXKeyValueSeparatorChar)
        {
          pos++;

          *valueLen = ExtractValueFromLine(dstString, dstStringMaxSize, line, pos);
        }
        else
        {
          keyFound = false; // skip malformed line (key is ok, but '=' char is absent)
        }
      }
    }
  }

  return keyFound;
}

static bool DoesLineHaveDosLineEnding(const char *line)
{
  size_t lineLen = strlen(line);

  if (lineLen < 2)
  {
    return false;
  }

  return line[lineLen - 1] == '\n' && line[lineLen - 2] == '\r';
}

static bool ReplaceValueInLineWithKey(char *line, size_t lineMaxLen, const char *key, const char *value)
{
  bool   result = false;
  size_t pos = FindFirstNonEmptyChar(line);

  if (StrSkipExpectedSubString(line, key, &pos))
  {
    pos = FindNextNonEmptyChar(line, pos);

    if (line[pos] == GetProfXKeyValueSeparatorChar)
    {
      pos++;

      bool   useDosLineEnding = DoesLineHaveDosLineEnding(line);
      size_t lineEndingLen = useDosLineEnding ? GetProfXDosLineEndingStrLen : GetProfXUnixLineEndingStrLen;

      ssize_t valueLen = strlen(value);
      ssize_t allowedValueLen = (ssize_t)lineMaxLen - pos - lineEndingLen;

      if (allowedValueLen >= 0)
      {
        if (valueLen > allowedValueLen)
        {
          valueLen = allowedValueLen;
        }

        memcpy(&line[pos], value, valueLen);

        pos += valueLen;

        if (useDosLineEnding)
        {
          line[pos++] = '\r';
        }

        line[pos++] = '\n';
        line[pos] = '\0';

        result = true;
      }
      else
      {
        // there is no space in buffer to place even empty value with line ending, so
        // we fail here
      }
    }
  }

  return result;
}

extern DWORD NanoWinGetPrivateProfileStringA
                                      (const char *lpszSection, const char *plszEntry,
                                       const char *lpszDefault, char *lpReturnedString, DWORD nSize, const char *lpszFilename)
{
  FILE  *srcFile = fopen(lpszFilename, "rt");
  bool   keyFound = false;
  size_t valueLen = 0;
  DWORD  errorCode = ERROR_SUCCESS;

  if (srcFile != NULL)
  {
    if (SeekToSectionStart(srcFile, lpszSection))
    {
      keyFound = ReadStringForKey(srcFile, plszEntry, lpReturnedString, nSize, &valueLen);
    }

    fclose(srcFile);
  }
  else
  {
    errorCode = NanoWinErrorByErrnoAtFail(errno);
  }

  if (!keyFound)
  {
    valueLen = CopyDefaultProfileStringValue(lpReturnedString, nSize, lpszDefault);

    if (valueLen == 0)
    {
      NanoWinSetLastError(errorCode != ERROR_SUCCESS ? errorCode : NW_DEFAULT_ERROR_AT_FAIL);
    }
  }

  return (DWORD)valueLen;
}

extern DWORD NanoWinGetPrivateProfileStringW
                                      (const wchar_t *lpszSection, const wchar_t *lpszEntry,
                                       const wchar_t *lpszDefault, wchar_t *lpReturnedString, DWORD nSize, const wchar_t *lpszFilename)
{
  size_t result = 0;

  try
  {
    WStrToStrClone section(lpszSection);
    WStrToStrClone entry(lpszEntry);
    WStrToStrClone defaultValue(lpszDefault);
    WStrToStrClone fileName(lpszFilename);
    char        resultBuffer[GetProfXMaxLineLength + 1];

    result = GetPrivateProfileStringA(section.c_str(), entry.c_str(), defaultValue.c_str(),
                                      resultBuffer, sizeof(resultBuffer), fileName.c_str());

    if (result > 0)
    {
      mbstate_t mbstate;

      memset(&mbstate, 0, sizeof(mbstate));

      const char *tmpResultBuffer = resultBuffer;

      result = mbsrtowcs(lpReturnedString, &tmpResultBuffer, nSize, &mbstate);

      if (result == (size_t)-1)
      {
        result = 0;

        NanoWinSetLastError(NanoWinErrorByErrnoAtFail(EILSEQ));
      }
    }
  }
  catch (...)
  {
    // exceptions during string conversions are treated as errors (zero is returned)

    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(EILSEQ));
  }

  if (result == 0)
  {
    if (nSize > 0)
    {
      lpReturnedString[0] = L'\0';
    }
  }

  return result;
}

extern DWORD NanoWinGetPrivateProfileSectionA
                                      (const char *lpszSection, char *lpReturnedString,
                                       DWORD nSize, const char *lpszFilename)
{
  FILE *srcFile = fopen(lpszFilename, "rt");
  DWORD result = 0;

  if (srcFile != NULL)
  {
    if (SeekToSectionStart(srcFile, lpszSection))
    {
      if (nSize > 0)
      {
        lpReturnedString[0] = GetProfXDummySectionValueChar;
        lpReturnedString[1] = '\0';

        result = 1;
      }
      else
      {
        NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL);
      }
    }

    fclose(srcFile);
  }
  else
  {
    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));
  }

  if (result == 0)
  {
    lpReturnedString[0] = '\0';
  }

  return result;
}

extern UINT NanoWinGetPrivateProfileIntA
                                      (const char *lpszSection,
                                       const char *lpszEntry, int nDefault, const char *lpszFilename)
{
  char  buffer[GetProfXMaxLineLength + 1];

  int   result = nDefault;
  DWORD valueLen = GetPrivateProfileStringA(lpszSection, lpszEntry, NULL, buffer, sizeof(buffer), lpszFilename);

  if (valueLen > 0)
  {
    if (sscanf(buffer, "%d", &result) < 1)
    {
      result = nDefault;
    }
  }

  return result;
}

extern UINT NanoWinGetPrivateProfileIntW
                                      (const wchar_t *lpszSection, const wchar_t *lpszEntry, int vdefault,
                                       const wchar_t *lpszFilename)
{
  UINT result;

  try
  {
    WStrToStrClone section(lpszSection);
    WStrToStrClone entry(lpszEntry);
    WStrToStrClone fileName(lpszFilename);

    result = NanoWinGetPrivateProfileIntA(section.c_str(), entry.c_str(), vdefault, fileName.c_str());
  }
  catch (...)
  {
    result = vdefault;
  }

  return result;
}

#if defined(LINUX)
static FILE *CreateTempFile(char **tempFileName, const char *baseFileName)
{
  FILE        *tmpFile = NULL;
  const size_t tmpFileNameMaskLen = 6;
  const char   tmpFileNameMaskChar = 'X';
  size_t       baseFileNameLen = strlen(baseFileName);

  char *fileNameTemplate = (char*)malloc(baseFileNameLen + tmpFileNameMaskLen + 1);

  if (fileNameTemplate != NULL)
  {
    memcpy(fileNameTemplate, baseFileName, baseFileNameLen);
    memset(&fileNameTemplate[baseFileNameLen], tmpFileNameMaskChar, tmpFileNameMaskLen);
    fileNameTemplate[baseFileNameLen + tmpFileNameMaskLen] = '\0';

    int tmpFileHandle = mkstemp(fileNameTemplate);

    if (tmpFileHandle != -1)
    {
      tmpFile = fdopen(tmpFileHandle, "wt");

      if (tmpFile == NULL)
      {
        NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));

        close(tmpFileHandle);
      }
    }
    else
    {
      NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));
    }

    if (tmpFile != NULL)
    {
      *tempFileName = fileNameTemplate;
    }
    else
    {
      free(fileNameTemplate);
    }
  }
  else
  {
    NanoWinSetLastError(ERROR_NOT_ENOUGH_MEMORY);
  }

  return tmpFile;
}
#elif defined(_WIN32) || defined(_WIN64)
static FILE *CreateTempFile(char **tempFileName, const char *baseFileName)
{
  FILE *tmpFile = NULL;

  *tempFileName = (char*)malloc(MAX_PATH + 1);

  if (*tempFileName != NULL)
  {
    if (GetTempFileNameA(".", baseFileName, 0, *tempFileName) != 0)
    {
      tmpFile = fopen(*tempFileName, "wt");
    }

    if (tmpFile == NULL)
    {
      free(*tempFileName);

      *tempFileName = NULL;
    }
  }

  return tmpFile;
}
#endif

static bool WriteSectionHeaderLine(FILE *dstFile, const char *section)
{
  return fprintf(dstFile, "[%s]\n", section) >= 0;
}

static bool WriteKeyValueLine(FILE *dstFile, const char *key, const char *value)
{
  return fprintf(dstFile, "%s=%s\n", key, value) >= 0;
}

#if defined(LINUX)
// this function tries to replace file dstFileName with file srcFileName
// without changing owner,group and mode of original file, note that
// errors during setting owner,group or mode are ignored, it is much more
// important to store file itself (at least this is the case for our application)
static bool	GetProfReplaceFile(const char *dstFileName, const char *srcFileName)
{
  bool hasStatInfo;
  struct stat dstFileStat;

  bool ok = true;

  hasStatInfo = stat(dstFileName, &dstFileStat) == 0;

  if (rename(srcFileName, dstFileName) != 0)
  {
    ok = false;

    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));

    unlink(srcFileName);
  }

  if (ok)
  {
    if (hasStatInfo)
    {
      chown(dstFileName, dstFileStat.st_uid, dstFileStat.st_gid);
      chmod(dstFileName, dstFileStat.st_mode);
    }
    else
    {
      chmod(dstFileName, GetProfXIniFileDefaultPermissions);
    }
  }

  return ok;
}
#elif defined(_WIN32) || defined(_WIN64)
static bool	GetProfReplaceFile(const char *dstFileName, const char *srcFileName)
{
  return MoveFileExA(srcFileName, dstFileName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
}
#endif

extern BOOL NanoWinWritePrivateProfileStringA
                                      (const char *lpszSection,
                                       const char *lpszEntry, const char *lpszString, const char *lpszFilename)
{
  bool ok = false;
  FILE *srcFile = fopen(lpszFilename, "rt");

  char *tmpFileName = NULL;

  FILE *tmpFile = CreateTempFile(&tmpFileName, lpszFilename);

  if (tmpFile != NULL)
  {
    bool fileCopied = srcFile == NULL;
    char line[GetProfXMaxLineLength + 1];

    enum { STATE_SEARCH_SECTION, STATE_SEARCH_KEY, STATE_COPY };

    int currentState = STATE_SEARCH_SECTION;

    ok = true;

    while (ok && !fileCopied)
    {
      if (fgets(line, sizeof(line), srcFile) != NULL)
      {
        if (currentState == STATE_SEARCH_SECTION)
        {
          if (IsSectionStartLine(line, lpszSection))
          {
            currentState = STATE_SEARCH_KEY;
          }
        }
        else if (currentState == STATE_SEARCH_KEY)
        {
          if (IsSectionStartLine(line))
          {
            // new section started but key not found

            ok = WriteKeyValueLine(tmpFile, lpszEntry, lpszString);

            if (ok)
            {
              currentState = STATE_COPY;
            }
          }
          else if (ReplaceValueInLineWithKey(line, GetProfXMaxLineLength, lpszEntry, lpszString))
          {
            currentState = STATE_COPY;
          }
        }
        else // if (currentState == STATE_COPY)
        {
          // nothing to do - line will be copied by the code below
        }

        if (ok)
        {
          ok = fputs(line, tmpFile) >= 0;
        }
      }
      else
      {
        if (feof(srcFile))
        {
          fileCopied = true;
        }
        else
        {
          ok = false;
        }
      }
    }

    if (ok)
    {
      // special case - end of file has been reached, but requested section is not found
      // so we add new section and switch state to STATE_SEARCH_KEY to force new line to be added in 
      // the following code
      if (currentState == STATE_SEARCH_SECTION)
      {
        ok = WriteSectionHeaderLine(tmpFile, lpszSection);

        if (ok)
        {
          currentState = STATE_SEARCH_KEY;
        }
      }
    }

    // special case - end of file has been reached, requested section is the last section of the file
    // and no record with requested key was found, so we just add new line with requested value
    if (ok)
    {
      if (currentState == STATE_SEARCH_KEY)
      {
        ok = WriteKeyValueLine(tmpFile, lpszEntry, lpszString);
      }
    }

    if (!ok)
    {
      NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));
    }

    fclose(tmpFile);
  }

  if (srcFile != NULL)
  {
    fclose(srcFile);
  }

  if (ok)
  {
    ok = GetProfReplaceFile(lpszFilename, tmpFileName);
  }

  if (tmpFileName != NULL)
  {
    free(tmpFileName);
  }

  return ok ? TRUE : FALSE;
}

extern BOOL NanoWinWritePrivateProfileStringW
                                      (const wchar_t *lpszSection,
                                       const wchar_t *lpszEntry, const wchar_t *lpszString, const wchar_t *lpszFileName)
{
  BOOL ok;

  try
  {
    WStrToStrClone section(lpszSection);
    WStrToStrClone entry(lpszEntry);
    WStrToStrClone value(lpszString);
    WStrToStrClone fileName(lpszFileName);

    ok = NanoWinWritePrivateProfileStringA(section.c_str(), entry.c_str(), value.c_str(), fileName.c_str());
  }
  catch (...)
  {
    ok = FALSE;

    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(EILSEQ));
  }

  return ok;
}
