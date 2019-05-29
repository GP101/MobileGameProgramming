// Win32Tetris_step1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Tetris_step1.h"
#include <MMSystem.h>
#include "ConsoleTetrisUtil.h"
#include "KBoard.h"

#pragma comment(lib,"winmm.lib")


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HWND                    g_hwndMain = NULL;
HDC                     g_hdc = 0;
HBITMAP                 g_hBitmap = 0;
RECT                    g_clientRect;

KBoard              g_visibleBoard;
KBoard              g_hiddenBoard;
KBlock              g_curBlock;
float               g_fCurBlockTimer = 0.f;
float               g_fCurBlockSpeed = 0.25f; ///< unit is seconds per a block


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void                    Initialize();
void                    Finalize();
void                    OnSize();
void                    OnIdle(float fElapsedTime_);
void                    OnFrameMove(float fElapsedTime_);
void                    OnFrameRender(HDC hdc_, float fElapsedTime_);


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
	LoadString(hInstance, IDC_WIN32TETRIS_STEP1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32TETRIS_STEP1));

    Initialize();

    DWORD dwOldTime = ::timeGetTime();

    while (true)
    {
        ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        const DWORD dwNewTime = ::timeGetTime();
        const BOOL bIsTranslateMessage = TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
        if (!bIsTranslateMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }//if

        OnIdle(float(dwNewTime - dwOldTime) / 1000.f);
        Sleep(10);

        dwOldTime = dwNewTime;

        if (msg.message == WM_QUIT)
        {
            break;
        }//if
    }//while

    Finalize();

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TETRIS_STEP1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32TETRIS_STEP1);
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

   hInst = hInstance; // Store instance handle in our global variable

   g_hwndMain = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
		// TODO: Add any drawing code here...
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


void GenerateNextBlock(OUT KBlock& block_)
{
    block_.m_aBlockPosition[0].x = 0;
    block_.m_aBlockPosition[0].y = 0;
    block_.m_aBlockPosition[1].x = 1;
    block_.m_aBlockPosition[1].y = 0;
    block_.m_aBlockPosition[2].x = 0;
    block_.m_aBlockPosition[2].y = 1;
    block_.m_aBlockPosition[3].x = 0;
    block_.m_aBlockPosition[3].y = 2;
    block_.m_worldPos.x = 2;
    block_.m_worldPos.y = 2;
    block_.m_iNumStone = 4;
}//GenerateNextBlock()


void Initialize()
{
    g_visibleBoard.Initialize();
    g_hiddenBoard.Initialize();

    // test initialize a current block
    GenerateNextBlock(g_curBlock);
}//Initialize()


void Finalize()
{
    if (g_hdc != 0) {
        DeleteDC(g_hdc);
        g_hdc = 0;
    }//if
    if (g_hBitmap != 0) {
        DeleteObject(g_hBitmap);
        g_hBitmap = 0;
    }//if
}//Finalize()


void OnSize()
{
    Finalize();

    ::GetClientRect(g_hwndMain, &g_clientRect);
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    HDC hdc = ::GetDC(g_hwndMain);
    g_hdc = CreateCompatibleDC(hdc);
    g_hBitmap = CreateCompatibleBitmap(g_hdc, iWidth, iHeight);
    SelectObject(g_hdc, g_hBitmap);
}//OnSize()


void OnIdle(float fElapsedTime_)
{
    OnFrameMove(fElapsedTime_);

    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    HDC hdc = ::GetDC(g_hwndMain);

    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(g_hdc, brush);
    Rectangle(g_hdc, 0, 0, iWidth, iHeight);

    {
        OnFrameRender(g_hdc, fElapsedTime_);
    }
    BitBlt(hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY);
    DeleteObject(brush);

    ::ReleaseDC(g_hwndMain, hdc);
    //::UpdateWindow( g_hwndMain );
}//OnIdle()


void OnFrameMove(float fElapsedTime_)
{
    // save current block state.
    KBlock savedBlock;
    savedBlock.CopyFrom(g_curBlock);

    // process user keyboard inputs
    if (::GetAsyncKeyState(VK_LEFT))
    {
        g_curBlock.m_worldPos.x -= 1;
        if (g_hiddenBoard.CanLocateBlock(g_curBlock) == false)
        {
            // restore current Block.
            g_curBlock.CopyFrom(savedBlock);
        }//if
    }
    if (::GetAsyncKeyState(VK_RIGHT))
    {
        g_curBlock.m_worldPos.x += 1;
        if (g_hiddenBoard.CanLocateBlock(g_curBlock) == false)
        {
            // restore current Block.
            g_curBlock.CopyFrom(savedBlock);
        }//if
    }//if.. else if..

    g_fCurBlockTimer += fElapsedTime_;
    if (g_fCurBlockTimer >= g_fCurBlockSpeed)
    {
        g_fCurBlockTimer -= g_fCurBlockSpeed; // reset current block timer
        g_curBlock.m_worldPos.y += 1;

        // try to locate a block on the Board
        if (g_hiddenBoard.CanLocateBlock(g_curBlock) == false)
        {
            // restore current Block.
            g_curBlock.CopyFrom(savedBlock);
            // finally locate current Block.
            g_hiddenBoard.LocateBlock(g_curBlock);

            // check whether removing rows are possible or not.
            int iNumOfRows = 0;
            int aRemoveRows[BOARD_SIZE_ROW];
            bool bIsRemovableRow = g_hiddenBoard.GetRemovableRowFromBoard(OUT &iNumOfRows, aRemoveRows);
            if (bIsRemovableRow == true)
            {
                g_hiddenBoard.RemoveRowsFromBoard(iNumOfRows, aRemoveRows);
            }//if

            // test regenerate a next Block.
            GenerateNextBlock(g_curBlock);
        }//if
    }//if
}//OnFrameMove()


void DrawBoard(HDC hdc_, const KBoard& board_)
{
    ::Ellipse(hdc_, 0, 0, g_clientRect.right, g_clientRect.bottom);
}//DrawBoard()


void OnFrameRender(HDC hdc_, float fElapsedTime_)
{
    // prepare working Board with current(previous frame) board data.
    g_visibleBoard.CopyFrom(g_hiddenBoard);

    g_visibleBoard.LocateBlock(g_curBlock);

    DrawBoard(hdc_, g_visibleBoard);
}//OnFrameRender()
