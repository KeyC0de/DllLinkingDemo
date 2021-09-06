#include "winner.h"

#pragma comment( lib, "dll_example.lib" )

#pragma region dllLinkage
// include this:
//#include "..\dll_example\dll_example.h"
// or use dllimport (and I will):
#ifdef  __cplusplus
// prevents name mangling, thus allowing the dll to be used by both C and C++ programs
#	define IMPORT extern "C" __declspec( dllimport )
#else
#	define IMPORT __declspec( dllexport )
#endif

#ifdef UNICODE
IMPORT BOOL CALLBACK CenterTextW( HDC, PRECT, PCWSTR );
#	define CenterText CenterTextW
#else
IMPORT BOOL CALLBACK CenterTextA( HDC, PRECT, PCSTR );
#	define CenterText CenterTextA
#endif
#pragma endregion


LRESULT CALLBACK WndProc( HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	PCWSTR nonDllStr = L"This string was displayed without the dll.";

	switch ( message )
	{
	case WM_PAINT:
	{
		hdc = BeginPaint( hwnd,
			&ps );
		GetClientRect( hwnd,
			&rect );
		CenterTextW( hdc,
			&rect,
			TEXT( "This string was displayed using a dll's function." ) );
		//TextOut( hdc, 100, 100, nonDllStr, sizeof(nonDllStr) * 12 );
		EndPaint( hwnd,
			&ps );
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage( 0 );
		return 0;
	}
	}

	return DefWindowProc( hwnd,
		message,
		wParam,
		lParam );
}


// This program centers a text string in its client area, acquiring the function to do so from the dll (dll_example)
int WINAPI WinMain( HINSTANCE hInst,
	HINSTANCE hPrevInst,
	PSTR szCmdLine,
	int iCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInst );
	static TCHAR szClassName[] = TEXT("UseTheDllExample");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInst;
	wndClass.hIcon = LoadIconW( nullptr,
		IDI_APPLICATION );
	wndClass.hCursor = LoadCursorW( nullptr,
		IDC_ARROW );
	wndClass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = szClassName;

	if ( !RegisterClassW( &wndClass ) )
	{
		MessageBox( nullptr,
			TEXT( "This program requires Windows NT!" ),
			szClassName,
			MB_ICONERROR );
		return 0;
	}

	hwnd = CreateWindowW( szClassName,
		TEXT( "DLL Demo Program" ),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		hInst,
		nullptr );

	ShowWindow( hwnd,
		iCmdShow );
	UpdateWindow( hwnd );

	// on WM_QUIT GetMessage() returns 0
	// or use PeekMessage
	while ( GetMessageW( &msg,
		nullptr,
		0,
		0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}

	return (int) msg.wParam;
}
