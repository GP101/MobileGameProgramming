#if !defined(AFX_POLYGON_H__ED0A4978_9BC3_11D5_82EE_008048E10474__INCLUDED_)
#define AFX_POLYGON_H__ED0A4978_9BC3_11D5_82EE_008048E10474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"
//{{ seojt: step3
#include "Matrix.h"
//}} seojt: step3

class CPolygon
{
public:
	CPolygon();
	virtual ~CPolygon();

    int     m_indexBuffer[100];
    int     m_sizeIndex;
    CVector m_vertexBuffer[100];
    int     m_sizeVertex;

    void SetIndexBuffer();
    void SetVertexBuffer();
    void Render(HDC hdc);

    //{{ seojt: step2
    void RotateX(float theta);
    void RotateY(float theta);
    void Translate(float tx, float ty, float tz);
    //}} seojt: step2

    //{{ seojt: step3
    void Transform(CMatrix& mat);
    void Projection(CMatrix& mat);
    void ApplyViewport( float xOrigin, float yOrigin );
    //}} seojt: step3
};//class CPolygon

#endif // !defined(AFX_POLYGON_H__ED0A4978_9BC3_11D5_82EE_008048E10474__INCLUDED_)
