#ifndef _WINDLL

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <conio.h>

#include <device/Property.h>
#include <device/OprosPrintMessage.h>

#include "DynamixelGripper.h"

Property CreateProperty()
{
	Property result;

	result.SetValue("DynamixelServoActuatorAPI", "Robotis_DynamixelUART_WithID");

	result.SetValue("UARTAPIName", "SerialCommunication");

	result.SetValue("PortName", "COM6");
	result.SetValue("TimeOut", "200");
	result.SetValue("BaudRate", "1000000");
	result.SetValue("DataBits", "8");
	result.SetValue("StopBits", "0");
	result.SetValue("Parity", "0");
	result.SetValue("FlowControl", "0");

	result.SetValue("Size", "6");

	result.SetValue("DynamixelID0", "224");
	result.SetValue("ComplianceMargine0", "1");
	result.SetValue("ComplianceSlope0", "128");
	result.SetValue("PositionResolution0", "0.06127472527472527472527472527473");
	result.SetValue("PositionOffset0", "125.46");
	result.SetValue("MaximumPower0", "100");
	result.SetValue("MaximumVelocity0", "90");
	result.SetValue("MinimumPositionLimit0", "-110");
	result.SetValue("MaximumPositionLimit0", "110");

	result.SetValue("DynamixelID1", "225");
	result.SetValue("ComplianceMargine1", "1");
	result.SetValue("ComplianceSlope1", "128");
	result.SetValue("PositionResolution1", "0.06127472527472527472527472527473");
	result.SetValue("PositionOffset1", "125.46");
	result.SetValue("MaximumPower1", "100");
	result.SetValue("MaximumVelocity1", "90");
	result.SetValue("MinimumPositionLimit1", "-90");
	result.SetValue("MaximumPositionLimit1", "90");

	result.SetValue("DynamixelID2", "226");
	result.SetValue("ComplianceMargine2", "1");
	result.SetValue("ComplianceSlope2", "128");
	result.SetValue("PositionResolution2", "0.29296875");
	result.SetValue("PositionOffset2", "150");
	result.SetValue("MaximumPower2", "100");
	result.SetValue("MaximumVelocity2", "90");
	result.SetValue("MinimumPositionLimit2", "-90");
	result.SetValue("MaximumPositionLimit2", "90");

	result.SetValue("DynamixelID3", "227");
	result.SetValue("ComplianceMargine3", "1");
	result.SetValue("ComplianceSlope3", "128");
	result.SetValue("PositionResolution3", "0.29296875");
	result.SetValue("PositionOffset3", "150");
	result.SetValue("MaximumPower3", "100");
	result.SetValue("MaximumVelocity3", "90");
	result.SetValue("MinimumPositionLimit3", "-140");
	result.SetValue("MaximumPositionLimit3", "140");

	result.SetValue("DynamixelID4", "228");
	result.SetValue("ComplianceMargine4", "1");
	result.SetValue("ComplianceSlope4", "128");
	result.SetValue("PositionResolution4", "0.29296875");
	result.SetValue("PositionOffset4", "150");
	result.SetValue("MaximumPower4", "100");
	result.SetValue("MaximumVelocity4", "90");
	result.SetValue("MinimumPositionLimit4", "-90");
	result.SetValue("MaximumPositionLimit4", "90");

	result.SetValue("DynamixelID5", "229");
	result.SetValue("ComplianceMargine5", "1");
	result.SetValue("ComplianceSlope5", "128");
	result.SetValue("PositionResolution5", "0.29296875");
	result.SetValue("PositionOffset5", "150");
	result.SetValue("MaximumPower5", "100");
	result.SetValue("MaximumVelocity5", "90");
	result.SetValue("MinimumPositionLimit5", "-90");
	result.SetValue("MaximumPositionLimit5", "90");

	result.SetValue("GripperDynamixelID", "230");
	result.SetValue("GripperComplianceMargine", "1");
	result.SetValue("GripperComplianceSlope", "128");
	result.SetValue("GripperPositionResolution", "0.29296875");
	result.SetValue("GripperPositionOffset", "150");
	result.SetValue("GripperMaximumPower", "100");
	result.SetValue("GripperMaximumVelocity", "90");
	result.SetValue("GripperMinimumPositionLimit", "-45");
	result.SetValue("GripperMaximumPositionLimit", "90");
	result.SetValue("GripperMaximumLoad", "30");


	return result;
}

int main()
{
	DynamixelGripper manipulator;


	if (manipulator.Initialize(CreateProperty()) != API_SUCCESS)
	{
		PrintMessage("Initialize Error\r\n");
		return 0;
	}

	if (manipulator.Enable() != API_SUCCESS)
	{
		PrintMessage("Enable Error\r\n");
		return 0;
	}

	manipulator.RunHoming();

	Sleep(1000);


	double degree = 0.0;
	double differnce = 10.0;
	for(;;)
	{
		/*
		if (kbhit())
		{
			char key = getch();

			if (key == 27)
			{
				break;
			}
			else if(key == 'a')
			{
				manipulator.StopGripping();
			}
			else if(key == 's')
			{
				manipulator.StartGripping();
			}
		}

		if (degree > 20 || degree < -20.0)
		{
			differnce *= -1;
		}

		degree += differnce;

		if (manipulator.IsGripped())
		{
			static int count = 0;
			std::cout << ++count << "잡힘\r\n";
		}
		Sleep(100);
		continue;
		*/

		size_t index;
		double degree;
		std::cin.clear();
		std::cout << "Index : "; 
		std::cin >> index;
		std::cout << "Degree : ";
		std::cin >> degree;

		vector<double> position;
		if (manipulator.GetPosition(position) != API_SUCCESS)
		{
			continue;
		}
		
		if (index >= position.size())
		{
			std::cout << "Index Error";
			continue;
		}

		static double axis[6] = {1.0, -1.0, -1.0, 1.0, -1.0, 1.0};

		position[index] = axis[index] * degree;
		vector<unsigned long> time(position.size());

		if (manipulator.SetPosition(position, time) != API_SUCCESS)
		{
			PrintMessage("SetPosition Error\r\n");
		}		
	}

	return 0;	
}

#endif