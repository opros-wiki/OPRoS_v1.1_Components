// OccupancyGridMap.cpp: implementation of the OccupancyGridMap class.
//
//////////////////////////////////////////////////////////////////////

#include "OccupancyGridMap.h"
#include <math.h>
#include <iostream>
#include <string.h>
#include "OprosMath.h"
#include "bmp256.h"

using namespace std;

#define occ_lo		10
#define occ_lfree	1
#define occ_locc	19


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
OccupancyGridMap::OccupancyGridMap(int width, int height, double cellWidth,
								   int sensorCount, double startAngle, double endAngle, double rangeMin, double rangeMax) :
	_cellWidth(cellWidth), GridMap<unsigned char>(0,0,width, height), 
	_sensorCount(sensorCount), _startAngle(startAngle), _endAngle(endAngle), _rangeMin(rangeMin), _rangeMax(rangeMax)
{
	MapXSize = width;
	MapYSize = height;
	Clear(127);

	_Zt = new Sensor;

	_Zt->alpha = _cellWidth*4;
	_Zt->beta = (_endAngle - _startAngle) / (_sensorCount-1);
	_Zt->MaxRange = _rangeMax;
	_Zt->MinRange = _rangeMin;
	_Zt->NumberOfSensor = _sensorCount;
	_Zt->SensorAngle = new double[_sensorCount];
	
	for (int i = 0; i < _sensorCount; ++i) {
		_Zt->SensorAngle[i] = (M_PI + M_PI_2) + i*_Zt->beta;
		_Zt->SensorAngle[i] = FixAng(_Zt->SensorAngle[i]);
	}
	_Zt->SensorData = new double[_sensorCount];
}

OccupancyGridMap::~OccupancyGridMap()
{
	//SaveMap("occ_map.bmp");

	delete [] _Zt->SensorAngle;
	delete [] _Zt->SensorData;
	delete _Zt;
}
/*
void OccupancyGridMap::InitialMap(long XSize, long YSize)
{
	MapXSize=XSize;
	MapYSize=YSize;

	OccMapData = new unsigned char*[YSize];
	for(int i=0; i<YSize ; i++) 
		OccMapData[i] = new unsigned char[XSize];

	for(int i=0 ; i<YSize ; i++)
		for(int j=0 ; j<XSize ; j++) 
			OccMapData[i][j]=127;
}
*/
int OccupancyGridMap::InverseSensorModel(int x, int y, ObjectLocation position )
{
	double xi, yi;
	double r, phi;
	int k;
	double min;
	int ret;
	double atantheta;
	double diffangle;
	double robot_th;

	xi = ((double)x+0.5)*_cellWidth;
	yi = ((double)y+0.5)*_cellWidth;
	
	r = CalcDistance(position, x, y);
	
	robot_th = FixAng(position.theta);

	atantheta = arctan( yi-(position.y), xi-(position.x) );
	
	phi=FixAng(atantheta-robot_th);

	k = argmin(phi, _Zt);

	if( _Zt->MaxRange > (_Zt->SensorData[k] + 0.5*_Zt->alpha)) 
		min = _Zt->SensorData[k] + 0.5*_Zt->alpha;
	else 
		min = _Zt->MaxRange;

	diffangle = fabs( phi - _Zt->SensorAngle[k] );

	if( (r>min) || (diffangle > (_Zt->beta*0.5)) ) 
		return ret = occ_lo;
	if( (_Zt->SensorData[k] < _Zt->MaxRange) && ( fabs(r-_Zt->SensorData[k]) < (_Zt->alpha*0.5) ) ) 
		return ret = occ_locc;
	if( (r <= _Zt->SensorData[k]) ) 
		return ret = occ_lfree;

	return 0;
}

