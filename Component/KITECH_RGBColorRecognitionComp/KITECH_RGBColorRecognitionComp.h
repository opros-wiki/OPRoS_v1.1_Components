#ifndef _KITECH_RGBColorRecognitionComponent_COMPONENT_H
#define _KITECH_RGBColorRecognitionComponent_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2)
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>
#include <ApiTypes.h>

#if defined(WIN32)
#include <windows.h>
#endif

#include "VisionRecognitionComp.h"
#include "VisionRecognitionServiceProvided.h"

//개요
/**
 *	<table><tr><td>
 *	KITECH_RGBColorRecognitionComp는 영상을 입력받아 색상을 인식하는 컴포넌트이다.
 *	색상에 대한 데이터베이스를 구하여 카메라를 통해 받은 영상에서 색상을 판별해낸다.
 *	KITECH_RGBColorRecognitionComp에는 KITECH_RGBColorRecognitionComp.xml,VisionRecognitionServiceProvided.h가
 *	포함되어야 하며, KITECH_RGBColorRecognitionComp.xml는 KITECH_RGBColorRecognitionComp의 프로파일이 저장되어 있다.
 *	VisionRecognitionServiceProvided.h는 이 컴포넌트에서 제공하는 서비스 포트를 호출할 수 있는
 *	인터페이스를 제공한다.
 *	</td></tr><tr><td>
 *	<center><img src="../../images/uml/UML_ColorRecognition.jpg"></center>
 *	</td></tr></table>
 * 이 컴포넌트를 실행하기 위해서는 OpenCV 라이브러리의 Lib 디렉토리에 있는
 * cv200.dll cvaux200.dll cxcore200.dll highgui200.dll 파일을 실행파일이 있는 폴더에 두어야 한다.
*/
class KITECH_RGBColorRecognitionComp : public Component, public IVisionRecognitionService
{
protected:
	// 속성
	//	Variables for KITECH_RGBColorRecognitionComp
	/**
	 *	KITECH_RGBColorRecognitionComp의 프로파일정보를 저장하는 변수이다.<br>
	 *	<!-- [표] Parameter -->
	 *	<table border=1>
	 *	<tr><td bgcolor="#EEEEEE"><center><b>이름</b></center></td> <td bgcolor="#EEEEEE"><center><b>타입</b></center></td> <td bgcolor="#EEEEEE" colspan=2><center><b>설명</b></center></td></tr>
	 *	<tr><td><b>DBName</b></td> <td>std::string</td> <td>컴포넌트에서 사용할 DataBase파일로서 컬러와 색 영역 정보가 포함되어 있다.</td></tr>
	 *	</table>
	*/
	Property parameter;

	std::vector<ColorRange>	_colorRange;

public:
	/**
	 *	KITECH_RGBColorRecognitionComp 클래스의 기본 생성자이다.<br>
	 *	포트를 초기화시키는 함수를 호출한다.
	*/
	KITECH_RGBColorRecognitionComp();

	/**
	 *	KITECH_RGBColorRecognitionComp 클래스의 전달인자를 포함한 생성자이다.<br>
	 *	포트를 초기화시키는 함수를 호출한다.
	 *	@param compId 컴포넌트의 ID.
	*/
	KITECH_RGBColorRecognitionComp(const std::string &compId);

	/**
	 *	KITECH_RGBColorRecognitionComp 클래스의 기본 소멸자이다.
	*/
	virtual ~KITECH_RGBColorRecognitionComp();

	/**
	 *	KITECH_RGBColorRecognitionComp에서 제공하는 서비스 포트를 초기화하고 등록한다.
	*/
	virtual void portSetup();

protected:
	// Callback API
	/**
	 *	컴포넌트가 초기화될 때 호출되는 콜백함수이다.
	 *	XML에 저장된 프로퍼티정보를 읽어서 DLL을 로드하고,
	 *	필요한 API를 로드한 후 초기화 한다.
	 *	@return OPROS_SUCCESS - 컴포넌트 초기화 성공<br>
	 *			OPROS_FIND_PROPERTY_ERROR - 프로퍼티정보에 DLL파일이름이 없음<br>
	 *			OPROS_FIND_DLL_ERROR - DLL을 찾을 수 없음<br>
	 *			OPROS_LOAD_DLL_ERROR - DLL을 로드할 수 없음<br>
	 *			OPROS_INITIALIZE_API_ERROR - API초기화 도중 에러발생<br>
	 */
	virtual ReturnType onInitialize();

	/**
	 *	컴포넌트가 활성화될 때 호출되는 콜백함수이다.
	 *	@return OPROS_SUCCESS - 컴포넌트 활성화 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL이거나 컴포넌트의 상태가 READY또는 INACTIVE 상태가 아님<br>
	 *			OPROS_ENABLE_API_ERROR - API활성화 도중 에러발생<br>
	 */
	virtual ReturnType onStart();

	/**
	 *	컴포넌트가 비활성화될 때 호출되는 콜백함수이다.
	 *	@return OPROS_SUCCESS - 컴포넌트 비활성화 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL이거나 컴포넌트의 상태가 ACTIVE 상태가 아님<br>
	 *			OPROS_DISABLE_API_ERROR - API비활성화 도중 에러발생<br>
	 */
	virtual ReturnType onStop();

