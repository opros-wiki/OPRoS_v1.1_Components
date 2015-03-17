#include "StellaB1WheelController.h"

#include <device/OprosPrintMessage.h>
#include <sstream>

#include "debug_macro.h"
#include "Communication/SerialCommunication.h"

//Define Required Profile Name List
#define UART_PORT_NAME "PortName"
#define UART_TIME_OUT "TimeOut"
#define UART_BAUD_RATE "BaudRate"
#define UART_DATA_BITS "DataBits"
#define UART_STOP_BITS "StopBits"
#define UART_PARITY "Parity"
#define UART_FLOW_CONTROL "FlowControl"

#define WHEEL_DIAMETER "WheelDiameter"
#define AXLE_DISTANCE "AxleDistance"
#define REDUCTION_RATIO "ReductionRatio"
#define ENCODER_CPR "EncoderCPR"
#define MAXIMUM_VELOCITY "MaximumVelocity"

#define ACCELERATION "Acceleration"
#define DEACCELERATION "Deacceleration"

#define RIGHT_WHEEL_ANGLE_SCALE_FACTOR "RightWheelAngleScaleFactor"
#define LEFT_WHEEL_ANGLE_SCALE_FACTOR "LeftWheelAngleScaleFactor"

#define RIGHT_WHEEL_DISTANCE_SCALE_FACTOR "RightWheelDistanceScaleFactor"
#define LEFT_WHEEL_DISTANCE_SCALE_FACTOR "LeftWheelDistanceScaleFactor"

#define VELOCITY_CONTROL_P "VelocityControlP"
#define VELOCITY_CONTROL_I "VelocityControlI"
#define VELOCITY_CONTROL_D "VelocityControlD"
#define VELOCITY_CONTROL_IL "VelocityControlIL"

#define POSITION_CONTROL_P "PositionControlP"
#define POSITION_CONTROL_I "PositionControlI"
#define POSITION_CONTROL_D "PositionControlD"
#define POSITION_CONTROL_IL "PositionControlIL"
//End of Define Required Profile Name List

StellaB1WheelController::StellaB1WheelController() : m_uart(NULL), m_stellaB1(NULL)
{
}

StellaB1WheelController::~StellaB1WheelController()
{
	Finalize();
}

