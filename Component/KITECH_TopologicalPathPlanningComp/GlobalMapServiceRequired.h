#ifndef _GlobalMapService_REQUIRED_PORT_H
#define _GlobalMapService_REQUIRED_PORT_H

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
 * 
 * GlobalMapService : public RequiredServicePort
 *
 */
class GlobalMapServiceRequired : public RequiredServicePort
{
protected:

	typedef RequiredMethod<bool> SetParameterFuncType;
	SetParameterFuncType *SetParameterFunc;

	typedef RequiredMethod<Property> GetParameterFuncType;
	GetParameterFuncType *GetParameterFunc;

	typedef RequiredMethod<int> GetErrorFuncType;
	GetErrorFuncType *GetErrorFunc;

	typedef RequiredMethod<bool> LoadMapFileFuncType;
	LoadMapFileFuncType *LoadMapFileFunc;

	typedef RequiredMethod<bool> SaveMapFileFuncType;
	SaveMapFileFuncType *SaveMapFileFunc;

	typedef RequiredMethod<double> GetMapCellWidthFuncType;
	GetMapCellWidthFuncType *GetMapCellWidthFunc;

	typedef RequiredMethod<int> GetMapWidthFuncType;
	GetMapWidthFuncType *GetMapWidthFunc;

	typedef RequiredMethod<int> GetMapHeightFuncType;
	GetMapHeightFuncType *GetMapHeightFunc;

	typedef RequiredMethod<unsigned char> GetPixelFuncType;
	GetPixelFuncType *GetPixelFunc;

	typedef RequiredMethod<unsigned char> GetPixelCellFuncType;
	GetPixelCellFuncType *GetPixelCellFunc;

	typedef RequiredMethod<void> SetPixelFuncType;
	SetPixelFuncType *SetPixelFunc;

	typedef RequiredMethod<void> SetPixelCellFuncType;
	SetPixelCellFuncType *SetPixelCellFunc;

	typedef RequiredMethod<bool> IsInFuncType;
	IsInFuncType *IsInFunc;

	typedef RequiredMethod<bool> IsInCellFuncType;
	IsInCellFuncType *IsInCellFunc;

	typedef RequiredMethod<bool> IsIntersectFuncType;
	IsIntersectFuncType *IsIntersectFunc;

	typedef RequiredMethod<bool> IsIntersectCellFuncType;
	IsIntersectCellFuncType *IsIntersectCellFunc;

	typedef RequiredMethod<void> DrawLineFuncType;
	DrawLineFuncType *DrawLineFunc;

	typedef RequiredMethod<void> DrawLineCellFuncType;
	DrawLineCellFuncType *DrawLineCellFunc;

	typedef RequiredMethod<void> ExpandObstacleFuncType;
	ExpandObstacleFuncType *ExpandObstacleFunc;

	typedef RequiredMethod<void> ReallocateFuncType;
	ReallocateFuncType *ReallocateFunc;

	typedef RequiredMethod<void> UpdateFuncType;
	UpdateFuncType *UpdateFunc;

	typedef RequiredMethod<int> M2CUxFuncType;
	M2CUxFuncType *M2CUxFunc;

	typedef RequiredMethod<int> M2CUyFuncType;
	M2CUyFuncType *M2CUyFunc;

	typedef RequiredMethod<double> CU2MxFuncType;
	CU2MxFuncType *CU2MxFunc;

