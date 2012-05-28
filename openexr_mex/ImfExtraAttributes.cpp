/*
 * ImfExtraAttributes.cpp
 *
 *  Created on: May 27, 2012
 *      Author: igkiou
 */

#include "ImfExtraAttributes.h"


#define IMF_STRING(name) #name

#define IMF_EXTRA_ATTRIBUTE_IMP(name,suffix,type)				 \
									 \
    void								 \
    add##suffix (Header &header, const type &value)			 \
    {									 \
	header.insert (IMF_STRING (name), TypedAttribute<type> (value)); \
    }									 \
									 \
    bool								 \
    has##suffix (const Header &header)					 \
    {									 \
	return header.findTypedAttribute <TypedAttribute <type> >	 \
		(IMF_STRING (name)) != 0;				 \
    }									 \
									 \
    const TypedAttribute<type> &					 \
    name##Attribute (const Header &header)				 \
    {									 \
	return header.typedAttribute <TypedAttribute <type> >		 \
		(IMF_STRING (name));					 \
    }									 \
									 \
    TypedAttribute<type> &						 \
    name##Attribute (Header &header)					 \
    {									 \
	return header.typedAttribute <TypedAttribute <type> >		 \
		(IMF_STRING (name));					 \
    }									 \
									 \
    const type &							 \
    name (const Header &header)						 \
    {									 \
	return name##Attribute(header).value();				 \
    }									 \
									 \
    type &								 \
    name (Header &header)						 \
    {									 \
	return name##Attribute(header).value();				 \
    }


namespace Imf {

IMF_EXTRA_ATTRIBUTE_IMP (gain, Gain, float)
IMF_EXTRA_ATTRIBUTE_IMP (wavelength, Wavelength, float)

//IMF_EXTRA_ATTRIBUTE_IMP (apertures, Apertures, std::vector<float>)
//IMF_EXTRA_ATTRIBUTE_IMP (expTimes, ExpTimes, std::vector<float>)
//IMF_EXTRA_ATTRIBUTE_IMP (gains, Gains, std::vector<float>)

IMF_EXTRA_ATTRIBUTE_IMP (extTube, ExtTube, std::string)
IMF_EXTRA_ATTRIBUTE_IMP (lens, Lens, std::string)
IMF_EXTRA_ATTRIBUTE_IMP (material, Material, std::string)

} // namespace Imf
