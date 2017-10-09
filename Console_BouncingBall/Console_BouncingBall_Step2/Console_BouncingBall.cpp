#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "ConsoleUtil.h"
#include "KBoard.h"

#pragma comment(lib,"winmm.lib")

bool                    g_bIsExitGameLoop = false;
KBoard                  g_visibleBoard;
KBoard                  g_hiddenBoard;
KBlock                  g_curBlock;

void InitializeBlock( OUT KBlock& block_ )
{
    block_.m_aBlockPosition[0].x = 0;
    block_.m_aBlockPosition[0].y = -1;
    block_.m_aBlockPosition[1].x = 1;
    block_.m_aBlockPosition[1].y = 0;
    block_.m_aBlockPosition[2].x = -1;
    block_.m_aBlockPosition[2].y = 0;
    block_.m_aBlockPosition[3].x = 0;
    block_.m_aBlockPosition[3].y = 1;
    block_.m_worldPos.x = 2;
    block_.m_worldPos.y = 2;
    block_.m_iNumStone = 4;
}

void Initialize()
{
    InitializeConsole();

    g_visibleBoard.Initialize();
    g_hiddenBoard.Initialize();

    InitializeBlock( g_curBlock );
}

void Finalize()
{
    FinalizeConsole();
}

void OnUpdate( float fElapsedTime_ )
{
    g_hiddenBoard.Initialize();
    g_hiddenBoard.LocateBlock( g_curBlock );
}

void DrawBoard( const KBoard& board_ )
{
    for( int y = 0; y < BOARD_SIZE_ROW; ++y ) {
        for( int x = 0; x < BOARD_SIZE_COL; ++x ) {
            SetCursorPosition( x, y );
            KPoint pos;
            pos.x = x;
            pos.y = y;
            int cellValue = 0;
            const bool isCellValue = board_.GetBoardCellValue( OUT &cellValue, pos );
            if( cellValue == 0 )
                cellValue = '.';
            printf( "%c", cellValue );
        }
    }
}

void OnDraw( float fElapsedTime_ )
{
    g_visibleBoard.CopyFrom( g_hiddenBoard );
    DrawBoard( g_visibleBoard );
}

void main()
{
    Initialize();

    DWORD dwCurrTime = 0L;
    DWORD dwPrevTime = 0L;
    dwCurrTime = timeGetTime();
    dwPrevTime = dwCurrTime;

    // game loop
    while( g_bIsExitGameLoop == false ) {
        if( _kbhit() ) g_bIsExitGameLoop = true;

        DWORD dwElapsedTime = 0L;
        float fElapsedTimeInSecond = 0.f;

        // calculate inter-frame elapsed time
        {
            dwPrevTime = dwCurrTime;
            dwCurrTime = timeGetTime();
            dwElapsedTime = dwCurrTime - dwPrevTime;
            fElapsedTimeInSecond = dwElapsedTime / 1000.f;
            Sleep( 10 );
        }

        // emulate conventional game loop
        OnUpdate( fElapsedTimeInSecond );
        OnDraw( fElapsedTimeInSecond );
    }//while

    Finalize();
}