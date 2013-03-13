/*
 * openexr_mex.cpp
 *
 *  Created on: May 28, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"
#include <typeinfo>

namespace exr {

/*
 * Input file handling.
 */
void EXRInputFile::readChannelRGB() {
	std::vector<std::string> channelNames;
	channelNames.push_back(std::string("R"));
	channelNames.push_back(std::string("G"));
	channelNames.push_back(std::string("B"));
	readChannel(channelNames);
}

void EXRInputFile::readChannelY() {
	readChannel(std::vector<std::string>(1, std::string("Y")));
}

void EXRInputFile::readChannel(const std::string &channelName) {

	readChannel(std::vector<std::string>(1, channelName));
}

void EXRInputFile::readChannel(const std::vector<std::string>& channelNames) {

	mexAssert((!m_createdFrameBuffer) && (!m_readFile));
	int width, height;
	getDimensions(width, height);
	Imath::Box2i dw = m_file.header().dataWindow();
	int numChannels = channelNames.size();
	std::vector<int> dimensions;
	dimensions.push_back(width);
	dimensions.push_back(height);
	if (numChannels > 1) {
		dimensions.push_back(numChannels);
	}
	m_pixelBuffer = mex::MxNumeric<FloatUsed>(static_cast<int>(dimensions.size()), &dimensions[0]);
	m_foundChannel.clear();

	for (int iter = 0; iter < numChannels; ++iter) {
		if (m_file.header().channels().findChannel(channelNames[iter].c_str())) {
			FloatUsed* tempBuffer = &m_pixelBuffer[iter * width * height];
			m_foundChannel.push_back(true);
			m_frameBuffer.insert(channelNames[iter].c_str(),
							Imf::Slice(kEXRFloatUsed,
									(char *) (tempBuffer - dw.min.x - dw.min.y * width),
									sizeof(*tempBuffer) * 1,
									sizeof(*tempBuffer) * width,
									1,
									1,
									FLT_MAX));
		} else {
			m_foundChannel.push_back(false);
		}
	}
	m_createdFrameBuffer = true;
}

mex::MxNumeric<FloatUsed> EXRInputFile::readFile() {

	mexAssert((m_createdFrameBuffer) && (!m_readFile));
	mexAssert(isComplete());
	Imath::Box2i dw = m_file.header().dataWindow();
	m_file.setFrameBuffer(m_frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);
	int width, height, numChannels;
	getDimensions(width, height);
	numChannels = m_foundChannel.size();
	std::vector<int> transposePermutation;
	transposePermutation.push_back(2);
	transposePermutation.push_back(1);
	if (numChannels > 1) {
		transposePermutation.push_back(3);
	}
	m_readFile = true;
	return m_pixelBuffer.permute(transposePermutation);
}

