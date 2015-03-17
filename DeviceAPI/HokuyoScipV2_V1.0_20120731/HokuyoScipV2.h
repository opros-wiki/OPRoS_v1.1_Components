#pragma once

#include "LaserScanner.h"
#include "UART.h"

class HokuyoScipV2 : public LaserScanner
{
public:
	HokuyoScipV2(void);
	virtual ~HokuyoScipV2(void);

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int GetSensorValue(vector<double> &sensorValue);

private:
	Uart *uart;

#if defined(WIN32)
	HMODULE hOprosAPI;
#else
	void *hOprosAPI;
#endif

	int maximumStep;
	int startStep;
	int endStep;
	int deltaStep;
	int scanStepSize;
	vector<int> scannedData;

	int InitializeUART(Property parameter);
	int FinalizeUART(void);

	int Hokuyo_SCIPV2_0_SendPack(unsigned char *data, int size);
	int Hokuyo_SCIPV2_0_RecvPack(unsigned char *data, int size);
	int Hokuyo_SCIPV2_0_CheckSum(unsigned char *data, int size);
	int Hokuyo_SCIPV2_0_ParsingData(unsigned char *data, int size);
	int Hokuyo_SCIPV2_0_ParsingTimeStamp(unsigned char *data);
	int Hokuyo_SCIPV2_0_ChangeVer2_0(void);
	int Hokuyo_SCIPV2_0_Reset(void);
	int Hokuyo_SCIPV2_0_SetBaudRate(int baudRate);
	int Hokuyo_SCIPV2_0_Enable(unsigned char *status);
	int Hokuyo_SCIPV2_0_Disable(void);
	int Hokuyo_SCIPV2_0_MeasureAndGetData(void);
	int Hokuyo_SCIPV2_0_GetLastData(void);
};
