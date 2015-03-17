#include "DynamixelGripper.h"

#include <device/ServoActuator.h>
#include <device/OprosPrintMessage.h>

#include "DynamixelUARTDef.h"

DynamixelGripper::DynamixelGripper()
	: uart(NULL), gripperControlThread(NULL), mIsGripped(false)
{}

DynamixelGripper::~DynamixelGripper()
{
	Finalize();
}

int DynamixelGripper::Initialize( Property parameter )
{
	if (Setting(parameter) == false)
	{
		PrintMessage("Error : DynamixelManipulator::Initialize()->Can't Initialize() << %s(%d)\n", __FILE__, __LINE__);
		_status = DEVICE_ERROR;
		return API_ERROR;
	}

	previousPosition.resize(dynamixelGroup.CountDynamixel());

	_status = DEVICE_READY;
	return API_SUCCESS;
}

int DynamixelGripper::Finalize()
{
	if (_status == DEVICE_ACTIVE)
		Disable();

	dynamixelGroup.Clear();
	gripper.reset();

	if(uart != NULL)
	{
		delete uart;
		uart = NULL;
	}

	uartLibraryLoader.reset();

	_status = DEVICE_CREATED;
	return API_SUCCESS;
}

int DynamixelGripper::Enable()
{
	if (_status == DEVICE_ACTIVE)
	{
		return API_SUCCESS;
	}
	else if(_status != DEVICE_READY)
	{
		PrintMessage("Error : DynamixelManipulator::Enable()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}
	
	bool error = false;
	const size_t dynamixelCount = dynamixelGroup.CountDynamixel();
	for (size_t i = 0; i < dynamixelCount; i++)
	{	
		//다이나믹셀 초기 설정
		if (EnableDynamixel(dynamixelGroup[i], dynamixelPropertyVector[i]) == false)
		{
			error = true;
		}
	}
		
	if (EnableDynamixel(*gripper, gripperProperty) == false)
	{
		error = true;
	}

	if (error)
	{
		_status = DEVICE_ERROR;
		return API_ERROR;
	}

	gripperControlThread = new boost::thread(boost::bind(&DynamixelGripper::GripperControlThreadHandler, this));

	_status = DEVICE_ACTIVE;
	return API_SUCCESS;
}

int DynamixelGripper::Disable()
{
	if (_status == DEVICE_READY)
	{
		return API_SUCCESS;
	}
	else if(_status != DEVICE_ACTIVE)
	{
		PrintMessage("Error : DynamixelManipulator::Disable()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (gripperControlThread != NULL)
	{
		gripperControlThread->interrupt();
		gripperControlThread->timed_join(boost::posix_time::millisec(100));
		delete gripperControlThread;
		gripperControlThread = NULL;
	}

	bool error = false;
	const size_t dynamixelCount = dynamixelGroup.CountDynamixel();
	for (size_t i = 0; i < dynamixelCount; i++)
	{
		if(dynamixelGroup[i].SetTorqueEnable(0) == false)
		{
			PrintMessage("Error : DynamixelManipulator::Disable()->Can't disable Torque[%d]<< %s(%d)\r\n", i, __FILE__, __LINE__);
			error = true;
		}
		else if (dynamixelGroup[i].SetLED(0) == false)
		{
			PrintMessage("Error : DynamixelManipulator::Disable()->Can't disable LED[%d]<< %s(%d)\r\n", i, __FILE__, __LINE__);
			error = true;
		}
	}

	if (error)
	{
		_status = DEVICE_ERROR;
		return API_ERROR;
	}
	_status =  DEVICE_READY;
	return API_SUCCESS;
}

bool DynamixelGripper::Setting( Property& parameter)
{
	//UARTAPIName
	string uartApiName;
	if (parameter.FindName("UARTAPIName") == false)
	{
		PrintMessage("Error : DynamixelManipulator::Setting()->Can't find UARTAPIName<< %s(%d)\r\n", __FILE__, __LINE__);
		return false;
	}
	uartApiName = parameter.GetValue("UARTAPIName");

	try
	{
		if (uart != NULL)
		{
			delete uart;
			uart = NULL;
			uartLibraryLoader.reset();
		}
		
		uartLibraryLoader.reset(new DynamicLibraryLoader(uartApiName));
	
		uart = static_cast<Uart*>(uartLibraryLoader->GetFunction<GET_OPROS_API>("GetAPI")());

		if(uart->Initialize(parameter) != API_SUCCESS)
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't Initialize UART<< %s(%d)\r\n", __FILE__, __LINE__);
			return false;
		}
		else if(uart->Enable() != API_SUCCESS)
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't Enable UART<< %s(%d)\r\n", __FILE__, __LINE__);
			return false;
		}
	}
	catch (DynamicLibraryLoader::CanNotLoadException&)
	{
		uartLibraryLoader.reset();	
		PrintMessage("Error : DynamixelManipulator::Setting()->Can't Load UART<< %s(%d)\r\n", __FILE__, __LINE__);
		return false;
	}

	dynamixelPropertyVector.clear();
	dynamixelGroup.Clear();
	dynamixelGroup.SetUart(uart);
	
	if (parameter.FindName("Size") == false)
	{
		PrintMessage("Error : DynamixelManipulator::Setting()->Can't find Size<< %s(%d)\r\n", __FILE__, __LINE__);
		return false;
	}
	const size_t dynamixelCount = atoi(parameter.GetValue("Size").c_str());

	const string DYNAMIXEL_ID = "DynamixelID";
	const string COMPLIANCE_MARGINE = "ComplianceMargine";
	const string COMPLIANCE_SLOPE = "ComplianceSlope";
	const string POSITION_RESOLUTION = "PositionResolution";
	const string POSITION_OFFSET = "PositionOffset";
	const string MAXIMUM_POWER = "MaximumPower";
	const string MAXIMUM_VELOCITY = "MaximumVelocity";
	const string MINIMUM_POSITION_LIMIT = "MinimumPositionLimit";
	const string MAXIMUM_POSITION_LIMIT = "MaximumPositionLimit";
	
	PrintMessage("\r\nDynamixelGripper Property Setting\r\n");

	char buff[100] = {0, };
	for (unsigned int i = 0;  i < dynamixelCount; i++)
	{
		DynamixelProperty dynamixelProperty;
			
		//DynamixelID
		sprintf(buff, "%s%d", DYNAMIXEL_ID.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.id = atoi(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %d \r\n", buff, dynamixelProperty.id);
		
		//ComplianceMargine
		sprintf(buff, "%s%d", COMPLIANCE_MARGINE.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.complianceMargine = atoi(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %d \r\n", buff, dynamixelProperty.complianceMargine);

		//ComplianceSlope
		sprintf(buff, "%s%d", COMPLIANCE_SLOPE.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.compliacneSlope = atoi(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %d \r\n", buff, dynamixelProperty.compliacneSlope);

		//PositionResolution
		sprintf(buff, "%s%d", POSITION_RESOLUTION.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.positionResolution = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.positionResolution);

		//PositionOffset
		sprintf(buff, "%s%d", POSITION_OFFSET.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.positionOffset = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.positionOffset);

		//MaximumPower
		sprintf(buff, "%s%d", MAXIMUM_POWER.c_str(), i);
		if (parameter.FindName(buff) == false)
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximumPower = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximumPower);
		
		//MaximumVelocity
		sprintf(buff, "%s%d", MAXIMUM_VELOCITY.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximuVelocity = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximuVelocity);
			
		//MinimumPositionLimit
		sprintf(buff, "%s%d", MINIMUM_POSITION_LIMIT.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.minimumPositionLimit = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.minimumPositionLimit);

		//MaximumPositionLimit
		sprintf(buff, "%s%d", MAXIMUM_POSITION_LIMIT.c_str(), i);
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximumPositionLimit = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximumPositionLimit);
		
		PrintMessage("\r\n");

		DynamixelUART dynamixel(uart, dynamixelProperty.id);
		dynamixelGroup.Add(dynamixel);
		dynamixelPropertyVector.push_back(dynamixelProperty);
	}

	{
		GripperDynamixelProperty dynamixelProperty;

		//DynamixelID
		sprintf(buff, "Gripper%s", DYNAMIXEL_ID.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.id = atoi(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %d \r\n", buff, dynamixelProperty.id);	

		//ComplianceMargine
		sprintf(buff, "Gripper%s", COMPLIANCE_MARGINE.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.complianceMargine = atoi(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %d \r\n", buff, dynamixelProperty.complianceMargine);	

		//ComplianceSlope
		sprintf(buff, "Gripper%s", COMPLIANCE_SLOPE.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.compliacneSlope = atoi(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %d \r\n", buff, dynamixelProperty.compliacneSlope);	

		//PositionResolution
		sprintf(buff, "Gripper%s", POSITION_RESOLUTION.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.positionResolution = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.positionResolution);	

		//PositionOffset
		sprintf(buff, "Gripper%s", POSITION_OFFSET.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.positionOffset = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.positionOffset);	

		//MaximumPower
		sprintf(buff, "Gripper%s", MAXIMUM_POWER.c_str());
		if (parameter.FindName(buff) == false)
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximumPower = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximumPower);	

		//MaximumVelocity
		sprintf(buff, "Gripper%s", MAXIMUM_VELOCITY.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximuVelocity = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximuVelocity);	

		//MinimumPositionLimit
		sprintf(buff, "Gripper%s", MINIMUM_POSITION_LIMIT.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.minimumPositionLimit = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.minimumPositionLimit);	

		//MaximumPositionLimit
		sprintf(buff, "Gripper%s", MAXIMUM_POSITION_LIMIT.c_str());
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximumPositionLimit = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximumPositionLimit);	

		//MaximumLoad
		sprintf(buff, "GripperMaximumLoad");
		if (parameter.FindName(buff) == false) 
		{
			PrintMessage("Error : DynamixelManipulator::Setting()->Can't find %s<< %s(%d)\r\n", buff, __FILE__, __LINE__);
			return false;
		}
		dynamixelProperty.maximumLoad = atof(parameter.GetValue(buff).c_str());
		PrintMessage("%s : %lf \r\n", buff, dynamixelProperty.maximumLoad);	

		PrintMessage("\r\n");	

		gripperProperty = dynamixelProperty;
		gripper.reset(new DynamixelUART(uart, gripperProperty.id));
	}


	return true;
}

int DynamixelGripper::SetParameter( Property parameter )
{
	if (_status == DEVICE_ACTIVE)
	{
		Disable();
	}
	else if(_status != DEVICE_READY)
	{
		PrintMessage("Error : DynamixelManipulator::SetParameter()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (Setting(parameter) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SetParameter()->Can't set parameter.<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (Enable() != API_SUCCESS)
	{
		return API_ERROR;
	}

	return API_SUCCESS;
}

bool DynamixelGripper::EnableDynamixel( DynamixelUART& dynamixel, const DynamixelProperty& property )
{
	if(dynamixel.SetStatusReturnLevel(1) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't set _statusRetrunLevel of ID(%d)<< %s(%d)\r\n", property.id,__FILE__, __LINE__);
		return false;
	}
	else if (dynamixel.SetCCWComplianceMargin(property.complianceMargine) == false
		|| dynamixel.SetCWComplianceMargin(property.complianceMargine) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't set ComplianceMargin of ID(%d)<< %s(%d)\r\n", property.id,__FILE__, __LINE__);
		return false;
	}
	else if(dynamixel.SetCCWComplianceSlope(property.compliacneSlope) == false
		|| dynamixel.SetCWComplianceSlope(property.compliacneSlope) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't set ComplianceSlope of ID(%d)<< %s(%d)\r\n", property.id,__FILE__, __LINE__);
		return false;
	}
	else if(dynamixel.SetMaximumTorque(ConvertPowerUnitToDynamixel(property.maximumPower)) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't set MaximumPower of ID(%d)<< %s(%d)\r\n", property.id,__FILE__, __LINE__);
		return false;
	}
	else if(dynamixel.SetMovingSpeed(ConvertVelocityUnitToDynamixel(property.maximuVelocity * 60 / 360)) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't set MaximumVelocity of ID(%d)<< %s(%d)\r\n", property.id,__FILE__, __LINE__);
		return false;
	}
	else if(dynamixel.SetCCWAngleLimit(ConvertPositionUnitToDynamixel(property.maximumPositionLimit, property.positionOffset, property.positionResolution)) == false
		|| dynamixel.SetCWAngleLimit(ConvertPositionUnitToDynamixel(property.minimumPositionLimit, property.positionOffset, property.positionResolution)) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't set PositionLimit of ID(%d)<< %s(%d)\r\n", property.id,__FILE__, __LINE__);
		return false;
	}
	else if (dynamixel.SetLED(1) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't enable LED of ID(%d)<< %s(%d)\r\n", property.id, __FILE__, __LINE__);
		return false;
	}
	else if(dynamixel.SetAlarmShutdown(OVERLOAD_ERROR | OVERHEATING_ERROR | INPUTVOLTAGE_ERROR) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't enable AlarmShutdown of ID(%d)<< %s(%d)\r\n", property.id, __FILE__, __LINE__);
		return false;
	}
	else if(dynamixel.SetTorqueEnable(1) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SettingDynamixel()->Can't enable Torque of ID(%d)<< %s(%d)\r\n", property.id, __FILE__, __LINE__);
		return false;
	}
	
	return true;
}

int DynamixelGripper::GetParameter( Property& parameter )
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::GetParameter()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (uart->GetParameter(parameter) != API_SUCCESS)
	{
		PrintMessage("Error : DynamixelManipulator::GetParameter()->Can't get parameter to UART.<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	const string DYNAMIXEL_ID = "DynamixelID";
	const string COMPLIANCE_MARGINE = "ComplianceMargine";
	const string COMPLIANCE_SLOPE = "ComplianceSlope";
	const string POSITION_RESOLUTION = "PositionResolution";
	const string POSITION_OFFSET = "PositionOffset";
	const string MAXIMUM_POWER = "MaximumPower";
	const string MAXIMUM_VELOCITY = "MaximumVelocity";
	const string MINIMUM_POSITION_LIMIT = "MinimumPositionLimit";
	const string MAXIMUM_POSITION_LIMIT = "MaximumPositionLimit";
	

	char buff[100] = {0, };
	const size_t dynamixelCount = dynamixelGroup.CountDynamixel();
	stringstream stringStream;

	for (size_t i = 0;  i < dynamixelCount; i++)
	{
		DynamixelProperty& dynamixelProperty = dynamixelPropertyVector[i];

		//DynamixelID
		sprintf(buff, "%s%d", DYNAMIXEL_ID, i);
		stringStream.str("");
		stringStream << dynamixelProperty.id;
		parameter.SetValue(buff, stringStream.str());

		//ComplianceMargine
		sprintf(buff, "%s%d", COMPLIANCE_MARGINE.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.complianceMargine;
		parameter.SetValue(buff, stringStream.str());

		//ComplianceSlope
		sprintf(buff, "%s%d", COMPLIANCE_SLOPE.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.compliacneSlope;
		parameter.SetValue(buff, stringStream.str());

		//PositionResolution
		sprintf(buff, "%s%d", POSITION_RESOLUTION.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.positionResolution;
		parameter.SetValue(buff, stringStream.str());

		//PositionOffset
		sprintf(buff, "%s%d", POSITION_OFFSET.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.positionOffset;
		parameter.SetValue(buff, stringStream.str());

		//MaximumPower
		sprintf(buff, "%s%d", MAXIMUM_POWER.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.maximumPower;
		parameter.SetValue(buff, stringStream.str());

		//MaximumVelocity
		sprintf(buff, "%s%d", MAXIMUM_VELOCITY.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.maximuVelocity;
		parameter.SetValue(buff, stringStream.str());

		//MinimumPositionLimit
		sprintf(buff, "%s%d", MINIMUM_POSITION_LIMIT.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.minimumPositionLimit;
		parameter.SetValue(buff, stringStream.str());

		//MaximumPositionLimit
		sprintf(buff, "%s%d", MAXIMUM_POSITION_LIMIT.c_str(), i);
		stringStream.str("");
		stringStream << dynamixelProperty.maximumPositionLimit;
		parameter.SetValue(buff, stringStream.str());
	}
	
	return API_SUCCESS;
}

int DynamixelGripper::RunHoming()
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::StartHoming()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	//모든 위치를 0으로 
	vector<double> position(dynamixelGroup.CountDynamixel());
	vector<unsigned long> time(dynamixelGroup.CountDynamixel());

	uart->Lock();
	if (SetPosition(position,time) != API_SUCCESS)
	{
		PrintMessage("Error : DynamixelManipulator::StartHoming()->Can't StartHoming Dynamixel<< %s(%d)\r\n", __FILE__, __LINE__);
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();
	return API_SUCCESS;
}

int DynamixelGripper::Stop()
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::Stop()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	vector<double> position;
	vector<unsigned long> time;

	uart->Lock();
	if (GetPosition(position) != API_SUCCESS)
	{
		PrintMessage("Error : DynamixelManipulator::Stop()->Can't Stop Dynamixel.<< %s(%d)\r\n", __FILE__, __LINE__);
		uart->Unlock();
		return API_ERROR;
	}

	time.resize(position.size());

	if (SetPosition(position, time) != API_SUCCESS)
	{
		PrintMessage("Error : DynamixelManipulator::Stop()->Can't Stop Dynamixel.<< %s(%d)\r\n", __FILE__, __LINE__);
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();
	return API_SUCCESS;
}

int DynamixelGripper::EmergencyStop()
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::EmergencyStop()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}
	
	uart->Lock();
	if (dynamixelGroup.SetTorqueEnable(false) == false)
	{
		PrintMessage("Error : DynamixelManipulator::EmergencyStop()->Can't EmergencyStop Dynamixel.<< %s(%d)\r\n", __FILE__, __LINE__);
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();
	return API_SUCCESS;
}

int DynamixelGripper::SetPosition( vector<double> position, vector<unsigned long> time )
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::SetPosition()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (position.size() != dynamixelGroup.CountDynamixel())
	{
		PrintMessage("Error : DynamixelManipulator::SetPosition()->position size must be equal Dynamixel count<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	if (time.size() != position.size())
	{
		PrintMessage("Error : DynamixelManipulator::SetPosition()->time size must be equal Dynamixel count<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	//단위 변환
	vector<unsigned short> dynamixelPositon(position.size());
	for (size_t i = 0;  i < dynamixelPositon.size(); i++)
	{
		dynamixelPositon[i] = ConvertPositionUnitToDynamixel(position[i], dynamixelPropertyVector[i].positionOffset, dynamixelPropertyVector[i].positionResolution);
	}

	uart->Lock();
	if (dynamixelGroup.SetGoalPosition(dynamixelPositon) == false)
	{
		PrintMessage("Error : DynamixelManipulator::SetPosition()->Can't SetPosition Dynamixel<< %s(%d)\r\n", __FILE__, __LINE__);
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();

	previousPosition = position;

	return API_SUCCESS;
}

int DynamixelGripper::GetPosition( vector<double> &position )
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::GetPosition()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	size_t dynamixelCount = dynamixelGroup.CountDynamixel();

	position.resize(dynamixelCount);
	position = previousPosition;
	return API_SUCCESS;

	uart->Lock();

	bool error = false;
	for (unsigned int i = 0; i < dynamixelCount; i++)
	{
		unsigned short presentPositionRaw = 0;
		if (dynamixelGroup[i].GetPresentPosition(presentPositionRaw) == false)
		{
			PrintMessage("Error : DynamixelManipulator::GetPosition()->Can't GetPosition Dynamixel[%d]<< %s(%d)\r\n", i, __FILE__, __LINE__);
			error = true;
			continue;
		}

		//convert
		position[i] = ConvertPositionUnitToDegree(presentPositionRaw
			, dynamixelPropertyVector[i].positionOffset
			, dynamixelPropertyVector[i].positionResolution);
	}
	uart->Unlock();

	if (error)
		return API_ERROR;
	return API_SUCCESS;
}

int DynamixelGripper::StartGripping()
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::IsGripped()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	uart->Lock();
	gripper->SetGoalPosition(ConvertPositionUnitToDynamixel(gripperProperty.maximumPositionLimit, gripperProperty.positionOffset, gripperProperty.positionResolution));
	uart->Unlock();

	mIsGripped = true;

	return API_SUCCESS;
}

int DynamixelGripper::StopGripping()
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::IsGripped()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}

	//gripperMessageQueue.Push(STOP_GRIPPING);

	uart->Lock();
	gripper->SetGoalPosition(ConvertPositionUnitToDynamixel(gripperProperty.minimumPositionLimit, gripperProperty.positionOffset, gripperProperty.positionResolution));
	uart->Unlock();

	mIsGripped = false;
	return API_SUCCESS;
}

int DynamixelGripper::IsGripped(bool &isGripped)
{
	if(_status == DEVICE_CREATED)
	{
		PrintMessage("Error : DynamixelManipulator::IsGripped()->Precondition not met<< %s(%d)\r\n", __FILE__, __LINE__);
		return API_ERROR;
	}
	isGripped = mIsGripped;
	return API_SUCCESS;

	uart->Lock();

	unsigned short dynamixelValue = 0;
	double loadPercent = 0.0;
	
	if (gripper->GetPresentLoad(dynamixelValue) == false)
	{
		PrintMessage("Error : DynamixelManipulator::IsGripped()->Can't GetPresentLoad of Gripper<< %s(%d)\r\n", __FILE__, __LINE__);
		uart->Unlock();
		return API_ERROR;
	}
	uart->Unlock();

	loadPercent = -ConvertLoadUnitToPercent(dynamixelValue);
	
	std::cout << loadPercent << std::endl;

	if (loadPercent > gripperProperty.maximumLoad * 0.9)
	{
		return 1;
	}
	
	return API_SUCCESS;
}

void DynamixelGripper::GripperControlThreadHandler()
{
	return;

	try
	{
		GripperCommand command;
		for (;;)
		{	
			command = gripperMessageQueue.Pop();

			switch(command)
			{
			case START_GRIPPING:
				{
					double presentPosition = 1;
					double presentLoad = 1;
				
					struct ScopedLock
					{
						ScopedLock(Uart* pUart)
							: mpUart(pUart)
						{mpUart->Lock();}

						~ScopedLock(){mpUart->Unlock();}

						Uart* mpUart;
					};

					try
					{
						for(;; boost::this_thread::sleep(boost::posix_time::millisec(100)))
						{
							ScopedLock lock(uart);

							unsigned short dynamixelValue;
							if(gripper->GetPresentPosition(dynamixelValue))
							{
								presentPosition = ConvertPositionUnitToDegree(dynamixelValue, gripperProperty.positionOffset, gripperProperty.positionResolution);
							}

							if(gripper->GetPresentLoad(dynamixelValue))
							{
								presentLoad = -ConvertLoadUnitToPercent(dynamixelValue);
							}
							
							if (presentLoad > gripperProperty.maximumLoad * 0.9)
							{
								break;
							}

							double loadDifference = gripperProperty.maximumLoad - presentLoad;
							double positionDifference = (gripperProperty.maximumPositionLimit - gripperProperty.minimumPositionLimit) / 7 * (loadDifference / gripperProperty.maximumLoad);

							gripper->SetGoalPosition(ConvertPositionUnitToDynamixel(presentPosition + positionDifference, gripperProperty.positionOffset, gripperProperty.positionResolution));
						}
					}
					catch(...)
					{
					}
				}
				break;
			case  STOP_GRIPPING:
				{
					uart->Lock();
					gripper->SetGoalPosition(ConvertPositionUnitToDynamixel(gripperProperty.minimumPositionLimit, gripperProperty.positionOffset, gripperProperty.positionResolution));
					uart->Unlock();
				}
				break;
			}
		}
	}
	catch(boost::thread_interrupted&)
	{
	}
}

unsigned short DynamixelGripper::ConvertPowerUnitToDynamixel( const double& percent)
{
	opros_assert(!(percent < 0.00 || percent > 100.0));
	unsigned short dynamixelValue = unsigned short(percent / 0.1);
	return dynamixelValue > 1023 ? 1023 : dynamixelValue; 
}

unsigned short DynamixelGripper::ConvertPositionUnitToDynamixel( const double& degree, const double& offset, const double& resolution)
{
	//opros_assert(!(degree > offset || degree < -offset));
	unsigned short dynamixelValue = unsigned short((degree + offset) / resolution);
	return dynamixelValue; 
}

unsigned short DynamixelGripper::ConvertVelocityUnitToDynamixel( const double& rpm )
{
	unsigned short dynamixelValue = unsigned short(rpm / 0.111);
	return dynamixelValue > 1023 ? 1023 : dynamixelValue; 
}

double DynamixelGripper::ConvertPowerUnitToPercent( unsigned short dynamixelValue )
{
	opros_assert(!(dynamixelValue > 1023));
	double percent = dynamixelValue * 0.1;
	return percent;
}

double DynamixelGripper::ConvertPositionUnitToDegree( unsigned short dynamixelValue, const double& offset, const double& resolution )
{
	double degree = dynamixelValue * resolution - offset;
	return degree;
}

double DynamixelGripper::ConvertVelocityUnitToRPM( unsigned short dynamixelValue )
{
	return dynamixelValue * 0.111;
}

double DynamixelGripper::ConvertLoadUnitToPercent( unsigned short dynamixelValue )
{
	opros_assert(!(dynamixelValue < 0 || dynamixelValue > 2047));
	double percent = (dynamixelValue & 0x3FF) * 0.1 * (dynamixelValue & 0x400 ? -1 : 1);
	return percent;
}


#ifdef WIN32
extern "C"
{
	__declspec(dllexport) OprosApi* GetAPI();
}

OprosApi* GetAPI()
{
	return new DynamixelGripper();
}

#else
extern "C"
{
	OprosApi* GetAPI();
}

OprosApi* GetAPI()
{
	return new SharedUART;
}

#endif