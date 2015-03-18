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


// ����
/**
*	KITECH_OccupancyGridGlobalMapComp�� �̵� �κ��� ������ ���� �������� �����ϴ� ������Ʈ�̴�. 
*	KITECH_OccupancyGridGlobalMapComp�� ���� ���·� �����Ǵ� �������� �����ϴ� ������Ʈ�ν�
*	�������� �����ϱ� ���� ǥ�� �������̽��� IGlobalMapCompService�� ��ӹ޾� �����ȴ�.
*	���� ���̺귯���� �����Ǹ� NavigationComponent���� KITECH_OccupancyGridGlobalMapComp�� ���� �Լ��� ȣ���Ͽ� ����Ѵ�.
*	KITECH_OccupancyGridGlobalMapComp�� ������ ��ĳ�� ������ ������ �����͸� �̿��� �������� �����ϴ� ������Ʈ�ν�
*	����� ������ BMP ���� �̸��� ����� �������� �� �ȼ��� ������ ���� �Ÿ� ����, �������� ���� ũ��, �������� ���� ũ��, 
*	������ ��ĳ�� ������ ��ĵ ���� ����, ��ĵ �� ����,
*	�ּ� ���� �Ÿ�, �ִ� ���� �Ÿ�, ��ĵ ������ ������ �˾ƾ� �Ѵ�.
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
	*	�ĸ����͸� �����ϴ� ǥ�� �Լ��̴�.
	*	@param parameter �Ķ����.
	*	@return true - �ĸ����� ���� ����<br>
	*			false - �Ķ���� ���� ����<br>
	*/
	virtual bool SetParameter(Property parameter);

	/**
	*	�ĸ����͸� ��� ǥ�� �Լ��̴�.
	*	@return parameter �Ķ����<br>
	*/
	virtual Property GetParameter();

	virtual int GetError();

	/**
	*	�� ������ �о� �� �����Ϳ� �����ϴ� ǥ�� �Լ��̴�.
	*	@param fileName �� ������ �̸�.
	*	@param cellWidth �� �������� �� �ȼ��� ���� ���� �Ÿ�.
	*	@return true - �� ���� �б� ����<br>
	*			false - �� ���� �б� ����<br>
	*/
	virtual bool LoadMapFile(std::string fileName, double cellWidth);

	/**
	*	�� �����͸� ���Ϸ� �����ϴ� ǥ�� �Լ��̴�.
	*	@param fileName �� ������ �̸�.
	*	@return true - �� ���� ���� ����<br>
	*			false - �� ���� ���� ����<br>
	*/
	virtual bool SaveMapFile(std::string fileName);

	/**
	*	���� �ȼ��� ���� �Ÿ����� ��� ǥ�� �Լ��̴�.
	*	@return �� �ȼ��� ���� �Ÿ���(����:m)<br>
	*/
	virtual double GetMapCellWidth();

	/**
	*	���� ���� ũ�⸦ ��� ǥ�� �Լ��̴�.
	*	@return ���� ũ��<br>
	*/
	virtual int GetMapWidth();

	/**
	*	���� ���� ũ�⸦ ��� ǥ�� �Լ��̴�.
	*	@return ���� ũ��<br>
	*/
	virtual int GetMapHeight();

	/**
	*	�����ʻ󿡼� ���� ��ġ�� ��ֹ� �����͸� ��� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� ���� ��ġ.
	*	@param y ������ �󿡼� ���������� ���� ��ġ.
	*	@return data - �� ������<br>
	*/
	virtual unsigned char GetPixel(double x, double y);

	/**
	*	�����ʻ󿡼� �ȼ� ��ġ�� ��ֹ� �����͸� ��� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@param y ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@return �� ������<br>
	*/
	virtual unsigned char GetPixelCell(int x, int y);

	/**
	*	�����ʻ��� ���� ��ġ�� ��ֹ��� ���� �����͸� ����ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� ���� ��ġ.
	*	@param y ������ �󿡼� ���������� ���� ��ġ.
	*	@param data �� ������<br>
	*	@return void<br>
	*/
	virtual void SetPixel(double x, double y, unsigned char data);

	/**
	*	�����ʻ��� �ȼ� ��ġ�� ��ֹ��� ���� �����͸� ����ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@param y ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@param data �� ������<br>
	*	@return void<br>
	*/
	virtual void SetPixelCell(int x, int y, unsigned char data);

	/**
	*	�Է��ϴ� ���� ��ġ�� ������ ���� ���� �ȿ� �ִ����� Ȯ���ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� ���� ��ġ.
	*	@param y ������ �󿡼� ���������� ���� ��ġ.
	*	@return true - ������ ���� �ȿ� ����<br>
	*			false - ������ ���� �ۿ� ����<br>
	*/
	virtual bool IsIn(double x, double y);

	/**
	*	�Է��ϴ� �ȼ� ��ġ�� ������ ���� ���� �ȿ� �ִ����� Ȯ���ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@param y ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@return true - ������ ���� �ȿ� ����<br>
	*			false - ������ ���� �ۿ� ����<br>
	*/
	virtual bool IsInCell(int x, int y);

	/**
	*	������ �󿡼� �� ���� ���� ��ġ ���� ����ġ ������ ū ��ֹ��� �ִ����� Ȯ���ϴ� ǥ�� �Լ��̴�.
	*	@param x1 ������ �󿡼� �� ���� ���� ������������ ���� ��ġ.
	*	@param y1 ������ �󿡼� �� ���� ���� ������������ ���� ��ġ.
	*	@param x2 ������ �󿡼� �ٸ� �� ���� ���� ������������ ���� ��ġ.
	*	@param y2 ������ �󿡼� �ٸ� �� ���� ���� ������������ ���� ��ġ.
	*	@param data ��ֹ��� �ν��ϱ� ���� ����ġ ��.
	*	@return true - ��ֹ��� ����<br>
	*			false - ��ֹ��� ����<br>
	*/
	virtual bool IsIntersect(double x1, double y1, double x2, double y2, unsigned char data);

	/**
	*	������ �󿡼� �� ���� �ȼ� ��ġ ���� ����ġ ������ ū ��ֹ��� �ִ����� Ȯ���ϴ� ǥ�� �Լ��̴�.
	*	@param x1 ������ �󿡼� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param y1 ������ �󿡼� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param x2 ������ �󿡼� �ٸ� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param y2 ������ �󿡼� �ٸ� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param data ��ֹ��� �ν��ϱ� ���� ����ġ ��.
	*	@return true - ��ֹ��� ����<br>
	*			false - ��ֹ��� ����<br>
	*/
	virtual bool IsIntersectCell(int x1, int y1, int x2, int y2, unsigned char data);

	/**
	*	������ �󿡼� ���� ��ġ�� �� ���� �����ϴ� ���� �׸��� ǥ�� �Լ��̴�.
	*	@param x1 ������ �󿡼� �� ���� ���� ������������ ���� ��ġ.
	*	@param y1 ������ �󿡼� �� ���� ���� ������������ ���� ��ġ.
	*	@param x2 ������ �󿡼� �ٸ� �� ���� ���� ������������ ���� ��ġ.
	*	@param y2 ������ �󿡼� �ٸ� �� ���� ���� ������������ ���� ��ġ.
	*	@param data �����ʿ� ������ ��ֹ� ��.
	*	@return void<br>
	*/
	virtual void DrawLine(double x1, double y1, double x2, double y2, unsigned char data);

	/**
	*	������ �󿡼� ���� ��ġ�� �� ���� �����ϴ� ���� �׸��� ǥ�� �Լ��̴�.
	*	@param x1 ������ �󿡼� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param y1 ������ �󿡼� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param x2 ������ �󿡼� �ٸ� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param y2 ������ �󿡼� �ٸ� �� ���� ���� ������������ �ȼ� ��ġ.
	*	@param data �����ʿ� ������ ��ֹ� ��.
	*	@return void<br>
	*/
	virtual void DrawLineCell(int x1, int y1, int x2, int y2, unsigned char data);

	/**
	*	�������� ��ֹ��� ���� ����ġ ������ ū ��ֹ��� ���� ������ŭ Ȯ���ϴ� ǥ�� �Լ��̴�.
	*	@param bulge Ȯ���ų ����ũ��.
	*	@param threshold Ȯ���ų ��ֹ��� ����ġ ��.
	*	@param data Ȯ�念���� �� ��ֹ� ������ ��.
	*	@return void<br>
	*/
	virtual void ExpandObstacle(int bulge, unsigned char threshold, unsigned char data);

	/**
	*	�������� �޸� ������ ���Ҵ��ϴ� ǥ�� �Լ��Ѵ�.
	*	@param offsetX ���������� ���� ��ġ.
	*	@param offsetY ���������� ���� ��ġ.
	*	@param width ������ ���� ��ġ�κ����� ��.
	*	@param height ������ ���� ��ġ�κ����� ����.
	*	@return void<br>
	*/
	virtual void Reallocate(int offsetX, int offsetY, int width, int height);

	/**
	*	������ �󿡼� �ԷµǴ� ���� ��ġ���� ������ ������ ��ĳ�� ���� �����͸� �����ʿ� ������Ʈ �ϴ� ǥ�� �Լ��̴�.
	*	@param position �κ��� ��ġ
	*	@param sensorValue ������ ������ ��ĳ�� ������ ������
	*	@return void<br>
	*/
	virtual void Update (ObjectLocation position, std::vector<double> sensorValue);

	/**
	*	������ �󿡼� �������� ���� ��ġ�� �ȼ� ��ġ�� ��ȯ�ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� ���� ��ġ.	
	*	@return ������ �󿡼� ���������� �ȼ� ��ġ<br>
	*/
	virtual int M2CUx(double x);

	/**
	*	������ �󿡼� �������� ���� ��ġ�� �ȼ� ��ġ�� ��ȯ�ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� ���� ��ġ.
	*	@return ������ �󿡼� ���������� �ȼ� ��ġ<br>
	*/
	virtual int M2CUy(double y);

	/**
	*	������ �󿡼� �������� �ȼ� ��ġ�� ���� ��ġ�� ��ȯ�ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@return ������ �󿡼� ���������� ���� ��ġ<br>
	*/
	virtual double CU2Mx(int x);

	/**
	*	������ �󿡼� �������� �ȼ� ��ġ�� ���� ��ġ�� ��ȯ�ϴ� ǥ�� �Լ��̴�.
	*	@param x ������ �󿡼� ���������� �ȼ� ��ġ.
	*	@return ������ �󿡼� ���������� ���� ��ġ<br>
	*/
	virtual double CU2My(int y);

