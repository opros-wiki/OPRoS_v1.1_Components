/*************************************************************

 file: ApiTypes.h
 author: E.C. Shin
 begin: January 31 2011
 copyright: (c) 2011 KITECH, OPRoS
 email: unchol@kitech.re.kr

***************************************************************

OPRoS source code is provided under a dual license mode:
 LGPL and OPRoS individually.

LGPL: You can redistribute it and/or modify it under the terms
 of the Less GNU General Public License as published by the Free
 Software Foundation, either version 3 of the License.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 Less GNU General Public License for more details. 

 You should have received a copy of the Less GNU General Public
 License along with this program. If not, see 
 <http://www.gnu.org/licenses/>.

OPRoS individual license: An individual license is a license for
 modifying the source code of OPRoS and distiributing it in a
 closed format for commercial purposes.

 If you are interested in this option, please see 
 <http://www.opros.or.kr>.

This license policy may be changed without prior notice.

***************************************************************/

#ifndef	_API_TYPES_H_
#define	_API_TYPES_H_

#include <assert.h>
#include <vector>
#include <valarray>
#include <deque>
#include "OprosMath.h"
#include "../archive/opros_archive.h"
#include "../archive/opros_access.h"

using namespace std;

class ObjectPosition
{
public:
	ObjectPosition() {
		x = 0.0;		y = 0.0;		z = 0.0;
		roll = 0.0;		pitch = 0.0;	yaw = 0.0;
	}
	ObjectPosition(double x, double y, double z, double roll, double pitch, double yaw) {
		this->x = x;			this->y = y;			this->z = z;
		this->roll = roll;		this->pitch = pitch;	this->yaw = yaw;
	}
	virtual ~ObjectPosition() { };

	friend class opros::archive::access;
	
	double x;
	double y;
	double z;
	double roll;
	double pitch;
	double yaw;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << x;			ar << y;			ar << z;
		ar << roll;			ar << pitch;		ar << yaw;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> x;			ar >> y;			ar >> z;
		ar >> roll;			ar >> pitch;		ar >> yaw;
	}

	ObjectPosition operator=(const ObjectPosition &objectPosition)
	{
		this->x = objectPosition.x;
		this->y = objectPosition.y;
		this->z = objectPosition.z;
		this->roll = objectPosition.roll;
		this->pitch = objectPosition.pitch;
		this->yaw = objectPosition.yaw;

		return *this;
	}

	ObjectPosition operator+(const ObjectPosition &objectPosition)
	{
		this->x += objectPosition.x;
		this->y += objectPosition.y;
		this->z += objectPosition.z;
		this->roll += objectPosition.roll;
		this->pitch += objectPosition.pitch;
		this->yaw += objectPosition.yaw;

		return *this;
	}

	ObjectPosition operator-(const ObjectPosition &objectPosition)
	{
		this->x -= objectPosition.x;
		this->y -= objectPosition.y;
		this->z -= objectPosition.z;
		this->roll -= objectPosition.roll;
		this->pitch -= objectPosition.pitch;
		this->yaw -= objectPosition.yaw;

		return *this;
	}

	ObjectPosition operator*(const ObjectPosition &objectPosition)
	{
		this->x *= objectPosition.x;
		this->y *= objectPosition.y;
		this->z *= objectPosition.z;
		this->roll *= objectPosition.roll;
		this->pitch *= objectPosition.pitch;
		this->yaw *= objectPosition.yaw;

		return *this;
	}

	ObjectPosition operator/(const ObjectPosition &objectPosition)
	{
		this->x /= objectPosition.x;
		this->y /= objectPosition.y;
		this->z /= objectPosition.z;
		this->roll /= objectPosition.roll;
		this->pitch /= objectPosition.pitch;
		this->yaw /= objectPosition.yaw;

		return *this;
	}
};

inline std::string typeName(ObjectPosition *p)	{ return "ObjectPosition"; }

class ObjectRotation
{
public:
	ObjectRotation() {
		roll = 0.0;		pitch = 0.0;	yaw = 0.0;
	}
	ObjectRotation(double roll, double pitch, double yaw) {
		this->roll = roll;		this->pitch = pitch;	this->yaw = yaw;
	}
	virtual ~ObjectRotation() { };

