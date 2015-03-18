
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0 [Symbol])
 *  
 */
#ifndef _Coodinates_DATATYPE_H
#define _Coodinates_DATATYPE_H
#include <archive/opros_archive.h>



/*
** 
** 
*/
class Coodinates
{
protected:
	friend class opros::archive::access;
	double m_x;
	double m_y;
	double m_z;
	
public:
	void save(opros::archive::oarchive &ar, const unsigned int) 
	{
		ar << m_x;
		ar << m_y;
		ar << m_z;
		

	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> m_x;
		ar >> m_y;
		ar >> m_z;
		

	}
public:
	Coodinates()
	{
	}

	Coodinates(const double& in_x,const double& in_y,const double& in_z)
	{
		m_x = in_x;
		m_y = in_y;
		m_z = in_z;
		
	}

	double& getX()
	{
		return m_x;
	}

	void setX(const double& in_x)
	{
		m_x = in_x;
	}

	double& getY()
	{
		return m_y;
	}

	void setY(const double& in_y)
	{
		m_y = in_y;
	}

	double& getZ()
	{
		return m_z;
	}

	void setZ(const double& in_z)
	{
		m_z = in_z;
	}

	

};

inline std::string typeName(Coodinates *p)
{
	return "Coodinates";
}
#endif
