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

// Win32 API GUI (types and functions)
// -----------------------------------------

typedef LONG_PTR LPARAM;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LRESULT;

typedef HANDLE HCURSOR;
typedef HANDLE HICON;

// Dialog Box Command IDs

#define IDOK       1
#define IDCANCEL   2
#define IDABORT    3
#define IDRETRY    4
#define IDIGNORE   5
#define IDYES      6
#define IDNO       7
#define IDCLOSE    8
#define IDHELP     9
#define IDTRYAGAIN 10
#define IDCONTINUE 11

// Helper class for storing strings which supports both UNICODE and multibyte strings
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

// Type modifier for message handlers
#ifndef afx_msg
 #define afx_msg
#endif

class CDataExchange
{
};

class  CWnd
{
  public:

  CWnd ();

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

  BOOL EnableWindow  (BOOL bEnable = TRUE);

  CWnd* GetDlgItem   (int nID) const;
  BOOL  UpdateData   (BOOL bSaveAndValidate = TRUE);

  protected : 
  
  virtual void DoDataExchange(CDataExchange* pDX);

  virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT *pResult);

  afx_msg void OnDestroy();

  private :

  bool           isEnabled;
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

  int     GetHotItem();

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

  int  GetCount    () const;
  void GetText     (int nIndex, CString& rString) const;
  int  AddString   (LPCTSTR lpszItem);

  void ResetContent();

  private :

  std::vector<CString> listStorage;
};

#define CB_ERR (-1)

class CComboBox : public CWnd
{
  public:

  CComboBox ();

  int AddString(LPCTSTR lpszItem);

  int SetCurSel(int nSelect);
  int GetCurSel() const;

  private :

  std::vector<CString> listStorage;
  int                  currSelection;
};

// CButton GetCheck/SetCheck
#define BST_UNCHECKED      0x0000 // The initial state.
#define BST_CHECKED        0x0001 // The button control is checked.
#define BST_INDETERMINATE  0x0002 // The state is indeterminate (only possible when the button control has three states).
// CButton GetState
#define BST_PUSHED         0x0004 // The button control is pressed.
#define BST_FOCUS          0x0008 // The button control has the focus.

class CButton
{
  public :

  CButton ();

  void SetIcon(HICON icon);

  void SetCheck(int nCheck);
  int  GetCheck() const;

  UINT GetState() const;
  void SetState(BOOL bHighlight);

  private :

  int  checkState;
  BOOL highlightState;
};

class CImageList : public CObject
{
};


class CDialog : public CWnd
{
  public:

  explicit        CDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

  virtual INT_PTR DoModal();

  virtual BOOL    OnInitDialog();

  protected :

  virtual void    OnOK();
  virtual void    OnCancel();

  private :

  UINT templateId;
};

class CDialogEx : public CDialog
{
  public:

  CDialogEx();
  CDialogEx(UINT nIDTemplate, CWnd *pParent = NULL);
  CDialogEx(LPCTSTR lpszTemplateName, CWnd *pParentWnd = NULL);
};

#define OFN_OVERWRITEPROMPT  (0x00000002)
#define OFN_HIDEREADONLY     (0x00000004)

typedef struct tagOFN
{
  DWORD  lStructSize;
  LPTSTR lpstrFile;
  DWORD  nMaxFile;
} OPENFILENAME, *LPOPENFILENAME;

class CFileDialog : public CDialog // in MFC CFileDialog inherits CCommonDialog
{
  public:

  explicit CFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL,
                       DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL,
                       CWnd* pParentWnd = NULL, DWORD dwSize = 8);

  CString             GetPathName() const;

  const OPENFILENAME& GetOFN() const;
  OPENFILENAME&       GetOFN();

  virtual INT_PTR     DoModal();

  private :

  void                ClearFileNameInfo ();

  OPENFILENAME fileNameInfo;
};

#endif // LINUX

#endif
