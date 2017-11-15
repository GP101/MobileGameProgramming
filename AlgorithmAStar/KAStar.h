// jintaeks on 2017-11-15_21-36
#pragma once

#include "KGen.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <queue>


struct KIntPoint3D
{
public:
    KIntPoint3D( int x_ = 0, int y_ = 0, int z_ = 0 )
    {
        x = x_;
        y = y_;
        z = z_;
    }

    bool operator==( const KIntPoint3D& rhs ) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    int x;
    int y;
    int z;
};


struct AStarNode;
typedef std::shared_ptr<AStarNode>  AStarNodePtr;
struct AStarNode
{
    AStarNodePtr        parent;     // parent node (zero pointer represents starting node)
    
    KIntPoint3D         location;   // location of node (some location representation)
    float               cost;       // cost to get to this node
    float               heuristic;  // heuristic cost to get to goal node
    float               total;      // total cost (cost + heuristic estimate)
    bool                isOpen;     // on Open list
    bool                isClosed;   // on Closed list
    
    // must have default constructor for A* internal structure
    AStarNode( int x = 0, int y = 0, int z = 0)
    {
        location = KIntPoint3D( x, y, z );
        parent    = NULL;
        cost      = 0.0f;
        heuristic = 0.0f;
        total     = 0.0f;
        isOpen    = false;
        isClosed  = false;
    }

    // for struct AStarNodeEqualTo
    bool operator==( const AStarNode& rhs_ ) const
    {
        return location == rhs_.location;
    }

    // for struct AStarNodeLessThan
    bool operator<( const AStarNode& rhs_ ) const
    {
        return total < rhs_.total;
    }

    void Print() const
    {
        printf( " location = (%i, %i, %i)\n", location.x, location.y, location.z );
        printf( " cost = %g\n", cost );
        printf( " heuristic = %g\n", heuristic );
        printf( " total = %g\n", total );
        printf( " open=%d, closed=%d\n", isOpen, isClosed );
    }

    void PrintLocation() const
    {
        printf( "(%i, %i, %i)", location.x, location.y, location.z );
    }
};


class AStarNodeHash
{
public:
    size_t operator()( const AStarNodePtr node ) const
    {
        size_t h1 = std::hash<int>()( node->location.x );
        size_t h2 = std::hash<int>()( node->location.y );
        size_t h3 = std::hash<int>()( node->location.z );
        return h1 ^ ( h2 << 8 ) ^ ( h3 << 16 );
    }
};


struct AStarNodeEqualTo
{
    bool operator()( const AStarNodePtr& _Left, const AStarNodePtr& _Right ) const
    {
        return ( *_Left == *_Right );
    }
};


struct AStarNodeLessThan
{
    bool operator()( const AStarNodePtr& _Left, const AStarNodePtr& _Right ) const
    {
        //return (*_Left < *_Right);
        return ( *_Right < *_Left );
    }
};


class AStarPriorityQueue : public std::priority_queue<AStarNodePtr, std::vector<AStarNodePtr>, AStarNodeLessThan >
{
public:
    typedef std::vector<AStarNodePtr>::iterator     qiterator;

public:
    bool remove( const AStarNodePtr& value )
    {
        for( auto it = this->c.begin(); it != this->c.end(); ++it ) {
            if( **it == *value ) {
                this->c.erase( it );
                std::make_heap( this->c.begin(), this->c.end(), this->comp );
                return true;
            }
        }

        return false;
    }

    qiterator qbegin()
    {
        return this->c.begin();
    }

    qiterator qend()
    {
        return this->c.end();
    }
};


class AStarPath
{
public:
    typedef std::list<AStarNodePtr>     PATH;
    typedef PATH::iterator              iterator;
    typedef PATH::reverse_iterator      reverse_iterator;

private:
    // is path progressing?
    bool                m_bInProgress;
    PATH                m_path; // doubly linked list path

public:
    CONSTRUCTOR         AStarPath(){ m_bInProgress = false; }
    DESTRUCTOR          ~AStarPath() { Clear(); }

    bool                IsInProgress() const
                        {
                            return m_bInProgress;
                        }

    void                SetInProgress( bool bProgress_ )
                        {
                            m_bInProgress = bProgress_;
                        }

    // push node at tail
    void                PushBack( AStarNodePtr pIn ) { m_path.push_back( pIn ); }

    // push node at head
    void                PushFront( AStarNodePtr pIn ) { m_path.push_front( pIn ); }

    // get node from front
    // Note: this function doesn't delete node in list
    //       If you want to delete node, use PopFront() or Erase() or customized iterator
    AStarNodePtr        GetFront( AStarNodePtr& pOut ) { pOut = m_path.front(); return pOut; }

    // get node from back
    // Note: this function doesn't delete node in list
    //       If you want to delete node, use PopBack() or Erase() or customized iterator
    AStarNodePtr        GetBack( AStarNodePtr& pOut ) { pOut = m_path.back(); return pOut; }

    // remove node at back
    void                PopBack() { m_path.pop_back(); }

    // remove node at front
    void                PopFront() { m_path.pop_front(); }

