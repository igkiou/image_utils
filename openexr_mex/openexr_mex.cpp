/*
 * openexr_mex.cpp
 *
 *  Created on: May 28, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"
#include <typeinfo>

namespace openexr {

/*
 * Input file handling.
 */
ExrInputFile::ExrInputFile(const mex::MxString& fileName):
						m_file(fileName.c_str()) {
	mexAssert(isValidFile()[0]);
}

mex::MxString ExrInputFile::getFileName() const {
	return mex::MxString(m_file.fileName());
}

mex::MxNumeric<bool> ExrInputFile::isValidFile() const {
	return mex::MxNumeric<bool>(Imf::isOpenExrFile(m_file.fileName()));
}

int ExrInputFile::getHeight() const {
	Imath::Box2i dw = m_file.header().dataWindow();
	return dw.max.y - dw.min.y + 1;
}

int ExrInputFile::getWidth() const {
	Imath::Box2i dw = m_file.header().dataWindow();
	return dw.max.x - dw.min.x + 1;
}

int ExrInputFile::getNumberOfChannels() const {
	int numChannels = 0;
	for (Imf::ChannelList::ConstIterator
			channelIter = m_file.header().channels().begin(),
			channelEnd = m_file.header().channels().end();
			channelIter != channelEnd;
			++channelIter) {
		++numChannels;
	}
	return numChannels;
}
mex::MxArray ExrInputFile::readDataRGB() {
	std::vector<mex::MxString> channelNames;
	channelNames.push_back(mex::MxString("R"));
	channelNames.push_back(mex::MxString("G"));
	channelNames.push_back(mex::MxString("B"));
	return readData(channelNames);
}

mex::MxArray ExrInputFile::readDataY() {
	return readData(std::vector<mex::MxString>(1, mex::MxString("Y")));
}

mex::MxArray ExrInputFile::readData(const mex::MxString &channelName) {
	return readData(std::vector<mex::MxString>(1, channelName));
}

mex::MxArray ExrInputFile::readData() {
	return readData(getChannelNames());
}

mex::MxArray ExrInputFile::readData(const std::vector<mex::MxString>& channelNames) {

	int width = getWidth();
	int height = getHeight();
	Imath::Box2i dw = m_file.header().dataWindow();
	int numChannels = channelNames.size();
	std::vector<int> dimensions;
	dimensions.push_back(width);
	dimensions.push_back(height);
	if (numChannels > 1) {
		dimensions.push_back(numChannels);
	}
	mex::MxNumeric<FloatUsed> pixelBuffer = mex::MxNumeric<FloatUsed>(
											static_cast<int>(
											dimensions.size()), &dimensions[0]);

	std::vector<bool> foundChannel;
	foundChannel.clear();

	Imf::FrameBuffer frameBuffer;
	for (int iter = 0; iter < numChannels; ++iter) {
		if (m_file.header().channels().findChannel(channelNames[iter].c_str())) {
			FloatUsed* tempBuffer = &pixelBuffer[iter * width * height];
			foundChannel.push_back(true);
			frameBuffer.insert(channelNames[iter].c_str(),
							Imf::Slice(kEXRFloatUsed,
									(char *) (tempBuffer - dw.min.x - dw.min.y * width),
									sizeof(*tempBuffer) * 1,
									sizeof(*tempBuffer) * width,
									1,
									1,
									FLT_MAX));
		} else {
			foundChannel.push_back(false);
		}
	}

	mexAssert(isComplete());
	Imath::Box2i dw = m_file.header().dataWindow();
	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);
	int width = getWidth();
	int height = getHeight();
	int numChannels = foundChannel.size();
	std::vector<int> transposePermutation;
	transposePermutation.push_back(2);
	transposePermutation.push_back(1);
	if (numChannels > 1) {
		transposePermutation.push_back(3);
	}
	mex::MxArray retArg = pixelBuffer.permute(transposePermutation).get_array();
	pixelBuffer.destroy();
	return retArg;
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

mex::MxArray ExrInputFile::get(const mex::MxString& attributeName) const {
	const Imf::Attribute& attribute = m_file.header()[attributeName.c_str()];
	const EExrAttributeType type = attributeTypeNameMap.find(std::string(attribute.typeName()));
	switch(type) {
		case EExrAttributeType::EChannelList: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::ChannelList>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::ECompression: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Compression>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::ELineOrder: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::LineOrder>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EEnvmap: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Envmap>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EString: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<std::string>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EChromaticities: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imf::Chromaticities>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EBox2f: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::Box2f>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EBox2i: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::Box2i>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EV2f: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::V2f>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EV2i: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<Imath::V2i>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EDouble: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<double>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EFloat: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<float>&>(
													attribute)).get_array());
		}
		case EExrAttributeType::EInt: {
			return mex::MxArray(toMxArray(
					static_cast<const Imf::TypedAttribute<int>&>(
													attribute)).get_array());
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return mex::MxArray();
		}
	}
}

mex::MxArray ExrInputFile::get() const {

	std::vector<std::string> nameVec;
	std::vector<mex::MxArray*> arrayVec;
	for (Imf::Header::ConstIterator iter = m_file.header().begin(),
		end = m_file.header().end();
		iter != end;
		++iter) {
		nameVec.push_back(std::string(iter.name()));
		mex::MxArray* tempAttributeArray = new mex::MxArray(
								get(mex::MxString(iter.name()))
								.get_array());
		arrayVec.push_back(tempAttributeArray);
	}
	mex::MxArray retArg(mex::MxStruct(nameVec, arrayVec).get_array());
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
ExrOutputFile::ExrOutputFile(const mex::MxString& fileName, int width,
							int height) :
						  m_header(width, height),
						  m_fileName(fileName.get_string()),
						  m_writtenFile(false) {	}

mex::MxString ExrOutputFile::getFileName() const {
	return mex::MxString(m_fileName);
}

int ExrOutputFile::getHeight() const {
	Imath::Box2i dw = m_header.dataWindow();
	return dw.max.y - dw.min.y + 1;
}

int ExrOutputFile::getWidth() const {
	Imath::Box2i dw = m_header.dataWindow();
	return dw.max.x - dw.min.x + 1;
}

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
	mexAssert((!m_createdFrameBuffer) && (!m_writtenFile));
	std::vector<int> dimensions = channelPixels.getDimensions();
	int width = getWidth();
	int height = getHeight();
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

void ExrOutputFile::writeData(const mex::MxString& fileName) {
	mexAssert((m_createdFrameBuffer) && (!m_writtenFile));
	int height = getHeight();
	Imf::OutputFile outFile(fileName.c_str(), m_header);
	outFile.setFrameBuffer(m_frameBuffer);
	outFile.writePixels(height);
	m_writtenFile = true;
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

void ExrOutputFile::set(const mex::MxArray& attribute,
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

void ExrOutputFile::set(const mex::MxStruct& attributes) {

	for (int iter = 0, numFields = attributes.getNumberOfFields();
		iter < numFields;
		++iter) {
		set(mex::MxArray(attributes[iter]), mex::MxString(attributes.getFieldName(iter)));
	}
}

}	/* namespace nuance */
