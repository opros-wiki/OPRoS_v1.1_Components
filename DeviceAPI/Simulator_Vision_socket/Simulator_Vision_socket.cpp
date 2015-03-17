#include "Simulator_Vision_socket.h"
#include "device/OprosPrintMessage.h"

#define ImageSize	320*240*3

Simulator_Vision::Simulator_Vision(void)
{
}

Simulator_Vision::~Simulator_Vision(void)
{
}

int Simulator_Vision::Initialize(Property parameter)
{
	if(SetParameter(parameter) != API_SUCCESS) {
		PrintMessage("Simulator_CameraComp :: initialize fails\n");
		return API_ERROR;
	}
	PrintMessage("Simulator_CameraComp :: initialize Success\n");
	return API_SUCCESS;
}

int Simulator_Vision::Enable(void)
{

	if(m_ClientSocket.enable(SimulatorIP,PortNumber) == false)
	{
		return API_ERROR;
	}

	deviceID = m_ClientSocket.findSystem(RobotName,DeviceName);

	return API_SUCCESS;
}

int Simulator_Vision::Disable(void)
{
	return API_SUCCESS;
}

int Simulator_Vision::Finalize(void)
{
	return API_SUCCESS;
}

int Simulator_Vision::GetParameter(Property &parameter)
{
	parameter = this->parameter;

	return API_SUCCESS;
}

int Simulator_Vision::SetParameter(Property parameter)
{
	SimulatorIP	= parameter.GetValue("userMessage").c_str();
	DeviceName	= parameter.GetValue("deviceName").c_str();
	RobotName	= parameter.GetValue("RobotName").c_str();
	PortNumber	= atoi(parameter.GetValue("Port").c_str());
	
	return API_SUCCESS;
}

int Simulator_Vision::GetImage(unsigned char *image)
{
	unsigned char ImageData[230400];
	if(m_ClientSocket.getValue(deviceID,ImageData) == false)
	{
		return false;
	}
	else
	{
		for(int i = 0 ; i < ImageSize ; i++)
		{
			*image++  = ImageData[i];
			//printf ("%d ",ImageData[i]);
		}
		return API_SUCCESS;
	}
}

extern "C"
{
	__declspec(dllexport) OprosApi *GetAPI();
}

OprosApi *GetAPI()
{
	return new Simulator_Vision();
}