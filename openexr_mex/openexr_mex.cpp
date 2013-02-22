/*
 * openexr_mex.cpp
 *
 *  Created on: May 28, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

namespace exr {

namespace {

const mex::ConstMap<std::string, EAttributeType> registeredAttributeNameToAttributeType
	= mex::ConstMap<std::string, EAttributeType>
	("gain",				EAttributeFloat)
	("wavelength",			EAttributeFloat)
	("extTube",				EAttributeString)
	("lens",				EAttributeString)
	("material",			EAttributeString)
	("chromaticities",		EAttributeChromaticities)
	("whiteLuminance",		EAttributeFloat)
	("adoptedNeutral",		EAttributeV2f)
	("renderingTransform",	EAttributeString)
	("lookModTransform",	EAttributeFloat)
	("xDensity",			EAttributeFloat)
	("owner",				EAttributeString)
	("comments",			EAttributeString)
	("capDate",				EAttributeString)
	("utcOffset",			EAttributeFloat)
	("longitude",			EAttributeFloat)
	("latitude",			EAttributeFloat)
	("altitude",			EAttributeFloat)
	("focus",				EAttributeFloat)
	("expTime",				EAttributeFloat)
	("aperture",			EAttributeFloat)
	("isoSpeed",			EAttributeFloat)
	("multExpTimes",		EAttributeVectorf)
	("multApertures",		EAttributeVectorf)
	("multIsoSpeeds",		EAttributeVectorf)
	("multGains",			EAttributeVectorf)
	("envmap",				EAttributeEnvmap)
	("unknown",				EAttributeTypeInvalid);

const mex::ConstMap<std::string, EAttributeType> stringToAttributeType
	= mex::ConstMap<std::string, EAttributeType>
	("channelList",		EAttributeChannelList)
	("compression",		EAttributeCompression)
	("lineOrder",		EAttributeLineOrder)
	("chromaticities",	EAttributeChromaticities)
	("envmap",			EAttributeEnvmap)
	("string",			EAttributeString)
	("box2d",			EAttributeBox2d)
	("box2f",			EAttributeBox2f)
	("box2i",			EAttributeBox2i)
	("v2d",				EAttributeV2d)
	("v2f",				EAttributeV2f)
	("v2i",				EAttributeV2i)
	("vd",				EAttributeVectord)
	("vf",				EAttributeVectorf)
	("vi",				EAttributeVectori)
	("double",			EAttributeDouble)
	("float",			EAttributeFloat)
	("int",				EAttributeInt)
	("unknown",			EAttributeTypeInvalid);

const mex::ConstMap<EAttributeType, std::string> attributeTypeToString
	= mex::ConstMap<EAttributeType, std::string>
	(EAttributeChannelList, 	"channelList")
	(EAttributeCompression,		"compression")
	(EAttributeLineOrder,		"lineOrder")
	(EAttributeChromaticities,	"chromaticities")
	(EAttributeEnvmap,			"envmap")
	(EAttributeString,			"string")
	(EAttributeBox2d,			"box2d")
	(EAttributeBox2f,			"box2f")
	(EAttributeBox2i,			"box2i")
	(EAttributeV2d,				"v2d")
	(EAttributeV2f,				"v2f")
	(EAttributeV2i,				"v2i")
	(EAttributeVectord,			"vectord")
	(EAttributeVectorf,			"vectorf")
	(EAttributeVectori,			"vectori")
	(EAttributeDouble,			"double")
	(EAttributeFloat,			"float")
	(EAttributeInt,				"int")
	(EAttributeTypeInvalid,		"unknown");

const mex::ConstMap<std::string, Imf::Compression> stringToCompressionType
	= mex::ConstMap<std::string, Imf::Compression>
	("no", 		Imf::NO_COMPRESSION)
	("rle",		Imf::RLE_COMPRESSION)
	("zips",	Imf::ZIPS_COMPRESSION)
	("zip",		Imf::ZIP_COMPRESSION)
	("piz",		Imf::PIZ_COMPRESSION)
	("pxr24",	Imf::PXR24_COMPRESSION)
	("b44",		Imf::B44_COMPRESSION)
	("b44a",	Imf::B44A_COMPRESSION)
	("unknown",	Imf::NUM_COMPRESSION_METHODS);

const mex::ConstMap<Imf::Compression, std::string> compressionTypeToString
	= mex::ConstMap<Imf::Compression, std::string>
	(Imf::NO_COMPRESSION, 			"no")
	(Imf::RLE_COMPRESSION,			"rle")
	(Imf::ZIPS_COMPRESSION,			"zips")
	(Imf::ZIP_COMPRESSION,			"zip")
	(Imf::PIZ_COMPRESSION,			"piz")
	(Imf::PXR24_COMPRESSION,		"pxr24")
	(Imf::B44_COMPRESSION,			"b44")
	(Imf::B44A_COMPRESSION,			"b44a")
	(Imf::NUM_COMPRESSION_METHODS,	"unknown");

const mex::ConstMap<std::string, Imf::LineOrder> stringToLineOrderType
	= mex::ConstMap<std::string, Imf::LineOrder>
	("increasing_y", 	Imf::INCREASING_Y)
	("decreasing_y",	Imf::DECREASING_Y)
	("random_y",		Imf::RANDOM_Y)
	("unknown",			Imf::NUM_LINEORDERS);

const mex::ConstMap<Imf::LineOrder, std::string> lineOrderTypeToString
	= mex::ConstMap<Imf::LineOrder, std::string>
	(Imf::INCREASING_Y,		"increasing_y")
	(Imf::DECREASING_Y,		"decreasing_y")
	(Imf::RANDOM_Y,			"random_y")
	(Imf::NUM_LINEORDERS,	"unknown");

const mex::ConstMap<std::string, Imf::Envmap> stringToEnvmapType
	= mex::ConstMap<std::string, Imf::Envmap>
	("latlong", Imf::ENVMAP_LATLONG)
	("cube",	Imf::ENVMAP_CUBE)
	("unknown",	Imf::NUM_ENVMAPTYPES);

const mex::ConstMap<Imf::Envmap, std::string> envmapTypeToString
	= mex::ConstMap<Imf::Envmap, std::string>
	(Imf::ENVMAP_LATLONG, 	"latlong")
	(Imf::ENVMAP_CUBE,		"cube")
	(Imf::NUM_ENVMAPTYPES,	"unknown");


/*
 * Routines to convert an Attribute to MxArray.
 * TODO: Perhaps use copy elision here to avoid using pointers, and return
 * object directly?
 */

