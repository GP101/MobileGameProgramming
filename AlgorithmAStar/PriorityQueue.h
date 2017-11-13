//////////////////////////////////////////////////////////////////////
// File: PriorityQueue.h
// Desc: interface for the CPriorityQueue class.
//       programmed by seojt@kogsoft.com
//       started on October 4, 2001
//

#if !defined(_CPriorityQueue_Defined_)
#define _CPriorityQueue_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <algorithm>
#include "kogGen.h"


template<typename DATA>
struct _queue_default_functor
{
    // heap sorting compare callbak
    // Note: if this function returns 1 when a < b
    //       heap is sorted decending order
    // ascending means there is smallest value on top of queue
    bool operator()(const DATA& a, const DATA& b) const 
    {
        return a > b;
        //       ^ ascending order sorting
    }

    // node compare callback for 'UpdateNode()'
    // first, second: first and second data to compare
    // return 0 if first == second
    // Note: this function must return 0 if two values are equal
    //       used to compare two data types to rearrange heap
    int DataCompare( const DATA& a, const DATA& b )
    {
        if (a == b)
            return 0;

        return 1;
    }//DataCompare()
};//struct _queue_default_functor


//-----------------------------------------------------------------
// Name: class CPriorityQueue
// Desc: priority queue using STL
// Param:
//      DATA: data type
//      GREATER_FUNCTOR: data compare functor
//
//      If data type is 'CValue*' then
//      GREATER_FUNCTOR must be defined like below;
//      (STL uses temporary CValueCompare object to compare)
//
//        class CValueCompare
//        {
//        public:
//
//            // heap sorting compare callbak
//            // Note: if this function returns 1 when a < b
//            //       heap is sorted decending order
//            // ascending means there is smallest value on top of queue
//            bool operator()(CValue* a, CValue* b) const 
//            {
//                return a->x > b->x;
//                //          ^ ascending order sorting
//            }
//
//            // node compare callback for 'UpdateNode()'
//            // first, second: first and second data to compare
//            // return 0 if first == second
//            // Note: this function must return 0 if two values are equal
//            //       used to compare two data types to rearrange heap
//            int DataCompare(CValue*& a, CValue*& b)
//            {
//                if (a->y == b->y)
//                    return 0;
//
//                return 1;
//            }//DataCompare()
//        };//class CValueCompare
//
//      And instantiate queue object like this;
//
//    CPriorityQueue<CValue*,CValueCompare> queue;
//
template<typename DATA, typename GREATER_FUNCTOR = _queue_default_functor<DATA> >
class CPriorityQueue
{
public:
    // members for external iteration
    typedef typename std::vector<DATA>::iterator iterator;
    iterator Begin() { return heap.begin(); }
    iterator End()   { return heap.end(); }

    // heap sort and node compare functor
    GREATER_FUNCTOR     functor;

public:
    //Heap implementation using an STL vector
    //Note: the vector is an STL container, but the
    //operations done on the container cause it to
    //be a priority queue organized as a heap
    std::vector<DATA>   heap;
    
public:
    CPriorityQueue(){}
    ~CPriorityQueue(){ heap.clear(); }

    // pop from queue
    DATA Pop();

    // pop from queue
    // *pOut: [out] popped node
    // Note!!! this function isn't tested
    DATA* Pop(DATA* pOut);

    // push to queue
    void Push( DATA& node );
    void Push( DATA* pNode ) { Push(*pNode); }

    // rearrange heap from modified node
    // if 'node' is modified from outside, you must call this to rearrange heap
    // Note: 'node' must not be a top node
    void UpdateNode( DATA& node );

    // check queue is empty
    bool IsEmpty();

    // get top node from queue
    inline DATA Top() { return heap.front(); }
    inline void GetTop(DATA* pOut) { *pOut = heap.front(); }

    void Clear() { heap.clear(); }
};//class CPriorityQueue


template<typename DATA, typename GREATER_FUNCTOR>
DATA CPriorityQueue<DATA,GREATER_FUNCTOR>::Pop()
{  //Total time = O(log n)
    
    //Get the node at the front - it has the lowest total cost
    DATA node = heap.front();
    
    //pop_heap will move the node at the front to the position N
    //and then sort the heap to make positions 1 through N-1 correct
    //(STL makes no assumptions about your data and doesn't want to change
    //the size of the container.)
    std::pop_heap( heap.begin(), heap.end(), functor );
    
    
    //pop_back() will actually remove the last element from the heap
    //(now the heap is sorted for positions 1 through N)
    heap.pop_back();
    
    return( node );
}//CPriorityQueue<DATA,GREATER_FUNCTOR>::Pop()


