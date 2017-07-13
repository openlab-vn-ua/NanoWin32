// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFC UI emulation class

#include <stdio.h>

#include "NanoWinMFCAfxNUI.h"

#define NW_STR_FMT_W "%ls"
#define NW_STR_FMT_A "%s"

#define NW_STR_A(S) S
#define NW_STR_W(S) L"" S

#if defined(UNICODE) || defined(_UNICODE)
 #define NW_STR_FMT NW_STR_FMT_W
#else
 #define NW_STR_FMT NW_STR_FMT_A
#endif

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

CWnd::CWnd()
{
  isEnabled = true;
}

void CWnd::SetWindowTextA(LPCSTR    lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog(NW_STR_A("CWnd::SetWindowTextA(" NW_STR_FMT_A ")"),lpszString);
}

void CWnd::SetWindowTextW(LPCWSTR   lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog(NW_STR_W("CWnd::SetWindowTextW(" NW_STR_FMT_W ")"),lpszString);
}

void CWnd::GetWindowTextA(CStringA &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog(NW_STR_A("CWnd::GetWindowTextA(" NW_STR_FMT_A ")"),rString.GetString());
}

void CWnd::GetWindowTextW(CStringW &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog(NW_STR_W("CWnd::GetWindowTextW(" NW_STR_FMT_W ")"),rString.GetString());
}

BOOL CWnd::EnableWindow(BOOL bEnable)
{
  BOOL oldState = isEnabled;

  isEnabled = bEnable;

  return oldState;
}

BOOL CWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
  return FALSE;
}

void CWnd::OnDestroy()
{
}

BOOL CWnd::UpdateData(BOOL /*bSaveAndValidate*/)
 {
  return FALSE;
 }

CWnd* CWnd::GetDlgItem(int /*nID*/) const
{
  return NULL;
}

void CWnd::DoDataExchange(CDataExchange* /*pDX*/)
{
  NanoWinNUILog("CWnd::DoDataExchange");
}

BOOL    CListCtrl::SetItemText(int nItem, int nSubItem, LPCSTR lpszText)
{
  if ((CStringVector::size_type)nItem    >= items.size())         { return FALSE; }
  if ((CStringVector::size_type)nSubItem >= columnHeaders.size()) { return FALSE; }

  items[nItem][nSubItem] = CStringA(lpszText);

  NanoWinNUILog(NW_STR_A("CListCtrl::SetItemText[A](item: %d, subitem: %d, text: " NW_STR_FMT_A ")"),nItem,nSubItem,lpszText);

  return TRUE;
}

BOOL    CListCtrl::SetItemText(int nItem, int nSubItem, LPCWSTR lpszText)
{
  if ((CStringVector::size_type)nItem    >= items.size())         { return FALSE; }
  if ((CStringVector::size_type)nSubItem >= columnHeaders.size()) { return FALSE; }

  items[nItem][nSubItem] = lpszText;

  NanoWinNUILog(NW_STR_W("CListCtrl::SetItemText[W](item: %d, subitem: %d, text: " NW_STR_FMT_W ")"),nItem,nSubItem,lpszText);

  return TRUE;
}

const char *CListCtrl::GetItemTextStr(int nItem, int nSubItem) const
{
  if ((CStringVector::size_type)nItem    >= items.size())         { return ""; }
  if ((CStringVector::size_type)nSubItem >= columnHeaders.size()) { return ""; }

  return items[nItem][nSubItem].GetString();
}

BOOL CListCtrl::DeleteAllItems ()
{
  items.clear();

  NanoWinNUILog(NW_STR_A("CListCtrl::DeleteAllItems()"));
  
  return TRUE;
}

int CListCtrl::InsertColumn (int nCol, LPCSTR lpszColumnHeading, int /*nFormat*/, int /*nWidth*/, int /*nSubItem*/)
{
  NanoWinNUILog(NW_STR_A("CListCtrl::InsertColumn(nCol: %d heading: " NW_STR_FMT_A ")"),nCol,lpszColumnHeading);

  if ((CStringVector::size_type)nCol < columnHeaders.size())
  {
    columnHeaders.insert(columnHeaders.begin() + nCol,lpszColumnHeading);

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      it->insert(it->begin() + nCol,CStringA());
    }

    return nCol;
  }
  else
  {
    columnHeaders.push_back(lpszColumnHeading);

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      it->push_back(CStringA());
    }

    return columnHeaders.size() - 1;
  }

  return nCol;
}

