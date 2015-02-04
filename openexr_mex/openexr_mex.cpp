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
void ExrInputFile::readChannelRGB() {
	std::vector<std::string> channelNames;
	channelNames.push_back(std::string("R"));
	channelNames.push_back(std::string("G"));
	channelNames.push_back(std::string("B"));
	readChannel(channelNames);
}

void ExrInputFile::readChannelY() {
	readChannel(std::vector<std::string>(1, std::string("Y")));
}

void ExrInputFile::readChannel(const std::string &channelName) {

	readChannel(std::vector<std::string>(1, channelName));
}

void ExrInputFile::readChannel(const std::vector<std::string>& channelNames) {

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

mex::MxNumeric<FloatUsed> ExrInputFile::readFile() {

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

enum class EExrAttributeType {
	EChannelList = 0,
	ECompression,
	ELineOrder,
	EChromaticities,
	EEnvmap,
	EString,
	EBox2f,
	EBox2i,
	EV2f,
	EV2i,
	EDouble,
	EFloat,
	EInt,
	ELength,
	EInvalid = -1
};

mex::ConstBiMap<EExrAttributeType, std::string> attributeTypeNameMap =
	mex::ConstBiMap<EExrAttributeType, std::string>
	(EExrAttributeType::EChannelList, std::string(Imf::ChannelListAttribute::staticTypeName()))
	(EExrAttributeType::ECompression, std::string(Imf::CompressionAttribute::staticTypeName()))
	(EExrAttributeType::ELineOrder, std::string(Imf::LineOrderAttribute::staticTypeName()))
	(EExrAttributeType::EChromaticities, std::string(Imf::ChromaticitiesAttribute::staticTypeName()))
	(EExrAttributeType::EEnvmap, std::string(Imf::EnvmapAttribute::staticTypeName()))
	(EExrAttributeType::EString, std::string(Imf::StringAttribute::staticTypeName()))
	(EExrAttributeType::EBox2f, std::string(Imf::Box2fAttribute::staticTypeName()))
	(EExrAttributeType::EBox2i, std::string(Imf::Box2iAttribute::staticTypeName()))
	(EExrAttributeType::EV2f, std::string(Imf::V2fAttribute::staticTypeName()))
	(EExrAttributeType::EV2i, std::string(Imf::V2iAttribute::staticTypeName()))
	(EExrAttributeType::EDouble, std::string(Imf::TypedAttribute<double>::staticTypeName()))
	(EExrAttributeType::EFloat, std::string(Imf::FloatAttribute::staticTypeName()))
	(EExrAttributeType::EInt, std::string(Imf::TypedAttribute<int>::staticTypeName()))
	(EExrAttributeType::EInvalid, std::string("unknown"));

mex::ConstBiMap<Imf::Compression, std::string> compressionTypeNameMap =
	mex::ConstBiMap<Imf::Compression, std::string>
	(Imf::NO_COMPRESSION, std::string("no"))
	(Imf::RLE_COMPRESSION, std::string("rle"))
	(Imf::ZIPS_COMPRESSION, std::string("zips"))
	(Imf::ZIP_COMPRESSION, std::string("zip"))
	(Imf::PIZ_COMPRESSION, std::string("piz"))
	(Imf::PXR24_COMPRESSION, std::string("pxr24"))
	(Imf::B44_COMPRESSION, std::string("b44"))
	(Imf::B44A_COMPRESSION, std::string("b44a"))
	(Imf::NUM_COMPRESSION_METHODS, std::string("unknown"));

mex::ConstBiMap<Imf::LineOrder, std::string> lineOrderTypeNameMap =
	mex::ConstBiMap<Imf::LineOrder, std::string>
	(Imf::INCREASING_Y, std::string("increasing_y"))
	(Imf::DECREASING_Y,	std::string("decreasing_y"))
	(Imf::RANDOM_Y, std::string("random_y"))
	(Imf::NUM_LINEORDERS, std::string("unknown"));

mex::ConstBiMap<Imf::Envmap, std::string> envmapTypeNameMap =
	mex::ConstBiMap<Imf::Envmap, std::string>
	(Imf::ENVMAP_LATLONG, 	std::string("latlong"))
	(Imf::ENVMAP_CUBE,		std::string("cube"))
	(Imf::NUM_ENVMAPTYPES,	std::string("unknown"));

/*
 * Routines to convert an Attribute to MxArray.
 * TODO: Find reason why DoubleAttribute and IntAttribute are not in Imf.
 */

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
	return mex::MxString(envmapTypeNameMap[attribute.value()]);
}

// LineOrder
mex::MxString toMxArray(
	const Imf::TypedAttribute<Imf::LineOrder>& attribute) {
	return mex::MxString(lineOrderTypeNameMap[attribute.value()]);
}

// Compression
mex::MxString toMxArray(
	const Imf::TypedAttribute<Imf::Compression>& attribute) {
	return mex::MxString(compressionTypeNameMap[attribute.value()]);
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

mex::MxArray* ExrInputFile::getAttribute_sub(const mex::MxString& attributeName) const {
	const Imf::Attribute& attribute = m_file.header()[attributeName.c_str()];
	const EExrAttributeType type = attributeTypeNameMap.find(std::string(attribute.typeName()));
	switch(type) {
		case EExrAttributeType::EChannelList: {
			return new mex::MxCell(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::ChannelList>&>(
																attribute)));
		}
		case EExrAttributeType::ECompression: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Compression>&>(
																attribute)));
		}
		case EExrAttributeType::ELineOrder: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::LineOrder>&>(
																attribute)));
		}
		case EExrAttributeType::EEnvmap: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Envmap>&>(
																attribute)));
		}
		case EExrAttributeType::EString: {
			return new mex::MxString(toMxArray(
					static_cast<const Imf::TypedAttribute<std::string>&>(
																attribute)));
		}
		case EExrAttributeType::EChromaticities: {
			return new mex::MxStruct(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Chromaticities>&>(
																attribute)));
		}
		case EExrAttributeType::EBox2f: {
			return new mex::MxStruct(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::Box2f>&>(
																attribute)));
		}
		case EExrAttributeType::EBox2i: {
			return new mex::MxStruct(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::Box2i>&>(
																attribute)));
		}
		case EExrAttributeType::EV2f: {
			return new mex::MxNumeric<float>(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::V2f>&>(
																attribute)));
		}
		case EExrAttributeType::EV2i: {
			return new mex::MxNumeric<int>(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::V2i>&>(
																attribute)));
		}
		case EExrAttributeType::EDouble: {
			return new mex::MxNumeric<double>(toMxArray(
					static_cast<const Imf::TypedAttribute<double>&>(
																attribute)));
		}
		case EExrAttributeType::EFloat: {
			return new mex::MxNumeric<float>(toMxArray(
					static_cast<const Imf::TypedAttribute<float>&>(
																attribute)));
		}
		case EExrAttributeType::EInt: {
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

mex::MxArray* ExrInputFile::getAttribute_sub() const {

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

mex::MxArray ExrInputFile::getAttribute(const mex::MxString& attributeName)
										const {
	mex::MxArray* temp = getAttribute_sub(attributeName);
	mex::MxArray retArg(temp->get_array());
	delete temp;
	return retArg;
}

mex::MxArray ExrInputFile::getAttribute() const {
	mex::MxArray* temp = getAttribute_sub();
	mex::MxArray retArg(temp->get_array());
	delete temp;
	return retArg;
}

/*
 * Output file handling.
 */
void ExrOutputFile::writeChannelRGB(const mex::MxNumeric<FloatUsed>& rgbPixels) {

	std::vector<std::string> rgbNames;
	rgbNames.push_back(std::string("R"));
	rgbNames.push_back(std::string("G"));
	rgbNames.push_back(std::string("B"));
	writeChannel(rgbPixels, rgbNames);
}

void ExrOutputFile::writeChannelY(const mex::MxNumeric<FloatUsed>& yPixels) {
	writeChannel(yPixels, std::vector<std::string>(1, std::string("Y")));
}

void ExrOutputFile::writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
									const std::string& channelName) {

	writeChannel(channelPixels, std::vector<std::string>(1, channelName));
}

void ExrOutputFile::writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
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

void ExrOutputFile::writeFile(const mex::MxString& fileName) {
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
	return Imf::TypedAttribute<Imf::Envmap>(envmapTypeNameMap.find(
														mxString.get_string()));
}

// LineOrder
template <>
Imf::TypedAttribute<Imf::LineOrder> toAttribute<Imf::LineOrder>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::LineOrder>(lineOrderTypeNameMap.find(
														mxString.get_string()));
}

