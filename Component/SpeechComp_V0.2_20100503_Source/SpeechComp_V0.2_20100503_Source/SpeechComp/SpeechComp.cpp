/*
 *  Generated sources by OPRoS Component Generator (OCG V2)
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>

#include "SpeechServiceProvided.h"
#include "SpeechComp.h"

#include "OprosPrintMessage.h"

//
// constructor declaration
//
SpeechComp::SpeechComp()
{
	hOprosAPI = NULL;
	speech = NULL;
	error = 0;

	portSetup();
}

//
// constructor declaration (with name)
//
SpeechComp::SpeechComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	speech = NULL;
	error = 0;

	portSetup();
}

//
// destructor declaration
//

SpeechComp::~SpeechComp()
{
	onDestroy();
}

void SpeechComp::portSetup() {

	// provided service port setup
	ProvidedServicePort *pa1;
	pa1=new SpeechServiceProvided(this);

	addPort("SpeechService",pa1);

}

// Call back Declaration
ReturnType SpeechComp::onInitialize()
{
	//	XML에 저장된 프라퍼티를 parameter에 저장
	Property parameter;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);

	//	dll 파일이름을 확인하여 없으면 에러 리턴
	if(parameter.FindName("APIName") == false) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't find the APIName in property\n");
		return OPROS_FIND_PROPERTY_ERROR;
	}


#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("APIName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("APIName").c_str());
		return OPROS_FIND_DLL_ERROR;
	}

	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("DllName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("APIName").c_str());
		return OPROS_FIND_DLL_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return OPROS_LOAD_DLL_ERROR;
	}
#endif

	speech = dynamic_cast<Speech *>(getOprosAPI());
	if(speech == NULL) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't get a handle of Speech API\n");
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return OPROS_LOAD_DLL_ERROR;
	}

	//	API 초기화
	if(speech->Initialize(parameter) != API_SUCCESS) {
		PrintMessage("ERROR : SpeechComp::onInitialize() -> Can't intilaize a Speech API\n");

		delete speech;
		speech = NULL;

#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return OPROS_INITIALIZE_API_ERROR;
	}

	error = 0;

	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onStart()
{
	if(speech == NULL) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	//	API의 enable를 호출
	if(speech->Enable() < 0) {
		PrintMessage("ERROR : SpeechComp::onStart() -> Can't enable the Speech API\n");
		return OPROS_ENABLE_API_ERROR;
	}

	return OPROS_SUCCESS;
}
	
ReturnType SpeechComp::onStop()
{
	if(speech == NULL) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	//	API의 disable 호출
	if(speech->Disable() < 0) {
		PrintMessage("ERROR : SpeechComp::onStop() -> Can't disable the Speech API\n");
		return OPROS_DISABLE_API_ERROR;
	}

	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onDestroy()
{
	//	API 핸들 삭제
	if(speech != NULL) {
		speech->Finalize();
		delete speech;
		speech = NULL;
	}

	//	DLL 핸들 삭제
	if(hOprosAPI != NULL) {
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
	}

	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onEvent(Event *evt)
{
	// user code here

	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onExecute()
{
	// user code here
	std::string message = getProperty("message");
	if(message.empty())
		PlaySpeech("Hello OPRoS", 1);
	else
		PlaySpeech(message.c_str(), 1);

	return OPROS_SUCCESS;
}
	
ReturnType SpeechComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType SpeechComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

bool SpeechComp::SetParameter(Property parameter)
{
	if(speech == NULL) {
		return false;
	}

	if(speech->SetParameter(parameter) < 0) {
		return false;
	}

	return true;
}

Property SpeechComp::GetParameter()
{
	Property parameter;
	error = 0;

	if(speech == NULL) {
		error = -1;
		return parameter;
	}

	if(speech->GetParameter(parameter) < 0) {
		error = -1;
	}

	return parameter;
}

int SpeechComp::GetError()
{
	return error;
}

bool SpeechComp::PlaySpeech(string speechData,int mode)
{
	if(speech == NULL) {
		return false;
	}

	if(speech->PlaySpeech(speechData, mode) < 0) {
		return false;
	}

	return true;
}

bool SpeechComp::StopSpeech()
{
	if(speech == NULL) {
		return false;
	}

	if(speech->StopSpeech() < 0) {
		return false;
	}

	return true;
}




#ifdef MAKE_COMPONENT_DLL
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new SpeechComp();
}

void releaseComponent(Component *com)
{
	delete com;
}


#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(IComponent *com);
}
Component *getComponent()
{
	return new SpeechComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

