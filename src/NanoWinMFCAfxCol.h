// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCStr functions

#if !defined(NanoWinMFCAfxColIncluded)
#define NanoWinMFCAfxColIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <vector>
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
  #define REQUIRE(f) if (!(f)) { throw "CArray has invalid param:" #f; }

  private:
//  TYPE *data;

  typedef std::vector<TYPE> storage_type;

  storage_type storage;

  protected:
  int m_nSize; // Invisble for children some reason?

  public:

  TYPE& GetAt(INT_PTR nIndex)
  {
    REQUIRE(nIndex >= 0);
    REQUIRE(nIndex < GetSize());

    return storage[nIndex];
  }

  const TYPE& GetAt(INT_PTR nIndex) const
  {
    REQUIRE(nIndex >= 0);
    REQUIRE(nIndex < GetSize());

    return storage[nIndex];
  }

  INT_PTR GetSize() const
  {
    return storage.size();
  }

  INT_PTR GetCount() const
  {
    return GetSize();
  }

  void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1)
  {
    REQUIRE(nNewSize >= 0);

    storage.resize(nNewSize);
  }

  INT_PTR Add(ARG_TYPE newElement)
  {
    storage.push_back(newElement);

    return storage.size() - 1;
  }

  void Copy(const CArray& src)
  {
    if (&src == this) return;

    typename storage_type::size_type srcSize = src.storage.size();
    typename storage_type::size_type dstSize = storage.size();
    typename storage_type::size_type minSize = srcSize < dstSize ? srcSize : dstSize;

    for (typename storage_type::size_type i = 0; i < minSize; i++)
    {
      storage[i] = src.storage[i];
    }

    if      (dstSize > srcSize)
    {
      storage.resize(srcSize);
    }
    else if (dstSize < srcSize)
    {
      for (typename storage_type::size_type i = dstSize; i < srcSize; i++)
      {
        storage.push_back(src.storage[i]);
      }
    }
  }

  void RemoveAll()
  {
    storage.clear();
  }

  const TYPE& operator[](INT_PTR nIndex) const
  {
    return GetAt(nIndex);
  }

  TYPE& operator[](INT_PTR nIndex)
  {
    return GetAt(nIndex);
  }

  // Operations that move elements around
  // void InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount = 1);
  void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1)
  {
    typename storage_type::size_type size = storage.size();

    REQUIRE(nIndex >= 0);
    REQUIRE(nIndex < size);

    typename storage_type::size_type afterLastIndex = nIndex + nCount;

    if (afterLastIndex > size)
    {
      //TODO: assert in debug mode

      afterLastIndex = size;
    }

    storage.erase(nIndex,afterLastIndex);
  }
  // void InsertAt(INT_PTR nStartIndex, CArray* pNewArray);

  TYPE& ElementAt(INT_PTR nIndex)
  {
    return GetAt(nIndex);
  }
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
