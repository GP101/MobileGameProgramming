//////////////////////////////////////////////////////////////////////
// File: HashTable.h
// Desc: interface for the CHashTable class.
//       programmed by seojt@kogsoft.com
//       started on October 4, 2001
//

#if !defined(_CHashTable_Defined_)
#define _CHashTable_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#define _LOGENABLE  // enable log.txt file write


#include <vector>
#include "kogGen.h"
#include <windows.h>
#ifdef _LOGENABLE
#include "writeLog.h"
#endif // _LOGENABLE
template<class KEY>
struct _hash_default_functor
{
    int Hash(int slotSize, KEY& key) const
    {
        return key % slotSize;
    }//Hash()

    int KeyCompare(const KEY& first, const KEY& second) const
    {
        if (first == second)
            return 0;

        return 1;
    }//KeyCompare()
};//struct _hash_default_functor


//----------------------------------------------------------------
// Name: class CHashTable
// Desc: hash table using chaining
// Param:
//      KEY: key data type
//      COMPARE: hash and compare functor
//
//          if KEY is 'CHashKey*' then 
//          you must define COMPARE like below:
//
//        class CHashFunctor
//        {
//        public:
//            // hash function
//            int Hash(int slotSize, CHashKey*& key)
//            {
//                return key->i % slotSize;
//            }
//
//            // compare function
//            // compare function must return 0 if first == second
//            int KeyCompare(CHashKey*& first, CHashKey*& second)
//            {
//                if (first->f == second->f)
//                    return 0;
//
//                return 1;
//            }
//        };//class CHashFunctor
//
//
//      SLOTSIZE: slot size
//          use prime number for SLOTSIZE
//          near 50: use   47
//              100:       97
//             1000:      997
//             2000:     1999
//             3000:     2999
//             5000:     4999
//             8000:     7993
//            10000:     9973
//            20000:    19997
//            30000:    29989
//            40000:    39989
//            50000:    49999
//           100000:    99991
//           200000:   199999
//          1000000:   999983
//
// Note: if KEY is class, and it's not safe on assignment operator
//       You must present overloaded operator=() on class KEY
//
//       Compare function(fpCompare) must return 0, 
//          if first==second
//
template<class KEY, class COMPARE=_hash_default_functor<KEY>, int SLOTSIZE = 701>
class CHashTable
{
public:
    typedef typename std::vector<KEY>              SLOT; // slot type
    typedef typename std::vector<KEY>::iterator    ITERATOR; // slot iterator type

    COMPARE functor; // hashing and compare functor

public:
    SLOT        slot[SLOTSIZE];     // slots

public:
    CHashTable(){}
    ~CHashTable(){ Clear(); }

    // find '*pOut' in hash table, and return found data to 'pOut'
    // pOut: [in,out] data to search
    // pSlotIndex: slot index of *pOut if exist
    // pOffsetIndex: offset index in slot if exist
    //      if this value is greater than 0, there is collision
    //      We use chaining to resolve collision
    // return TRUE if 'pOut' exist
    // Note: 'pOut' is [in] [out] parameter
    bool Get(KEY* pInOut, int* pSlotIndex=NULL, int* pOffsetIndex=NULL);

    // find 'in' in hash table, and return found data pointer
    // pOut: [out] found data
    // in: [in] data to search
    // return true if found
    bool Get(KEY* pOut, KEY in);

    // store '*pIn' to slot
    // pIn: [in] key data
    // return 1 if store is successful
    // return 0 if can't store
    // return >=2 if store is successful and there is collision
    // i.e. return number of keys in hashed slot
    int  Store(KEY* pIn);

    // store 'in' to slot
    // return 1 if store is successful
    // return 0 if can't store
    // return 2 if store is successful and there is collision
    inline int  Store(KEY in) { return Store(&in); }

    // clear hash table
    void Clear();

    // get i-th slot
    // Note: slot type is 'std::vector<KEY>'
    //       So you can traverse standard STL iterator
    inline SLOT* GetIthSlot(int i) { return &slot[i]; }

#ifdef _LOGENABLE
    // bWriteFile: write log file(log.txt)
    // nItem: number of item in hash table
    // return used slot size
    int DumpSlotSize(bool bWriteFile = false, int* nItem = NULL);
#endif // _LOGENABLE
};//class CHashTable


