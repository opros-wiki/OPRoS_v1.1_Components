//////////////////////////////////////////////////////
// Vector and Matrix files for Robotics applications
// 
// Copyright (c) 2002-2010. All Rights Reserved.
// Division of Applied Robot Technology, KITECH
// Web: http://www.orobot.net
// Written by KwangWoong Yang<page365@gmail.com>
//
// Collection of Mathematical functions

#pragma	once

#include "matrix.h"
#include <assert.h>

class dMatrix : public matrix<double> 
{
public:
	dMatrix (const dMatrix &m): matrix<double> (m) { }
	dMatrix (const matrix<double> &m): matrix<double> (m) { }
	dMatrix (size_t row = 1, size_t col = 1): matrix<double> (row, col) { }

	size_t rowno () const { return RowNo(); }
	size_t colno () const { return ColNo(); }

	size_t size()   const { return RowNo()*ColNo(); }
	void resize(size_t row, size_t col) { SetSize (row, col); }

	void unit () { Unit (); }
	void zero () { Null (); }

	dMatrix operator = (const dMatrix &m)
	{
		if (this != &m) {
			matrix<double>::operator = (m);
		}
		return *this;
	}
};

class dVector : public matrix<double> 
{
public:
	dVector (const dVector &m): matrix<double> (m) { }
	dVector (const matrix<double> &m): matrix<double> (m) { }
	dVector (size_t size = 3): matrix<double> (size, 1) { }

	size_t size()   const { return RowNo()*ColNo(); }
	void resize(size_t size) { SetSize (size, 1); }

	void zero () { Null (); }

	double operator[] (size_t i)  const 
	{
		assert (ColNo() == 1);

		return (*this)(i,0);
	}

	double& operator[] (size_t i)
	{
		assert (ColNo() == 1);

		return (*this)(i,0);
	}

	dVector operator = (const dVector &m)
	{
		if (this != &m) {
			matrix<double>::operator = (m);
		}
		return *this;
	}
};


// dVector의 내적(dot product)을 계산한다.
inline double Dot (const dVector &v1, const dVector &v2)
{
	if(v1.size() != 3 || v2.size() != 3) 
		assert (0 && "ERROR: Dot(): Inconsistent vector size in Inner Product !");

	double	v = 0.;
	for(unsigned int i=0; i<3; ++i)
		v += v1[i] * v2[i];

	return v;
}

// dVector의 외적(cross product)을 계산한다.
inline dVector Cross (const dVector &v1, const dVector &v2)
{
	if(v1.size() != 3 || v2.size() != 3) 
		assert (0 && "ERROR: Cross(): dVector dimension should be 3 in Cross Product !");

	dVector v(3);
	v[0] = v1[1]*v2[2] - v1[2]*v2[1];
	v[1] = v1[2]*v2[0] - v1[0]*v2[2];
	v[2] = v1[0]*v2[1] - v1[1]*v2[0];

	return v;
}

// Vector의 크기를 2-norm으로 계산한다
inline double Norm2 (const dVector &v)
{
	double s = 0.;
	for (int i=0; i<(int)v.size(); ++i) {
		s += v[i]*v[i];
	}
	return sqrt(s);
}

inline void Set (dVector &V, double v0, double v1, double v2)
{
	if (V.size() != 3) {
		V.resize (3);
	}

	V[0] = v0;
	V[1] = v1;
	V[2] = v2;
}

inline dMatrix RotationMatrix (double r, double p, double y)
{
	double sr = sin(r),		cr = cos(r);
	double sp = sin(p),		cp = cos(p);
	double sy = sin(y),		cy = cos(y);

	dMatrix A(3,3);

	// 변환 메트릭스: Rx(r) x Ry(p) x Rz(y)
	A(0, 0) =  cp*cy;				A(0, 1) = -cp*sy;				A(0, 2) =  sp;		
	A(1, 0) =  sr*sp*cy + cr*sy;	A(1, 1) = -sr*sp*sy + cr*cy;	A(1, 2) = -sr*cp;	
	A(2, 0) = -cr*sp*cy + sr*sy;	A(2, 1) =  cr*sp*sy + sr*cy;	A(2, 2) =  cr*cp;	

	return A;
}