int OccupancyGridMap::argmin(double angle, Sensor *Stemp)
{
	double value, min;
	int ret;

	value = angle-Stemp->SensorAngle[0];
	value = fabs(value);
	min = value;
	ret = 0;

	for( int i=1; i<Stemp->NumberOfSensor ; i++)
	{
		value = angle-Stemp->SensorAngle[i];
		value = fabs(value);
		if( value<min )
		{
			min = value;
			ret = i;
		}			
	}
	return ret;	
}

void OccupancyGridMap::OccupancyGridMapping(ObjectLocation position, double value[])
{
	int x, y;
	double r;
	int tempmodel;
	int xi, yi;
	int rangeMax=(int)(_rangeMax/_cellWidth);

	rangeMax*=2;

	for( int i=0 ; i<_Zt->NumberOfSensor ; i++) _Zt->SensorData[i]=value[i];

	xi = (int)(position.x/_cellWidth) ;
	yi = (int)(position.y/_cellWidth) ;

	y = yi-rangeMax;

	while( (y!=MapYSize) && (y!=yi+rangeMax) )
	{

		if( y<0 ) 
			y=0;
		else if( y>MapYSize-1) 
			y=MapYSize-1;
		
		x = xi-rangeMax;

		while( (x != MapXSize) && (x != xi+rangeMax) )
		{
			if( x<0 ) 
				x=0;
			else if( x>MapXSize-1 ) 
				x=MapXSize-1;
			
			r=sqrt( pow( (x+0.5)*_cellWidth - (position.x), 2) + pow( (y+0.5)*_cellWidth - (position.y), 2) );

			if( r < (_rangeMax)) {
				tempmodel = InverseSensorModel(x,y,position);

				if( (tempmodel+GetPixelCELL(x,y)-occ_lo) >= 250-occ_locc ) 
					SetPixelCELL(x, y, 250-occ_locc);
				else if( (tempmodel+GetPixelCELL(x,y)-occ_lo) <= occ_locc+1 ) 
					SetPixelCELL(x, y, occ_locc+1);
				else 
					SetPixelCELL(x, y, tempmodel+GetPixelCELL(x, y)-occ_lo);
			}
			x++;
		}
		y++;
	}
}

void OccupancyGridMap::SaveMap(const char* filename)
{
	unsigned char pallete[256][4];

	for( int i = 0;  i<256 ; i++)
	{
		unsigned char c = (unsigned char)(255 - i);
		pallete[i][0] = c;
		pallete[i][1] = c;
		pallete[i][2] = c;
		pallete[i][3] = 0;
	}
	
	unsigned char *imgdata = new unsigned char[_width*_height];

	for (int i = _height - 1; 0 <= i; i--) {
		memcpy(imgdata + (_height - i - 1)*_width , _cp + _width * i, sizeof(unsigned char)*_width);
	}

	WriteBmp256(filename, MapXSize, MapYSize, imgdata, pallete);

	delete [] imgdata;
}

void OccupancyGridMap::GetMapData(char** Dest)
{
	for(int i=0; i<MapYSize ; i++)
	{
		for(int j=0; j<MapXSize ; j++)
		{
			Dest[i][j]=255-OccMapData[i][j];
		}
	}
}

double OccupancyGridMap::CalcDistance(ObjectLocation position, int x, int y)
{
	double ret;
	double Xd, Yd;

	Xd = (double)x*_cellWidth - (position.x);
	Yd = (double)y*_cellWidth - (position.y);

	ret = sqrt(Xd*Xd + Yd*Yd);

	return ret;
}

double OccupancyGridMap::arctan(double dY, double dX)
{
	double ret, retemp;

	retemp=atan2(dY, dX);

	ret = FixAng(retemp);
	
	return ret;
}

double OccupancyGridMap::FixAng(double angle)
{
	double ret;
	int quotient;
	
	quotient = (int)(fabs(angle)/(M_PI*2));
	
	if( angle <0 )
	{
		ret = angle + (double)(quotient+1)*(M_PI*2);
	}
	else 
	{
		ret = angle - (double)quotient*(M_PI*2);
	}
	return ret;
}