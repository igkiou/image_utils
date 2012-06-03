/*
 * ImfVectorAttribute.cpp
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#include "ImfVectorAttribute.h"
#include <iterator>


namespace Imf {


//------------------------------------
// Implementation of VectorAttribute<T>
//------------------------------------

template <>
const char *
ViAttribute::staticTypeName ()
{
    return "vi";
}

template <>
const char *
VfAttribute::staticTypeName ()
{
    return "vf";
}

} // namespace Imf