int CListCtrl::InsertColumn (int nCol, LPCWSTR lpszColumnHeading, int /*nFormat*/, int /*nWidth*/, int /*nSubItem*/)
{
  NanoWinNUILog(NW_STR_W("CListCtrl::InsertColumn(nCol: %d heading: " NW_STR_FMT_W ")"),nCol,lpszColumnHeading);

  if ((CStringVector::size_type)nCol < columnHeaders.size())
  {
    columnHeaders.insert(columnHeaders.begin() + nCol,CW2A(lpszColumnHeading));

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      it->insert(it->begin() + nCol,CStringA());
    }

    return nCol;
  }
  else
  {
    columnHeaders.push_back(CW2A(lpszColumnHeading));

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      it->push_back(CStringA());
    }

    return columnHeaders.size() - 1;
  }

  return nCol;
}


BOOL CListCtrl::DeleteColumn (int nCol)
{
  NanoWinNUILog(NW_STR_A("CListCtrl::DeleteColumn(nCol: %d)"),nCol);

  if ((CStringVector::size_type)nCol >= columnHeaders.size()) { return FALSE; }

  columnHeaders.erase(columnHeaders.begin() + nCol);

  for (auto it = items.begin(); it != items.end(); ++it)
  {
    it->erase(it->begin() + nCol);
  }

  return TRUE;
}

int CListCtrl::InsertItem (int nItem, LPCSTR lpszItem)
{
  NanoWinNUILog(NW_STR_A("CListCtrl::InsertItem[A](nItem: %d, item: " NW_STR_FMT_A ")"),nItem,lpszItem);

  if ((CStringVector::size_type)nItem < items.size())
  {
    items.insert(items.begin() + nItem,CStringVector());
  }
  else
  {
    nItem = items.size();

    items.push_back(CStringVector());
  }

  CStringVector &newItem = items[nItem];

  for (CStringVector::size_type i = 0; i < columnHeaders.size(); i++)
  {
    newItem.push_back(CStringA());
  }

  return nItem;
}

int CListCtrl::InsertItem (int nItem, LPCWSTR lpszItem)
{
  NanoWinNUILog(NW_STR_W("CListCtrl::InsertItem[W](nItem: %d, item: " NW_STR_FMT_W ")"),nItem,lpszItem);

  if ((CStringVector::size_type)nItem < items.size())
  {
    items.insert(items.begin() + nItem,CStringVector());
  }
  else
  {
    nItem = items.size();

    items.push_back(CStringVector());
  }

  CStringVector &newItem = items[nItem];

  for (CStringVector::size_type i = 0; i < columnHeaders.size(); i++)
  {
    newItem.push_back(CStringA());
  }

  return nItem;
}


int  CListBox::GetCount() const
{
  return (int)listStorage.size();
}

void CListBox::GetText (int nIndex, CStringA& rString) const
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

void CListBox::GetText (int nIndex, CStringW& rString) const
{
  if (nIndex >= 0 && nIndex < GetCount())
  {
    rString = CStringW(listStorage[nIndex]);
  }
  else
  {
    rString.Empty();
  }
}

int  CListBox::AddString (LPCSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(lpszItem);

  NanoWinNUILog(NW_STR_A("CListBox::AddString[A](index: %d, text: " NW_STR_FMT_A ")"),index,lpszItem);

  return index;
}

int  CListBox::AddString (LPCWSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(CW2A(lpszItem));

  NanoWinNUILog(NW_STR_W("CListBox::AddString[W](index: %d, text: " NW_STR_FMT_W ")"),index,lpszItem);

  return index;
}

void CListBox::ResetContent()
{
  listStorage.clear();
}

CComboBox::CComboBox()
{
  currSelection = CB_ERR;
}

int  CComboBox::AddString (LPCSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(lpszItem);

  NanoWinNUILog(NW_STR_A("CComboBox::AddString[A](index: %d, text: " NW_STR_FMT_A ")"),index,lpszItem);

  return index;
}

int  CComboBox::AddString (LPCWSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(CW2A(lpszItem));

  NanoWinNUILog(NW_STR_W("CComboBox::AddString[W](index: %d, text: " NW_STR_FMT_W ")"),index,lpszItem);

  return index;
}

