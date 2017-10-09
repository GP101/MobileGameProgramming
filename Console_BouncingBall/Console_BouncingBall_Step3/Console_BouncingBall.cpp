#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "ConsoleUtil.h"
#include "KBoard.h"

#pragma comment(lib,"winmm.lib")

enum EKeys
{
    KEY_ESC = 27,
    KEY_UP = 72,
    KEY_LEFT = 75,
    KEY_RIGHT = 77,
    KEY_DOWN = 80,
};

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

    KBlock tempBlock;
    tempBlock.CopyFrom( g_curBlock );

    // process user keyboard inputs
    int ch = 0;
    if( _kbhit() ) {
        ch = _getch();

        if( ch == KEY_ESC ) {
            g_bIsExitGameLoop = true;
        }
        else if( ch == 224 ) {
            ch = _getch();
            if( ch == KEY_LEFT ) {
                tempBlock.m_worldPos.x -= 1;
            }
            else if( ch == KEY_RIGHT ) {
                tempBlock.m_worldPos.x += 1;
            }
            else if( ch == KEY_UP ) {
                tempBlock.m_worldPos.y -= 1;
            }
            else if( ch == KEY_DOWN ) {
                tempBlock.m_worldPos.y += 1;
            }

            if( g_hiddenBoard.CanLocateBlock( tempBlock ) ) {
                g_curBlock.CopyFrom( tempBlock );
            }
        }
    }//if
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