namespace {

/*
 * Routines to convert an Attribute to MxArray.
 * TODO: Find reason why DoubleAttribute and IntAttribute are not in Imf.
 */

const mex::ConstMap<std::string, EAttributeType> stringToAttributeType
	= mex::ConstMap<std::string, EAttributeType>
	(std::string(Imf::ChannelListAttribute::staticTypeName()),		EAttributeChannelList)
	(std::string(Imf::CompressionAttribute::staticTypeName()),		EAttributeCompression)
	(std::string(Imf::LineOrderAttribute::staticTypeName()),		EAttributeLineOrder)
	(std::string(Imf::ChromaticitiesAttribute::staticTypeName()),	EAttributeChromaticities)
	(std::string(Imf::EnvmapAttribute::staticTypeName()),			EAttributeEnvmap)
	(std::string(Imf::StringAttribute::staticTypeName()),			EAttributeString)
	(std::string(Imf::Box2fAttribute::staticTypeName()),			EAttributeBox2f)
	(std::string(Imf::Box2iAttribute::staticTypeName()),			EAttributeBox2i)
	(std::string(Imf::V2fAttribute::staticTypeName()),				EAttributeV2f)
	(std::string(Imf::V2iAttribute::staticTypeName()),				EAttributeV2i)
	(std::string(Imf::VfAttribute::staticTypeName()),				EAttributeVectorf)
	(std::string(Imf::ViAttribute::staticTypeName()),				EAttributeVectori)
	(std::string(Imf::TypedAttribute<double>::staticTypeName()),	EAttributeDouble)
	(std::string(Imf::FloatAttribute::staticTypeName()),			EAttributeFloat)
	(std::string(Imf::TypedAttribute<int>::staticTypeName()),		EAttributeInt)
	(std::string("unknown"),										EAttributeTypeInvalid);

const mex::ConstMap<Imf::Compression, std::string> compressionTypeToString
	= mex::ConstMap<Imf::Compression, std::string>
	(Imf::NO_COMPRESSION, 			std::string("no"))
	(Imf::RLE_COMPRESSION,			std::string("rle"))
	(Imf::ZIPS_COMPRESSION,			std::string("zips"))
	(Imf::ZIP_COMPRESSION,			std::string("zip"))
	(Imf::PIZ_COMPRESSION,			std::string("piz"))
	(Imf::PXR24_COMPRESSION,		std::string("pxr24"))
	(Imf::B44_COMPRESSION,			std::string("b44"))
	(Imf::B44A_COMPRESSION,			std::string("b44a"))
	(Imf::NUM_COMPRESSION_METHODS,	std::string("unknown"));

const mex::ConstMap<Imf::LineOrder, std::string> lineOrderTypeToString
	= mex::ConstMap<Imf::LineOrder, std::string>
	(Imf::INCREASING_Y,		std::string("increasing_y"))
	(Imf::DECREASING_Y,		std::string("decreasing_y"))
	(Imf::RANDOM_Y,			std::string("random_y"))
	(Imf::NUM_LINEORDERS,	std::string("unknown"));

const mex::ConstMap<Imf::Envmap, std::string> envmapTypeToString
	= mex::ConstMap<Imf::Envmap, std::string>
	(Imf::ENVMAP_LATLONG, 	std::string("latlong"))
	(Imf::ENVMAP_CUBE,		std::string("cube"))
	(Imf::NUM_ENVMAPTYPES,	std::string("unknown"));

// VT
template <typename T>
mex::MxNumeric<T> toMxArray(
	const Imf::TypedAttribute<T>& attribute) {
	return mex::MxNumeric<T>(attribute.value());
}

// V2T
template <typename T>
mex::MxNumeric<T> toMxArray(
	const Imf::TypedAttribute<Imath::Vec2<T> >& attribute) {
	std::vector<T> temp;
	temp.push_back(attribute.value().x);
	temp.push_back(attribute.value().y);
	return mex::MxNumeric<T>(temp);
}

// VectorT
template <typename T>
mex::MxNumeric<T> toMxArray(
	const Imf::TypedAttribute<std::vector<T> >& attribute) {
//	std::vector<T> vecj = attribute.value();
//	std::cout << "length " << attribute.value().size() << std::endl;
//	for (int iter = 0, end = attribute.value().size(); iter < end; ++iter) {
//		std::cout << "iter " << iter << " value "<< attribute.value()[iter] << std::endl;
//	}
//	mex::MxNumeric<T> mxNumeric(attribute.value());
//	std::vector<T> veci = mxNumeric.vectorize();
//	std::cout << "length " << veci.size() << std::endl;
//	for (int iter = 0, end = veci.size(); iter < end; ++iter) {
//		std::cout << "iter " << iter << " value "<< veci[iter] << std::endl;
//	}
	return mex::MxNumeric<T>(attribute.value());
}

// Box2T
template <typename T>
mex::MxStruct toMxArray(
	const Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > >& attribute) {
	std::vector<std::string> boxNames;
	std::vector<mex::MxArray *> boxMx;

	/* min */
	boxNames.push_back(std::string("min"));
	std::vector<T> min;
	min.push_back(attribute.value().min.x);
	min.push_back(attribute.value().min.y);
	mex::MxNumeric<T> minMx(min);
	boxMx.push_back(&minMx);

	/* max */
	boxNames.push_back(std::string("max"));
	std::vector<T> max;
	max.push_back(attribute.value().max.x);
	max.push_back(attribute.value().max.y);
	mex::MxNumeric<T> maxMx(max);
	boxMx.push_back(&maxMx);

	return mex::MxStruct(boxNames, boxMx);
}

// Chromaticities
mex::MxStruct toMxArray(
	const Imf::TypedAttribute<Imf::Chromaticities>& attribute) {
	std::vector<std::string> chromaticityNames;
	std::vector<mex::MxArray*> chromaticityValues;

	/* red */
	chromaticityNames.push_back(std::string("red"));
	std::vector<float> red;
	red.push_back(attribute.value().red.x);
	red.push_back(attribute.value().red.y);
	mex::MxNumeric<float> redMx(red);
	chromaticityValues.push_back(&redMx);

	/* green */
	chromaticityNames.push_back(std::string("green"));
	std::vector<float> green;
	green.push_back(attribute.value().green.x);
	green.push_back(attribute.value().green.y);
	mex::MxNumeric<float> greenMx(green);
	chromaticityValues.push_back(&greenMx);

	/* blue */
	chromaticityNames.push_back(std::string("blue"));
	std::vector<float> blue;
	blue.push_back(attribute.value().blue.x);
	blue.push_back(attribute.value().blue.y);
	mex::MxNumeric<float> blueMx(blue);
	chromaticityValues.push_back(&blueMx);

	/* white */
	chromaticityNames.push_back(std::string("white"));
	std::vector<float> white;
	white.push_back(attribute.value().white.x);
	white.push_back(attribute.value().white.y);
	mex::MxNumeric<float> whiteMx(white);
	chromaticityValues.push_back(&whiteMx);

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
	for (Imf::ChannelList::ConstIterator iter = attribute.value().begin(),
		end = attribute.value().end();
		iter != end;
		++iter) {
		channelNames.push_back(new mex::MxString(iter.name()));
	}
	mex::MxCell retArg(channelNames);
	for (int iter = 0, numChannels = channelNames.size();
		iter < numChannels;
		++iter) {
		delete channelNames[iter];
	}
	return retArg;
}

} /* namespace */

