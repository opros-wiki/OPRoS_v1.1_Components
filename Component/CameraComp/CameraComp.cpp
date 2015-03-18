/*************************************************************

 file: CameraComp.cpp
 author: E.C. Shin
begin: January 31 2011
 copyright: (c) 2011 KITECH, OPRoS
 email: unchol@kitech.re.kr

***************************************************************

OPRoS source code is provided under a dual license mode:
 LGPL and OPRoS individually.

LGPL: You can redistribute it and/or modify it under the terms
 of the Less GNU General Public License as published by the Free
 Software Foundation, either version 3 of the License.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 Less GNU General Public License for more details. 

 You should have received a copy of the Less GNU General Public
 License along with this program. If not, see 
 <http://www.gnu.org/licenses/>.

OPRoS individual license: An individual license is a license for
 modifying the source code of OPRoS and distiributing it in a
 closed format for commercial purposes.

 If you are interested in this option, please see 
 <http://www.opros.or.kr>.

This license policy may be changed without prior notice.

***************************************************************/

/*
 *  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>



#include "CameraComp.h"
#include "OprosPrintMessage.h"

//
// constructor declaration
//
CameraComp::CameraComp()
{
	hOprosAPI = NULL;
	camera = NULL;
	lastError = OPROS_SUCCESS;

	portSetup();
}

//
// constructor declaration (with name)
//
CameraComp::CameraComp(const std::string &name):Component(name)
{
	hOprosAPI = NULL;
	camera = NULL;
	lastError = OPROS_SUCCESS;
	
	portSetup();
}

//
// destructor declaration
//

CameraComp::~CameraComp() {
}
vector<unsigned char> CameraComp::GetImage()
{
//	_lock.Lock();
	lastError = OPROS_SUCCESS;

	if(camera->GetImage(&imageBuffer[0]) < 0) {
		lastError = OPROS_CALL_API_ERROR;
		vector<unsigned char> tmp(0);
		return tmp;
	}

//	_lock.Unlock();
	return imageBuffer;
}

ReturnType CameraComp::GetError()
{
	return lastError;
}

Property CameraComp::GetParameter()
{
	Property parameter;
	lastError = OPROS_SUCCESS;

	if(camera == NULL) {
		lastError = OPROS_PRECONDITION_NOT_MET;
		PrintMessage("ERROR:CameraComp::GetParameter() -> Can't camera api is NULL\n");
		return parameter;
	}

	if(camera->GetParameter(parameter) < 0) {
		PrintMessage("ERROR:CameraComp::GetParameter() -> Can't get parameter from dll \n");
		lastError = OPROS_CALL_API_ERROR;
	}

	return parameter;
}

ReturnType CameraComp::SetParameter(Property parameter)
{
	if(camera == NULL) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	if(camera->SetParameter(parameter) < 0) {
		return OPROS_CALL_API_ERROR;
	}
	return OPROS_SUCCESS;
}


void CameraComp::portSetup() {
//data port setup
	addPort("imageData", &imageData);

	ProvidedServicePort *pa1;
	pa1=new CameraServiceProvided(this);
	addPort("CameraService",pa1);




	// export variable setup


}

// Call back Declaration
ReturnType CameraComp::onInitialize()
{
	//	XML에 저장된 프라퍼티를 parameter에 저장
//	lastError = OPROS_SUCCESS;
	std::map<std::string, std::string> temp = getPropertyMap();
	parameter.SetProperty(temp);
	
	//	dll 파일이름을 확인하여 없으면 에러 리턴
	if(parameter.FindName("APIName") == false) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't find the APIName in property\n");
		return OPROS_FIND_PROPERTY_ERROR;
	}

#if defined(WIN32)
	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("APIName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("APIName").c_str());
		return OPROS_FIND_DLL_ERROR;
	}
	
	//	API 로드
	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
		return OPROS_LOAD_DLL_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("DllName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't find the %s\n", parameter.GetValue("DllName").c_str());
		return OPROS_FIND_DLL_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	char *lastError = dlerror();
	if(lastError != NULL) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		hOprosAPI = NULL;
		return OPROS_LOAD_DLL_ERROR;
	}
#endif
	
	camera = static_cast<Camera *>(getOprosAPI());
	if(camera == NULL) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't get a handle of Camera API\n");
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return OPROS_LOAD_DLL_ERROR;
	}

	//	API 초기화
	if(camera->Initialize(parameter) != API_SUCCESS) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't intilaize a Camera API\n");
		delete camera;
		camera = NULL;

#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
		return OPROS_INITIALIZE_API_ERROR;
	}
	int width = atoi(parameter.GetValue("Width").c_str());
	int height = atoi(parameter.GetValue("Height").c_str());
	int pixelBytes = atoi(parameter.GetValue("PixelBytes").c_str());

	if(width * height * pixelBytes <= 0) {
		PrintMessage("ERROR : CameraComp::onInitialize() -> Can't enable the Camera API(Camera parameter's size is zero\n");
		return OPROS_ENABLE_API_ERROR;
	}
	PrintMessage("SUCCESS : CameraComp::onInitialize() -> width = %d, height = %d, pixelBytes = %d\n", width, height, pixelBytes);

	imageBuffer.resize(width * height * pixelBytes);

	return OPROS_SUCCESS;
}

ReturnType CameraComp::onStart()
{
	if(camera == NULL) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	//	API의 enable를 호출
	if(camera->Enable() < 0) {
		PrintMessage("ERROR : CameraComp::onStart() -> Can't enable the Camera API\n");
		return OPROS_ENABLE_API_ERROR;
	}

	return OPROS_SUCCESS;
}
	
ReturnType CameraComp::onStop()
{
	if(camera == NULL) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	//	API의 disable 호출
	if(camera->Disable() < 0) {
		PrintMessage("ERROR : CameraComp::onStop() -> Can't disable the Camera API\n");
		return OPROS_DISABLE_API_ERROR;
	}

	PrintMessage("SUCCESS : CameraComp::onStop()\n");
	return OPROS_SUCCESS;
}

ReturnType CameraComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CameraComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CameraComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CameraComp::onDestroy()
{
	//	API 핸들 삭제
	if(camera != NULL) {
		camera->Finalize();
		delete camera;
		camera = NULL;
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

ReturnType CameraComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CameraComp::onExecute()
{
//	PrintMessage("INFO:CameraComp::onExecute() Start. \n");
//	_lock.Lock();
	if(camera == NULL) {
		PrintMessage("ERROR:CameraComp::onExecute() -> Camera device is NULL.\n");
		return OPROS_PRECONDITION_NOT_MET;
	}

	if(camera->GetImage(&imageBuffer[0]) < 0) {
		PrintMessage("ERROR:CameraComp::onExecute() -> Can't get image from device.\n");
		return OPROS_CALL_API_ERROR;
	}

	imageData.push(imageBuffer);

//	PrintMessage("INFO:CameraComp::onExecute() End. \n");
//	_lock.Unlock();
	return OPROS_SUCCESS;
}
	
ReturnType CameraComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CameraComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}







#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new CameraComp();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new CameraComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

