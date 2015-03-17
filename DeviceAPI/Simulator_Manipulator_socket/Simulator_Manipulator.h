#pragma once

#include "Manipulator.h"
#include "ClientSocket.h"

class Simulator_Manipulator : public Manipulator
{
public :
	Simulator_Manipulator (void);
	virtual ~Simulator_Manipulator (void);

int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int Stop(void);
	int RunHoming(void);
	int SetPosition(vector<double> position, vector<unsigned long> time);
	int GetPosition(vector<double> &position);

private :

	Property _parameter;

	ClientSocket _clientSocket;
	string _ip;
	int _port;

	string _robotName;
	vector<string> _devicesName;
	unsigned int _numberOfJoints;
	struct data _devicesInfo;

};