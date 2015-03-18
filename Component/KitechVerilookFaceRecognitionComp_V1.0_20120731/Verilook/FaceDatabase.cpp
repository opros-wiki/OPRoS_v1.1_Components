/**
 * Class CFaceDatabase, used to enroll and match faces
 * using ADO database.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

//#include "StdAfx.h"
#include <algorithm>
#include "FaceDatabase.h"

using std::sort;
using neurotec::verilook::sample::defaultDatabaseProvider;

/**
 * Class constructor. 
 * @param	nFeaturesSize	size of one face template (in bytes).
 */
CFaceDatabase::CFaceDatabase()
{
}

/**
 * Class destructor. Closes face database.
 */
CFaceDatabase::~CFaceDatabase(void)
{
	Close();
}

/**
 * Clears all records from face database.
 */
void CFaceDatabase::Clear()
{
	database.Clear();
}

/**
 * Opens face database.
 * @param	lpszFileName	database file name.
 */
void CFaceDatabase::Open(const LPCTSTR lpszFileName)
{
	database.Open(defaultDatabaseProvider, lpszFileName);
}

/**
 * Closes previously open database.
 */
void CFaceDatabase::Close()
{
	database.Close();
}

/**
 * Enrolls new record to face database.
 * @param	id			subject id.
 *			features	feature vector.
 * @return	index of newly added record.
 */
int CFaceDatabase::Add(LPCTSTR id, void *features, NSizeType featuresSize)
{
	return database.Add(features, (DWORD) featuresSize, id);
}

/**
 * Matches face to all records in database.
 * @param	features	face feature vector.
 *			id			best match face id.
 * @return	similarity of best match.
 */
NInt CFaceDatabase::Match(HNMatcher hMatcher, const void *features, NSizeType size, string& id)
{
	NMMatchDetails* matchDetails;
	if (NFailed(NMIdentifyStart(hMatcher, features, size, &matchDetails)))
	{
		return 0;
	}

	NInt max_score = 0;
	for (int i = 0; i < database.Count; i++)
	{
		NInt score;
		if (NFailed(NMIdentifyNext(hMatcher, database.Features(i), database.FeaturesSize(i), matchDetails, &score)))
		{
			score = -1;
		}

		if (score > max_score)
		{
			max_score = score;
			id = database.FaceID(i);
		}
	}
	if (NFailed(NMIdentifyEnd(hMatcher)))
	{
		return -1;
	}

	return max_score;
}

/**
 * Matches face to all records in database.
 * @param	features	face feature vector.
 *			lst			ranking list.
 * @return	similarity of best match.
 */
NInt CFaceDatabase::MatchList(HNMatcher hMatcher, const void *features, NSizeType size, match_list& lst)
{
	NMMatchDetails* matchDetails;
	if (NFailed(NMIdentifyStart(hMatcher, features, size, &matchDetails)))
	{
		return 0;
	}

	NInt max_score = 0;
	lst.resize(database.Count);
	for (int i = 0; i < database.Count; i++)
	{
		NInt score;
		if (NFailed(NMIdentifyNext(hMatcher, database.Features(i), database.FeaturesSize(i), matchDetails, &score)))
		{
			score = -1;
		}
		CMatch m(database.FaceID(i), score);
		lst[i] = m;
		if (score > max_score)
		{
			max_score = score;
		}
	}
	if (NFailed(NMIdentifyEnd(hMatcher)))
	{
		return -1;
	}

	return max_score;
}
