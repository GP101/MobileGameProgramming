#include <stdio.h>
#include <windows.h>
#include "AStar.h"
#include "console.h"
#include "graphics2d.h"

/////////////////////////////////////////////////////////////
// Global objects
CConsole        con(80,40,"A* test");
CGraphics2D     graphics;
BYTE            board[39][80]; // 80*39 board

/////////////////////////////////////////////////////////////
// Global functions

void PutPixel(int x, int y, int value)
{   // you don't need to clip test. it's clipped
    board[y][x] = value;
}//PutPixel()


void MemSet(int offset, int value)
{
    if (offset < 80*39) // clipping test
    {
        int x,y;
        y = offset/80;
        x = offset%80;
        board[y][x] = value;
    }//if
}//MemSet()


// add boundary to board
void InitBoard()
{
    memset( board, 0, sizeof(board) );

    int row, col;
    for (row=0; row<39; ++row)
    {
        board[row][0] = 'O';
        board[row][79] = 'O';
    }//for
    for (col=0; col<80; ++col)
    {
        board[0][col] = 'O';
        board[38][col] = 'O';
    }//for
}//InitBoard()


// draw current board[][] state
void DrawBoard()
{
    int row, col;

    for (row=0; row<39; ++row)
    {
        for (col=0; col<80; ++col)
        {
            con.gotoxy( col, row );
            _putch( board[row][col] );
        }//for
    }//for
    con.gotoxy(0,40);
    printf("S: source, G: goal, .: seen, Green .: heap");
}//DrawBoard()


// Helper function to stuff a FLOAT into a DWORD argument 
//  without loss of bit sequence
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


//---------------------------------------------------------------
// Name: struct AStarNode
// Desc: A* node type for 'Extreme Rally'
struct AStarNode
{
    AStarNode*   parent;     // parent node (zero pointer represents starting node)
    
    KVector      location;   // location of node (some location representation)
    float        cost;       // cost to get to this node
    float        heuristic;  // heuristic cost to get to goal node
    float        total;      // total cost (cost + heuristic estimate)
    BYTE         onOpen;     // on Open list
    BYTE         onClosed;   // on Closed list
    
    // must have default constructor for A* internal structure
    AStarNode()
    {
        location = KVector( 0, 0, 0 );
        parent    = NULL;
        cost      = 0.0f;
        heuristic = 0.0f;
        total     = 0.0f;
        onOpen    = 0;
        onClosed  = 0;
    }//Node()

    void Print()
    {
        printf(" location = (%g, %g, %g)\n", location.x, location.y, location.z );
        printf(" cost = %g\n", cost );
        printf(" heuristic = %g\n", heuristic );
        printf(" total = %g\n", total);
        printf(" open=%d, closed=%d\n", onOpen, onClosed );
    }//Print()

    void PrintLocation()
    {
        printf("(%g, %g, %g)", location.x, location.y, location.z );
    }//Print()
};//struct AStarNode


//---------------------------------------------------------------------
// Name: struct AStarFunctor
// Desc: functor for A* open list(priority queue)
struct AStarFunctor
{
    /////////////////////////////////////////////////////////////
    // for Open List

    // Open List priority queue sorting compare function
    // if this function returns 1 when first < second
    // priority queue(open list) is sorted decending order
    // i.e. there is largest value on top of the open list
    bool operator()(const AStarNode* first, const AStarNode* second) const 
    {
        return first->total > second->total;
        //                  ^there is smallest value on top of open list
        //                   i.e. there is smallest value on the top of the open list
    }// heap greater test

    // Open List compare function
    // must return 0 if first==second
    int DataCompare(const AStarNode* first, const AStarNode* second) const
    {
        // check two node's position
        if (first->location == second->location)
            return 0;
    
        return 1; // first != second
    }//DataCompare()


    ////////////////////////////////////////////////////////
    // for Closed List

    // Closed List compare function
    // A* Node equality test function
    // must return 0 if first==second
    int KeyCompare(const AStarNode* first, const AStarNode* second) const
    {
        // check two node's position
        if (first->location == second->location)
            return 0;
    
        return 1; // first != second
    }//KeyCompare()


