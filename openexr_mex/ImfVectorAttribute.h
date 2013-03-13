/*
 * ImfVectorAttribute.h
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#ifndef IMFVECTORATTRIBUTE_H_
#define IMFVECTORATTRIBUTE_H_

//-----------------------------------------------------------------------------
//
//	class ViAttribute
//	class VfAttribute
//
//-----------------------------------------------------------------------------

#include <ImfAttribute.h>
#include "ImathVec.h"
#include <vector>
#include <iterator>


namespace Imf {


typedef TypedAttribute<std::vector<int> > ViAttribute;
template <> const char *ViAttribute::staticTypeName ();
template <> void ViAttribute::writeValueTo (OStream &, int) const;
template <> void ViAttribute::readValueFrom (IStream &, int, int);


typedef TypedAttribute<std::vector<float> > VfAttribute;
template <> const char *VfAttribute::staticTypeName ();
template <> void VfAttribute::writeValueTo (OStream &, int) const;
template <> void VfAttribute::readValueFrom (IStream &, int, int);


} // namespace Imf

// Metrowerks compiler wants the .cpp file inlined, too
#ifdef __MWERKS__
#include <ImfVectorAttribute.cpp>
#endif


#endif /* IMFVECTORATTRIBUTE_H_ */
