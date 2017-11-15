#include <stdio.h>
#include <windows.h>
#include "KAStar.h"
#include "KConsole.h"
#include "KGraphics2d.h"
#include <memory>


KConsole                g_console( 80, 40, "A* test" );
KGraphics2D             g_graphics;
BYTE                    g_board[39][80]; // 80*39 g_board


void PutPixel( int x, int y, int value )
{
    g_board[y][x] = value;
}

void MemSet( int offset, int value )
{
    if( offset < 80 * 39 ) // clipping test
    {
        int x, y;
        y = offset / 80;
        x = offset % 80;
        g_board[y][x] = value;
    }//if
}

void InitBoard()
{
    memset( g_board, 0, sizeof( g_board ) );

    int row, col;
    for( row = 0; row < 39; ++row ) {
        g_board[row][0] = 'O';
        g_board[row][79] = 'O';
    }//for
    for( col = 0; col < 80; ++col ) {
        g_board[0][col] = 'O';
        g_board[38][col] = 'O';
    }//for
}

void DrawBoard()
{
    int row, col;

    for( row = 0; row < 39; ++row ) {
        for( col = 0; col < 80; ++col ) {
            g_console.GotoXy( col, row );
            _putch( g_board[row][col] );
        }//for
    }//for
    g_console.GotoXy( 0, 40 );
    printf( "S: source, G: goal, .: closed, Green .: open" );
}


struct AStarFunctor
{
    bool IsSameNode( const AStarNode& first, const AStarNode& second ) const
    {
        // check two node's position
        if( first.location == second.location )
            return true;

        return false;
    }

    // called when node is newly allocated
    void SetNewlyAllocatedNodeInfo( OUT AStarNodePtr& spNewNode, const AStarNode& infoNode )
    {
        ( spNewNode )->location = infoNode.location;
        //(spNewNode)->cost      = infoNode->cost;
        //(spNewNode)->heuristic = infoNode->heuristic;
        //(spNewNode)->total     = infoNode->total;
        ( spNewNode )->isOpen = 0;
        ( spNewNode )->isClosed = 0;
    }

    // setup start node
    // spStartNode: [out] recently created start node
    // goalNode: [in] goal node
    void SetupStartNode( OUT AStarNodePtr& spStartNode, const AStarNode& goalNode )
    {
        spStartNode->isOpen = 1;    //This node is going on the Open list
        spStartNode->isClosed = 0;   //This node is not on the Closed list
        spStartNode->parent = NULL;    //This node has no parent
        spStartNode->cost = 0;       //This node has no cost to get to
        spStartNode->heuristic = _GetNodeHeuristicCost( *spStartNode, goalNode );
        spStartNode->total = spStartNode->cost + spStartNode->heuristic;
    }

    // called after found Goal
    // construct the path at Goal
    // reverse tracing from goal to parent until meets NULL
    void ConstructPathToGoal( OUT AStarPath& path, AStarNodePtr spGoalNode )
    {
        AStarNodePtr spNode = spGoalNode;

        // construct front to back linked list
        while( spNode != nullptr ) {
            path.PushFront( spNode );
            spNode = spNode->parent;
        }
    }

    // called when node is set to candidate
    // newCandidateNode: [out] new candidate node
    // spBestNode: [in] previous node(candidate motive node)
    // goalNode: [in] goal node
    void SetupNewCandidateNode( IN OUT AStarNode& newCandidateNode
        , AStarNodePtr spBestNode, const AStarNode& goalNode )
    {
        newCandidateNode.parent = spBestNode;
        newCandidateNode.cost = spBestNode->cost + _CostFromNodeToNode( *spBestNode, newCandidateNode );
        newCandidateNode.heuristic = _GetNodeHeuristicCost( IN OUT newCandidateNode, goalNode );
        newCandidateNode.total = newCandidateNode.cost + newCandidateNode.heuristic;
    }

    // check whether new candidate node is more promising than previously calculated one
    // spPreviousNode: [in] previously calculated node
    // newCandidateNode: [in] new candidate node
    bool IsCandidateMorePromising( const AStarNodePtr spPreviousNode, const AStarNode& newCandidateNode )
    {
        return newCandidateNode.total < spPreviousNode->total // is new candidate is more promising
            || ( spPreviousNode->isOpen == 0 && spPreviousNode->isClosed == 0 ); // or brand new node
    }

