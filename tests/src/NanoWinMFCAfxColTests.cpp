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

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayElementAt)
{
	CArray<int> arr;

	for (int i = 0; i < 10; i++) { arr.Add(i); }

	for (int i = 0; i < 10; i++)
	{
		NW_CHECK_EQUAL_INTS(i, arr.ElementAt(i));
	}
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayInsertAt)
{
	CArray<int> arr;

	for (int i = 0; i < 5; i++) { arr.Add(i); }

	arr.InsertAt(3, 100);

	for (int i = 0; i < 3; i++) { NW_CHECK_EQUAL_INTS(i, arr.ElementAt(i)); }

	NW_CHECK_EQUAL_INTS(100, arr.ElementAt(3));
	NW_CHECK_EQUAL_INTS(3,   arr.ElementAt(4));
	NW_CHECK_EQUAL_INTS(6,   arr.GetSize());
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArraySetAt)
{
	CArray<int> arr;

	for (int i = 0; i < 5; i++) { arr.Add(i); }

	arr.SetAt(3, 100);
	arr.SetAt(4, 200);

	for (int i = 0; i < 3; i++) { NW_CHECK_EQUAL_INTS(i, arr.ElementAt(i)); }

	NW_CHECK_EQUAL_INTS(100, arr.ElementAt(3));
	NW_CHECK_EQUAL_INTS(200, arr.ElementAt(4));

	NW_CHECK_EQUAL_INTS(5, arr.GetSize());
}

NW_TEST(NanoWinMFCAfxColCArrayTestGroup, CArrayIsEmpty)
{
	CArray<int> arr;

	NW_CHECK_TRUE(arr.IsEmpty());

	arr.Add(1);

	NW_CHECK_FALSE(arr.IsEmpty());
}

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

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCMapTestGroup)

NW_TEST(NanoWinMFCAfxColCMapTestGroup, CMapIsEmptyByDefault)
{
	CMap<int, int, CString, CString> map;

	NW_CHECK_TRUE(map.GetStartPosition() == NULL);
}

NW_TEST(NanoWinMFCAfxColCMapTestGroup, CMapSetAt)
{
	CMap<int, int, CString, CString> map;
	
	POSITION pos;
	int      key;
	CString  str;

	map.SetAt(0, "0");

	pos = map.GetStartPosition();

	map.GetNextAssoc(pos, key, str);

	NW_CHECK_EQUAL_INTS(0, key);
	NW_CHECK_EQUAL_STRCMP(_T("0"), str);
}

NW_TEST(NanoWinMFCAfxColCMapTestGroup, CMapGetNextAssoc)
{
	CMap<int, int, CString, CString> map;

	POSITION pos;
	int      key;
	CString  str;

	map.SetAt(0, "0");
	map.SetAt(1, "1");

	pos = map.GetStartPosition();

	map.GetNextAssoc(pos, key, str);

	NW_CHECK_EQUAL_INTS(0, key);
	NW_CHECK_EQUAL_STRCMP(_T("0"), str);

	map.GetNextAssoc(pos, key, str);

	NW_CHECK_EQUAL_INTS(1, key);
	NW_CHECK_EQUAL_STRCMP(_T("1"), str);
}

NW_TEST(NanoWinMFCAfxColCMapTestGroup, CMapGetStartPosition)
{
	CMap<int, int, CString, CString> map;

	POSITION pos;
	int      key;
	CString  str;

	NW_CHECK_TRUE(map.GetStartPosition() == NULL);

	map.SetAt(0, "0");

	pos = map.GetStartPosition();

	map.GetNextAssoc(pos, key, str);

	NW_CHECK_TRUE(map.GetStartPosition() != NULL);

	NW_CHECK_EQUAL_INTS(0, key);
	NW_CHECK_EQUAL_STRCMP(_T("0"), str);
}

