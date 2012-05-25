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
//	class V2iAttribute
//	class V2fAttribute
//	class V3iAttribute
//	class V3fAttribute
//
//-----------------------------------------------------------------------------

#include <ImfAttribute.h>
#include "ImathVec.h"
#include <vector>

typedef std::vector<int> Veci;
typedef std::vector<float> Vecf;
typedef std::vector<double> Vecd;

namespace Imf {

typedef TypedAttribute<Veci> ViAttribute;
template <> const char *ViAttribute::staticTypeName ();
template <> void ViAttribute::writeValueTo (OStream &, int) const;
template <> void ViAttribute::readValueFrom (IStream &, int, int);

typedef TypedAttribute<Vecf> VfAttribute;
template <> const char *VfAttribute::staticTypeName ();
template <> void VfAttribute::writeValueTo (OStream &, int) const;
template <> void VfAttribute::readValueFrom (IStream &, int, int);

typedef TypedAttribute<Vecd> VdAttribute;
template <> const char *VdAttribute::staticTypeName ();
template <> void VdAttribute::writeValueTo (OStream &, int) const;
template <> void VdAttribute::readValueFrom (IStream &, int, int);

} // namespace Imf

#endif /* IMFVECTORATTRIBUTE_H_ */
