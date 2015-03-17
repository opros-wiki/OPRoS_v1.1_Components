#include "GoogleTTS.h"

#include <string>
#include <fstream>

#include <device/OprosPrintMessage.h>

using namespace boost::asio;

GoogleTTS::GoogleTTS()
	: pFmodSystem(NULL), defaultLanguage("ko")
{
}

GoogleTTS::~GoogleTTS()
{
	Finalize();
	releaseSounds();
}

int GoogleTTS::Initialize( Property parameter )
{	
	boost::recursive_mutex::scoped_lock lock(mutex);
	if (pFmodSystem != NULL)
	{
		if (pFmodSystem->release() != FMOD_OK)
		{
			PrintMessage("Error : GoogleTTS::Initialize()->Can't Initialize() << %s(%d)\n", __FILE__, __LINE__);
			return API_ERROR;
		}
		pFmodSystem = NULL;
	}

	if (FMOD::System_Create(&pFmodSystem) != FMOD_OK)
	{
		PrintMessage("Error : GoogleTTS::Initialize()->Can't Initialize() << %s(%d)\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (pFmodSystem->init(32, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
	{
		PrintMessage("Error : GoogleTTS::Initialize()->Can't Initialize() << %s(%d)\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (SetParameter(parameter) != API_SUCCESS)
	{
		PrintMessage("Error : GoogleTTS::Initialize()->Can't Initialize() << %s(%d)\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	return API_SUCCESS;
}

int GoogleTTS::Finalize()
{
	boost::recursive_mutex::scoped_lock lock(mutex);
	if (Disable() != API_SUCCESS)
	{
		PrintMessage("Error : GoogleTTS::Finalize()->Can't Finalize() << %s(%d)\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (pFmodSystem != NULL)
	{
		if (pFmodSystem->close() != FMOD_OK)
		{
			PrintMessage("Error : GoogleTTS::Finalize()->Can't close() << %s(%d)\n", __FILE__, __LINE__);
			return API_ERROR;
		}

		if (pFmodSystem->release() != FMOD_OK)
		{
			PrintMessage("Error : GoogleTTS::Finalize()->Can't Finalize() << %s(%d)\n", __FILE__, __LINE__);
			return API_ERROR;
		}

		pFmodSystem = NULL;
	}

	releaseSounds();

	return API_SUCCESS;
}

int GoogleTTS::Enable()
{
	if (pFmodSystem == NULL)
	{
		PrintMessage("Error : GoogleTTS::Enable()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	boost::recursive_mutex::scoped_lock lock(mutex);
	updateFmod();
	return API_SUCCESS;
}

int GoogleTTS::Disable()
{
	if (pFmodSystem == NULL)
	{
		PrintMessage("Error : GoogleTTS::Disable()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}
	
	boost::recursive_mutex::scoped_lock lock(mutex);
	StopSpeech();

	return API_SUCCESS;
}

int GoogleTTS::SetParameter( Property parameter )
{
	boost::recursive_mutex::scoped_lock lock(mutex);
	if (Disable() != API_SUCCESS)
	{
		PrintMessage("Error : GoogleTTS::SetParameter()->Can't SetParameter() << %s(%d)\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	const char* TTS_LANGUAGE = "TtsLanguage";
	if (parameter.FindName(TTS_LANGUAGE) == false)
	{
		PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", TTS_LANGUAGE, __FILE__, __LINE__);
		return API_ERROR;
	}
	defaultLanguage = parameter.GetValue(TTS_LANGUAGE);

	if (Enable() != API_SUCCESS)
	{
		PrintMessage("Error : GoogleTTS::SetParameter()->Can't SetParameter() << %s(%d)\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	return API_SUCCESS;
}

int GoogleTTS::GetParameter( Property& parameter )
{
	if (pFmodSystem == NULL)
	{
		PrintMessage("Error : GoogleTTS::GetParameter()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	boost::recursive_mutex::scoped_lock lock(mutex);
	const char* TTS_LANGUAGE = "TtsLanguage";
	parameter.SetValue(TTS_LANGUAGE, defaultLanguage);

	return API_SUCCESS;
}

int GoogleTTS::PlaySpeech( string speechData, int mode )
{
	if (pFmodSystem == NULL)
	{
		PrintMessage("Error : GoogleTTS::PlaySpeech()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}
	
	if (mode == SPEECH_TEXT_STREAM)
	{
		HttpRequest request;
		request.adress = "translate.google.com";
		request.path = "/translate_tts?ie=UTF-8&tl=" + defaultLanguage + "&q=";

		std::wstring unicodeSpeechData = HttpClient::MbsToWcs(speechData);
		std::string utf8SpeechData = HttpClient::WcsToUtf8(unicodeSpeechData);
		request.path += HttpClient::ConvertToHex((unsigned char*)utf8SpeechData.c_str(), utf8SpeechData.size(), "%");

		request.SetProperty("Host", request.adress);
		request.SetProperty("Connection", "close");
		request.SetProperty("User-Agent", "Mozilla/5.0 (OPRoS)");
		request.SetProperty("Accept", "*/*");

		boost::optional<HttpResponse> response = httpClient.Request(request);

		if( !response || response->statusCode != 200)
		{
			PrintMessage("Error : GoogleTTS::PlaySpeech()->Http Response Error(%d)<< %s(%d)\r\n", response->statusCode, __FILE__, __LINE__);
			return API_ERROR;
		}

		boost::recursive_mutex::scoped_lock lock(mutex);

		const char* p = (const char*)(&response->content->operator[](0));

		FMOD_CREATESOUNDEXINFO exInfo = {0, };
		exInfo.cbsize = sizeof(exInfo);
		exInfo.length = response->content->size();

		FMOD::Sound* fmodSound = NULL;
		FMOD::Channel* fmodChannel = NULL;

		if (pFmodSystem->createSound(p, FMOD_2D | FMOD_LOOP_OFF| FMOD_HARDWARE | FMOD_OPENMEMORY, &exInfo, &fmodSound) != FMOD_OK)
		{
			PrintMessage("Error : GoogleTTS::PlaySpeech()->Can't create sound.<< %s(%d)\r\n", __FILE__, __LINE__);
			updateFmod();
			return API_ERROR;
		}

		if (pFmodSystem->playSound(FMOD_CHANNEL_FREE, fmodSound, false, &fmodChannel) != FMOD_OK)
		{
			PrintMessage("Error : GoogleTTS::PlaySpeech()->Can't play sound.<< %s(%d)\r\n", __FILE__, __LINE__);
			updateFmod();
			return API_ERROR;
		}

		fmodChannel->setUserData(this);
		if (fmodChannel->setCallback(&GoogleTTS::FmodChannelCallBack) != FMOD_OK)
		{
			PrintMessage("Error : GoogleTTS::PlaySpeech()->Can't setCallback<< %s(%d)\r\n", __FILE__, __LINE__);
			updateFmod();
			return API_ERROR;
		}

		updateFmod();
		return API_SUCCESS;
	}

	return API_ERROR;
}

int GoogleTTS::StopSpeech()
{	
	if (pFmodSystem == NULL)
	{
		PrintMessage("Error : GoogleTTS::StopSpeech()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	boost::recursive_mutex::scoped_lock lock(mutex);

	FMOD::ChannelGroup* channelGroup = NULL;

	if (pFmodSystem->getMasterChannelGroup(&channelGroup) != FMOD_OK)
	{
		PrintMessage("Error : GoogleTTS::StopSpeech()->Can't get channel group<< %s(%d)\r\n", __FILE__, __LINE__);
		updateFmod();
		return API_ERROR;
	}

	if (channelGroup->stop() != FMOD_OK)
	{
		PrintMessage("Error : GoogleTTS::StopSpeech()->Can't stop speech<< %s(%d)\r\n", __FILE__, __LINE__);
		updateFmod();
		return API_ERROR;
	}

	updateFmod();	
	return API_SUCCESS;
}

void GoogleTTS::updateFmod()
{
	pFmodSystem->update();
	releaseSounds();
}


void GoogleTTS::releaseSounds()
{
	FMOD::Sound* pSound = NULL;
	while (!mCompletedSounds.empty())
	{
		pSound = mCompletedSounds.front();
		mCompletedSounds.pop_front();
		pSound->release();
	}
}

FMOD_RESULT F_CALLBACK GoogleTTS::FmodChannelCallBack( FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2 )
{
	switch(type)
	{
	case FMOD_CHANNEL_CALLBACKTYPE_END :
		{
			FMOD::Channel* fmodChannel = reinterpret_cast<FMOD::Channel*>(channel);
			FMOD::Sound* sound = NULL;
			void* userData = NULL;
			GoogleTTS* gooleTTS = NULL;

			fmodChannel->getUserData(&userData);
			fmodChannel->getCurrentSound(&sound);

			gooleTTS = static_cast<GoogleTTS*>(userData);

			if (sound != NULL && gooleTTS != NULL)
				gooleTTS->mCompletedSounds.push_back(sound);
		}
	}

	return FMOD_OK;
}

#ifdef WIN32
extern "C"
{
	__declspec(dllexport) OprosApi* GetAPI();
	__declspec(dllexport) void ReleaseAPI(OprosApi* p);
}
#else
extern "C"
{
	OprosApi* GetAPI();
	void ReleaseAPI(OprosApi* p);
}
#endif


OprosApi* GetAPI()
{
	return new GoogleTTS();
}

void ReleaseAPI(OprosApi* p)
{
	delete p;
}