int CComboBox::SetCurSel(int nSelect)
{
  if (nSelect != CB_ERR)
  {
    if (nSelect >= 0 && nSelect < (int)listStorage.size())
    {
      currSelection = nSelect;
    }
    else
    {
      currSelection = CB_ERR;
    }
  }
  else
  {
    currSelection = CB_ERR;
  }

  return currSelection;
}

int CComboBox::GetCurSel() const
{
  return currSelection;
}

CButton::CButton()
{
  checkState     = BST_UNCHECKED;
  highlightState = FALSE;
}

void CButton::SetIcon(HICON /*icon*/)
{
}

void CButton::SetCheck(int nCheck)
{
  if (nCheck == BST_UNCHECKED || nCheck == BST_CHECKED || nCheck == BST_INDETERMINATE)
  {
    checkState = nCheck;
  }
}

int CButton::GetCheck() const
{
  return checkState;
}

UINT CButton::GetState() const
{
  return checkState | (highlightState ? (BST_PUSHED | BST_FOCUS) : 0); // not sure if highlightState must change state mask
}

void CButton::SetState(BOOL bHighlight)
{
  highlightState = bHighlight;
}

CDialog::CDialog(UINT nIDTemplate, CWnd* /*pParentWnd*/)
{
  templateId = nIDTemplate;

  NanoWinNUILog(NW_STR_A("CDialog::CDialog(nIDTemplate: %u)"),(unsigned int)templateId);
}

INT_PTR CDialog::DoModal()
{
  NanoWinNUILog(NW_STR_A("CDialog::DoModal(nIDTemplate: %u)"),(unsigned int)templateId);

  this->OnInitDialog();

  OnCancel();

  return IDCANCEL;
}

BOOL CDialog::OnInitDialog()
{
  return TRUE;
}

void CDialog::OnOK()
{
}

void CDialog::OnCancel()
{
}

CDialogEx::CDialogEx() : CDialog(0,NULL)
{
}

CDialogEx::CDialogEx(UINT nIDTemplate, CWnd *pParent) : CDialog(nIDTemplate,pParent)
{
}

CDialogEx::CDialogEx(LPCSTR lpszTemplateName, CWnd *pParentWnd) : CDialog(0,pParentWnd)
{
  NanoWinNUILog(NW_STR_A("CDialogEx::CDialogEx[A](template name: " NW_STR_FMT_A ")"),lpszTemplateName);
}

CDialogEx::CDialogEx(LPCWSTR lpszTemplateName, CWnd *pParentWnd) : CDialog(0, pParentWnd)
{
	NanoWinNUILog(NW_STR_W("CDialogEx::CDialogEx[W](template name: " NW_STR_FMT_W ")"), lpszTemplateName);
}

CFileDialog::CFileDialog(BOOL bOpenFileDialog) : CDialog(0, NULL)
{
	ClearFileNameInfo();
}

CFileDialog::CFileDialog(BOOL bOpenFileDialog, LPCSTR lpszDefExt, LPCSTR lpszFileName,
                         DWORD dwFlags, LPCSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize) : CDialog(0,pParentWnd)
{
  ClearFileNameInfo();
}

CFileDialog::CFileDialog(BOOL bOpenFileDialog, LPCWSTR lpszDefExt, LPCWSTR lpszFileName,
                         DWORD dwFlags, LPCWSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize) : CDialog(0,pParentWnd)
{
  ClearFileNameInfo();
}

INT_PTR CFileDialog::DoModal()
{
  NanoWinNUILog(NW_STR_A("CFileDialog::DoModal()"));

  ClearFileNameInfo();

  OnCancel();

  return IDCANCEL;
}

const char *CFileDialog::GetPathName() const
{
  return "";
}

const OPENFILENAME &CFileDialog::GetOFN() const
{
  return fileNameInfo;
}

OPENFILENAME &CFileDialog::GetOFN()
{
  return fileNameInfo;
}

void CFileDialog::ClearFileNameInfo()
{
  memset(&fileNameInfo,0,sizeof(fileNameInfo));

  fileNameInfo.lStructSize = sizeof(fileNameInfo);
  fileNameInfo.lpstrFile   = NW_TCHAR_EMPTY_STR;
}

// MFC App

CWinApp *CWinApp::TheCurrentApp = NULL;
