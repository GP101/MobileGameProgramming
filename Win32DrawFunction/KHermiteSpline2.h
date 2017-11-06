#pragma once

#include "KHermiteCurve.h"
#include "KVector.h"


///-----------------------------------------------------------------------------
/// @class  KHermiteSpline2
/// @brief  2-dimensional Hermite spline
/// @see    KHermiteCurve
///
class KHermiteSpline2
{
private:
    KHermiteCurve   m_aHermiteCurve[2];

public:
    KHermiteSpline2(){}
    KHermiteSpline2( const KVector& p0, const KVector& p1, const KVector& dp0, const KVector& dp1 )
    {
        m_aHermiteCurve[ 0 ].Construct( p0.m_x, p1.m_x, dp0.m_x, dp1.m_x );
        m_aHermiteCurve[ 1 ].Construct( p0.m_y, p1.m_y, dp0.m_y, dp1.m_y );
    }

    ~KHermiteSpline2(){}

    void Construct( const KVector& p0, const KVector& p1, const KVector& dp0, const KVector& dp1 )
    {
        m_aHermiteCurve[ 0 ].Construct( p0.m_x, p1.m_x, dp0.m_x, dp1.m_x );
        m_aHermiteCurve[ 1 ].Construct( p0.m_y, p1.m_y, dp0.m_y, dp1.m_y );
    }

    const KVector GetPosition( float u_ ) const
    {
        return KVector( m_aHermiteCurve[ 0 ].CalculateX( u_ ), m_aHermiteCurve[ 1 ].CalculateX( u_ ), 0.f );
    }

    const KVector GetTangent( float u_ ) const
    {
        return KVector( m_aHermiteCurve[ 0 ].CalculateDxDu( u_ ), m_aHermiteCurve[ 1 ].CalculateDxDu( u_ ), 0.f );
    }

    const KVector GetCurvature( float u_ ) const
    {
        return KVector( m_aHermiteCurve[ 0 ].CalculateDx2D2u( u_ ), m_aHermiteCurve[ 1 ].CalculateDx2D2u( u_ ), 0.f );
    }
};//class KHermiteSpline2
