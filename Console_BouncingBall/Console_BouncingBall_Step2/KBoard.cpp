#include "KBoard.h"


void KBoard::Initialize()
{
    ::memset( (char*)m_aBoard, 0, BOARD_SIZE_ROW * BOARD_SIZE_COL * sizeof( int ) );
}

void KBoard::CopyFrom( const KBoard& rhs_ )
{
    memcpy( m_aBoard, rhs_.m_aBoard, sizeof( m_aBoard ) );
}

bool KBoard::GetBoardCellValue( int* pCellValue_, const KPoint worldPos_ ) const
{
    if( IsValidBoardPos( worldPos_ ) == true ) {
        // set [out] parameter
        *pCellValue_ = m_aBoard[worldPos_.y][worldPos_.x];
        return true;
    }//if
    return false;
}

void KBoard::LocateBlock( const KBlock& block_ )
{
    for( int iStone = 0; iStone < block_.m_iNumStone; ++iStone ) {
        // transform local coordinate to global coordinate
        const KPoint worldPos = block_.GetPos( iStone );
        // draw a block on the board
        const bool isValid = IsValidBoardPos( worldPos );
        if( isValid == true ) {
            m_aBoard[worldPos.y][worldPos.x] = '#';
        }//if
    }//for
}

bool KBoard::CanLocateBlock( const KBlock& block_ ) const
{
    for( int iStone = 0; iStone < block_.m_iNumStone; ++iStone ) {
        const KPoint worldPos = block_.GetPos( iStone );
        int cellValue = 0;
        const bool bIsGetValue = GetBoardCellValue( OUT &cellValue, worldPos );
        if( bIsGetValue == false || cellValue != 0 )
            return false;
    }//for
    return true;
}
