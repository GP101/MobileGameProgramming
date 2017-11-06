#include "stdafx.h"
#include "KVector.h"
#include <math.h> // step1

KVector operator+( const KVector& left, const KVector& right )
{
    KVector t;
    t.m_x = left.m_x + right.m_x;
    t.m_y = left.m_y + right.m_y;
    t.m_z = left.m_z + right.m_z;
    return t;
}

KVector operator-( const KVector& left, const KVector& right )
{
    KVector t;
    t.m_x = left.m_x - right.m_x;
    t.m_y = left.m_y - right.m_y;
    t.m_z = left.m_z - right.m_z;
    return t;
}

KVector::KVector()
{
    m_x = 0;
    m_y = 0;
    m_z = 0;
}

KVector::~KVector()
{
}

KVector::KVector( float x, float y, float z )
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void KVector::Translate( float tx, float ty, float tz )
{
    m_x += tx;
    m_y += ty;
    m_z += tz;
}

void KVector::Scale( float sx, float sy, float sz )
{
    m_x *= sx;
    m_y *= sy;
    m_z *= sz;
}

KVector KVector::operator*( float fScale ) {
    KVector t( m_x * fScale, m_y * fScale, m_z * fScale );
    return t;
}

void KVector::RotateX( float theta )
{
    float tx,ty,tz;

    tx = m_x;
    ty = m_y*cosf(theta) - m_z*sinf(theta);
    tz = m_y*sinf(theta) + m_z*cosf(theta);

    m_y = ty;
    m_z = tz;
}

void KVector::RotateY( float theta )
{
    float tx,ty,tz;

    tx =  m_x*cosf(theta) + m_z*sinf(theta);
    ty =  m_y;
    tz = -m_x*sinf(theta) + m_z*cosf(theta);

    m_x = tx;
    m_z = tz;
}

void KVector::RotateZ( float theta )
{
    float tx,ty,tz;

    tx = m_x*cosf(theta) - m_y*sinf(theta);
    ty = m_x*sinf(theta) + m_y*cosf(theta);
    tz = m_z;

    m_x = tx;
    m_y = ty;
}

float KVector::Length() const {
    return sqrt( m_x*m_x + m_y*m_y + m_z*m_z );
}

void KVector::Normalize() {
    const float fLength = Length();
    m_x /= fLength;
    m_y /= fLength;
    m_z /= fLength;
}
