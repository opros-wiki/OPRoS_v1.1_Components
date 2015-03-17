#ifndef __GOOGLE_TTS_H__
#define __GOOGLE_TTS_H__

#include <device/Speech.h>

#include <boost/thread/recursive_mutex.hpp>

#include <fmod.hpp>

#include "HttpClient.h"

class GoogleTTS : public Speech
{
public:
	GoogleTTS();
	virtual ~GoogleTTS();

public:
	virtual int Initialize(Property parameter);
	virtual int Finalize();
	virtual int Enable();
	virtual int Disable();
	virtual int SetParameter(Property parameter);
	virtual int GetParameter(Property& parameter);

public:
	int PlaySpeech(string speechData, int mode);
	int StopSpeech();

	void ReleaseSoundThreadHandler();

private:
	static FMOD_RESULT F_CALLBACK FmodChannelCallBack(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2);

	void updateFmod();
	void releaseSounds();

private:
	boost::recursive_mutex mutex;
	HttpClient httpClient;

	std::string defaultLanguage;
	FMOD::System* pFmodSystem;
	std::list<FMOD::Sound*> mCompletedSounds;
};

#endif //__GOOGLE_TTS_H__


