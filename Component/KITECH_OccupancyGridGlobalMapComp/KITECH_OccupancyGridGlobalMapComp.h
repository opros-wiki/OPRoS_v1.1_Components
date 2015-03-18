#ifndef _KITECH_OccupancyGridGlobalMapComp_COMPONENT_H
#define _KITECH_OccupancyGridGlobalMapComp_COMPONENT_H
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

#include "GlobalMapServiceProvided.h"
#include "OccupancyGridMap.h"


// 개요
/**
*	KITECH_OccupancyGridGlobalMapComp는 이동 로봇의 주행을 위한 전역맵을 제어하는 컴포넌트이다. 
*	KITECH_OccupancyGridGlobalMapComp는 격자 형태로 구성되는 전역맵을 제어하는 컴포넌트로써
*	전역맵을 제어하기 위한 표준 인터페이스인 IGlobalMapCompService를 상속받아 구현된다.
*	동적 라이브러리로 생성되며 NavigationComponent에서 KITECH_OccupancyGridGlobalMapComp의 서비스 함수를 호출하여 사옹한다.
*	KITECH_OccupancyGridGlobalMapComp는 레이저 스캐너 센서의 측정된 데이터를 이용해 전역맵을 생성하는 컴포넌트로써
*	저장될 전역맵 BMP 파일 이름과 저장될 전역맵의 한 픽셀당 가지는 실제 거리 정보, 전역맵의 가로 크기, 전역맵의 세로 크기, 
*	레이저 스캐너 센서의 스캔 시작 각도, 스캔 끝 각도,
*	최소 측정 거리, 최대 측정 거리, 스캔 포인터 개수를 알아야 한다.
*/
class KITECH_OccupancyGridGlobalMapComp: public Component, public IGlobalMapService
{
public:
	KITECH_OccupancyGridGlobalMapComp();
	KITECH_OccupancyGridGlobalMapComp(const std::string &compId);
	virtual ~KITECH_OccupancyGridGlobalMapComp();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();

	/**
	*	파리미터를 설정하는 표준 함수이다.
	*	@param parameter 파라미터.
	*	@return true - 파리미터 설정 성공<br>
	*			false - 파라미터 설정 실패<br>
	*/
	virtual bool SetParameter(Property parameter);

	/**
	*	파리미터를 얻는 표준 함수이다.
	*	@return parameter 파라미터<br>
	*/
	virtual Property GetParameter();

	virtual int GetError();

	/**
	*	맵 파일을 읽어 맵 데이터에 저장하는 표준 함수이다.
	*	@param fileName 맵 파일의 이름.
	*	@param cellWidth 맵 데이터의 한 픽셀에 대한 실제 거리.
	*	@return true - 맵 파일 읽기 성공<br>
	*			false - 맵 파일 읽기 실패<br>
	*/
	virtual bool LoadMapFile(std::string fileName, double cellWidth);

	/**
	*	맵 데이터를 파일로 저장하는 표준 함수이다.
	*	@param fileName 맵 파일의 이름.
	*	@return true - 맵 파일 저장 성공<br>
	*			false - 맵 파일 저장 실패<br>
	*/
	virtual bool SaveMapFile(std::string fileName);

	/**
	*	맵의 픽셀당 실제 거리값을 얻는 표준 함수이다.
	*	@return 한 픽셀당 실제 거리값(단위:m)<br>
	*/
	virtual double GetMapCellWidth();

	/**
	*	맵의 가로 크기를 얻는 표준 함수이다.
	*	@return 가로 크기<br>
	*/
	virtual int GetMapWidth();

	/**
	*	맵의 세로 크기를 얻는 표준 함수이다.
	*	@return 세로 크기<br>
	*/
	virtual int GetMapHeight();

	/**
	*	전역맵상에서 실제 위치의 장애물 데이터를 얻는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 실제 위치.
	*	@param y 전역맵 상에서 세로축으로 실제 위치.
	*	@return data - 맵 데이터<br>
	*/
	virtual unsigned char GetPixel(double x, double y);

	/**
	*	전역맵상에서 픽셀 위치의 장애물 데이터를 얻는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 픽셀 위치.
	*	@param y 전역맵 상에서 세로축으로 픽셀 위치.
	*	@return 맵 데이터<br>
	*/
	virtual unsigned char GetPixelCell(int x, int y);

