
#include "ARToolkitOPRoSApp.h"
	
#ifndef _ARToolkitOPRoS_COMPONENT_H
#define _ARToolkitOPRoS_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>

#include "RawImage.h"
#include "Coodinates.h"
		
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <windows.h>
#include <stdio.h>

//OpenGL
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

//ARToolkit
#include <AR/ar.h>
#include <AR/param.h>
#include <AR/video.h>
#include <AR/gsub.h>
//#include <AR/vrml97.h>
#include <AR/arvrml.h>

void MainLoop(void);
void DrawObject(void);
void Display(void);

void InitTexture( void );
void InitRenderbuffer( void );
void InitFramebuffer( void );

#define	FRAMEBUFFER_WIDTH		640
#define	FRAMEBUFFER_HEIGHT		480

#define	TEXTURE_WIDTH			FRAMEBUFFER_WIDTH
#define	TEXTURE_HEIGHT			FRAMEBUFFER_HEIGHT

#define	RENDERBUFFER_WIDTH		FRAMEBUFFER_WIDTH
#define	RENDERBUFFER_HEIGHT		FRAMEBUFFER_HEIGHT

class ARToolkitOPRoS: public Component
{
protected:
// data

	OutputDataPort< RawImage > ImageOut;
	OutputDataPort< Coodinates > PositionOut;
	InputDataPort< RawImage > ImageIn;

//event


// method for provider
	

// method for required
	


// symbol value generation
	
// member
	std::string m_cpath;

public:
	ARToolkitOPRoS();
	ARToolkitOPRoS(const std::string &compId);
	virtual ~ARToolkitOPRoS();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();

	//설정값 검사하는 함수
	bool cfgCheck_scale(void);
	bool cfgCheck_pattName(void);
	bool cfgCheck_vrmlName(void);


};

#endif

