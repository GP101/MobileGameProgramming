// ConsoleProject.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <iostream>
#include <windows.h>
#include <conio.h>

#pragma comment( lib, "winmm.lib" )

class KMonster
{
public:
    enum EMonsterState
    {
        IDLE,
        MOVING,
        ATTACK,
    };//enum EMonsterState
    
public:
    void OnFrameMove( float fElapsedTime_ );
    void OnFrameRender(){}

private:
    void _ProcessMonsterStateDiagram();

private:
    EMonsterState   m_eMonsterState;
};//class KMonster


void KMonster::_ProcessMonsterStateDiagram()
{
}//void KMonster::_ProcessMonsterStateDiagram()

void KMonster::OnFrameMove( float fElapsedTime_ )
{
    _ProcessMonsterStateDiagram();
    std::cout << fElapsedTime_ << std::endl;
}//KMonster::OnFrameMove()

void main(void)
{
    KMonster    monster;
    DWORD       dwPreviousTime = 0L;
    DWORD       dwElapsedTime = 0L;
    dwPreviousTime = timeGetTime();

    while( true )
    {
        if( _kbhit() )
        {
            const int ch = _getch();
            if( ch == 27 ) break;
        }//if

        monster.OnFrameMove( dwElapsedTime / 1000.f );
        monster.OnFrameRender();

        const DWORD dwCurrentTime = timeGetTime();
        dwElapsedTime = dwCurrentTime - dwPreviousTime;
        dwPreviousTime = dwCurrentTime;
        Sleep( 500 );
    }//while
}//main()
