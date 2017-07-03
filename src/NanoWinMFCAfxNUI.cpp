// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFC UI emulation class

#include <stdio.h>

#include "NanoWinMFCAfxNUI.h"

#if defined(UNICODE) || defined(_UNICODE)
 #define NW_STR_FMT "%ls"
#else
 #define NW_STR_FMT "%s"
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

#if defined(UNICODE) || defined(_UNICODE)

void NanoWinTextStr::SetText(LPCSTR    lpszString)
{
  str.SetString(CA2W(lpszString));
}

void NanoWinTextStr::SetText(LPCWSTR   lpszString)
{
  str.SetString(lpszString);
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

CWnd::CWnd()
{
  isEnabled = true;
}

void CWnd::SetWindowTextA(LPCSTR    lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog("CWnd::SetWindowTextA(%s)",lpszString);
}

void CWnd::SetWindowTextW(LPCWSTR   lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog(L"CWnd::SetWindowTextW(%ls)",lpszString);
}

void CWnd::GetWindowTextA(CStringA &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog("CWnd::GetWindowTextA(%s)",rString.GetString());
}

void CWnd::GetWindowTextW(CStringW &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog(L"CWnd::GetWindowTextW(%ls)",rString.GetString());
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

BOOL    CListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
  if ((CStringVector::size_type)nItem    >= items.size())         { return FALSE; }
  if ((CStringVector::size_type)nSubItem >= columnHeaders.size()) { return FALSE; }

  items[nItem][nSubItem] = CString(lpszText);

  NanoWinNUILog(_T("CListCtrl::SetItemText(item: %d, subitem: %d, text: ") NW_STR_FMT _T(")"),nItem,nSubItem,lpszText);

  return TRUE;
}

CString CListCtrl::GetItemText(int nItem, int nSubItem) const
{
  if ((CStringVector::size_type)nItem    >= items.size())         { return CString(); }
  if ((CStringVector::size_type)nSubItem >= columnHeaders.size()) { return CString(); }

  return items[nItem][nSubItem];
}

BOOL CListCtrl::DeleteAllItems ()
{
  items.clear();

  NanoWinNUILog(_T("CListCtrl::DeleteAllItems()"));
  
  return TRUE;
}

int CListCtrl::InsertColumn (int nCol, LPCTSTR lpszColumnHeading, int /*nFormat*/, int /*nWidth*/, int /*nSubItem*/)
{
  NanoWinNUILog(_T("CListCtrl::InsertColumn(nCol: %d heading: ") NW_STR_FMT _T(")"),nCol,lpszColumnHeading);

  if ((CStringVector::size_type)nCol < columnHeaders.size())
  {
    columnHeaders.insert(columnHeaders.begin() + nCol,CString());

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      it->insert(it->begin() + nCol,CString());
    }

    return nCol;
  }
  else
  {
    columnHeaders.push_back(lpszColumnHeading);

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      it->push_back(CString());
    }

    return columnHeaders.size() - 1;
  }

  return nCol;
}

BOOL CListCtrl::DeleteColumn (int nCol)
{
  NanoWinNUILog(_T("CListCtrl::DeleteColumn(nCol: %d)"),nCol);

  if ((CStringVector::size_type)nCol >= columnHeaders.size()) { return FALSE; }

  columnHeaders.erase(columnHeaders.begin() + nCol);

  for (auto it = items.begin(); it != items.end(); ++it)
  {
    it->erase(it->begin() + nCol);
  }

  return TRUE;
}

int CListCtrl::InsertItem (int nItem, LPCTSTR lpszItem)
{
  NanoWinNUILog(_T("CListCtrl::InsertItem(nItem: %d, item: ") NW_STR_FMT _T(")"),nItem,lpszItem);

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
    newItem.push_back(CString());
  }

  return nItem;
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

  NanoWinNUILog(_T("CListBox::AddString(index: %d, text: ") NW_STR_FMT _T(")"),index,lpszItem);

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

int  CComboBox::AddString (LPCTSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(lpszItem);

  NanoWinNUILog(_T("CComboBox::AddString(index: %d, text: ") NW_STR_FMT _T(")"),index,lpszItem);

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

  NanoWinNUILog("CDialog::CDialog(nIDTemplate: %u)",(unsigned int)templateId);
}

INT_PTR CDialog::DoModal()
{
  NanoWinNUILog("CDialog::DoModal(nIDTemplate: %u)",(unsigned int)templateId);

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

CDialogEx::CDialogEx(LPCTSTR lpszTemplateName, CWnd *pParentWnd) : CDialog(0,pParentWnd)
{
  NanoWinNUILog(_T("CDialogEx::CDialogEx(template name: ") NW_STR_FMT _T(")"),lpszTemplateName);
}

CFileDialog::CFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
                         DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize) : CDialog(0,pParentWnd)
{
  ClearFileNameInfo();
}

INT_PTR CFileDialog::DoModal()
{
  NanoWinNUILog("CFileDialog::DoModal");

  ClearFileNameInfo();

  OnCancel();

  return IDCANCEL;
}

CString CFileDialog::GetPathName() const
{
  return CString();
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
}

// MFC App

CWinApp *CWinApp::TheCurrentApp = NULL;