// Compression
template <>
Imf::TypedAttribute<Imf::Compression> toAttribute<Imf::Compression>(
	const mex::MxString& mxString) {
	return Imf::TypedAttribute<Imf::Compression>(compressionTypeNameMap.find(
														mxString.get_string()));
}

// ChannelList
// Not supported.

} /* namespace */

const mex::ConstMap<std::string, EExrAttributeType> registeredAttributeNameAttributeTypeMap
	= mex::ConstMap<std::string, EExrAttributeType>
	(std::string("gain"),				EExrAttributeType::EFloat)
	(std::string("wavelength"),			EExrAttributeType::EFloat)
	(std::string("extTube"),			EExrAttributeType::EString)
	(std::string("lens"),				EExrAttributeType::EString)
	(std::string("material"),			EExrAttributeType::EString)
	(std::string("chromaticities"),		EExrAttributeType::EChromaticities)
	(std::string("whiteLuminance"),		EExrAttributeType::EFloat)
	(std::string("adoptedNeutral"),		EExrAttributeType::EV2f)
	(std::string("renderingTransform"),	EExrAttributeType::EString)
	(std::string("lookModTransform"),	EExrAttributeType::EFloat)
	(std::string("xDensity"),			EExrAttributeType::EFloat)
	(std::string("owner"),				EExrAttributeType::EString)
	(std::string("comments"),			EExrAttributeType::EString)
	(std::string("capDate"),			EExrAttributeType::EString)
	(std::string("utcOffset"),			EExrAttributeType::EFloat)
	(std::string("longitude"),			EExrAttributeType::EFloat)
	(std::string("latitude"),			EExrAttributeType::EFloat)
	(std::string("altitude"),			EExrAttributeType::EFloat)
	(std::string("focus"),				EExrAttributeType::EFloat)
	(std::string("expTime"),			EExrAttributeType::EFloat)
	(std::string("aperture"),			EExrAttributeType::EFloat)
	(std::string("isoSpeed"),			EExrAttributeType::EFloat)
	(std::string("envmap"),				EExrAttributeType::EEnvmap);

