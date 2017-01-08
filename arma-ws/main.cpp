#include <boost/filesystem.hpp>

#include "Extension.hpp"

Extension *ext;

#ifdef __GNUC__
	#include <dlfcn.h>
	// Code for GNU C compiler
	static void __attribute__((constructor))
	extension_init(void)
	{
		Dl_info dl_info;
		dladdr((void*)extension_init, &dl_info);
		ext = new Extension(boost::filesystem::path (dl_info.dli_fname).string());
	}

	static void __attribute__((destructor))
	extension_destroy(void)
	{
		ext->stop();
	}

	extern "C"
	{
		void RVExtension(char *output, int outputSize, const char *function);
	};

	void RVExtension(char *output, int outputSize, const char *function)
	{
		ext->callExtension(output, outputSize, function);
	};

#elif _MSC_VER

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <atlstr.h>
	#include <shellapi.h>
	#pragma warning(disable:4996)

	EXTERN_C IMAGE_DOS_HEADER __ImageBase;

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

#endif