	friend class opros::archive::access;
	
	double roll;
	double pitch;
	double yaw;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << roll;			ar << pitch;		ar << yaw;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> roll;			ar >> pitch;		ar >> yaw;
	}

	ObjectRotation operator=(const ObjectRotation &objectRotation)
	{
		this->roll = objectRotation.roll;
		this->pitch = objectRotation.pitch;
		this->yaw = objectRotation.yaw;

		return *this;
	}

	ObjectRotation operator+(const ObjectRotation &objectRotation)
	{
		this->roll += objectRotation.roll;
		this->pitch += objectRotation.pitch;
		this->yaw += objectRotation.yaw;

		return *this;
	}

	ObjectRotation operator-(const ObjectRotation &objectRotation)
	{
		this->roll -= objectRotation.roll;
		this->pitch -= objectRotation.pitch;
		this->yaw -= objectRotation.yaw;

		return *this;
	}

	ObjectRotation operator*(const ObjectRotation &objectRotation)
	{
		this->roll *= objectRotation.roll;
		this->pitch *= objectRotation.pitch;
		this->yaw *= objectRotation.yaw;

		return *this;
	}

	ObjectRotation operator/(const ObjectRotation &objectRotation)
	{
		this->roll /= objectRotation.roll;
		this->pitch /= objectRotation.pitch;
		this->yaw /= objectRotation.yaw;

		return *this;
	}
};

inline std::string typeName(ObjectRotation *p)	{ return "ObjectRotation"; }

class ObjectPosture
{
public:
    friend class opros::archive::access;
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << pRate;			ar << qRate;			ar << rRate;
		ar << xAcceleration;	ar << yAcceleration;	ar << zAcceleration;
		ar << roll;				ar << pitch;			ar << yaw;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> pRate;			ar >> qRate;			ar >> rRate;
		ar >> xAcceleration;	ar >> yAcceleration;	ar >> zAcceleration;
		ar >> roll;				ar >> pitch;			ar >> yaw;
	}
    
    double pRate;
    double qRate;
    double rRate;
    double xAcceleration;
    double yAcceleration;
    double zAcceleration;
    double roll;
    double pitch;
    double yaw;
    
	ObjectPosture operator=(const ObjectPosture &objectPosture)
    {
        this->pRate = objectPosture.pRate;
        this->qRate = objectPosture.qRate;
        this->rRate = objectPosture.rRate;
        this->xAcceleration = objectPosture.xAcceleration;
        this->yAcceleration = objectPosture.yAcceleration;
        this->zAcceleration = objectPosture.zAcceleration;
        this->roll = objectPosture.roll;
        this->pitch = objectPosture.pitch;
        this->yaw = objectPosture.yaw;
        
        return *this;
    }
    
    ObjectPosture operator+(const ObjectPosture &objectPosture)
    {
        this->pRate += objectPosture.pRate;
        this->qRate += objectPosture.qRate;
        this->rRate += objectPosture.rRate;
        this->xAcceleration += objectPosture.xAcceleration;
        this->yAcceleration += objectPosture.yAcceleration;
        this->zAcceleration += objectPosture.zAcceleration;
        this->roll += objectPosture.roll;
        this->pitch += objectPosture.pitch;
        this->yaw += objectPosture.yaw;
        
        return *this;
    }
    
    ObjectPosture operator-(const ObjectPosture &objectPosture)
    {
        this->pRate -= objectPosture.pRate;
        this->qRate -= objectPosture.qRate;
        this->rRate -= objectPosture.rRate;
        this->xAcceleration -= objectPosture.xAcceleration;
        this->yAcceleration -= objectPosture.yAcceleration;
        this->zAcceleration -= objectPosture.zAcceleration;
        this->roll -= objectPosture.roll;
        this->pitch -= objectPosture.pitch;
        this->yaw -= objectPosture.yaw;
        
        return *this;
    }
    
    ObjectPosture operator*(const ObjectPosture &objectPosture)
    {
        this->pRate *= objectPosture.pRate;
        this->qRate *= objectPosture.qRate;
        this->rRate *= objectPosture.rRate;
        this->xAcceleration *= objectPosture.xAcceleration;
        this->yAcceleration *= objectPosture.yAcceleration;
        this->zAcceleration *= objectPosture.zAcceleration;
        this->roll *= objectPosture.roll;
        this->pitch *= objectPosture.pitch;
        this->yaw *= objectPosture.yaw;
        
        return *this;
    }
    
    ObjectPosture operator/(const ObjectPosture &objectPosture)
    {
        this->pRate /= objectPosture.pRate;
        this->qRate /= objectPosture.qRate;
        this->rRate /= objectPosture.rRate;
        this->xAcceleration /= objectPosture.xAcceleration;
        this->yAcceleration /= objectPosture.yAcceleration;
        this->zAcceleration /= objectPosture.zAcceleration;
        this->roll /= objectPosture.roll;
        this->pitch /= objectPosture.pitch;
        this->yaw /= objectPosture.yaw;
        
        return *this;
    }
};

