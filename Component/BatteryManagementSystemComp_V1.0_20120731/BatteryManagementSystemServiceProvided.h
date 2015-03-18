#ifndef _BatteryManagementSystemService_PROVIDED_PORT_H
#define _BatteryManagementSystemService_PROVIDED_PORT_H

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

#ifndef _IBatteryManagementSystemService_CLASS_DEF
#define _IBatteryManagementSystemService_CLASS_DEF
/*
 * IBatteryManagementSystemService
 *
 *  The comonent inherits this class and implements methods. 
 */
class IBatteryManagementSystemService
{
 public:
    virtual ReturnType SetParameter(Property parameter)=0;
    virtual Property GetParameter()=0;
    virtual ReturnType GetError()=0;
    virtual ReturnType Enable()=0;
    virtual ReturnType Disable()=0;
    virtual double GetChargingRate()=0;
    virtual double GetVoltage()=0;
    virtual vector<double> GetCellVoltage()=0;
 };
#endif

/*
 * 
 * BatteryManagementSystemService : public ProvidedServicePort
 *
 */
class BatteryManagementSystemServiceProvided
	:public ProvidedServicePort, public IBatteryManagementSystemService
{
protected:
    IBatteryManagementSystemService *pcom;


   typedef ProvidedMethod<ReturnType> SetParameterFuncType;
   SetParameterFuncType *SetParameterFunc;

   typedef ProvidedMethod<Property> GetParameterFuncType;
   GetParameterFuncType *GetParameterFunc;

   typedef ProvidedMethod<ReturnType> GetErrorFuncType;
   GetErrorFuncType *GetErrorFunc;

   typedef ProvidedMethod<ReturnType> EnableFuncType;
   EnableFuncType *EnableFunc;

   typedef ProvidedMethod<ReturnType> DisableFuncType;
   DisableFuncType *DisableFunc;

   typedef ProvidedMethod<double> GetChargingRateFuncType;
   GetChargingRateFuncType *GetChargingRateFunc;

   typedef ProvidedMethod<double> GetVoltageFuncType;
   GetVoltageFuncType *GetVoltageFunc;

   typedef ProvidedMethod< vector<double> > GetCellVoltageFuncType;
   GetCellVoltageFuncType *GetCellVoltageFunc;


public: // default method
   virtual ReturnType SetParameter(Property parameter)
   {
		opros_assert(SetParameterFunc != NULL);
		
            return (*SetParameterFunc)(parameter);
		
   }
   virtual Property GetParameter()
   {
		opros_assert(GetParameterFunc != NULL);
		
            return (*GetParameterFunc)();
		
   }
   virtual ReturnType GetError()
   {
		opros_assert(GetErrorFunc != NULL);
		
            return (*GetErrorFunc)();
		
   }
   virtual ReturnType Enable()
   {
		opros_assert(EnableFunc != NULL);
		
            return (*EnableFunc)();
		
   }
   virtual ReturnType Disable()
   {
		opros_assert(DisableFunc != NULL);
		
            return (*DisableFunc)();
		
   }
   virtual double GetChargingRate()
   {
		opros_assert(GetChargingRateFunc != NULL);
		
            return (*GetChargingRateFunc)();
		
   }
   virtual double GetVoltage()
   {
		opros_assert(GetVoltageFunc != NULL);
		
            return (*GetVoltageFunc)();
		
   }
   virtual vector<double> GetCellVoltage()
   {
		opros_assert(GetCellVoltageFunc != NULL);
		
            return (*GetCellVoltageFunc)();
		
   }


public:
    //
    // Constructor
    //
    BatteryManagementSystemServiceProvided(IBatteryManagementSystemService *fn)
    {
        pcom = fn;

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

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::SetParameter,pcom,"SetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("SetParameter",ptr_method);
        SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::GetParameter,pcom,"GetParameter");

        opros_assert(ptr_method != NULL);
        addMethod("GetParameter",ptr_method);
        GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::GetError,pcom,"GetError");

        opros_assert(ptr_method != NULL);
        addMethod("GetError",ptr_method);
        GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::Enable,pcom,"Enable");

        opros_assert(ptr_method != NULL);
        addMethod("Enable",ptr_method);
        EnableFunc = reinterpret_cast<EnableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::Disable,pcom,"Disable");

        opros_assert(ptr_method != NULL);
        addMethod("Disable",ptr_method);
        DisableFunc = reinterpret_cast<DisableFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::GetChargingRate,pcom,"GetChargingRate");

        opros_assert(ptr_method != NULL);
        addMethod("GetChargingRate",ptr_method);
        GetChargingRateFunc = reinterpret_cast<GetChargingRateFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::GetVoltage,pcom,"GetVoltage");

        opros_assert(ptr_method != NULL);
        addMethod("GetVoltage",ptr_method);
        GetVoltageFunc = reinterpret_cast<GetVoltageFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IBatteryManagementSystemService::GetCellVoltage,pcom,"GetCellVoltage");

        opros_assert(ptr_method != NULL);
        addMethod("GetCellVoltage",ptr_method);
        GetCellVoltageFunc = reinterpret_cast<GetCellVoltageFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