NW_TEST(NanoWinMFCAfxColCMapTestGroup, CMapLookup)
{
	CMap<int, int, CString, CString> map;

	CString  str;

	map.SetAt(0, _T("0"));
	map.SetAt(1, _T("1"));

	map.Lookup(0, str);

	NW_CHECK_EQUAL_STRCMP(_T("0"), str);

	map.Lookup(1, str);

	NW_CHECK_EQUAL_STRCMP(_T("1"), str);
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCStringArrayTestGroup)

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayIsEmptyByDefault)
{
	CStringArray lst;

	NW_CHECK_EQUAL_INTS(0, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayGetSize)
{
	CStringArray lst;

	NW_CHECK_EQUAL_INTS(0, lst.GetSize());

	lst.Add("aaa");
	lst.Add("bbb");

	NW_CHECK_EQUAL_INTS(2, lst.GetSize());
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayGetCount)
{
	CStringArray lst;

	NW_CHECK_EQUAL_INTS(0, lst.GetCount());

	lst.Add("aaa");
	lst.Add("bbb");

	NW_CHECK_EQUAL_INTS(2, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayIsEmpty)
{
	CStringArray lst;

	NW_CHECK_TRUE(lst.IsEmpty());

	lst.Add("aaa");
	lst.Add("bbb");

	NW_CHECK_FALSE(lst.IsEmpty());
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayRemoveAll)
{
	CStringArray lst;

	lst.Add("aaa");
	lst.Add("bbb");

	lst.RemoveAll();

	NW_CHECK_TRUE(lst.IsEmpty());
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringArrayAdd)
{
	CStringArray lst;

	lst.Add("aaa");

	NW_CHECK_EQUAL_INTS(1, lst.GetCount());
	NW_CHECK_EQUAL_STRCMP(_T("aaa"), lst.GetAt(0));
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringGetAt)
{
	CStringArray lst;

	lst.Add("aaa");
	lst.Add("bbb");

	NW_CHECK_EQUAL_STRCMP(_T("aaa"), lst.GetAt(0));
	NW_CHECK_EQUAL_STRCMP(_T("bbb"), lst.GetAt(1));
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringAppend)
{
	CStringArray lst1;
	CStringArray lst2;

	lst1.Add("aaa");

	lst2.Add("bbb");
	lst2.Add("ccc");

	lst1.Append(lst2);

	NW_CHECK_EQUAL_INTS(3, lst1.GetCount());

	NW_CHECK_EQUAL_STRCMP(_T("aaa"), lst1.GetAt(0));
	NW_CHECK_EQUAL_STRCMP(_T("bbb"), lst1.GetAt(1));
	NW_CHECK_EQUAL_STRCMP(_T("ccc"), lst1.GetAt(2));
}

NW_TEST(NanoWinMFCAfxColCStringArrayTestGroup, CStringOperator)
{
	CStringArray lst;

	lst.Add("aaa");
	lst.Add("bbb");

	NW_CHECK_EQUAL_STRCMP(lst[0], lst.GetAt(0));
	NW_CHECK_EQUAL_STRCMP(lst[1], lst.GetAt(1));
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxColCMapStringToPtrTestGroup)

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrIsEmptyByDefault)
{
	CMapStringToPtr lst;

	NW_CHECK_EQUAL_INTS(0, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrGetCount)
{
	CMapStringToPtr lst;

	int arr[2] = { 0, 1 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);

	NW_CHECK_EQUAL_INTS(2, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrRemoveAll)
{
	CMapStringToPtr lst;

	int arr[2] = { 0, 1 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);

	NW_CHECK_EQUAL_INTS(2, lst.GetCount());

	lst.RemoveAll();

	NW_CHECK_EQUAL_INTS(0, lst.GetCount());
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrGetStartPosition)
{
	CMapStringToPtr lst;

	int arr[3] = { 1, 2, 3 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);
	lst.SetAt(_T("third"), &arr[2]);

	POSITION pos = lst.GetStartPosition();

	CString str;
	void   *val;

	lst.GetNextAssoc(pos, str, val);
	
	NW_CHECK_EQUAL(&arr[0], val);
	NW_CHECK_EQUAL_STRCMP(str, _T("first"));
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrGetStartPositionEmpty)
{
	CMapStringToPtr lst;

	POSITION pos = lst.GetStartPosition();

	NW_CHECK(pos == NULL);
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrGetNextAssoc)
{
	CMapStringToPtr lst;
	CString         str;
	void           *val;

	int arr[3] = { 1, 2, 3 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);
	lst.SetAt(_T("third"), &arr[2]);

	POSITION pos = lst.GetStartPosition();

	lst.GetNextAssoc(pos, str, val);

	NW_CHECK_EQUAL(&arr[0], val);
	NW_CHECK_EQUAL_STRCMP(str, _T("first"));

	lst.GetNextAssoc(pos, str, val);

	NW_CHECK_EQUAL(&arr[1], val);
	NW_CHECK_EQUAL_STRCMP(str, _T("second"));
}



NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrLookup)
{
	CMapStringToPtr lst;

	int arr[2] = { 1, 2 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);

	void *result;

	NW_CHECK_TRUE(lst.Lookup(_T("first"), result));
	NW_CHECK_EQUAL(&arr[0], result);

	NW_CHECK_FALSE(lst.Lookup(_T("third"), result));
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrLookupKey)
{
	CMapStringToPtr lst;

	int arr[2] = { 1, 2 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);

	LPCTSTR result;

	NW_CHECK_TRUE(lst.LookupKey(_T("first"), result));
	NW_CHECK_EQUAL_STRCMP(_T("first"), result);

	NW_CHECK_FALSE(lst.LookupKey(_T("third"), result));
}

NW_TEST(NanoWinMFCAfxColCMapStringToPtrTestGroup, CMapStringToPtrSetAt)
{
	CMapStringToPtr lst;

	int arr[3] = { 1, 2 };

	lst.SetAt(_T("first"), &arr[0]);
	lst.SetAt(_T("second"), &arr[1]);

	NW_CHECK_EQUAL_INTS(2, lst.GetCount());
	
	void *result;

	NW_CHECK_TRUE(lst.Lookup(_T("first"), result));
	NW_CHECK_EQUAL(&arr[0], result);

	NW_CHECK_TRUE(lst.Lookup(_T("second"), result));
	NW_CHECK_EQUAL(&arr[1], result);
}

NW_END_TEST_GROUP()