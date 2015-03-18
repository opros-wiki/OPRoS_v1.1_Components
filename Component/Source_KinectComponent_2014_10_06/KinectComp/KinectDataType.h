#ifndef __KINECT_DATA_TYPE_H__
#define __KINECT_DATA_TYPE_H__

#include <cstdint>
#include <vector>
#include <memory>

#include <archive/opros_archive.h>

class ImageFrame
{
public:
	enum Type{BGR8 = 0, RGB8 = 1};

	ImageFrame(uint32_t width_ = 0, uint32_t height_ = 0
		, Type type_ = RGB8, std::shared_ptr<std::vector<uint8_t> const> data_ = NULL)
		: width(width_), height(height_), type(type_), data(data_)
	{}

	uint32_t width;
	uint32_t height;
	Type type;
	std::shared_ptr<std::vector<uint8_t> const> data;

	inline bool isValid()
	{
		return data.get() != NULL && width != 0 && height != 0;
	}

	void save(opros::archive::oarchive& ar, unsigned int const)
	{
		bool isValidData = isValid();

		ar << width << height << type << isValidData;

		if (isValidData)
			ar << *data;
	}

	void load(opros::archive::iarchive& ar, unsigned int const)
	{
		bool isValidData = false;

		ar >> width >> height >> type >> isValidData;		

		if (isValidData)
		{
			std::unique_ptr<std::vector<uint8_t>> pData(new std::vector<uint8_t>());
			ar >> *pData;

			data.reset(pData.release());
		}
		else
		{
			data.reset();
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
	DepthFrame(uint32_t width_ = 0, unsigned int height_ = 0
		, std::shared_ptr<std::vector<uint16_t> const> data_ = NULL)
		: width(width_), height(height_), data(data_)
	{}

	uint32_t width;
	uint32_t height;
	std::shared_ptr<std::vector<uint16_t> const> data;	

	inline bool isValid()
	{
		return data.get() != NULL && width != 0 && height != 0;
	}

	void save(opros::archive::oarchive& ar, unsigned int const)
	{
		bool isValidData = isValid();

		ar << width << height << isValidData;

		if (isValidData)
			ar << *data;
	}

	void load(opros::archive::iarchive& ar, unsigned int const)
	{
		bool isValidData = false;

		ar >> width >> height >> isValidData;		

		if (isValidData)
		{
			std::unique_ptr<std::vector<uint16_t>> pData(new std::vector<uint16_t>());
			ar >> *pData;

			data.reset(pData.release());
		}
		else
		{
			data.reset();
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

	void save(opros::archive::oarchive& ar, unsigned int const)
	{
		ar << x << y << z;
	}

	void load(opros::archive::iarchive& ar, unsigned int const)
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

	Skeleton(uint32_t userID_ = 0, Result result_ = NOT_TRACKED)
		: userID(userID_), result(result_)
	{}

	uint32_t userID;
	Result result;

	Position position;
	Position joints[JOINT_COUNT];

	void save(opros::archive::oarchive& ar, unsigned int const)
	{
		ar << userID << result << position << joints;
	}

	void load(opros::archive::iarchive& ar, unsigned int const)
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
	std::vector<uint32_t> userIDs;

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

