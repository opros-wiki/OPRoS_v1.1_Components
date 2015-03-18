#pragma once

#include "ApiTypes.h"
#include "OprosMath.h"
#include "GridMap.h"

typedef struct{
	double MaxRange;
	double MinRange;
	double *SensorData;
	double *SensorAngle;
	double beta;
	double alpha;
	int NumberOfSensor;
} Sensor;


class OccupancyGridMap : public GridMap<unsigned char>  
{
public:
	OccupancyGridMap(int width, int height, double cellWidth, 
		int sensorCount, double startAngle, double endAngle, double rangeMin, double rangeMax);
	virtual ~OccupancyGridMap();

	//void InitialMap(long XSize, long YSize);
	void OccupancyGridMapping(ObjectLocation position, double value[]);
	void GetMapData(char** Dest );
	void SaveMap(const char* filename);

	double _cellWidth;

	inline int M2CUx(double x_)	{ return INTEGER(x_ / _cellWidth - _offsetX); }
	inline int M2CUy(double y_)	{ return INTEGER(y_ / _cellWidth - _offsetY); }
	inline double CU2Mx(int x_)	{ return (x_ + _offsetX) * _cellWidth; }
	inline double CU2My(int y_)	{ return (y_ + _offsetY) * _cellWidth; }

private:
	long MapYSize;
	long MapXSize;
	unsigned char** OccMapData;
	double MapResolution;

	Sensor *_Zt;

	int		_sensorCount;
	double	_startAngle;
	double	_endAngle;
	double	_rangeMin;
	double	_rangeMax;

	double FixAng(double angle);
	double arctan(double dY, double dX);
	double CalcDistance(ObjectLocation position, int  x, int y);
	int argmin(double angle, Sensor* Stemp);
	int InverseSensorModel(int i, int j, ObjectLocation position);
	void convert2bmpfile();
};