inline std::string typeName(ObjectPosture *p)	{ return "ObjectPosture"; }

class ObjectLocation// : public boost::operators<ObjectLocation>
{
public:
	double x, y, theta;

public:
	friend class opros::archive::access;
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << x;			ar << y;			ar << theta;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> x;			ar >> y;			ar >> theta;
	}

public:
	ObjectLocation () { x = 0.0; y = 0.0; theta = 0.0; }
	ObjectLocation (double len, double th_) : x (len * cos (th_)), y (len * sin (th_)), theta (th_) { }
	ObjectLocation (double x_, double y_, double th_) : x (x_), y (y_), theta (th_) { }

public: 
	ObjectLocation & operator += (const ObjectLocation &p) { x += p.x; y += p.y; theta += p.theta; return *this; }
	ObjectLocation & operator -= (const ObjectLocation &p) { x -= p.x; y -= p.y; theta -= p.theta; return *this; }
	ObjectLocation & operator *= (const ObjectLocation &p) { x *= p.x; y *= p.y; theta *= p.theta; return *this; }
	ObjectLocation & operator /= (const ObjectLocation &p) { x /= p.x; y /= p.y; theta /= p.theta; return *this; }
	ObjectLocation & operator *= (double d)                { x *= d;   y *= d;   theta *= d;    return *this; }

	ObjectLocation &Rotate (double rad)
	{
		double c = cos (rad);
		double s = sin (rad);
		double x_ = x * c - y * s;
		double y_ = x * s + y * c;
		x = x_;
		y = y_;
		theta += rad;
		return *this;
	}

	ObjectLocation &Translate (double x_, double y_)
	{
		x += x_;
		y += y_;
		return *this;
	}

	ObjectLocation &Transform (const ObjectLocation &p)
	{
		Rotate (p.theta);
		Translate (p.x, p.y);
		return *this;
	}

	double Atan ()   { return atan2 (y, x); }
	double Length () { return sqrt (x*x + y*y); }
	double Distance (ObjectLocation &p) { double dx = p.x - x, dy = p.y - y; return sqrt (dx * dx + dy * dy); }
};
inline ObjectLocation diff (const ObjectLocation &v1, const ObjectLocation &v2) { return ObjectLocation (v1.x - v2.x, v1.y - v2.y, DeltaRad (v1.theta, v2.theta)); }

inline std::string typeName(ObjectLocation *p)	{ return "ObjectLocation"; }

class JoystickData
{
public:
	JoystickData(void)
	{
	}
	virtual ~JoystickData(void) { }

	friend class opros::archive::access;
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << x;
		ar << y;			
		ar << z;			
		ar << button;		
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> x;
		ar >> y;	
		ar >> z;	
		ar >> button;
	}
    
	JoystickData operator=(const JoystickData &joystickData)
	{
		this->x = joystickData.x;
		this->y = joystickData.y;
		this->z = joystickData.z;
		this->button = joystickData.button;
						
		return *this;
	}

	double x;
	double y;
	double z;
	vector<bool> button;
};

