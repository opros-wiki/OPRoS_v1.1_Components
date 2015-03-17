#ifndef _KINECT_DATA_TYPE_H_
#define _KINECT_DATA_TYPE_H_

#include <cstdint>
#include <vector>

#include <archive/opros_archive.h>
#include <opros_smart_ptr.h>

using std::vector;

class ImageFrame
{
public:
	enum Type{BGR8 = 0, RGB8 = 1};

	ImageFrame(unsigned int width_ = 0, unsigned int height_ = 0
		, Type type_ = RGB8, opros::smart_ptr<const vector<uint8_t>> data_ = NULL)
		: width(width_), height(height_), type(type_), data(data_)
	{}

	unsigned int width;
	unsigned int height;
	Type type;
	opros::smart_ptr<const vector<uint8_t>> data;

	void save(opros::archive::oarchive& ar, const unsigned int)
	{
		bool isValidData = (data.isNULL() == false);

		ar << width << height << type << isValidData;

		if (isValidData)
			ar << *data;
	}

	void load(opros::archive::iarchive& ar, const unsigned int)
	{
		bool isValidData = false;

		ar >> width >> height >> type >> isValidData;		

		if (isValidData)
		{
			vector<uint8_t>* pData = new vector<uint8_t>();
			ar >> *pData;
			data = pData;
		}
		else
		{
			data = NULL;
		}		
	}
};
inline std::string typeName(ImageFrame *p)
{
	return std::string("ImageFrame");
}

class DepthFrame
{
public:
	DepthFrame(unsigned int width_ = 0, unsigned int height_ = 0
		, opros::smart_ptr<const vector<uint16_t>> data_ = NULL)
		: width(width_), height(height_), data(data_)
	{}

	unsigned int width;
	unsigned int height;
	opros::smart_ptr<const vector<uint16_t>> data;	

	void save(opros::archive::oarchive& ar, const unsigned int)
	{
		bool isValidData = (data.isNULL() == false);

		ar << width << height << isValidData;

		if (isValidData)
			ar << *data;
	}

	void load(opros::archive::iarchive& ar, const unsigned int)
	{
		bool isValidData = false;

		ar >> width >> height >> isValidData;		

		if (isValidData)
		{
			vector<uint16_t>* pData = new vector<uint16_t>();
			ar >> *pData;
			data = pData;
		}
		else
		{
			data = NULL;
		}		
	}
};
inline std::string typeName(DepthFrame *p)
{
	return std::string("DepthFrame");
}

class Position
{
public:
	Position(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f)
		: x(x_), y(y_), z(z_)
	{}

	float x;
	float y; 
	float z;

	void save(opros::archive::oarchive& ar, const unsigned int)
	{
		ar << x << y << z;
	}

	void load(opros::archive::iarchive& ar, const unsigned int)
	{
		ar >> x >> y >> z;
	}
};
inline std::string typeName(Position *p)
{
	return std::string("Position");
}


class Skeleton
{
public:
	enum Result {NOT_TRACKED = -1, POSITION_ONLY = 1, TRACKED = 2};
	enum JointIndex {HIP_CENTER = 0, SPINE=1, SHOULDER_CENTER=2, HEAD=3, SHOULDER_LEFT=4, ELBOW_LEFT=5, WRIST_LEFT=6,
		HAND_LEFT=7, SHOULDER_RIGHT=8, ELBOW_RIGHT=9, WRIST_RIGHT=10,HAND_RIGHT=11, HIP_LEFT=12, KNEE_LEFT=13, ANKLE_LEFT=14,
		FOOT_LEFT=15, HIP_RIGHT=16, KNEE_RIGHT=17, ANKLE_RIGHT=18,FOOT_RIGHT=19 ,JOINT_COUNT = 20};

	Skeleton(unsigned long userID_ = 0, Result result_ = NOT_TRACKED)
		: userID(userID_), result(result_)
	{}

	unsigned long userID;
	Result result;

	Position position;
	Position joints[JOINT_COUNT];

	void save(opros::archive::oarchive& ar, const unsigned int)
	{
		ar << userID << result << position << joints;
	}

	void load(opros::archive::iarchive& ar, const unsigned int)
	{
		ar >> userID >> result >> position >> joints;
	}
};
inline std::string typeName(Skeleton *p)
{
	return std::string("Skeleton");
}


class KinectEvent
{
public:
	enum State{NO_USER = 0, USER_IN = 1, USER_OUT = 2};

	KinectEvent(State state_ = NO_USER)
		: state(state_)
	{}

	State state; 
	vector<unsigned long> userIDs;

	void save(opros::archive::oarchive& ar, const unsigned int)
	{
		ar << state << userIDs;
	}

	void load(opros::archive::iarchive& ar, const unsigned int)
	{
		ar >> state >> userIDs;
	}
};
inline std::string typeName(KinectEvent *p)
{
	return std::string("KinectEvent");
}

#endif