mex::MxArray* EXRInputFile::getAttribute_sub(const mex::MxString& attributeName) const {
	const Imf::Attribute& attribute = m_file.header()[attributeName.c_str()];
	const EAttributeType type = stringToAttributeType[std::string(attribute.typeName())];
	switch(type) {
		case EAttributeChannelList: {
			return new mex::MxCell(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::ChannelList>&>(
																attribute)));
		}
		case EAttributeCompression: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Compression>&>(
																attribute)));
		}
		case EAttributeLineOrder: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::LineOrder>&>(
																attribute)));
		}
		case EAttributeEnvmap: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Envmap>&>(
																attribute)));
		}
		case EAttributeString: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<std::string>&>(
																attribute)));
		}
		case EAttributeChromaticities: {
			return new mex::MxStruct(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Chromaticities>&>(
																attribute)));
		}
		case EAttributeBox2f: {
			return new mex::MxStruct(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::Box2f>&>(
																attribute)));
		}
		case EAttributeBox2i: {
			return new mex::MxStruct(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::Box2i>&>(
																attribute)));
		}
		case EAttributeVectorf: {
			return new mex::MxNumeric<float>(toMxArray(
					static_cast<const Imf::TypedAttribute<std::vector<float> >&>(
																attribute)));
		}
		case EAttributeVectori: {
			return new mex::MxNumeric<int>(toMxArray(
					static_cast<const Imf::TypedAttribute<std::vector<int> >&>(
																attribute)));
		}
		case EAttributeV2f: {
			return new mex::MxNumeric<float>(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::V2f>&>(
																attribute)));
		}
		case EAttributeV2i: {
			return new mex::MxNumeric<int>(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::V2i>&>(
																attribute)));
		}
		case EAttributeDouble: {
			return new mex::MxNumeric<double>(toMxArray(
					static_cast<const Imf::TypedAttribute<double>&>(
																attribute)));
		}
		case EAttributeFloat: {
			return new mex::MxNumeric<float>(toMxArray(
					static_cast<const Imf::TypedAttribute<float>&>(
																attribute)));
		}
		case EAttributeInt: {
			return new mex::MxNumeric<int>(toMxArray(
					static_cast<const Imf::TypedAttribute<int>&>(
																attribute)));
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return NULL;
		}
	}
}

