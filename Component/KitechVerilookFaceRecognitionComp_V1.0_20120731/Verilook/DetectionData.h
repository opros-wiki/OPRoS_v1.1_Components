/**
 * Class CDetectionData used to store information
 * provided by face detection algorithm.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#pragma once

#include <NLExtractor.h>
#include <NImages.h>

class CDetectionData {
public:
	HNImage FaceImage;
	NleDetectionDetails *faces;
	int faceCount;

public:
	CDetectionData();
	~CDetectionData();
	void Reset();
	void Assign(CDetectionData& data);
};
