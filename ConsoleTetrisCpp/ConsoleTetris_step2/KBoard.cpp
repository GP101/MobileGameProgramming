#include "KBoard.h"


void KBoard::Initialize()
{
    ::memset((char*)m_aBoard, 0, BOARD_SIZE_ROW * BOARD_SIZE_COL * sizeof(BOARD_CELL_TYPE));
}//KBoard::Initialize()


void KBoard::CopyFrom(const KBoard& rhs_)
{
    memcpy(m_aBoard, rhs_.m_aBoard, sizeof(m_aBoard));
}//KBoard::CopyFrom()


/// @brief  get cell value of the given World point from the 'm_aCurBoard[]'
bool KBoard::GetBoardCellValue(OUT char* pCellValue_, const KPoint worldPos_) const
{
    if (IsValidBoardPos(worldPos_) == true)
    {
        // set [out] parameter
        *pCellValue_ = m_aBoard[worldPos_.y][worldPos_.x];
        return true;
    }//if
    return false;
}//KBoard::GetBoardCellValue()


void KBoard::LocateBlock(const KBlock& block_)
{
    for (int iStone = 0; iStone < block_.m_iNumStone; ++iStone)
    {
        // transform local coordinate to global coordinate
        const KPoint worldPos = block_.GetPos(iStone);
        // draw a block on the board
        const bool isValid = IsValidBoardPos(worldPos);
        if (isValid == true)
        {
            m_aBoard[worldPos.y][worldPos.x] = '#';
        }//if
    }//for
}//KBoard::LocateBlock()


/// @brief  check whether this Block can be located on the m_aCurBoard[] or not.
bool KBoard::CanLocateBlock(const KBlock& block_) const
{
    for (int iStone = 0; iStone < block_.m_iNumStone; ++iStone)
    {
        const KPoint worldPos = block_.GetPos(iStone);
        char cellValue = 0;
        const bool bIsGetValue = GetBoardCellValue(OUT &cellValue, worldPos);
        if (bIsGetValue == false || cellValue != 0)
            return false;
    }//for
    return true;
}//KBoard::CanLocateBlock()


/// @brief  draw Board on the console Window.
void KBoard::DrawBoard() const
{
    for (int y = 0; y < BOARD_SIZE_ROW; ++y)
    {
        for (int x = 0; x < BOARD_SIZE_COL; ++x)
        {
            SetCursorPosition(x, y);
            printf("%c", m_aBoard[y][x]);
        }//for
    }//for
}//KBoard::DrawBoard()
