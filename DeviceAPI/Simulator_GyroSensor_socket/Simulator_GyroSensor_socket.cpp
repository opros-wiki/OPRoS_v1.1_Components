#include "Simulator_GyroSensor_socket.h"
#include "OprosPrintMessage.h"

Simulator_GyroSensor::Simulator_GyroSensor(void)
{

}

Simulator_GyroSensor::~Simulator_GyroSensor(void)
{

}

int Simulator_GyroSensor::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS) {
		PrintMessage("Simulator_GyroSensorComp :: initialize fails\n");
		return API_ERROR;
	}
	PrintMessage("Simulator_GyroSensorComp :: initialize Success\n");
	return API_SUCCESS;
}

int Simulator_GyroSensor::Enable(void)
{
	if(m_ClientSocket.enable(SimulatorIP,PortNumber) == false)
	{
		return API_ERROR;
	}

	redata = m_ClientSocket.findSystem(RobotName,deviceName);

	return API_SUCCESS;
}

int Simulator_GyroSensor::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_GyroSensor::Finalize(void)
{
	return API_SUCCESS;
}

int Simulator_GyroSensor::SetParameter(Property parameter)
{
	SimulatorIP		= parameter.GetValue("userMessage").c_str();
	deviceName		= parameter.GetValue("GyroName").c_str();
	RobotName		= parameter.GetValue("RobotName").c_str();
	PortNumber		= atoi(parameter.GetValue("Port").c_str());

	this->parameter = parameter;
	return 0;
}

int Simulator_GyroSensor::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int Simulator_GyroSensor::GetSensorValue(std::vector<ObjectRotation> &GyroSensorData)
{
	float gyroValue[3];

	ObjectRotation data;
	
	m_ClientSocket.getPosition(redata.deviceID[0],gyroValue);

	data.roll = gyroValue[0];
	data.pitch = gyroValue[1];
	data.yaw = gyroValue[2];

	GyroSensorData.push_back(data);

	return true;
}



#if !defined(DEVICE_TEST)
#if defined(WIN32)
extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_GyroSensor();
}
#else
extern "C"
{
	OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_GyroSensor();
}
#endif
#endif