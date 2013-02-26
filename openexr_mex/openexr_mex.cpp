/*
 * openexr_mex.cpp
 *
 *  Created on: May 28, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

namespace exr {

/*
 * Input file handling.
 */

namespace {

void copyArray2DtoMxArray(mex::MxNumeric<FloatUsed>& destination,
						const Imf::Array2D<FloatUsed>* origin,
						const int width, const int height,
						const int numChannels) {
	std::vector<int> dimensions = destination.getDimensions();
	mexAssert((((numChannels == 1) && (dimensions.size() == 2)) ||
				((dimensions.size() == 3) && (dimensions[2] == numChannels))) &&
				(dimensions[0] == height) &&
				(dimensions[1] == width));
	for (int iterChannel = 0; iterChannel < numChannels; ++iterChannel) {
//		for (int iterWidth = 0; iterWidth < width; ++iterWidth) {
//			for (int iterRow = 0; iterRow < height; ++iterRow) {
//				destination[height * width * iterChannel
//				            + height * iterWidth
//				            + iterRow] =
//				static_cast<FloatUsed>(origin[iterChannel][iterRow][iterWidth]);
//			}
//		}
		memcpy(destination.data() + height * width * iterChannel,
			&origin[iterChannel][0],
			height * width * sizeof(FloatUsed));
	}

}

} /* namespace */

void EXRInputFile::readChannelRGB() const {
	std::vector<std::string> channelNames;
	channelNames.push_back(std::string("R"));
	channelNames.push_back(std::string("G"));
	channelNames.push_back(std::string("B"));
	readChannel(channelNames);
}

void EXRInputFile::readChannelY() const {
	readChannel(std::vector<std::string>(1, std::string("Y")));
}

void EXRInputFile::readChannel(const std::string &channelName) const {

	readChannel(std::vector<std::string>(1, channelName));
}

void EXRInputFile::readChannel(const std::vector<std::string>& channelNames) const {

	mexAssert((!m_createdFrameBuffer) && (!m_readFile));
	int width, height;
	getDimensions(width, height);
	Imath::Box2i dw = m_file.header().dataWindow();
	int numChannels = channelNames.size();
	m_pixelBuffer = new Imf::Array2D<FloatUsed>[numChannels];
	m_foundChannel.clear();

	for (int iter = 0; iter < numChannels; ++iter) {
		Imf::Array2D<FloatUsed>* tempBuffer = &m_pixelBuffer[iter];
		tempBuffer->resizeErase(height, width);
		if (m_file.header().channels().findChannel(channelNames[iter].c_str())) {
			m_foundChannel.push_back(true);
			m_frameBuffer.insert(channelNames[iter].c_str(),
							Imf::Slice(kEXRFloatUsed,
									(char *) (&((*tempBuffer)[0][0]) - dw.min.x - dw.min.y * width), \
									sizeof((*tempBuffer)[0][0]) * 1, \
									sizeof((*tempBuffer)[0][0]) * width, \
									1, \
									1, \
									FLT_MAX));
		} else {
			m_foundChannel.push_back(false);
		}
	}
	m_createdFrameBuffer = true;
}

mex::MxNumeric<FloatUsed> EXRInputFile::readFile() const {

	mexAssert((m_createdFrameBuffer) && (!m_readFile));
	Imath::Box2i dw = m_file.header().dataWindow();
	m_file.setFrameBuffer(m_frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);
	int width, height, numChannels;
	getDimensions(width, height);
	numChannels = m_foundChannel.size();
	mex::MxNumeric<FloatUsed> retArg;
	if (numChannels == 1) {
		retArg = mex::MxNumeric<FloatUsed>(height, width);
	} else {
		int dimensions[3];
		dimensions[0] = height;
		dimensions[1] = width;
		dimensions[2] = numChannels;
		retArg = mex::MxNumeric<FloatUsed>(3, dimensions);
	}
	copyArray2DtoMxArray(retArg, m_pixelBuffer, width, height, numChannels);
	m_readFile = true;
	return retArg;
}

