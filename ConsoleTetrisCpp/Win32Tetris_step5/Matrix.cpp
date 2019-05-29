#include "stdafx.h"
#include "Matrix.h"
#include <math.h>

CMatrix::CMatrix()
{
    SetIdentity();
}//CMatrix::CMatrix()

CMatrix::~CMatrix()
{
}//CMatrix::~CMatrix()

float& CMatrix::operator()(int iRow, int iCol)
{
    return m_afElements[iRow][iCol];
}//CMatrix::operator()

CMatrix CMatrix::operator*(CMatrix& mRight)
{
    CMatrix mRet;

    mRet.SetZero();
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            for (int k=0; k<4; ++k)
            {
                mRet(i,j) += m_afElements[i][k] * mRight(k,j);
            }//for
        }//for
    }//for

    return mRet;
}//CMatrix::operator*()

CVector CMatrix::operator*(CVector& vLeft)
{
    CVector vRet;

    vRet.m_x = vLeft.m_x * m_afElements[0][0] +
               vLeft.m_y * m_afElements[0][1] +
               vLeft.m_z * m_afElements[0][2] +
               m_afElements[0][3];
    vRet.m_y = vLeft.m_x * m_afElements[1][0] +
               vLeft.m_y * m_afElements[1][1] +
               vLeft.m_z * m_afElements[1][2] +
               m_afElements[1][3];
    vRet.m_z = vLeft.m_x * m_afElements[2][0] +
               vLeft.m_y * m_afElements[2][1] +
               vLeft.m_z * m_afElements[2][2] +
               m_afElements[2][3];
    return vRet;
}//CMatrix::operator*()

CMatrix CMatrix::operator+(CMatrix& mRight)
{
    CMatrix mRet;

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            mRet(i,j) = m_afElements[i][j] + mRight(i,j);
        }//for
    }//for

    return mRet;
}//CMatrix::operator+()

CMatrix& CMatrix::operator=(CMatrix& mRight)
{
    memcpy( m_afElements, mRight.m_afElements, sizeof(m_afElements) );
    return *this;
}//CMatrix::operator=()

CMatrix CMatrix::SetZero()
{
    memset( m_afElements, 0, sizeof(m_afElements) );

    return *this;
}//CMatrix::SetZero()

CMatrix CMatrix::SetIdentity()
{
    SetZero();

    m_afElements[0][0] =
    m_afElements[1][1] =
    m_afElements[2][2] =
    m_afElements[3][3] = 1.f;

    return *this;
}//CMatrix::SetIdentity()

CMatrix CMatrix::SetRotationX(float fRadian)
{
    SetIdentity();
    m_afElements[1][1] = cosf(fRadian);
    m_afElements[1][2] = -sinf(fRadian);
    m_afElements[2][1] = sinf(fRadian);
    m_afElements[2][2] = cosf(fRadian);
    return *this;
}//CMatrix::SetRotationX()

CMatrix CMatrix::SetRotationY(float fRadian)
{
    SetIdentity();
    m_afElements[0][0] = cosf(fRadian);
    m_afElements[0][2] = sinf(fRadian);
    m_afElements[2][0] = -sinf(fRadian);
    m_afElements[2][2] = cosf(fRadian);
    return *this;
}//CMatrix::SetRotationY()

CMatrix CMatrix::SetRotationZ(float fRadian)
{
    SetIdentity();
    m_afElements[0][0] = cosf(fRadian);
    m_afElements[0][1] = -sinf(fRadian);
    m_afElements[1][0] = sinf(fRadian);
    m_afElements[1][1] = cosf(fRadian);
    return *this;
}//CMatrix::SetRotationZ()

CMatrix CMatrix::SetScale(float fxScale, float fyScale, float fzScale)
{
    SetIdentity();
    m_afElements[0][0] = fxScale;
    m_afElements[1][1] = fyScale;
    m_afElements[2][2] = fzScale;

    return *this;
}//CMatrix::SetScale()

CMatrix CMatrix::SetTranslation(float x, float y, float z)
{
    SetIdentity();
    m_afElements[0][3] = x;
    m_afElements[1][3] = y;
    m_afElements[2][3] = z;

    return *this;
}//CMatrix::SetTranslation()

CMatrix CMatrix::SetProjection(float d)
{
    SetZero();
    m_afElements[0][0] = d;
    m_afElements[1][1] = d;
    m_afElements[3][2] = 1;
    m_afElements[3][3] = d;

    return *this;
}//CMatrix::SetProjection
