#pragma once
#include <assert.h>
#include <math.h>

#define MM_WALL			0x01
#define MM_BULGE_X		0x02
#define MM_BULGE_Y		0x04
#define MM_BULGE		(MM_BULGE_X|MM_BULGE_Y)
#define MM_DEADCELL		(MM_WALL|MM_BULGE)

template <typename T>
class GridMap {
public:
	int _offsetX, _offsetY;
	int _width, _height;
	T *_cp;

public:
	GridMap () :
		_cp (NULL), _offsetX (0), _offsetY (0), _width (0), _height (0) { }
	GridMap (int mapSx, int mapSy, int mapWidth, int mapHeight) : 
		_cp (NULL)
	{
		Reallocate (mapSx, mapSy, mapWidth, mapHeight); 
		Clear (0);
	}
	~GridMap () 
	{ 
		if (_cp) delete [] _cp; 
	}

	void Reallocate (int offsetX, int offsetY, int _width, int _height);
	void Clear (T c);

	inline bool IsInCELL (int x, int y)
	{
		return (_cp && 0 <= x && x < _width && 0 <= y && y < _height);
	}

	inline void SetPixelCELL (int x, int y, T c)
	{
		assert (_cp && 0 <= x && x < _width && 0 <= y && y < _height);

		*(_cp + y*_width + x) = c;
	}

	inline T &GetPixelCELL (int x, int y)
	{
		assert (_cp && 0 <= x && x < _width && 0 <= y && y < _height);

		return *(_cp + y*_width + x);
	}

	void DrawLineCELL (int x1, int y1, int x2, int y2, T c);
	bool IsIntersectCELL (int x1, int y1, int &x2, int &y2, T cm);
	void ExpandObstacle (int bulge, T threshold, T newValue);
};

template <typename T>
void GridMap<T>::Reallocate (int offsetX, int offsetY, int width, int height)
{
	if (_cp) delete [] _cp;
	_cp = NULL;

	_offsetX = offsetX;
	_offsetY = offsetY;

	_width  = width;
	_height = height;

	_width = ((_width + 7)/8)*8;
	// _height = ((_height + 7)/8)*8;
//	assert (0 < _width && 0 < _height);

	if (0 < _width * _height) {
		_cp = new T [_width * _height];
	}
}

template <typename T>
void GridMap<T>::Clear (T c)
{
	T *p = _cp;
	int i, n = _width * _height;
	for (i=0; i<n; ++i) *p++ = c;
}

template <typename T>
void GridMap<T>::DrawLineCELL (int x1, int y1, int x2, int y2, T c)
{
	int deltax = abs(x2 - x1);        // The difference between the x's
	int deltay = abs(y2 - y1);        // The difference between the y's
	int x = x1;                       // Start x off at the first pixel
	int y = y1;                       // Start y off at the first pixel
	int xinc1, xinc2;
	int yinc1, yinc2;
	int den, num, numadd;
	int numpixels, curpixel;
	
	if (x2 >= x1)	xinc1 = 1,	xinc2 = 1;
	else			xinc1 = -1, xinc2 = -1;
	if (y2 >= y1) 	yinc1 = 1,	yinc2 = 1;
	else			yinc1 = -1,	yinc2 = -1;
	
	if (deltax >= deltay) {        // There is at least one x-value for every y-value
		xinc1 = 0;                  // Don't change the x when numerator >= denominator
		yinc2 = 0;                  // Don't change the y for every iteration
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;         // There are more x-values than y-values
	}
	else {                         // There is at least one y-value for every x-value
		xinc2 = 0;                  // Don't change the x for every iteration
		yinc1 = 0;                  // Don't change the y when numerator >= denominator
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;         // There are more y-values than x-values
	}
	
	for (curpixel = 0; curpixel <= numpixels; ++curpixel) {
		if (IsInCELL(x, y)) SetPixelCELL(x, y, c);             // Draw the current pixel
		num += numadd;              // Increase the numerator by the top of the fraction
		if (num >= den) {            // Check if numerator >= denominator
			num -= den;               // Calculate the new numerator value
			x += xinc1;               // Change the x as appropriate
			y += yinc1;               // Change the y as appropriate
		}
		x += xinc2;                 // Change the x as appropriate
		y += yinc2;                 // Change the y as appropriate
	}
}