void ExrOutputFile::setAttribute(const mex::MxArray& attribute,
								const mex::MxString& attributeName) {

	std::map<std::string, EExrAttributeType>::const_iterator iteratorToType =
											registeredAttributeNameAttributeTypeMap.
											get_map().
											find(attributeName.get_string());
	const EExrAttributeType type(
				(iteratorToType != registeredAttributeNameAttributeTypeMap.get_map().end())
				?(iteratorToType->second)
				:(EExrAttributeType::EString));
	switch(type) {
		case EExrAttributeType::EChannelList: {
			mexAssertEx(0, "Unsupported attribute type");
			break;
		}
		case EExrAttributeType::ECompression: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<Imf::Compression>(
							tempAttribute));
			break;
		}
		case EExrAttributeType::ELineOrder: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<Imf::LineOrder>(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EEnvmap: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<Imf::Envmap>(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EString: {
			const mex::MxString tempAttribute(attribute.get_array());
			m_header.insert(attributeName.c_str(), toAttribute<std::string>(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EChromaticities: {
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
		case EExrAttributeType::EBox2f: {
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
		case EExrAttributeType::EBox2i: {
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
		case EExrAttributeType::EV2f: {
			const mex::MxNumeric<float> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 2);
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<float> >(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EV2i: {
			const mex::MxNumeric<int> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 2);
			m_header.insert(attributeName.c_str(), toAttribute<Imath::Vec2<int> >(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EDouble: {
			const mex::MxNumeric<double> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 1);
			m_header.insert(attributeName.c_str(), toAttribute<double>(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EFloat: {
			const mex::MxNumeric<float> tempAttribute(attribute.get_array());
			mexAssert(tempAttribute.getNumberOfElements() == 1);
			m_header.insert(attributeName.c_str(), toAttribute<float>(
							tempAttribute));
			break;
		}
		case EExrAttributeType::EInt: {
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

void ExrOutputFile::setAttribute(const mex::MxStruct& attributes) {

	for (int iter = 0, numFields = attributes.getNumberOfFields();
		iter < numFields;
		++iter) {
		setAttribute(mex::MxArray(attributes[iter]), mex::MxString(attributes.getFieldName(iter)));
	}
}

}	/* namespace nuance */
