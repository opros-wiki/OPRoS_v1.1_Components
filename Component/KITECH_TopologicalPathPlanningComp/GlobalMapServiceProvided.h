#ifndef _GlobalMapService_PROVIDED_PORT_H
#define _GlobalMapService_PROVIDED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>

#include "ApiTypes.h"
#include "Property.h"

/*
 * IGlobalMapService
 *
 * The comonent inherits this class and implements methods. 
*/
class IGlobalMapService
{
public:
	virtual bool SetParameter(Property parameter)=0;
	virtual Property GetParameter()=0;
	virtual int GetError()=0;
	virtual bool LoadMapFile(std::string fileName,double cellWidth)=0;
	virtual bool SaveMapFile(std::string fileName)=0;
	virtual double GetMapCellWidth()=0;
	virtual int GetMapWidth()=0;
	virtual int GetMapHeight()=0;
	virtual unsigned char GetPixel(double x,double y)=0;
	virtual unsigned char GetPixelCell(int x,int y)=0;
	virtual void SetPixel(double x,double y,unsigned char data)=0;
	virtual void SetPixelCell(int x,int y,unsigned char data)=0;
	virtual bool IsIn(double x,double y)=0;
	virtual bool IsInCell(int x,int y)=0;
	virtual bool IsIntersect(double x1,double y1,double x2,double y2,unsigned char data)=0;
	virtual bool IsIntersectCell(int x1,int y1,int x2,int y2,unsigned char data)=0;
	virtual void DrawLine(double x1,double y1,double x2,double y2,unsigned char data)=0;
	virtual void DrawLineCell(int x1,int y1,int x2,int y2,unsigned char data)=0;
	virtual void ExpandObstacle(int bulge,unsigned char threshold,unsigned char data)=0;
	virtual void Reallocate(int offsetX,int offsetY,int width,int height)=0;
	virtual void Update(ObjectLocation position, std::vector<double> sensorValue)=0;
	virtual int M2CUx(double x)=0;
	virtual int M2CUy(double y)=0;
	virtual double CU2Mx(int x)=0;
	virtual double CU2My(int y)=0;
 };

/*
 * 
 * GlobalMapService : public ProvidedServicePort
 *
 */