inline std::string typeName(JoystickData *p)	{ return "JoystickData"; }

class GpsData
{
public:
	GpsData(void)
	{
	}
	virtual ~GpsData(void) { }

	friend class opros::archive::access;
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << latitude;			ar << longitude;
		ar << time;				ar << velocity;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> latitude;			ar >> longitude;
		ar >> time;				ar >> velocity;
	}
    
	GpsData operator=(const GpsData &gpsData)
	{
		this->latitude = gpsData.latitude;
		this->longitude = gpsData.longitude;
		this->time = gpsData.time;
		this->velocity = gpsData.velocity;
						
		return *this;
	}

	double latitude;
	double longitude;
	string time;
	double velocity;
};

inline std::string typeName(GpsData *p)	{ return "GpsData"; }

class VisionRecognitionResult
{
public:
	VisionRecognitionResult(void)
	{
	}
	virtual ~VisionRecognitionResult(void) { }

	friend class opros::archive::access;
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << name;
		ar << point1X;				ar << point1Y;
		ar << point2X;				ar << point2Y;
		ar << point3X;				ar << point3Y;
		ar << point4X;				ar << point4Y;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> name;
		ar >> point1X;				ar >> point1Y;
		ar >> point2X;				ar >> point2Y;
		ar >> point3X;				ar >> point3Y;
		ar >> point4X;				ar >> point4Y;
	}
    
	VisionRecognitionResult operator=(const VisionRecognitionResult &visionRecognitionData)
	{
		this->name = visionRecognitionData.name;
		this->point1X = visionRecognitionData.point1X;
		this->point1Y = visionRecognitionData.point1Y;
		this->point2X = visionRecognitionData.point2X;
		this->point2Y = visionRecognitionData.point2Y;
		this->point3X = visionRecognitionData.point3X;
		this->point3Y = visionRecognitionData.point3Y;
		this->point4X = visionRecognitionData.point4X;
		this->point4Y = visionRecognitionData.point4Y;

		return *this;
	}

	std::string name;
	int point1X;
	int point1Y;
	int point2X;
	int point2Y;
	int point3X;
	int point3Y;
	int point4X;
	int point4Y;
};

inline std::string typeName(VisionRecognitionResult *p)	{ return "VisionRecognitionResult"; }


class ColorRange
{
public:
	ColorRange(void)
	{
	}
	virtual ~ColorRange(void) { }

	friend class opros::archive::access;

	template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & name & min1 & max1 & min2 & max2 & min3 & max3;
    }
    
	ColorRange operator=(const ColorRange &colorRange)
	{
		this->name = colorRange.name;
		this->min1 = colorRange.min1;
		this->max1 = colorRange.max1;
		this->min2 = colorRange.min2;
		this->max2 = colorRange.max2;
		this->min3 = colorRange.min3;
		this->max3 = colorRange.max3;

		return *this;
	}

	std::string name;
	unsigned char	min1;
	unsigned char	max1;
	unsigned char	min2;
	unsigned char	max2;
	unsigned char	min3;
	unsigned char	max3;
};

inline std::string typeName(ColorRange *p)	{ return "ColorRange"; }

class CartesianCoordinate
{
public:
	CartesianCoordinate() {
		x = 0.0;		y = 0.0;		z = 0.0;
		roll = 0.0;		pitch = 0.0;	yaw = 0.0;
	}
	CartesianCoordinate(double x, double y, double z, double roll, double pitch, double yaw) {
		this->x = x;				this->y = y;				this->z = z;
		this->roll = roll;		this->pitch = pitch;		this->yaw = yaw;
	}
	virtual ~CartesianCoordinate() { };

	friend class opros::archive::access;
	
