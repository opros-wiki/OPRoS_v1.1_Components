/**
 * Class CFaceDatabase, used to enroll and match faces
 * using ADO database.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#pragma once

#include <vector>
#include <string>
#include "Utils.h"
#include "NMatcher.h"

#ifdef _WIN64

#include "SQLiteDatabase.h"
using neurotec::verilook::sample::CSQLiteDatabase;

#else

#include "VLDatabase.h"
using neurotec::verilook::sample::CVLDatabase;

#endif


using std::string;
using std::vector;

class CMatch
{
public:
	string id;
	int similarity;

public:
	CMatch() : id(""), similarity(0)
	{
	}

	CMatch(const string& id, const int similarity)
	{
		this->id = id;
		this->similarity = similarity;
	}
	static bool greater(CMatch a, CMatch b)
	{
		return a.similarity > b.similarity;
	}
};

typedef vector<CMatch> match_list;

class CFaceDatabase
{
private:

#ifdef _WIN64
	CSQLiteDatabase database;
#else
	CVLDatabase database;
#endif

public:
	CFaceDatabase();
	~CFaceDatabase();
	
	void Open(LPCTSTR lpszFileName);
	void Close();
	void Clear();

	int Add(const LPCTSTR id, void *features, NSizeType featuresSize);
	NInt Match(HNMatcher hMatcher, const void *features, NSizeType size, string& id);
	NInt MatchList(HNMatcher hMatcher, const void *features, NSizeType size, match_list& lst);
	int GetCount() { return database.Count;}
};
