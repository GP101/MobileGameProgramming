#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "ConsoleUtil.h"

#pragma comment(lib,"winmm.lib")

bool                    g_bIsExitGameLoop = false;


void Initialize()
{
    InitializeConsole();
}

void Finalize()
{
    FinalizeConsole();
}

void OnUpdate( float fElapsedTime_ )
{
}

void OnDraw( float fElapsedTime_ )
{
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
