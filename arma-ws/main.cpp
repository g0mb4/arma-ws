#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <atlstr.h>
#include <shellapi.h>
#pragma warning(disable:4996)

#include <boost/filesystem.hpp>

#include "Extension.hpp"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

Extension *ext;

BOOL APIENTRY 
DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		WCHAR path[MAX_PATH + 1];
		GetModuleFileNameW((HINSTANCE)&__ImageBase, path, (MAX_PATH + 1));

		fs::path wdPath(path);

		ext = new Extension(wdPath.parent_path().string());
	}
	break;
	case DLL_PROCESS_DETACH:
		ext->stop();
		break;
	}
	return true;
}

extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

void __stdcall 
RVExtension(char *output, int outputSize, const char *function)
{
	ext->callExtension(output, outputSize, function);
};