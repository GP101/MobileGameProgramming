//////////////////////////////////////////////////////////////////
// File: CAList.h
// Desc: advanced list by seojt@kogsoft.com
//       CAdvancedList template class using BMT(bucket mapping table)
//       for incredible speed, O(1) to access a node.
//       programmed on 5/25/2000
//       last modified on 4 May, 2001
//          - added usage example comments
//

#ifndef _CADVANCEDLIST_DEFINED_
#define _CADVANCEDLIST_DEFINED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>

//////////////////////////////////////////////////////////////////
// struct AdvancedListBucket
// one BUCKET of CAdvancedList
template <class T,int size=200> 
struct AdvancedListBucket {
// T   : node data type
// size: bucket size. A bucket has 100 nodes by default
    T                   data[size]; //'size' is template parameter
    AdvancedListBucket* next;       //singly linked list 'next' pointer
    T& operator[](int i) { return data[i]; }
};//struct AdvancedListBucket

//////////////////////////////////////////////////////////////////
// class CAdvancedList
template<class T,int _sizeBucket=200> 
class CAdvancedList {
// T          : node data type
// _sizeBucket: default bucket size(=100)
private:
    const int                           sizeBucket; //a bucket size(=100) which is equal to '_sizeBucket'
    AdvancedListBucket<T,_sizeBucket>*  head;       //head bucket pointer
    AdvancedListBucket<T,_sizeBucket>*  tail;       //tail bucket pointer
    int                                 nItemCount;   //number of total items in list
    int                                 nBucketCount; //number of buckets in list 
    int                                 fPointerType; //1 when 'T' is pointer type
    AdvancedListBucket<T,_sizeBucket>*  BMT[128];   //Bucket Mapping Table
      //128 used for fast multiplication 2^8
    int                                 nItemPointer; //points an insertion point of a last bucket 

    //belows are used for First(), Next() functions
    AdvancedListBucket<T,_sizeBucket>*  current; //current bucket pointer
    int                                 nCurrentItemPointer; //points an insertion point of a current bucket. used with 'current'
    int                                 nCurrentBucketCount; //counts current working buckets
public:
    //enum {SIMPLE=0,POINTER=1,POINTER_A=2};

private:
    void _Init(); //initialize member variables
    AdvancedListBucket<T,_sizeBucket>* _InsertBucket();      //insert a bucket
    int  _DeleteBucket(AdvancedListBucket<T,_sizeBucket>* p); //delete a bucket
    void _FreeBucket(AdvancedListBucket<T,_sizeBucket>* p);  //delete contents of a bucket

public:
    CAdvancedList(int fPointerNode=0); //you must set fPointerNode to 1, when node's data is pointer
    ~CAdvancedList() { FreeAll(); }
    int Insert(T d);     //insert an item, buckets are maintained automatically
    //int Delete(T d);    //delete an item, buckets are maintained automatically
    //int DeleteAt(int i);//delete an i-th item
    int GetItemCount() { return nItemCount; }
    int GetSize() { return nItemCount; }
    int GetBucketCount() { return nBucketCount; }
    int GetBucketSize() { return sizeBucket; }
    int First(T& d);
    int First(T* pNode);
    int Next(T& d);
    int Next(T* pNode);
    T&  At(int i);
    T&  operator[](int i);
    void FreeAll();
#ifdef _DEBUG_
    void Debug();
#endif
};//CAdvancedList

template<class T,int _sizeBucket> 
void CAdvancedList<T,_sizeBucket>::_Init() { 
// initialize all pointers to NULL
// initialize all variables
    head                = tail = NULL; 
    nItemCount          = nBucketCount = 0; 
    //sizeBucket=_sizeBucket;    //set bucket size
    memset(BMT, 0, sizeof(BMT)); //initialize BMT(bucket mapping table)
    nItemPointer        = 0;
    current             = NULL;
    nCurrentItemPointer = 0;
    nCurrentBucketCount = 0;
}//CAdvancedList::Init()