	/**
	*	전역맵상의 실제 위치에 장애물에 대한 데이터를 기록하는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 실제 위치.
	*	@param y 전역맵 상에서 세로축으로 실제 위치.
	*	@param data 맵 데이터<br>
	*	@return void<br>
	*/
	virtual void SetPixel(double x, double y, unsigned char data);

	/**
	*	전역맵상의 픽셀 위치에 장애물에 대한 데이터를 기록하는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 픽셀 위치.
	*	@param y 전역맵 상에서 세로축으로 픽셀 위치.
	*	@param data 맵 데이터<br>
	*	@return void<br>
	*/
	virtual void SetPixelCell(int x, int y, unsigned char data);

	/**
	*	입력하는 실제 위치가 전역맵 상의 영역 안에 있는지를 확인하는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 실제 위치.
	*	@param y 전역맵 상에서 세로축으로 실제 위치.
	*	@return true - 전역맵 영역 안에 있음<br>
	*			false - 전역맵 영역 밖에 있음<br>
	*/
	virtual bool IsIn(double x, double y);

	/**
	*	입력하는 픽셀 위치가 전역맵 상의 영역 안에 있는지를 확인하는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 픽셀 위치.
	*	@param y 전역맵 상에서 세로축으로 픽셀 위치.
	*	@return true - 전역맵 영역 안에 있음<br>
	*			false - 전역맵 영역 밖에 있음<br>
	*/
	virtual bool IsInCell(int x, int y);

	/**
	*	전역맵 상에서 두 점의 실제 위치 선상에 문턱치 값보다 큰 장애물이 있는지를 확인하는 표준 함수이다.
	*	@param x1 전역맵 상에서 한 점에 대한 가로축으로의 실제 위치.
	*	@param y1 전역맵 상에서 한 점에 대한 세로축으로의 실제 위치.
	*	@param x2 전역맵 상에서 다른 한 점에 대한 가로축으로의 실제 위치.
	*	@param y2 전역맵 상에서 다른 한 점에 대한 세로축으로의 실제 위치.
	*	@param data 장애물로 인식하기 위한 문턱치 값.
	*	@return true - 장애물이 있음<br>
	*			false - 장애물이 없음<br>
	*/
	virtual bool IsIntersect(double x1, double y1, double x2, double y2, unsigned char data);

	/**
	*	전역맵 상에서 두 점의 픽셀 위치 선상에 문턱치 값보다 큰 장애물이 있는지를 확인하는 표준 함수이다.
	*	@param x1 전역맵 상에서 한 점에 대한 가로축으로의 픽셀 위치.
	*	@param y1 전역맵 상에서 한 점에 대한 세로축으로의 픽셀 위치.
	*	@param x2 전역맵 상에서 다른 한 점에 대한 가로축으로의 픽셀 위치.
	*	@param y2 전역맵 상에서 다른 한 점에 대한 세로축으로의 픽셀 위치.
	*	@param data 장애물로 인식하기 위한 문턱치 값.
	*	@return true - 장애물이 있음<br>
	*			false - 장애물이 없음<br>
	*/
	virtual bool IsIntersectCell(int x1, int y1, int x2, int y2, unsigned char data);

	/**
	*	전역맵 상에서 실제 위치의 두 점을 연결하는 선을 그리는 표준 함수이다.
	*	@param x1 전역맵 상에서 한 점에 대한 가로축으로의 실제 위치.
	*	@param y1 전역맵 상에서 한 점에 대한 세로축으로의 실제 위치.
	*	@param x2 전역맵 상에서 다른 한 점에 대한 가로축으로의 실제 위치.
	*	@param y2 전역맵 상에서 다른 한 점에 대한 세로축으로의 실제 위치.
	*	@param data 전역맵에 저장할 장애물 값.
	*	@return void<br>
	*/
	virtual void DrawLine(double x1, double y1, double x2, double y2, unsigned char data);

	/**
	*	전역맵 상에서 실제 위치의 두 점을 연결하는 선을 그리는 표준 함수이다.
	*	@param x1 전역맵 상에서 한 점에 대한 가로축으로의 픽셀 위치.
	*	@param y1 전역맵 상에서 한 점에 대한 세로축으로의 픽셀 위치.
	*	@param x2 전역맵 상에서 다른 한 점에 대한 가로축으로의 픽셀 위치.
	*	@param y2 전역맵 상에서 다른 한 점에 대한 세로축으로의 픽셀 위치.
	*	@param data 전역맵에 저장할 장애물 값.
	*	@return void<br>
	*/
	virtual void DrawLineCell(int x1, int y1, int x2, int y2, unsigned char data);

