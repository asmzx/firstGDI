// testGDI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "testGDI.h"
#include <vector>

#define MAX_LOADSTRING 100

//add new comment

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWnd;
HBITMAP hBitmap = NULL;
int sizeX=800, sizeY=600;
int nWidth = 0;  // ������ ������� ������� 
int nHeight = 0; // ������ ������� ������� 

std::vector<unsigned char> bits;
void Render();

HBITMAP CreateBitmap(int x, int y)
{
	//create bmp
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biHeight = y;
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biPlanes = 1;

	HDC dc;
	dc = CreateCompatibleDC(NULL);

	bits.resize(x*y * 4);
	HBITMAP bitmap = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)(&bits[0]), NULL, 0);
	DeleteDC(dc);

	return bitmap;
}

void DeleteBitmap(HBITMAP bitmap);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg = { 0 };
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTGDI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTGDI));

	// Main message loop
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTGDI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTGDI);
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
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      10, 10, sizeX, sizeY, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   RECT rec;
   GetClientRect(hWnd, &rec);
   sizeX = rec.right - rec.left;
   sizeY = rec.bottom - rec.top;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	case WM_CREATE:
		

		break;
	case WM_SIZE:
		//if (wParam)// ������ ��������� �������� 
		{
			nWidth = LOWORD(lParam);  // ������ ������� ������� 
			nHeight = HIWORD(lParam); // ������ ������� �������
			DeleteObject(hBitmap);
			hBitmap=CreateBitmap(nWidth, nHeight);
			sizeX = nWidth;
			sizeY = nHeight;
		}
		break;
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
		PAINTSTRUCT 	ps;
		HDC 			hdc;
		BITMAP 			bitmap;
		HDC 			hdcMem;
		HGDIOBJ 		oldBitmap;

		hdc = BeginPaint(hWnd, &ps);

		hdcMem = CreateCompatibleDC(hdc);

		oldBitmap = SelectObject(hdcMem, hBitmap);

		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
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

void Render()
{
	//////////////////////////////////////////////////////////////////////////
	BITMAPINFO bmi1;
	ZeroMemory(&bmi1, sizeof(BITMAPINFO));
	bmi1.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi1.bmiHeader.biBitCount = 32;
	bmi1.bmiHeader.biHeight = sizeY;
	bmi1.bmiHeader.biWidth = sizeX;
	bmi1.bmiHeader.biPlanes = 1;

	for (int y = 0; y < sizeY; ++y)
	for (int x = 0; x < sizeX; ++x)
	{
		bits[y*sizeX * 4 + x * 4 + 0] = rand() / (float)RAND_MAX*255.0;
		bits[y*sizeX * 4 + x * 4 + 1] = rand() / (float)RAND_MAX*255.0;
		bits[y*sizeX * 4 + x * 4 + 2] = rand() / (float)RAND_MAX*255.0;
	}

	HDC dc;
	dc = CreateCompatibleDC(NULL);
	SetDIBits(dc, hBitmap, 0, sizeY, (void**)(&bits[0]), &bmi1, DIB_RGB_COLORS);
	DeleteDC(dc);

	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	InvalidateRect(hWnd, NULL, FALSE);
	//////////////////////////////////////////////////////////////////////////
}