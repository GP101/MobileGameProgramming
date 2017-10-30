// BouncingBall_Steps.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BouncingBall_Steps.h"
#include <mmsystem.h>
#include "KVector.h"

#pragma comment( lib, "winmm.lib" )

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE               g_hInstance; // current instance
HWND                    g_hwndMain = NULL;
HDC                     g_hdc = NULL;
HBITMAP                 g_hBitmap = NULL;
RECT                    g_clientRect;
KVector                 g_pos( 0, 0 );
KVector                 g_velocity( 100, 100 );
KVector                 g_acceleration( 0, 0 );
float                   g_forceFactor = 1000.f;

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

KVector UpdateVelocity( const KVector& v0, const KVector& a, float fTime )
{
    KVector v = v0 + a * fTime;
    return v;
}

KVector UpdatePosition( const KVector& s0, const KVector& v0, const KVector& a, float fTime )
{
    KVector s = s0 + v0 * fTime + a * fTime * fTime * 0.5f;
    return s;
}

void OnPaint( HDC hdc_, float fElapsedTime_ )
{
    WCHAR wszText[1024] = L"";
    wsprintf( wszText, L"Hello" );
    const int iLen = wcslen( wszText );

    g_velocity = UpdateVelocity( g_velocity, g_acceleration, fElapsedTime_ );
    g_pos = UpdatePosition( g_pos, g_velocity, g_acceleration, fElapsedTime_ );
    g_acceleration = KVector( 0, 0 );
    g_velocity = g_velocity - g_velocity * fElapsedTime_; // simulate friction

    int iLeft =  g_pos.x;
    int iTop = g_pos.y;
    int iWidth = 200;
    int iHeight = 100;
    int iCenterX = iLeft + iWidth / 2;
    int iCenterY = iTop + iHeight / 2;

    if( iLeft < 0 || (iLeft + iWidth) > g_clientRect.right )
        g_velocity.x = -g_velocity.x;
    else if( iTop < 0 || (iTop + iHeight) > g_clientRect.bottom )
        g_velocity.y = -g_velocity.y;

    ::Ellipse( hdc_, iLeft, iTop, iLeft + iWidth, iTop + iHeight );
    ::TextOut( hdc_, iCenterX, iCenterY, wszText, iLen );
}

void CreateResource()
{
    ::GetClientRect( g_hwndMain, &g_clientRect );
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    HDC hdc = ::GetDC( g_hwndMain );
    g_hdc = CreateCompatibleDC( hdc );
    g_hBitmap = CreateCompatibleBitmap( g_hdc, iWidth, iHeight );
    SelectObject( g_hdc, g_hBitmap );
}

void DestroyResource()
{
    if( g_hdc != NULL ) {
        DeleteDC( g_hdc );
        g_hdc = 0;
    }//if
    if( g_hBitmap != NULL ) {
        DeleteObject( g_hBitmap );
        g_hBitmap = 0;
    }//if
}

void OnSize()
{
    DestroyResource();
    CreateResource();
}

void OnUpdate()
{
    if( ::GetAsyncKeyState( VK_LEFT ) ) {
        g_acceleration = g_acceleration + KVector( -g_forceFactor, 0 );
    }//if
    if( ::GetAsyncKeyState( VK_RIGHT ) ) {
        g_acceleration = g_acceleration + KVector( g_forceFactor, 0 );
    }//if
    if( ::GetAsyncKeyState( VK_UP ) ) {
        g_acceleration = g_acceleration + KVector( 0, -g_forceFactor );
    }//if
    if( ::GetAsyncKeyState( VK_DOWN ) ) {
        g_acceleration = g_acceleration + KVector( 0, g_forceFactor );
    }//if
}

void OnIdle( float fElapsedTime_ )
{
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    HDC hdc = ::GetDC( g_hwndMain );

    HBRUSH brush;
    brush = CreateSolidBrush( RGB( 255, 255, 255 ) );
    SelectObject( g_hdc, brush );
    Rectangle( g_hdc, 0, 0, iWidth, iHeight );
    Sleep( 10 );

    OnUpdate();
    OnPaint( g_hdc, fElapsedTime_ / 1000.f );

    BitBlt( hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY );
    DeleteObject( brush );

    ::ReleaseDC( g_hwndMain, hdc );
}//OnIdle()

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BOUNCINGBALL_STEPS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOUNCINGBALL_STEPS));

    DWORD dwOldTime = ::timeGetTime();
	while( true )
	{
        ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
        if( msg.message == WM_QUIT )
            break;

        const DWORD dwNewTime = ::timeGetTime();
        const BOOL bIsTranslateMessage = TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
		if( bIsTranslateMessage == FALSE )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}//if

        OnIdle( float( dwNewTime - dwOldTime ) );
        Sleep( 100 / 30.f );

        dwOldTime = dwNewTime;
	}//while

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOUNCINGBALL_STEPS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BOUNCINGBALL_STEPS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   //HWND hWnd;

   g_hInstance = hInstance; // Store instance handle in our global variable

   g_hwndMain = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);

   if (!g_hwndMain)
   {
      return FALSE;
   }

   ShowWindow(g_hwndMain, nCmdShow);
   UpdateWindow(g_hwndMain);

   return TRUE;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
        case ID_FILE_CREATEBALL:
            MessageBoxA( hWnd, "Hello World", "Title", MB_OK );
            break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        //OnPaint( hdc );

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_SIZE:
        OnSize();
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