// VT
template <typename T>
mex::MxArray* toMxArray(
	const Imf::TypedAttribute<T>* pAttribute) {
	return new mex::MxNumeric<T>(pAttribute->value());
}

// V2T
template <typename T>
mex::MxArray* toMxArray<Imath::Vec2<T> >(
	const Imf::TypedAttribute<Imath::Vec2<T> >* pAttribute) {
	std::vector<T> temp;
	temp.push_back(pAttribute->value().x);
	temp.push_back(pAttribute->value().y);
	return new mex::MxNumeric<T>(temp);
}

// Box2T
template <typename T>
mex::MxArray* toMxArray<Imath::Box<Imath::Vec2<T> > >(
	const Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > >* pAttribute) {
	std::vector<mex::MxArray *> temp;
	std::vector<T> tempV;
	tempV.push_back(pAttribute->value().min.x);
	tempV.push_back(pAttribute->value().min.y);
	temp.push_back(new mex::MxNumeric<T>(tempV));
	tempV.clear();
	tempV.push_back(pAttribute->value().max.x);
	tempV.push_back(pAttribute->value().max.y);
	temp.push_back(new mex::MxNumeric<T>(tempV));
	return new mex::MxCell(temp);
}

// VectorT
template <typename T>
mex::MxArray* toMxArray<std::vector<T> >(
	const Imf::TypedAttribute<std::vector<T> >* pAttribute) {
	return new mex::MxNumeric<T>(pAttribute->value());
}

// String
template <>
mex::MxArray* toMxArray<std::string>(
	const Imf::TypedAttribute<std::string>* pAttribute) {
	return new mex::MxString(pAttribute->value());
}

// Envmap
template <>
mex::MxArray* toMxArray<Imf::Envmap>(
	const Imf::TypedAttribute<Imf::Envmap>* pAttribute) {
	return new mex::MxString(
							envmapTypeToString[pAttribute->value()]);
}

// Chromaticities
template <>
mex::MxArray* toMxArray<Imf::Chromaticities>(
	const Imf::TypedAttribute<Imf::Chromaticities>* pAttribute) {
	/*
	 * TODO: Change this to return struct instead.
	 */
	std::vector<mex::MxArray*> temp;
	temp.push_back(new mex::MxString("red"));
	temp.push_back(new mex::MxString("green"));
	temp.push_back(new mex::MxString("blue"));
	temp.push_back(new mex::MxString("white"));
	std::vector<float> tempV;
	tempV.push_back(pAttribute->value().red.x);
	tempV.push_back(pAttribute->value().red.y);
	temp.push_back(new mex::MxNumeric<float>(tempV));
	tempV.clear();
	tempV.push_back(pAttribute->value().green.x);
	tempV.push_back(pAttribute->value().green.y);
	temp.push_back(new mex::MxNumeric<float>(tempV));
	tempV.clear();
	tempV.push_back(pAttribute->value().blue.x);
	tempV.push_back(pAttribute->value().blue.y);
	temp.push_back(new mex::MxNumeric<float>(tempV));
	tempV.clear();
	tempV.push_back(pAttribute->value().white.x);
	tempV.push_back(pAttribute->value().white.y);
	temp.push_back(new mex::MxNumeric<float>(tempV));
	return new mex::MxCell(temp/*, 3, 2*/);
}

