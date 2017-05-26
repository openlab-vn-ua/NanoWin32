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

  #undef REQUIRE
};

template<class TYPE, class ARG_TYPE = const TYPE&>
class CList : public CObject
{
  #define REQUIRE(f) if (!(f)) { throw "CList has invalid param:" #f; }
  #define NODEPTR_TO_POSITION(n) ((POSITION)(void*)(n))
  #define POSITION_TO_NODEPTR(p) ((Node*)(void*)(p))

  public:

  CList()
  {
    head  = NULL;
    tail  = NULL;
    count = 0;
  }

  ~CList ()
  {
    RemoveAll();
  }

  // Gets the number of elements in this list.
  INT_PTR GetCount() const
  {
    return count;
  }

  BOOL IsEmpty() const
  {
    return count == 0;
  }

  // Searches the list sequentially to find the first element matching the specified searchValue.
  POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const
  {
    Node *node = startAfter == NULL ? head : POSITION_TO_NODEPTR(startAfter);
    bool found = false;

    while (!found && node != NULL)
    {
      found = node->value == searchValue;

      if (!found)
      {
        node = node->next;
      }
    }

    return found ? NODEPTR_TO_POSITION(node) : NULL;
  }

  TYPE RemoveHead()
  {
    REQUIRE(!IsEmpty());

    Node *node = head;

    if (tail == head) // single element in list
    {
      head = NULL;
      tail = NULL;
      count = 0;
    }
    else
    {
      head = node->next;
      head->prev = NULL;
      count--;
    }

    TYPE result = node->value;

    delete node;

    return result;
  }

  TYPE RemoveTail()
  {
    REQUIRE(!IsEmpty());

    Node *node = head;

    if (tail == head) // single element in list
    {
      head = NULL;
      tail = NULL;
      count = 0;
    }
    else
    {
      tail = node->prev;
      tail->next = NULL;
      count--;
    }

    TYPE result = node->value;

    delete node;

    return result;
  }

  // add before head or after tail
  POSITION AddHead(ARG_TYPE newElement)
  {
    Node *node = new Node(newElement);

    node->prev = NULL;
    node->next = head;

    if (node->next != NULL)
    {
      node->next->prev = node;
    }
    else
    {
      tail = node;
    }

    head = node;

    count++;

    return NODEPTR_TO_POSITION(node);
  }

  POSITION AddTail(ARG_TYPE newElement)
  {
    Node *node = new Node(newElement);

    node->prev = tail;
    node->next = NULL;

    if (node->prev != NULL)
    {
      node->prev->next = node;
    }
    else
    {
      head = node;
    }

    tail = node;

    count++;

    return NODEPTR_TO_POSITION(node);
  }

  POSITION GetHeadPosition() const
  {
    return NODEPTR_TO_POSITION(head);
  }

  TYPE& GetNext(POSITION& rPosition)
  {
    REQUIRE(rPosition != NULL);

    Node *node = POSITION_TO_NODEPTR(rPosition);

    rPosition = NODEPTR_TO_POSITION(node->next);

    return node->value;
  }

  const TYPE& GetNext(POSITION& rPosition) const
  {
    REQUIRE(rPosition != NULL);

    Node *node = POSITION_TO_NODEPTR(rPosition);

    rPosition = NODEPTR_TO_POSITION(node->next);

    return node->value;
   }

  void RemoveAll()
  {
    Node *node = head;

    head  = NULL;
    tail  = NULL;
    count = 0;

    while (node != NULL)
    {
      Node *next = node->next;

      delete node;

      node = next;
    }
  }

  private :

  struct Node
  {
    Node (TYPE src, Node *prev = NULL, Node *next = NULL) { value = src; this->prev = prev; this->next = next; }

    TYPE  value;
    Node *prev;
    Node *next;
  };

  Node   *head;
  Node   *tail;
  size_t  count;

  #undef NODEPTR_TO_POSITION
  #undef POSITION_TO_NODEPTR
  #undef REQUIRE
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