    // A* Master node list Hash function
    int Hash(int slotSize, const AStarNode* key) const
    {
        DWORD dw;
        DWORD temp;
        DWORD temp2;
    
        // this is my own heuristic hash fun.

        // get bit sequence of x-value
        dw = FtoDW( key->location.x );
    
        // cross over z-value
        temp = FtoDW( key->location.z );
        temp2 = temp & 0xffff0000;
        temp2 >>= 16;
        temp <<= 16;
        temp |= temp2;
    
        // add x+z
        dw += temp;
    
        return dw % slotSize;
    }//Hash()


    ////////////////////////////////////////////////////////////////
    // for A* FindPath()

    // called when node is newly allocated
    void SetNewlyAllocatedNodeInfo(AStarNode** pNewNode, AStarNode*& infoNode)
    {
        (*pNewNode)->location  = infoNode->location;
        //(*pNewNode)->cost      = infoNode->cost;
        //(*pNewNode)->heuristic = infoNode->heuristic;
        //(*pNewNode)->total     = infoNode->total;
        (*pNewNode)->onOpen    = 0;
        (*pNewNode)->onClosed  = 0;
    }//SetNewlyAllocatedNodeInfo()

    // setup start node
    // pStartNode: [out] recently created start node
    // pGoal: [in] goal node
    void SetupStartNode( AStarNode* pStartNode, AStarNode* pGoal )
    {
        pStartNode->onOpen    = 1;    //This node is going on the Open list
        pStartNode->onClosed  = 0;   //This node is not on the Closed list
        pStartNode->parent    = NULL;    //This node has no parent
        pStartNode->cost      = 0;       //This node has no cost to get to
        pStartNode->heuristic = GetNodeHeuristicCost( pStartNode, pGoal );
        pStartNode->total     = pStartNode->cost + pStartNode->heuristic;
    }//SetupStartNode()

    // called after found Goal
    // construct the path at Goal
    // reverse tracing from goal to parent until meets NULL
    void ConstructPathToGoal( CPath<AStarNode*>* pPath, AStarNode* pGoal )
    {
        AStarNode* pNode = pGoal;

        // construct front to back linked list
        while ( pNode )
        {
            pPath->PushFront( pNode );
            pNode = pNode->parent;
        }//while
    }//ConstructPathToGoal()

    // test pNode's parent exist
    bool IsParentExist( AStarNode* pNode )
    {
        return pNode->parent != NULL;
    }//IsParentExist()

    // called when node is set to candidate
    // pNewNode: [out] new candidate node
    // pBestNode: [in] previous node(candidate motive node)
    // pGoal: [in] goal node
    void SetupNewCandidateNode( AStarNode* pNewCandidateNode, 
        AStarNode* pBestNode, AStarNode* pGoal )
    {
        pNewCandidateNode->parent    = pBestNode;
        pNewCandidateNode->cost      = pBestNode->cost 
            + CostFromNodeToNode( pBestNode, pNewCandidateNode );
        pNewCandidateNode->heuristic = GetNodeHeuristicCost( pNewCandidateNode, pGoal );
        pNewCandidateNode->total     = pNewCandidateNode->cost 
            + pNewCandidateNode->heuristic;
    }//SetupNewCandidateNode()

    // check whether new candidate node is more promising than previously calculated one
    // pSamePrevious: [in] previously calculated node
    // pNewCandidate: [in] new candidate node
    bool IsCandidateMorePromising( AStarNode* pSamePrevious, AStarNode* pNewCandidate)
    {
        //return !( pSamePrevious->onOpen && pNewCandidate->total > pSamePrevious->total ) &&
        //    !( pSamePrevious->onClosed && pNewCandidate->total > pSamePrevious->total );
        return pNewCandidate->total < pSamePrevious->total ||
            ( pSamePrevious->onOpen == 0 && pSamePrevious->onClosed == 0 );
    }//IsCandidateMorePromising()

    ///////////////////////////////////////////////////////////////
    // cost calculation 