namespace {

/*
 * Routines to convert an Attribute to MxArray.
 */

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

const mex::ConstMap<Imf::LineOrder, std::string> lineOrderTypeToString
	= mex::ConstMap<Imf::LineOrder, std::string>
	(Imf::INCREASING_Y,		"increasing_y")
	(Imf::DECREASING_Y,		"decreasing_y")
	(Imf::RANDOM_Y,			"random_y")
	(Imf::NUM_LINEORDERS,	"unknown");

const mex::ConstMap<Imf::Envmap, std::string> envmapTypeToString
	= mex::ConstMap<Imf::Envmap, std::string>
	(Imf::ENVMAP_LATLONG, 	"latlong")
	(Imf::ENVMAP_CUBE,		"cube")
	(Imf::NUM_ENVMAPTYPES,	"unknown");

// VT
template <typename T>
mex::MxNumeric<T> toMxArray(
	const Imf::TypedAttribute<T>& attribute) {
	return mex::MxNumeric<T>(attribute.value());
}

// V2T
template <typename T>
mex::MxNumeric<T> toMxArray<Imath::Vec2<T> >(
	const Imf::TypedAttribute<Imath::Vec2<T> >& attribute) {
	std::vector<T> temp;
	temp.push_back(attribute.value().x);
	temp.push_back(attribute.value().y);
	return mex::MxNumeric<T>(temp);
}

// VectorT
template <typename T>
mex::MxNumeric<T> toMxArray<std::vector<T> >(
	const Imf::TypedAttribute<std::vector<T> >& attribute) {
	return mex::MxNumeric<T>(attribute.value());
}

// Box2T
template <typename T>
mex::MxCell toMxArray(
	const Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > >& attribute) {
	std::vector<mex::MxArray *> temp;
	std::vector<T> tempV;
	tempV.push_back(attribute.value().min.x);
	tempV.push_back(attribute.value().min.y);
	temp.push_back(new mex::MxNumeric<T>(tempV));
	tempV.clear();
	tempV.push_back(attribute.value().max.x);
	tempV.push_back(attribute.value().max.y);
	temp.push_back(new mex::MxNumeric<T>(tempV));
	return mex::MxCell(temp);
}

// Chromaticities
mex::MxStruct toMxArray(
	const Imf::TypedAttribute<Imf::Chromaticities>& attribute) {
	std::vector<std::string> chromaticityNames;
	chromaticityNames.push_back(std::string("red"));
	chromaticityNames.push_back(std::string("green"));
	chromaticityNames.push_back(std::string("blue"));
	chromaticityNames.push_back(std::string("white"));
	std::vector<mex::MxNumeric<float>*> chromaticityValues;
	std::vector<float> temp(2, 0);
	temp[0] = attribute.value().red.x;
	temp[0] = attribute.value().red.y;
	chromaticityValues.push_back(new mex::MxNumeric<float>(temp));
	temp[0] = attribute.value().green.x;
	temp[0] = attribute.value().green.y;
	chromaticityValues.push_back(new mex::MxNumeric<float>(temp));
	temp[0] = attribute.value().blue.x;
	temp[0] = attribute.value().blue.y;
	chromaticityValues.push_back(new mex::MxNumeric<float>(temp));
	temp[0] = attribute.value().white.x;
	temp[0] = attribute.value().white.y;
	chromaticityValues.push_back(new mex::MxNumeric<float>(temp));
	for (int iter = 0, numChromaticities = chromaticityValues.size();
		iter < numChromaticities;
		++iter) {
		delete chromaticityValues[iter];
	}
	return mex::MxStruct(chromaticityNames, chromaticityValues);
}

// String
mex::MxString toMxArray(
	const Imf::TypedAttribute<std::string>& attribute) {
	return mex::MxString(attribute.value());
}

// Envmap
mex::MxString toMxArray(
	const Imf::TypedAttribute<Imf::Envmap>& attribute) {
	return mex::MxString(envmapTypeToString[attribute.value()]);
}

// LineOrder
mex::MxString toMxArray(
	const Imf::TypedAttribute<Imf::LineOrder>& attribute) {
	return mex::MxString(lineOrderTypeToString[attribute.value()]);
}

// Compression
mex::MxString toMxArray(
	const Imf::TypedAttribute<Imf::Compression>& attribute) {
	return mex::MxString(compressionTypeToString[attribute.value()]);
}

// ChannelList
mex::MxCell toMxArray(
	const Imf::TypedAttribute<Imf::ChannelList>& attribute) {
	std::vector<mex::MxArray*> channelNames;
	for (Imf::ChannelList::ConstIterator chIt = attribute.value().begin(), \
		chEnd = attribute.value().end(); \
		chIt != chEnd; \
		++chIt) {
		channelNames.push_back(new mex::MxString(chIt.name()));
	}
	for (int iter = 0, numChannels = channelNames.size();
		iter < numChannels;
		++iter) {
		delete channelNames[iter];
	}
	return mex::MxCell(channelNames);
}

} /* namespace */