// find '*pOut' in hash table, and return found data to 'pOut'
// pOut: [in,out] data to search
// pSlotIndex: slot index of *pOut if exist
// pOffsetIndex: offset index in slot if exist
//      if this value is greater than 0, there is collision
//      We use chaining to resolve collision
// return TRUE if 'pOut' exist
// Note: 'pOut' is [in] [out] parameter
template<class KEY, class COMPARE, int SLOTSIZE>
bool CHashTable<KEY,COMPARE,SLOTSIZE>::Get(KEY* pInOut, int* pSlotIndex, int* pOffsetIndex)
{
    // get index of key(*pInOut) using customized hash function
    int     index;
#ifdef _DEBUG
    index = 0;
#endif // _DEBUG
    // call hash function
    index = functor.Hash( SLOTSIZE, *pInOut );
    ASSERT( index < SLOTSIZE );

    // check empty
    if ( slot[index].empty() )
        return false;

    // set [out] param
    if (pSlotIndex)
        *pSlotIndex = index;

    // iterate each keys
    for (unsigned int i = 0; i < slot[index].size(); ++i)
    {
        // call customized compare callback
        if ( 0 == functor.KeyCompare( *pInOut, slot[index][i] ) )
        {
            // set [out] param
            if (pOffsetIndex)
                *pOffsetIndex = i;

            // set [out] param
            *pInOut = slot[index][i]; // call bitwise copy or operator=()

            return true;
        }//if
    }//for

    return false;
}//Get()


// find 'in' in hash table, and return found data pointer
// pOut: [out] found data
// in: [in] data to search
// return true if found
template<class KEY, class COMPARE, int SLOTSIZE>
bool CHashTable<KEY,COMPARE,SLOTSIZE>::Get(KEY* pOut, KEY in)
{
    *pOut = in;
    return Get( pOut, NULL, NULL );
}//Get()


// store '*pIn' to slot
// return 1 if store is successful
// return 0 if can't store
// return >=2 if store is successful and there is collision
template<class KEY, class COMPARE, int SLOTSIZE>
int CHashTable<KEY,COMPARE,SLOTSIZE>::Store(KEY* pIn)
{
    // get index of key(*pOut) using customized hash function
    int     index;
#ifdef _DEBUG
    index = 0;
#endif // _DEBUG
    // call hash function
    index = functor.Hash( SLOTSIZE, *pIn );
    ASSERT( index < SLOTSIZE );

    // store key
    slot[index].push_back( *pIn );

    // return number of keys in this slot
    return slot[index].size();
}//Store()


// clear hash table
template<class KEY, class COMPARE, int SLOTSIZE>
void CHashTable<KEY,COMPARE,SLOTSIZE>::Clear()
{
    for (int i=0; i<SLOTSIZE; ++i)
        slot[i].clear();
}//Clear()


#ifdef _LOGENABLE
template<class KEY, class COMPARE, int SLOTSIZE>
int CHashTable<KEY,COMPARE,SLOTSIZE>::DumpSlotSize(bool bWriteFile, int* nItem)
{
    int count = 0; // valid slot counter
    int item = 0;  // number of key in hash table
    int s;

    for (int i=0; i<SLOTSIZE; ++i)
    {
        s = slot[i].size();
        if (s>0)
            ++count;
        item += s;
        if (bWriteFile)
            Log_WriteVString( "slot %04d=%d\n", i, s );
        else
            DebugOut( "slot %04d=%d\n", i, s );
    }//for

    DebugOut( "slot size=%d\n", SLOTSIZE);

    // set number of keys in hash table
    if (nItem)
        *nItem = item;

    return count;
}//DumpSlotSize()
#endif // _LOGENABLE


#endif // !defined(_CHashTable_Defined_)


