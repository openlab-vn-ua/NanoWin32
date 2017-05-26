#include <map>
#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxCol.h"
 #include <time.h>
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCArrayTestGroup)

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayIsEmptyByDefault)
{
  CArray<int> arr;

  NW_CHECK_EQUAL_INTS(0,arr.GetCount());
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCListTestGroup)

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListIsEmptyByDefault)
{
  CList<int> lst;

  NW_CHECK_TRUE(lst.IsEmpty());
  NW_CHECK_EQUAL_INTS(0,lst.GetCount());
}

NW_END_TEST_GROUP()

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCStringArrayTestGroup)

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayIsEmptyByDefault)
{
  CStringArray lst;

  NW_CHECK_EQUAL_INTS(0,lst.GetCount());
}

NW_END_TEST_GROUP()

