//////////////////////////////////////////////////////////////////////
// File: AStar.h
// Desc: interface for the CAStar class.
//       programmed by seojt@kogsoft.com
//       started on October 5, 2001
//

#if !defined(_CAStar_Defined_)
#define _CAStar_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KVector.h"
#include "priorityQueue.h"
#include "hashTable.h"
#include "path.h"
#include "memoryBank.h"
#include <unordered_set>

//-----------------------------------------------------------------------
// Name: class CAStar
// Desc: A* path finding
// Param:
//      NODE: node type of A*
//            This is type of node bank(m_nodeBank)
//            We actually use 'NODE*' in 'master node list(Close list)' 
//              and 'Open list'
//
//      FUNCTOR: A* functor(refer to example commented)
//
//      sizeNodeBank: node bank size
//
//      sizeMasterNode: slot number of hash table(m_masterNodeList)
//          We implictly maintain 'Close list' as hash table
//          We actually maintain 'Master Node List' for 'Close List'
//          We use 'Master Node List' for 1) checking the node is already 
//              examined and 2) fast node fetching.
//
//      Note: If you use divider as hash function, it is strongly recommended
//            that 'sizeMasterNode' is prime number like 701
//
// Note: This implementation was heavily referenced from 
//       Steve Rabin, "3.5 A* Speed Optimization", 
//       Game Programming Gems, 2000, pp.272-287
//
template <class NODE, class FUNCTOR, int sizeNodeBank=701, int sizeMasterNode=701>
class CAStar
{
public:
    // data type for master node list and open list
    typedef NODE*               PNODE;

    // candidate nodes to gather
    typedef std::vector<NODE>   CANDIDATES;

    // A* related functor
    FUNCTOR functor;


public:
    // node bank
    CMemoryBank<NODE>              m_nodeBank;//(sizeNodeBank)

    // examined node list, i.e. Close list
    CHashTable<PNODE,FUNCTOR,sizeMasterNode>  m_masterNodeList; // CLOSE list and some more stuffs
        
    // unexamined node list, i.e. Open list
    CPriorityQueue<PNODE,FUNCTOR>  m_openList; // OPEN list

public:
    /////////////////////////////////////////////////////////////////////
    // constructor & destructor
    CAStar() { m_nodeBank.Reserve( sizeNodeBank ); }//CAStar()
    ~CAStar()
    {
        m_nodeBank.FreeAll();
        m_masterNodeList.Clear();
        m_openList.Clear();
    }//~CAStar()


    ////////////////////////////////////////////////////////////////////////////
    // A* related members

    // get PNODE from master node list
    // pOut: [out] found(or newly allocated if exceeds node bank size) NODE pointer
    // in: [in] PNODE to compare
    //     this is always pointer so we passing it by value
    // return 'true' if node is already exist in 'master node list'
    // Desc: find 'in' node at Master Node List and return the pointer at 'pOut'
    //       If there is no 'in' node in Master Node List, 
    //       allocate new NODE from Node Bank(m_nodeBank) then
    //       call 'functor.SetNewlyAllocatedNodeInfo()' to initialize newly allocated node
    //       and finally return the pointer of new node at 'pOut'
    bool GetNode(PNODE* pOut, PNODE in);

    // find path from 'src' to 'goal'
    // and out found path if exist
    // pOutPath: [out] found path if exist
    //      You must instantiate this object, before call this function
    // src: [in] source node description
    // goal: [in] destination node description
    bool FindPath(CPath<PNODE>* pOutPath, NODE* pSrc, NODE* pGoal);

    void Clear() 
    {
        m_nodeBank.Reset();
        m_masterNodeList.Clear();
        m_openList.Clear();
    }//Clear()
};//class CAStar


template <class NODE, class FUNCTOR, int sizeNodeBank, int sizeMasterNode>
bool CAStar<NODE,FUNCTOR,sizeNodeBank,sizeMasterNode>::
GetNode(PNODE* pOut, PNODE in)
{
    *pOut = in;

    if ( m_masterNodeList.Get( pOut ) )
    {   // there is the node in master node
        return true;
    }
    else
    {   // allocate a new node
        *pOut = m_nodeBank.Alloc();

        // set newly create node information
        functor.SetNewlyAllocatedNodeInfo( pOut, in );

        // store this node to master node(Closed List)
        m_masterNodeList.Store( pOut );

        return false;
    }//if.. else..
}//GetNode()