    // return heuristic cost from 'pSrc' to 'pDest'
    // this must be well defined
    // pSrc: [in] start node
    // pDest: [in] destination node
    float GetNodeHeuristicCost(AStarNode* pSrc, AStarNode* pDest)
    {
        float dx, dz;

        dx = pSrc->location.x - pDest->location.x;
        dz = pSrc->location.z - pDest->location.z;

        //return (dx*dx+dz*dz); // overestimate! Wow, powerful!!
        return 0.f; // qff
    }//GetNodeHeuristicCost()

    // return cost from 'pPrevious' to 'pNext'
    // pPrevious: [in] previous node
    // pNext: [in] next node
    float CostFromNodeToNode( AStarNode* pPrevious, AStarNode* pNext )
    {
        float dx, dz;

        dx = pPrevious->location.x - pNext->location.x;
        dz = pPrevious->location.z - pNext->location.z;

        return sqrtf(dx*dx+dz*dz);
    }//CostFromNodeToNode()

    // gather candidate nodes at pCurrent position
    // candidate: [out] gathered nodes
    // pCurrent: start node to gather
    void GatherCandidates( std::vector<AStarNode>& candidate, AStarNode* pCurrent )
    {
        AStarNode node;
        node = *pCurrent;

        // Moore neighbors(8-connected components)
        static int offset[8][2]={1,0,0,-1,-1,0,0,1, 1,-1,-1,-1,-1,1,1,1};

        //for( int i = 0; i<8; ++i ) // 8 neighbors
        for( int i = 0; i<4; ++i ) // 4 neighbors
        {
            int x;
            int z;

            x = int( pCurrent->location.x + offset[ i ][ 0 ] );
            z = int( pCurrent->location.z + offset[ i ][ 1 ] );
            node.location.x = (float)x;
            node.location.z = (float)z;

            // if board is empty or Goal
            if ( 0 == board[z][x] || 'G' == board[z][x] )
                candidate.push_back( node );
        }//for
    }//GatherCandidates()

    // return true if you want to interrupt FindPath()
    // If you want to resume FindPath() just call same FindPath()
    //  without modifying any related variables
    bool ShouldAbortSearch()
    {
        return false;
    }//ShouldAbortSearch()

};//struct AStarFunctor


void main()
{
    //setup graphics
    con.showcursor( false );
    graphics.SetCallback( PutPixel, MemSet );
    graphics.SetMemPitch(80);
    graphics.SetClipRect(0,0,79,38);


    // setup A* object
    CPath<AStarNode*>               path;
    CAStar<AStarNode,AStarFunctor>  astar;
      //   ^real node type to allocate
      //             ^functor for priority queue(open list) comparison
      //             ^functor for hash table(master node list) comparison
      //                          ^reserved node size

    ///////////////////////////////////////////////////
    // setup source and goal
    AStarNode src;
    AStarNode goal;
    src.location = KVector( 20, 0, 15 );
    goal.location = KVector( 43, 0, 25 ); // original
    //goal.location = KVector( 62, 0, 25 ); // test

    // initialize board[][]
    InitBoard();

    // add some walls
    graphics.Bresenham(40,10,40,28,'O');
    graphics.Bresenham(40,28,50,28,'O');
    graphics.Bresenham( 30, 23, 37, 23, 'O' );
    //graphics.Bresenham( 53, 15, 66, 33, 'O' );
    PutPixel( (int)src.location.x, (int)src.location.z, 'S' );   // source
    PutPixel( (int)goal.location.x, (int)goal.location.z, 'G' ); // goal

    // draw board
    DrawBoard();

    // find path
    if ( astar.FindPath( &path, &src, &goal ) == true )
    {
        // print path
        for (CPath<AStarNode*>::iterator i = path.Begin(); i != path.End(); ++i)
        {
            con.setcolor(9,7);
            con.gotoxy( (int)( *i )->location.x, (int)( *i )->location.z );
            _putch('*');
        }//for
    }//if

    astar.Clear(); // actually this isn't necessary

    con.gotoxy(1,1);
    con.showcursor( true );
    _getch();
}//main()