    // begin position of node
    iterator            Begin() { return m_path.begin(); }
    // end position of node
    iterator            End() { return m_path.end(); }

    // reverse begin position of node
    reverse_iterator    RBegin() { return m_path.rbegin(); }
    // reverse end position of node
    reverse_iterator    REnd() { return m_path.rend(); }

    // get number of nodes in list
    int                 Size() const { return m_path.size(); }

    // check empty
    bool                IsEmpty() const { return m_path.empty(); }

    // clear all nodes in path(linked-list)
    void                Clear() { m_path.clear(); }

    // find node 'n'
    // n: [in] node to search
    // return 'Node*'
    AStarNodePtr        Find( const AStarNode& n );
    AStarNodePtr        Find( AStarNodePtr pNodeToSearch ) { return Find( *pNodeToSearch ); }

    // find node n
    // n: [in] node to search
    // return iterator of node 'n'
    bool                Search( OUT iterator& itorOut, AStarNode& n );
    bool                Search( OUT iterator& itorOut, AStarNodePtr pNodeToSearch ) { return Search( itorOut, *pNodeToSearch ); }

    // erase node 'n'
    // n: [in] node to erase
    bool                Erase( IN AStarNode& n );
    bool                Erase( IN AStarNodePtr pNodeErase ) { return Erase( *pNodeErase ); }
};//class AStarPath


AStarNodePtr AStarPath::Find( const AStarNode& n )
{
    for( iterator itor = Begin(); itor != End(); ++itor ) {
        if( n == **itor ) {
            return ( *itor );
        }
    }

    return NULL;
}


bool AStarPath::Search( OUT iterator& itorOut, AStarNode& n )
{
    for( iterator itor = Begin(); itor != End(); ++itor ) {
        if( n == **itor ) {
            itorOut = itor;
            return true;
        }
    }

    return false;
}


bool AStarPath::Erase( IN AStarNode& n )
{
    for( iterator itor = Begin(); itor != End(); ) {
        if( n == **itor ) {
            itor = m_path.erase( itor );
            return true;
        }
        else {
            ++itor;
        }
    }

    return false;
}


template <class Node, class FUNCTOR>
class KAStar
{
public:
    // data type for master node list and open list
    typedef std::shared_ptr<Node>
                        NodePtr;

    // candidate nodes to gather
    typedef std::vector<Node>
                        CANDIDATES;

    // A* related functor
    FUNCTOR             functor;

private:
    // examined node list, i.e. Close list
    std::unordered_set<AStarNodePtr, AStarNodeHash, AStarNodeEqualTo>
                        m_masterNodeList;
        
    // unexamined node list, i.e. Open list
    AStarPriorityQueue  m_openList;

public:
    CONSTRUCTOR         KAStar()
                        {
                        }

    DESTRUCTOR          ~KAStar()
                        {
                            m_masterNodeList.clear();
                            AStarPriorityQueue temp;
                            m_openList.swap( temp );
                        }

    // get NodePtr from master node list
    // pOut: [out] found(or newly allocated if exceeds node bank size) Node pointer
    // in: [in] NodePtr to compare
    //     this is always pointer so we passing it by value
    // return 'true' if node is already exist in 'master node list'
    // Desc: find 'in' node at Master Node List and return the pointer at 'pOut'
    //       If there is no 'in' node in Master Node List, 
    //       allocate new Node from Node Bank(m_nodeBank) then
    //       call 'functor.SetNewlyAllocatedNodeInfo()' to initialize newly allocated node
    //       and finally return the pointer of new node at 'pOut'
    bool                GetNode( OUT NodePtr& spNode, Node in );

    // find path from 'src' to 'goal'
    // and out found path if exist
    // pOutPath: [out] found path if exist
    //      You must instantiate this object, before call this function
    // src: [in] source node description
    // goal: [in] destination node description
    bool                FindPath( OUT AStarPath& pOutPath, const Node& srcNode, const Node& goalNode );

    void                Clear() 
                        {
                            m_masterNodeList.clear();
                            AStarPriorityQueue temp;
                            m_openList.swap( temp );
                        }
};


template <class Node, class FUNCTOR>
bool KAStar<Node, FUNCTOR>::GetNode( OUT NodePtr& spNode, Node in )
{
    NodePtr tempPtr;
    tempPtr.reset( new Node() );
    *tempPtr = in;

    auto it = m_masterNodeList.find( tempPtr );
    if( it != m_masterNodeList.end() ) {
        spNode = *it; // set [out] parameter
        return true;
    }
    else {
        // allocate a new node
        spNode.reset( new Node() );

        // set newly create node information
        functor.SetNewlyAllocatedNodeInfo( IN OUT spNode, in );

        // store this node to master node(Closed List)
        m_masterNodeList.insert( spNode );

        return false;
    }
}


