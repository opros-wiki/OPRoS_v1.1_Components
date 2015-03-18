#ifndef __STD_WSTRING_SERIALIZE_H__
#define __STD_WSTRING_SERIALIZE_H__

void load_this(opros::archive::iarchive& ar, std::wstring& str)
{
	int size;
	ar >> size;

	if (size < 0)
		throw opros::archive::archive_format_exception();
	
	str.resize(size);

	for (int i = 0; i < size; i++)
	{
		unsigned short temp;
		ar >> temp;
		str[i] = temp;
	}	
}

void save_this(opros::archive::oarchive& ar, std::wstring& str)
{
	int size = str.size();
	ar << size;

	for (int i = 0;  i < size; i++)
	{
		ar << (unsigned char)str[i];
	}
}

std::string typeName(std::wstring* p)
{
	return "std::wstring";
}

#endif