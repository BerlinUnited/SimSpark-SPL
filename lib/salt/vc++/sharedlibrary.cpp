#include "../sharedlibrary.h"
#include <windows.h>

using namespace salt;

bool SharedLibrary::Open(const std::string &libName)
{
	if (mLibHandle)
	{
		Close();
	}

	mLibHandle = ::LoadLibrary((libName + ".dll").c_str());

	return (mLibHandle!=NULL);
}

void* SharedLibrary::GetProcAddress(const std::string &procName)
{
	if (mLibHandle)
	{
		return ::GetProcAddress((HMODULE)mLibHandle, procName.c_str());
	}
	return NULL;
}

void SharedLibrary::Close()
{
	if (mLibHandle)
	{
		::FreeLibrary((HMODULE)mLibHandle);
		mLibHandle = NULL;
	}
}