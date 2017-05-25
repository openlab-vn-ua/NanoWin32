// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCStr functions

#if !defined(NanoWinMFCAfxColIncluded)
#define NanoWinMFCAfxColIncluded

#include "NanoWinTypes.h"

#if defined LINUX

// MFC subset (collection classes)

#include "NanoWinMFCAfx.h"
#include "NanoWinMFCAfxStr.h"

// Classes
// ------------------------------------------

// abstract iteration position (for CList-like collections)
struct __POSITION {};
typedef __POSITION* POSITION;

template <class TYPE, class ARG_TYPE = const TYPE&>
class CArray : public CObject
{
  private:
  TYPE *data;

  protected:
  int m_nSize; // Invisble for children some reason?

  public:
  TYPE& GetAt(INT_PTR nIndex);
  const TYPE& GetAt(INT_PTR nIndex) const;

  INT_PTR GetSize() const;
  INT_PTR GetCount() const;

  void SetSize(INT_PTR nNewSize,INT_PTR nGrowBy = -1);

  INT_PTR Add(ARG_TYPE newElement);

  void Copy(const CArray& src);
  void RemoveAll();

  const TYPE& operator[](INT_PTR nIndex) const;
  TYPE& operator[](INT_PTR nIndex);

  // Operations that move elements around
  // void InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount = 1);
  void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1);
  // void InsertAt(INT_PTR nStartIndex, CArray* pNewArray);

  TYPE& ElementAt(INT_PTR nIndex);
};

template<class TYPE, class ARG_TYPE = const TYPE&>
class CList : public CObject
{
  public:

  // Gets the number of elements in this list.
  INT_PTR GetCount() const;
  // Searches the list sequentially to find the first element matching the specified searchValue.
  POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;

  TYPE RemoveHead();
  TYPE RemoveTail();

  // add before head or after tail
  POSITION AddHead(ARG_TYPE newElement);
  POSITION AddTail(ARG_TYPE newElement);

  POSITION GetHeadPosition() const;

  TYPE& GetNext(POSITION& rPosition);
  const TYPE& GetNext(POSITION& rPosition) const;

  void RemoveAll();
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMap : public CObject
{
  public:

  // CPair
  struct CPair
  {
    const KEY key;
    VALUE value;
    protected:
    CPair(ARG_KEY keyval) : key(keyval) {}
  };
};

class CStringArray : public CObject
{
  public:

  INT_PTR GetSize() const;
  INT_PTR GetCount() const;

  void RemoveAll();
  void Add(const CString &item);

  const CString& GetAt(INT_PTR nIndex) const;

  INT_PTR Append(const CStringArray& src);

  const CString& operator[](INT_PTR nIndex) const;
  // CString& operator[](INT_PTR nIndex);
};

class CMapStringToPtr : public CObject
{
  public:
  void RemoveAll();
  INT_PTR GetCount() const;
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, void*& rValue) const;

  // Lookup
  BOOL Lookup(LPCTSTR key, void*& rValue) const;
  BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;

  // add a new (key, value) pair
  void SetAt(LPCTSTR key, void* newValue);
};

// Functions
// ------------------------------------------

#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

#endif // linux
#endif // ...Included