mex::MxArray* EXRInputFile::getAttribute(const mex::MxString& attributeName) const {
	const Imf::Attribute& attribute = m_file.header[attributeName.c_str()];
	const EAttributeType type = stringToAttributeType(attribute.typeName());
	switch(type) {
		case EAttributeChannelList: {
			return new mex::MxCell(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imf::ChannelList>&>(
																attribute)));
		}
		case EAttributeCompression: {
			return new mex::MxString(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imf::Compression>&>(
																attribute)));
		}
		case EAttributeLineOrder: {
			return new mex::MxString(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imf::LineOrder>&>(
																attribute)));
		}
		case EAttributeEnvmap: {
			return new mex::MxString(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imf::Envmap>&>(
																attribute)));
		}
		case EAttributeString: {
			return new mex::MxString(toMxArray(
					dynamic_cast<Imf::TypedAttribute<std::string>&>(
																attribute)));
		}
		case EAttributeChromaticities: {
			return new mex::MxStruct(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imf::Chromaticities>&>(
																attribute)));
		}
		case EAttributeBox2d: {
			return new mex::MxCell(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imath::Box2d>&>(
																attribute)));
		}
		case EAttributeBox2f: {
			return new mex::MxCell(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imath::Box2f>&>(
																attribute)));
		}
		case EAttributeBox2i: {
			return new mex::MxCell(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imath::Box2i>&>(
																attribute)));
		}
		case EAttributeVectord: {
			return new mex::MxNumeric<double>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<std::vector<double> >&>(
																attribute)));
		}
		case EAttributeVectorf: {
			return new mex::MxNumeric<float>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<std::vector<float> >&>(
																attribute)));
		}
		case EAttributeVectori: {
			return new mex::MxNumeric<int>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<std::vector<int> >&>(
																attribute)));
		}
		case EAttributeV2d: {
			return new mex::MxNumeric<double>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imath::V2d>&>(
																attribute)));
		}
		case EAttributeV2f: {
			return new mex::MxNumeric<float>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imath::V2f>&>(
																attribute)));
		}
		case EAttributeV2i: {
			return new mex::MxNumeric<int>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<Imath::V2i>&>(
																attribute)));
		}
		case EAttributeDouble: {
			return new mex::MxNumeric<double>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<double>&>(
																attribute)));
		}
		case EAttributeFloat: {
			return new mex::MxNumeric<float>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<float>&>(
																attribute)));
		}
		case EAttributeInt: {
			return new mex::MxNumeric<int>(toMxArray(
					dynamic_cast<Imf::TypedAttribute<int>&>(
																attribute)));
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return NULL;
		}
	}
}

mex::MxArray* EXRInputFile::getAttribute() const {

	std::vector<std::string> nameVec;
	std::vector<mex::MxArray*> arrayVec;
	for (Imf::Header::ConstIterator attIt = m_file.header().begin(),
		attEnd = m_file.header().end(); attIt != attEnd; ++attIt) {
		nameVec.push_back(std::string(attIt.name()));
		arrayVec.push_back(getAttribute(mex::MxString(attIt.name())));
	}
	mex::MxArray* retArg = new mex::MxStruct(nameVec, arrayVec);
	for (int iter = 0, numAttributes = arrayVec.size();
		iter < numAttributes;
		++iter) {
		delete arrayVec[iter];
	}
	return retArg;
}


/*
 * Output file handling.
 */
void EXROutputFile::writeChannelRGB(const mex::MxNumeric<FloatUsed>& rgbPixels) {

	std::vector<std::string> rgbNames;
	rgbNames.push_back(std::string("R"));
	rgbNames.push_back(std::string("G"));
	rgbNames.push_back(std::string("B"));
	writeChannel(rgbPixels, rgbNames);
}

void EXROutputFile::writeChannelY(const mex::MxNumeric<FloatUsed>& yPixels) {
	writeChannel(yPixels, std::vector<std::string>(1, std::string("Y")));
}

