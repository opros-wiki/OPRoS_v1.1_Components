#include "Simulator_PositionSensor.h"
#include "OprosPrintMessage.h"

Simulator_PositionSensor::Simulator_PositionSensor(void)
{

}

Simulator_PositionSensor::~Simulator_PositionSensor(void)
{

}

int Simulator_PositionSensor::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS) {
		PrintMessage("Simulator_PositionSensorComp :: initialize fails\n");
		return API_ERROR;
	}
	PrintMessage("Simulator_PositionSensorComp :: initialize Success\n");
	return API_SUCCESS;
}

int Simulator_PositionSensor::Enable(void)
{
	if(m_ClientSocket.enable(SimulatorIP,PortNumber) == false)
	{
		return API_ERROR;
	}

	redata = m_ClientSocket.findSystem(RobotName,deviceName);

	return API_SUCCESS;
}

int Simulator_PositionSensor::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_PositionSensor::Finalize(void)
{
	return API_SUCCESS;
}

int Simulator_PositionSensor::SetParameter(Property parameter)
{
	SimulatorIP		= parameter.GetValue("userMessage").c_str();
	deviceName[0]	= parameter.GetValue("GpsName").c_str();
	deviceName[1]	= parameter.GetValue("GyroName").c_str();
	RobotName	= parameter.GetValue("RobotName").c_str();
	PortNumber		= atoi(parameter.GetValue("Port").c_str());

	this->parameter = parameter;
	return 0;
}

int Simulator_PositionSensor::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int Simulator_PositionSensor::GetSensorValue(std::vector<ObjectPosition> &positionSensorData)
{
	float gpsValue[3];
	float gyroValue[3];

	ObjectPosition data;
	
	m_ClientSocket.getPosition(redata.deviceID[0],gpsValue);
	m_ClientSocket.getPosition(redata.deviceID[1],gyroValue);

	data.x = gpsValue[0];
	data.y = gpsValue[1];
	data.z = gpsValue[2];

	data.roll = gyroValue[0];
	data.pitch = gyroValue[1];
	data.yaw = gyroValue[2];

	positionSensorData.push_back(data);

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
	return new Simulator_PositionSensor();
}
#else
extern "C"
{
	OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_PositionSensor();
}
#endif
#endif