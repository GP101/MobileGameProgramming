#include "ConsoleUtil.h"

HANDLE                  g_hConsole = INVALID_HANDLE_VALUE;
CONSOLE_CURSOR_INFO     g_oldConsoleCursorInfo;

void InitializeConsole()
{
    // Get the Win32 handle representing standard output.
    // This generally only has to be done once, so we make it static.
    g_hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

    // save console cursor info.
    //
    GetConsoleCursorInfo( g_hConsole, &g_oldConsoleCursorInfo );
    CONSOLE_CURSOR_INFO consoleCursorInfo;
    consoleCursorInfo = g_oldConsoleCursorInfo;
    consoleCursorInfo.bVisible = false;
    SetConsoleCursorInfo( g_hConsole, &consoleCursorInfo );
}

void FinalizeConsole()
{
    // restore console cursor info.
    SetConsoleCursorInfo( g_hConsole, &g_oldConsoleCursorInfo );
}

void SetCursorPosition( int x, int y )
{
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition( g_hConsole, coord );
}