void EXROutputFile::writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
									const std::string& channelName) {

	writeChannel(channelPixels, std::vector<std::string>(1, channelName));
}

void EXROutputFile::writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
									const std::vector<std::string>& channelNames) {
	mexAssert((!m_createdFrameBuffer) && (!m_wroteFile));
	std::vector<int> dimensions = channelPixels.getDimensions();
	int width, height;
	getDimensions(width, height);
	int numChannels = channelNames.size();
	mexAssert((((numChannels == 1) && (dimensions.size() == 2)) ||
					((dimensions.size() == 3) && (dimensions[2] == numChannels))) &&
					(dimensions[0] == height) &&
					(dimensions[1] == width));

	for (int iterChannel = 0; iterChannel < numChannels; ++iterChannel) {
		m_header.channels().insert(channelNames[iterChannel].c_str(),
								Imf::Channel(kEXRFloatUsed));
		m_frameBuffer.insert(channelNames[iterChannel].c_str(),
							Imf::Slice(kEXRFloatUsed,
									(char *) &channelPixels[width * height * iterChannel],
									sizeof(FloatUsed) * 1,
									sizeof(FloatUsed) * width));
	}
	m_createdFrameBuffer = true;
}

void EXROutputFile::writeFile(const mex::MxString& fileName) {
	mexAssert((m_createdFrameBuffer) && (!m_wroteFile));
	int height = getHeight();
	Imf::OutputFile outFile(fileName.c_str(), m_header);
	outFile.setFrameBuffer(m_frameBuffer);
	outFile.writePixels(height);
	m_wroteFile = true;
}

namespace {

/*
 * Routines to convert an MxArray to Attribute.
 */

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

const mex::ConstMap<std::string, Imf::LineOrder> stringToLineOrderType
	= mex::ConstMap<std::string, Imf::LineOrder>
	("increasing_y", 	Imf::INCREASING_Y)
	("decreasing_y",	Imf::DECREASING_Y)
	("random_y",		Imf::RANDOM_Y)
	("unknown",			Imf::NUM_LINEORDERS);

const mex::ConstMap<std::string, Imf::Envmap> stringToEnvmapType
	= mex::ConstMap<std::string, Imf::Envmap>
	("latlong", Imf::ENVMAP_LATLONG)
	("cube",	Imf::ENVMAP_CUBE)
	("unknown",	Imf::NUM_ENVMAPTYPES);

// VT
template <typename T>
Imf::TypedAttribute<T> toAttribute(
	const mex::MxNumeric<T>& mxNumeric) {
	return Imf::TypedAttribute<T>(mxNumeric[0]);
}

// V2T
template <typename T>
Imf::TypedAttribute<Imath::Vec2<T> > toAttribute<Imath::Vec2<T> >(
	const mex::MxNumeric<T>& mxNumeric) {
	mex::MxNumeric<T>* pTemp = (mex::MxNumeric<T>*) mxNumeric;
	return Imf::TypedAttribute<Imath::Vec2<T> >(
											Imath::Vec2<T>(mxNumeric[0],
														mxNumeric[1]));
}

// VectorT
template <typename T>
Imf::TypedAttribute<std::vector<T> > toAttribute<std::vector<T> >(
	const mex::MxNumeric<T>& mxNumeric) {
	return Imf::TypedAttribute<std::vector<T> >(mxNumeric.vectorize());
}

// Box2T
template <typename T>
Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > > toAttribute(
	const mex::MxCell& mxCell) {
	mex::MxNumeric<T> minArray = mex::MxNumeric<T>(mxCell[0]);
	Imath::Vec2<T> minVec(minArray[0], minArray[1]);
	mex::MxNumeric<T> maxArray = mex::MxNumeric<T>(mxCell[1]);
	Imath::Vec2<T> maxVec(maxArray[0], maxArray[1]);
	return Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > >(
								Imath::Box<Imath::Vec2<T> >(minVec, maxVec));
}

// Chromaticities
Imf::TypedAttribute<Imf::Chromaticities> toAttribute(
	const mex::MxStruct& mxStruct) {
	mex::MxNumeric<float> redMx(mxStruct[std::string("red")]);
	Imath::V2f red(redMx[0], redMx[1]);
	mex::MxNumeric<float> greenMx(mxStruct[std::string("green")]);
	Imath::V2f green(greenMx[0], greenMx[1]);
	mex::MxNumeric<float> blueMx(mxStruct[std::string("blue")]);
	Imath::V2f blue(blueMx[0], blueMx[1]);
	mex::MxNumeric<float> whiteMx(mxStruct[std::string("white")]);
	Imath::V2f white(whiteMx[0], whiteMx[1]);
	return Imf::Chromaticities(red, green, blue, white);
}

// String
template <typename T>
Imf::TypedAttribute<T> toAttribute(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<T>(mxString.string());
}

// Envmap
template <>
Imf::TypedAttribute<Imf::Envmap> toAttribute<Imf::Envmap>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::Envmap>(stringToEnvmapType(
															mxString.string()));
}

