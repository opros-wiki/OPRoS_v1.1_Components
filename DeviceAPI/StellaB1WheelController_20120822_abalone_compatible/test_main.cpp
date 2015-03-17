#if defined(WIN32)
#include <conio.h>
#include <windows.h>
#else
#include <pthread.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <device/OprosPrintMessage.h>
#include <device/OprosTimer.h>

#include "debug_macro.h"
#include "StellaB1Tester.h"
#include "Communication/SerialCommunication.h"
#include "StellaB1WheelController.h"

#ifndef WIN32
int getch()
{
	int ch;
	struct termios buf;
	struct termios save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}
#endif

Property GetProperty()
{
	Property result;

#if defined(WIN32)
	result.SetValue("PortName", "COM3");
#else
	result.SetValue("PortName", "/dev/ttyUSB0");
#endif
	result.SetValue("TimeOut", "1");
	result.SetValue("BaudRate", "115200");
	result.SetValue("DataBits", "8");
	result.SetValue("StopBits", "0");
	result.SetValue("Parity", "0");
	result.SetValue("FlowControl", "0");

	return result;
}

#if defined(WIN32)
DWORD WINAPI ThreadHandler(LPVOID lParam)
#else
void* ThreadHandler(void* lParam)
#endif
{
	StellaB1WheelController* wheel = (StellaB1WheelController*)lParam;

	for (;;)
	{
		wheel->OnExecute();
		OprosSleep(10);
	}

	return 0;
}

bool TestStellaB1()
{
	PrintMessage("StellB1 Test\r\n");
	Uart* uart = new SerialCommunication();
	if(uart->Initialize(GetProperty()) != API_SUCCESS)
	{
		PrintMessage("Can not initialize uart\r\n");
		return false;
	}
	if (uart->Enable() != API_SUCCESS)
	{
		PrintMessage("Can not enalbe uart\r\n");
		return false;
	}

	StellaB1 stellB1(uart);
	StellaB1Tester stellaB1Tester(&stellB1);

	if (stellaB1Tester.TestAll() == false)
	{
		delete uart;
		uart = NULL;
		return false;
	}

	delete uart;
	uart = NULL;
	return true;
}

bool TestStellaB1WheelController()
{
	PrintMessage("StellaB1WheelController Test\r\n");
	StellaB1WheelController wheel;

	if(wheel.Initialize(GetProperty()) != API_SUCCESS)
	{
		PrintMessage("Can not initialize StellaB1WheelController\r\n");
		return false;
	}
	if(wheel.Enable() != API_SUCCESS)
	{
		PrintMessage("Can not initialize StellaB1WheelController\r\n");
		return false;
	}

	
#if defined(WIN32)
	CreateThread(NULL, 0, &ThreadHandler, &wheel, 0, NULL);
#else
	pthread_t receiveThread;
	pthread_create(&receiveThread, NULL, &ThreadHandler, &wheel);
#endif

	const double MAX_LINEAR_SPEED = 0.2;
	const double MAX_ANGULAR_SPEED = 45;
	const double ROTATION_DEGREE = 45;

	for (;;)
	{
		PrintMessage("Please input command\r\n");
		char command = getch();

		switch(command)
		{
		case 'w':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.DriveWheel(MAX_LINEAR_SPEED, 0.0);
			break;
		case 'q':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.DriveWheel(MAX_LINEAR_SPEED, MAX_ANGULAR_SPEED);
			break;
		case 'x':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.DriveWheel(-MAX_LINEAR_SPEED, 0.0);
			break;
		case 'e':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.DriveWheel(MAX_LINEAR_SPEED, -MAX_ANGULAR_SPEED);
			break;
		case 's':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.StopWheel();
			break;
		case 'a':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.RotateWheel(ROTATION_DEGREE, MAX_ANGULAR_SPEED);
			break;
		case 'd':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.RotateWheel(-ROTATION_DEGREE, MAX_ANGULAR_SPEED);
			break;
		case 'z':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.DriveWheel(-MAX_LINEAR_SPEED, -MAX_ANGULAR_SPEED);
			break;
		case 'c':
			PrintMessage("DriveWheel(%lf, 0.0)\r\n", MAX_LINEAR_SPEED);
			wheel.DriveWheel(-MAX_LINEAR_SPEED, MAX_ANGULAR_SPEED);
			break;
		case 0x03:
			PrintMessage("Exit\r\n");
			wheel.Disable();
			wheel.Finalize();
			return true;
		default:
			break;
		}
	}

	return true;
}

int main()
{
	/*
	if(TestStellaB1() == false)
		return 0;
	*/

	TestStellaB1WheelController();

	return 0;
}
