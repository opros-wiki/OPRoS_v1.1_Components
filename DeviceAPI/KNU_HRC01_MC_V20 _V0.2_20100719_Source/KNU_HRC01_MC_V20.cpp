/*************************************************************

 file: DasaRobot_TetraMobile.cpp
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
#include "KNU_HRC01_MC_V20.h"
#include "OprosPrintMessage.h"

using namespace boost::posix_time;

enum eTetraMobileState {
	TS_IDLE = 0,
	TS_MOVE = 1,
	TS_TURN = 2,
	TS_STOP = 3,
};

KNU_HRC01_MC_V20::KNU_HRC01_MC_V20(void)
{
	CNRobot = NULL;

	hOprosAPI = NULL;

	pThread = NULL;
	threadOn = false;

	position = ObjectLocation (0, 0, 0);

	_forwardVelocity = 0;
	_forwardAccel = 0;
	_centripetalAccel = 0;
}

KNU_HRC01_MC_V20::~KNU_HRC01_MC_V20(void)
{
	Finalize();
}

int KNU_HRC01_MC_V20::Initialize(Property parameter)
{
	if(InitializeManipulator(parameter) < 0) {
		PrintMessage("ERROR : KNU_HRC01_MC_V20::Initialize() -> Occur a error in InitializeManipulator().\n");
		return API_ERROR;
	}

	if(SetParameter(parameter) == API_ERROR) {
		PrintMessage("ERROR : KNU_HRC01_MC_V20::Initialize() -> Occur a error in SetParameter().\n");
		return API_ERROR;
	}

	//leftWheel->GetPosition(leftWheelAnglePrev);
	//rightWheel->GetPosition(rightWheelAnglePrev);


	vector<double> WheelAnglePrev;
	
	CNRobot->GetPosition(WheelAnglePrev);


	leftWheelAnglePrev=WheelAnglePrev[0];
	rightWheelAnglePrev=WheelAnglePrev[1];

	currentForward = 0;
	currentTurn = 0;
	targetForward = 0;
	targetTurn = 0;

	mobileCommand = TS_IDLE;

	isRunning = false;

	StartTask(100);

	PrintMessage("SUCCESS : KNU_HRC01_MC_V20::onInitialize()\n");

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Finalize(void)
{
	StopTask();

	FinalizeManipulator();

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Enable(void)
{
	if(CNRobot == NULL ) {
		return API_ERROR;
	}

// 	if(CNRobot->Enable() < 0) {
// 		return API_ERROR;
// 	}



	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Disable(void)
{
	if(CNRobot == NULL ) {
		return API_ERROR;
	}

	if(CNRobot->Disable() < 0) {
		return API_ERROR;
	}


	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::SetParameter(Property parameter)
{
	if (parameter.FindName("WheelDiameter") == false)		return API_ERROR;
	if (parameter.FindName("AxleDistance") == false)		return API_ERROR;
	if (parameter.FindName("VarianceDistance") == false)	return API_ERROR;
	if (parameter.FindName("VarianceDirection") == false)	return API_ERROR;
	if (parameter.FindName("SafeRadius") == false)			return API_ERROR;
	if (parameter.FindName("MaximumVelocity") == false)		return API_ERROR;
	if (parameter.FindName("Acceleration") == false)		return API_ERROR;
	
	_profile.wheelDiameter = atof (parameter.GetValue("WheelDiameter").c_str());
	_profile.axleDistance = atof (parameter.GetValue("AxleDistance").c_str());
	_profile.varianceDistance = atof (parameter.GetValue("VarianceDistance").c_str());
	_profile.varianceDirection = DEG2RAD (atof (parameter.GetValue("VarianceDirection").c_str()));
	_profile.safeRadius = atof (parameter.GetValue("SafeRadius").c_str());
	_profile.maximumVelocity = atof (parameter.GetValue("MaximumVelocity").c_str());
	_profile.acceleration = atof (parameter.GetValue("Acceleration").c_str());

	this->parameter = parameter;

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::SetPosition(ObjectLocation position)
{
	this->position = position;

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::GetPosition(ObjectLocation &position)
{
	position = this->position;

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Drive(double forwardVelocity, double angularVelocity)
{
	if(CNRobot == NULL  ){
		return API_ERROR;
	}
	EstimateAcceleration (forwardVelocity, angularVelocity, 0.1);
	if (fabs(forwardVelocity) < 0.01 &&  fabs(angularVelocity) < DEG2RAD(0.5)) {
		forwardVelocity = 0.;
		angularVelocity = 0.;
	}

	forwardVelocity = bound (forwardVelocity, -_profile.maximumVelocity, _profile.maximumVelocity);
	angularVelocity = bound (angularVelocity, -DEG2RAD (40), DEG2RAD (40));

	double f = 2.*forwardVelocity/_profile.wheelDiameter;
	double a = angularVelocity*_profile.axleDistance/_profile.wheelDiameter;

//   	PrintMessage ("KNU_HRC01_MC_V20::Drive() -> f:%.3lf\n", f);
//   	PrintMessage ("KNU_HRC01_MC_V20::Drive() -> a:%.3lf\n", a);

	vector<double> velocity;
	velocity.resize(2);
	
	
 	velocity[0] = RAD2DEG(f - a);	
 	velocity[1] = -RAD2DEG(f + a);


 	vector<unsigned long> times;
	times.resize(2);

	if (CNRobot->SetVelocity (velocity,times) == API_ERROR) return API_ERROR;

// 	PrintMessage (" KNU_HRC01_MC_V20::Drive(%.3lf, %.3lf)\n",velocity[0], velocity[1]);

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Move(double distance)
{
	if(CNRobot == NULL ) {
		return API_ERROR;
	}

	if (0 < mobileCommand) {
		return API_ERROR;
	}

	targetForward = currentForward + distance;
	mobileCommand = TS_MOVE;
	isRunning = true;

	PrintMessage ("KNU_HRC01_MC_V20::Move(%.3lf)\n", distance);

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Rotate(double degree)
{
	if(CNRobot == NULL ) {
		return API_ERROR;
	}
	if (0 < mobileCommand) {
		return API_ERROR;
	}

	targetTurn = currentTurn + DEG2RAD (-degree);
	mobileCommand = TS_TURN;
	isRunning = true;

	PrintMessage ("KNU_HRC01_MC_V20::Rotate(%.3lf)\n", degree);

	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::Stop(void)
{
	if(CNRobot == NULL ) {
		return API_ERROR;
	}

	if(CNRobot->Stop() < 0) {
		return API_ERROR;
	}
	mobileCommand = TS_STOP;
	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::EmergencyStop(void)
{
	if(CNRobot == NULL) {
		return API_ERROR;
	}

	if(CNRobot->EmergencyStop() < 0) {
		return API_ERROR;
	}
	mobileCommand = TS_STOP;
	return API_SUCCESS;
}

int KNU_HRC01_MC_V20::InitializeManipulator(Property parameter)
{
	//	CAN API 초기화
	if(parameter.FindName("ManipulatorAPIName") == false) {
		PrintMessage("ERROR -> KNU_HRC01_MC_V20::InitializeManipulator() -> Can't find the ManipulatorAPIName\n");
		return API_ERROR;
	}

	Property WheelParameter;
	



	if(parameter.FindName("Size") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Size", parameter.GetValue("Size"));

	//	Servo Actuator Profile for Left Wheel
	if(parameter.FindName("MaximumPower0") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MaximumPower0", parameter.GetValue("MaximumPower0"));

	if(parameter.FindName("LinePerRevolution0") == false)			goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("LinePerRevolution0", parameter.GetValue("LinePerRevolution0"));
	
	if(parameter.FindName("ReductionRatio0") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("ReductionRatio0", parameter.GetValue("ReductionRatio0"));

	if(parameter.FindName("MaximumVelocity0") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MaximumVelocity0", parameter.GetValue("MaximumVelocity0"));

	if(parameter.FindName("Acceleration0") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Acceleration0", parameter.GetValue("Acceleration0"));

	if(parameter.FindName("MinimumPositionLimit0") == false)		goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MinimumPositionLimit0", parameter.GetValue("MinimumPositionLimit0"));

	if(parameter.FindName("MaximumPositionLimit0") == false)		goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MaximumPositionLimit0", parameter.GetValue("MaximumPositionLimit0"));

	if(parameter.FindName("Kp0") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Kp0", parameter.GetValue("Kp0"));

	if(parameter.FindName("Ki0") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Ki0", parameter.GetValue("Ki0"));

	if(parameter.FindName("Kd0") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Kd0", parameter.GetValue("Kd0"));

	if(parameter.FindName("ISum0") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("ISum0", parameter.GetValue("ISum0"));



	//	Servo Actuator Profile for Right Wheel
	if(parameter.FindName("MaximumPower1") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MaximumPower1", parameter.GetValue("MaximumPower1"));

	if(parameter.FindName("LinePerRevolution1") == false)			goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("LinePerRevolution1", parameter.GetValue("LinePerRevolution1"));
	
	if(parameter.FindName("ReductionRatio1") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("ReductionRatio1", parameter.GetValue("ReductionRatio1"));

	if(parameter.FindName("MaximumVelocity1") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MaximumVelocity1", parameter.GetValue("MaximumVelocity1"));

	if(parameter.FindName("Acceleration1") == false)				goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Acceleration1", parameter.GetValue("Acceleration1"));

	if(parameter.FindName("MinimumPositionLimit1") == false)		goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MinimumPositionLimit1", parameter.GetValue("MinimumPositionLimit1"));

	if(parameter.FindName("MaximumPositionLimit1") == false)		goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("MaximumPositionLimit1", parameter.GetValue("MaximumPositionLimit1"));

	if(parameter.FindName("Kp1") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Kp1", parameter.GetValue("Kp1"));

	if(parameter.FindName("Ki1") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Ki1", parameter.GetValue("Ki1"));

	if(parameter.FindName("Kd1") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("Kd1", parameter.GetValue("Kd1"));

	if(parameter.FindName("ISum1") == false)							goto KNU_HRC01_MC_V20_INIT_ERROR;
	WheelParameter.SetValue("ISum1", parameter.GetValue("ISum1"));


	//	DLL 로드
	hOprosAPI = LoadLibrary((LPCSTR)parameter.GetValue("ManipulatorAPIName").c_str());
	if(hOprosAPI == NULL) {
		PrintMessage("ERROR : KNU_HRC01_MC_V20::InitializeManipulator() -> Can't find the ManipulatorAPIName in property\n");
		return API_ERROR;
	}

	GET_OPROS_API getOprosAPI;
	getOprosAPI = (GET_OPROS_API)GetProcAddress(hOprosAPI, "GetAPI");
	if(getOprosAPI == NULL) {
		PrintMessage("ERROR : KNU_HRC01_MC_V20::InitializeManipulator() -> Can't get a handle of GetAPI Funtion\n");
		FinalizeManipulator();
		return API_ERROR;
	}

	CNRobot = dynamic_cast<Manipulator *>(getOprosAPI());
	if(CNRobot == NULL) {
		PrintMessage("ERROR : KNU_HRC01_MC_V20::InitializeManipulator() -> Can't get a handle of Manipulator API\n");
		FinalizeManipulator();
		return API_ERROR;
	}

	if(CNRobot->Initialize(WheelParameter) == API_ERROR) {
		FinalizeManipulator();
		return API_ERROR;
	}

 	if(CNRobot->Enable() == API_ERROR) {
 		FinalizeManipulator();
 		return API_ERROR;
 	}

	PrintMessage("SUCCESS : KNU_HRC01_MC_V20::InitializeManipulator()\n");

	return 0;

KNU_HRC01_MC_V20_INIT_ERROR :
	PrintMessage("ERROR : KNU_HRC01_MC_V20::InitializeManipulator() -> Can't find parameters\n");
	return -1;
}

int KNU_HRC01_MC_V20::FinalizeManipulator(void)
{
	if(CNRobot != NULL) {
		delete CNRobot;
		CNRobot = NULL;
	}

	if(hOprosAPI != NULL) {
		FreeLibrary(hOprosAPI);
		hOprosAPI = NULL;
	}

	return 0;
}

bool KNU_HRC01_MC_V20::UpdatePosition()
{

	vector<double> WheelAnglePrev;

	if(CNRobot->GetPosition(WheelAnglePrev)<0)
	{
		PrintMessage ("ERROR : KNU_HRC01_MC_V20::CNRobot::GetPosition()\n ");

		return  -1;
	}

	double leftWheelAngle;
	double rightWheelAngle;
	
	leftWheelAngle=WheelAnglePrev[0];
	rightWheelAngle=-WheelAnglePrev[1];
	
	//PrintMessage ("KNU_HRC01_MC_V20::leftWheelAngle %f rightWheelAngle %f\n", leftWheelAngle, rightWheelAngle);


	double dl = DEG2RAD(leftWheelAngle  - leftWheelAnglePrev);
	double dr = DEG2RAD(rightWheelAngle - rightWheelAnglePrev);

	leftWheelAnglePrev = leftWheelAngle;
	rightWheelAnglePrev = rightWheelAngle;

	double moveDelta = (dr + dl)/2 * _profile.wheelDiameter*.5;
	double turnDelta = (dr - dl)/_profile.axleDistance * _profile.wheelDiameter*.5;

	// 바퀴의 엔코더가 한번에 너무 많이 변하지 않은 경우, 
	// 유효한 엔코더로 생각한다.
	const double acceptableForwardDelta = 1.;
	const double acceptableTurnDelta = M_PI/2;

	if (-acceptableForwardDelta < moveDelta && moveDelta < acceptableForwardDelta &&
		-acceptableTurnDelta < turnDelta && turnDelta < acceptableTurnDelta ){

			//_locationPf->Predict (forwardDelta, turnDelta);

			double th = position.theta + turnDelta/2.;
			double dx = moveDelta*cos(th);
			double dy = moveDelta*sin(th);
			double dth = turnDelta;
			position += ObjectLocation (dx, dy, dth);

			currentForward += moveDelta;
			currentTurn += turnDelta;


			double varDist = moveDelta*_profile.varianceDistance;
			double varAng = turnDelta*_profile.varianceDirection;
			variance = ObjectLocation (varDist*fabs(cos(position.theta)), varDist*fabs(sin(position.theta)), fabs(varAng));
	}
	else {
		PrintMessage ("ERROR : KNU_HRC01_MC_V20::UpdatePosition() -> Move too many:moveDelta = %g, turnDelta = %g", moveDelta, turnDelta);
	}
	//PrintMessage ("KNU_HRC01_MC_V20::UpdatePosition() -> position:x = %f, y = %f, theta = %4.4f\n", position.x, position.y,RAD2DEG(position.theta));

	return true;
}

void KNU_HRC01_MC_V20::StartTask (long period)
{
	if((threadOn == false) && (pThread == NULL)) {
		taskPeriod = period;
		pThread = new boost::thread(boost::bind(&KNU_HRC01_MC_V20::ThreadFunc, this));
		threadOn = true;
	}
}

void KNU_HRC01_MC_V20::StopTask ()
{
	if((threadOn == true) && pThread) {
		threadOn = false;
		pThread->join();
		delete pThread;
		pThread = NULL;
	}
}

void KNU_HRC01_MC_V20::DoTask()
{
	UpdatePosition ();
	switch (mobileCommand) {
		case TS_IDLE:
			isRunning = false;
			break;
		case TS_MOVE: {
			double dLength = bound (targetForward - currentForward, -_profile.maximumVelocity, _profile.maximumVelocity);
			if (fabs (dLength) < 0.05) {
				mobileCommand = TS_STOP;
			}
			else {
				Drive(dLength, 0);
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
				Drive(0 ,dAngle);
				isRunning = true;
			}
			break;
		}
		case TS_STOP: {
			Drive(0, 0);
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

void KNU_HRC01_MC_V20::EstimateAcceleration (double &forwardVelocity, double &angularVelocity, double deltaTime)
{
	const double maxJerk  = 1.0;						// Unit: m/s^3
	const double maxAccel = _profile.acceleration;		// Unit: m/s^2

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
void KNU_HRC01_MC_V20::ThreadFunc()
{
	while(threadOn) {
		ptime beginTime(microsec_clock::local_time());	// 시작 시간
		DoTask();										// 실행
		ptime endTime(microsec_clock::local_time());	// 완료 시간

		// 실행시간을 계산해서 sleep할 시간을 계산한다.
		time_period elapsedTime(beginTime, endTime);	// 경과 시간
		long elapsedMicrosec = (long)elapsedTime.length().total_microseconds();  // elapsed micro sec
		long sleepTime = (long)(taskPeriod * 1000) - elapsedMicrosec;
		// 주기를 over해서 실행한 경우로써 warning을 report해야함.
		if(sleepTime < 0) {
#ifdef _DEBUG
			//cerr << "WARNING : Deadline missed !!!!!!" << endl;
#endif
		}
		else {
			boost::posix_time::microsec sleepBoostTime(sleepTime); // sleep 시간 설정 (microsec)
			boost::this_thread::sleep<boost::posix_time::microsec>(sleepBoostTime);
		}
	}
}

#if !defined(DEVICE_TEST)
#if defined(WIN32)
extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new KNU_HRC01_MC_V20();
}
#else
extern "C"
{
	OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new KNU_HRC01_MC_V20();
}
#endif
#endif