    // return heuristic cost from 'startNode' to 'destNode'
    // this must be well defined
    // startNode: [in] start node
    // destNode: [in] destination node
    float _GetNodeHeuristicCost( const AStarNode& startNode, const AStarNode& destNode )
    {
        const float dx = float( startNode.location.x - destNode.location.x );
        const float dz = float( startNode.location.z - destNode.location.z );

        return ( dx*dx + dz*dz ); // overestimate! Wow, powerful!!
        //return 0.f; // test
    }

    // return cost from 'prevNode' to 'nextNode'
    // prevNode: [in] previous node
    // nextNode: [in] next node
    float _CostFromNodeToNode( const AStarNode& prevNode, const AStarNode& nextNode )
    {
        const float dx = float( prevNode.location.x - nextNode.location.x );
        const float dz = float( prevNode.location.z - nextNode.location.z );

        return sqrtf( dx*dx + dz*dz );
    }

    // gather candidate nodes at spCurrent position
    // candidate: [out] gathered nodes
    // spCurrent: start node to gather
    void GatherCandidates( IN OUT std::vector<AStarNode>& vecCandidate, AStarNodePtr spCurrent )
    {
        AStarNode node;
        node = *spCurrent;

        // Moore neighbors(8-connected components)
        static int offset[8][2] = {
            1, 0,
            0, -1,
            -1, 0,
            0, 1,
            1, -1,
            -1, -1,
            -1, 1,
            1, 1 };

        //for( int i = 0; i<8; ++i ) { // 8 neighbors
        for( int i = 0; i < 4; ++i ) { // 4 neighbors
            const int x = int( spCurrent->location.x + offset[i][0] );
            const int z = int( spCurrent->location.z + offset[i][1] );
            node.location.x = x;
            node.location.z = z;

            // if g_board is empty or Goal
            if( 0 == g_board[z][x] || 'G' == g_board[z][x] )
                vecCandidate.push_back( node );
        }//for
    }

    // return true if you want to interrupt FindPath()
    // If you want to resume FindPath() just call same FindPath()
    //  without modifying any related variables
    bool ShouldAbortSearch()
    {
        return false;
    }
};//struct AStarFunctor


void main()
{
    // setup g_graphics
    //
    g_console.ShowCursor( false );
    g_graphics.SetCallback( PutPixel, MemSet );
    g_graphics.SetMemPitch( 80 );
    g_graphics.SetClipRect( 0, 0, 79, 38 );

    // setup A* object
    //
    AStarPath           path;
    KAStar<AStarNode, AStarFunctor>
                        astar;

    // setup source and goal
    //
    AStarNode src;
    AStarNode goal;
    src.location = KIntPoint3D( 20, 0, 15 );
    goal.location = KIntPoint3D( 43, 0, 25 ); // original
    //goal.location = KIntPoint3D( 62, 0, 25 ); // test

    InitBoard();

    // add some colliders
    //
    g_graphics.Bresenham( 40, 10, 40, 28, 'O' );
    g_graphics.Bresenham( 40, 28, 50, 28, 'O' );
    g_graphics.Bresenham( 30, 23, 37, 23, 'O' );
    //g_graphics.Bresenham( 53, 15, 66, 33, 'O' );
    PutPixel( (int)src.location.x, (int)src.location.z, 'S' ); // source
    PutPixel( (int)goal.location.x, (int)goal.location.z, 'G' ); // goal

    DrawBoard();

    // find path
    bool isFindPath = astar.FindPath( IN OUT path, src, goal );
    if( isFindPath == true  ) {
        // print path
        for( AStarPath::iterator i = path.Begin(); i != path.End(); ++i ) {
            g_console.SetColor( 9, 7 );
            g_console.GotoXy( (int)( *i )->location.x, (int)( *i )->location.z );
            _putch( '*' );
        }
    }

    astar.Clear();

    g_console.GotoXy( 1, 1 );
    g_console.ShowCursor( true );
    _getch();
}
