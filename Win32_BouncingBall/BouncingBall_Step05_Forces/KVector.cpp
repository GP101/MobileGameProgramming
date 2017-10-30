#include "stdafx.h"
#include "KVector.h"
#include <math.h> // step1

KVector operator+( const KVector& left, const KVector& right ) {
    KVector t;
    t.x = left.x + right.x;
    t.y = left.y + right.y;
    return t;
}

KVector operator-( const KVector& left, const KVector& right ) {
    KVector t;
    t.x = left.x - right.x;
    t.y = left.y - right.y;
    return t;
}

KVector operator*( const float fScalar, const KVector& right ) {
    KVector v = right;
    v.Scale( fScalar, fScalar );
    return v;
}

KVector operator*( const KVector& left, const float fScalar ) {
    KVector v = left;
    v.Scale( fScalar, fScalar );
    return v;
}

KVector::KVector()
{
    x = 0;
    y = 0;
}

KVector::~KVector()
{
}

KVector::KVector( float x_, float y_ )
{
    x = x_;
    y = y_;
}//KVector::KVector()

void KVector::Translate( float tx, float ty )
{
    x += tx;
    y += ty;
}//KVector::Translate()

void KVector::Scale( float sx, float sy )
{
    x *= sx;
    y *= sy;
}//KVector::Scale()

float KVector::Length() const {
    return sqrt( x*x + y*y );
}

void KVector::Normalize() {
    const float fLength = Length();
    x /= fLength;
    y /= fLength;
}
