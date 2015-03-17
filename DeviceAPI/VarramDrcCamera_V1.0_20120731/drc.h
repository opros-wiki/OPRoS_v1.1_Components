#pragma once

#include <stdio.h>

#include <cv.h>
#include <highgui.h>
#include "USBLibrary.h"

#define BOUNDRY_MAXNUM	1000


typedef struct _region{
  int color;         // id of the color
  int x1,y1,x2,y2;   // bounding box (x1,y1) - (x2,y2)
  double cen_x,cen_y; // centroid
  int area;          // occupied area in pixels
  int boundx[BOUNDRY_MAXNUM];
  int boundy[BOUNDRY_MAXNUM];
  int bnd_dst[BOUNDRY_MAXNUM];
  int boundcnt;
  int diagoncnt;
  int cir_ratio;
  int bnd_num;
  long mv[4];
  int possible;
}Region;
#define MAXREGION	50

typedef struct _Area
{
	int r[2];
	int g[2];
	int b[2];
}
Area;

typedef struct _IRSENSOR
{
	int ir[8];
}
IRSENSOR;

typedef struct _colormap
{
	int Y_min;
	int Y_max;
	int Cr_min;
	int Cr_max;
	int Cb_min;
	int Cb_max;
}
colmap;


typedef struct _FACE
{
	int cx;
	int cy;
	int width;
	int flag;
}
face;

#define MAX_CORNER	500

typedef struct _CornerPoints
{
	int num;
	int x[MAX_CORNER];
	int y[MAX_CORNER];
}CornerPoints;




typedef struct _drcBound
{
  	int xchain[BOUNDRY_MAXNUM];
	int ychain[BOUNDRY_MAXNUM]; 
}
drcBound;

typedef struct _DRCIMG {
	IplImage *image;
	BYTE sensor[21];
} DRCIMG;


//명령데이터의 인덱스 결정
#define HEADER		0
#define RMOTOR		1
#define LMOTOR		2
#define LRSERVO		3
#define UDSERVO		4
#define IMGQUALITY	30
#define BAUDRATE	31
#define PC2DRC_DATA	32

#define DRC2PC_DATA	33
#define DRC_D		34
#define DRC_R		35
#define DRC_C		36

#define FREQUENCY		63

#define DCGAIN	127	
#define DCMAX	DCGAIN
#define DCMIN	(-DCGAIN)

#define IMG_W	160
#define IMG_H	120

#define HIST_W	256
#define HIST_H	100
#define DMIN		0
#define DCEN		1
#define DMAX	2


#define STACKSIZE	IMG_H*IMG_W



typedef struct _DEVICE {
	char *name;
	unsigned int number;
	unsigned int txrate;
	struct _DEVICE *next;
	char active;
} *PDEVICE, DEVICE;

#include "api.h"
