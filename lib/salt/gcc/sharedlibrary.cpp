#include "../sharedlibrary.h"
#include <dlfcn.h>

using namespace salt;

bool SharedLibrary::Open(const std::string &libName)
{
	if (mLibHandle)
	{
		Close();
	}

	mLibHandle = ::dlopen((libName + ".so").c_str(), RTLD_LAZY);

	return (mLibHandle!=NULL);
}

void* SharedLibrary::GetProcAddress(const std::string &procName)
{
	if (mLibHandle)
	{
		return ::dlsym(mLibHandle, procName.c_str());
	}
	return NULL;
}

void SharedLibrary::Close()
{
	if (mLibHandle)
	{
		::dlclose(mLibHandle);
		mLibHandle = NULL;
	}
}
