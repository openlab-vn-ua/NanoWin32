// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFC UI emulation class

#include <stdio.h>

#include "NanoWinMFCAfxNUI.h"

// NW specials
// -----------------------------------------

// Proc

bool NanoWinNUIRunProc::TheNUILogActive = true;

void NanoWinNUIRunProc::NanoWinNUIRunProc::Log(const char    *text)
{
  printf("NUILog: %s\n",text);
}

void NanoWinNUIRunProc::RunMessageLoopDoModal()
{
  while(!IsRunMessageLoopDoModalComplete())
  {
    Sleep(1*1000);
    Log("RunMessageLoopDoModal::heartbeat");
  }
}

void NanoWinNUIRunProc::RunMessageLoopApp()
{
  while(true)
  {
    auto App = AfxGetApp();
    if (App != NULL) { App->IsPostQuitMessageDone();  break; }
    Sleep(1*1000);
    Log("RunMessageLoopApp::heartbeat");
  }
}

static NanoWinNUIRunProc NanoWinNUIRunDefaultProc;
NanoWinNUIRunProc *NanoWinNUIRunProc::TheCurrentProc = &NanoWinNUIRunDefaultProc;

#define NW_STR_FMT_W "%ls"
#define NW_STR_FMT_A "%s"

static void NanoWinNUILog(const char *format, ...)
{
  auto Proc = NanoWinNUIRunGetProc();

  if ((Proc != NULL) && (Proc->IsNUILogActive()))
  {
    CStringA str;
    va_list  args;

    va_start(args,format);
    str.FormatV(format,args);
    va_end(args);

    Proc->Log(str.GetString());
  }
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

// MFC Controls subset
// -----------------------------------------

CWnd::CWnd()
{
  isEnabled = true;
}

void CWnd::SetWindowTextA(LPCSTR    lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog("CWnd::SetWindowTextA(" NW_STR_FMT_A ")",lpszString);
}

void CWnd::SetWindowTextW(LPCWSTR   lpszString)
{
  windowText.SetText(lpszString);

  NanoWinNUILog("CWnd::SetWindowTextW(" NW_STR_FMT_W ")",lpszString);
}

void CWnd::GetWindowTextA(CStringA &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog("CWnd::GetWindowTextA(" NW_STR_FMT_A ")",rString.GetString());
}

void CWnd::GetWindowTextW(CStringW &rString) const
{
  windowText.GetText(rString);

  NanoWinNUILog("CWnd::GetWindowTextW(" NW_STR_FMT_W ")",rString.GetString());
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

  NanoWinNUILog("CListCtrl::SetItemText[A](item: %d, subitem: %d, text: " NW_STR_FMT_A ")",nItem,nSubItem,lpszText);

  return TRUE;
}

BOOL    CListCtrl::SetItemText(int nItem, int nSubItem, LPCWSTR lpszText)
{
  if ((CStringVector::size_type)nItem    >= items.size())         { return FALSE; }
  if ((CStringVector::size_type)nSubItem >= columnHeaders.size()) { return FALSE; }

  items[nItem][nSubItem] = lpszText;

  NanoWinNUILog("CListCtrl::SetItemText[W](item: %d, subitem: %d, text: " NW_STR_FMT_W ")",nItem,nSubItem,lpszText);

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

  NanoWinNUILog("CListCtrl::DeleteAllItems()");
  
  return TRUE;
}

int CListCtrl::InsertColumn (int nCol, LPCSTR lpszColumnHeading, int /*nFormat*/, int /*nWidth*/, int /*nSubItem*/)
{
  NanoWinNUILog("CListCtrl::InsertColumn(nCol: %d heading: " NW_STR_FMT_A ")",nCol,lpszColumnHeading);

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

    return static_cast<int>(columnHeaders.size() - 1);
  }

  return nCol;
}

int CListCtrl::InsertColumn (int nCol, LPCWSTR lpszColumnHeading, int /*nFormat*/, int /*nWidth*/, int /*nSubItem*/)
{
  NanoWinNUILog("CListCtrl::InsertColumn(nCol: %d heading: " NW_STR_FMT_W ")",nCol,lpszColumnHeading);

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

    return static_cast<int>(columnHeaders.size() - 1);
  }

  return nCol;
}


BOOL CListCtrl::DeleteColumn (int nCol)
{
  NanoWinNUILog("CListCtrl::DeleteColumn(nCol: %d)",nCol);

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
  NanoWinNUILog("CListCtrl::InsertItem[A](nItem: %d, item: " NW_STR_FMT_A ")",nItem,lpszItem);

  if ((CStringVector::size_type)nItem < items.size())
  {
    items.insert(items.begin() + nItem,CStringVector());
  }
  else
  {
    nItem = static_cast<int>(items.size());

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
  NanoWinNUILog("CListCtrl::InsertItem[W](nItem: %d, item: " NW_STR_FMT_W ")",nItem,lpszItem);

  if ((CStringVector::size_type)nItem < items.size())
  {
    items.insert(items.begin() + nItem,CStringVector());
  }
  else
  {
    nItem = static_cast<int>(items.size());

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

  NanoWinNUILog("CListBox::AddString[A](index: %d, text: " NW_STR_FMT_A ")",index,lpszItem);

  return index;
}

int  CListBox::AddString (LPCWSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(CW2A(lpszItem));

  NanoWinNUILog("CListBox::AddString[W](index: %d, text: " NW_STR_FMT_W ")",index,lpszItem);

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

  NanoWinNUILog("CComboBox::AddString[A](index: %d, text: " NW_STR_FMT_A ")",index,lpszItem);

  return index;
}

int  CComboBox::AddString (LPCWSTR lpszItem)
{
  int index = (int)listStorage.size();

  listStorage.push_back(CW2A(lpszItem));

  NanoWinNUILog("CComboBox::AddString[W](index: %d, text: " NW_STR_FMT_W ")",index,lpszItem);

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

  this->OnInitDialog();

  auto Proc = NanoWinNUIRunGetProc();
  if (Proc != NULL) { Proc->RunMessageLoopDoModal(); }

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
  NanoWinNUILog("CDialogEx::CDialogEx[A](template name: " NW_STR_FMT_A ")", lpszTemplateName);
}

CDialogEx::CDialogEx(LPCWSTR lpszTemplateName, CWnd *pParentWnd) : CDialog(0, pParentWnd)
{
	NanoWinNUILog("CDialogEx::CDialogEx[W](template name: " NW_STR_FMT_W ")", lpszTemplateName);
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
  NanoWinNUILog("CFileDialog::DoModal()");

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
int      CWinApp::TheExitCode = 0;
bool     CWinApp::TheExitCodeIsSet = false;
