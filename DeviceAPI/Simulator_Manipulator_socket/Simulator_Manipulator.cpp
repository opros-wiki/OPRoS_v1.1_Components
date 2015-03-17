
#include "Simulator_Manipulator.h"
#include "OprosPrintMessage.h"

Simulator_Manipulator::Simulator_Manipulator(void)
{
	//status = DEVICE_CREATED;
}


Simulator_Manipulator::~Simulator_Manipulator(void)
{
	Finalize();
}

int Simulator_Manipulator::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS) {
		return API_ERROR;
	}

	//status = DEVICE_READY;

	return API_SUCCESS;
}

int Simulator_Manipulator::Finalize(void)
{
	//status = DEVICE_CREATED;

	return API_SUCCESS;
}

int Simulator_Manipulator::Enable(void)
{
	if(_clientSocket.enable(_ip, _port) == false) {
		PrintMessage("ERROR : Simulator_Manipulator::Enable() -> Can't connect to server[IP : %s, Port : %d]\n", _ip.c_str(), _port);
		return API_ERROR;
	}

	_devicesInfo = _clientSocket.findSystem(_robotName, &_devicesName[0], _numberOfJoints * 2);

	return API_SUCCESS;
}

int Simulator_Manipulator::Disable(void)
{

	return API_SUCCESS;
}

int Simulator_Manipulator::SetParameter(Property parameter)
{
	if(parameter.FindName("NumberOfJoints") == false)		return API_ERROR;

	if(parameter.FindName("IP") == false)					return API_ERROR;
	if(parameter.FindName("Port") == false)					return API_ERROR;
	if(parameter.FindName("RobotName") == false)			return API_ERROR;

	_numberOfJoints = atoi(parameter.GetValue("NumberOfJoints").c_str());
	_devicesName.resize(_numberOfJoints * 2);

	char name[128];

	for(size_t i = 0; i < _numberOfJoints; i++) {
		sprintf(name, "MotorName%d", i);
		if(parameter.FindName(name) == false) {
			_numberOfJoints = 0;
			_devicesName.clear();
			return API_ERROR;
		}
		_devicesName[i] = parameter.GetValue(name);

		sprintf(name, "EncoderName%d", i);
		if(parameter.FindName(name) == false) {
			_numberOfJoints = 0;
			_devicesName.clear();
			return API_ERROR;
		}
		_devicesName[i + _numberOfJoints] = parameter.GetValue(name);
	}

	_ip = parameter.GetValue("IP");
	_port = atoi(parameter.GetValue("Port").c_str());
	_robotName = parameter.GetValue("RobotName");

	return API_SUCCESS;
}

int Simulator_Manipulator::GetParameter(Property &parameter)
{
	return API_SUCCESS;
}

int Simulator_Manipulator::Stop(void)
{
	return API_SUCCESS;
}

int Simulator_Manipulator::RunHoming(void)
{
	return API_SUCCESS;
}

int Simulator_Manipulator::SetPosition(vector<double> position, vector<unsigned long> time)
{
	if(_numberOfJoints != position.size()) {
		return API_ERROR;
	}

	float q;
	for(size_t i = 0; i < position.size(); i++) {
		q = (float)(position[i]);

		if(_clientSocket.setPosition(_devicesInfo.deviceID[i], q) == false) {
			return API_ERROR;
		}
	}

	return API_SUCCESS;
}

int Simulator_Manipulator::GetPosition(vector<double> &position)
{
	position.resize(_numberOfJoints);

	for(size_t i = 0; i < _numberOfJoints; i++) {
		float buf;

		if(_clientSocket.getPosition(_devicesInfo.deviceID[_numberOfJoints + i], &buf) == false) {
			position.clear();
			return API_ERROR;
		}

		position[i] = (double)(buf / M_PI * 180.0);
	}

	return API_ERROR;	
}

#if defined(WIN32)
extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_Manipulator();
}

#endif