// LineOrder
template <>
mex::MxArray* toMxArray<Imf::LineOrder>(
	const Imf::TypedAttribute<Imf::LineOrder>* pAttribute) {
	return new mex::MxString(lineOrderTypeToString[pAttribute->value()]);
}

// Compression
template <>
mex::MxArray* toMxArray<Imf::Compression>(
	const Imf::TypedAttribute<Imf::Compression>* pAttribute) {
	return new mex::MxString(compressionTypeToString[pAttribute->value()]);
}

// ChannelList
template <>
mex::MxArray* toMxArray<Imf::ChannelList>(
	const Imf::TypedAttribute<Imf::ChannelList>* pAttribute) {
	std::vector<mex::MxArray*> temp;
	for (Imf::ChannelList::ConstIterator chIt = pAttribute->value().begin(), \
		chEnd = pAttribute->value().end(); \
		chIt != chEnd; \
		++chIt) {
		temp.push_back(new mex::MxString(chIt.name()));
	}
	return new mex::MxCell(temp);
}

/*
 * Routines to convert an MxArray to Attribute.
 */

// VT
template <typename T>
Imf::Attribute* toAttribute(const mex::MxArray* pMxArray) {
	mex::MxNumeric<T>* pTemp = (mex::MxNumeric<T>*) pMxArray;
	return new Imf::TypedAttribute<T>(*(pTemp->data()));
}

// V2T
template <typename T>
Imf::Attribute* toAttribute<Imath::Vec2<T> >(const mex::MxArray* pMxArray) {
	mex::MxNumeric<T>* pTemp = (mex::MxNumeric<T>*) pMxArray;
	return new Imf::TypedAttribute<Imath::Vec2<T> >(Imath::Vec2<T>(
													(*pTemp)[0], (*pTemp[1])));
}

// Box2T
template <typename T>
Imf::Attribute* toAttribute<Imath::Box<Imath::Vec2<T> > >(
											const mex::MxArray* pMxArray) {
	mex::MxCell* pTemp = (mex::MxCell*) pMxArray;
	mex::MxNumeric<T> minArray = mex::MxNumeric<T>((*pTemp)[0]);
	Imath::Vec2<T> minVec(minArray[0], minArray[1]);
	mex::MxNumeric<T> maxArray = mex::MxNumeric<T>((*pTemp)[1]);
	Imath::Vec2<T> maxVec(maxArray[0], maxArray[1]);
	return new Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > >(
								Imath::Box<Imath::Vec2<T> >(minVec, maxVec));
}

// VectorT
template <typename T>
Imf::Attribute* toAttribute<std::vector<T> >(const mex::MxArray* pMxArray) {
	mex::MxNumeric<T>* pTemp = (mex::MxNumeric<T>*) pMxArray;
	return new Imf::TypedAttribute<std::vector<T> >(pTemp->vectorize());
}

// String
template <>
Imf::Attribute* toAttribute<std::string>(const mex::MxArray* pMxArray) {
	mex::MxString* pTemp = (mex::MxString*) pMxArray;
	return new Imf::TypedAttribute<std::string>(pTemp->string());
}

// Envmap
template <>
Imf::Attribute* toAttribute<Imf::Envmap>(const mex::MxArray* pMxArray) {
	mex::MxString* pTemp = (mex::MxString*) pMxArray;
	return new Imf::TypedAttribute<Imf::Envmap>(stringToEnvmapType(
															pTemp->string()));
}

//// Chromaticities
//template <>
//mex::MxArray* toMxArray<Imf::Chromaticities>(
//	const Imf::TypedAttribute<Imf::Chromaticities>* pAttribute) {
//	/*
//	 * TODO: Change this to return struct instead.
//	 */
//	std::vector<mex::MxArray*> temp;
//	temp.push_back(new mex::MxString("red"));
//	temp.push_back(new mex::MxString("green"));
//	temp.push_back(new mex::MxString("blue"));
//	temp.push_back(new mex::MxString("white"));
//	std::vector<float> tempV;
//	tempV.push_back(pAttribute->value().red.x);
//	tempV.push_back(pAttribute->value().red.y);
//	temp.push_back(new mex::MxNumeric<float>(tempV));
//	tempV.clear();
//	tempV.push_back(pAttribute->value().green.x);
//	tempV.push_back(pAttribute->value().green.y);
//	temp.push_back(new mex::MxNumeric<float>(tempV));
//	tempV.clear();
//	tempV.push_back(pAttribute->value().blue.x);
//	tempV.push_back(pAttribute->value().blue.y);
//	temp.push_back(new mex::MxNumeric<float>(tempV));
//	tempV.clear();
//	tempV.push_back(pAttribute->value().white.x);
//	tempV.push_back(pAttribute->value().white.y);
//	temp.push_back(new mex::MxNumeric<float>(tempV));
//	return new mex::MxCell(temp/*, 3, 2*/);
//}
//
// LineOrder
template <>
Imf::Attribute* toAttribute<Imf::LineOrder>(const mex::MxArray* pMxArray) {
	mex::MxString* pTemp = (mex::MxString*) pMxArray;
	return new Imf::TypedAttribute<Imf::LineOrder>(stringToLineOrderType(
															pTemp->string()));
}

