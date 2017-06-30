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

  virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT *pResult);

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

#endif // LINUX

#endif
