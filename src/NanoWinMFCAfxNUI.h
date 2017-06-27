// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFC UI emulation class

#if !defined(NanoWinMFCAfxNUIIncluded)
#define NanoWinErrorIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <map>
#include <utility>

#include "NanoWinMFCAfxStr.h"
#include "NanoWinMFCAfxCol.h"

class NanoWinTextStr
{
  public :

  void SetText (LPCSTR    lpszString);
  void SetText (LPCWSTR   lpszString);
  void GetText (CStringA &rString) const;
  void GetText (CStringW &rString) const;

  private :

  CString str;
};

class  CWnd
{
  public:

  void SetWindowTextA (LPCSTR    lpszString);
  void SetWindowTextW (LPCWSTR   lpszString);
  void GetWindowTextA (CStringA &rString) const;
  void GetWindowTextW (CStringW &rString) const;

  #if defined(SetWindowText)
  #else
  void SetWindowText (LPCSTR  lpszString) { SetWindowTextA(lpszString); }
  void SetWindowText (LPCWSTR lpszString) { SetWindowTextW(lpszString); }
  void GetWindowText (CStringA &rString) const { GetWindowTextA(rString); }
  void GetWindowText (CStringW &rString) const { GetWindowTextW(rString); }
  #endif

  private :

  NanoWinTextStr windowText;
};

class CHeaderCtrl : public CWnd
{
  public :
};

class CListCtrl
{
  public:

  BOOL SetColumnWidth(int /*nCol*/, int /*cx*/) { return TRUE; }

  BOOL    SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
  CString GetItemText(int nItem, int nSubItem) const;

  private :

  typedef std::pair<int,int> IntPair;

  std::map<IntPair,CString> itemStorage;
};

class CStatic : public CWnd
{
};

class CEdit : public CWnd
{
};

class CListBox  : public CWnd
{
  public:

  int  GetCount   () const;
  void GetText    (int nIndex, CString& rString) const;
  int  AddString  (LPCTSTR lpszItem);

  private :

  std::vector<CString> listStorage;
};

class CComboBox : public CWnd
{
  public:

  int AddString(LPCTSTR lpszItem);

  private :

  std::vector<CString> listStorage;
};

class CImageList : public CObject
{
};

#endif // LINUX

#endif