// Compression
template <>
Imf::Attribute* toAttribute<Imf::Compression>(const mex::MxArray* pMxArray) {
	mex::MxString* pTemp = (mex::MxString*) pMxArray;
	return new Imf::TypedAttribute<Imf::Compression>(stringToCompressionType(
															pTemp->string()));
}

// ChannelList
// Not supported.

} /* namespace */

EXRAttribute::EXRAttribute()
						: m_type(EAttributeTypeInvalid),
						  m_pAttribute(),
						  m_isAttributeInitialized(false),
						  m_pArray(),
						  m_isArrayInitialized(false),
						  m_isBuilt(false) {	}

explicit EXRAttribute::EXRAttribute(const Imf::Attribute* pAttribute)
									: m_type(
											stringToAttributeType(
													pAttribute->typeName())),
									  m_pAttribute(pAttribute),
									  m_isAttributeInitialized(true),
									  m_pArray(),
									  m_isArrayInitialized(false),
									  m_isBuilt(false) {	}

explicit EXRAttribute::EXRAttribute(const mex::MxArray* pArray,
									const EAttributeType eAttributeType)
									: m_type(eAttributeType),
									  m_pAttribute(),
									  m_isAttributeInitialized(false),
									  m_pArray(pArray),
									  m_isArrayInitialized(true),
									  m_isBuilt(false) {
	switch(m_type) {
		case EAttributeChannelList: {
			mexAssertEx(0, "Unsupported attribute type");
			return;
//			mexAssert(m_pArray->m_kClass == mxCELL_CLASS);
//			for (int iter = 0, numel = m_pArray->numel(); iter < numel;
//				++iter) {
//				mexAssert(mxGetClassID((*m_pArray)[iter]) == mxCHAR_CLASS);
//			}
//			return;
		}
		case EAttributeCompression: {
			mexAssert(m_pArray->m_kClass == mxCHAR_CLASS);
			return;
		}
		case EAttributeLineOrder: {
			mexAssert(m_pArray->m_kClass == mxCHAR_CLASS);
			return;
		}
		case EAttributeChromaticities: {
			/*
			 * TODO: Remember to fix this once I change chromaticities to
			 * become mex struct.
			 */
			mexAssert(m_pArray->m_kClass == mxCELL_CLASS);
			return;
		}
		case EAttributeEnvmap: {
			mexAssert(m_pArray->m_kClass == mxCHAR_CLASS);
			return;
		}
		case EAttributeString: {
			mexAssert(m_pArray->m_kClass == mxCHAR_CLASS);
			return;
		}
		case EAttributeBox2d: {
			mexAssert((m_pArray->m_kClass == mxCELL_CLASS) &&
					(m_pArray->numel() == 2) &&
					(mxGetClassID((*m_pArray)[0]) == mex::MxClassId<double>()) &&
					(mxGetNumberOfElements((*m_pArray)[0]) == 2) &&
					(mxGetClassID((*m_pArray)[1]) == mex::MxClassId<double>()) &&
					(mxGetNumberOfElements((*m_pArray)[1]) == 2));
			return;
		}
		case EAttributeBox2f: {
			mexAssert((m_pArray->m_kClass == mxCELL_CLASS) &&
					(m_pArray->numel() == 2) &&
					(mxGetClassID((*m_pArray)[0]) == mex::MxClassId<float>()) &&
					(mxGetNumberOfElements((*m_pArray)[0]) == 2) &&
					(mxGetClassID((*m_pArray)[1]) == mex::MxClassId<float>()) &&
					(mxGetNumberOfElements((*m_pArray)[1]) == 2));
			return;
		}
		case EAttributeBox2i: {
			mexAssert((m_pArray->m_kClass == mxCELL_CLASS) &&
					(m_pArray->numel() == 2) &&
					(mxGetClassID((*m_pArray)[0]) == mex::MxClassId<int>()) &&
					(mxGetNumberOfElements((*m_pArray)[0]) == 2) &&
					(mxGetClassID((*m_pArray)[1]) == mex::MxClassId<int>()) &&
					(mxGetNumberOfElements((*m_pArray)[1]) == 2));
			return;
		}
		case EAttributeV2d: {
			mexAssert((m_pArray->m_kClass == mex::MxClassId<double>()) &&
					(m_pArray->numel() == 2));
			return;
		}
		case EAttributeV2f: {
			mexAssert((m_pArray->m_kClass == mex::MxClassId<float>()) &&
					(m_pArray->numel() == 2));
			return;
		}
		case EAttributeV2i: {
			mexAssert((m_pArray->m_kClass == mex::MxClassId<int>()) &&
					(m_pArray->numel() == 2));
			return;
		}
		case EAttributeVectord: {
			mexAssert(m_pArray->m_kClass == mex::MxClassId<double>());
			return;
		}
		case EAttributeVectorf: {
			mexAssert(m_pArray->m_kClass == mex::MxClassId<float>());
			return;
		}
		case EAttributeVectori: {
			mexAssert(m_pArray->m_kClass == mex::MxClassId<int>());
			return;
		}
		case EAttributeDouble: {
			mexAssert((m_pArray->m_kClass == mex::MxClassId<double>()) &&
					(m_pArray->numel() == 1));
			return;
		}
		case EAttributeFloat: {
			mexAssert((m_pArray->m_kClass == mex::MxClassId<float>()) &&
					(m_pArray->numel() == 1));
			return;
		}
		case EAttributeInt: {
			mexAssert((m_pArray->m_kClass == mex::MxClassId<int>()) &&
					(m_pArray->numel() == 1));
			return;
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return;
		}
	}
}

