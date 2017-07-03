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
#include "NanoWinMFCAfxWin.h"

// MFC Controls subset
// -----------------------------------------

// Constants for CListCtrl
#define LVCFMT_LEFT   0x0000
#define LVCFMT_RIGHT  0x0001
#define LVCFMT_CENTER 0x0002

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
#define afx_msg         // intentional placeholder [just a source marker]
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

  //Note: decendant may have only SetIcon(HICON hIcon)?
  //HICON SetIcon(HICON hIcon,  BOOL bBigIcon = TRUE); 
  void SetIcon(HICON hIcon, BOOL bBigIcon = TRUE)
  {
    UNREFERENCED_PARAMETER(hIcon);
    UNREFERENCED_PARAMETER(bBigIcon);
  }

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

  BOOL    SetColumnWidth(int nCol, int cx) { UNREFERENCED_PARAMETER(nCol); UNREFERENCED_PARAMETER(cx); return TRUE; }

  BOOL    SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
  CString GetItemText(int nItem, int nSubItem) const;

  int     GetHotItem() { return(-1); }

  BOOL    DeleteAllItems();
  BOOL    DeleteColumn(int nCol);

  int     GetColumnCount() const // Get the column count.
  {
    return(columnHeaders.size());
  }

  int     InsertItem  (int nItem, LPCTSTR lpszItem);
  int     InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);

  private :

  typedef std::vector<CString> CStringVector;

  CStringVector              columnHeaders;
  std::vector<CStringVector> items;
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

// MFC misc items
// ==================================

#include "NanoWinTCHAR.h"
#include "NanoWinOther.h"

typedef const RECT *PCRECT;
typedef const RECT *LPCRECT;

inline BOOL AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite = TRUE)
{
  if (bReadWrite) { return(!IsBadWritePtr(const_cast<LPVOID>(lp), nBytes)); }
  else            { return(!IsBadReadPtr(lp, nBytes));  }
}

inline BOOL AfxIsValidString(LPCSTR lpsz, int nLength = -1)
{
  if (lpsz == NULL) { return(FALSE); }
  return(TRUE);
}

inline BOOL AfxIsValidString(LPCWSTR lpsz, int nLength = -1)
{
  if (lpsz == NULL) { return(FALSE); }
  return(TRUE);
}

#if defined(DEBUG) || defined(_DEBUG)
#define TRACE          (0) && // Prevent evaluation of fail expression, since lasy evaluation policy: TRACE(x) becomes (0) && (x), so x would not evaluated
#else
#define TRACE          printf
#endif // !DEBUG

// Bindings

#define DDX_Text(pDX,id,ctrl)       // TODO: Do nothing
#define DDX_Control(pDX,id,ctrl)    // TODO: Do nothing
#define DDX_Check(pDX,id,ctrl)      // TODO: Do nothing
#define DDX_Radio(pDX,id,ctrl)      // TODO: Do nothing

// Message map

#define DECLARE_MESSAGE_MAP() // TODO: Do nothing

#define BEGIN_MESSAGE_MAP(class_name, base_class_name) static void class_name##_MESSAGE_MAP() {

#define ON_BN_CLICKED(id,handler)    // TODO: Do nothing
#define ON_STN_CLICKED(id,handler)   // TODO: Do nothing
#define ON_COMMAND(id,handler)       // TODO: Do nothing
#define ON_WM_PAINT()                // TODO: Do nothing

#define END_MESSAGE_MAP() }

// MFC Dialog box

inline int AfxMessageBox(const TCHAR *lpszText, int nType, int nIDHelp)
{
  UNREFERENCED_PARAMETER(nIDHelp);
  return(MessageBox(NULL, lpszText, NULL, nType));
}

// MFC App

class CWinApp
{
  private:

  static CWinApp     *TheCurrentApp;
  static CWinApp     *GetCurrentApp() { return(TheCurrentApp); }
  static void         SetCurrentApp(CWinApp *pApp) { TheCurrentApp = pApp; };

  friend CWinApp     *AfxGetApp();

  public:

  CWinApp()
  {
    SetCurrentApp(this); // Not sure this is correct place to do this
  }

  virtual BOOL InitInstance() { return(TRUE); }
  virtual int  Run()          { return(0);    } // Do nothing here
  virtual BOOL ExitInstance() { return(TRUE); }

  HICON LoadIcon(UINT nIDResource) const { UNREFERENCED_PARAMETER(nIDResource); return(NULL); }

  afx_msg void OnHelp() { } // Empty

  // members

  CWnd  *m_pMainWnd  = NULL;
  TCHAR *m_lpCmdLine = _T("");
};

inline CWinApp *AfxGetApp() { return(CWinApp::GetCurrentApp()); }

#define AfxEnableControlContainer() // Empty

// NanoSubset of AfxThreads

#include "NanoWinThreads.h"

typedef PTHREAD_START_ROUTINE AFX_THREADPROC;
inline void AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam)
{
  DWORD threadId;
  CreateThread(NULL, 0, pfnThreadProc, pParam, 0, &threadId);
}

inline void AFXAPI AfxEndThread(UINT nExitCode)
{
  ExitThread(nExitCode);
}

#endif // LINUX

#endif
