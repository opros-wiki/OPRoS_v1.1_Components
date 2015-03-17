#include "GoogleTTS.h"

#include <boost/thread.hpp>

int main()
{
	GoogleTTS tts;

	Property property;
	property.SetValue("TtsLanguage", "ko");

	tts.Initialize(property);
	tts.Enable();

	boost::this_thread::sleep(boost::posix_time::millisec(1000));

	for (;;)
	{
		std::string message;

		std::cout << "//메시지를 입력해 주세요." << std::endl;

		std::getline(std::cin, message);

		if (message == "/stop")
		{
			break;
		}
		else
		{
			tts.PlaySpeech(message, SPEECH_TEXT_STREAM);
		}
	}
	tts.Finalize();

	getchar();
	return 0;
}
