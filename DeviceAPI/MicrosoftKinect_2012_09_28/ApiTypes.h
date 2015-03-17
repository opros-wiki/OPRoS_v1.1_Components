/*************************************************************

 file: ApiTypes.h
 author: E.C. Shin
 begin: December 31 2012
 copyright: (c) 2012 KITECH, OPRoS
 email: uncholshin@gmail.com

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

#pragma once

#include <string>
#include <vector>

#include "../archive/opros_archive.h"
#include "../archive/opros_access.h"
#include "ReturnType.h"

#include <cstdint>

typedef char char_t;
typedef float float32_t;
typedef double float64_t;

namespace OPRoS
{

class Bool
{
public:
	Bool(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	Bool(bool data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Bool(void) {
	}

	ReturnType status;
	bool data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Bool operator=(const Bool &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Bool *p)	{ return "Bool"; }

class Char
{
public:
	Char(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	Char(char_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Char(void) {
	}

	ReturnType status;
	char_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Char operator=(const Char &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Char *p)	{ return "Char"; }

class Int8
{
public:
	Int8(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	Int8(int8_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int8(void) {
	}

	ReturnType status;
	int8_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int8 operator=(const Int8 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int8 *p)	{ return "Int8"; }

class UInt8
{
public:
	UInt8(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	UInt8(uint8_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt8(void) {
	}

	ReturnType status;
	uint8_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt8 operator=(const UInt8 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt8 *p)	{ return "UInt8"; }

class Int16
{
public:
	Int16(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	Int16(int16_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int16(void) {
	}

	ReturnType status;
	int16_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int16 operator=(const Int16 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int16 *p)	{ return "Int16"; }

class UInt16
{
public:
	UInt16(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	UInt16(uint16_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt16(void) {
	}

	ReturnType status;
	uint16_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt16 operator=(const UInt16 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt16 *p)	{ return "UInt16"; }

class Int32
{
public:
	Int32(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	Int32(int32_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int32(void) {
	}

	ReturnType status;
	int32_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int32 operator=(const Int32 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int32 *p)	{ return "Int32"; }

class UInt32
{
public:
	UInt32(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	UInt32(uint32_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt32(void) {
	}

	ReturnType status;
	uint32_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt32 operator=(const UInt32 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt32 *p)	{ return "UInt32"; }

class Int64
{
public:
	Int64(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	Int64(int64_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int64(void) {
	}

	ReturnType status;
	int64_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int64 operator=(const Int64 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int64 *p)	{ return "Int64"; }

class UInt64
{
public:
	UInt64(void) {
		status = OPROS_SUCCESS;
		data = 0;
	}
	UInt64(uint64_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt64(void) {
	}

	ReturnType status;
	uint64_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt64 operator=(const UInt64 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt64 *p)	{ return "UInt64"; }

class Float32
{
public:
	Float32(void) {
		status = OPROS_SUCCESS;
		data = 0.0;
	}
	Float32(float32_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Float32(void) {
	}

	ReturnType status;
	float32_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Float32 operator=(const Float32 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Float32 *p)	{ return "Float32"; }

class Float64
{
public:
	Float64(void) {
		status = OPROS_SUCCESS;
		data = 0.0;
	}
	Float64(float64_t data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Float64(void) {
	}

	ReturnType status;
	float64_t data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Float64 operator=(const Float64 &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Float64 *p)	{ return "Float64"; }

class String
{
public:
	String(void) {
		status = OPROS_SUCCESS;
		data = "";
	}
	String(std::string data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~String(void) {
	}

	ReturnType status;
	std::string data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	String operator=(const String &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(String *p)	{ return "String"; }

class BoolArray
{
public:
	BoolArray(void) {
		status = OPROS_SUCCESS;
	}
	BoolArray(std::vector<bool> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~BoolArray(void) {
	}

	ReturnType status;
	std::vector<bool> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	BoolArray operator=(const BoolArray &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(BoolArray *p)	{ return "BoolArray"; }

class CharArray
{
public:
	CharArray(void) {
		status = OPROS_SUCCESS;
	}
	CharArray(std::vector<char_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~CharArray(void) {
	}

	ReturnType status;
	std::vector<char_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	CharArray operator=(const CharArray &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(CharArray *p)	{ return "CharArray"; }

class Int8Array
{
public:
	Int8Array(void) {
		status = OPROS_SUCCESS;
	}
	Int8Array(std::vector<int8_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int8Array(void) {
	}

	ReturnType status;
	std::vector<int8_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int8Array operator=(const Int8Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int8Array *p)	{ return "Int8Array"; }

class UInt8Array
{
public:
	UInt8Array(void) {
		status = OPROS_SUCCESS;
	}
	UInt8Array(std::vector<uint8_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt8Array(void) {
	}

	ReturnType status;
	std::vector<uint8_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt8Array operator=(const UInt8Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt8Array *p)	{ return "UInt8Array"; }

class Int16Array
{
public:
	Int16Array(void) {
		status = OPROS_SUCCESS;
	}
	Int16Array(std::vector<int16_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int16Array(void) {
	}

	ReturnType status;
	std::vector<int16_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int16Array operator=(const Int16Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int16Array *p)	{ return "Int16Array"; }

class UInt16Array
{
public:
	UInt16Array(void) {
		status = OPROS_SUCCESS;
	}
	UInt16Array(std::vector<uint16_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt16Array(void) {
	}

	ReturnType status;
	std::vector<uint16_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt16Array operator=(const UInt16Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt16Array *p)	{ return "UInt16Array"; }

class Int32Array
{
public:
	Int32Array(void) {
		status = OPROS_SUCCESS;
	}
	Int32Array(std::vector<int32_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int32Array(void) {
	}

	ReturnType status;
	std::vector<int32_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int32Array operator=(const Int32Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int32Array *p)	{ return "Int32Array"; }

class UInt32Array
{
public:
	UInt32Array(void) {
		status = OPROS_SUCCESS;
	}
	UInt32Array(std::vector<uint32_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt32Array(void) {
	}

	ReturnType status;
	std::vector<uint32_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt32Array operator=(const UInt32Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt32Array *p)	{ return "UInt32Array"; }

class Int64Array
{
public:
	Int64Array(void) {
		status = OPROS_SUCCESS;
	}
	Int64Array(std::vector<int64_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Int64Array(void) {
	}

	ReturnType status;
	std::vector<int64_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Int64Array operator=(const Int64Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Int64Array *p)	{ return "Int64Array"; }

class UInt64Array
{
public:
	UInt64Array(void) {
		status = OPROS_SUCCESS;
	}
	UInt64Array(std::vector<uint64_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~UInt64Array(void) {
	}

	ReturnType status;
	std::vector<uint64_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	UInt64Array operator=(const UInt64Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(UInt64Array *p)	{ return "UInt64Array"; }

class Float32Array
{
public:
	Float32Array(void) {
		status = OPROS_SUCCESS;
	}
	Float32Array(std::vector<float32_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Float32Array(void) {
	}

	ReturnType status;
	std::vector<float32_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Float32Array operator=(const Float32Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Float32Array *p)	{ return "Float32Array"; }

class Float64Array
{
public:
	Float64Array(void) {
		status = OPROS_SUCCESS;
	}
	Float64Array(std::vector<float64_t> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~Float64Array(void) {
	}

	ReturnType status;
	std::vector<float64_t> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	Float64Array operator=(const Float64Array &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(Float64Array *p)	{ return "Float64Array"; }

class StringArray
{
public:
	StringArray(void) {
		status = OPROS_SUCCESS;
	}
	StringArray(std::vector<std::string> data) {
		status = OPROS_SUCCESS;
		this->data = data;
	}
	virtual ~StringArray(void) {
	}

	ReturnType status;
	std::vector<std::string> data;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;		ar << data;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;		ar >> data;
	}

	StringArray operator=(const StringArray &arg)
	{
		this->status = arg.status;
		this->data = arg.data;

		return *this;
	}
};
inline std::string typeName(StringArray *p)	{ return "StringArray"; }

}		//	namespace OPRoS

