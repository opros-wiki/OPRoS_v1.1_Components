#ifndef _BatteryManagementSystemService_REQUIRED_PORT_H
#define _BatteryManagementSystemService_REQUIRED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>

#include "Property.h"

/*
 * 
 * BatteryManagementSystemService : public RequiredServicePort
 *
 */
class BatteryManagementSystemServiceRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<ReturnType> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<ReturnType> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<ReturnType> EnableFuncType;
	EnableFuncType *EnableFunc;

	typedef RequiredMethod<ReturnType> DisableFuncType;
	DisableFuncType *DisableFunc;

	typedef RequiredMethod<double> GetChargingRateFuncType;
	GetChargingRateFuncType *GetChargingRateFunc;

	typedef RequiredMethod<double> GetVoltageFuncType;
	GetVoltageFuncType *GetVoltageFunc;

	typedef RequiredMethod< vector<double> > GetCellVoltageFuncType;
	GetCellVoltageFuncType *GetCellVoltageFunc;

public:
	//
	// Constructor
	//
	BatteryManagementSystemServiceRequired()
	{
            SetParameterFunc = NULL;
            GetParameterFunc = NULL;
            GetErrorFunc = NULL;
            EnableFunc = NULL;
            DisableFunc = NULL;
            GetChargingRateFunc = NULL;
            GetVoltageFunc = NULL;
            GetCellVoltageFunc = NULL;
            
	   setup();
	}

	// method implementation for required method
	ReturnType SetParameter(Property parameter)
	{
            opros_assert(SetParameterFunc != NULL);
	
            return (*SetParameterFunc)(parameter);
		
	}

	Property GetParameter()
	{
            opros_assert(GetParameterFunc != NULL);
	
            return (*GetParameterFunc)();
		
	}

	ReturnType GetError()
	{
            opros_assert(GetErrorFunc != NULL);
	
            return (*GetErrorFunc)();
		
	}

	ReturnType Enable()
	{
            opros_assert(EnableFunc != NULL);
	
            return (*EnableFunc)();
		
	}

	ReturnType Disable()
	{
            opros_assert(DisableFunc != NULL);
	
            return (*DisableFunc)();
		
	}

	double GetChargingRate()
	{
            opros_assert(GetChargingRateFunc != NULL);
	
            return (*GetChargingRateFunc)();
		
	}

	double GetVoltage()
	{
            opros_assert(GetVoltageFunc != NULL);
	
            return (*GetVoltageFunc)();
		
	}

	vector<double> GetCellVoltage()
	{
            opros_assert(GetCellVoltageFunc != NULL);
	
            return (*GetCellVoltageFunc)();
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::SetParameter,"SetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::GetParameter,"GetParameter");
        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::GetError,"GetError");
        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::Enable,"Enable");
        opros_assert(ptr_method != NULL);
        addMethod("Enable",ptr_method);
        EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::Disable,"Disable");
        opros_assert(ptr_method != NULL);
        addMethod("Disable",ptr_method);
        DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::GetChargingRate,"GetChargingRate");
        opros_assert(ptr_method != NULL);
        addMethod("GetChargingRate",ptr_method);
        GetChargingRateFunc = reinterpret_cast<GetChargingRateFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::GetVoltage,"GetVoltage");
        opros_assert(ptr_method != NULL);
        addMethod("GetVoltage",ptr_method);
        GetVoltageFunc = reinterpret_cast<GetVoltageFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&BatteryManagementSystemServiceRequired::GetCellVoltage,"GetCellVoltage");
        opros_assert(ptr_method != NULL);
        addMethod("GetCellVoltage",ptr_method);
        GetCellVoltageFunc = reinterpret_cast<GetCellVoltageFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