class GlobalMapServiceProvided
	:public ProvidedServicePort, public IGlobalMapService
{
protected:
	IGlobalMapService *pcom;

	typedef ProvidedMethod<bool> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef ProvidedMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef ProvidedMethod<int> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef ProvidedMethod<bool> LoadMapFileFuncType;
	LoadMapFileFuncType *LoadMapFileFunc;

	typedef ProvidedMethod<bool> SaveMapFileFuncType;
	SaveMapFileFuncType *SaveMapFileFunc;

	typedef ProvidedMethod<double> GetMapCellWidthFuncType;
	GetMapCellWidthFuncType *GetMapCellWidthFunc;

	typedef ProvidedMethod<int> GetMapWidthFuncType;
	GetMapWidthFuncType *GetMapWidthFunc;

	typedef ProvidedMethod<int> GetMapHeightFuncType;
	GetMapHeightFuncType *GetMapHeightFunc;

	typedef ProvidedMethod<unsigned char> GetPixelFuncType;
	GetPixelFuncType *GetPixelFunc;

	typedef ProvidedMethod<unsigned char> GetPixelCellFuncType;
	GetPixelCellFuncType *GetPixelCellFunc;

	typedef ProvidedMethod<void> SetPixelFuncType;
	SetPixelFuncType *SetPixelFunc;

	typedef ProvidedMethod<void> SetPixelCellFuncType;
	SetPixelCellFuncType *SetPixelCellFunc;

	typedef ProvidedMethod<bool> IsInFuncType;
	IsInFuncType *IsInFunc;

	typedef ProvidedMethod<bool> IsInCellFuncType;
	IsInCellFuncType *IsInCellFunc;

	typedef ProvidedMethod<bool> IsIntersectFuncType;
	IsIntersectFuncType *IsIntersectFunc;

	typedef ProvidedMethod<bool> IsIntersectCellFuncType;
	IsIntersectCellFuncType *IsIntersectCellFunc;

	typedef ProvidedMethod<void> DrawLineFuncType;
	DrawLineFuncType *DrawLineFunc;

	typedef ProvidedMethod<void> DrawLineCellFuncType;
	DrawLineCellFuncType *DrawLineCellFunc;

	typedef ProvidedMethod<void> ExpandObstacleFuncType;
	ExpandObstacleFuncType *ExpandObstacleFunc;

	typedef ProvidedMethod<void> ReallocateFuncType;
	ReallocateFuncType *ReallocateFunc;

	typedef ProvidedMethod<void> UpdateFuncType;
	UpdateFuncType *UpdateFunc;

	typedef ProvidedMethod<int> M2CUxFuncType;
	M2CUxFuncType *M2CUxFunc;

	typedef ProvidedMethod<int> M2CUyFuncType;
	M2CUyFuncType *M2CUyFunc;

	typedef ProvidedMethod<double> CU2MxFuncType;
	CU2MxFuncType *CU2MxFunc;

	typedef ProvidedMethod<double> CU2MyFuncType;
	CU2MyFuncType *CU2MyFunc;


public: // default method
	virtual bool SetParameter(Property parameter)
	{
		assert(SetParameterFunc != NULL);
		return (*SetParameterFunc)(parameter);
	}

	virtual Property GetParameter()
	{
		assert(GetParameterFunc != NULL);
		return (*GetParameterFunc)();
	}

	virtual int GetError()
	{
		assert(GetErrorFunc != NULL);
		return (*GetErrorFunc)();
	}

	virtual bool LoadMapFile(std::string fileName,double cellWidth)
	{
		assert(LoadMapFileFunc != NULL);
		return (*LoadMapFileFunc)(fileName,cellWidth);
	}

	virtual bool SaveMapFile(std::string fileName)
	{
		assert(SaveMapFileFunc != NULL);
		return (*SaveMapFileFunc)(fileName);
	}

	virtual double GetMapCellWidth()
	{
		assert(GetMapCellWidthFunc != NULL);
		return (*GetMapCellWidthFunc)();
	}

	virtual int GetMapWidth()
	{
		assert(GetMapWidthFunc != NULL);
		return (*GetMapWidthFunc)();
	}

	virtual int GetMapHeight()
	{
		assert(GetMapHeightFunc != NULL);
		return (*GetMapHeightFunc)();
	}

	virtual unsigned char GetPixel(double x,double y)
	{
		assert(GetPixelFunc != NULL);
		return (*GetPixelFunc)(x,y);
	}

	virtual unsigned char GetPixelCell(int x,int y)
	{
		assert(GetPixelCellFunc != NULL);
		return (*GetPixelCellFunc)(x,y);
	}

	virtual void SetPixel(double x,double y,unsigned char data)
	{
		assert(SetPixelFunc != NULL);
		(*SetPixelFunc)(x,y,data);
	}

	virtual void SetPixelCell(int x,int y,unsigned char data)
	{
		assert(SetPixelCellFunc != NULL);
		(*SetPixelCellFunc)(x,y,data);
	}

	virtual bool IsIn(double x,double y)
	{
		assert(IsInFunc != NULL);
		return (*IsInFunc)(x,y);
	}

	virtual bool IsInCell(int x,int y)
	{
		assert(IsInCellFunc != NULL);
		return (*IsInCellFunc)(x,y);
	}

	virtual bool IsIntersect(double x1,double y1,double x2,double y2,unsigned char data)
	{
		assert(IsIntersectFunc != NULL);
		return (*IsIntersectFunc)(x1,y1,x2,y2,data);
	}

	virtual bool IsIntersectCell(int x1,int y1,int x2,int y2,unsigned char data)
	{
		assert(IsIntersectCellFunc != NULL);
		return (*IsIntersectCellFunc)(x1,y1,x2,y2,data);
	}

	virtual void DrawLine(double x1,double y1,double x2,double y2,unsigned char data)
	{
		assert(DrawLineFunc != NULL);
		(*DrawLineFunc)(x1,y1,x2,y2,data);
	}

	virtual void DrawLineCell(int x1,int y1,int x2,int y2,unsigned char data)
	{
		assert(DrawLineCellFunc != NULL);
		(*DrawLineCellFunc)(x1,y1,x2,y2,data);
	}

	virtual void ExpandObstacle(int bulge,unsigned char threshold,unsigned char data)
	{
		assert(ExpandObstacleFunc != NULL);
		(*ExpandObstacleFunc)(bulge,threshold,data);
	}

	virtual void Reallocate(int offsetX,int offsetY,int width,int height)
	{
		assert(ReallocateFunc != NULL);
		(*ReallocateFunc)(offsetX,offsetY,width,height);
	}

	virtual void Update(ObjectLocation position,std::vector<double> sensorValue)
	{
		assert(UpdateFunc != NULL);
		(*UpdateFunc)(position,sensorValue);
	}

	virtual int M2CUx(double x)
	{
		assert(M2CUxFunc != NULL);
		return (*M2CUxFunc)(x);
	}

	virtual int M2CUy(double y)
	{
		assert(M2CUyFunc != NULL);
		return (*M2CUyFunc)(y);
	}

	virtual double CU2Mx(int x)
	{
		assert(CU2MxFunc != NULL);
		return (*CU2MxFunc)(x);
	}

	virtual double CU2My(int y)
	{
		assert(CU2MyFunc != NULL);
		return (*CU2MyFunc)(y);
	}

public:
	//
	// Constructor
	//
	GlobalMapServiceProvided(IGlobalMapService *fn)
	{
		pcom = fn;

		SetParameterFunc = NULL;
		GetParameterFunc = NULL;
		GetErrorFunc = NULL;
		LoadMapFileFunc = NULL;
		SaveMapFileFunc = NULL;
		GetMapCellWidthFunc = NULL;
		GetMapWidthFunc = NULL;
		GetMapHeightFunc = NULL;
		GetPixelFunc = NULL;
		GetPixelCellFunc = NULL;
		SetPixelFunc = NULL;
		SetPixelCellFunc = NULL;
		IsInFunc = NULL;
		IsInCellFunc = NULL;
		IsIntersectFunc = NULL;
		IsIntersectCellFunc = NULL;
		DrawLineFunc = NULL;
		DrawLineCellFunc = NULL;
		ExpandObstacleFunc = NULL;
		ReallocateFunc = NULL;
		UpdateFunc = NULL;
		M2CUxFunc = NULL;
		M2CUyFunc = NULL;
		CU2MxFunc = NULL;
		CU2MyFunc = NULL;

		setup();
	}

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeProvidedMethod(&IGlobalMapService::SetParameter,pcom,"SetParameter");
		assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetParameter,pcom,"GetParameter");
		assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetError,pcom,"GetError");
		assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::LoadMapFile,pcom,"LoadMapFile");
		assert(ptr_method != NULL);
		addMethod("LoadMapFile",ptr_method);
		LoadMapFileFunc = reinterpret_cast<LoadMapFileFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::SaveMapFile,pcom,"SaveMapFile");
		assert(ptr_method != NULL);
		addMethod("SaveMapFile",ptr_method);
		SaveMapFileFunc = reinterpret_cast<SaveMapFileFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetMapCellWidth,pcom,"GetMapCellWidth");
		assert(ptr_method != NULL);
		addMethod("GetMapCellWidth",ptr_method);
		GetMapCellWidthFunc = reinterpret_cast<GetMapCellWidthFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetMapWidth,pcom,"GetMapWidth");
		assert(ptr_method != NULL);
		addMethod("GetMapWidth",ptr_method);
		GetMapWidthFunc = reinterpret_cast<GetMapWidthFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetMapHeight,pcom,"GetMapHeight");
		assert(ptr_method != NULL);
		addMethod("GetMapHeight",ptr_method);
		GetMapHeightFunc = reinterpret_cast<GetMapHeightFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetPixel,pcom,"GetPixel");
		assert(ptr_method != NULL);
		addMethod("GetPixel",ptr_method);
		GetPixelFunc = reinterpret_cast<GetPixelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::GetPixelCell,pcom,"GetPixelCell");
		assert(ptr_method != NULL);
		addMethod("GetPixelCell",ptr_method);
		GetPixelCellFunc = reinterpret_cast<GetPixelCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::SetPixel,pcom,"SetPixel");
		assert(ptr_method != NULL);
		addMethod("SetPixel",ptr_method);
		SetPixelFunc = reinterpret_cast<SetPixelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::SetPixelCell,pcom,"SetPixelCell");
		assert(ptr_method != NULL);
		addMethod("SetPixelCell",ptr_method);
		SetPixelCellFunc = reinterpret_cast<SetPixelCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::IsIn,pcom,"IsIn");
		assert(ptr_method != NULL);
		addMethod("IsIn",ptr_method);
		IsInFunc = reinterpret_cast<IsInFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::IsInCell,pcom,"IsInCell");
		assert(ptr_method != NULL);
		addMethod("IsInCell",ptr_method);
		IsInCellFunc = reinterpret_cast<IsInCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::IsIntersect,pcom,"IsIntersect");
		assert(ptr_method != NULL);
		addMethod("IsIntersect",ptr_method);
		IsIntersectFunc = reinterpret_cast<IsIntersectFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::IsIntersectCell,pcom,"IsIntersectCell");
		assert(ptr_method != NULL);
		addMethod("IsIntersectCell",ptr_method);
		IsIntersectCellFunc = reinterpret_cast<IsIntersectCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::DrawLine,pcom,"DrawLine");
		assert(ptr_method != NULL);
		addMethod("DrawLine",ptr_method);
		DrawLineFunc = reinterpret_cast<DrawLineFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::DrawLineCell,pcom,"DrawLineCell");
		assert(ptr_method != NULL);
		addMethod("DrawLineCell",ptr_method);
		DrawLineCellFunc = reinterpret_cast<DrawLineCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::ExpandObstacle,pcom,"ExpandObstacle");
		assert(ptr_method != NULL);
		addMethod("ExpandObstacle",ptr_method);
		ExpandObstacleFunc = reinterpret_cast<ExpandObstacleFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::Reallocate,pcom,"Reallocate");
		assert(ptr_method != NULL);
		addMethod("Reallocate",ptr_method);
		ReallocateFunc = reinterpret_cast<ReallocateFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::Update,pcom,"Update");
		assert(ptr_method != NULL);
		addMethod("Update",ptr_method);
		UpdateFunc = reinterpret_cast<UpdateFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::M2CUx,pcom,"M2CUx");
		assert(ptr_method != NULL);
		addMethod("M2CUx",ptr_method);
		M2CUxFunc = reinterpret_cast<M2CUxFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::M2CUy,pcom,"M2CUy");
		assert(ptr_method != NULL);
		addMethod("M2CUy",ptr_method);
		M2CUyFunc = reinterpret_cast<M2CUyFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::CU2Mx,pcom,"CU2Mx");
		assert(ptr_method != NULL);
		addMethod("CU2Mx",ptr_method);
		CU2MxFunc = reinterpret_cast<CU2MxFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeProvidedMethod(&IGlobalMapService::CU2My,pcom,"CU2My");
		assert(ptr_method != NULL);
		addMethod("CU2My",ptr_method);
		CU2MyFunc = reinterpret_cast<CU2MyFuncType *>(ptr_method);
		ptr_method = NULL;
	}
};
#endif
