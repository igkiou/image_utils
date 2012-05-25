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
void
ViAttribute::writeValueTo (OStream &os, int version) const
{
	for (int vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::write <StreamIO> (os, _value[vecIt]);
	}
}

template <>
void
ViAttribute::readValueFrom (IStream &is, int size, int version)
{
	for (int vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::read <StreamIO> (is, _value[vecIt]);
	}
}

template <>
const char *
VfAttribute::staticTypeName ()
{
    return "vf";
}

template <>
void
VfAttribute::writeValueTo (OStream &os, int version) const
{
	for (int vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::write <StreamIO> (os, _value[vecIt]);
	}
}

template <>
void
VfAttribute::readValueFrom (IStream &is, int size, int version)
{
	for (int vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::read <StreamIO> (is, _value[vecIt]);
	}
}

template <>
const char *
VdAttribute::staticTypeName ()
{
    return "vd";
}


template <>
void
VdAttribute::writeValueTo (OStream &os, int version) const
{
	for (int vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::write <StreamIO> (os, _value[vecIt]);
	}
}

template <>
void
VdAttribute::readValueFrom (IStream &is, int size, int version)
{
	for (int vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::read <StreamIO> (is, _value[vecIt]);
	}
}


} // namespace Imf
