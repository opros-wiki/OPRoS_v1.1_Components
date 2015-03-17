/*************************************************************

 file: Property.h
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

#ifndef	_PROPERTY_H_
#define	_PROPERTY_H_

#include <map>
#include <string>

#include "ArchiveFactory.h"

using namespace std;

class Property
{
public:
	Property(void) { };
	virtual ~Property(void) { };

protected:
	friend class opros::archive::access;

	map<string, string> data;

	
public:
	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> data;
	}

	Property operator=(const Property &data)
	{
		this->data = data.data;
		
		return *this;
	}

	void SetProperty(map<string, string> data)
	{
		this->data = data;
	}

	map<string, string> GetProperty(void)
	{
		return data;
	}

	void SetValue(string name, string value)
	{
		data[name] = value;
	}

	std::string GetValue(string name)
	{
		return data[name];
	}

	bool FindName(string name)
	{
		if(data.find(name) != data.end()) {
			return true;
		}

		return false;
	}
};

inline std::string typeName(Property *p)	{ return "Property"; }

#endif	//	_PROPERTY_H_