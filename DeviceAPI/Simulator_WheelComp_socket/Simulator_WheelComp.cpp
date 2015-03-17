/*************************************************************

 file: Simulator_WheelComp.cpp
 author: D.W. Kyoung
 begin: September 16 2010
 copyright: (c) 2010 KETI, OPRoS
 email: timetocomez@gmail.com

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


#include "data.h"
#include "Simulator_WheelComp.h"
#include "device/OprosPrintMessage.h"


enum eTetraMobileState {
	TS_IDLE = 0,
	TS_MOVE = 1,
	TS_TURN = 2,
	TS_STOP = 3,
};


Simulator_WheelComp::Simulator_WheelComp(void)
{
	hOprosAPI = NULL;

	threadOn = false;

	position = ObjectLocation (0, 0, 0);

	_forwardVelocity = 0;
	_forwardAccel = 0;
	_centripetalAccel = 0;
}

Simulator_WheelComp::~Simulator_WheelComp(void)
{
	Finalize();
}

int Simulator_WheelComp::Initialize(Property parameter)
{
	if(SetParameter(parameter) == API_ERROR) {
		PrintMessage("ERROR : Simulator_WheelComp::Initialize() -> Occur a error in SetParameter().\n");
		return API_ERROR;
	}

	if(m_ClientSocket.getPosition(redata.deviceID[3],&leftWheelAnglePrev) == false){
		printf ("ERROR : readEncoder()\n");
		return false;
	}
	if(m_ClientSocket.getPosition(redata.deviceID[2],&rightWheelAnglePrev) == false){
		printf ("ERROR : readEncoder()\n");
		return false;
	}

	currentForward = 0;
	currentTurn = 0;
	targetForward = 0;
	targetTurn = 0;
	preVelocity = 0;
	preAccel = 0;

	mobileCommand = TS_IDLE;

	isRunning = false;

	StartTask(100);

	PrintMessage("SUCCESS : Simulator_WheelComp::onInitialize()\n");

	leftWheelAnglePrev = 0;
	rightWheelAnglePrev = 0;

	return API_SUCCESS;
}

int Simulator_WheelComp::Finalize(void)
{
	StopTask();

	return API_SUCCESS;
}

int Simulator_WheelComp::Enable(void)
{
	return API_SUCCESS;
}

int Simulator_WheelComp::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_WheelComp::SetParameter(Property parameter)
{

	std::string dName[6];
	
	if (parameter.FindName("WheelDiameter") == false)		return API_ERROR;
	if (parameter.FindName("AxleDistance") == false)		return API_ERROR;
	if (parameter.FindName("VarianceDistance") == false)	return API_ERROR;
	if (parameter.FindName("VarianceDirection") == false)	return API_ERROR;
	if (parameter.FindName("SafeRadius") == false)			return API_ERROR;
	if (parameter.FindName("MaximumVelocity") == false)		return API_ERROR;
	if (parameter.FindName("Acceleration") == false)		return API_ERROR;
	if (parameter.FindName("R_Motor_Name") == false)		return API_ERROR;
	if (parameter.FindName("L_Motor_Name") == false)		return API_ERROR;
	if (parameter.FindName("RobotName") == false)			return API_ERROR;
	if (parameter.FindName("R_Enc_Name") == false)			return API_ERROR;
	if (parameter.FindName("L_Enc_Name") == false)			return API_ERROR;
	if (parameter.FindName("GPS_Name")	 == false)			return API_ERROR;
	if (parameter.FindName("GyroSensor") == false)			return API_ERROR;
	
	rMotorName	= parameter.GetValue("R_Motor_Name").c_str();
	lMotorName	= parameter.GetValue("L_Motor_Name").c_str();
	rEncName	= parameter.GetValue("R_Enc_Name").c_str();
	lEncName	= parameter.GetValue("L_Enc_Name").c_str();
	SimulatorIP = parameter.GetValue("userMessage").c_str();
	robotName	= parameter.GetValue("RobotName").c_str();
	PortNumber	= atoi(parameter.GetValue("Port").c_str());
	gps			= parameter.GetValue("GPS_Name").c_str();
	gyro		= parameter.GetValue("GyroSensor").c_str();

	this->parameter = parameter;

	dName[0] = rMotorName;
	dName[1] = lMotorName;
	dName[2] = rEncName;
	dName[3] = lEncName;
	dName[4] = gps;
	dName[5] = gyro;

	if(m_ClientSocket.enable(SimulatorIP, PortNumber) == false)
	{
		return false;
	}
	redata = m_ClientSocket.findSystem(robotName,dName);

	return API_SUCCESS;
}

int Simulator_WheelComp::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int Simulator_WheelComp::SetPosition(ObjectLocation position)
{
	return API_SUCCESS;
}

int Simulator_WheelComp::GetPosition(ObjectLocation &position)
{
	UpdatePosition();
	position = this->position;

	return API_SUCCESS;
}

int Simulator_WheelComp::DriveWheel(double linearVelocity, double angularVelocity)
{
	if (mobileCommand != TS_IDLE)	EstimateAcceleration(linearVelocity, angularVelocity, 0.1);

	if (fabs(linearVelocity) < 0.01 &&  fabs(angularVelocity) < DEG2RAD(0.5)) {
		linearVelocity = 0.;
		angularVelocity = 0.;
	}

	float leftVelocity = (float)(linearVelocity - angularVelocity);	
	float rightVelocity = (float)(linearVelocity + angularVelocity);

	if (m_ClientSocket.setVelocity(redata.deviceID[0],rightVelocity) == false)
	{
		return API_ERROR;
	}
	if (m_ClientSocket.setVelocity(redata.deviceID[1],leftVelocity) == false)
	{
		return API_ERROR;
	}

	return API_SUCCESS;
}

int Simulator_WheelComp::MoveWheel(double distance, double linearVelocity)
{
	if (0 < mobileCommand) {
		return API_ERROR;
	}

	targetForward = currentForward + distance;
	printf("current : %f distance : %f \n",currentForward, distance);
	mobileCommand = TS_MOVE;
	isRunning = true;

	return API_SUCCESS;
}

int Simulator_WheelComp::RotateWheel(double degree, double angularVelocity)
{
	if (0 < mobileCommand) {
		return API_ERROR;
	}

	targetTurn = currentTurn + (degree/180.) * M_PI;
	mobileCommand = TS_TURN;
	isRunning = true;

	return API_SUCCESS;
}

int Simulator_WheelComp::StopWheel(void)
{
	mobileCommand = TS_STOP;
	return API_SUCCESS;
}

int Simulator_WheelComp::InitializeServoActuator(Property parameter)
{
	
#if defined(WIN32)
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ServoActuatorAPIName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : Simulator_WheelComp::InitializeServoActuator() -> Can't find the ServoActuatorAPIName in property\n");
		return API_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : Simulator_WheelComp::InitializeServoActuator() -> Can't get a handle of GetAPI Funtion\n");
		FinalizeServoActuator();
		return API_ERROR;
	}
#else
	hOprosAPI = dlopen(parameter.GetValue("ServoActuatorAPIName").c_str(), RTLD_LAZY);
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : Simulator_WheelComp::InitializeServoActuator() -> Can't find the ServoActuatorAPIName in property\n");
		return API_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)dlsym(hOprosAPI, "GetAPI");
	error = dlerror();
	if(error != NULL) {
		PrintMessage("ERROR : Simulator_WheelComp::InitializeServoActuator() -> Can't get a handle of GetAPI Funtion\n");
		dlclose(hOprosAPI);
		FinalizeServoActuator();
		return API_ERROR;
	}
#endif

	return 0;
}


int Simulator_WheelComp::FinalizeServoActuator(void)
{
	if(hOprosAPI != NULL) {
#if defined(WIN32)
		FreeLibrary(hOprosAPI);
#else
		dlclose(hOprosAPI);
#endif
		hOprosAPI = NULL;
	}

	return 0;
}


bool Simulator_WheelComp::UpdatePosition()
{
	float leftWheelAngle = 0;
	float rightWheelAngle = 0;

	if(m_ClientSocket.getPosition(redata.deviceID[3],&leftWheelAngle) == false){
		return false;
	}
	if(m_ClientSocket.getPosition(redata.deviceID[2],&rightWheelAngle) == false){
		return false;
	}
	
	float dl = (leftWheelAngle  - leftWheelAnglePrev);
	float dr = (rightWheelAngle - rightWheelAnglePrev);

	leftWheelAnglePrev = leftWheelAngle;
	rightWheelAnglePrev = rightWheelAngle;

	double moveDelta = (dr + dl)/2. * atof (parameter.GetValue("WheelDiameter").c_str()) * .5;
	double turnDelta = fabs(dr - dl)/ atof (parameter.GetValue("AxleDistance").c_str())  * atof (parameter.GetValue("WheelDiameter").c_str()) * .5;

	double th = position.theta + turnDelta/2.;
	double dx = moveDelta*cos(th);
	double dy = moveDelta*sin(th);
	double dth = turnDelta;
	position += ObjectLocation (dx, dy, dth);

	currentForward += moveDelta;
	currentTurn += turnDelta;
	return true;
}

void Simulator_WheelComp::StartTask (long period)
{

}

void Simulator_WheelComp::StopTask ()
{
	
}

void Simulator_WheelComp::DoTask()
{
	UpdatePosition ();

	switch (mobileCommand) {

		case TS_IDLE:
			isRunning = false;
			break;

		case TS_MOVE: {

			if (fabs(targetForward - currentForward) < 0.03) {
			currentForwarderr = currentForward;
			mobileCommand = TS_STOP;
			}

			else {
				double tdata = 0;
				DriveWheel(Velocity,0);
				isRunning = true;

			}

			break;
		}
		case TS_TURN: {

			double dAngle = bound (targetTurn - currentTurn, -DEG2RAD (45), DEG2RAD (45));
			if (fabs(dAngle) < DEG2RAD(1.)) {
				mobileCommand = TS_STOP;
			}
			else {
				DriveWheel(0 ,dAngle);
				isRunning = true;
			}
			break;
		}
		case TS_STOP: {
			DriveWheel(0, 0);
			if (fabs (_forwardVelocity) == 0 && fabs (_forwardAccel) == 0) {
				mobileCommand = TS_IDLE	;
			}
			break;
		}
		default:
			isRunning = false;
			break;
	}
}


void Simulator_WheelComp::ThreadFunc()
{
	while(threadOn) {
		
		DoTask();										// ½ÇÇà
		
	}
}

void Simulator_WheelComp::EstimateAcceleration
(double &forwardVelocity, double &angularVelocity, double deltaTime)
{
	const double maxJerk  = 1.5;						// Unit: m/s^3
	const double maxAccel = 2;//_profile.acceleration;		// Unit: m/s^2

	double accel = (forwardVelocity - _forwardVelocity)/deltaTime;
	accel = bound (accel, -maxAccel, +maxAccel);

	double jerk  = (accel - _forwardAccel)/deltaTime;
	jerk = bound (jerk, -maxJerk, maxJerk);

	accel = _forwardAccel + jerk*deltaTime;
	double accelLimit = sqrt(2.*maxJerk*fabs(forwardVelocity - _forwardVelocity));
	accel = bound (accel, -accelLimit, accelLimit);

	forwardVelocity = _forwardVelocity + accel*deltaTime;

	_forwardVelocity = forwardVelocity;
	_forwardAccel = accel;	
	_centripetalAccel = forwardVelocity*angularVelocity;
}


#if !defined(DEVICE_TEST)
#if defined(WIN32)
extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_WheelComp();
}
#else
extern "C"
{
	OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_WheelComp();
}
#endif
#endif