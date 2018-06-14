/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

// This file is part of the ObjectFusion cross platform (XP)
// class library.
// Copyright (C) 1998 ObjectFusion, L.L.C.
// All rights reserved.

#ifndef __XPHASHTABLE_H__
#define __XPHASHTABLE_H__

#include <xparray.h>

typedef void* TXpTableItem;

template <class KEY, class TYPE>
class CXpHashTable {
protected:
  struct SNode {
    SNode(const KEY& Key, const TYPE& Value)
      : m_pNext(NULL)
      , m_Key(Key)
      , m_Value(Value)
    {
    }

    SNode* m_pNext;
    KEY m_Key;
    TYPE m_Value;
  };

  typedef CXpArray<SNode*> CPtrArray;

  CPtrArray m_Buckets;
  INT_PTR m_nCount;

public:
  CXpHashTable(const INT_PTR nHashTableSize, const bool bCalculate = true);
  virtual ~CXpHashTable();

  inline HRESULT Insert(const KEY& Key, const TYPE& Value);

  inline HRESULT Remove(const KEY& Key);
  inline void RemoveAll();

  inline INT_PTR GetCount() const;

  inline bool HasKey(const KEY& Key) const;
  inline TXpTableItem FindKey(const KEY& Key) const;
  inline bool FindValue(const KEY& Key, TYPE& Value) const;

  inline const KEY& GetKey(const TXpTableItem Item) const;

  inline const TYPE& GetValue(const TXpTableItem Item) const;
  inline TYPE& GetValue(const TXpTableItem Item);

  inline void GetPair(const TXpTableItem Item, KEY& Key, TYPE& Value) const;

  inline TXpTableItem GetFirst() const;
  inline TXpTableItem GetNext(const TXpTableItem Item) const;

  inline HRESULT Copy(const CXpHashTable& HashTable);

  // Result always gets the value from this table, not tHashTable.
  inline HRESULT Intersection(const CXpHashTable& HashTable, CXpHashTable& Result);
  inline HRESULT Union(const CXpHashTable& HashTable, CXpHashTable& Result);
  // Result contains elements from this table not in HashTable.
  inline HRESULT Difference(const CXpHashTable& HashTable, CXpHashTable& Result);

  inline bool operator==(const CXpHashTable& HashTable) const;

protected:
  // Not implemented, not copyable
  CXpHashTable& operator=(const CXpHashTable& HashTable);

  inline INT_PTR GetBucket(const KEY& Key) const;

  bool IsPrime(const INT_PTR nNbr);
  INT_PTR GetClosestPrime(const INT_PTR nNbr);
};

template <class KEY>
inline unsigned long XpHashKey(const KEY& Key)
{
  // This works for most integral types (INT_PTR, char, float, etc).
  return (unsigned long)Key >> 4;
}

inline unsigned long XpHashKey(const char* pszString);
inline unsigned long XpHashKey(const wchar_t* pszString);

#ifdef __AFX_H__
inline unsigned long XpHashKey(const CString& csString);
#endif // __AFX_H__

#include <xphashtable.inl>

#endif //__XPHASHTABLE_H__