template <typename T>
bool GridMap<T>::IsIntersectCELL (int x1, int y1, int &x2, int &y2, T cm)
{
	int deltax = abs(x2 - x1);        // The difference between the x's
	int deltay = abs(y2 - y1);        // The difference between the y's
	int x = x1;                       // Start x off at the first pixel
	int y = y1;                       // Start y off at the first pixel
	int xinc1, xinc2;
	int yinc1, yinc2;
	int den, num, numadd;
	int numpixels, curpixel;
	
	if (x2 >= x1)	xinc1 = 1,	xinc2 = 1;
	else			xinc1 = -1, xinc2 = -1;
	if (y2 >= y1) 	yinc1 = 1,	yinc2 = 1;
	else			yinc1 = -1,	yinc2 = -1;
	
	if (deltax >= deltay) {        // There is at least one x-value for every y-value
		xinc1 = 0;                  // Don't change the x when numerator >= denominator
		yinc2 = 0;                  // Don't change the y for every iteration
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;         // There are more x-values than y-values
	}
	else {                         // There is at least one y-value for every x-value
		xinc2 = 0;                  // Don't change the x for every iteration
		yinc1 = 0;                  // Don't change the y when numerator >= denominator
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;         // There are more y-values than x-values
	}
	
	for (curpixel = 0; curpixel <= numpixels; ++curpixel) {
		if (!IsInCELL(x, y)) break;

		if (cm <= GetPixelCELL (x, y)) {
			x2 = x;
			y2 = y;
			return true;
		}
		num += numadd;              // Increase the numerator by the top of the fraction
		if (num >= den) {            // Check if numerator >= denominator
			num -= den;               // Calculate the new numerator value
			x += xinc1;               // Change the x as appropriate
			y += yinc1;               // Change the y as appropriate
		}
		x += xinc2;                 // Change the x as appropriate
		y += yinc2;                 // Change the y as appropriate
	}

	return false;
}


template <typename T>
void GridMap<T>::ExpandObstacle (int bulge, T threshold, T newValue)
{
	const T mark = 1;
	int b = bulge;
	GridMap<T> *mapTemp1 = new GridMap<T> (_offsetX, _offsetY, _width, _height);
	GridMap<T> *mapTemp2 = new GridMap<T> (_offsetX, _offsetY, _width, _height);

	for (int ry=0; ry<_height; ++ry) {
		T pc = 0;
		for (int rx=0; rx<_width; ++rx) {
			T c = GetPixelCELL (rx, ry);
			if (pc < threshold && threshold <= c)		mapTemp1->DrawLineCELL (max (rx - b, 0), ry, rx, ry, mark);
			else if (threshold <= pc && c < threshold)	mapTemp1->DrawLineCELL (rx, ry, min (rx + b - 1, _width-1), ry, mark);
			if (threshold <= c)							mapTemp1->SetPixelCELL (rx, ry, mark);
			pc = c;
		}
	}
	for (int rx=0; rx<_width; ++rx) {
		T pc = 0;
		for (int ry=0; ry<_height; ++ry) {
			T c = mapTemp1->GetPixelCELL (rx, ry);
			if (mark != pc && mark == c) 		mapTemp2->DrawLineCELL (rx, max (ry - b, 0), rx, ry, mark);
			else if (mark == pc && mark != c)	mapTemp2->DrawLineCELL (rx, ry, rx, min (ry + b - 1, _width-1), mark);
			if (mark == c)						mapTemp2->SetPixelCELL (rx, ry, mark);
			pc = c;
		}
	}
	for (int ry=0; ry<_height; ++ry) {
		for (int rx=0; rx<_width; ++rx) {
			T c = GetPixelCELL (rx, ry);
			T tc = mapTemp2->GetPixelCELL (rx, ry);
			if (c < threshold && tc == mark) SetPixelCELL (rx, ry, newValue);
		}
	}
	delete mapTemp1;
	delete mapTemp2;
}
