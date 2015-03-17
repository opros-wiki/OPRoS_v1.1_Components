#ifndef __DLL_LOADER_H__
#define __DLL_LOADER_H__

#include <string>
#include <stdexcept>


#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

class DynamicLibraryLoader
{
public:
	class CanNotLoadException : public std::exception
	{
	public:
		CanNotLoadException(const std::string& message = "Can Not Load Exception")
			: std::exception(message.c_str())	
		{}
	};

public:
	DynamicLibraryLoader(std::string dllName_) throw(CanNotLoadException)
		: hmodule(NULL), dllName(dllName_)
	{
#if defined(WIN32)
		hmodule = LoadLibraryA(dllName.c_str());
#else
		hmodule = dlopen(dllName.c_str(), RTLD_LAZY);
#endif

		if (hmodule == NULL)
		{
			throw CanNotLoadException("Can't Load " + dllName);
		}
	}

	~DynamicLibraryLoader()
	{
		if (hmodule != NULL)
		{			
#if defined(WIN32)
			FreeLibrary(hmodule);
#else
			dlclose(hmodule);
#endif
			hmodule = NULL;
		}
	}

private:
	DynamicLibraryLoader(const DynamicLibraryLoader& rhs){}
	void operator=(const DynamicLibraryLoader& rhs){}

public:
	inline std::string GetDllName()
	{
		return dllName;
	}

	template<typename Func>
	Func GetFunction(const std::string& functionName) throw(CanNotLoadException)
	{
#if defined(WIN32)
		Func function = (Func)GetProcAddress(hmodule, functionName.c_str());
#else
		Func function = (Func)dlsym(hmodule, functionName.c_str());
		if(dlerror() != NULL)
			function = NULL;
#endif

		if (function == NULL)
		{
			throw CanNotLoadException("Can't find " + functionName + " in " + dllName);
		}

		return function;
	}
	
private:
#if defined(WIN32)
	HMODULE hmodule;
#else
	void* hmodule;
#endif

	const std::string dllName;
};

#endif //__DLL_LOADER_H__