mex::MxArray* EXRInputFile::getAttribute_sub() const {

	std::vector<std::string> nameVec;
	std::vector<mex::MxArray*> arrayVec;
	for (Imf::Header::ConstIterator iter = m_file.header().begin(),
		end = m_file.header().end();
		iter != end;
		++iter) {
		nameVec.push_back(std::string(iter.name()));
		arrayVec.push_back(getAttribute_sub(mex::MxString(iter.name())));
	}
	mex::MxArray* retArg = new mex::MxStruct(nameVec, arrayVec);
	for (int iter = 0, numAttributes = arrayVec.size();
		iter < numAttributes;
		++iter) {
		delete arrayVec[iter];
	}
	return retArg;
}

mex::MxArray EXRInputFile::getAttribute(const mex::MxString& attributeName)
										const {
	mex::MxArray* temp = getAttribute_sub(attributeName);
	mex::MxArray retArg(temp->get_array());
	delete temp;
	return retArg;
}

mex::MxArray EXRInputFile::getAttribute() const {
	mex::MxArray* temp = getAttribute_sub();
	mex::MxArray retArg(temp->get_array());
	delete temp;
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

	std::vector<int> transposePermutation;
	transposePermutation.push_back(2);
	transposePermutation.push_back(1);
	if (numChannels > 1) {
		transposePermutation.push_back(3);
	}
	m_pixelBuffer = channelPixels.permute(transposePermutation);
	for (int iterChannel = 0; iterChannel < numChannels; ++iterChannel) {
		m_header.channels().insert(channelNames[iterChannel].c_str(),
								Imf::Channel(kEXRFloatUsed));
		m_frameBuffer.insert(channelNames[iterChannel].c_str(),
							Imf::Slice(kEXRFloatUsed,
									(char *) &m_pixelBuffer[
									              width * height * iterChannel],
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
	(EAttributeChannelList, 	std::string(Imf::ChannelListAttribute::staticTypeName()))
	(EAttributeCompression,		std::string(Imf::CompressionAttribute::staticTypeName()))
	(EAttributeLineOrder,		std::string(Imf::LineOrderAttribute::staticTypeName()))
	(EAttributeChromaticities,	std::string(Imf::ChromaticitiesAttribute::staticTypeName()))
	(EAttributeEnvmap,			std::string(Imf::EnvmapAttribute::staticTypeName()))
	(EAttributeString,			std::string(Imf::StringAttribute::staticTypeName()))
	(EAttributeBox2f,			std::string(Imf::Box2fAttribute::staticTypeName()))
	(EAttributeBox2i,			std::string(Imf::Box2iAttribute::staticTypeName()))
	(EAttributeV2f,				std::string(Imf::V2fAttribute::staticTypeName()))
	(EAttributeV2i,				std::string(Imf::V2iAttribute::staticTypeName()))
	(EAttributeVectorf,			std::string(Imf::VfAttribute::staticTypeName()))
	(EAttributeVectori,			std::string(Imf::ViAttribute::staticTypeName()))
	(EAttributeDouble,			std::string(Imf::TypedAttribute<double>::staticTypeName()))
	(EAttributeFloat,			std::string(Imf::FloatAttribute::staticTypeName()))
	(EAttributeInt,				std::string(Imf::TypedAttribute<int>::staticTypeName()))
	(EAttributeTypeInvalid,		std::string("unknown"));

const mex::ConstMap<std::string, Imf::Compression> stringToCompressionType
	= mex::ConstMap<std::string, Imf::Compression>
	(std::string("no"), 		Imf::NO_COMPRESSION)
	(std::string("rle"),		Imf::RLE_COMPRESSION)
	(std::string("zips"),		Imf::ZIPS_COMPRESSION)
	(std::string("zip"),		Imf::ZIP_COMPRESSION)
	(std::string("piz"),		Imf::PIZ_COMPRESSION)
	(std::string("pxr24"),		Imf::PXR24_COMPRESSION)
	(std::string("b44"),		Imf::B44_COMPRESSION)
	(std::string("b44a"),		Imf::B44A_COMPRESSION)
	(std::string("unknown"),	Imf::NUM_COMPRESSION_METHODS);

const mex::ConstMap<std::string, Imf::LineOrder> stringToLineOrderType
	= mex::ConstMap<std::string, Imf::LineOrder>
	(std::string("increasing_y"), 	Imf::INCREASING_Y)
	(std::string("decreasing_y"),	Imf::DECREASING_Y)
	(std::string("random_y"),		Imf::RANDOM_Y)
	(std::string("unknown"),			Imf::NUM_LINEORDERS);

const mex::ConstMap<std::string, Imf::Envmap> stringToEnvmapType
	= mex::ConstMap<std::string, Imf::Envmap>
	(std::string("latlong"), Imf::ENVMAP_LATLONG)
	(std::string("cube"),	Imf::ENVMAP_CUBE)
	(std::string("unknown"),	Imf::NUM_ENVMAPTYPES);

// VT
template <typename T>
Imf::TypedAttribute<T> toAttribute(
	const mex::MxNumeric<double>& mxNumeric) {
	return Imf::TypedAttribute<T>(mxNumeric[0]);
}

template <typename T>
Imf::TypedAttribute<T> toAttribute(
	const mex::MxNumeric<float>& mxNumeric) {
	return Imf::TypedAttribute<T>(mxNumeric[0]);
}

template <typename T>
Imf::TypedAttribute<T> toAttribute(
	const mex::MxNumeric<int>& mxNumeric) {
	return Imf::TypedAttribute<T>(mxNumeric[0]);
}

// V2T
template <>
Imf::TypedAttribute<Imath::Vec2<float> > toAttribute<Imath::Vec2<float> >(
	const mex::MxNumeric<float>& mxNumeric) {
	return Imf::TypedAttribute<Imath::Vec2<float> >(
											Imath::Vec2<float>(mxNumeric[0],
														mxNumeric[1]));
}

template <>
Imf::TypedAttribute<Imath::Vec2<int> > toAttribute<Imath::Vec2<int> >(
	const mex::MxNumeric<int>& mxNumeric) {
	return Imf::TypedAttribute<Imath::Vec2<int> >(
											Imath::Vec2<int>(mxNumeric[0],
														mxNumeric[1]));
}

// VectorT
template <>
Imf::TypedAttribute<std::vector<float> > toAttribute<std::vector<float> >(
	const mex::MxNumeric<float>& mxNumeric) {
//	std::vector<float> veci = mxNumeric.vectorize();
//	std::cout << "length " << veci.size() << std::endl;
//	for (int iter = 0, end = veci.size(); iter < end; ++iter) {
//		std::cout << "iter " << iter << " value "<< veci[iter] << std::endl;
//	}
//
//	Imf::TypedAttribute<std::vector<float> > attr(mxNumeric.vectorize());
//	std::vector<float> vecj = attr.value();
//	std::cout << "length " << vecj.size() << std::endl;
//	for (int iter = 0, end = vecj.size(); iter < end; ++iter) {
//		std::cout << "iter " << iter << " value "<< vecj[iter] << std::endl;
//	}
	return Imf::TypedAttribute<std::vector<float> >(mxNumeric.vectorize());
}

template <>
Imf::TypedAttribute<std::vector<int> > toAttribute<std::vector<int> >(
	const mex::MxNumeric<int>& mxNumeric) {
	return Imf::TypedAttribute<std::vector<int> >(mxNumeric.vectorize());
}

// Box2T
template <typename T>
Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > > toAttribute(
	const mex::MxStruct& mxStruct) {
	mex::MxNumeric<T> minMx = mex::MxNumeric<T>(mxStruct[std::string("min")]);
	Imath::Vec2<T> min(minMx[0], minMx[1]);
	mex::MxNumeric<T> maxMx = mex::MxNumeric<T>(mxStruct[std::string("max")]);
	Imath::Vec2<T> max(maxMx[0], maxMx[1]);
	return Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > >(
								Imath::Box<Imath::Vec2<T> >(min, max));
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
	return Imf::TypedAttribute<T>(mxString.get_string());
}

// Envmap
template <>
Imf::TypedAttribute<Imf::Envmap> toAttribute<Imf::Envmap>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::Envmap>(stringToEnvmapType[
															mxString.get_string()]);
}

// LineOrder
template <>
Imf::TypedAttribute<Imf::LineOrder> toAttribute<Imf::LineOrder>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::LineOrder>(stringToLineOrderType[
															mxString.get_string()]);
}

// Compression
template <>
Imf::TypedAttribute<Imf::Compression> toAttribute<Imf::Compression>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::Compression>(stringToCompressionType[
															mxString.get_string()]);
}

// ChannelList
// Not supported.

} /* namespace */