// LineOrder
template <>
Imf::TypedAttribute<Imf::LineOrder> toAttribute<Imf::LineOrder>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::LineOrder>(stringToLineOrderType(
															mxString.string()));
}

// Compression
template <>
Imf::TypedAttribute<Imf::Compression> toAttribute<Imf::Compression>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::Compression>(stringToCompressionType(
															mxString.string()));
}

// ChannelList
// Not supported.

} /* namespace */

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

void EXROutputFile::setAttribute(const mex::MxArray& attribute,
								const mex::MxString& attributeName) {

	std::map<std::string, EAttributeType>::const_iterator iteratorToType =
											std::map<std::string, EAttributeType>
											(registeredAttributeNameToAttributeType).
											find(attributeName.string());
	const EAttributeType type;
	if (iteratorToType != registeredAttributeNameToAttributeType.end()) {
		type = iteratorToType->second;
	} else {
		type = EAttributeString;
	}
	switch(type) {
		case EAttributeChannelList: {
			mexAssertEx(0, "Unsupported attribute type");
			break;
		}
		case EAttributeCompression: {
			mexAssert(attribute.m_kClass == mxCHAR_CLASS);
			m_header.insert(attributeName.c_str(), toAttribute<Imf::Compression>(
					dynamic_cast<mex::MxString&>(attribute)));
			break;
		}
		case EAttributeLineOrder: {
			mexAssert(attribute.m_kClass == mxCHAR_CLASS);
			m_header.insert(attributeName.c_str(), toAttribute<Imf::LineOrder>(
					dynamic_cast<mex::MxString&>(attribute)));
			break;
		}
		case EAttributeEnvmap: {
			mexAssert(attribute.m_kClass == mxCHAR_CLASS);
			m_header.insert(attributeName.c_str(), toAttribute<Imf::Envmap>(
					dynamic_cast<mex::MxString&>(attribute)));
			break;
		}
		case EAttributeString: {
			mexAssert(attribute.m_kClass == mxCHAR_CLASS);
			m_header.insert(attributeName.c_str(), toAttribute<std::string>(
					dynamic_cast<mex::MxString&>(attribute)));
			break;
		}
		case EAttributeChromaticities: {
			mexAssert((attribute.m_kClass == mxSTRUCT_CLASS) &&
					((dynamic_cast<mex::MxStruct>(attribute).isField(std::string("red"))) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("red")]).m_kClass == mex::MxClassId<float>) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("red")]).getNumberOfElements() == 2)) &&
					((dynamic_cast<mex::MxStruct>(attribute).isField(std::string("green"))) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("green")]).m_kClass == mex::MxClassId<float>) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("green")]).getNumberOfElements() == 2)) &&
					((dynamic_cast<mex::MxStruct>(attribute).isField(std::string("blue"))) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("blue")]).m_kClass == mex::MxClassId<float>) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("blue")]).getNumberOfElements() == 2)) &&
					((dynamic_cast<mex::MxStruct>(attribute).isField(std::string("white"))) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("white")]).m_kClass == mex::MxClassId<float>) &&
					(mex::MxArray((dynamic_cast<mex::MxStruct>(attribute))[std::string("white")]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute(
					dynamic_cast<mex::MxStruct&>(attribute)));
			break;
		}
		case EAttributeBox2d: {
			mexAssert((attribute.m_kClass == mxCELL_CLASS) &&
					(attribute.getNumberOfElements() == 2) &&
					((mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[0]).m_kClass == mex::MxClassId<double>) &&
					(mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[0]).getNumberOfElements() == 2)) &&
					((mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[1]).m_kClass == mex::MxClassId<double>) &&
					(mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[1]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute<double>(
					dynamic_cast<mex::MxCell&>(attribute)));
			break;
		}
		case EAttributeBox2f: {
			mexAssert((attribute.m_kClass == mxCELL_CLASS) &&
					(attribute.getNumberOfElements() == 2) &&
					((mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[0]).m_kClass == mex::MxClassId<float>) &&
					(mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[0]).getNumberOfElements() == 2)) &&
					((mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[1]).m_kClass == mex::MxClassId<float>) &&
					(mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[1]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute<float>(
					dynamic_cast<mex::MxCell&>(attribute)));
			break;
		}
		case EAttributeBox2i: {
			mexAssert((attribute.m_kClass == mxCELL_CLASS) &&
					(attribute.getNumberOfElements() == 2) &&
					((mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[0]).m_kClass == mex::MxClassId<int>) &&
					(mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[0]).getNumberOfElements() == 2)) &&
					((mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[1]).m_kClass == mex::MxClassId<int>) &&
					(mex::MxArray((dynamic_cast<mex::MxCell>(attribute))[1]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute<int>(
					dynamic_cast<mex::MxCell&>(attribute)));
			break;
		}
		case EAttributeV2d: {
			mexAssert((attribute.m_kClass == mex::MxClassId<double>()) &&
					(attribute.getNumberOfElements() == 2));
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<double> >(
					dynamic_cast<mex::MxNumeric<double>&>(attribute)));
			break;
		}
		case EAttributeV2f: {
			mexAssert((attribute.m_kClass == mex::MxClassId<float>()) &&
					(attribute.getNumberOfElements() == 2));
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<float> >(
					dynamic_cast<mex::MxNumeric<float>&>(attribute)));
			break;
		}
		case EAttributeV2i: {
			mexAssert((attribute.m_kClass == mex::MxClassId<int>()) &&
					(attribute.getNumberOfElements() == 2));
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<int> >(
					dynamic_cast<mex::MxNumeric<int>&>(attribute)));
			break;
		}
		case EAttributeVectord: {
			mexAssert(attribute.m_kClass == mex::MxClassId<double>());
			m_header.insert(attributeName.c_str(), toAttribute<std::vector<double> >(
					dynamic_cast<mex::MxNumeric<double>&>(attribute)));
			break;
		}
		case EAttributeVectorf: {
			mexAssert(attribute.m_kClass == mex::MxClassId<float>());
			m_header.insert(attributeName.c_str(), toAttribute<std::vector<float> >(
					dynamic_cast<mex::MxNumeric<float>&>(attribute)));
			break;
		}
		case EAttributeVectori: {
			mexAssert(attribute.m_kClass == mex::MxClassId<int>());
			m_header.insert(attributeName.c_str(), toAttribute<std::vector<int> >(
					dynamic_cast<mex::MxNumeric<int>&>(attribute)));
			break;
		}
		case EAttributeDouble: {
			mexAssert((attribute.m_kClass == mex::MxClassId<double>()) &&
					(attribute.getNumberOfElements() == 1));
			m_header.insert(attributeName.c_str(), toAttribute<double>(
					dynamic_cast<mex::MxNumeric<double>&>(attribute)));
			break;
		}
		case EAttributeFloat: {
			mexAssert((attribute.m_kClass == mex::MxClassId<float>()) &&
					(attribute.getNumberOfElements() == 1));
			m_header.insert(attributeName.c_str(), toAttribute<float>(
					dynamic_cast<mex::MxNumeric<float>&>(attribute)));
			break;
		}
		case EAttributeInt: {
			mexAssert((attribute.m_kClass == mex::MxClassId<int>()) &&
					(attribute.getNumberOfElements() == 1));
			m_header.insert(attributeName.c_str(), toAttribute<int>(
					dynamic_cast<mex::MxNumeric<int>&>(attribute)));
			break;
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			break;
		}
	}
}

void EXROutputFile::setAttribute(const mex::MxStruct& attributes) {

	for (int iter = 0, numElements = attributes.getNumberOfElements();
		iter < numElements;
		++iter) {
		mxClassID
		setAttribute(mex::MxArray(attributes[iter]), mex::MxString(attributes.getFieldName(iter)));
	}
}

}	/* namespace nuance */
