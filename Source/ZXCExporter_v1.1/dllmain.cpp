
#include "Header.h"

extern ClassDesc2* GetExportDesc();

HINSTANCE hInstance;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		hInstance = hModule;
		DisableThreadLibraryCalls(hInstance);
	}break;
    }
    return TRUE;
}

__declspec(dllexport) const TCHAR* LibDescription()
{
	return _T("Exporter ver 100");
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch (i)
	{
	case 0:
		return GetExportDesc();
	}
	return 0;
}

__declspec(dllexport) int LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec(dllexport) int LibInitialize()
{
	return TRUE;
}

__declspec(dllexport) int LibShutdown()
{
	return TRUE;
}