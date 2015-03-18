
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0 [Symbol])
 *  
 */
#ifndef _PositionDataType_DATATYPE_H
#define _PositionDataType_DATATYPE_H
#include <archive/opros_archive.h>


#include <string>
            

/*
** 
** 
*/
class PositionDataType
{
protected:
	friend class opros::archive::access;
	std::string m_name;
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	double m_radian;
	
public:
	void save(opros::archive::oarchive &ar, const unsigned int) 
	{
		ar << m_name;
		ar << m_x;
		ar << m_y;
		ar << m_width;
		ar << m_height;
		ar << m_radian;
		

	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> m_name;
		ar >> m_x;
		ar >> m_y;
		ar >> m_width;
		ar >> m_height;
		ar >> m_radian;
		

	}
public:
	PositionDataType()
	{
	}

	PositionDataType(const std::string& in_name,const int& in_x,const int& in_y,const int& in_width,const int& in_height,const double& in_radian)
	{
		m_name = in_name;
		m_x = in_x;
		m_y = in_y;
		m_width = in_width;
		m_height = in_height;
		m_radian = in_radian;
		
	}

	std::string& getName()
	{
		return m_name;
	}

	void setName(const std::string& in_name)
	{
		m_name = in_name;
	}

	int& getX()
	{
		return m_x;
	}

	void setX(const int& in_x)
	{
		m_x = in_x;
	}

	int& getY()
	{
		return m_y;
	}

	void setY(const int& in_y)
	{
		m_y = in_y;
	}

	int& getWidth()
	{
		return m_width;
	}

	void setWidth(const int& in_width)
	{
		m_width = in_width;
	}

	int& getHeight()
	{
		return m_height;
	}

	void setHeight(const int& in_height)
	{
		m_height = in_height;
	}

	double& getRadian()
	{
		return m_radian;
	}

	void setRadian(const double& in_radian)
	{
		m_radian = in_radian;
	}

	

};

inline std::string typeName(PositionDataType *p)
{
	return "PositionDataType";
}
#endif
