#include "Simulator_LaserScanner.h"
#include "device/OprosPrintMessage.h"


#define LASER_MAX_SIZE	65
#define	MAXIMUM_CLUSTER_SIZE			(99)

Simulator_LaserScanner::Simulator_LaserScanner(void)
{

}

Simulator_LaserScanner::~Simulator_LaserScanner(void)
{

}

int Simulator_LaserScanner::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS) {
		PrintMessage("Simulator_LaserComp :: initialize fails\n");
		return API_ERROR;
	}
	PrintMessage("Simulator_LaserComp :: initialize Success\n");
	return API_SUCCESS;
}

int Simulator_LaserScanner::Enable(void)
{
	if(m_ClientSocket.enable(SimulatorIP,PortNumber) == false)
	{
		return API_ERROR;
	}

	deviceID = m_ClientSocket.findSystem(RobotName,DeviceName);

		return API_SUCCESS;
}

int Simulator_LaserScanner::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_LaserScanner::Finalize(void)
{
	return API_SUCCESS;
}

int Simulator_LaserScanner::SetParameter(Property parameter)
{
	maximumStep = atoi(parameter.GetValue("MaximumStep").c_str());
	startStep = atoi(parameter.GetValue("StartStep").c_str());
	endStep = atoi(parameter.GetValue("EndStep").c_str());
	deltaStep = atoi(parameter.GetValue("DeltaStep").c_str());

	SimulatorIP	= parameter.GetValue("userMessage").c_str();
	DeviceName	= parameter.GetValue("deviceName").c_str();
	RobotName	= parameter.GetValue("RobotName").c_str();
	PortNumber	= atoi(parameter.GetValue("Port").c_str());


	if(endStep < 0 || endStep > maximumStep) {
		return API_ERROR;
	}

	if(startStep < 0 || startStep >= endStep || startStep >= endStep) {
		return API_ERROR;
	}

	if(deltaStep <= 0 || deltaStep > MAXIMUM_CLUSTER_SIZE) {
		return API_ERROR;
	}

	scanStepSize = (endStep	- startStep) / deltaStep + 1;
	scannedData.resize(scanStepSize);
	m_ClientSocket.StepSize = scanStepSize;

	this->parameter = parameter;

	return 0;

}

int Simulator_LaserScanner::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int Simulator_LaserScanner::SetSensorValue(vector<float> &sensorValue)
{
	float *value = new float[sensorValue.size()];

	for (int i = 0 ; i < sensorValue.size() ; i++)
	{
		value[i] = sensorValue[i];
	}

	m_ClientSocket.setValue(deviceID,value,sensorValue.size());

	delete[] value;

	return true;
}

extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_LaserScanner();
}