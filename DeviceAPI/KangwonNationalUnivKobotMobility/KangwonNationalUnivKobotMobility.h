#pragma once

#include "WheelController.h"
#include "HRC01.h"

typedef struct {
	double wheelDiameter;
	double axleDistance;
	double linePerRevolution;
	double reductionRatio;
	double maximumVelocity;
	double acceleration;

	double radToAngle;
	double angleToRad;
	double radToPulse;
	double pulseToRad;

	unsigned short kp[2];
	unsigned short ki[2];
	unsigned short kd[2];
	unsigned short uiSum[2];
} KangwonNationalUnivKobotMobilityProfile;

class KangwonNationalUnivKobotMobility : public WheelController
{
public:
	KangwonNationalUnivKobotMobility(void);
	virtual ~KangwonNationalUnivKobotMobility(void);

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int SetPosition(ObjectLocation position);
	int GetPosition(ObjectLocation &position);
	int GetOdometery(vector<long> &odometery);
	int DriveWheel(double linearVelocity, double angularVelocity);
	int MoveWheel(double distance, double linearVelocity);
	int RotateWheel(double angle, double angularVelocity);
	int StopWheel(void);
	int IsWheelRunning(bool &isWheelRunning);

	int OnExecute(void);

protected:
	HRC01 *_hcr01;
	Property _parameter;
	ObjectLocation _position;

	KangwonNationalUnivKobotMobilityProfile _profile;

private:
	double _prevLeftWheelAngle;
	double _prevRightWheelAngle;
	int SetupHrc01(void);
};

/*
	<execution_semantics>
		<type>periodic</type>
		<period>100</period>
		<priority>5</priority>
		<instance_creation>multiple</instance_creation>
		<lifecycle_type>start</lifecycle_type>
	</execution_semantics>
	<properties>
		<property name="ApiName" type="string">KangwonNationalUnivKobotMobility</property>
		<property name="WheelDiameter" type="double">0.1</property>
		<property name="AxleDistance" type="double">0.3</property>
		<property name="LinePerRevolution" type="double">104.0</property>
		<property name="ReductionRatio" type="double">51.0</property>
		<property name="MaximumVelocity" type="double">5200.0</property>
		<property name="Acceleration" type="double">10400.0</property>
		<property name="Kp0" type="unsigned short">32</property>
		<property name="Ki0" type="unsigned short">7</property>
		<property name="Kd0" type="unsigned short">600</property>
		<property name="UiSum0" type="unsigned short">1400</property>
		<property name="Kp1" type="unsigned short">32</property>
		<property name="Ki1" type="unsigned short">7</property>
		<property name="Kd1" type="unsigned short">600</property>
		<property name="UiSum1" type="unsigned short">1400</property>
	</properties>
*/