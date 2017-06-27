// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFC UI emulation class

#include <stdio.h>

#include "NanoWinMFCAfxNUI.h"

static void NanoWinNUILog(const char *format, ...)
{
  CStringA str;
  va_list  args;

  va_start(args,format);

  str.FormatV(format,args);

  va_end(args);

  printf("NUILog: %s\n",str.GetString());
}

static void NanoWinNUILog(const wchar_t *format, ...)
{
  CStringW str;
  va_list  args;

  va_start(args,format);

  str.FormatV(format,args);

  va_end(args);

  printf("NUILog: %s\n",CW2A(str.GetString()).c_str());
}

#if defined(UNICODE) || defined(_UNICODE)

void NanoWinTextStr::SetText(LPCSTR    lpszString)
{
  str.SetString(CA2W(lpszString));
}

void NanoWinTextStr::SetText(LPCWSTR   lpszString)
{
  str.SetString(lpsString);
}

void NanoWinTextStr::GetText(CStringA &rString) const
{
  rString.SetString(CW2A(str.GetString()));
}

void NanoWinTextStr::GetText (CStringW &rString) const
{
  rString = str;
}

#else

void NanoWinTextStr::SetText(LPCSTR    lpszString)
{
  str.SetString(lpszString);
}

void NanoWinTextStr::SetText(LPCWSTR   lpszString)
{
  str.SetString(CW2A(lpszString));
}

void NanoWinTextStr::GetText(CStringA &rString) const
{
  rString = str;
}

void NanoWinTextStr::GetText (CStringW &rString) const
{
  rString.SetString(CA2W(str.GetString()));
}

#endif

void CWnd::SetWindowTextA(LPCSTR    lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog("CWnd::SetWindowTextA(%s)",lpszString);
}

void CWnd::SetWindowTextW(LPCWSTR   lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog(L"CWnd::SetWindowTextW(%s)",lpszString);
}

void CWnd::GetWindowTextA(CStringA &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog("CWnd::GetWindowTextA(%s)",rString.GetString());
}

void CWnd::GetWindowTextW(CStringW &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog(L"CWnd::GetWindowTextW(%s)",rString.GetString());
}

BOOL    CListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
  itemStorage[IntPair(nItem,nSubItem)] = CString(lpszText);

  NanoWinNUILog(_T("CListCtrl::SetItemText(item: %d, subitem: %d, text: %s)"),nItem,nSubItem,lpszText);

  return TRUE;
}

CString CListCtrl::GetItemText(int nItem, int nSubItem) const
{
  auto it = itemStorage.find(IntPair(nItem,nSubItem));

  if (it != itemStorage.cend())
  {
    return it->second;
  }
  else
  {
    return CString();
  }
}

int  CListBox::GetCount() const
{
  return (int)listStorage.size();
}

void CListBox::GetText (int nIndex, CString& rString) const
{
  if (nIndex >= 0 && nIndex < GetCount())
  {
    rString = listStorage[nIndex];
  }
  else
  {
    rString.Empty();
  }
}

int  CListBox::AddString (LPCTSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(lpszItem);

  NanoWinNUILog(_T("CListBox::AddString(index: %d, text: %s)"),index,lpszItem);

  return index;
}

int  CComboBox::AddString (LPCTSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(lpszItem);

  NanoWinNUILog(_T("CComboBox::AddString(index: %d, text: %s)"),index,lpszItem);

  return index;
}