template<class T,int _sizeBucket> 
AdvancedListBucket<T,_sizeBucket>* CAdvancedList<T,_sizeBucket>::_InsertBucket() {
// return pointer to the added bucket
// new bucket is added to 'tail' for preserve item sequence
    AdvancedListBucket<T,_sizeBucket>* t;

    t = new AdvancedListBucket<T,_sizeBucket>;
    memset(t, 0, sizeof(AdvancedListBucket<T,_sizeBucket>)); //initialize bucket
    nItemPointer = 0;//initialize item pointer
    
    if (head == NULL) {//when there is empty bucket
        head        = tail = t;
        head->next  = NULL;
    } else { //normal append at 'tail'
        tail->next  = t;
        t->next     = NULL;
        tail        = t;//'tail' must indicate last bucket
    }//if.. else..

    if (nBucketCount < 128) //maintain BMT if size is enough for fitting
        BMT[nBucketCount] = t;
    ++nBucketCount;

    return tail;
}//CAdvancedList::InsertBucket()

template<class T,int _sizeBucket> 
int CAdvancedList<T,_sizeBucket>::_DeleteBucket(AdvancedListBucket<T,_sizeBucket>* pDelete) {
// return 1 when deleted
// In our linked list, there are almost no deletion. 
// If you are developing a program which uses many deletion, 
//  you must use doubly linked list. You can do it! Modify this 'CAdvancedList'.
    AdvancedListBucket<T,_sizeBucket>* p     = head;
    AdvancedListBucket<T,_sizeBucket>* pPrev = NULL;
    int count = 0;

    //find 'pDelete' because we need previous pointer 'pPrev'
    while (p != pDelete) {
        pPrev   = p;   // conserve previous pointer
        p       = p->next;
        ++count;
    }//while

    if (p == pDelete) {
        if (pPrev != NULL) //when there is previous bucket node
            pPrev->next = p->next;
        if (pPrev == NULL) //when pDelete points 'head'
            head        = p->next;
        if (p->next == NULL) //when pDelete points 'tail'
            tail        = pPrev;
        if (fPointerType) 
            _FreeBucket(p);
        if ((nBucketCount-count-1) > 0 && nBucketCount <= 128) {
            memmove(&BMT[count], &BMT[count+1], sizeof(BMT[0])*(nBucketCount-count-1));
        }//if

        if (nBucketCount == 128)//fix when last bucket is deleted
            BMT[127] = BMT[127]->next;
        delete p;

        nItemPointer = sizeBucket-1;//set item pointer
        --nBucketCount;
        return 1;
    }//if

    return 0;
}//CAdvancedList::DeleteBucket()

template<class T,int _sizeBucket> 
void CAdvancedList<T,_sizeBucket>::_FreeBucket(AdvancedListBucket<T,_sizeBucket>* p) {
    //int i;

    if (fPointerType==0) return;
    //nItemPointer=0;//set item pointer
    /*
    if (fPointerType==1) {
        for (i=0;i<sizeBucket;++i) {
            if ((*p)[i] != NULL) {
                delete (*p)[i];
                (*p)[i]=NULL; //for safety
            }//if
        }//for
    } else if (fPointerType==2) {
        for (i=0;i<sizeBucket;++i) {
            if ((*p)[i] != NULL) {
                delete[] (*p)[i];
                (*p)[i]=NULL; //for safety
            }//if
        }//for
    }//if.. else if..
    */
}//CAdvancedList::_FreeBucket()

template<class T,int _sizeBucket> 
CAdvancedList<T,_sizeBucket>::CAdvancedList(int fPointerNode) : sizeBucket(_sizeBucket) {
    _Init();
    fPointerType = fPointerNode;//set 1 when type 'T' is pointer
}//CAdvancedList::CList

template<class T,int _sizeBucket> 
int CAdvancedList<T,_sizeBucket>::Insert(T d) {
    if (head==NULL || nItemPointer>=sizeBucket) 
        _InsertBucket();

    (*tail)[nItemPointer]   = d;
    ++nItemCount;
    ++nItemPointer;

    return nItemCount - 1;
}//CAdvancedList::Insert()

/*
template<class T,int _sizeBucket> 
int CAdvancedList<T,_sizeBucket>::Delete(T d) {
    d;
    return 0;
}//CAdvancedList::Delete()

template<class T,int _sizeBucket> 
int CAdvancedList<T,_sizeBucket>::DeleteAt(int i) {
    d;
    return 0;
}//CAdvancedList::DeleteAt()
*/

template<class T,int _sizeBucket> 
int CAdvancedList<T,_sizeBucket>::First(T& d) {
    if (head == NULL)
        return 0;

    current             = head;
    nCurrentItemPointer = 0;
    nCurrentBucketCount = 1;
    d                   = (*current)[ nCurrentItemPointer ];
    ++nCurrentItemPointer;

    return 1;
}//CAdvancedList::First()