	typedef RequiredMethod<double> CU2MyFuncType;
	CU2MyFuncType *CU2MyFunc;

public:
	//
	// Constructor
	//
	GlobalMapServiceRequired()
	{
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

	// method implementation for required method
	bool SetParameter(Property parameter)
	{
		assert(SetParameterFunc != NULL);
		return (*SetParameterFunc)(parameter);
	}

	Property GetParameter()
	{
		assert(GetParameterFunc != NULL);
		return (*GetParameterFunc)();
	}

	int GetError()
	{
		assert(GetErrorFunc != NULL);
		return (*GetErrorFunc)();
	}

	bool LoadMapFile(std::string fileName,double cellWidth)
	{
		assert(LoadMapFileFunc != NULL);
		return (*LoadMapFileFunc)(fileName,cellWidth);
	}

	bool SaveMapFile(std::string fileName)
	{
		assert(SaveMapFileFunc != NULL);
		return (*SaveMapFileFunc)(fileName);
	}

	double GetMapCellWidth()
	{
		assert(GetMapCellWidthFunc != NULL);
		return (*GetMapCellWidthFunc)();
	}

	int GetMapWidth()
	{
		assert(GetMapWidthFunc != NULL);
		return (*GetMapWidthFunc)();
	}

	int GetMapHeight()
	{
		assert(GetMapHeightFunc != NULL);
		return (*GetMapHeightFunc)();
	}

	unsigned char GetPixel(double x,double y)
	{
		assert(GetPixelFunc != NULL);
		return (*GetPixelFunc)(x,y);
	}

	unsigned char GetPixelCell(int x,int y)
	{
		assert(GetPixelCellFunc != NULL);
		return (*GetPixelCellFunc)(x,y);
	}

	void SetPixel(double x,double y,unsigned char data)
	{
		assert(SetPixelFunc != NULL);
		(*SetPixelFunc)(x,y,data);
	}

	void SetPixelCell(int x,int y,unsigned char data)
	{
		assert(SetPixelCellFunc != NULL);
		(*SetPixelCellFunc)(x,y,data);
	}

	bool IsIn(double x,double y)
	{
		assert(IsInFunc != NULL);
		return (*IsInFunc)(x,y);
	}

	bool IsInCell(int x,int y)
	{
		assert(IsInCellFunc != NULL);
		return (*IsInCellFunc)(x,y);
	}

	bool IsIntersect(double x1,double y1,double x2,double y2,unsigned char data)
	{
		assert(IsIntersectFunc != NULL);
		return (*IsIntersectFunc)(x1,y1,x2,y2,data);
	}

	bool IsIntersectCell(int x1,int y1,int x2,int y2,unsigned char data)
	{
		assert(IsIntersectCellFunc != NULL);
		return (*IsIntersectCellFunc)(x1,y1,x2,y2,data);
	}

	void DrawLine(double x1,double y1,double x2,double y2,unsigned char data)
	{
		assert(DrawLineFunc != NULL);
		(*DrawLineFunc)(x1,y1,x2,y2,data);
	}

	void DrawLineCell(int x1,int y1,int x2,int y2,unsigned char data)
	{
		assert(DrawLineCellFunc != NULL);
		(*DrawLineCellFunc)(x1,y1,x2,y2,data);
	}

	void ExpandObstacle(int bulge,unsigned char threshold,unsigned char data)
	{
		assert(ExpandObstacleFunc != NULL);
		(*ExpandObstacleFunc)(bulge,threshold,data);
	}

	void Reallocate(int offsetX,int offsetY,int width,int height)
	{
		assert(ReallocateFunc != NULL);
		(*ReallocateFunc)(offsetX,offsetY,width,height);
	}

	void Update(ObjectLocation position,std::vector<double> sensorValue)
	{
		assert(UpdateFunc != NULL);
		(*UpdateFunc)(position,sensorValue);
	}

	int M2CUx(double x)
	{
		assert(M2CUxFunc != NULL);
		return (*M2CUxFunc)(x);
	}

	int M2CUy(double y)
	{
		assert(M2CUyFunc != NULL);
		return (*M2CUyFunc)(y);
	}

	double CU2Mx(int x)
	{
		assert(CU2MxFunc != NULL);
		return (*CU2MxFunc)(x);
	}

	double CU2My(int y)
	{
		assert(CU2MyFunc != NULL);
		return (*CU2MyFunc)(y);
	}

	// generated setup function
	virtual void setup()
	{
		Method *ptr_method;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::SetParameter,"SetParameter");
		assert(ptr_method != NULL);
		addMethod("SetParameter",ptr_method);
		SetParameterFunc = reinterpret_cast<SetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetParameter,"GetParameter");
		assert(ptr_method != NULL);
		addMethod("GetParameter",ptr_method);
		GetParameterFunc = reinterpret_cast<GetParameterFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetError,"GetError");
		assert(ptr_method != NULL);
		addMethod("GetError",ptr_method);
		GetErrorFunc = reinterpret_cast<GetErrorFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::LoadMapFile,"LoadMapFile");
		assert(ptr_method != NULL);
		addMethod("LoadMapFile",ptr_method);
		LoadMapFileFunc = reinterpret_cast<LoadMapFileFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::SaveMapFile,"SaveMapFile");
		assert(ptr_method != NULL);
		addMethod("SaveMapFile",ptr_method);
		SaveMapFileFunc = reinterpret_cast<SaveMapFileFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetMapCellWidth,"GetMapCellWidth");
		assert(ptr_method != NULL);
		addMethod("GetMapCellWidth",ptr_method);
		GetMapCellWidthFunc = reinterpret_cast<GetMapCellWidthFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetMapWidth,"GetMapWidth");
		assert(ptr_method != NULL);
		addMethod("GetMapWidth",ptr_method);
		GetMapWidthFunc = reinterpret_cast<GetMapWidthFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetMapHeight,"GetMapHeight");
		assert(ptr_method != NULL);
		addMethod("GetMapHeight",ptr_method);
		GetMapHeightFunc = reinterpret_cast<GetMapHeightFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetPixel,"GetPixel");
		assert(ptr_method != NULL);
		addMethod("GetPixel",ptr_method);
		GetPixelFunc = reinterpret_cast<GetPixelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::GetPixelCell,"GetPixelCell");
		assert(ptr_method != NULL);
		addMethod("GetPixelCell",ptr_method);
		GetPixelCellFunc = reinterpret_cast<GetPixelCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::SetPixel,"SetPixel");
		assert(ptr_method != NULL);
		addMethod("SetPixel",ptr_method);
		SetPixelFunc = reinterpret_cast<SetPixelFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::SetPixelCell,"SetPixelCell");
		assert(ptr_method != NULL);
		addMethod("SetPixelCell",ptr_method);
		SetPixelCellFunc = reinterpret_cast<SetPixelCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::IsIn,"IsIn");
		assert(ptr_method != NULL);
		addMethod("IsIn",ptr_method);
		IsInFunc = reinterpret_cast<IsInFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::IsInCell,"IsInCell");
		assert(ptr_method != NULL);
		addMethod("IsInCell",ptr_method);
		IsInCellFunc = reinterpret_cast<IsInCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::IsIntersect,"IsIntersect");
		assert(ptr_method != NULL);
		addMethod("IsIntersect",ptr_method);
		IsIntersectFunc = reinterpret_cast<IsIntersectFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::IsIntersectCell,"IsIntersectCell");
		assert(ptr_method != NULL);
		addMethod("IsIntersectCell",ptr_method);
		IsIntersectCellFunc = reinterpret_cast<IsIntersectCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::DrawLine,"DrawLine");
		assert(ptr_method != NULL);
		addMethod("DrawLine",ptr_method);
		DrawLineFunc = reinterpret_cast<DrawLineFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::DrawLineCell,"DrawLineCell");
		assert(ptr_method != NULL);
		addMethod("DrawLineCell",ptr_method);
		DrawLineCellFunc = reinterpret_cast<DrawLineCellFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::ExpandObstacle,"ExpandObstacle");
		assert(ptr_method != NULL);
		addMethod("ExpandObstacle",ptr_method);
		ExpandObstacleFunc = reinterpret_cast<ExpandObstacleFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::Reallocate,"Reallocate");
		assert(ptr_method != NULL);
		addMethod("Reallocate",ptr_method);
		ReallocateFunc = reinterpret_cast<ReallocateFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::Update,"Update");
		assert(ptr_method != NULL);
		addMethod("Update",ptr_method);
		UpdateFunc = reinterpret_cast<UpdateFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::M2CUx,"M2CUx");
		assert(ptr_method != NULL);
		addMethod("M2CUx",ptr_method);
		M2CUxFunc = reinterpret_cast<M2CUxFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::M2CUy,"M2CUy");
		assert(ptr_method != NULL);
		addMethod("M2CUy",ptr_method);
		M2CUyFunc = reinterpret_cast<M2CUyFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::CU2Mx,"CU2Mx");
		assert(ptr_method != NULL);
		addMethod("CU2Mx",ptr_method);
		CU2MxFunc = reinterpret_cast<CU2MxFuncType *>(ptr_method);
		ptr_method = NULL;

		ptr_method = makeRequiredMethod(&GlobalMapServiceRequired::CU2My,"CU2My");
		assert(ptr_method != NULL);
		addMethod("CU2My",ptr_method);
		CU2MyFunc = reinterpret_cast<CU2MyFuncType *>(ptr_method);
		ptr_method = NULL;
	}
};
#endif
