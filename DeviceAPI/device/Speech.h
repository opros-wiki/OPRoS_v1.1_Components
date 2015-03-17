#ifndef	_SPEECH_H_
#define	_SPEECH_H_

#include "OprosApi.h"

#define	SPEECH_TEXT_STREAM	0
#define	SPEECH_TEXT_FILE	1

class Speech : public OprosApi
{
public:
	Speech(void) { };
	virtual ~Speech(void) { };

public:
	virtual int PlaySpeech(string speechData, int mode) { return API_NOT_SUPPORTED; };
	virtual int StopSpeech(void) { return API_NOT_SUPPORTED; };
};

#endif	//	_SPEECH_H_
