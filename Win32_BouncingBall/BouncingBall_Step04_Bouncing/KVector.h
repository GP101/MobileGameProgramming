#pragma once

class KVector
{
public:
    friend KVector operator+( const KVector& left, const KVector& right );
    friend KVector operator-( const KVector& left, const KVector& right );
    friend KVector operator*( const float fScalar, const KVector& right );
    friend KVector operator*( const KVector& left, const float fScalar );

    KVector();
    virtual ~KVector();
    KVector( float x, float y );

    void Translate(float tx, float ty);
    void Scale(float sx, float sy);
    float Length() const;
    void Normalize();

public:
    float x;
    float y;
};//class KVector