void EXRAttribute::buildMxArray() {
	mexAssert(m_isAttributeInitialized && !m_isArrayInitialized && !m_isBuilt);
	switch(m_type) {
		case EAttributeChannelList: {
			m_pArray = toMxArray<Imf::ChannelList>(
					static_cast<Imf::TypedAttribute<Imf::ChannelList>*>(
																m_pAttribute));
			return;
		}
		case EAttributeCompression: {
			m_pArray = toMxArray<Imf::Compression>(
					static_cast<Imf::TypedAttribute<Imf::Compression>*>(
																m_pAttribute));
			return;
		}
		case EAttributeLineOrder: {
			m_pArray = toMxArray<Imf::LineOrder>(
					static_cast<Imf::TypedAttribute<Imf::LineOrder>*>(
																m_pAttribute));
			return;
		}
		case EAttributeChromaticities: {
			m_pArray = toMxArray<Imf::Chromaticities>(
					static_cast<Imf::TypedAttribute<Imf::Chromaticities>*>(
																m_pAttribute));
			return;
		}
		case EAttributeEnvmap: {
			m_pArray = toMxArray<Imf::Envmap>(
					static_cast<Imf::TypedAttribute<Imf::Envmap>*>(
																m_pAttribute));
			return;
		}
		case EAttributeString: {
			m_pArray = toMxArray<std::string>(
					static_cast<Imf::TypedAttribute<std::string>*>(
																m_pAttribute));
			return;
		}
		case EAttributeBox2d: {
			m_pArray = toMxArray<Imath::Box2d>(
					static_cast<Imf::TypedAttribute<Imath::Box2d>*>(
																m_pAttribute));
			return;
		}
		case EAttributeBox2f: {
			m_pArray = toMxArray<Imath::Box2f>(
					static_cast<Imf::TypedAttribute<Imath::Box2f>*>(
																m_pAttribute));
			return;
		}
		case EAttributeBox2i: {
			m_pArray = toMxArray<Imath::Box2i>(
					static_cast<Imf::TypedAttribute<Imath::Box2i>*>(
																m_pAttribute));
			return;
		}
		case EAttributeV2d: {
			m_pArray = toMxArray<Imath::V2d>(
					static_cast<Imf::TypedAttribute<Imath::V2d>*>(
																m_pAttribute));
			return;
		}
		case EAttributeV2f: {
			m_pArray = toMxArray<Imath::V2f>(
					static_cast<Imf::TypedAttribute<Imath::V2f>*>(
																m_pAttribute));
			return;
		}
		case EAttributeV2i: {
			m_pArray = toMxArray<Imath::V2i>(
					static_cast<Imf::TypedAttribute<Imath::V2i>*>(
																m_pAttribute));
			return;
		}
		case EAttributeVectord: {
			m_pArray = toMxArray<std::vector<double> >(
					static_cast<Imf::TypedAttribute<std::vector<double> >*>(
																m_pAttribute));
			return;
		}
		case EAttributeVectorf: {
			m_pArray = toMxArray<std::vector<float> >(
					static_cast<Imf::TypedAttribute<std::vector<float> >*>(
																m_pAttribute));
			return;
		}
		case EAttributeVectori: {
			m_pArray = toMxArray<std::vector<int> >(
					static_cast<Imf::TypedAttribute<std::vector<int> >*>(
																m_pAttribute));
			return;
		}
		case EAttributeDouble: {
			m_pArray = toMxArray<double>(
					static_cast<Imf::TypedAttribute<double>*>(
																m_pAttribute));
			return;
		}
		case EAttributeFloat: {
			m_pArray = toMxArray<float>(
					static_cast<Imf::TypedAttribute<float>*>(
																m_pAttribute));
			return;
		}
		case EAttributeInt: {
			m_pArray = toMxArray<int>(
					static_cast<Imf::TypedAttribute<int>*>(
																m_pAttribute));
			return;
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return;
		}
	}
}

