/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 3. 13
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RawImage.h
 *  @Modified : modified for oce 2011
 *
 *
 */

#ifndef _RAWIMAGE_H
#define _RAWIMAGE_H

#include <OPRoSTypes.h>
#include <opros_smart_ptr.h>
#include <archive/opros_archive_smart_ptr.h>
#include <memory.h>

class RawImageData {
protected:
	unsigned char *m_data;
	int m_width;
	int m_height;
	int m_pixelByte;
	unsigned long m_size;

public:
	RawImageData(int width, int height, int pixelByte)
	{
		m_width = width;
		m_height = height;
		m_pixelByte = pixelByte;

		m_size = m_width * m_height * m_pixelByte;

		m_data = new unsigned char[m_size];
	}

	RawImageData(RawImageData &ptr)
	{
		m_width = ptr.getWidth();
		m_height = ptr.getHeight();
		m_pixelByte = ptr.getPixelByte();

		m_size = ptr.getSize();

		m_data = new unsigned char[m_size];
		
		memcpy(m_data, ptr.getData(), m_size);
	}

	RawImageData()
	{
		m_data = 0;
		m_size = 0;
		m_width = 0;
		m_height = 0;
		m_pixelByte = 0;
	}

	virtual ~RawImageData()
	{
		if (m_data != NULL)
			delete m_data;
	}

	RawImageData(unsigned char *data, int width, int height, int pixelByte)
	{
		m_width = width;
		m_height = height;
		m_pixelByte = pixelByte;
		m_size = m_width * m_height * m_pixelByte;
		m_data = new unsigned char[m_size];

		memcpy(m_data, data, m_size);
	}

	void reset(RawImageData &p)
	{
		unsigned char *pdata = p.getData();
		reset(pdata, p.getWidth(), p.getHeight(), p.getPixelByte());
	}

	void reset(unsigned char *data, int width, int height, int pixelByte)
	{
		unsigned long  n_size = width * height * pixelByte;
		
		if (m_data == NULL) 
		{
			m_size = n_size;
			m_data = new unsigned char [m_size];
		}
		else
		if (m_size < n_size)
		{
			if (m_data != NULL) delete m_data;
			m_size = n_size;
			m_data = new unsigned char [m_size];
		}

		m_width = width;
		m_height = height;
		m_pixelByte = pixelByte;
		
		memcpy(m_data, data, n_size);
	}

	void resize(int width, int height, int pixelByte)
	{
		if (m_data != NULL) delete m_data;
		m_width = width;
		m_height = height;
		m_pixelByte = pixelByte;
		m_size = m_width * m_height * m_pixelByte;
		m_data = new unsigned char[m_size];
	}

	unsigned char *getData()
	{
		return m_data;
	}

	unsigned long getSize()
	{
		return m_size;
	}

	int getWidth()
	{
		return m_width;
	}

	int getHeight()
	{
		return m_height;
	}

	int getPixelByte()
	{
		return m_pixelByte;
	}
};



class RawImage {
	friend class opros::archive::access;

	opros::smart_ptr<RawImageData> m_image;

public:
	void save(opros::archive::oarchive &ar, const unsigned int version) {
		ar << OPROS_SERIALIZATION_NVP(m_image);
	}

	void load(opros::archive::iarchive &ar, const unsigned int version) {
		ar >> OPROS_SERIALIZATION_NVP(m_image);
	}

	RawImage() : m_image(new RawImageData) {
	}
	RawImage(RawImageData &ptr) : m_image( new RawImageData(ptr)) {
	}

	RawImage(const RawImage &rht) {
		m_image = rht.m_image;
	}

	virtual ~RawImage()
	{
	}

	RawImageData *getImage() {
		return m_image.get();
	}

	void setImage(RawImageData *ptr) {
		m_image.reset(ptr);
	}


	RawImage &operator=(const RawImage &rhs) {
		m_image = rhs.m_image;
		return *this;
	}
};

inline std::string typeName(RawImage *p)
{
	return std::string("RawImage");
}



inline void save_this(opros::archive::oarchive &ar, RawImageData &g)
{
	std::string raw("raw");

	ar << OPROS_SERIALIZATION_NVP(raw);

	int width = g.getWidth();
	int height = g.getHeight();
	int pixelByte = g.getPixelByte();

	unsigned long size = g.getSize();

	ar << OPROS_SERIALIZATION_NVP(width);
	ar << OPROS_SERIALIZATION_NVP(height);
	ar << OPROS_SERIALIZATION_NVP(pixelByte);
	ar << OPROS_SERIALIZATION_NVP(size);

	ar.save((const char *)g.getData(), size);
}


inline void load_this(opros::archive::iarchive &ar, RawImageData &g)
{
	std::string raw;

	ar >> OPROS_SERIALIZATION_NVP(raw);

	int width;
	int height;
	int pixelByte;

	unsigned long size = 0;

	ar >> OPROS_SERIALIZATION_NVP(width);
	ar >> OPROS_SERIALIZATION_NVP(height);
	ar >> OPROS_SERIALIZATION_NVP(pixelByte);
	ar >> OPROS_SERIALIZATION_NVP(size);

	g.resize(width, height, pixelByte);

	char *p = (char *)g.getData();

	ar.load(&p, size);
 ;
}

#endif /* _RAWIMAGE_H */