template<class T,int _sizeBucket> 
inline int CAdvancedList<T,_sizeBucket>::First(T* pNode) {
    return First(*pNode);
}//CAdvancedList::First()

template<class T,int _sizeBucket> 
int CAdvancedList<T,_sizeBucket>::Next(T& d) {
    if (nCurrentBucketCount >= nBucketCount && nCurrentItemPointer >= nItemPointer)
        return 0;
    if (nCurrentItemPointer >= sizeBucket) {
        current             = current->next;
        nCurrentItemPointer = 0;
        ++nCurrentBucketCount;
    }//if
    if (current == NULL)
        return 0;

    d = (*current)[ nCurrentItemPointer ];
    ++nCurrentItemPointer;

    return 1;
}//CAdvancedList::Next()

template<class T,int _sizeBucket> 
inline int CAdvancedList<T,_sizeBucket>::Next(T* pNode) {
    return Next(*pNode);
}//CAdvancedList::First()

template<class T,int _sizeBucket> 
inline T& CAdvancedList<T,_sizeBucket>::At(int i) {
    return operator[](i);
}//CAdvancedList::At()

template<class T,int _sizeBucket> 
T& CAdvancedList<T,_sizeBucket>::operator[](int i) {
    AdvancedListBucket<T,_sizeBucket>* p;
    int iBMT,dBMT;
    int t = sizeBucket << 7; //means sizeBucket*128

    iBMT = i / sizeBucket;
    dBMT = i % sizeBucket;
    if (i < t) {
        p = BMT[iBMT];
        return (*p)[dBMT];
    } else {
        int d           = i - t;
        int countLink   = d / sizeBucket + 1;
        int j;

        p = BMT[127];
        for (j=0; j<countLink; ++j)
            p = p->next;
        return (*p)[ d%sizeBucket ];
    }//if.. else..
}//CAdvancedList::operator[]()

template<class T,int _sizeBucket> 
void CAdvancedList<T,_sizeBucket>::FreeAll() {
    AdvancedListBucket<T,_sizeBucket>* t=head;
    AdvancedListBucket<T,_sizeBucket>* prev;

    while (t != NULL) {
        prev = t;
        t = t->next;
        if (fPointerType) 
            _FreeBucket(prev);
        delete prev;
    }//while
    _Init();
}//CAdvancedList::FreeAll()

#ifdef _DEBUG_
template<class T,int _sizeBucket> 
void CAdvancedList<T,_sizeBucket>::Debug() {
    printf("%d\n",sizeof(BMT));
    printf("%d\n",sizeof(AdvancedListBucket<T,_sizeBucket>));
    printf("%d\n",sizeof(BMT[0]));
}//CAdvancedList::Debug()
#endif // #ifdef _DEBUG_

#endif


//////////////////////////////////////////////////////////////////
// usage example of CAdvancedList

/*
    void main() {
        int* p;
        CAdvancedList<int*> plist;

        for (i=0;i<20000;++i) {
            p=new int;
            *p=i;
            plist.Insert(p);
        }//for
        for (i=0;i<plist.GetSize();++i)
            printf("%d\n",*plist[i]);
        for (i=0;i<plist.GetSize();++i)
            delete plist[i];
    }//main()


  one more example:

    #include <stdio.h>
    #include <conio.h>
    #include "CAList.h" 

    //#define max(a,b)            ((a)>(b)?(a):(b))
    //#define min(a,b)            ((a)<(b)?(a):(b)) 

    void main() {
        CAdvancedList<int> list; 
        int i,b,c; 

        for (i=0;i<20000;++i)
            list.Insert(i); 
        
        //first usage
        b=list.First(&c);
        
        while (b) 
        {
            printf("%05d\n",c);
            b=list.Next(&c);
        }//while
        getch(); 
        
        //second usage
        for (i=0;i<list.GetSize();++i)
            printf("%05d\n",list[i]);
        getch(); 
        
        //when node type is pointer
        int* p;
        CAdvancedList<int*> plist; 
        
        for (i=0;i<20000;++i) 
        {
            p=new int;
            *p=i;
            plist.Insert(p);
        }//for
        
        for (i=0;i<plist.GetSize();++i)
            printf("%d\n",*plist[i]);
        
        // clean up
        for (i=0;i<plist.GetSize();++i)
            delete plist[i];
    }//main() 

*/