	/**
	*	전역맵의 장애물에 대해 문턱치 값보다 큰 장애물을 일정 영역만큼 확장하는 표준 함수이다.
	*	@param bulge 확장시킬 영역크기.
	*	@param threshold 확장시킬 장애물의 문턱치 값.
	*	@param data 확장영역에 들어갈 장애물 데이터 값.
	*	@return void<br>
	*/
	virtual void ExpandObstacle(int bulge, unsigned char threshold, unsigned char data);

	/**
	*	전역맵의 메모리 영역을 재할당하는 표준 함수한다.
	*	@param offsetX 가로축으로 시작 위치.
	*	@param offsetY 세로축으로 시작 위치.
	*	@param width 가로축 시작 위치로부터의 폭.
	*	@param height 세로축 시작 위치로부터의 높이.
	*	@return void<br>
	*/
	virtual void Reallocate(int offsetX, int offsetY, int width, int height);

	/**
	*	전역맵 상에서 입력되는 실제 위치에서 측정된 레이저 스캐너 센서 데이터를 전역맵에 업데이트 하는 표준 함수이다.
	*	@param position 로봇의 위치
	*	@param sensorValue 측정된 레이저 스캐너 센서의 데이터
	*	@return void<br>
	*/
	virtual void Update (ObjectLocation position, std::vector<double> sensorValue);

	/**
	*	전역맵 상에서 가로축의 실제 위치를 픽셀 위치로 변환하는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 실제 위치.	
	*	@return 전역맵 상에서 가로축으로 픽셀 위치<br>
	*/
	virtual int M2CUx(double x);

	/**
	*	전역맵 상에서 세로축의 실제 위치를 픽셀 위치로 변환하는 표준 함수이다.
	*	@param x 전역맵 상에서 세로축으로 실제 위치.
	*	@return 전역맵 상에서 세로축으로 픽셀 위치<br>
	*/
	virtual int M2CUy(double y);

	/**
	*	전역맵 상에서 가로축의 픽셀 위치를 실제 위치로 변환하는 표준 함수이다.
	*	@param x 전역맵 상에서 가로축으로 픽셀 위치.
	*	@return 전역맵 상에서 가로축으로 실제 위치<br>
	*/
	virtual double CU2Mx(int x);

	/**
	*	전역맵 상에서 세로축의 픽셀 위치를 실제 위치로 변환하는 표준 함수이다.
	*	@param x 전역맵 상에서 세로축으로 픽셀 위치.
	*	@return 전역맵 상에서 세로축으로 실제 위치<br>
	*/
	virtual double CU2My(int y);

private:
	/**
	*	파라미터를 저장하는 변수이다.
	*	파라미터의 항목은 전역맵 파일 이름과 전역맵의 한 픽셀당 가지는 실제 거리와 이동 로봇의 안전 반경이다.
	*/
	Property parameter;

	int error;

	/**
	*	파라미터 변수로 부터 각 항목별로 데이터를 얻는다.
	*	@return true - 파리미터 얻기 성공<br>
	*			false - 파라미터 얻기 실패<br>
	*/
	bool LoadProperty ();

	/**
	*	전역맵 파일 이름
	*/
	string mapFileName;

	/**
	*	전역맵의 한 픽셀당 실제 거리
	*/
	double mapResolution;

	/**
	*	전역맵의 가로 크기
	*/
	int mapWidth;

	/**
	*	전역맵의 세로 크기
	*/
	int mapHeight;

	/**
	*	레이저 스캐너 센서의 스캔 시작 각도
	*/
	double laserSensorStartAngle;

	/**
	*	레이저 스캐너 센서의 스캔 끝 각도
	*/
	double laserSensorEndAngle;

	/**
	*	레이저 스캐너 센서의 최소 측정 거리
	*/
	double laserSensorMinimumRange;

	/**
	*	레이저 스캐너 센서의 최대 측정 거리
	*/
	double laserSensorMaximumRange;

	/**
	*	레이저 스캐너 센서의 측정 포인터 개수
	*/
	int laserSensorCount;

	/**
	*	전역맵 알고리즘 클래스
	*/
	OccupancyGridMap *occupancyGridMap;
};

#endif