template<typename DATA, typename GREATER_FUNCTOR>
DATA* CPriorityQueue<DATA,GREATER_FUNCTOR>::Pop(DATA* pOut)
{  //Total time = O(log n)
    
    //Get the node at the front - it has the lowest total cost
    // set [out] parameter
    *pOut = heap.front();
    
    //pop_heap will move the node at the front to the position N
    //and then sort the heap to make positions 1 through N-1 correct
    //(STL makes no assumptions about your data and doesn't want to change
    //the size of the container.)
    std::pop_heap( heap.begin(), heap.end(), functor );
    
    
    //pop_back() will actually remove the last element from the heap
    //(now the heap is sorted for positions 1 through N)
    heap.pop_back();
    
    return pOut;
}//CPriorityQueue<DATA,GREATER_FUNCTOR>::Pop()


template<typename DATA, typename GREATER_FUNCTOR>
void CPriorityQueue<DATA,GREATER_FUNCTOR>::Push( DATA& node )
{  //Total time = O(log n)
    
    //Pushes the node onto the back of the vector (the heap is now unsorted)
    heap.push_back( node );
    
    //Sorts the new element into the heap
    std::push_heap( heap.begin(), heap.end(), functor );
}//CPriorityQueue<DATA,GREATER_FUNCTOR>::Push()


template<typename DATA, typename GREATER_FUNCTOR>
void CPriorityQueue<DATA,GREATER_FUNCTOR>::UpdateNode( DATA& node )
{  //Total time = O(n+log n)
    
#ifdef _DEBUG
    int counter = 0;
#endif // _DEBUG
    //Loop through the heap and find the node to be updated
    std::vector<DATA>::iterator i;
    for( i=heap.begin(); i!=heap.end(); i++ )
    {
        if ( 0 == functor.DataCompare(*i, node) )
        {  //Found node - resort from this position in the heap
            //(since its total value was changed before this function was called)
#ifdef _DEBUG
            // qff: must be tested
            //if (counter == 0)
            //    assert( !"CPriorityQueue: cannot update top node" );
#endif // _DEBUG
            std::push_heap( heap.begin(), i+1, functor );
            return;
        }//if
#ifdef _DEBUG
        ++counter;
#endif // _DEBUG
    }//for
}//CPriorityQueue<DATA,GREATER_FUNCTOR>::UpdateNode()


template<typename DATA, typename GREATER_FUNCTOR>
bool CPriorityQueue<DATA,GREATER_FUNCTOR>::IsEmpty()
{
    //empty() is an STL function that determines if
    //the STL vector has no elements 
    return( heap.empty() );
}//CPriorityQueue<DATA,GREATER_FUNCTOR>::IsEmpty()

#endif // !defined(_CPriorityQueue_Defined_)


/* working example:

#include <stdio.h>
#include <iostream>
#include "PriorityQueue.h"

using namespace std;

class CValue
{
public:
    int x,y;
    
    CValue( int _x=0, int _y=0)
    {
        x = _x;
        y = _y;
    }
    void Print()
    {
        cout << "x=" << x << ", y=" << y << endl;
    }
};//class CValue


//-----------------------------------------------------
class CValueCompare
{
public:

    // heap sorting compare callbak
    // Note: if this function returns 1 when a < b
    //       heap is sorted decending order
    // ascending means there is smallest value on top of queue
    bool operator()(CValue* a, CValue* b) const 
    {
        return a->x > b->x;
        //          ^ ascending order sorting
    }

    // node compare callback for 'UpdateNode()'
    // first, second: first and second data to compare
    // return 0 if first == second
    // Note: this function must return 0 if two values are equal
    //       used to compare two data types to rearrange heap
    int DataCompare(CValue*& a, CValue*& b)
    {
        if (a->y == b->y)
            return 0;

        return 1;
    }//DataCompare()
};//class CValueCompare


void main()
{
    CPriorityQueue<CValue*,CValueCompare> q;
    
    CValue a,b,c;
    a = CValue(5,30);
    b = CValue(1,10);
    c = CValue(7,50);
    

    // push to heap
    CValue* pValue;
    pValue = &a;
    q.Push( pValue );
    pValue = &b;
    q.Push( pValue );
    pValue = &c;
    q.Push( pValue );
    
    // update test
    a.x = 6;
    a.y = 11;
    pValue = &a;
    q.UpdateNode( pValue ); // use node compare callback
    
    CValue* p;
    
    // pop test
    cout << "\n[top]\n" << endl;
    p = q.Top();
    p->Print();
    q.GetTop( &p );
    p->Print();
    
    // iterate queue
    cout << "\n[queue contents]\n" << endl;
    q.Pop( &p ); // first method
    p->Print();
    p = q.Pop(); // second method
    p->Print();
    p = q.Pop();
    p->Print();
}//main()

*/