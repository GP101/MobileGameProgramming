//////////////////////////////////////////////////////////////////////
// File: Path.h
// Desc: interface for the CPath class.
//       programmed by seojt@kogsoft.com
//       started on October 5, 2001
//

#if !defined(_CPath_Defined_)
#define _CPath_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list> // STL list
#include "kogGen.h"


// default compare functor
template<typename T>
class _path_default_functor
{
public:
    // compare two values
    // must return 0 if first==second
    int operator()(T& first, T& second)
    {
        if ( first == second )
            return 0;

        return 1;
    }//operator()()
};//class _path_default_functor


//----------------------------------------------------------------------
// Name: class CPath
// Desc: represents a result of 'class CAStar', i.e. a path
//       use STL doubly linked list
//
// Param: 
//      NODE: doubly linked list node type
//            Path is a linked list of 'NODE'
//
template<typename NODE, typename COMPARE = _path_default_functor<NODE> >
class CPath
{
public:
    // type exporting
    typedef typename std::list<NODE>         PATH;
    typedef typename PATH::iterator          iterator;
    typedef typename PATH::reverse_iterator  reverse_iterator;

    // compare functor
    COMPARE functor;

    // path is progressing
    bool    m_bInProgress;

public:
    PATH    m_path; // doubly linked list path

public:
    CPath(){ m_bInProgress = false; }
    ~CPath() { Clear(); }

    // push node at tail
    inline void PushBack(NODE n) { m_path.push_back(n); }
    inline void PushBack(NODE* pIn) { m_path.push_back(*pIn); }

    // push node at head
    inline void PushFront(NODE n) { m_path.push_front(n); }
    inline void PushFront(NODE* pIn) { m_path.push_front(*pIn); }

    // get node from front
    // Note: this function doesn't delete node in list
    //       If you want to delete node, use PopFront() or Erase() or customized iterator
    inline NODE GetFront(NODE* pOut) { *pOut = m_path.front(); return *pOut; }
    // get node from back
    // Note: this function doesn't delete node in list
    //       If you want to delete node, use PopBack() or Erase() or customized iterator
    inline NODE GetBack(NODE* pOut) { *pOut = m_path.back(); return *pOut; }

    // remove node at back
    inline void PopBack() { m_path.pop_back(); }
    // remove node at front
    inline void PopFront() { m_path.pop_front(); }

    // begin position of node
    inline iterator Begin() { return m_path.begin(); }
    // end position of node
    inline iterator End() { return m_path.end(); }

    // reverse begin position of node
    inline reverse_iterator RBegin() { return m_path.rbegin(); }
    // reverse end position of node
    inline reverse_iterator REnd() { return m_path.rend(); }

    // get path pointer
    inline PATH* GetPathPointer() const { return &m_path; }

    // get number of nodes in list
    inline int  Size() { return m_path.size(); }

    // check empty
    inline bool IsEmpty() { return m_path.empty(); }

    // set auto delete flag
    //void SetAutoDelete(bool bAutoDelete) { m_bAutoDelete = bAutoDelete; }

    // clear all nodes in path(linked-list)
    inline void Clear() { m_path.clear(); }

    // find node 'n'
    // n: [in] node to search
    // return 'NODE*'
    NODE* Find(NODE& n);
    NODE* Find(NODE* pNodeToSearch) { return Find(*pNodeToSearch); }

    // find node n
    // n: [in] node to search
    // return iterator of node 'n'
    bool Search( iterator& itorOut, NODE& n );
    bool Search( iterator& itorOut, NODE* pNodeToSearch ) { return Search( itorOut, *pNodeToSearch ); }

    // erase node 'n'
    // n: [in] node to erase
    bool Erase(NODE& n);
    bool Erase(NODE* pNodeErase) { return Erase(*pNodeErase); }
};//class CPath


template<typename NODE, typename COMPARE>
NODE* CPath<NODE,COMPARE>::Find(NODE& n)
{
    for (iterator itor=Begin(); itor != End(); ++itor)
    {
        if ( 0 == functor(n, *itor) )
        {
            return &(*itor);
        }//
    }//for

    return NULL;
}//Find()


template<typename NODE, typename COMPARE>
bool CPath<NODE, COMPARE>::Search( iterator& itorOut, NODE& n )
{
    for (iterator itor=Begin(); itor != End(); ++itor)
    {
        if ( 0 == functor(n, *itor) )
        {
            itorOut = itor;
            return true;
        }//
    }//for

    return false;
}//Search()


template<typename NODE, typename COMPARE>
bool CPath<NODE,COMPARE>::Erase(NODE& n)
{
    for (iterator itor=Begin(); itor != End();)
    {
        if ( 0 == functor(n, *itor) )
        {
            itor = m_path.erase(itor);
            return true;
        }
        else
            ++itor;
    }//for

    return false;
}//Erase()