const mex::ConstMap<std::string, EAttributeType> registeredAttributeNameToAttributeType
	= mex::ConstMap<std::string, EAttributeType>
	(std::string("gain"),				EAttributeFloat)
	(std::string("wavelength"),			EAttributeFloat)
	(std::string("extTube"),			EAttributeString)
	(std::string("lens"),				EAttributeString)
	(std::string("material"),			EAttributeString)
	(std::string("chromaticities"),		EAttributeChromaticities)
	(std::string("whiteLuminance"),		EAttributeFloat)
	(std::string("adoptedNeutral"),		EAttributeV2f)
	(std::string("renderingTransform"),	EAttributeString)
	(std::string("lookModTransform"),	EAttributeFloat)
	(std::string("xDensity"),			EAttributeFloat)
	(std::string("owner"),				EAttributeString)
	(std::string("comments"),			EAttributeString)
	(std::string("capDate"),			EAttributeString)
	(std::string("utcOffset"),			EAttributeFloat)
	(std::string("longitude"),			EAttributeFloat)
	(std::string("latitude"),			EAttributeFloat)
	(std::string("altitude"),			EAttributeFloat)
	(std::string("focus"),				EAttributeFloat)
	(std::string("expTime"),			EAttributeFloat)
	(std::string("aperture"),			EAttributeFloat)
	(std::string("isoSpeed"),			EAttributeFloat)
	(std::string("multExpTimes"),		EAttributeVectorf)
	(std::string("multApertures"),		EAttributeVectorf)
	(std::string("multIsoSpeeds"),		EAttributeVectorf)
	(std::string("multGains"),			EAttributeVectorf)
	(std::string("envmap"),				EAttributeEnvmap);