template <class NODE, class FUNCTOR, int sizeNodeBank, int sizeMasterNode>
bool CAStar<NODE,FUNCTOR,sizeNodeBank,sizeMasterNode>::
FindPath(CPath<PNODE>* pPath, NODE* pSrc, NODE* pGoal)
{
    ///////////////////////////
    // debugging
    int debug=1;


    // is this first call on 'pPath'
    if( !pPath->m_bInProgress )
    {
        // path is now progressing
        pPath->m_bInProgress = true;

        PNODE pStartNode; // start node

        //Create the very first node and put it on the Open list
        GetNode( &pStartNode, pSrc );

        // setup start node
        functor.SetupStartNode( pStartNode, pGoal );

        // pust to open list
        m_openList.Push( pStartNode );
    }//if
    
    PNODE pBestNode; // top node in Open List

    // if there is a node on open list
    while( !m_openList.IsEmpty() )
    {
        //Get the best candidate node to search next
        m_openList.Pop( &pBestNode );
        
        if( 0 == functor.DataCompare( pBestNode, pGoal ) )
        {   //Found the goal node - construct a path and exit
            //The complete path will be stored inside the game object
            functor.ConstructPathToGoal( pPath, pBestNode );
            
            return true;   //return with success
        }//if
        
        // gather candidate nodes
        CANDIDATES candidates;
        functor.GatherCandidates( candidates, pBestNode );

        ////////////////////////////////////
        // debugging
        for (CANDIDATES::iterator itor2=candidates.begin(); itor2 != candidates.end(); ++itor2 )
        {
            con.gotoxy( (int)( *itor2 ).location.x, (int)( *itor2 ).location.z );
            con.setcolor(8);
            _putch('c');
        }//for


        // loop through all connecting nodes of bestnode
        for (CANDIDATES::iterator itor = candidates.begin(); itor != candidates.end(); ++itor)
        {
            // set new node
            NODE newnode;
            newnode = (*itor);
                // Note: if NODE is not assignment safe
                //       you must prepare proper 'operator=()' in class NODE
            
            // This avoids searching the node we just came from
            if( !functor.IsParentExist( pBestNode ) || 
                0 != functor.DataCompare(pBestNode->parent, &newnode) )
            {   
                ////////////////////////////////////////
                // debugging
                con.gotoxy( (int)newnode.location.x, (int)newnode.location.z );
                con.setcolor(15);
                _putch('C');

                // setup new node
                // set 'newnode' using pBestNode info and new heuristic cost
                functor.SetupNewCandidateNode( &newnode, pBestNode, pGoal );
                
                //Get the preallocated node for this location
                //Both newnode and actualnode represent the same node location,
                //but the search at this point may not want to clobber over the
                //data from a more promising route - thus the duplicate nodes for now
                PNODE actualnode;
                GetNode( &actualnode, &newnode );
                
                //Note: the following test takes O(1) time (no searching through lists)
                if ( functor.IsCandidateMorePromising( actualnode, &newnode ) )
                {   //This node is very promising
                    //Take it off the Open and Closed lists (in theory) and push on Open
                    // update actualnode's parent and cost info using updated 'newnode' info.
                    {
                        actualnode->onClosed = 0; //effectively removing it from Closed list
                        // means re-examine this node
                        actualnode->parent = newnode.parent;
                        actualnode->cost = newnode.cost;
                        actualnode->heuristic = newnode.heuristic;
                        actualnode->total = newnode.total;
                    }

                    if( actualnode->onOpen != 0 )
                    {   //Since this node is already on the Open list, update it's position
                        // update priority queue
                        m_openList.UpdateNode( actualnode );
                    }
                    else
                    {   //Put the node on the Open list
                        m_openList.Push( actualnode );
                        
                        // set onOpen flag of actualnode
                        actualnode->onOpen = 1; // just set flag
                    }//if.. else..
                }//if
            }//if
        }//for


        
        //Now that we've explored bestnode, put it on the Closed list
        // set 'onClosed' flag of 'pBestNode'
        pBestNode->onClosed = 1; // now best node is examined
        
        ////////////////////////////////////
        // debugging
        if (debug)
        {
            if ( 27 == _getch() )
                debug = 0;
        }
        // print candidate
        for( CANDIDATES::iterator itor2 = candidates.begin(); itor2 != candidates.end(); ++itor2 )
        {
            con.gotoxy( (int)( *itor2 ).location.x, (int)( *itor2 ).location.z );
            con.setcolor(7);
            _putch('.');
        }//for
        // print priority queue
        int counter=0;
        char buffer[256];
        for (CPriorityQueue<PNODE,FUNCTOR>::iterator itor3=m_openList.Begin(); itor3 != m_openList.End(); ++itor3)
        {
            con.gotoxy( (int)( *itor3 )->location.x, (int)( *itor3 )->location.z );
            con.setcolor(10);
            _putch('.');
            ++counter;
        }//for

        sprintf_s( buffer, "priority queue size = %d\n", counter );
        OutputDebugStringA( buffer ); /**/
        Sleep(10);
        if (debug)
            _getch();

        candidates.clear();

        //Use some method to determine if we've taken too much time
        //this tick and should abort the search until next tick
        // this can be triggered thread blocking and timer expiring
        if( functor.ShouldAbortSearch() ) 
        {
            return false;
        }//if
    }//while
    
    //If we got here, all nodes have been searched without finding the goal
    return false;
}//FindPath()


#endif // !defined(_CAStar_Defined_)


/* working example:


*/