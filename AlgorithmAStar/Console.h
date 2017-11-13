//////////////////////////////////////////////////////////////////////
// File: Console.h
// Desc: interface for the CConsole class.
//       programmed by seojt@kogsoft.com
//       started on October 8, 2001
//

#if !defined(_CConsole_Defined_)
#define _CConsole_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <conio.h>
#include <stdio.h>


class CConsole  
{
public:
    HANDLE hConsole;

public:
	CConsole(SHORT width=80, SHORT height=40, const char* title="console") 
    {
        hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
        setscreensize( width, height );
        settitle( title );
    }//CConsole()

	~CConsole() 
    {
        setcolor(7,0);
        showcursor(true);
    }//CConsole()

    void showcursor( bool bShow ) 
    {
        CONSOLE_CURSOR_INFO cci;

        GetConsoleCursorInfo( hConsole, &cci );
        cci.bVisible = bShow;
        SetConsoleCursorInfo( hConsole, &cci );
    }//showcursor()

    void settitle( const char* title ) 
    {
        SetConsoleTitleA( title );
    }//settitle()

    void setscreensize( SHORT width, SHORT height ) 
    {
        COORD size;

        size = GetLargestConsoleWindowSize( hConsole );
        size.X = width;
        size.Y = height;
        SetConsoleScreenBufferSize( hConsole, size );
    }//setscreensize()

    void gotoxy( SHORT x, SHORT y ) {
        COORD coord;

        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition( hConsole, coord );
    }//gotoxy()

    void setcolor( WORD fore, WORD back = 0 ) 
    {
        WORD attribute;
        attribute = (back & 0x0f) << 4 | (fore & 0x0f);
        SetConsoleTextAttribute( hConsole, attribute );

        /* predefined flags
        FOREGROUND_BLUE
        FOREGROUND_GREEN
        FOREGROUND_RED
        FOREGROUND_INTENSITY
        BACKGROUND_BLUE
        BACKGROUND_GREEN
        BACKGROUND_RED
        BACKGROUND_INTENSITY
        */
    }//setcolor()

};//class CConsole

#endif // !defined(_CConsole_Defined_)


/* working example:

#include <stdio.h>
#include <stdio.h>
#include "Console.h"

CConsole con;

void main()
{
    con.showcursor(false);    

    con.gotoxy(10,10);

    con.setcolor(2);
    printf("hello world\n");

    con.setcolor(15);
    printf("another world\n");

    getch();
}//main()

*/