void EXRAttribute::buildAttribute() {
	mexAssert(m_isArrayInitialized && !m_isAttributeInitialized && !m_isBuilt);
	switch(m_type) {
		case EAttributeChannelList: {
			m_pArray = toMxArray<Imf::ChannelList>(
					static_cast<Imf::TypedAttribute<Imf::ChannelList>*>(
																m_pAttribute));
			return;
		}
		case EAttributeCompression: {
			m_pArray = toMxArray<Imf::Compression>(
					static_cast<Imf::TypedAttribute<Imf::Compression>*>(
																m_pAttribute));
			return;
		}
		case EAttributeLineOrder: {
			m_pArray = toMxArray<Imf::LineOrder>(
					static_cast<Imf::TypedAttribute<Imf::LineOrder>*>(
																m_pAttribute));
			return;
		}
		case EAttributeChromaticities: {
			m_pArray = toMxArray<Imf::Chromaticities>(
					static_cast<Imf::TypedAttribute<Imf::Chromaticities>*>(
																m_pAttribute));
			return;
		}
		case EAttributeEnvmap: {
			m_pArray = toMxArray<Imf::Envmap>(
					static_cast<Imf::TypedAttribute<Imf::Envmap>*>(
																m_pAttribute));
			return;
		}
		case EAttributeString: {
			m_pArray = toMxArray<std::string>(
					static_cast<Imf::TypedAttribute<std::string>*>(
																m_pAttribute));
			return;
		}
		case EAttributeBox2d: {
			m_pArray = toMxArray<Imath::Box2d>(
					static_cast<Imf::TypedAttribute<Imath::Box2d>*>(
																m_pAttribute));
			return;
		}
		case EAttributeBox2f: {
			m_pArray = toMxArray<Imath::Box2f>(
					static_cast<Imf::TypedAttribute<Imath::Box2f>*>(
																m_pAttribute));
			return;
		}
		case EAttributeBox2i: {
			m_pArray = toMxArray<Imath::Box2i>(
					static_cast<Imf::TypedAttribute<Imath::Box2i>*>(
																m_pAttribute));
			return;
		}
		case EAttributeV2d: {
			m_pArray = toMxArray<Imath::V2d>(
					static_cast<Imf::TypedAttribute<Imath::V2d>*>(
																m_pAttribute));
			return;
		}
		case EAttributeV2f: {
			m_pArray = toMxArray<Imath::V2f>(
					static_cast<Imf::TypedAttribute<Imath::V2f>*>(
																m_pAttribute));
			return;
		}
		case EAttributeV2i: {
			m_pArray = toMxArray<Imath::V2i>(
					static_cast<Imf::TypedAttribute<Imath::V2i>*>(
																m_pAttribute));
			return;
		}
		case EAttributeVectord: {
			m_pArray = toMxArray<std::vector<double> >(
					static_cast<Imf::TypedAttribute<std::vector<double> >*>(
																m_pAttribute));
			return;
		}
		case EAttributeVectorf: {
			m_pArray = toMxArray<std::vector<float> >(
					static_cast<Imf::TypedAttribute<std::vector<float> >*>(
																m_pAttribute));
			return;
		}
		case EAttributeVectori: {
			m_pArray = toMxArray<std::vector<int> >(
					static_cast<Imf::TypedAttribute<std::vector<int> >*>(
																m_pAttribute));
			return;
		}
		case EAttributeDouble: {
			m_pArray = toMxArray<double>(
					static_cast<Imf::TypedAttribute<double>*>(
																m_pAttribute));
			return;
		}
		case EAttributeFloat: {
			m_pArray = toMxArray<float>(
					static_cast<Imf::TypedAttribute<float>*>(
																m_pAttribute));
			return;
		}
		case EAttributeInt: {
			m_pArray = toMxArray<int>(
					static_cast<Imf::TypedAttribute<int>*>(
																m_pAttribute));
			return;
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return;
		}
	}
}



mex::MxArray EXRInputFile::getAttribute(const std::string &attributeName) {
	const Imf::Attribute& attr = m_file.header[attributeName];
	return const_cast<mxArray *>(attributeToMxArray(attr));
}

mex::MxArray EXRInputFile::getAttribute(const Imf::Header& head) {

	mxArray* matStruct;
	int numAttributes = 0;
	for (Imf::Header::ConstIterator attIt = head.begin(); attIt != head.end(); \
		++attIt, ++numAttributes);

	const char **attributeNames = (const char **) mxMalloc(numAttributes * sizeof(*attributeNames));
	int iterField = 0;
	for (Imf::Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
		attributeNames[iterField] = attIt.name();
	}
	matStruct = mxCreateStructMatrix(1, 1, numAttributes, attributeNames);
	mxFree((void *) attributeNames);
	iterField = 0;
	for (Imf::Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
		mxSetFieldByNumber(matStruct, 0, iterField, const_cast<mxArray*>(attributeToMxArray(attIt.attribute())));
	}
	return matStruct;
}

