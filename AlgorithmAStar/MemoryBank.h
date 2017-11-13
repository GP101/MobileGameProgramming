//////////////////////////////////////////////////////////////////////
// File: MemoryBank.h
// Desc: interface for the CMemoryBank class.
//       programmed by seojt@kogsoft.com
//       started on October 9, 2001
//

#if !defined(_CMemoryBank_Defined_)
#define _CMemoryBank_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "kogGen.h"
#include "CAList.h"


//-----------------------------------------------------------------
// Name: class CMemoryBank
// Desc: pre allocate memory blocks
//
// Note: BLOCK must be a struct or class, and have a default constructor
//       if BLOCK is built-in type like 'int', 
//       instantiation will be failed.
template<class BLOCK, int _sizeBucket=200>
class CMemoryBank : public CAdvancedList<BLOCK,_sizeBucket>
{
public:
    int m_size;  // number of allocated BLOCKs
    int m_index; // current BLOCK index

public:
    CMemoryBank(int sizeBucketReserved=0) { Reserve( sizeBucketReserved ); }
    ~CMemoryBank() { FreeAll(); }

    // reserve memory for speed up
    // sizeBucketReserved: reserved bucket(memoty block) count
    // Actual amount of memory is 'sizeBucketReserved*sizeof(BLOCK)'
    void Reserve(int sizeBucketReserved);

    // get a free BLOCK pointer
    BLOCK* Alloc();

    // clear all memory
    void Reset() { m_index = 0; }
};//class CMemoryBank



template<class BLOCK, int _sizeBucket>
void CMemoryBank<BLOCK,_sizeBucket>::Reserve(int sizeBucketReserved)
{
    if (sizeBucketReserved <= 0)
        return;

    // delete previous BLOCKs if exist
    FreeAll();

    // initialize BLOCK index
    m_index = 0;
    m_size  = sizeBucketReserved;

    // pre push empty buckets
    for (register int i=0; i<m_size; i++)
    {
        Insert( BLOCK() );
          //    ^call default constructor
    }//for
}//CMemoryBank<BLOCK>::Reserve(int sizeBucketReserved)


template<class BLOCK, int _sizeBucket>
BLOCK* CMemoryBank<BLOCK,_sizeBucket>::Alloc()
{
    if ( m_size == m_index )
    {
        BLOCK* p;

        Insert( BLOCK() );
        p = &At(m_index);
        ++m_size;
        ++m_index;

#ifdef _DEBUG
        OutputDebugStringA( " [WARNING] CMemoryBank exceeds reserved bank size\n" );
#endif // _DEBUG

        return p;
    }
    else
    {
        BLOCK* p;

        p = &At(m_index);
        ++m_index;

        return p;
    }//if.. else..
}//CMemoryBank<BLOCK>::Alloc(int countBucket)


#endif // !defined(_CMemoryBank_Defined_)


/* working example

    #include <stdio.h>
    #include "memoryBank.h"

    CMemoryBank<D3DXVECTOR3> g_vBank(3);

    void main()
    {
        D3DXVECTOR3* p;
        D3DXVECTOR3* q[10];

        p = g_vBank.Alloc();
        *p = D3DXVECTOR3(1,2,3);

        printf("%g,%g,%g\n", p->x, p->y, p->z );

        for (int i=0; i<10; ++i)
        {
            q[i] = g_vBank.Alloc();
            *q[i] = D3DXVECTOR3(i,i,i);
            printf("%g,%g,%g\n", q[i]->x, q[i]->y, q[i]->z );
        }//for

        g_vBank.FreeAll();
    }//main()


*/