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

	NW_CHECK_EQUAL_INTS(0, arr.GetCount());
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayGetAt)
{
	CArray<int> arr;

	for (int i = 0; i < 10; i++) { arr.Add(i); }

	NW_CHECK_EQUAL_INTS(10, arr.GetCount());
	
	for (int i = 0; i < 10; i++) 
	{
		NW_CHECK_EQUAL_INTS(i, arr.GetAt(i));
	}
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayGetSize)
{
	CArray<int> arr;

	NW_CHECK_EQUAL_INTS(0, arr.GetSize());

	arr.Add(1);
	arr.Add(2);

	NW_CHECK_EQUAL_INTS(2, arr.GetSize());
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArraySetSize)
{
	CArray<int> arr;

	arr.SetSize(64);
	NW_CHECK_EQUAL_INTS(64, arr.GetSize());

	arr.SetSize(32);
	NW_CHECK_EQUAL_INTS(32, arr.GetSize());
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayAdd)
{
	CArray<int> arr;

	arr.Add(5);

	NW_CHECK_EQUAL_INTS(5, arr.GetAt(0));
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayAppend)
{
	CArray<int> arr1;
	CArray<int> arr2;

	for (int i = 0; i < 10; i++)
	{
		arr1.Add(i);
		arr2.Add(i);
	}

	arr1.Append(arr2);

	NW_CHECK_EQUAL_INTS(20, arr1.GetSize());

	for (int i = 0; i < 10; i++)
	{
		NW_CHECK_EQUAL_INTS(arr1.GetAt(i), arr1.GetAt(i + 10));
	}
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayCopy)
{
	CArray<int> arr1;
	CArray<int> arr2;

	for (int i = 0; i < 10; i++)
	{
		arr2.Add(i);
	}

	arr1.Copy(arr2);

	NW_CHECK_EQUAL_INTS(arr1.GetSize(), arr2.GetSize());

	for (int i = 0; i < 10; i++)
	{
		NW_CHECK_EQUAL_INTS(arr1.GetAt(i), arr2.GetAt(i));
	}
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayRemoveAll)
{
	CArray<int> arr;

	arr.Add(1);
	arr.Add(2);

	arr.RemoveAll();

	NW_CHECK_EQUAL_INTS(0, arr.GetSize());
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayRemoveAt)
{
	CArray<int> arr;

	for (int i = 0; i < 10; i++)
	{
		arr.Add(i);
	}

	arr.RemoveAt(0);
	NW_CHECK_EQUAL_INTS(9, arr.GetSize());

	arr.RemoveAt(2, 4);
	NW_CHECK_EQUAL_INTS(5, arr.GetSize());
}
/*
NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayElementAt)
{
	CArray<int> arr;
}
*/

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCListTestGroup)

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListIsEmptyByDefault)
{
	CList<int> lst;

	NW_CHECK_TRUE(lst.IsEmpty());
	NW_CHECK_EQUAL_INTS(0, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListGetCount)
{
	CList<int> lst;

	for (int i = 0; i < 5; i++)
	{
		lst.AddHead(i);
	}

	NW_CHECK_EQUAL_INTS(5, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListFind)
{
 	CList<int> lst;

	for (int i = 0; i < 5; i++)
	{
		lst.AddTail(i);
	}

	for (int i = 0; i < 5; i++)
	{
		POSITION pos = lst.Find(i);
		NW_CHECK_EQUAL_INTS(i, lst.GetNext(pos));
	}
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListRemoveHead)
{
	CList<int> lst;

	lst.AddHead(1);
	lst.RemoveHead();
	NW_CHECK_TRUE(lst.IsEmpty());

	for (int i = 0; i < 5; i++)
	{
		lst.AddTail(i);
	}

	int val = lst.RemoveHead();

	NW_CHECK_EQUAL_INTS(4, lst.GetCount());
	NW_CHECK_EQUAL_INTS(0, val);

	for (int i = 1; i < 5; i++)
	{
		POSITION pos = lst.Find(i);
		NW_CHECK_EQUAL_INTS(i, lst.GetNext(pos));
	}
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListRemoveTail)
{
	CList<int> lst;

	lst.AddTail(1);
	lst.RemoveTail();
	NW_CHECK_TRUE(lst.IsEmpty());

	for (int i = 0; i < 5; i++)
	{
		lst.AddTail(i);
	}

	int val = lst.RemoveTail();

	NW_CHECK_EQUAL_INTS(4, lst.GetCount());
	NW_CHECK_EQUAL_INTS(4, val);
	
	for (int i = 0; i < 4; i++)
	{
		POSITION pos = lst.Find(i);
		NW_CHECK_EQUAL_INTS(i, lst.GetNext(pos));
	}
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListAddTail)
{
	CList<int> lst;

	lst.AddTail(0);
	lst.AddTail(1);
	lst.AddTail(2);

	NW_CHECK_EQUAL_INTS(3, lst.GetCount());

	for (int i = 0; i < 3; i++)
	{
		POSITION pos = lst.Find(i);
		NW_CHECK_EQUAL_INTS(i, lst.GetNext(pos));
	}
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListAddHead)
{
	CList<int> lst;

	lst.AddHead(2);
	lst.AddHead(1);
	lst.AddHead(0);

	NW_CHECK_EQUAL_INTS(3, lst.GetCount());

	for (int i = 2; i >= 0; i--)
	{
		POSITION pos = lst.Find(i);
		NW_CHECK_EQUAL_INTS(i, lst.GetNext(pos));
	}
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListGetHeadPosition)
{
	CList<int> lst;

	lst.AddTail(1);
	lst.AddTail(2);
	lst.AddTail(3);

	POSITION pos1 = lst.Find(1);
	int res1 = lst.GetNext(pos1);

	POSITION pos2 = lst.GetHeadPosition();
	int res2 = lst.GetNext(pos2);

	NW_CHECK_EQUAL_INTS(res1, res2);
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListGetNext)
{
	CList<int> lst;

	lst.AddTail(1);
	lst.AddTail(2);
	lst.AddTail(3);

	POSITION pos = lst.Find(3);
	int val = lst.GetNext(pos);

	NW_CHECK_EQUAL(val, 3);

	NW_CHECK(pos == NULL);
}

NW_TEST(NanoWinMFCAfxColCListTestGroup, CListRemoveAll)
{
	CList<int> lst;

	lst.AddTail(1);
	lst.AddTail(2);
	lst.AddTail(3);

	lst.RemoveAll();

	NW_CHECK_TRUE(lst.IsEmpty());
	NW_CHECK_EQUAL_INTS(0, lst.GetCount());
}

NW_END_TEST_GROUP()



NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCStringArrayTestGroup)

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayIsEmptyByDefault)
{
	CStringArray lst;

	NW_CHECK_EQUAL_INTS(0, lst.GetCount());
}

NW_END_TEST_GROUP()