int StellaB1WheelController::Initialize( Property parameter )
{
	if (_status != DEVICE_CREATED)
	{
		PrintMessage(DEBUG_MESSAGE("Already initialized").c_str());
		return API_ERROR;
	}
	
	//////////////////////////////////////////////////////////////////////////

	m_uart = new SerialCommunication;

	if (SetStellaB1Profile(parameter) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Can not set profile").c_str());
		return API_ERROR;
	}	

	if (m_uart->Initialize(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not initialize uart").c_str());
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	_status = DEVICE_READY;
	return API_SUCCESS;
}

int StellaB1WheelController::Finalize()
{
	if (_status == DEVICE_ACTIVE || _status == DEVICE_ERROR)
		Disable();

	//////////////////////////////////////////////////////////////////////////
	
	if(m_uart != NULL)
	{
		m_uart->Finalize();
		delete m_uart;
		m_uart = NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	_status = DEVICE_CREATED;
	return API_SUCCESS;
}

int StellaB1WheelController::Enable()
{
	if (_status == DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Already enabled").c_str());
		return API_SUCCESS;
	}
	else if(_status != DEVICE_READY)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////

	if (m_uart->Enable() != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not enable uart").c_str());
		return API_ERROR;
	}

	m_stellaB1.RegisterUart(m_uart);

	m_previousWheelSpeed.rightSpeed = 0;
	m_previousWheelSpeed.leftSpeed = 0;
	m_previousWheelSpeed.time = 0;

	m_wheelPosition.x = 0.0;
	m_wheelPosition.y = 0.0;
	m_wheelPosition.theta = 0.0;
	
	StellaB1::ErrorCode errorCode;
	if ((errorCode = m_stellaB1.ControlInit()) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	//Wheel Diameter
	if ((errorCode = m_stellaB1.SetWheelRadius(m_profile.wheelDiameter / 2.0f)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	//Axle Distance
	if ((errorCode = m_stellaB1.SetAxleWidth(m_profile.axleDistance)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}
	
	//Acceleration
	if ((errorCode = m_stellaB1.SetVelocityAcceleration((float)DEG2RAD(m_profile.acceleration), (float)DEG2RAD(m_profile.acceleration))) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	//Deacceleration
	if ((errorCode = m_stellaB1.SetVelocityDeacceleration((float)DEG2RAD(m_profile.deacceleration), (float)DEG2RAD(m_profile.deacceleration))) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	//Angle Scale Factor
	if ((errorCode = m_stellaB1.SetScaleFactorAngle(m_profile.rightWheelAngleScaleFactor, m_profile.leftWheelAngleScaleFactor)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	//Distacne Scale Factor
	if ((errorCode = m_stellaB1.SetScaleFactorDistance(m_profile.rightWheelDistanceScaleFactor, m_profile.leftWheelDistanceSacleFactor)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	//Velocity Control
	if ((errorCode = m_stellaB1.SetControlVelocityParameterRightWheel(m_profile.velocityControlParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}

	if ((errorCode = m_stellaB1.SetControlVelocityParameterLeftWheel(m_profile.velocityControlParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}
	
	//Position Control
	if ((errorCode = m_stellaB1.SetControlPositionParameterRightWheel(m_profile.positionControlParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}
	
	if ((errorCode = m_stellaB1.SetControlPositionParameterLeftWheel(m_profile.positionControlParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return API_ERROR;
	}	

	//////////////////////////////////////////////////////////////////////////

	_status = DEVICE_ACTIVE;
	return API_SUCCESS;
}

int StellaB1WheelController::Disable()
{
	m_rwLockStatus.WriterLock();
	if (_status == DEVICE_READY)
	{
		PrintMessage(DEBUG_MESSAGE("Already disabled").c_str());
		m_rwLockStatus.WriterUnlock();
		return API_SUCCESS;
	}
	else if(_status != DEVICE_ACTIVE && _status != DEVICE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.WriterUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////

	StellaB1::ErrorCode errorCode;
	if((errorCode = m_stellaB1.ControlStop(StellaB1::CSTOP3)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.WriterUnlock();
		return API_ERROR;
	}

	if (m_uart->Disable() != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not disable uart").c_str());
		m_rwLockStatus.WriterUnlock();
		return API_ERROR;
	}

	m_stellaB1.UnregisterUart();

	//////////////////////////////////////////////////////////////////////////
	_status = DEVICE_READY;
	m_rwLockStatus.WriterUnlock();
	return API_SUCCESS;
}

int StellaB1WheelController::SetParameter( Property parameter )
{
	if (_status == DEVICE_CREATED)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	if (Disable() != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not disable").c_str());
		_status = DEVICE_ERROR;
		return API_ERROR;
	}
	
	//////////////////////////////////////////////////////////////////////////

	if (SetStellaB1Profile(parameter) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Can not set profile").c_str());
		return API_ERROR;
	}	

	if (m_uart->SetParameter(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not set uart parameter").c_str());
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////

	if (Enable() != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not enable").c_str());
		_status = DEVICE_ERROR;
		return API_ERROR;
	}

	return API_SUCCESS;
}

int StellaB1WheelController::GetParameter( Property &parameter )
{
	if (_status == DEVICE_CREATED)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////

	if(m_uart->GetParameter(parameter) != API_SUCCESS)
	{
		PrintMessage(DEBUG_MESSAGE("Can not get uart parameter").c_str());
		return API_ERROR;
	}
	
	std::ostringstream stringConverter;
	
	//Wheel Diameter
	stringConverter << m_profile.wheelDiameter;
	parameter.SetValue(WHEEL_DIAMETER, stringConverter.str());
	stringConverter.str("");

	//Axle Distance
	stringConverter << m_profile.axleDistance;
	parameter.SetValue(AXLE_DISTANCE, stringConverter.str());
	stringConverter.str("");

	//Maximum Velocity
	stringConverter << m_profile.maximumVelocity;
	parameter.SetValue(MAXIMUM_VELOCITY, stringConverter.str());
	stringConverter.str("");
	
	//Acceleration
	stringConverter << m_profile.acceleration;
	parameter.SetValue(ACCELERATION, stringConverter.str());
	stringConverter.str("");

	//Deacceleration
	stringConverter << m_profile.deacceleration;
	parameter.SetValue(DEACCELERATION, stringConverter.str());
	stringConverter.str("");

	//Angle Scale Factor
	stringConverter << m_profile.rightWheelAngleScaleFactor;
	parameter.SetValue(RIGHT_WHEEL_ANGLE_SCALE_FACTOR, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.leftWheelAngleScaleFactor;
	parameter.SetValue(LEFT_WHEEL_ANGLE_SCALE_FACTOR, stringConverter.str());
	stringConverter.str("");

	//Distacne Scale Factor
	stringConverter << m_profile.rightWheelDistanceScaleFactor;
	parameter.SetValue(RIGHT_WHEEL_DISTANCE_SCALE_FACTOR, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.leftWheelDistanceSacleFactor;
	parameter.SetValue(LEFT_WHEEL_DISTANCE_SCALE_FACTOR, stringConverter.str());
	stringConverter.str("");

	//Velocity Control
	stringConverter << m_profile.velocityControlParameter.gainP;
	parameter.SetValue(VELOCITY_CONTROL_P, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.velocityControlParameter.gainI;
	parameter.SetValue(VELOCITY_CONTROL_I, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.velocityControlParameter.gainD;
	parameter.SetValue(VELOCITY_CONTROL_D, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.velocityControlParameter.gainIL;
	parameter.SetValue(VELOCITY_CONTROL_IL, stringConverter.str());
	stringConverter.str("");

	//Position Control
	stringConverter << m_profile.positionControlParameter.gainP;
	parameter.SetValue(POSITION_CONTROL_P, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.positionControlParameter.gainI;
	parameter.SetValue(POSITION_CONTROL_I, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.positionControlParameter.gainD;
	parameter.SetValue(POSITION_CONTROL_D, stringConverter.str());
	stringConverter.str("");

	stringConverter << m_profile.positionControlParameter.gainIL;
	parameter.SetValue(POSITION_CONTROL_IL, stringConverter.str());
	stringConverter.str("");

	//////////////////////////////////////////////////////////////////////////

	return API_SUCCESS;
}

int StellaB1WheelController::OnExecute()
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	if (ProcessOdometricLocalization() == false)
	{
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}
	
	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}

bool StellaB1WheelController::ProcessOdometricLocalization()
{
	WheelSpeed wheelSpeed;

	StellaB1::ErrorCode errorCode;
	if ((errorCode = m_stellaB1.GetVelocity(wheelSpeed.rightSpeed, wheelSpeed.leftSpeed)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	wheelSpeed.time = m_timer.GetTimeTick();
	wheelSpeed.rightSpeed = wheelSpeed.rightSpeed * m_profile.reductionRatio * (m_profile.wheelDiameter / 2.0f);
	wheelSpeed.leftSpeed  = wheelSpeed.leftSpeed * m_profile.reductionRatio * (m_profile.wheelDiameter / 2.0f);

	if(m_previousWheelSpeed.time != 0)
	{
		double periode = (wheelSpeed.time - m_previousWheelSpeed.time) / 1000.0;
		double linearVelocity = (m_previousWheelSpeed.leftSpeed + m_previousWheelSpeed.rightSpeed) / 2.0;
		double angularVelocity = (m_previousWheelSpeed.rightSpeed - m_previousWheelSpeed.leftSpeed) / m_profile.axleDistance;

		m_mutexWheelPosition.Lock();
		if (abs(angularVelocity) < 0.001)
		{
			//2nd order Runge-Kutta integration
			// x(t+1) = x(t) + v(t) * Periode * cos(theta(t) + w(t) * Periode / 2)
			// y(t+1) = y(t) + v(t) * Periode * sin(theta(t) + w(t) * Periode / 2)
			// theta(t+1) = theta(t) + w(t) * Periode
			double theta = m_wheelPosition.theta + angularVelocity * periode / 2.0;
			double moveDistance = linearVelocity * periode;

			m_wheelPosition.x += moveDistance * cos(theta);
			m_wheelPosition.y += moveDistance * sin(theta);
			m_wheelPosition.theta += angularVelocity * periode;
		}
		else
		{
			// exact integration
			// x(t+1) = x(t) + v(t) / w(t) * (sin(theta(t+1)) - sin(theta(t))))
			// y(t+1) = y(t) - v(t) / w(t) * (cos(theta(t+1)) - cos(theta(t))))
			// theta(t+1) = theta(t) + w(t) * Periode
			double theta = m_wheelPosition.theta;
			double theta2 = m_wheelPosition.theta + angularVelocity * periode;
			double linearVelocityPerAngularVelocity = linearVelocity / angularVelocity;

			m_wheelPosition.x += linearVelocityPerAngularVelocity * (sin(theta2) - sin(theta));
			m_wheelPosition.y -= linearVelocityPerAngularVelocity * (cos(theta2) - cos(theta));
			m_wheelPosition.theta = theta2;
		}
		m_mutexWheelPosition.Unlock();
	}

	m_previousWheelSpeed = wheelSpeed;
	return true;
}

int StellaB1WheelController::SetPosition( ObjectLocation position )
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	m_mutexWheelPosition.Lock();

	m_wheelPosition = position;
	m_wheelPosition.theta = DEG2RAD(position.theta);

	m_mutexWheelPosition.Unlock();

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}

int StellaB1WheelController::GetPosition( ObjectLocation& position )
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	m_mutexWheelPosition.Lock();

	position = m_wheelPosition;
	position.theta = RAD2DEG(m_wheelPosition.theta);

	m_mutexWheelPosition.Unlock();

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}

int StellaB1WheelController::GetOdometery( vector<long>& odometery )
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	float rightWheelPosition, leftWheelPosition;
	StellaB1::ErrorCode errorCode;
	if ((errorCode = m_stellaB1.GetPosition(rightWheelPosition, leftWheelPosition)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}
	
	long rightWheelOdometry = INTEGER(rightWheelPosition * m_profile.encoderCPR / (M_PI * m_profile.wheelDiameter) / m_profile.reductionRatio);
	long leftWheelOdometry = INTEGER(leftWheelPosition * m_profile.encoderCPR / (M_PI * m_profile.wheelDiameter) / m_profile.reductionRatio);
	
	odometery.resize(2);
	odometery[0] = rightWheelOdometry;
	odometery[1] = leftWheelOdometry;
	
	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();	
	return API_SUCCESS;
}

int StellaB1WheelController::DriveWheel( double linearVelocity, double angularVelocity )
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	// IWLV : ȸ�� �� ���� ���� ���ӵ�, OWLV : ȸ�� �� �ٱ��� ���� ���ӵ�
	// LV : �κ��� ���ӵ�, AV : �κ��� ���ӵ�, AD : �� ���� ���� �� �Ÿ�
	// R : ȸ�� �� ���� ���� ȸ�� �߽��� �Ÿ�
	// LV = (IWLV + OWLV) / 2	(1)
	// IWLV = AV * R			(2)
	// OWLV = AV * (R + AD)		(3)
	// �� (1), (2), (3)�� ���� R�� ���� �ϸ�
	// R = (2LV - AV * AD) / 2AV
	// IWLV = AV * (2LV - AV * AD) / 2AV = LV - (AV * AD)/2
	// OWLV = AV * ((2LV - AV * AD) / 2AV + AD) = LV +  (AV * AD)/2
	linearVelocity = bound(linearVelocity, (double)-m_profile.maximumVelocity, (double)m_profile.maximumVelocity);

	double rightWheelSpeed = linearVelocity + m_profile.axleDistance * DEG2RAD(angularVelocity) / 2.0;
	double leftWheelSpeed = linearVelocity - m_profile.axleDistance * DEG2RAD(angularVelocity) / 2.0;
		
	// m/s �� rad/s�� ��ȯ
	rightWheelSpeed = rightWheelSpeed / (m_profile.wheelDiameter / 2.0) / m_profile.reductionRatio;
	leftWheelSpeed = leftWheelSpeed / (m_profile.wheelDiameter / 2.0) / m_profile.reductionRatio;

	StellaB1::ErrorCode errorCode;
	if((errorCode = m_stellaB1.ControlVelocity((float)rightWheelSpeed, (float)leftWheelSpeed)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}

int StellaB1WheelController::MoveWheel( double distance, double linearVelocity )
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	linearVelocity = bound(linearVelocity, (double)-m_profile.maximumVelocity, (double)m_profile.maximumVelocity);
	
	double meter = Sign(linearVelocity) * distance;
	int time = INTEGER(abs(distance / linearVelocity));

	if(time == 0)
	{
		PrintMessage(DEBUG_MESSAGE("Velocity is so fast").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	StellaB1::ErrorCode errorCode;
	if((errorCode = m_stellaB1.ControlPositionDistance((float)meter, time)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}

int StellaB1WheelController::RotateWheel( double angle, double angularVelocity )
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////

	double degree = Sign(angularVelocity) * angle;
	int time = INTEGER(abs(angle / angularVelocity));

	if(time == 0)
	{
		PrintMessage(DEBUG_MESSAGE("Velocity is so fast").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	StellaB1::ErrorCode errorCode;
	if((errorCode = m_stellaB1.ControlPositionAngle((float)degree, time)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();	
	return API_SUCCESS;
}

int StellaB1WheelController::StopWheel()
{
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	StellaB1::ErrorCode errorCode;
	if((errorCode = m_stellaB1.ControlStop(StellaB1::CSTOP3)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}

int StellaB1WheelController::IsWheelRunning( bool& isWheelRunning )
{	
	m_rwLockStatus.ReaderLock();
	if (_status != DEVICE_ACTIVE)
	{
		PrintMessage(DEBUG_MESSAGE("Precondition not met").c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	StellaB1::State state;

	StellaB1::ErrorCode errorCode;
	if((errorCode = m_stellaB1.GetState(state)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		m_rwLockStatus.ReaderUnlock();
		return API_ERROR;
	}

	if(state == StellaB1::RUNNING_WHEEL_STATE)
	{
		isWheelRunning = true;
	}
	else
	{
		isWheelRunning = false;
	}

	//////////////////////////////////////////////////////////////////////////
	m_rwLockStatus.ReaderUnlock();
	return API_SUCCESS;
}


bool StellaB1WheelController::SetStellaB1Profile(Property& parameter )
{
	PrintMessage("--------------------------------\r\n");
	PrintMessage("StellaB1WheelController Profiles\r\n");
	PrintMessage("--------------------------------\r\n");

	//Uart Port Name
	PrintMessage("%s : ", UART_PORT_NAME);
	if (parameter.FindName(UART_PORT_NAME) == false)
		return false;
	PrintMessage("%s\r\n", parameter.GetValue(UART_PORT_NAME).c_str());

	//Uart Time Out
	parameter.SetValue(UART_TIME_OUT, "1");
	PrintMessage("%s : %s\r\n", UART_TIME_OUT, parameter.GetValue(UART_TIME_OUT).c_str());

	//Uart Buad Rate
	PrintMessage("%s : ", UART_BAUD_RATE);
	if (parameter.FindName(UART_BAUD_RATE) == false)
		parameter.SetValue(UART_BAUD_RATE, "115200");
	PrintMessage("%s\r\n", parameter.GetValue(UART_BAUD_RATE).c_str());

	//Uart Data Bits
	PrintMessage("%s : ", UART_DATA_BITS);
	if (parameter.FindName(UART_DATA_BITS) == false)
		parameter.SetValue(UART_DATA_BITS, "8");
	PrintMessage("%s\r\n", parameter.GetValue(UART_DATA_BITS).c_str());

	//Uart Stop Bits
	PrintMessage("%s : ", UART_STOP_BITS);
	if (parameter.FindName(UART_STOP_BITS) == false)
		parameter.SetValue(UART_STOP_BITS, "0");
	PrintMessage("%s\r\n", parameter.GetValue(UART_STOP_BITS).c_str());

	//Uarts Parity
	PrintMessage("%s : ", UART_PARITY);
	if (parameter.FindName(UART_PARITY) == false)
		parameter.SetValue(UART_PARITY, "0");
	PrintMessage("%s\r\n", parameter.GetValue(UART_PARITY).c_str());

	//Uart Flow Control
	PrintMessage("%s : ", UART_FLOW_CONTROL);
	if (parameter.FindName(UART_FLOW_CONTROL) == false)
		parameter.SetValue(UART_FLOW_CONTROL, "0");
	PrintMessage("%s\r\n", parameter.GetValue(UART_FLOW_CONTROL).c_str());
	
	//Wheel Diameter
	if (parameter.FindName(WHEEL_DIAMETER) == true)
		m_profile.wheelDiameter = (float)atof(parameter.GetValue(WHEEL_DIAMETER).c_str());
	PrintMessage("%s : %f\r\n", WHEEL_DIAMETER, m_profile.wheelDiameter);

	//Axle Distance
	if (parameter.FindName(AXLE_DISTANCE) == true)
		m_profile.axleDistance = (float)atof(parameter.GetValue(AXLE_DISTANCE).c_str());
	PrintMessage("%s : %f\r\n", AXLE_DISTANCE, m_profile.axleDistance);

	//Reduction Ratio
	if (parameter.FindName(REDUCTION_RATIO) == true)
		m_profile.reductionRatio = (float)atof(parameter.GetValue(REDUCTION_RATIO).c_str());
	PrintMessage("%s : %f\r\n", REDUCTION_RATIO, m_profile.reductionRatio);

	//Encoder CPR
	if (parameter.FindName(ENCODER_CPR) == true)
		m_profile.encoderCPR = (float)atof(parameter.GetValue(ENCODER_CPR).c_str());
	PrintMessage("%s : %f\r\n", ENCODER_CPR, m_profile.encoderCPR);

	//Maximum Velocity
	if (parameter.FindName(MAXIMUM_VELOCITY) == true)
		m_profile.maximumVelocity = (float)atof(parameter.GetValue(MAXIMUM_VELOCITY).c_str());
	PrintMessage("%s : %f\r\n", MAXIMUM_VELOCITY, m_profile.maximumVelocity);

	//Acceleration
	if (parameter.FindName(ACCELERATION) == true)
		m_profile.acceleration = (float)atof(parameter.GetValue(ACCELERATION).c_str());
	PrintMessage("%s : %f\r\n", ACCELERATION, m_profile.acceleration);

	//Deacceleration
	if (parameter.FindName(DEACCELERATION) == true)
		m_profile.deacceleration = (float)atof(parameter.GetValue(DEACCELERATION).c_str());
	PrintMessage("%s : %f\r\n", DEACCELERATION, m_profile.deacceleration);

	//Angle Scale Factor
	if (parameter.FindName(RIGHT_WHEEL_ANGLE_SCALE_FACTOR) == true)
		m_profile.rightWheelAngleScaleFactor = (float)atof(parameter.GetValue(RIGHT_WHEEL_ANGLE_SCALE_FACTOR).c_str());
	PrintMessage("%s : %f\r\n", RIGHT_WHEEL_ANGLE_SCALE_FACTOR, m_profile.rightWheelAngleScaleFactor);

	if (parameter.FindName(LEFT_WHEEL_ANGLE_SCALE_FACTOR) == true)
		m_profile.leftWheelAngleScaleFactor = (float)atof(parameter.GetValue(LEFT_WHEEL_ANGLE_SCALE_FACTOR).c_str());
	PrintMessage("%s : %f\r\n", LEFT_WHEEL_ANGLE_SCALE_FACTOR, m_profile.leftWheelAngleScaleFactor);

	//Distacne Scale Factor
	if (parameter.FindName(RIGHT_WHEEL_DISTANCE_SCALE_FACTOR) == true)
		m_profile.rightWheelDistanceScaleFactor = (float)atof(parameter.GetValue(RIGHT_WHEEL_DISTANCE_SCALE_FACTOR).c_str());
	PrintMessage("%s : %f\r\n", RIGHT_WHEEL_DISTANCE_SCALE_FACTOR, m_profile.rightWheelDistanceScaleFactor);

	if (parameter.FindName(LEFT_WHEEL_DISTANCE_SCALE_FACTOR) == true)
		m_profile.leftWheelDistanceSacleFactor = (float)atof(parameter.GetValue(LEFT_WHEEL_DISTANCE_SCALE_FACTOR).c_str());
	PrintMessage("%s : %f\r\n", LEFT_WHEEL_DISTANCE_SCALE_FACTOR, m_profile.leftWheelDistanceSacleFactor);

	//Velocity Control
	if (parameter.FindName(VELOCITY_CONTROL_P) == true)
		m_profile.velocityControlParameter.gainP = (float)atof(parameter.GetValue(VELOCITY_CONTROL_P).c_str());
	PrintMessage("%s : %f\r\n", VELOCITY_CONTROL_P, m_profile.velocityControlParameter.gainP);

	if (parameter.FindName(VELOCITY_CONTROL_I) == true)
		m_profile.velocityControlParameter.gainI = (float)atof(parameter.GetValue(VELOCITY_CONTROL_I).c_str());
	PrintMessage("%s : %f\r\n", VELOCITY_CONTROL_I, m_profile.velocityControlParameter.gainI);

	if (parameter.FindName(VELOCITY_CONTROL_D) == true)
		m_profile.velocityControlParameter.gainD = (float)atof(parameter.GetValue(VELOCITY_CONTROL_D).c_str());
	PrintMessage("%s : %f\r\n", VELOCITY_CONTROL_D, m_profile.velocityControlParameter.gainD);

	if (parameter.FindName(VELOCITY_CONTROL_IL) == true)
		m_profile.velocityControlParameter.gainIL = (float)atof(parameter.GetValue(VELOCITY_CONTROL_IL).c_str());
	PrintMessage("%s : %f\r\n", VELOCITY_CONTROL_IL, m_profile.velocityControlParameter.gainIL);

	//Position Control
	if (parameter.FindName(POSITION_CONTROL_P) == true)
		m_profile.positionControlParameter.gainP = (float)atof(parameter.GetValue(POSITION_CONTROL_P).c_str());
	PrintMessage("%s : %f\r\n", POSITION_CONTROL_P, m_profile.positionControlParameter.gainP);

	if (parameter.FindName(POSITION_CONTROL_I) == true)
		m_profile.positionControlParameter.gainI = (float)atof(parameter.GetValue(POSITION_CONTROL_I).c_str());
	PrintMessage("%s : %f\r\n", POSITION_CONTROL_I, m_profile.positionControlParameter.gainI);

	if (parameter.FindName(POSITION_CONTROL_D) == true)
		m_profile.positionControlParameter.gainD = (float)atof(parameter.GetValue(POSITION_CONTROL_D).c_str());
	PrintMessage("%s : %f\r\n", POSITION_CONTROL_D, m_profile.positionControlParameter.gainD);

	if (parameter.FindName(POSITION_CONTROL_IL) == true)
		m_profile.positionControlParameter.gainIL = (float)atof(parameter.GetValue(POSITION_CONTROL_IL).c_str());
	PrintMessage("%s : %f\r\n", POSITION_CONTROL_IL, m_profile.positionControlParameter.gainIL);

	PrintMessage("--------------------------------\r\n");
	
	return true;
}

#ifdef WIN32
extern "C"
{
	__declspec(dllexport) OprosApi* GetAPI();
}

OprosApi* GetAPI()
{
	return new StellaB1WheelController();
}

#else
extern "C"
{
	OprosApi* GetAPI();
}

OprosApi* GetAPI()
{
	return new StellaB1WheelController();
}

#endif

//Undefine Required Profile Name List
#undef UART_PORT_NAME
#undef UART_TIME_OUT
#undef UART_BAUD_RATE
#undef UART_DATA_BITS
#undef UART_STOP_BITS
#undef UART_PARITY 
#undef UART_FLOW_CONTROL 

#undef WHEEL_DIAMETER
#undef AXLE_DISTANCE 
#undef  REDUCTION_RATIO
#undef  ENCODER_CPR
#undef MAXIMUM_VELOCITY 

#undef ACCELERATION 
#undef DEACCELERATION 

#undef RIGHT_WHEEL_ANGLE_SCALE_FACTOR 
#undef LEFT_WHEEL_ANGLE_SCALE_FACTOR 

#undef RIGHT_WHEEL_DISTANCE_SCALE_FACTOR 
#undef LEFT_WHEEL_DISTANCE_SCALE_FACTOR 

#undef VELOCITY_CONTROL_P 
#undef VELOCITY_CONTROL_I 
#undef VELOCITY_CONTROL_D 
#undef VELOCITY_CONTROL_IL 

#undef POSITION_CONTROL_P 
#undef POSITION_CONTROL_I 
#undef POSITION_CONTROL_D 
#undef POSITION_CONTROL_IL
//End of Undefine Required Profile Name List
