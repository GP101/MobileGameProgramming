#pragma once
#include "ConsoleUtil.h"

#define BOARD_SIZE_ROW      20
#define BOARD_SIZE_COL      40
#define MAX_BLOCK_SIZE      6

struct KPoint
{
    int x;
    int y;
};

struct KBlock
{
    KPoint              m_aBlockPosition[MAX_BLOCK_SIZE];   ///< stone positions(local space)
    int                 m_iNumStone;    ///< number of stones.
    KPoint              m_worldPos;     ///< World position of the Block

    KPoint GetPos( int iStone_ ) const
    {
        KPoint p;
        p.x = m_aBlockPosition[iStone_].x + m_worldPos.x;
        p.y = m_aBlockPosition[iStone_].y + m_worldPos.y;
        return p;
    }

    void CopyFrom( const KBlock& rhs_ )
    {
        memcpy( this, &rhs_, sizeof( KBlock ) );
    }
};

class KBoard
{
public:
    static bool IsValidBoardPos( const KPoint worldPos_ )
    {
        if( worldPos_.y >= 0 && worldPos_.y < BOARD_SIZE_ROW
            && worldPos_.x >= 0 && worldPos_.x < BOARD_SIZE_COL ) {
            return true;
        }

        return false;
    }

public:
    void Initialize();
    void CopyFrom( const KBoard& rhs_ );
    bool GetBoardCellValue( int* pCellValue_, const KPoint worldPos_ ) const;
    void LocateBlock( const KBlock& block_ );
    bool CanLocateBlock( const KBlock& block_ ) const;

private:
    int                 m_aBoard[BOARD_SIZE_ROW][BOARD_SIZE_COL]; // Working board used in current frame.
};
