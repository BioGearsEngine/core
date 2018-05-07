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

#ifndef __XPARRAY_H__
#define __XPARRAY_H__


template <typename TYPE>
class CXpArray
{
    protected:

    typedef void (* PFN_MoveCallback)(TYPE* pObject, const INT_PTR nCount);

    static void*        m_pfnMoveCallback;
    TYPE*				m_pData;   
    INT_PTR				m_nElementCount;
    INT_PTR				m_nAllocedCount;
    INT_PTR				m_nGrowBy; 

    public:

    inline CXpArray(const INT_PTR nGrowBy = 10);
    inline CXpArray(const CXpArray& Array);
    virtual ~CXpArray();

    // The following callback can be used to track when an object
    // moves in memory due to a reallocation.  Note that this is called
    // after the object has moved.
    static void SetMoveCallback(void (* pfnMoveCallback)(TYPE* pObject, const INT_PTR nCount));

    inline HRESULT Add(const TYPE& Element, INT_PTR* pNewIndex = NULL);
    inline HRESULT Insert(const INT_PTR nIndex, const TYPE& Element);

    inline HRESULT Remove(const INT_PTR nIndex, const INT_PTR nCount = 1);
    inline void RemoveAll();

    inline INT_PTR GetCount() const;
    inline HRESULT SetCount(const INT_PTR nNewCount);

    inline INT_PTR GetGrowBy() const;
    inline void SetGrowBy(const INT_PTR nGrowBy);

    inline HRESULT PreAlloc(const INT_PTR nCount);
    inline HRESULT FreeExtra();

    inline HRESULT Append(const CXpArray& Array);

    // Similar to basic string functions.  Out
    // of range values are fine.
    HRESULT Mid(CXpArray& Subset, const INT_PTR nFrom, const INT_PTR nCount = -1) const;
    HRESULT Left(CXpArray& Subset, const INT_PTR nCount) const;
    HRESULT Right(CXpArray& Subset, const INT_PTR nCount) const;

    INT_PTR Find(const TYPE& Element,
    const INT_PTR nStart = 0) const;

    INT_PTR Find(const CXpArray& Subset,
    const INT_PTR nStart = 0) const;

    // Use these methods with care, they can return NULL!  Also
    // this is temporary and can change during reallocation.
    TYPE* GetData();
    const TYPE* GetData() const;

    inline TYPE& operator[](const INT_PTR nIndex);
    inline const TYPE& operator[](const INT_PTR nIndex) const;

    inline bool operator==(const CXpArray& Array) const;
    inline bool operator!=(const CXpArray& Array) const
    {
        return(!operator==(Array));
    }
    
    inline HRESULT Copy(const CXpArray& Array);
    inline CXpArray& operator=(const CXpArray& Array) {Copy(Array); return(*this);}
    protected:
    HRESULT Allocate(const INT_PTR nCount, const bool bInitialize = true);

    inline void ConstructElement(const INT_PTR nIndex, const INT_PTR nCount = 1);
    inline void DestructElement(const INT_PTR nIndex, const INT_PTR nCount = 1);
};

template <typename TYPE>
void XpFreePointerArray(CXpArray<TYPE>& Array)
{
    const INT_PTR nCount = Array.GetCount();

    for(INT_PTR i = 0; i < nCount; i ++)
    {
        delete Array[i];
    }

    Array.RemoveAll();  
}
				 
#include <xparray.inl>

#endif //__XP_ARRAY_H__
