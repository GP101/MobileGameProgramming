#if !defined(AFX_MATRIX_H__B51863C1_1ABB_11D7_B10E_444553540001__INCLUDED_)
#define AFX_MATRIX_H__B51863C1_1ABB_11D7_B10E_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"

//{{ seojt: step3
class CMatrix
//}} seojt: step3
{
public:
    float   m_afElements[4][4];

	CMatrix();
	~CMatrix();

    /// access matrix element (iRow,iCol)
    /// @param  iRow: row index
    /// @param  iCol: column index
    /// @return reference of element (iRow,iCol)
    float& operator()(int iRow, int iCol);
    CMatrix operator*(CMatrix& mRight);
    CVector operator*(CVector& vRight);
    CMatrix operator+(CMatrix& mRight);
    CMatrix& operator=(CMatrix& mRight);

    CMatrix SetZero();
    CMatrix SetIdentity();
    CMatrix SetRotationX(float fRadian);
    CMatrix SetRotationY(float fRadian);
    CMatrix SetRotationZ(float fRadian);
    CMatrix SetScale(float fxScale, float fyScale, float fzScale);
    CMatrix SetTranslation(float x, float y, float z);

    CMatrix SetProjection(float d);
};//class CMatrix


#endif // !defined(AFX_MATRIX_H__B51863C1_1ABB_11D7_B10E_444553540001__INCLUDED_)
