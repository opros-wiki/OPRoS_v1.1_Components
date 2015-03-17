/*************************************************************

 file: KangwonNationalUnivKobotMobility.cpp
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

#include "KangwonNationalUnivKobotMobility.h"
#include "OprosPrintMessage.h"
#include <math.h>

#define	PI	3.141592

#define	LEFT_WHEEL		1
#define	RIGHT_WHEEL		2

KangwonNationalUnivKobotMobility::KangwonNationalUnivKobotMobility(void)
{
	_hcr01 = NULL;
}


KangwonNationalUnivKobotMobility::~KangwonNationalUnivKobotMobility(void)
{
	Finalize();
}

int KangwonNationalUnivKobotMobility::Initialize(Property parameter)
{
	Finalize();

	_hcr01 = new HRC01();
	if(_hcr01->Open() < 0) {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::Initialize() -> Occur a error in _hcr01->Open()\n");
		return API_ERROR;
	}

	if(_hcr01->Reset() < 0) {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::Initialize() -> Occur a error in _hcr01->Reset()\n");
		_hcr01->Close();
		return API_ERROR;
	}

	_prevLeftWheelAngle = 0.0;
	_prevRightWheelAngle = 0.0;

	if(SetParameter(parameter) < 0) {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::Initialize().\n");
		return API_ERROR;
	}

	_status = DEVICE_READY;

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::Finalize(void)
{
	Disable();

	if(_hcr01 != NULL) {
		_hcr01->Close();
		delete _hcr01;
		_hcr01 = NULL;
	}

	_status = DEVICE_CREATED;

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::Enable(void)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}
	else if(_status == DEVICE_ACTIVE) {
		return API_SUCCESS;
	}

	if(_hcr01->SetPositionDelta(0, 0, 0) < 0) {
		return API_ERROR;
	}

	_status = DEVICE_ACTIVE;

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::Disable(void)
{
	if(_status == DEVICE_ACTIVE) {
		_hcr01->Disable();

		_status = DEVICE_READY;
	}

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::SetParameter(Property parameter)
{
	if(parameter.FindName("WheelDiameter") == false) {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[WheelDiameter].\n");
		return API_ERROR;
	}
	_profile.wheelDiameter = atof(parameter.GetValue("WheelDiameter").c_str());

	if(parameter.FindName("AxleDistance") == false)	 {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[WheelDiameter].\n");
		return API_ERROR;
	}
	_profile.axleDistance = atof(parameter.GetValue("AxleDistance").c_str());

	if(parameter.FindName("LinePerRevolution") == false) {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[LinePerRevolution].\n");
		return API_ERROR;
	}
	_profile.linePerRevolution = atof(parameter.GetValue("LinePerRevolution").c_str());

	if(parameter.FindName("ReductionRatio") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[ReductionRatio].\n");
		return API_ERROR;
	}
	_profile.reductionRatio = atof(parameter.GetValue("ReductionRatio").c_str());

	if(parameter.FindName("MaximumVelocity") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[MaximumVelocity].\n");
		return API_ERROR;
	}
	_profile.maximumVelocity = atof(parameter.GetValue("MaximumVelocity").c_str());

	if(parameter.FindName("Acceleration") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Acceleration].\n");
		return API_ERROR;
	}
	_profile.acceleration = atof(parameter.GetValue("Acceleration").c_str());

	if(parameter.FindName("Kp0") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Kp0].\n");
		return API_ERROR;
	}
	_profile.kp[0] = (unsigned short)atoi(parameter.GetValue("Kp0").c_str());

	if(parameter.FindName("Kp1") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Kp1].\n");
		return API_ERROR;
	}
	_profile.kp[1] = (unsigned short)atoi(parameter.GetValue("Kp1").c_str());

	if(parameter.FindName("Ki0") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Ki0].\n");
		return API_ERROR;
	}
	_profile.ki[0] = (unsigned short)atoi(parameter.GetValue("Ki0").c_str());

	if(parameter.FindName("Ki1") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Ki1].\n");
		return API_ERROR;
	}
	_profile.ki[1] = (unsigned short)atoi(parameter.GetValue("Ki1").c_str());

	if(parameter.FindName("Kd0") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Kd0].\n");
		return API_ERROR;
	}
	_profile.kd[0] = (unsigned short)atoi(parameter.GetValue("Kd0").c_str());

	if(parameter.FindName("Kd1") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[Kd1].\n");
		return API_ERROR;
	}
	_profile.kd[1] = (unsigned short)atoi(parameter.GetValue("Kd1").c_str());

	if(parameter.FindName("UiSum0") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[UiSum0].\n");
		return API_ERROR;
	}
	_profile.uiSum[0] = (unsigned short)atoi(parameter.GetValue("UiSum0").c_str());

	if(parameter.FindName("UiSum1") == false)	{
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Can't find a property[UiSum1].\n");
		return API_ERROR;
	}
	_profile.uiSum[1] = (unsigned short)atoi(parameter.GetValue("KUiSum1").c_str());

	if(SetupHrc01() < 0) {
		PrintMessage("ERROR : KangwonNationalUnivKobotMobility::SetParameter() -> Occur a error in SetupHrc01()\n");
		return API_ERROR;
	}

	if(_profile.linePerRevolution == 0.0 || _profile.reductionRatio == 0.0) {
		return API_ERROR;
	}
	
	_profile.pulseToRad = (2.0 * PI) / (_profile.linePerRevolution * _profile.reductionRatio);
	_profile.radToPulse = (_profile.linePerRevolution * _profile.reductionRatio) / (2.0 * PI);
	_profile.angleToRad = PI / 180.0;
	_profile.radToAngle = 180.0 / PI;

	PrintMessage("\n\nKangwonNationalUnivKobotMobility Parameters\n");
	PrintMessage("WheelDiameter     : %.3f\n", _profile.wheelDiameter);
	PrintMessage("AxleDistance      : %.3f\n", _profile.axleDistance);
	PrintMessage("LinePerRevolution : %.3f\n", _profile.linePerRevolution);
	PrintMessage("ReductionRatio    : %.3f\n", _profile.reductionRatio);
	PrintMessage("Maximum Velocity  : %.3f\n", _profile.maximumVelocity);
	PrintMessage("Acceleration      : %.3f\n", _profile.acceleration);

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::GetParameter(Property &parameter)
{
	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::SetPosition(ObjectLocation position)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	position.theta = position.theta * _profile.angleToRad;
	_position = position;

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::GetPosition(ObjectLocation &position)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	position = _position;

	position.theta = position.theta * _profile.radToAngle;

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::GetOdometery(vector<long> &odometery)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	int position[3];
	_hcr01->GetPosition(position);

	odometery.resize(2);
	odometery[0] = position[1];	//	Left
	odometery[1] = position[0];	//	Right

	//PrintMessage("[INFO] : KangwonNationalUnivKobotMobility::GetOdometery() -> %d, %d\n", position[0], position[1]);

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::DriveWheel(double linearVelocity, double angularVelocity)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	//	rad/s
	linearVelocity = 2.0 * linearVelocity / _profile.wheelDiameter;
	angularVelocity = angularVelocity * _profile.axleDistance / _profile.wheelDiameter;

	//	pulse/s
	int leftVelocity = -(int)((linearVelocity - angularVelocity) * _profile.radToPulse);
	int rightVelocity = (int)((linearVelocity + angularVelocity) * _profile.radToPulse);

	if(_hcr01->SetVelocity(rightVelocity, leftVelocity, 0) < 0) {
		return -1;
	}

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::MoveWheel(double distance, double linearVelocity)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	int leftPosition = -(int)(distance * _profile.radToPulse / (_profile.wheelDiameter / 2.0));
	int rightPosition = (int)(distance * _profile.radToPulse / (_profile.wheelDiameter / 2.0));

	if(_hcr01->SetPositionDelta(rightPosition, leftPosition, 0) < 0) {
		return -1;
	}

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::RotateWheel(double angle, double angularVelocity)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	double deltaMove = angle * (PI / 180.0) * _profile.axleDistance / 2.0;

	int leftPosition = -(int)(-deltaMove * _profile.radToPulse / (_profile.wheelDiameter / 2.0));
	int rightPosition = (int)(deltaMove * _profile.radToPulse / (_profile.wheelDiameter / 2.0));

	if(_hcr01->SetPositionDelta(rightPosition, leftPosition, 0) < 0) {
		return -1;
	}

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::StopWheel(void)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	if(_hcr01->Stop() < 0) {
		return API_ERROR;
	}

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::IsWheelRunning(bool &isWheelRunning)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	if(_hcr01->IsRunning(1, isWheelRunning) < 0) {
		return -1;
	}
	if(isWheelRunning == true) return API_SUCCESS;

	if(_hcr01->IsRunning(1, isWheelRunning) < 0) {
		return -1;
	}

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::OnExecute(void)
{
	if(_status == DEVICE_CREATED || _status == DEVICE_ERROR) {
		return API_ERROR;
	}

	int position[3];
	_hcr01->GetPosition(position);

	double leftWheelAngle = -(double)position[1];
	double rightWheelAngle = (double)position[0];

	//	Radian
	leftWheelAngle = leftWheelAngle * _profile.pulseToRad;
	rightWheelAngle = rightWheelAngle * _profile.pulseToRad;
	
	double deltaLeftWheelAngle = (leftWheelAngle - _prevLeftWheelAngle);
	double deltaRightWheelAngle = (rightWheelAngle - _prevRightWheelAngle);

	double deltaMove = (deltaRightWheelAngle + deltaLeftWheelAngle) / 2.0 * _profile.wheelDiameter * 0.5;
	double deltaRotate = (deltaRightWheelAngle - deltaLeftWheelAngle) / _profile.axleDistance * _profile.wheelDiameter * 0.5;

	double theta = _position.theta + deltaRotate / 2.0;
	double dx = deltaMove*cos(theta);
	double dy = deltaMove*sin(theta);

	_position.x += dx;
	_position.y += dy;
	_position.theta += deltaRotate;

	_prevLeftWheelAngle = leftWheelAngle;
	_prevRightWheelAngle = rightWheelAngle;

	//PrintMessage("[INFO] : KangwonNationalUnivKobotMobility::OnExecute() -> %.3f, %.3f, %.3f\n", _position.x, _position.y, _position.theta);

	return API_SUCCESS;
}

int KangwonNationalUnivKobotMobility::SetupHrc01(void)
{
	if(_hcr01 == NULL) {
		return API_ERROR;
	}

	if(_hcr01->SetPIDGain(	_profile.kp[0],	_profile.ki[0], _profile.kd[0], _profile.uiSum[0],
							_profile.kp[1], _profile.ki[1], _profile.kd[1], _profile.uiSum[1],
							0,				0,				0,				0) < 0) {
		return API_ERROR;
	}
	
	if(_hcr01->SetMaximumVelocity((unsigned int)_profile.maximumVelocity, (unsigned int)_profile.maximumVelocity, 0) < 0) {
		return API_ERROR;
	}

	if(_hcr01->SetAcceleration((unsigned int)_profile.acceleration, (unsigned int)_profile.acceleration, 0) < 0) {
		return API_ERROR;
	}

	return API_SUCCESS;
}

#if defined(WIN32)
extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new KangwonNationalUnivKobotMobility();
}

#endif