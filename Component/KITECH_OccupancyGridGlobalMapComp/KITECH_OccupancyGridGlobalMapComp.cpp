/*************************************************************

file: KITECH_OccupancyGridGlobalMapComp.cpp
author: E.C. Shin
begin: January 30 2010
copyright: (c) 2010 KITECH, OPRoS
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

#include "GlobalMapServiceProvided.h"
#include "KITECH_OccupancyGridGlobalMapComp.h"
#include "OprosPrintMessage.h"


//
// constructor declaration
//
KITECH_OccupancyGridGlobalMapComp::KITECH_OccupancyGridGlobalMapComp()
{
	error = 0;
	occupancyGridMap = NULL;
	portSetup();
}

//
// constructor declaration (with name)
//
KITECH_OccupancyGridGlobalMapComp::KITECH_OccupancyGridGlobalMapComp(const std::string &name):Component(name)
{
	error = 0;
	occupancyGridMap = NULL;
	portSetup();
}

//
// destructor declaration
//
KITECH_OccupancyGridGlobalMapComp::~KITECH_OccupancyGridGlobalMapComp()
{

}

void KITECH_OccupancyGridGlobalMapComp::portSetup()
{
	// provided service port setup
	ProvidedServicePort *pa1 = new GlobalMapServiceProvided(this);
	addPort("GlobalMapService",pa1);
}

// Call back Declaration
ReturnType KITECH_OccupancyGridGlobalMapComp::onInitialize()
{
	PrintMessage ("KITECH_OccupancyGridGlobalMapComp::onInitialize()\n");

	parameter.SetProperty(getPropertyMap());
	if (!LoadProperty()) {
		error = -1;
		return OPROS_FIND_PROPERTY_ERROR;
	}

	occupancyGridMap = new OccupancyGridMap (mapWidth, mapHeight, mapResolution, 
		laserSensorCount, laserSensorStartAngle, laserSensorEndAngle, laserSensorMinimumRange, laserSensorMaximumRange);
	if (occupancyGridMap == NULL) {
		error = -1;
		return OPROS_INTERNAL_FAULT;
	}

	error = 0;

	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KITECH_OccupancyGridGlobalMapComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onDestroy()
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->SaveMap(mapFileName.c_str());
		delete occupancyGridMap;
		occupancyGridMap = NULL;
	}

	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KITECH_OccupancyGridGlobalMapComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_OccupancyGridGlobalMapComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

bool KITECH_OccupancyGridGlobalMapComp::SetParameter(Property parameter)
{
	this->parameter = parameter;
	return LoadProperty();
}

Property KITECH_OccupancyGridGlobalMapComp::GetParameter()
{
	error = 0;
	return parameter;
}

int KITECH_OccupancyGridGlobalMapComp::GetError()
{
	return error;
}

bool KITECH_OccupancyGridGlobalMapComp::LoadMapFile(std::string fileName,double cellWidth)
{
	return true;
}

bool KITECH_OccupancyGridGlobalMapComp::SaveMapFile(std::string fileName)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->SaveMap(fileName.c_str());
		return true;
	}

	return false;
}

double KITECH_OccupancyGridGlobalMapComp::GetMapCellWidth()
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->_cellWidth;
	}

	return -1.0;
}

int KITECH_OccupancyGridGlobalMapComp::GetMapWidth()
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->_width;
	}

	return -1;
}

int KITECH_OccupancyGridGlobalMapComp::GetMapHeight()
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->_height;
	}

	return -1;
}

unsigned char KITECH_OccupancyGridGlobalMapComp::GetPixel(double x,double y)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->GetPixelCELL(occupancyGridMap->M2CUx(x), occupancyGridMap->M2CUy(y));
	}

	return 0;
}

unsigned char KITECH_OccupancyGridGlobalMapComp::GetPixelCell(int x,int y)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->GetPixelCELL(x, y);
	}

	return 0;
}

void KITECH_OccupancyGridGlobalMapComp::SetPixel(double x,double y,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->SetPixelCELL(occupancyGridMap->M2CUx(x), occupancyGridMap->M2CUy(y), data);
	}
}

void KITECH_OccupancyGridGlobalMapComp::SetPixelCell(int x,int y,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->SetPixelCELL(x, y, data);
	}
}

bool KITECH_OccupancyGridGlobalMapComp::IsIn(double x,double y)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->IsInCELL(occupancyGridMap->M2CUx(x), occupancyGridMap->M2CUy(y));
	}
	
	return false;
}

bool KITECH_OccupancyGridGlobalMapComp::IsInCell(int x,int y)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->IsInCELL(x, y);
	}

	return false;
}

bool KITECH_OccupancyGridGlobalMapComp::IsIntersect(double x1,double y1,double x2,double y2,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		int x1_ = occupancyGridMap->M2CUx(x1);
		int y1_ = occupancyGridMap->M2CUx(y1);
		int x2_ = occupancyGridMap->M2CUx(x2);
		int y2_ = occupancyGridMap->M2CUx(y2);
		return occupancyGridMap->IsIntersectCELL(x1_, y1_, x2_, y2_, data);
	}

	return false;
}

bool KITECH_OccupancyGridGlobalMapComp::IsIntersectCell(int x1,int y1,int x2,int y2,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->IsIntersectCELL(x1, y1, x2, y2, data);
	}

	return false;
}

void KITECH_OccupancyGridGlobalMapComp::DrawLine(double x1,double y1,double x2,double y2,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->DrawLineCELL(occupancyGridMap->M2CUx(x1), occupancyGridMap->M2CUy(y1),
			occupancyGridMap->M2CUx(x2), occupancyGridMap->M2CUy(y2), data);
	}
}

void KITECH_OccupancyGridGlobalMapComp::DrawLineCell(int x1,int y1,int x2,int y2,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->DrawLineCELL(x1, y1, x2, y2, data);
	}
}

void KITECH_OccupancyGridGlobalMapComp::ExpandObstacle(int bulge,unsigned char threshold,unsigned char data)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->ExpandObstacle(bulge, threshold, data);
	}
}

void KITECH_OccupancyGridGlobalMapComp::Reallocate(int offsetX,int offsetY,int width,int height)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->Reallocate(offsetX, offsetY, width, height);
	}
}

void KITECH_OccupancyGridGlobalMapComp::Update(ObjectLocation position,std::vector<double> sensorValue)
{
	if (occupancyGridMap != NULL) {
		occupancyGridMap->OccupancyGridMapping(position, &sensorValue[0]);
	}
}

int KITECH_OccupancyGridGlobalMapComp::M2CUx(double x)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->M2CUx(x);
	}

	return 0;
}

int KITECH_OccupancyGridGlobalMapComp::M2CUy(double y)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->M2CUy(y);
	}

	return 0;
}

double KITECH_OccupancyGridGlobalMapComp::CU2Mx(int x)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->CU2Mx(x);
	}

	return 0;
}

double KITECH_OccupancyGridGlobalMapComp::CU2My(int y)
{
	if (occupancyGridMap != NULL) {
		return occupancyGridMap->CU2My(y);
	}

	return 0;
}

bool KITECH_OccupancyGridGlobalMapComp::LoadProperty ()
{
	if (parameter.FindName ("MapFileName") == false) {
		PrintMessage ("ERROR -> Can not found MapFileName parameter\n");
		return false;
	}
	mapFileName = parameter.GetValue("MapFileName");
	PrintMessage ("<GridBasedGlobalMap Parameter> MapFileName : %s\n", mapFileName.c_str());

	if (parameter.FindName ("MapResolution") == false) {
		PrintMessage ("ERROR -> Can not found MapResolution parameter\n");
		return false;
	}
	mapResolution = atof (parameter.GetValue ("MapResolution").c_str ());
	PrintMessage ("<GridBasedGlobalMap Parameter> MapResolution : %.3lf\n", mapResolution);
	
	if (parameter.FindName ("MapWidth") == false) {
		PrintMessage ("ERROR -> Can not found MapWidth parameter\n");
		return false;
	}
	mapWidth = atoi (parameter.GetValue ("MapWidth").c_str ());
	PrintMessage ("<GridBasedGlobalMap Parameter> MapWidth : %d\n", mapWidth);

	if (parameter.FindName ("MapHeight") == false) {
		PrintMessage ("ERROR -> Can not found MapHeight parameter\n");
		return false;
	}
	mapHeight = atoi (parameter.GetValue ("MapHeight").c_str ());
	PrintMessage ("<GridBasedGlobalMap Parameter> MapHeight : %d\n", mapHeight);

	//if (parameter.FindName ("MobileSafeRadius") == false) {
	//	PrintMessage ("ERROR -> Can not found MobileSafeRadius parameter\n");
	//	return false;
	//}
	//safeRadius = atof (parameter.GetValue ("MobileSafeRadius").c_str ());
	//PrintMessage ("<GridBasedGlobalMap Parameter> MobileSafeRadius : %.3lf\n", safeRadius);

	if (parameter.FindName ("LaserSensorStartAngle") == false) {
		PrintMessage ("ERROR -> Can not found LaserSensorStartAngle parameter\n");
		return false;
	}
	laserSensorStartAngle = DEG2RAD (atof (parameter.GetValue("LaserSensorStartAngle").c_str ()));
	PrintMessage ("<GridBasedGlobalMap Parameter> LaserSensorStartAngle : %.3lf\n", RAD2DEG(laserSensorStartAngle));

	if (parameter.FindName ("LaserSensorEndAngle") == false) {
		PrintMessage ("ERROR -> Can not found LaserSensorEndAngle parameter\n");
		return false;
	}
	laserSensorEndAngle = DEG2RAD (atof (parameter.GetValue("LaserSensorEndAngle").c_str ()));
	PrintMessage ("<GridBasedGlobalMap Parameter> LaserSensorEndAngle : %.3lf\n", RAD2DEG(laserSensorEndAngle));

	if (parameter.FindName ("LaserSensorMinimumRange") == false) {
		PrintMessage ("ERROR -> Can not found LaserSensorMinimumRange parameter\n");
		return false;
	}
	laserSensorMinimumRange = atof (parameter.GetValue("LaserSensorMinimumRange").c_str ());
	PrintMessage ("<GridBasedGlobalMap Parameter> LaserSensorMinimumRange : %.3lf\n", laserSensorMinimumRange);

	if (parameter.FindName ("LaserSensorMaximumRange") == false) {
		PrintMessage ("ERROR -> Can not found LaserSensorMaximumRange parameter\n");
		return false;
	}
	laserSensorMaximumRange = atof (parameter.GetValue("LaserSensorMaximumRange").c_str ());
	PrintMessage ("<GridBasedGlobalMap Parameter> LaserSensorMaximumRange : %.3lf\n", laserSensorMaximumRange);

	if (parameter.FindName ("LaserSensorCount") == false) {
		PrintMessage ("ERROR -> Can not found LaserSensorCount parameter\n");
		return false;
	}
	laserSensorCount = atoi (parameter.GetValue("LaserSensorCount").c_str ());	
	PrintMessage ("<GridBasedGlobalMap Parameter> LaserSensorCount : %d\n", laserSensorCount);

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
	return new KITECH_OccupancyGridGlobalMapComp();
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
	return new KITECH_OccupancyGridGlobalMapComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