void EXROutputFile::setAttribute(const mex::MxArray& attribute,
								const mex::MxString& attributeName) {

	std::map<std::string, EAttributeType>::const_iterator iteratorToType =
											registeredAttributeNameToAttributeType.
											get_map().
											find(attributeName.get_string());
	const EAttributeType type(
				(iteratorToType != registeredAttributeNameToAttributeType.get_map().end())
				?(iteratorToType->second)
				:(EAttributeString));
	switch(type) {
		case EAttributeChannelList: {
			mexAssertEx(0, "Unsupported attribute type");
			break;
		}
		case EAttributeCompression: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<Imf::Compression>(
							tempAttribute));
			break;
		}
		case EAttributeLineOrder: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<Imf::LineOrder>(
							tempAttribute));
			break;
		}
		case EAttributeEnvmap: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<Imf::Envmap>(
							tempAttribute));
			break;
		}
		case EAttributeString: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<std::string>(
							tempAttribute));
			break;
		}
		case EAttributeChromaticities: {
			const mex::MxStruct tempAttribute(attribute.get_array());
			mexAssert(
					(tempAttribute.getNumberOfFields() == 4) &&
					((tempAttribute.isField(std::string("red"))) &&
					(mex::MxNumeric<float>(tempAttribute[std::string("red")]).getNumberOfElements() == 2)) &&
					((tempAttribute.isField(std::string("green"))) &&
					(mex::MxNumeric<float>(tempAttribute[std::string("green")]).getNumberOfElements() == 2)) &&
					((tempAttribute.isField(std::string("blue"))) &&
					(mex::MxNumeric<float>(tempAttribute[std::string("blue")]).getNumberOfElements() == 2)) &&
					((tempAttribute.isField(std::string("white"))) &&
					(mex::MxNumeric<float>(tempAttribute[std::string("white")]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute(
							tempAttribute));
			break;
		}
		case EAttributeBox2f: {
			const mex::MxStruct tempAttribute(attribute.get_array());
			mexAssert(
					(tempAttribute.getNumberOfFields() == 1) &&
					((tempAttribute.isField(std::string("min"))) &&
					(mex::MxNumeric<float>(tempAttribute[std::string("min")]).getNumberOfElements() == 2)) &&
					((tempAttribute.isField(std::string("max"))) &&
					(mex::MxNumeric<float>(tempAttribute[std::string("max")]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute<float>(
							tempAttribute));
			break;
		}
		case EAttributeBox2i: {
			const mex::MxStruct tempAttribute(attribute.get_array());
			mexAssert(
					(tempAttribute.getNumberOfFields() == 1) &&
					((tempAttribute.isField(std::string("min"))) &&
					(mex::MxNumeric<int>(tempAttribute[std::string("min")]).getNumberOfElements() == 2)) &&
					((tempAttribute.isField(std::string("max"))) &&
					(mex::MxNumeric<int>(tempAttribute[std::string("max")]).getNumberOfElements() == 2))
			);
			m_header.insert(attributeName.c_str(), toAttribute<int>(
							tempAttribute));
			break;
		}
		case EAttributeV2f: {
			const mex::MxNumeric<float> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 2);
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<float> >(
							tempAttribute));
			break;
		}
		case EAttributeV2i: {
			const mex::MxNumeric<int> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 2);
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<int> >(
							tempAttribute));
			break;
		}
		case EAttributeVectorf: {
			const mex::MxNumeric<float> tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<std::vector<float> >(
							tempAttribute));
			break;
		}
		case EAttributeVectori: {
			const mex::MxNumeric<int> tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<std::vector<int> >(
							tempAttribute));
			break;
		}
		case EAttributeDouble: {
			const mex::MxNumeric<double> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 1);
			m_header.insert(attributeName.c_str(), toAttribute<double>(
							tempAttribute));
			break;
		}
		case EAttributeFloat: {
			const mex::MxNumeric<float> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 1);
			m_header.insert(attributeName.c_str(), toAttribute<float>(
							tempAttribute));
			break;
		}
		case EAttributeInt: {
			const mex::MxNumeric<int> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 1);
			m_header.insert(attributeName.c_str(), toAttribute<int>(
							tempAttribute));
			break;
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			break;
		}
	}
}

void EXROutputFile::setAttribute(const mex::MxStruct& attributes) {

	for (int iter = 0, numFields = attributes.getNumberOfFields();
		iter < numFields;
		++iter) {
		setAttribute(mex::MxArray(attributes[iter]), mex::MxString(attributes.getFieldName(iter)));
	}
}

}	/* namespace nuance */
