/*
 * ImfVectorAttribute.cpp
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#include "ImfVectorAttribute.h"
#include <iterator>

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
	for (int iter = 0, size = _value.size(); iter < size; ++iter) {
		Xdr::write <StreamIO> (os, _value[iter]);
	}
}


template <>
void ViAttribute::readValueFrom (IStream &is, int size, int version)
{
	for (int iter = 0, size = _value.size(); iter < size; ++iter) {
		Xdr::read <StreamIO> (is, _value[iter]);
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
	for (int iter = 0, size = _value.size(); iter < size; ++iter) {
		Xdr::write <StreamIO> (os, _value[iter]);
	}
}


template <>
void VfAttribute::readValueFrom (IStream &is, int size, int version)
{
	for (int iter = 0, size = _value.size(); iter < size; ++iter) {
		Xdr::read <StreamIO> (is, _value[iter]);
	}
}


} // namespace Imf

