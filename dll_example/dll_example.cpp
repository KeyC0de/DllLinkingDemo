#include "winner.h"
#include "dll_example.h"


#ifdef _UNICODE
EXPORT BOOL CALLBACK CenterTextW( HDC hdc,
	PRECT prc,
	PCWSTR pString )
{
	int iLen;
	SIZE size;

	iLen = lstrlenW( pString );

	GetTextExtentPoint32W( hdc,
		pString,
		iLen,
		&size );

	return TextOutW( hdc,
		(prc->right - prc->left - size.cx) / 2,
		(prc->bottom - prc->top - size.cy) / 2,
		pString,
		iLen );
}
#else
EXPORT BOOL CALLBACK CenterTextA( HDC hdc,
	PRECT prc,
	PCSTR pString )
{
	int iLen;
	SIZE size;

	iLen = lstrlenA( pString );

	GetTextExtentPoint32A( hdc,
		pString,
		iLen,
		&size );

	return TextOutA( hdc,
		(prc->right - prc->left - size.cx) / 2,
		(prc->bottom - prc->top - size.cy) / 2,
		pString,
		iLen );
}
#endif


int WINAPI DllMain( HINSTANCE hInst,
	DWORD ulReasonForcall,
	PVOID pReserved )
{
	return TRUE;
}