#endif // !defined(_CPath_Defined_)


//////////////////////////////////////////////////////////////////////////////////
/* 'int' node path example:

#include <stdio.h>
#include "Path.h"


void main()
{
    CPath<int>  path;

    // push test
    path.PushBack(1);
    path.PushBack(3);
    path.PushBack(5);
    path.PushBack(7);
    path.PushBack(9);

    int i;

    // front test
    printf("front = %d\n", path.GetFront(&i) );

    path.PopFront();
    printf("front after pop = %d\n", path.GetFront(&i) );

    // iterator test
    printf("\n\niterator result\n");
    for (CPath<int>::iterator itor=path.Begin(); itor != path.End(); ++itor)
    {
        printf("%d\n", (*itor));
    }//for

    // erase test
    i = 7;
    if ( path.Erase(i) )
        printf("\n erase %d\n", 7 );

    // reverse iterator test
    printf("\nreverse iterator result\n");
    for (CPath<int>::reverse_iterator itor2=path.RBegin(); itor2 != path.REnd(); ++itor2)
    {
        printf("%d\n", (*itor2));
    }//for


    // find test
    i = 5;
    int* p = path.Find(i);

    if (p)
        printf( "\n found %d\n", *p );
    else
        printf( "\n not found\n" );

    // search test
    CPath<int>::iterator itor3;

    i = 5;
    itor3 = path.Search(i);
    if (itor3 != NULL)
        printf("\n search %d\n", 5);
    else
        printf("\n cannot search\n" );
}//main() 

*/


//////////////////////////////////////////////////////////////////////////////////
/* object pointer example

#include <d3dx8.h>
#include <stdio.h>
#include "Path.h"

struct Node
{
    D3DXVECTOR3 pos;
    float       weight;

    Node() { pos = D3DXVECTOR3(0,0,0); weight=0; }
    Node(float x, float y, float z, float w) {
        pos.x = x;
        pos.y = y;
        pos.z = z;
        weight = w;
    }//Node()
    void Print() {
        printf("(%g,%g,%g), weight=%g\n", pos.x, pos.y, pos.z, weight );
    }//Print()
};//struct Node

struct PNodeFunctor
{
    // compare function callback used in Find(), Search()
    // first,second: nodes to compare
    // must return 0 if first==second
    // return 1 else
    int operator()(Node*& first, Node*& second)
    {
        if ( first->pos == second->pos )
            return 0;

        return 1;
    }//Compare()
};//struct PNodeFunctor

void main()
{
    Node* pNode[5];
    int   i;

    // instantiate test objects
    for (i=0; i<5; ++i)
    {
        pNode[i] = new Node(i,i,i, 4-i);
    }//for

    // path object
    CPath<Node*,PNodeFunctor>  path;

    // push test
    for (i=0; i<5; ++i)
    {
        path.PushBack( pNode[i] );
    }//for

    Node* p; // temp pointer

    // front test
    path.GetFront(&p);
    printf("front: ");
    p->Print();

    // pop test
    delete p; // Danger!!! you must delete node 'p'
              // Otherwise you must keep 'pNode[]', and delete those manually
    path.PopFront();
    path.GetFront(&p);
    printf("front after pop: ");
    p->Print();

    // iterator test
    printf("\n\niterator result\n");
    for (CPath<Node*>::iterator itor=path.Begin(); itor != path.End(); ++itor)
    {
        (*itor)->Print();
    }//for

    // erase test
    Node* n;
    n = pNode[2];
    if ( path.Erase(n) )
    {
        printf("\n erase pNode[2]\n");
        delete pNode[2];
    }//if

    // reverse iterator test
    printf("\nreverse iterator result\n");
    for (CPath<Node*>::reverse_iterator itor2=path.RBegin(); itor2 != path.REnd(); ++itor2)
    {
        (*itor2)->Print();
    }//for


    // find test
    Node** p2 = path.Find(pNode[3]);

    if (p2)
    {
        printf(" found pNode[3] = ");
        (*p2)->Print();
    }//if

    // search test
    CPath<Node*>::iterator itor3;

    itor3 = path.Search(pNode[4]);
    if (itor3 != NULL)
    {
        printf(" search pNode[4] = ");
        (*itor3)->Print();
    }//if

    // manual delete test
    for (CPath<Node*>::iterator itor4=path.Begin(); itor4 != path.End(); ++itor4)
    {
        printf(" delete ");
        (*itor4)->Print();
        delete (*itor4);
    }//for
}//main()

*/