/*
 * ImfVectorAttribute.cpp
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#include "ImfVectorAttribute.h"

namespace Imf {


template <>
const char *
ViAttribute::staticTypeName ()
{
    return "vi";
}


template <>
void ViAttribute::writeValueTo (OStream &os, int version) const
{
	for (int iter = 0, end = _value.size(); iter < end; ++iter) {
		Xdr::write <StreamIO> (os, _value[iter]);
	}
}


template <>
void ViAttribute::readValueFrom (IStream &is, int size, int version)
{
	int read = 0;

	while (read < size)
	{
		int temp;
		Xdr::read <StreamIO> (is, temp);
		read ++;
       _value.push_back (temp);
	}
}


template <>
const char *
VfAttribute::staticTypeName ()
{
    return "vf";
}


template <>
void VfAttribute::writeValueTo (OStream &os, int version) const
{
	for (int iter = 0, end = _value.size(); iter < end; ++iter) {
		Xdr::write <StreamIO> (os, _value[iter]);
	}
}


template <>
void VfAttribute::readValueFrom (IStream &is, int size, int version)
{
	int read = 0;

	while (read < size)
	{
		float temp;
		Xdr::read <StreamIO> (is, temp);
		read ++;
       _value.push_back (temp);
	}
}

//{
//	int read = 0;
//	while (read < size) {
//		float temp;
//		Xdr::read <StreamIO> (is, temp);
//		_value.push_back(temp);
//		++read;
//	}
//}


} // namespace Imf

