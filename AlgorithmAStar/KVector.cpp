#include "KVector.h"
#include <math.h> // step1

KVector operator+( const KVector& left, const KVector& right ) {
    KVector t;
    t.x = left.x + right.x;
    t.y = left.y + right.y;
    t.z = left.z + right.z;
    return t;
}

KVector operator-( const KVector& left, const KVector& right ) {
    KVector t;
    t.x = left.x - right.x;
    t.y = left.y - right.y;
    t.z = left.z - right.z;
    return t;
}

KVector operator*( const float fScalar, const KVector& right ) {
    KVector v = right;
    v.Scale( fScalar, fScalar, fScalar );
    return v;
}

KVector operator*( const KVector& left, const float fScalar ) {
    KVector v = left;
    v.Scale( fScalar, fScalar, fScalar );
    return v;
}

bool operator==( const KVector& left, const KVector& right ) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

float Dot( const KVector& left, const KVector& right ) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}//Dot()

KVector Cross( const KVector& P, const KVector& Q ) {
    KVector t( P.y*Q.z-P.z*Q.y, P.z*Q.x-P.x*Q.z, P.x*Q.y-P.y*Q.x);
    return t;
}//Cross()

// constructor.
KVector::KVector()
{
    x = 0;
    y = 0;
    z = 0;
}//KVector::KVector()

KVector::~KVector()
{
}//KVector::~KVector()

KVector::KVector( float x_, float y_, float z_ )
{
    x = x_;
    y = y_;
    z = z_;
}//KVector::KVector()

//KVector::KVector( const KVector& lhs ) {
//    operator=( lhs );
//}
//
//KVector& KVector::operator=( const KVector& lhs ) {
//    if( &lhs == this )
//        return *this;
//    x = lhs.x;
//    y = lhs.y;
//    z = lhs.z;
//    return *this;
//}

void KVector::Translate( float tx, float ty, float tz )
{
    x += tx;
    y += ty;
    z += tz;
}//KVector::Translate()

void KVector::Scale( float sx, float sy, float sz )
{
    x *= sx;
    y *= sy;
    z *= sz;
}//KVector::Scale()

//KVector KVector::operator*( float fScale ) {
//    KVector t( x * fScale, y * fScale, z * fScale );
//    return t;
//}//KVector::Scale()

void KVector::RotateX( float theta )
{
    float tx,ty,tz;

    tx = x;
    ty = y*cosf( theta ) - z*sinf( theta );
    tz = y*sinf( theta ) + z*cosf( theta );

    y = ty;
    z = tz;
}//KVector::RotateX()

void KVector::RotateY( float theta )
{
    float tx,ty,tz;

    tx = x*cosf( theta ) + z*sinf( theta );
    ty = y;
    tz = -x*sinf( theta ) + z*cosf( theta );

    x = tx;
    z = tz;
}//KVector::RotateY()

void KVector::RotateZ( float theta )
{
    float tx,ty,tz;

    tx = x*cosf( theta ) - y*sinf( theta );
    ty = x*sinf( theta ) + y*cosf( theta );
    tz = z;

    x = tx;
    y = ty;
}//KVector::RotateZ()

float KVector::Length() const {
    return sqrt( x*x + y*y + z*z );
}

void KVector::Normalize() {
    const float fLength = Length();
    x /= fLength;
    y /= fLength;
    z /= fLength;
}

