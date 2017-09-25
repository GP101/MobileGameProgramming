// ConsoleProject.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <iostream>
#include <windows.h>
#include <conio.h>

#pragma comment( lib, "winmm.lib" )

float g_fPseudoTestRange = 100.f;

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
    KMonster() : m_eMonsterState(IDLE)
    {
    }
    void OnFrameMove( float fElapsedTime_ );
    void OnFrameRender(){}

private:
    void _ProcessMonsterStateDiagram();
    void _RequestStateChange( const EMonsterState eNewState_
        , DWORD dwWaitMilliseconds_ )
    {
        m_eMonsterState = eNewState_;
    }//_RequestStateChange()
    void _ProcessState_IDLE();
    void _ProcessState_MOVING();
    void _ProcessState_ATTACK();

    bool _IsIdleTimerExpire() const { return false; }
    bool _IsTargetInRange( float fWithinDistance_ = 10.f ) const;

private:
    EMonsterState   m_eMonsterState;
    float           m_fIdleTimer;
    DWORD           m_dwIdleMotion;
    DWORD           m_dwTargetId;
    float           m_dwDistanceToTarget;
    float           m_fSpellGauge;
    DWORD           m_dwAttackMotion;
    DWORD           m_dwBboxId;
    float           m_fPrepareAttackTimer;
};//class KMonster


void KMonster::_ProcessMonsterStateDiagram()
{
    if( m_eMonsterState == IDLE )
        _ProcessState_IDLE();
    else if( m_eMonsterState == MOVING  )
        _ProcessState_MOVING();
    else if( m_eMonsterState == ATTACK )
        _ProcessState_ATTACK();
}//KMonster::_ProcessMonsterStateDiagram()

void KMonster::_ProcessState_IDLE()
{
    if( _IsIdleTimerExpire() == true )
        "Change idle motion";
    else if( _IsTargetInRange( 10 ) == true )
    {
        "Update spell gauge";
        _RequestStateChange( MOVING, 0 );
    }
}//KMonster::_ProcessState_IDLE()

void KMonster::_ProcessState_MOVING()
{
    "Approach to target";

    if( "target is out in range" )
        _RequestStateChange( IDLE, 0 );
    else if ( "bbox collide with" )
        _RequestStateChange( ATTACK, 2000 );
}//KMonster::_ProcessState_MOVING()

void KMonster::_ProcessState_ATTACK()
{
    "Blow a first to target";

    if( "target is out in range" )
        _RequestStateChange( IDLE, 0 );
}//KMonster::_ProcessState_ATTACK()

bool KMonster::_IsTargetInRange( float fWithinDistance_ ) const
{
    return g_fPseudoTestRange < fWithinDistance_;
}//KMonster::_IsTargetInRange()

void KMonster::OnFrameMove( float fElapsedTime_ )
{
    _ProcessMonsterStateDiagram();
    std::cout << fElapsedTime_
        << " State: "
        << m_eMonsterState
        << std::endl;
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
            if( ch == 27 )
                break;
            else if( ch == '1' )
                g_fPseudoTestRange = 9.f;
        }//if

        monster.OnFrameMove( dwElapsedTime / 1000.f );
        monster.OnFrameRender();

        const DWORD dwCurrentTime = timeGetTime();
        dwElapsedTime = dwCurrentTime - dwPreviousTime;
        dwPreviousTime = dwCurrentTime;
        Sleep( 500 );
    }//while
}//main()