	double x, y, z;
	double roll, pitch, yaw;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << x;			ar << y;			ar << z;
		ar << roll;			ar << pitch;		ar << yaw;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> x;			ar >> y;			ar >> z;
		ar >> roll;			ar >> pitch;		ar >> yaw;
	}

	CartesianCoordinate operator=(const CartesianCoordinate &cartesianCoordinate)
	{
		this->x = cartesianCoordinate.x;
		this->y = cartesianCoordinate.y;
		this->z = cartesianCoordinate.z;
		this->roll = cartesianCoordinate.roll;
		this->pitch = cartesianCoordinate.pitch;
		this->yaw = cartesianCoordinate.yaw;

		return *this;
	}

	CartesianCoordinate operator+(const CartesianCoordinate &cartesianCoordinate)
	{
		this->x += cartesianCoordinate.x;
		this->y += cartesianCoordinate.y;
		this->z += cartesianCoordinate.z;
		this->roll += cartesianCoordinate.roll;
		this->pitch += cartesianCoordinate.pitch;
		this->yaw += cartesianCoordinate.yaw;

		return *this;
	}

	CartesianCoordinate operator-(const CartesianCoordinate &cartesianCoordinate)
	{
		this->x -= cartesianCoordinate.x;
		this->y -= cartesianCoordinate.y;
		this->z -= cartesianCoordinate.z;
		this->roll -= cartesianCoordinate.roll;
		this->pitch -= cartesianCoordinate.pitch;
		this->yaw -= cartesianCoordinate.yaw;

		return *this;
	}
};

inline std::string typeName(CartesianCoordinate *p)	{ return "CartesianCoordinate"; }

class ForceTorqueSensorData
{
public:
	ForceTorqueSensorData() {
		fx = 0.0;	fy = 0.0;	fz = 0.0;
		tx = 0.0;	ty = 0.0;	tz = 0.0;
	}
	ForceTorqueSensorData(double fx, double fy, double fz, double tx, double ty, double tz) {
		this->fx = fx;		this->fy = fy;		this->fz = fz;
		this->tx = tx;		this->ty = ty;		this->tz = tz;
	}
	virtual ~ForceTorqueSensorData() { };

	friend class opros::archive::access;
	
	double fx, fy, fz;
	double tx, ty, tz;
	
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << fx;		ar << fy;		ar << fz;
		ar << tx;		ar << ty;		ar << tz;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> fx;		ar >> fy;		ar >> fz;
		ar >> tx;		ar >> ty;		ar >> tz;
	}

	ForceTorqueSensorData operator=(const ForceTorqueSensorData &ftSensorData)
	{
		this->fx = ftSensorData.fx;
		this->fy = ftSensorData.fy;
		this->fz = ftSensorData.fz;
		this->tx = ftSensorData.tx;
		this->ty = ftSensorData.ty;
		this->tz = ftSensorData.tz;

		return *this;
	}

	ForceTorqueSensorData operator+(const ForceTorqueSensorData &ftSensorData)
	{
		this->fx += ftSensorData.fx;
		this->fy += ftSensorData.fy;
		this->fz += ftSensorData.fz;
		this->tx += ftSensorData.tx;
		this->ty += ftSensorData.ty;
		this->tz += ftSensorData.tz;

		return *this;
	}

	ForceTorqueSensorData operator-(const ForceTorqueSensorData &ftSensorData)
	{
		this->fx -= ftSensorData.fx;
		this->fy -= ftSensorData.fy;
		this->fz -= ftSensorData.fz;
		this->tx -= ftSensorData.tx;
		this->ty -= ftSensorData.ty;
		this->tz -= ftSensorData.tz;

		return *this;
	}
};

inline std::string typeName(ForceTorqueSensorData *p)	{ return "ForceTorqueSensorData"; }

class CanData
{
public:
	CanData() {
		id = 0;
		data.clear();
	}
	CanData(long id, vector<unsigned char> data) {
		this->id = id;
		this->data = data;
	}
	virtual ~CanData() { };

	friend class opros::archive::access;
	
	long id;
	vector<unsigned char> data;
	
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << id;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> id;		ar >> data;
	}

	CanData operator=(const CanData &canData)
	{
		this->id = canData.id;
		this->data = canData.data;
		
		return *this;
	}
};

inline std::string typeName(CanData *p)	{ return "CanData"; }

#endif	//	_API_TYPES_H_
