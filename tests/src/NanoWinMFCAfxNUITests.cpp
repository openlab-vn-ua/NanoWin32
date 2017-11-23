#include <vector>
#include <map>
#include "NWUnitTest.h"

#ifdef __linux
#include "NanoWinMFCAfxNUI.h"
#else
#include <windows.h>
#endif

NW_BEGIN_TEST_GROUP(NanoWinMFCAfxNUITestGroup)

NW_BEGIN_SETUP_TEARDOWN
NW_SETUP_METHOD()
 {
   NanoWinNUIRunGetProc()->SetNUILogActive(false);
 }
NW_TEARDOWN_METHOD()
 {
   NanoWinNUIRunGetProc()->SetNUILogActive(true);
 }
NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinMFCAfxNUITestGroup, SetWindowTextATest)
{
  CWnd cwnd;
  const char *mess = "hello";
  CStringA res;

  cwnd.SetWindowTextA(mess);
  cwnd.GetWindowTextA(res);

  NW_CHECK_EQUAL_STRCMP("hello", res.GetString());
}

NW_TEST(NanoWinMFCAfxNUITestGroup, SetWindowTextWTest)
{
  CWnd cwnd;
  const wchar_t *mess = L"hello";
  CStringW res;

  cwnd.SetWindowTextW(mess);
  cwnd.GetWindowTextW(res);

  NW_CHECK_EQUAL_MEMCMP(L"hello", res.GetString(), 5 * sizeof(wchar_t));
}

NW_TEST(NanoWinMFCAfxNUITestGroup, CListCtrlInsertColumnTest)
{
  CListCtrl list;

  int index1 = list.InsertColumn(0, _T("Column 1"));
  NW_CHECK_EQUAL(0, index1);

  int index2 = list.InsertColumn(0, _T("Column 2"));
  NW_CHECK_EQUAL(0, index2);

  int index3 = list.InsertColumn(1, _T("Column 3"));
  NW_CHECK_EQUAL(1, index3);

  int index4 = list.InsertColumn(10, _T("Column 4"));
  NW_CHECK_EQUAL(3, index4);
}

NW_TEST(NanoWinMFCAfxNUITestGroup, CListCtrlGetColumnCountTest)
{
  CListCtrl list;

  int index1 = list.InsertColumn(0, _T("Column 1"));
  NW_CHECK_EQUAL(0, index1);

  int index2 = list.InsertColumn(0, _T("Column 2"));
  NW_CHECK_EQUAL(0, index2);

  int index3 = list.InsertColumn(1, _T("Column 3"));
  NW_CHECK_EQUAL(1, index3);

  int count = list.GetColumnCount();
  NW_CHECK_EQUAL(3, count);

  list.DeleteColumn(2);

  count = list.GetColumnCount();
  NW_CHECK_EQUAL(2, count);
}

NW_TEST(NanoWinMFCAfxNUITestGroup, CListCtrlDeleteColumnTest)
{
  CListCtrl list;

  int index1 = list.InsertColumn(0, _T("Column 1"));
  NW_CHECK_EQUAL(0, index1);

  int index2 = list.InsertColumn(1, _T("Column 2"));
  NW_CHECK_EQUAL(1, index2);

  int index3 = list.InsertColumn(2, _T("Column 3"));
  NW_CHECK_EQUAL(2, index3);
  
  list.DeleteColumn(1);

  int index4 = list.InsertColumn(10, _T("Column 4"));
  NW_CHECK_EQUAL(2, index4);
  
}

NW_TEST(NanoWinMFCAfxNUITestGroup, CListCtrlInsertItemTest)
{
  CListCtrl list;

  list.InsertColumn(0, _T("Column 1"));
  
  int index1 = list.InsertItem(0, _T("Item 1"));
  NW_CHECK_EQUAL(0, index1);
  
  int index2 = list.InsertItem(0, _T("Item 2"));
  NW_CHECK_EQUAL(0, index2);

  int index3 = list.InsertItem(1, _T("Item 3"));
  NW_CHECK_EQUAL(1, index3);

  int index4 = list.InsertItem(10, _T("Item 4"));
  NW_CHECK_EQUAL(3, index4);
}

NW_TEST(NanoWinMFCAfxNUITestGroup, CListCtrlDeleteAllItemsTest)
{
  CListCtrl list;

  list.InsertColumn(0, _T("Column 1"));

  int index1 = list.InsertItem(0, _T("Item 1"));
  NW_CHECK_EQUAL(0, index1);

  int index2 = list.InsertItem(1, _T("Item 2"));
  NW_CHECK_EQUAL(1, index2);

  list.DeleteAllItems();

  int index3 = list.InsertItem(3, _T("Item 2"));
  NW_CHECK_EQUAL(0, index3);
}

NW_TEST(NanoWinMFCAfxNUITestGroup, CListCtrlSetItemTextTest)
{
  CListCtrl list;
  CString str;

  list.InsertColumn(0, _T("Column 1"));

  int index1 = list.InsertItem(0, _T("Item 1"));
  NW_CHECK_EQUAL(0, index1);

  bool res = list.SetItemText(0, 0, _T("item text"));
  NW_CHECK_TRUE(res);

  str = list.GetItemText(0, 0);
  NW_CHECK_EQUAL_STRCMP(_T("item text"), str.GetString());
}

NW_END_TEST_GROUP()