/**
 * Utility functions
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#include "Utils.h"
#include <math.h>
#include <tchar.h>
#include <shlobj.h>

#pragma warning(disable: 4996)

namespace neurotec { namespace system
{
	static const int max_format_string = 1000;

	/**
	 * Formats string.
	 * @param	format	format specification. 
	 * @return	formatted string.
	 */
	string Format(LPCTSTR format, ...)
	{
		string s(max_format_string, 0);
		va_list list;
		va_start(list, format);
		_vsntprintf((CHAR *)s.data(), max_format_string, format, list);
		va_end(list);

		return s;
	}

	string FormatV(LPCTSTR format, va_list list)
	{
		string s(max_format_string, 0);
		_vsntprintf((CHAR *)s.data(), max_format_string, format, list);

		return s;
	}

	/**
	 * Checks for file existence.
	 * @param	path	file name, including path.
	 * @return	true	if file exists
	 *			false	otherwise
	 */
	bool FileExists(const char * path)
	{
		bool result = false;
		WIN32_FIND_DATA fd;
		HANDLE handle = ::FindFirstFile(path, &fd);
		if (handle != INVALID_HANDLE_VALUE)
		{
			do 
			{
				const bool isFile = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
				if (isFile)
				{
					result = true;
					break;
				}
			} while (::FindNextFile(handle, &fd));
			::FindClose(handle);
		}
		return result;
	}

	/**
	 * Extracts folder name part of path string.
	 * @param	path	file name, including path.
	 *			folder	extracted folder name.
	 */
	void ExtractFolder(const string& path, string& folder)
	{
		static const string::size_type npos = (unsigned)-1;
		
		const string::size_type i = path.find_last_of('\\');
		
		if (i != npos && i+1 < path.length())
			folder = path.substr(0, i);
		else
			folder = path.substr(0);
	}

	/**
	 * Strips path information and extension.
	 * @param	path	file name, including path.
	 * @return	file name without extension
	 */
	string GetFileNameWithoutExtension(const string &path)
	{
		static const string::size_type npos = (unsigned)-1;

		string::size_type i = path.find_last_of('\\');
		i = (i == npos) ? 0 : i + 1;

		string::size_type c = path.find_last_of('.');
		c = (c == npos) ? path.length() - i : c - i;

		return path.substr(i, c);
	}

	/**
	 * Strips path information and extension,
	 * limits length to 50 characters.
	 * @param	path	file name, including path.
	 * @return	file name without extension
	 */
	string GetFileNameID(const string &path)
	{
		string fileID = GetFileNameWithoutExtension(path);
		if (fileID.length() >= 50)
		{
			fileID.resize(49, 0);
		}

		return fileID;
	}

	/**
	 * Constructs file filter for open/save dialogs.
	 * @param	name
	 *			filter
	 *			filter_in_name
	 */
	string MakeFileFilterString(const string &name, const string &filter, bool filter_in_name)
	{
		string s = name;
		if (filter_in_name)
		{
			s += TEXT(" (") + filter + TEXT(")");
		}
		s += TEXT('|') + filter;

		return s;
	}

	/**
	 * Converts boolean value to string.
	 * @param	value	boolean to convert.
	 * @return	conversion result.
	 */
	string BoolToString(const int value, const string &trueValue, const string &falseValue)
	{
		return value ? trueValue : falseValue;
	}

	string StringToString(string value)
	{
		return value;
	}

	/**
	 * Converts integer number to string value.
	 * @param	value	integer to convert.
	 * @return	conversion result.
	 */
	string IntToString(const int value)
	{
		return Format("%d", value);
	}

	/**
	 * Converts float number to string value.
	 * @param	value	float to convert.
	 * @return	conversion result.
	 */
	string FloatToString(const double value)
	{
		TCHAR buf[100];
		_stprintf(buf, TEXT("%f"), value);
		size_t l = _tcslen(buf);

		// - remove trailing zeros
		for(int i = (int)l - 1; i >= 0; i--)
		{
			if (buf[i] != TEXT('0'))
			{
				l = i + 1;
				buf[l] = TEXT('\0');
				break;
			}
		}
		// - remove trailing period
		if (l > 0 && buf[l - 1] == TEXT('.'))
		{
			l--;
			buf[l] = TEXT('\0');
		}

		string s(buf);
		// - add zero in the beginning
		if (s.length() > 0 && s[0] == TEXT('.'))
			s = TEXT('0') + s;

		return s;
	}
	
	/**
	 * Converts string to boolean value.
	 * @param	value	string to convert.
	 * @return	boolean value.
	 */
	int StringToBool(const string value, const string &trueValue, const string &falseValue)
	{
		if (value == trueValue) return 1;
		else if (value == falseValue) return 0;
		else throw exception("Can not convert string to bool");
	}

	/**
	 * Converts string to integer number.
	 * @param	value	string to convert.
	 * @return	integer value.
	 */
	int StringToInt(const string &value)
	{
		int v;
		int res = sscanf(value.c_str(), "%d", &v);
		if (res == EOF || res == 0) throw exception("Can not convert string to integer");
		return v;
	}

	/**
	 * Converts string to float.
	 * @param	value	string to convert.
	 * @return	float value.
	 */
	float StringToFloat(const string &value)
	{
		float v;
		int res = sscanf(value.c_str(), "%f", &v);
		if (res == EOF || res == 0) throw exception("Can not convert string to float");
		if (v == HUGE_VAL || v == -HUGE_VAL) throw exception("Overflow when converting string to integer");

		return v;
	}

	#ifndef VER_NT_WORKSTATION
	#define VER_NT_WORKSTATION			0x0000001
	#endif
	#ifndef VER_NT_DOMAIN_CONTROLLER
	#define VER_NT_DOMAIN_CONTROLLER	0x0000002
	#endif
	#ifndef VER_NT_SERVER
	#define VER_NT_SERVER				0x0000003
	#endif

	#ifndef VER_SERVER_NT
	#define VER_SERVER_NT                       0x80000000
	#endif
	#ifndef VER_WORKSTATION_NT
	#define VER_WORKSTATION_NT                  0x40000000
	#endif
	#ifndef VER_SUITE_SMALLBUSINESS
	#define VER_SUITE_SMALLBUSINESS             0x00000001
	#endif
	#ifndef VER_SUITE_ENTERPRISE
	#define VER_SUITE_ENTERPRISE                0x00000002
	#endif
	#ifndef VER_SUITE_BACKOFFICE
	#define VER_SUITE_BACKOFFICE                0x00000004
	#endif
	#ifndef VER_SUITE_COMMUNICATIONS
	#define VER_SUITE_COMMUNICATIONS            0x00000008
	#endif
	#ifndef VER_SUITE_TERMINAL
	#define VER_SUITE_TERMINAL                  0x00000010
	#endif
	#ifndef VER_SUITE_SMALLBUSINESS_RESTRICTED
	#define VER_SUITE_SMALLBUSINESS_RESTRICTED  0x00000020
	#endif
	#ifndef VER_SUITE_EMBEDDEDNT
	#define VER_SUITE_EMBEDDEDNT                0x00000040
	#endif
	#ifndef VER_SUITE_DATACENTER
	#define VER_SUITE_DATACENTER                0x00000080
	#endif
	#ifndef VER_SUITE_SINGLEUSERTS
	#define VER_SUITE_SINGLEUSERTS              0x00000100
	#endif
	#ifndef VER_SUITE_PERSONAL
	#define VER_SUITE_PERSONAL                  0x00000200
	#endif
	#ifndef VER_SUITE_BLADE
	#define VER_SUITE_BLADE                     0x00000400
	#endif

	/**
	 * Gets operating system version information.
	 * @param	desc	structure to fill with version information.
	 */
	void GetOsDesc(OsDesc & desc)
	{	
		OSVERSIONINFOEX vie;
		vie.dwOSVersionInfoSize = sizeof(vie);
		if (GetVersionEx((LPOSVERSIONINFO)&vie))
		{
			desc.osMajorVersion = vie.dwMajorVersion;
			desc.osMinorVersion = vie.dwMinorVersion;
			desc.osPlatformId = vie.dwPlatformId;
			desc.osBuild = vie.dwBuildNumber;
			desc.osUpdateName = string(vie.szCSDVersion);
			desc.osSPMajorVersion = vie.wServicePackMajor;
			desc.osSPMinorVersion = vie.wServicePackMinor;
	#if WINVER > 0x0400
			desc.osSuite = vie.wSuiteMask;
			desc.osType = vie.wProductType;
	#else
			desc.osSuite = vie.wReserved[0];
			desc.osType = LOBYTE(vie.wReserved[1]);
	#endif
		}
		else
		{
			desc.osSPMajorVersion = 0;
			desc.osSPMinorVersion = 0;
			desc.osSuite = 0;
			desc.osType = 0;
			OSVERSIONINFO vi;
			vi.dwOSVersionInfoSize = sizeof(vi);
			if (GetVersionEx(&vi))
			{
				desc.osMajorVersion = vi.dwMajorVersion;
				desc.osMinorVersion = vi.dwMinorVersion;
				desc.osPlatformId = vi.dwPlatformId;
				desc.osBuild = vi.dwBuildNumber;
				desc.osUpdateName = string(vi.szCSDVersion);
			}
			else
			{
				desc.osMajorVersion = 0;
				desc.osMinorVersion = 0;
				desc.osPlatformId = 0;
				desc.osBuild = 0;
			}
		}
		if (desc.osPlatformId != VER_PLATFORM_WIN32_NT) desc.osBuild = LOWORD(desc.osBuild);

		// - OS name
		if (desc.osPlatformId == VER_PLATFORM_WIN32s) desc.osName = TEXT("Win32s");
		else if (desc.osPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			if (desc.osMajorVersion > 4) desc.osName = TEXT("Windows");
			else if (desc.osMajorVersion == 4)
			{
				if (desc.osMinorVersion >= 90) desc.osName = TEXT("Windows Me");
				else if (desc.osMinorVersion >= 10) desc.osName = TEXT("Windows 98");
				else desc.osName = TEXT("Windows 95");
			}
			else desc.osName = Format(TEXT("Windows %ul.%ul"), desc.osMajorVersion, desc.osMinorVersion);
		}
		else if (desc.osPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if (desc.osMajorVersion > 5) desc.osName = TEXT("Windows NT");
			else if (desc.osMajorVersion == 5)
			{
				if (desc.osMinorVersion >= 1)
				{
					if (desc.osType == VER_NT_SERVER && desc.osMinorVersion >= 2)
					{
						if (desc.osSuite & VER_SUITE_BLADE) desc.osName = TEXT("Windows Server 2003, Web Edition");
						if (desc.osSuite & VER_SUITE_DATACENTER) desc.osName = TEXT("Windows Server 2003, Datacenter Edition");
						if (desc.osSuite & VER_SUITE_ENTERPRISE) desc.osName = TEXT("Windows Server 2003, Enterprise Edition");
						else desc.osName = TEXT("Windows Server 2003 family");
					}
					else if (desc.osType == VER_NT_WORKSTATION)
					{
						if (desc.osSuite & VER_SUITE_PERSONAL) desc.osName = TEXT("Windows XP Home Edition");
						else desc.osName = TEXT("Windows XP Professional");
					}
					else
					{
						desc.osName = TEXT("Windows XP/Server");
					}
				}
				else
				{
					if (desc.osType == VER_NT_SERVER)
					{
						if (desc.osSuite & VER_SUITE_DATACENTER) desc.osName = TEXT("Windows 2000 DataCenter Server");
						else if (desc.osSuite & VER_SUITE_ENTERPRISE) desc.osName = TEXT("Windows 2000 Advanced Server");
						else desc.osName = TEXT("Windows 2000 Server");
					}
					else if (desc.osType == VER_NT_WORKSTATION) desc.osName = TEXT("Windows 2000 Professional");
					else desc.osName = TEXT("Windows 2000");
				}
			}
			else if (desc.osMajorVersion == 4)
			{
				if (desc.osType == VER_NT_SERVER) desc.osName = TEXT("Windows NT 4.0 Server");
				else if (desc.osType == VER_NT_WORKSTATION) desc.osName = TEXT("Windows NT 4.0 Workstation");
				else desc.osName = TEXT("Windows NT 4.0");
			}
			else desc.osName = Format(TEXT("Windows NT %ul.%ul"), desc.osMajorVersion, desc.osMinorVersion);
		}
		else desc.osName = TEXT("[Unknown]");
		if (!(desc.osUpdateName == ""))
			if (desc.osPlatformId == VER_PLATFORM_WIN32_WINDOWS) desc.osName += desc.osUpdateName;
			else desc.osName += string(", ") + desc.osUpdateName;
	}
}}