/* example of int hash table

#include <d3dx8.h>
#include <stdio.h>
#include "HashTable.h"
#include "writeLog.h"


int HashInt(int slotSize, int& key)
{
    return key % slotSize;
}//HashInt()

int CompareInt(int& first, int& second)
{
    if (first==second)
        return 0;
    else if (first > second)
        return 1;
    else
        return 2;
}//CompareInt()

void main()
{
    CHashTable<int> hash;
    hash.SetHashingCallback( HashInt );
    hash.SetCompareCallback( CompareInt );

    int     test[5]={1,5,3,7,2};
    int     i;

    for (i=0; i<5; ++i)
    {
        hash.Store( &test[i] );
    }//for

    int  key = 3;
    bool br  = false;
    int  slot = 0, offset = 0;

    if ( hash.Get( &key, &slot, &offset ) )
        printf("found, key=%d,slot=%d,offset=%d\n", key, slot, offset );
    else
        printf("not found\n");

    //hash.DumpSlotSize();
}//main() 

*/


/* example of D3DXVECTOR3 hash table

#include <d3dx8.h>
#include <stdio.h>
#include "HashTable.h"
#include "writeLog.h"


// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

int HashVector3( int slotSize, D3DXVECTOR3& key )
{
    DWORD dw;
    DWORD temp;
    DWORD temp2;

    // get bit sequence of x-value
    dw = FtoDW( key.x );

    // cross over z-value
    temp = FtoDW( key.z );
    temp2 = temp & 0xffff0000;
    temp2 >>= 16;
    temp <<= 16;
    temp |= temp2;

    // add x+z
    dw += temp;

    return dw % slotSize;
}//HashVector3()

int CompareVector3( D3DXVECTOR3& first, D3DXVECTOR3& second )
{
    if (first==second)
        return 0;

    return 1;
}//CompareVector3()

void main()
{
    CHashTable<D3DXVECTOR3> hash;
    hash.SetHashingCallback( HashVector3 );
    hash.SetCompareCallback( CompareVector3 );

    D3DXVECTOR3 test[200];
    int     i;

    for (i=0; i<sizeof(test)/sizeof(test[0]); ++i)
    {
        test[i].x = (float)rand()/(float)RAND_MAX*200;
        test[i].y = (float)rand()/(float)RAND_MAX*200;
        test[i].z = (float)rand()/(float)RAND_MAX*200;
        //DebugOut("%g,%g,%g\n", test[i].x, test[i].y, test[i].z );
        hash.Store( &test[i] );
    }//for

    D3DXVECTOR3  key    = D3DXVECTOR3(1,1,1);
    int          slot   = 0;
    int          offset = 0;

    hash.Store( &key );

    if ( hash.Get( &key, &slot, &offset ) )
        printf(" found, key=(%g,%g,%g),slot=%d,offset=%d\n\n", 
            key.x, key.y, key.z, slot, offset );
    else
        printf("not found\n");

    //hash.Clear();

    int m,n;

    // get used slot size
    m = hash.DumpSlotSize(false, &n);
      //                         ^keys in table
    printf(" keys in table=%d\n used slot size=%d/701\n\n", n, m );

}//main() */


/* class example

#include <d3dx8.h>
#include <stdio.h>
#include "HashTable.h"

class CHashKey
{
public:
    int   i;
    float f;

public:
    CHashKey(int _i, float _f) { i = _i; f = _f; }
    void Print() { printf("%d,%f\n", i, f ); }
};//class CHashKey

class CHashFunctor
{
public:
    // hash function
    int Hash(int slotSize, CHashKey*& key)
    {
        return key->i % slotSize;
    }

    // compare function
    // compare function must return 0 if first == second
    int KeyCompare(CHashKey*& first, CHashKey*& second)
    {
        if (first->i == second->i)
            return 0;

        return 1;
    }
};//class CHashFunctor

void main()
{
    CHashTable<CHashKey*, CHashFunctor> hash;

    CHashKey a(1,5.0f);
    CHashKey b(3,2.0f);
    CHashKey c(2,3.0f);

    hash.Store( &a );
    hash.Store( &b );
    hash.Store( &c );

    CHashKey key(3, 100.0f);
    CHashKey* pRet;

    if ( hash.Get( &pRet, &key ) )
        pRet->Print();

    pRet = &key;
    if ( hash.Get( &pRet ) )
        pRet->Print();

    int m,n;

    // get used slot size
    m = hash.DumpSlotSize(false, &n);
      //                         ^keys in table
    printf(" keys in table=%d\n used slot size=%d/701\n\n", n, m );

}//main()

*/