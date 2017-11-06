#pragma once

class KVector
{
public:
    friend KVector operator+( const KVector& left, const KVector& right );
    friend KVector operator-( const KVector& left, const KVector& right );
    KVector operator*( const float fScale );

    KVector();
    ~KVector();
    KVector( float x, float y, float z );

    void Translate(float tx, float ty, float tz);
    void Scale(float sx, float sy, float sz);
    void RotateX(float theta);
    void RotateY(float theta);
    void RotateZ(float theta);
    float Length() const;
    void Normalize();
public:
    float m_x;
    float m_y;
    float m_z;
};//class KVector