	/**
	 *	컴포넌트가 리셋 될 때 호출되는 콜백함수이다.
	 *	리셋작업을 수행할 때 API호출 순서는 Finalize, Initialize, Enable순이다.
	 *	사용하는 모든 API에 대해 같은 작업을 수행한다.
	 *	호출에 실패할 경우 에러를 리턴 한다. Enable호출에 실패하면 다시 Finalize를 호출한다.
	 *	@return OPROS_SUCCESS - 컴포넌트 리셋 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL인 상태<br>
	 *			OPROS_RESET_API_ERROR - API리셋 도중 에러발생<br>
	 */
	virtual ReturnType onReset();

	/**
	 *	컴포넌트에 에러가 발생했을 때 호출되는 콜백함수이다.
	 *	에러가 발생하면 API의 Disable을 호출하여 비활성화 시킨다.
	 *	사용하는 모든 API에 대해 같은 작업을 수행한다.
	 *	@return OPROS_SUCCESS - 에러발생 후 비활성화 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL인 상태<br>
	 *			OPROS_DISABLE_API_ERROR - API비활성화 도중 에러발생<br>
	 */
	virtual ReturnType onError();

	/**
	 *	컴포넌트의 에러를 복구할 때 호출되는 콜백함수이다.
	 *	에러 복구 시 API호출 순서는 Finalize, Initialize순이다.
	 *	사용하는 모든 API에 대해 같은 작업을 수행한다.
	 *	@return OPROS_SUCCESS - 에러복구 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL인 상태<br>
	 *			OPROS_RECOVER_API_ERROR - API복구 도중 에러발생<br>
	 */
	virtual ReturnType onRecover();

	/**
	 *	컴포넌트가 종료될 때 호출되는 콜백함수이다.
	 *	Finalize를 호출하여 API를 종료하고, API핸들과 DLL핸들을 삭제한다.
	 *	사용하는 모든 API에 대해 같은 작업을 수행한다.
	 *	@return OPROS_SUCCESS - 컴포넌트 종료 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL인 상태<br>
	 *			OPROS_FINALIZE_API_ERROR - API종료 도중 에러발생<br>
	 */
	virtual ReturnType onDestroy();

	void GetDataFromDB(const char* database);
	void SetDataToDB(const char* database);

public:
	/**
	 *	컴포넌트가 이벤트를 받았을 때 호출되는 콜백함수이다.
	 *	KITECH_RGBColorRecognitionComp에서는 사용되지 않음.
	 *	@return OPROS_SUCCESS &nbsp;
	 */
	virtual ReturnType onEvent(Event *evt);

	/**
	 *	컴포넌트가 주기적으로 동작할 때 호출되는 콜백함수이다.
	 *	입력 데이터포트에서 카메라의 영상이 입력되면
	 *	영상을 버퍼에 복사하고, recognize를 호출하여 영상인식을 수행한다.
	 *	@return OPROS_SUCCESS - 성공<br>
	 *			OPROS_PRECONDITION_NOT_MET - API핸들이 NULL인 상태<br>
	 */
	virtual ReturnType onExecute();

	/**
	 *	컴포넌트가 주기적으로 동작할 때 호출되는 콜백함수이다.
	 *	KITECH_RGBColorRecognitionComp에서는 사용되지 않음.
	 *	@return OPROS_SUCCESS &nbsp;
	 */
	virtual ReturnType onUpdated();

	/**
	 *	컴포넌트의 동작 주기가 변경되었을 때 호출되는 콜백함수이다.
	 *	KITECH_RGBColorRecognitionComp에서는 사용되지 않음.
	 *	@return OPROS_SUCCESS &nbsp;
	 */
	virtual ReturnType onPeriodChanged();


	// 서비스포트
	/**
	 *	KITECH_RGBColorRecognitionComp의 프로파일을 변경한다.
	 *	다른 컴포넌트에서 이 컴포넌트의 프로파일을 변경할 때 사용된다.
	 *	API의 SetParameter를 호출하여 그 결과를 리턴한다.
	 *	@param parameter 설정할 파라미터 정보
	 *	@return true - 파라미터 데이터설정 성공<br>
	 *			false - 파라미터 데이터설정을 실패했거나, API핸들이 NULL인 경우, 또는<br>
	 * &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 컴포넌트의 상태가 ACTIVE, INACTIVE가 아닌 다른 상태인 경우<br>
	 */
	virtual bool SetParameter(Property parameter);

	/**
	 *	KITECH_RGBColorRecognitionComp의 프로파일을 다른 컴포넌트에서 가져갈 때 사용된다.
	 *	@return property - 파라미터 데이터<br>
	 */
	virtual Property GetParameter();

	/**
	*	색상인식을 수행하는 함수이다.
	*	recognize 함수를 실행하면 카메라 컴포넌트와 연결된 service port로 영상을 읽어와서
	*	색상을 인식하여 영상 안의 특정 영역을 나타내는 4개의 좌표 형태로 넘겨준다.
	*	@param image 영상 이미지<br>
	*          width 이미지의 가로 해상도<br>
	*          height 이미지의 세로 해상도<br>
	*          pixelBytes 이미지의 픽셀당 바이트 수<br>
	*	@return 영상안의 인식된 색상의 영역 좌표<br>
	*/
	virtual vector<VisionRecognitionResult> Recognize(vector<unsigned char> image,int width,int height,int pixelBytes);

	int GetError() {return 0;};
};

#endif

