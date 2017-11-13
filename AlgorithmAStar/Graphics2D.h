/////////////////////////////////////////////////////////////////////////
// File: Graphics2D.h
// Desc: interface for the CGraphics2D class.
//       programmed by seojt@kogsoft.com on December 6, 2000
//       last modified on December 10, 2000
//
#if !defined(_CGraphics2D_Defined_)
#define _CGraphics2D_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
//#include "t3dlib1.h"

// a 2D vertex
typedef struct VERTEX2DF_TYP
{
    float x,y; // the vertex
} VERTEX2DF, *VERTEX2DF_PTR; /**/

// a 2D polygon
typedef struct POLYGON2D_TYP
{
    int         state;    // state of polygon
    int         num_verts;// number of vertices
    int         x0, y0;   // position of center of polygon  
    int         xv, yv;   // initial velocity
    DWORD       color;    // could be index or PALETTENTRY
    VERTEX2DF*  vlist;    // pointer to vertex list
} POLYGON2D, *POLYGON2D_PTR; /**/

/////////////////////////////////////////////////////////////
// Name: class CGraphics2D
// Desc: first implemented for terrain Field Of View control
//       draw simple shape to destinaton board(or image)
class CGraphics2D
{
    // PutPixel callback function
    void    (*m_fpPutPixel)(int x, int y, int value);
    // MemSet callback function
    void    (*m_fpMemSet)(int offset, int value); // obsolete!!!

    // clipping rectangle 
    int     m_min_clip_x, 
            m_max_clip_x,                  
            m_min_clip_y,     
            m_max_clip_y;

    int     m_width;       // width of image

private:
	void _DrawHLine(int x, int y, int width, int value); // draw horizontal line
    void _CirclePlotPoints(int xCenter, int yCenter, int x, int y, int value);
	int  _IsPointIn(int x, int y);  // is point in clipping region?

public:
    // static member functions
    static int   FastDistance2D(int x, int y);
    static float FastDistance3D(float fx, float fy, float fz);

public:

    // writer
#if defined(_DEBUG)
    void PutPixel(int x, int y, int value);
    void MemSet(int offset, int value);     // used only in DrawFilledPolygon2D()
#else
    inline void PutPixel(int x, int y, int value) { m_fpPutPixel(x, y, value); }
    inline void MemSet(int offset, int value) { m_fpMemSet(offset, value); }
#endif // defined(_DEBUG)

    void Bresenham(int xStart, int yStart, int xEnd, int yEnd, int value);  // draw line
	void Rectangle(int x1, int y1, int x2, int y2, int value);              // draw rectangle
    void CircleMidPoint(int xCenter, int yCenter, int radius, int value);

    void DrawTopTri(int x1,int y1, int x2,int y2, int x3,int y3, int value);
    void DrawBottomTri(int x1,int y1, int x2,int y2, int x3,int y3, int value);
    void DrawTriangle2D(int x1,int y1, int x2,int y2, int x3,int y3, int value );
    void DrawFilledPolygon2D(POLYGON2D_PTR poly);

    // helper
	void SetCallback( void (*fpPutPixel)(int, int, int), 
                      void (*fpMemSet)(int, int) = NULL );
	void SetMemSetCallback( void (*fp)(int, int) ); // obsolete!!!
    void SetPutPixelCallback( void (*fp)(int, int, int) );
    void SetMemPitch(int pitch) { m_width = pitch; }
	void SetClipRect(int x_min, int y_min, int x_max, int y_max);
    int  ClipLine(int& x1,int& y1,int& x2, int& y2);    // most internally used
    
    // constructor and destructor
	CGraphics2D();
    CGraphics2D( void (*fpPutPixel)(int, int, int) );
	virtual ~CGraphics2D();
};// class CGraphics2D

#endif // !defined(_CGraphics2D_Defined_)

/* you can use like this:

    #include <stdio.h>
    #include <memory.h>
    #include <conio.h>
    #include "StringEx.h"
    #include "Fov2D.h"

    #define CLIP_X1     10 
    #define CLIP_Y1     7
    #define CLIP_X2     60
    #define CLIP_Y2     15

    int board[24][79];


    void PutPixelCallback(int x, int y, int value)
    {   // you don't need to clip test. it's clipped
        board[y][x] = value;
    }//PutPixelCallback()

    void MemSetCallback(int offset, int value)
    {
        if (offset < 79*24) // clipping test
            *(board[0] + offset) = value;
    }//MemSetCallback()

    void InitBoard()
    {
        memset( board, '.', sizeof(board) );
    }//InitBoard()

    void DrawBoard()
    {
        int         row, col;
        CStringEx   str;

        for (row=0; row<24; ++row) {
            str = "";
            for (col=0; col<79; ++col) {
                str += board[row][col];
            }//for
            printf("%s\n", str.GetBuffer() );
        }//for
    }//DrawBoard()

    void main()
    {
        InitBoard();
    
        CGraphics2D fov;
        fov.SetCallback( PutPixelCallback, MemSetCallback );
        fov.SetMemPitch( 79 );

        fov.SetClipRect(CLIP_X1, CLIP_Y1, CLIP_X2, CLIP_Y2);
        fov.Rectangle(CLIP_X1, CLIP_Y1, CLIP_X2, CLIP_Y2, 'o');

        fov.SetClipRect(0, 0, 78, 23);
        fov.DrawTriangle2D(2,2, 40,10, 20,21, 'T');


        VERTEX2DF   vertex[6]={{15,-5},{5,-5},{0,-11},{-5,-5},{-15,-5},{0,10}};
        POLYGON2D   poly={0, 6, 39,12, 0,0, 'R', NULL};
          //              ^state
          //                 ^num verts
          //                    ^center
          //                           ^direction
          //                                ^color
          //                                   ^vertex list

        poly.vlist = vertex;

        fov.DrawFilledPolygon2D(&poly);

        DrawBoard();
    }//main()
*/