template <class Node, class FUNCTOR>
bool KAStar<Node, FUNCTOR>::FindPath( IN OUT AStarPath& outPath, const Node& pSrc, const Node& goalNode )
{
    ///////////////////////////
    // debugging
    int debug = 1;

    // is this first call on 'outPath'
    if( outPath.IsInProgress() == false ) {
        // path is now progressing
        outPath.SetInProgress( true );

        NodePtr pStartNode; // start node

        //Create the very first node and put it on the Open list
        GetNode( pStartNode, pSrc );

        // setup start node
        functor.SetupStartNode( pStartNode, goalNode );

        // pust to open list
        m_openList.push( pStartNode );
    }//if

    // if there is a node on open list
    while( m_openList.empty() == false ) {
        // Get the best candidate node to search next
        NodePtr spBestNode = m_openList.top();
        m_openList.pop();

        // Found the goal node - construct a path and exit
        if( functor.IsSameNode( *spBestNode, goalNode ) == true ) {
            // The complete path will be stored inside the game object
            functor.ConstructPathToGoal( IN OUT outPath, spBestNode );

            return true;   //return with success
        }//if

        // gather candidate nodes
        CANDIDATES candidates;
        functor.GatherCandidates( IN OUT candidates, spBestNode );

        ////////////////////////////////////
        // debugging
        for( CANDIDATES::iterator itor2 = candidates.begin(); itor2 != candidates.end(); ++itor2 ) {
            g_console.GotoXy( (int)( *itor2 ).location.x, (int)( *itor2 ).location.z );
            g_console.SetColor( 8 );
            _putch( 'c' );
        }//for

        // loop through all connecting nodes of bestnode
        for( CANDIDATES::iterator itor = candidates.begin(); itor != candidates.end(); ++itor ) {
            // set new node
            Node newnode;
            newnode = ( *itor );
            // Note: if Node is not assignment safe
            //       you must prepare proper 'operator=()' in class Node

            // This avoids searching the node we just came from
            if( spBestNode->parent == nullptr
                || functor.IsSameNode( *spBestNode->parent, newnode ) == false ) {
                ////////////////////////////////////////
                // debugging
                g_console.GotoXy( (int)newnode.location.x, (int)newnode.location.z );
                g_console.SetColor( 15 );
                _putch( 'C' );

                // setup new node
                // set 'newnode' using spBestNode info and new heuristic cost
                functor.SetupNewCandidateNode( IN OUT newnode, spBestNode, goalNode );

                //Get the preallocated node for this location
                //Both newnode and actualnode represent the same node location,
                //but the search at this point may not want to clobber over the
                //data from a more promising route - thus the duplicate nodes for now
                NodePtr actualnode;
                GetNode( OUT actualnode, newnode );

                // Note: the following test takes O(1) time (no searching through lists)
                // This node is very promising
                if( functor.IsCandidateMorePromising( IN actualnode, newnode ) == true ) {
                    //Take it off the Open and Closed lists (in theory) and push on Open
                    // update actualnode's parent and cost info using updated 'newnode' info.
                    {
                        actualnode->isClosed = false; // effectively removing it from Closed list
                        // means re-examine this node
                        actualnode->parent = newnode.parent;
                        actualnode->cost = newnode.cost;
                        actualnode->heuristic = newnode.heuristic;
                        actualnode->total = newnode.total;
                    }

                    if( actualnode->isOpen == true ) {
                        // Since this node is already on the Open list, update it's position
                        // update priority queue
                        m_openList.remove( actualnode );
                        m_openList.push( actualnode );
                    }
                    else {
                        //Put the node on the Open list
                        m_openList.push( actualnode );

                        // set isOpen flag of actualnode
                        actualnode->isOpen = true; // just set flag
                    }//if.. else..
                }//if
            }//if
        }//for

        //Now that we've explored bestnode, put it on the Closed list
        // set 'isClosed' flag of 'spBestNode'
        spBestNode->isClosed = true; // now best node is examined

        ////////////////////////////////////
        // debugging
        if( debug ) {
            if( 27 == _getch() )
                debug = 0;
        }
        // print candidate
        for( CANDIDATES::iterator itor2 = candidates.begin(); itor2 != candidates.end(); ++itor2 ) {
            g_console.GotoXy( (int)( *itor2 ).location.x, (int)( *itor2 ).location.z );
            g_console.SetColor( 7 );
            _putch( '.' );
        }//for
        // print priority queue
        int counter = 0;
        char buffer[256];
        for( auto itor3 = m_openList.qbegin(); itor3 != m_openList.qend(); ++itor3 ) {
            g_console.GotoXy( (int)( *itor3 )->location.x, (int)( *itor3 )->location.z );
            g_console.SetColor( 10 );
            _putch( '.' );
            ++counter;
        }//for

        sprintf_s( buffer, "priority queue size = %d\n", counter );
        OutputDebugStringA( buffer ); /**/
        Sleep( 10 );
        if( debug )
            _getch();

        candidates.clear();

        //Use some method to determine if we've taken too much time
        //this tick and should abort the search until next tick
        // this can be triggered thread blocking and timer expiring
        if( functor.ShouldAbortSearch() ) {
            return false;
        }//if
    }//while

    //If we got here, all nodes have been searched without finding the goal
    return false;
}