void setAttribute(Imf::Header& head, const char attrName[], const mxArray* mxarr) {

	if (!strcmp(attrName, "gain")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'gain', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addGain(head, value);
	} else if (!strcmp(attrName, "wavelength")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'wavelength', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addWavelength(head, value);
	} else if (!strcmp(attrName, "extTube")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'extTube', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addExtTube(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "lens")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'lens', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addLens(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "material")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'material', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addMaterial(head, std::string(stringValue));
		mxFree(stringValue);
	} /* else if (!strcmp(attrName, "chromaticities")) {

		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'material', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addMaterial(head, std::string(stringValue));
		mxFree(stringValue);
	}*/ else if (!strcmp(attrName, "whiteLuminance")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'whiteLuminance', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addWhiteLuminance(head, value);
	} else if (!strcmp(attrName, "adoptedNeutral")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 2))) {
			mexErrMsgTxt("For setting attribute 'adoptedNeutral', a single array with 2 elements must be provided.");
		}
		const float *value = (float *) mxGetData(mxarr);
		Imf::addAdoptedNeutral(head, V2f(value[0], value[1]));
	} else if (!strcmp(attrName, "renderingTransform")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'renderingTransform', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addRenderingTransform(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "lookModTransform")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'lookModTransform', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addLookModTransform(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "xDensity")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'xDensity', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addXDensity(head, value);
	} else if (!strcmp(attrName, "owner")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'owner', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addOwner(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "comments")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'comments', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addComments(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "capDate")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'capDate', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addCapDate(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "utcOffset")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'utcOffset', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addUtcOffset(head, value);
	} else if (!strcmp(attrName, "longitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'longitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addLongitude(head, value);
	} else if (!strcmp(attrName, "latitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'latitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addLatitude(head, value);
	} else if (!strcmp(attrName, "altitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'altitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addAltitude(head, value);
	} else if (!strcmp(attrName, "focus")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'focus', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addFocus(head, value);
	} else if (!strcmp(attrName, "expTime")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'expTime', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addExpTime(head, value);
	} else if (!strcmp(attrName, "aperture")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'aperture', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addAperture(head, value);
	} else if (!strcmp(attrName, "isoSpeed")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'isoSpeed', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addIsoSpeed(head, value);
	} else if (!strcmp(attrName, "envmap")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'envmap', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addEnvmap(head, stringToEnvmapType(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "multApertures")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multApertures', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		Imf::addMultApertures(head, value);
	} else if (!strcmp(attrName, "multExpTimes")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multExpTimes', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(lengthVector);
		for (size_t iter = 0; iter < lengthVector; ++iter) {
			value[iter] = vecData[iter];
		}
		Imf::addMultExpTimes(head, value);
	} else if (!strcmp(attrName, "multIsoSpeeds")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multIsoSpeeds', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		Imf::addMultIsoSpeeds(head, value);
	} else if (!strcmp(attrName, "multGains")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multGains', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		Imf::addMultGains(head, value);
	} else {
		mexErrMsgTxt("Setting this attribute not implemented.");
	}
}

void setAttribute(Imf::Header& head, const mxArray* mxstruct) {

	if (mxIsStruct(mxstruct)) {
		int numAttributes = mxGetNumberOfFields(mxstruct);
		for (int iterAttr = 0; iterAttr < numAttributes; ++iterAttr) {
			setAttribute(head, mxGetFieldNameByNumber(mxstruct, iterAttr), \
							mxGetFieldByNumber(mxstruct, 0, iterAttr));
		}
	} else if (!mxIsEmpty(mxstruct)) {
		mexErrMsgTxt("For setting multiple attributes, a struct array must be provided.");
	}
}

void EXRInputFile::readChannelRGBA(Imf::Array2D<FloatUsed> &rPixels, bool &rFlag, \
								Imf::Array2D<FloatUsed> &gPixels, bool &gFlag, \
								Imf::Array2D<FloatUsed> &bPixels, bool &bFlag, \
								Imf::Array2D<FloatUsed> &aPixels, bool &aFlag) const {

	int width, height;
	getDimensions(width, height);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	if (m_file.header().channels().findChannel("R")) {
		rFlag = true;
		rPixels.resizeErase(height, width);
		frameBuffer.insert("R", \
						Imf::Slice(USEDC, \
								(char *) (&rPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(rPixels[0][0]) * 1, \
								sizeof(rPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		rFlag = false;
	}

	if (m_file.header().channels().findChannel("G")) {
		gFlag = true;
		gPixels.resizeErase(height, width);
		frameBuffer.insert("G", \
						Imf::Slice(USEDC, \
								(char *) (&gPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(gPixels[0][0]) * 1, \
								sizeof(gPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		gFlag = false;
	}

	if (m_file.header().channels().findChannel("B")) {
		bFlag = true;
		bPixels.resizeErase(height, width);
		frameBuffer.insert("B", \
						Imf::Slice(USEDC, \
								(char *) (&bPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(bPixels[0][0]) * 1, \
								sizeof(bPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		bFlag = false;
	}

	if (m_file.header().channels().findChannel("A")) {
		aFlag = true;
		aPixels.resizeErase(height, width);
		frameBuffer.insert("A", \
						Imf::Slice(USEDC, \
								(char *) (&aPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(aPixels[0][0]) * 1, \
								sizeof(aPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		aFlag = false;
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);

}

void EXRInputFile::readChannelYA(Imf::Array2D<FloatUsed> &yPixels, bool &yFlag, \
								Imf::Array2D<FloatUsed> &aPixels, bool &aFlag) const {

	int width, height;
	getDimensions(width, height);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	if (m_file.header().channels().findChannel("Y")) {
		yFlag = true;
		yPixels.resizeErase(height, width);
		frameBuffer.insert("R", \
						Imf::Slice(USEDC, \
								(char *) (&yPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(yPixels[0][0]) * 1, \
								sizeof(yPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		yFlag = false;
	}

	if (m_file.header().channels().findChannel("A")) {
		aFlag = true;
		aPixels.resizeErase(height, width);
		frameBuffer.insert("A", \
						Imf::Slice(USEDC, \
								(char *) (&aPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(aPixels[0][0]) * 1, \
								sizeof(aPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		aFlag = false;
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);

}

void EXRInputFile::readChannel(Imf::Array2D<FloatUsed> &cPixels, bool &cFlag, \
							const std::string &cName) const {

	int width, height;
	getDimensions(width, height);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	if (m_file.header().channels().findChannel(cName.c_str())) {
		cFlag = true;
		cPixels.resizeErase(height, width);
		frameBuffer.insert("R", \
						Imf::Slice(USEDC, \
								(char *) (&cPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(cPixels[0][0]) * 1, \
								sizeof(cPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		cFlag = false;
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);

}


void EXRInputFile::readChannel(std::vector<Imf::Array2D<FloatUsed> > &cPixels, \
							std::vector<bool> &cFlags, \
							const std::vector<std::string> &cNames) const {

	int width, height;
	getDimensions(width, height);
	int numChannels = cNames.size();
	cPixels.resize(numChannels);
	cFlags.resize(numChannels, false);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	for (int iter = 0; iter < numChannels; ++iter) {
		if (m_file.header().channels().findChannel(cNames[iter].c_str())) {
			cFlags[iter] = true;
			cPixels[iter].resizeErase(height, width);
			frameBuffer.insert(cNames[iter].c_str(), \
							Imf::Slice(USEDC, \
									(char *) (&cPixels[iter][0][0] - dw.min.x - dw.min.y * width), \
									sizeof(cPixels[iter][0][0]) * 1, \
									sizeof(cPixels[iter][0][0]) * width, \
									1, \
									1, \
									FLT_MAX));
		}
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);
}

void EXROutputFile::writeChannelRGBA(const FloatUsed *rPixels, \
										const FloatUsed *gPixels, \
										const FloatUsed *bPixels, \
										const FloatUsed *aPixels) {

	std::vector<const FloatUsed *> cPixels;
	cPixels.push_back(rPixels);
	cPixels.push_back(gPixels);
	cPixels.push_back(bPixels);
	cPixels.push_back(aPixels);
	std::vector<const std::string> cNames;
	cNames.push_back(std::string("R"));
	cNames.push_back(std::string("G"));
	cNames.push_back(std::string("B"));
	cNames.push_back(std::string("A"));
	writeChannel(cPixels, cNames);
}

void EXROutputFile::writeChannelYA(const FloatUsed *yPixels, \
										const FloatUsed *aPixels) {

	std::vector<const FloatUsed *> cPixels;
	cPixels.push_back(yPixels);
	cPixels.push_back(aPixels);
	std::vector<const std::string> cNames;
	cNames.push_back(std::string("Y"));
	cNames.push_back(std::string("A"));
	writeChannel(cPixels, cNames);
}

void EXROutputFile::writeChannel(const FloatUsed *cPixels, \
									const std::string &cName) {

	std::vector<const FloatUsed *> ccPixels;
	ccPixels.push_back(cPixels);
	std::vector<const std::string> cNames;
	cNames.push_back(cName);
	writeChannel(ccPixels, cNames);
}

void EXROutputFile::writeChannel(const std::vector<const FloatUsed *> &cPixels, \
								const std::vector<const std::string> &cNames) {
	mexAssert((!m_createdFrameBuffer) && (!m_wroteFile));
	mexAssert(cPixels.size() == cNames.size());
	int channels = cPixels.size();
	int width, height;
	getDimensions(width, height);
	for (int iterChannel = 0; iterChannel < channels; ++iterChannel) {
		if (cPixels[iterChannel] != NULL) {
			m_frameBuffer.insert(cNames[iterChannel].c_str(), \
					Imf::Slice(USEDC, (char *) cPixels[iterChannel], \
					sizeof(*cPixels[iterChannel]) * 1, sizeof(*cPixels[iterChannel]) * width));
		}
	}
	m_createdFrameBuffer = true;
}

}	/* namespace nuance */
