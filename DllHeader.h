#pragma once

// Conditional compilation for exporting/importing functions
#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

extern "C"
{
	// Declare a function to be exported from the DLL
	DECLDIR void Share();	
}