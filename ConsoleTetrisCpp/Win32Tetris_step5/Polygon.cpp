#include "stdafx.h"
#include "Polygon.h"

void DrawIndexedPrimitive( HDC hdc,
        int*        m_indexBuffer,      // index buffer
        int         primitiveCounter,   // primitive counter
        CVector*    m_vertexBuffer )    // vertex buffer
{
    int   i1, i2;
    int   counter = 0;

    for (int i=0; i<primitiveCounter; ++i)
    {
		// get index
        i1 = m_indexBuffer[counter];
        i2 = m_indexBuffer[counter+1];

		// draw line
        MoveToEx( hdc, int(m_vertexBuffer[i1].m_x),
            int(m_vertexBuffer[i1].m_y), NULL );
        LineTo( hdc, int(m_vertexBuffer[i2].m_x),
            int(m_vertexBuffer[i2].m_y) );

		// advance to next primitive
        counter += 2;
    }//for
}//DrawIndexedPrimitive()


CPolygon::CPolygon()
{
    m_sizeIndex  = 0;
    m_sizeVertex = 0;
}//CPolygon::CPolygon()

CPolygon::~CPolygon()
{
}//CPolygon::~CPolygon()

void CPolygon::SetIndexBuffer()
{
    //{{ seojt: step3
    const int numVertices = 24;
    int buffer[] = {0,1,
                    1,2,
                    2,3,
                    3,0,
                    4,5,
                    5,6,
                    6,7,
                    7,4,
                    0,4,
                    1,5,
                    2,6,
                    3,7,
    };
    for (int i = 0; i<numVertices; ++i)
    {
        m_indexBuffer[i] = buffer[i];
    }//for
    m_sizeIndex = numVertices;
    //}} seojt: step3
}//CPolygon::SetIndexBuffer()

void CPolygon::SetVertexBuffer()
{
    //{{ seojt: step3
    m_vertexBuffer[0] = CVector(-16.f, 16.f, -16.f);
    m_vertexBuffer[1] = CVector(-16.f, -16.f, -16.f);
    m_vertexBuffer[2] = CVector(16.f, -16.f, -16.f);
    m_vertexBuffer[3] = CVector(16.f, 16.f, -16.f);
    m_vertexBuffer[4] = CVector(-16.f, 16.f, 16.f);
    m_vertexBuffer[5] = CVector(-16.f, -16.f, 16.f);
    m_vertexBuffer[6] = CVector(16.f, -16.f, 16.f);
    m_vertexBuffer[7] = CVector(16.f, 16.f, 16.f);
    //}} seojt: step3
    m_sizeVertex = 8;
}//CPolygon::SetVertexBuffer()

void CPolygon::Render(HDC hdc)
{
    //{{ seojt: step3
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        m_sizeIndex/2,                  // primitive counter
        m_vertexBuffer );   // vertex buffer
    //}} seojt: step3
}//CPolygon::Render()

//{{ seojt: step2
void CPolygon::RotateX(float theta)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].RotateX(theta);
    }//for
}//CPolygon::RotateX(float theta)

void CPolygon::RotateY(float theta)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].RotateY(theta);
    }//for
}//CPolygon::RotateY(float theta)

void CPolygon::Translate(float tx, float ty, float tz)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].Translate(tx,ty,tz);
    }//for
}//CPolygon::Translate(float tx, float ty, float tz)
//}} seojt: step2


//{{ seojt: step3
void CPolygon::Transform(CMatrix& mat)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i] = mat * m_vertexBuffer[i];
    }//for
}//CPolygon::Transform()
//}} seojt: step3

//{{ seojt: step3
void CPolygon::Projection(CMatrix& mat)
{
    float d = mat(0,0);
    float z;

    for (int i=0; i<m_sizeVertex; ++i)
    {
        z = m_vertexBuffer[i].m_z;
        m_vertexBuffer[i] = mat * m_vertexBuffer[i];
        m_vertexBuffer[i].m_x /= (z+d);
        m_vertexBuffer[i].m_y /= (z+d);
    }//for
}//CPolygon::Projection()
//}} seojt: step3

//{{ seojt: step3
void CPolygon::ApplyViewport( float xOrigin, float yOrigin ) {
    for( int i = 0; i<m_sizeVertex; ++i ) {
        m_vertexBuffer[ i ].m_x += xOrigin;
        m_vertexBuffer[ i ].m_y += yOrigin;
    }//for
}//CPolygon::ApplyViewport()
//}} seojt: step3