private:
	/**
	*	�Ķ���͸� �����ϴ� �����̴�.
	*	�Ķ������ �׸��� ������ ���� �̸��� �������� �� �ȼ��� ������ ���� �Ÿ��� �̵� �κ��� ���� �ݰ��̴�.
	*/
	Property parameter;

	int error;

	/**
	*	�Ķ���� ������ ���� �� �׸񺰷� �����͸� ��´�.
	*	@return true - �ĸ����� ��� ����<br>
	*			false - �Ķ���� ��� ����<br>
	*/
	bool LoadProperty ();

	/**
	*	������ ���� �̸�
	*/
	string mapFileName;

	/**
	*	�������� �� �ȼ��� ���� �Ÿ�
	*/
	double mapResolution;

	/**
	*	�������� ���� ũ��
	*/
	int mapWidth;

	/**
	*	�������� ���� ũ��
	*/
	int mapHeight;

	/**
	*	������ ��ĳ�� ������ ��ĵ ���� ����
	*/
	double laserSensorStartAngle;

	/**
	*	������ ��ĳ�� ������ ��ĵ �� ����
	*/
	double laserSensorEndAngle;

	/**
	*	������ ��ĳ�� ������ �ּ� ���� �Ÿ�
	*/
	double laserSensorMinimumRange;

	/**
	*	������ ��ĳ�� ������ �ִ� ���� �Ÿ�
	*/
	double laserSensorMaximumRange;

	/**
	*	������ ��ĳ�� ������ ���� ������ ����
	*/
	int laserSensorCount;

	/**
	*	������ �˰��� Ŭ����
	*/
	OccupancyGridMap *occupancyGridMap;
};

#endif

