#include "data.h"
#include "Simulator_Tachometer_socket.h"
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

Simulator_Tachometer_socket::Simulator_Tachometer_socket(void)
{
}
Simulator_Tachometer_socket::~Simulator_Tachometer_socket(void)
{
}

int Simulator_Tachometer_socket::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS)	{
		PrintMessage("Simulator_EncoderComp :: initialize fails\n");
		return API_ERROR;
	}
	PrintMessage("Simulator_EncoderComp :: initalize Success\n");
	return API_SUCCESS;

	if (SetParameter (parameter) == false) {
		return false;
	}
}


int Simulator_Tachometer_socket::Enable(void)
{
	if (m_ClientSocket.enable(SimulatorIP,PortNumber) == false)	{
		return API_ERROR;
	}
	redata = m_ClientSocket.findSystem(RobotName,DeviceName,sensorCount);

	return API_SUCCESS;
}

int Simulator_Tachometer_socket::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_Tachometer_socket::Finalize(void)
{
	return API_SUCCESS;
}

int Simulator_Tachometer_socket::SetParameter(Property parameter)
{
	SimulatorIP = parameter.GetValue("IP").c_str();
	sensorCount = atoi(parameter.GetValue("Size").c_str());
	deviceName = parameter.GetValue("deviceName").c_str();
	RobotName = parameter.GetValue("RobotName").c_str();
	PortNumber	= atoi(parameter.GetValue("Port").c_str());

	if(sensorCount > TACHOMETER_MAX_SIZE)	{
		sensorCount = TACHOMETER_MAX_SIZE;
	}

	for(int i=0 ; i<sensorCount ; i++)	{
		DeviceName[i] = ParsingUserMessage(&deviceName);
	}

	this->parameter = parameter;

	return 0;
}

int Simulator_Tachometer_socket::GetParameter(Property &parameter)
{
	parameter = this->parameter;
	return API_SUCCESS;
}

int Simulator_Tachometer_socket::GetSensorValue(vector<double> &TachometerData)
{
	float *sensorValue = new float [TACHOMETER_MAX_SIZE];

	if(!ReadEncoderArray(sensorValue, sensorCount))	{
		delete [] sensorValue;
		return -1;
	}

	for(int i = 0 ; i < sensorCount ; i++)	{
		TachometerData.push_back((double)sensorValue[i]);
	}

	delete [] sensorValue;

	return sensorCount;
}

int Simulator_Tachometer_socket::ReadEncoderArray (float sensorValue [], int &sensorSize)
{

	bool ret = false;
	float value[4];
	int count = 0;

	if(sensorValue){
		for(int i = 0; i < sensorSize && i < TACHOMETER_MAX_SIZE ; i++) {
			if(m_ClientSocket.getValue(redata.deviceID[i],value) == false)
			{
				return false;
			}
			sensorValue[i] = value[0];
			count = count+1;
		}
		sensorSize = count;
		ret = true;
	}else{
		sensorSize = 0;
	}

	return ret;
}

int Simulator_Tachometer_socket::getValue(int index, std::valarray<double> *value)
{

	if(index < 0 || index >= sensorCount) {
		return -1;
	}
	if(index + (int)value->size() > sensorCount) {
		value->resize(sensorCount - index);
	}

	int sensorSize = TACHOMETER_MAX_SIZE;
	float *sensorValue = new float [sensorSize];

	if (!ReadEncoderArray (sensorValue, sensorSize)) {
		delete [] sensorValue;
		return -1;
	}

	if (value) {
		for(int i = 0; i < (int)value->size(); i++) {
			(*value)[i] = (double)sensorValue[index + i];
		}
	}
	else {
		delete [] sensorValue;
		return -1;
	}

	delete [] sensorValue;

	return (int)value->size();
}


extern "C"
{
__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_Tachometer_socket();
}
