// Win32DrawFunction.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32DrawFunction.h"
#include "KVector.h"
#include "KHermiteSpline2.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[ MAX_LOADSTRING ];					// The title bar text
TCHAR szWindowClass[ MAX_LOADSTRING ];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass( HINSTANCE hInstance );
BOOL				InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadString( hInstance, IDC_WIN32DRAWFUNCTION, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Perform application initialization:
    if( !InitInstance( hInstance, nCmdShow ) )
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_WIN32DRAWFUNCTION ) );

    // Main message loop:
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof( WNDCLASSEX );

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_WIN32DRAWFUNCTION ) );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = MAKEINTRESOURCE( IDC_WIN32DRAWFUNCTION );
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    return RegisterClassEx( &wcex );
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    HWND hWnd;

    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL );

    if( !hWnd )
    {
        return FALSE;
    }

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    return TRUE;
}

float Function( float x )
{
    return 0.1*x*x;
}

float FPrime( float x )
{
    KVector v1;
    KVector v2;
    v1.m_x = x;
    v1.m_y = Function( v1.m_x );

    v2.m_x = x + 0.0001f;
    v2.m_y = Function( v2.m_x );

    KVector vDir = v2 - v1;
    return vDir.m_y / vDir.m_x;
}

void DrawLine( HDC hdc, int x1, int y1, int x2, int y2 )
{
    MoveToEx( hdc, x1, y1, NULL );
    LineTo( hdc, x2, y2 );
}

void DrawLine( HDC hdc, const KVector& v1, const KVector& v2 )
{
    DrawLine( hdc, v1.m_x, v1.m_y, v2.m_x, v2.m_y );
}

void OnPaint( HDC hdc )
{
    // draw Hermite spline
    //
    KHermiteSpline2     spline;
    KVector p1( 100, 200, 0 );
    KVector p2( 400, 200, 0 );
    KVector r1( 1000, 1000, 0 );
    KVector r2( 1000, 1000, 0 );
    spline.Construct( p1, p2, r1, r2 );

    KVector prev;
    KVector curr;
    float t = 0.f;
    const float fStep = 0.1f;
    const int steps = int( 1.f / fStep );
    prev = spline.GetPosition( 0 );
    for( int i = 0; i <= steps; ++i, t += fStep )
    {
        curr = spline.GetPosition( t );
        DrawLine( hdc, prev, curr );
        prev = curr;
    }//for

    SelectObject( hdc, GetStockObject( DC_PEN ) );
    SetDCPenColor( hdc, RGB( 255, 0, 0 ) );
    KVector q1 = p1 + r1 * 0.25;
    KVector q2 = p2 + r2 * 0.25;
    DrawLine(hdc, p1, q1);
    DrawLine(hdc, p2, q2);

    const float u = 0.5f;
    KVector position = spline.GetPosition(u);
    KVector tangent = spline.GetTangent(u);
    DrawLine(hdc, position, position + tangent * 100.f);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
    case WM_COMMAND:
        wmId = LOWORD( wParam );
        wmEvent = HIWORD( wParam );
        // Parse the menu selections:
        switch( wmId )
        {
        case IDM_ABOUT:
            DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
            break;
        case IDM_EXIT:
            DestroyWindow( hWnd );
            break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint( hWnd, &ps );
        // TODO: Add any drawing code here...
        OnPaint( hdc );
        EndPaint( hWnd, &ps );
        break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    switch( message )
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
