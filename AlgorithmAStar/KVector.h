#if !defined(AFX_VECTOR_H__ED0A4977_9BC3_11D5_82EE_008048E10474__INCLUDED_)
#define AFX_VECTOR_H__ED0A4977_9BC3_11D5_82EE_008048E10474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KVector
{
public:
    friend KVector operator+( const KVector& left, const KVector& right );
    friend KVector operator-( const KVector& left, const KVector& right );
    //{{ step4
    friend KVector operator*( const float fScalar, const KVector& right );
    friend KVector operator*( const KVector& left, const float fScalar );
    //}} step4
    friend bool operator==( const KVector& left, const KVector& right );
    //friend KVector operator*( const float fScale );
    friend float Dot( const KVector& left, const KVector& right );
    friend KVector Cross( const KVector& left, const KVector& right );

    KVector();
    //KVector( const KVector& lhs );
    virtual ~KVector();
    KVector( float x, float y, float z );
    //KVector& operator=( const KVector& lhs );

    void Translate(float tx, float ty, float tz);
    void Scale(float sx, float sy, float sz);
    void RotateX(float theta);
    void RotateY(float theta);
    void RotateZ(float theta);
    float Length() const;
    void Normalize();
public:
    float x;
    float y;
    float z;
};//class KVector

#endif // !defined(AFX_VECTOR_H__ED0A4977_9BC3_11D5_82EE_008048E10474__INCLUDED_)
