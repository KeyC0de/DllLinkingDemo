#pragma once

#ifdef  __cplusplus
// prevents name mangling, thus allowing the dll to be used by both C and C++ programs
#	define EXPORT extern "C" __declspec( dllexport )
#else
#	define EXPORT __declspec( dllexport )
#endif

#ifdef UNICODE
EXPORT BOOL CALLBACK CenterTextW( HDC, PRECT, PCWSTR );
#	define CenterText CenterTextW
#else
EXPORT BOOL CALLBACK CenterTextA( HDC, PRECT, PCSTR );
#	define CenterText CenterTextA
#endif