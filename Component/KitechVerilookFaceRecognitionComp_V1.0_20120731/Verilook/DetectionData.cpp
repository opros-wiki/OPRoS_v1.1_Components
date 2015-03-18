/**
 * Class CDetectionData used to store information
 * provided by face detection algorithm.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#include "DetectionData.h"

/**
 * Class constructor.
 */
CDetectionData::CDetectionData()
	: FaceImage(NULL)
{
	faces = new NleDetectionDetails[128];
	Reset();
}

/**
 * Class destructor, destroys face image.
 */
CDetectionData::~CDetectionData()
{
	if (FaceImage)
	{
		NImageFree(FaceImage);
	}
	delete[] faces;
}

/**
 * Resets eye locations.
 */
void CDetectionData::Reset()
{
	faceCount = 0;
	NImageFree(FaceImage);
	FaceImage = NULL;
}

/**
 * Sets member variables.
 * @param	data	CDetectionData class instance from which new member values will be assigned.
 */
void CDetectionData::Assign(CDetectionData& data)
{
	Reset();
	if (!data.FaceImage)
	{
		return;
	}

	NImageClone(data.FaceImage, &FaceImage);
	for (int i = 0; i < data.faceCount; ++i)
		faces[i] = data.faces[i];
	faceCount = data.faceCount;
}
