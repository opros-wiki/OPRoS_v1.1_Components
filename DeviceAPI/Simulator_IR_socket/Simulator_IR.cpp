#include "data.h"
#include "Simulator_IR.h"
#include "OprosPrintMessage.h"

using namespace std;

string ParsingUserMessage(string *userMsg)
{
	string tmp = "";
	int len = userMsg->find(";");

	tmp.assign(*userMsg, 0, len);
	userMsg->erase(0, len + 1);

	return tmp;
}


Simulator_IR::Simulator_IR(void)
{
}

Simulator_IR::~Simulator_IR(void)
{
}

int Simulator_IR::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS)	{
		PrintMessage("Simulator_IR :: initialize fails\n");
		return API_ERROR;
	}
	PrintMessage("Simulator_IR :: initalize Success\n");
	return API_SUCCESS;

	if (SetParameter (parameter) == false) {
		return false;
	}
}

int Simulator_IR::Enable(void)
{
	if (m_ClientSocket.enable(SimulatorIP,PortNumber) == false)	{
		return API_ERROR;
	}
	redata = m_ClientSocket.findSystem(RobotName,DeviceName,sensorCount);

	return API_SUCCESS;
}

int Simulator_IR::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_IR::Finalize(void)
{
	return API_SUCCESS;
}

int Simulator_IR::SetParameter(Property parameter)
{
	SimulatorIP	= parameter.GetValue("IP").c_str();
	sensorCount	= atoi(parameter.GetValue("Size").c_str());
	deviceName	= parameter.GetValue("deviceName").c_str();
	RobotName	= parameter.GetValue("RobotName").c_str();
	PortNumber	= atoi(parameter.GetValue("Port").c_str());

	if(sensorCount > PSD_MAX_SIZE)	{
		sensorCount = PSD_MAX_SIZE;
	}

	for(int i=0 ; i<sensorCount ; i++)	{
		DeviceName[i] = ParsingUserMessage(&deviceName);
	}

	this->parameter = parameter;

	return 0;
}

int Simulator_IR::GetParameter(Property &parameter)
{
	parameter = this->parameter;
	return API_SUCCESS;
}

int Simulator_IR::GetSensorValue(vector<double> &PsdSensorData)
{
	float *sensorValue = new float [PSD_MAX_SIZE];

	if(!ReadPsdSensorArray(sensorValue, sensorCount))	{
		delete [] sensorValue;
		return -1;
	}

	for(int i = 0 ; i < sensorCount ; i++)	{
		PsdSensorData.push_back((double)sensorValue[i]);
	}

	return sensorCount;
}



bool Simulator_IR::ReadPsdSensorArray (float sensorValue [], int &sensorSize)
{
	bool ret = false;
	float value;

	if(sensorValue){
		int count = 0;
		for(int i = 0; i < sensorSize && i < PSD_MAX_SIZE ; i++) {
			if(m_ClientSocket.getValue(redata.deviceID[i],&value) == false)
			{
				return false;
			}
			if (value < 0 )	value = 10;
			sensorValue[i] = value;
			++count;
		}
		sensorSize = count;
		ret = true;
	}else{
		sensorSize = 0;
	}

	return ret;
}

extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_IR();
}