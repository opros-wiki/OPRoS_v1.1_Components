/**
 * Utility functions
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */
#pragma once

#include <ComDef.h>
#include <string>
#include <NImage.h>

using std::string;
using std::exception;

namespace neurotec { namespace system
{
	
	/**
	 * OS version info structure.
	 */
	struct OsDesc
	{
		DWORD osMajorVersion;
		DWORD osMinorVersion;
		DWORD osPlatformId;
		DWORD osBuild;
		string osUpdateName;
		WORD osSPMajorVersion;
		WORD osSPMinorVersion;
		WORD osSuite;
		WORD osType;

		string osName;
	};

	void ExtractFolder(const string& path, string& folder);

	// - Arithmetic routines
	#ifndef PI
	const double PI = 3.1415926535897932384626433832795;
	#endif
	inline int Round(double value) { return (int)(value < 0 ? value - 0.5 : value + 0.5); }
	inline int RoundP(double value) { return (int)(value + 0.5); }
	inline double PercentF(int count, int total) { return 100.0 * count / total; }
	inline double PercentFZ(int count, int total) { return total ? 100.0 * count / total : 0.0; }
	inline int Percent(int count, int total) { return Round(PercentF(count, total)); }
	inline int PercentZ(int count, int total) { return Round(PercentFZ(count, total)); }

	// - Flag routines
	inline bool IsFlagSet(DWORD flags, DWORD flag)
	{
		return (flags & flag) == flag;
	}

	inline void SetFlag(DWORD &flags, DWORD flag)
	{
		flags |= flag;
	}

	inline void ClearFlag(DWORD &flags, DWORD flag)
	{
		flags &= ~flag;
	}

	inline void SetFlag(DWORD &flags, DWORD flag, bool value)
	{
		if (value)
			SetFlag(flags, flag);
		else
			ClearFlag(flags, flag);
	}

	// - String routines
	string Format(LPCTSTR format, ...);
	string FormatV(LPCTSTR format, va_list list);
	string AdjustLineBreaks(const string &s);
	string GetOneLine(const string &s);

	// - File operations
	bool FileExists(const char * path);
	string MakeFileFilterString(const string &name, const string &filter, bool filter_in_name = true);
	string GetFileNameWithoutExtension(const string &path);
	string GetFileNameID(const string &path);
	
	const string trueString = "1";
	const string falseString = "0";

	// - Conversion routines
	string BoolToString(int value, const string &trueValue = trueString, const string &falseValue = falseString);
	string IntToString(int value);
	string FloatToString(double value);
	string StringToString(string value);
	
	int StringToBool(const string value, const string &trueValue = trueString, const string &falseValue = falseString);
	int StringToInt(const string &value);
	float StringToFloat(const string &value);

	// - OS description
	void GetOsDesc(OsDesc & desc);

	// - Error handling
	inline string FormatComError(_com_error &e)
	{
		return Format(TEXT("COM error:\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s"),
			e.Error(), e.ErrorMessage(), (LPCTSTR)e.Source(), (LPCTSTR)e.Description());
	}

	inline string FormatComError(_com_error &e, const string &where)
	{
		return Format(TEXT("%s\n")
			TEXT("COM error:\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s"),
			(LPCTSTR)where.c_str(),
			e.Error(), e.ErrorMessage(), (LPCTSTR)e.Source(), (LPCTSTR)e.Description());
	}

	inline void ComErrorToException(_com_error &e)
	{
		throw exception(FormatComError(e).c_str());
	}

	inline void ComErrorToException(_com_error &e, const string &where)
	{
		throw exception(FormatComError(e, where).c_str());
	}
	
}}
