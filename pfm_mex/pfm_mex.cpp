/*
 * image_utils//image_utils/pfm_mex/pfm_mex.cpp/pfm_mex.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include "pfm_mex.h"

namespace pfm {

namespace {

static EByteOrder getHostByteOrder() {
	union {
		uint8_t  charValue[2];
		uint16_t shortValue;
	};
	charValue[0] = 1;
	charValue[1] = 0;

	return (shortValue == 1)?(ELittleEndian):(EBigEndian);
}

} /* namespace */

inline mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName) {
	return PFMHeader::read(std::ifstream(fileName.get_string(),
										std::ifstream::in |
										std::ifstream::binary));
}

/*
 * PFMHeader implementation.
 */
PFMHeader::PFMHeader()
			: m_colorFormat(),
			  m_width(),
			  m_height(),
			  m_scale(),
			  m_byteOrder() {	}

void PFMHeader::build(const EColorFormat colorFormat,
					const int width,
					const int height,
					const float scale,
					const EByteOrder byteOrder) {
	mexAssert((colorFormat == ERGB) || (colorFormat == EGrayscale));
	m_colorFormat = colorFormat;
	mexAssert(width > 0);
	m_width = width;
	mexAssert(height > 0);
	m_height = height;
	mexAssert(scale > 0);
	m_height = height;
	mexAssert((byteOrder == ELittleEndian) || (byteOrder == EBigEndian));
	m_byteOrder = byteOrder;
}

bool PFMHeader::read(std::ifstream& stream) {
	char format[2];
	stream.read(format, 2);
	mexAssert((format[0] == 'P') &&
			(format[1] == 'F' || format[1] == 'f'));
	EColorFormat colorFormat = (format[1] == 'F')?(ERGB):(EGrayscale);
	mexAssert(stream);

	char whitespace;
	stream.get(whitespace);
	mexAssert(std::isspace(whitespace));

	int width;
	stream >> width;
	mexAssert(stream);

	stream.get(whitespace);
	mexAssert(whitespace == ' ');
	mexAssert(stream);

	int height;
	stream >> height;
	mexAssert(height > 0);
	mexAssert(stream);

	stream.get(whitespace);
	mexAssert(whitespace == ' ');
	mexAssert(stream);

	float scaledByteOrder;
	stream >> scaledByteOrder;
	float scale = std::abs(scaledByteOrder);
	EByteOrder byteOrder = (scaledByteOrder < 0)?(ELittleEndian):(EBigEndian);
	mexAssert(stream);

	stream.get(whitespace);
	mexAssert(std::isspace(whitespace));
	mexAssert(stream);

	build(colorFormat, width, height, scale, byteOrder);
}

void PFMHeader::write(std::ofstream& stream) const {
	stream << 'P' << (m_colorFormat == ERGB)?('F'):('f') << '\n';
	stream << m_width << ' ' << m_height << '\n';
	stream << m_scale * (m_byteOrder == ELittleEndian)?(-1):(1) << '\n';
}

mex::MxStruct PFMHeader::toMxArray() const {
	std::vector<std::string> fieldNames;
	fieldNames.push_back(std::string("colorformat"));
	fieldNames.push_back(std::string("width"));
	fieldNames.push_back(std::string("height"));
	fieldNames.push_back(std::string("scale"));
	fieldNames.push_back(std::string("byteorder"));
	std::vector<mex::MxArray*> fieldValues;
	fieldValues.push_back(new mex::MxString(
							(m_colorFormat == ERGB)?("rgb"):("grayscale")));
	fieldValues.push_back(new mex::MxNumeric<int>(m_width));
	fieldValues.push_back(new mex::MxNumeric<int>(m_height));
	fieldValues.push_back(new mex::MxNumeric<float>(m_scale));
	fieldValues.push_back(new mex::MxString((m_byteOrder == ELittleEndian)?
											("littleendian"):
											("bigendian")));
	mex::MxStruct retArg(fieldNames, fieldValues);
	for (int iter = 0, end = fieldValues.size(); iter < end; ++iter) {
		delete fieldValues[iter];
	}
	return retArg;
}

/*
 * PFMInputFile implementation.
 */
explicit PFMInputFile::PFMInputFile(const mex::MxString& fileName)
								: m_file(fileName.c_str(),
										std::ifstream::in |
										std::ifstream::binary),
								  m_header(),
								  m_readHeader(false),
								  m_readFile(false) {
	mexAssert(m_file);
}

mex::MxStruct PFMInputFile::readHeader() const {
	mexAssert((!m_readHeader) && (!m_readFile));
	m_header.read(m_file);
	m_readHeader = true;
	return m_header.toMxArray();
}

mex::MxNumeric<float> PFMInputFile::readFile() const {
	mexAssert((m_readHeader) && (!m_readFile));
	int numChannels;
	std::vector<int> dimensions;
	dimensions.push_back(m_header.get_height());
	dimensions.push_back(m_header.get_width());
	if (m_header.get_colorFormat() == ERGB) {
		numChannels = 3;
		dimensions.push_back(numChannels);
	} else {
		numChannels = 1;
	}
	int numPixels = m_header.get_width() * m_header.get_height() *
					numChannels;
	mex::MxNumeric<float> pixels(3, &dimensions[0]);
	float* pixelBuffer = pixels.getData();

	m_file.read(static_cast<char*>(pixelBuffer), numPixels);
	mexAssert(m_file);
	std::vector<int> permutationVector;
	if (m_header.get_colorFormat() == ERGB) {
		permutationVector.push_back(3);
	}
	permutationVector.push_back(2);
	permutationVector.push_back(1);
	m_readFile = 0;
	return pixels.permute(permutationVector);
}

PFMInputFile::~PFMInputFile() {
	m_file.close();
}

/*
 * PFMOutputFile implementation.
 */
explicit PFMOutputFile::PFMOutputFile(const mex::MxString& fileName)
			: m_file(fileName.c_str(),
					std::ofstream::out | std::ofstream::binary),
			  m_header() {
	mexAssert(m_file);
}

void PFMOutputFile::writeFile(mex::MxNumeric<float>& pixels) const {
	std::vector<int> dimensions = pixels.getDimensions();
	mexAssert((dimensions.size() == 2) ||
			((dimensions.size() == 3) &&
			((dimensions[2] == 3) || (dimensions[2] == 1))));
	EColorFormat colorFormat = ((dimensions.size() == 2) ||
								(dimensions[2] == 1))?(EGrayscale):(ERGB);
	int width = dimensions[2];
	int height = dimensions[1];
	float scale = 1;
	EByteOrder byteOrder = getHostByteOrder();
	m_header.build(colorFormat, width, height, scale, byteOrder);
	m_header.write(m_file);
	std::vector<int> permutationVector;
	if (m_header.get_colorFormat() == ERGB) {
		permutationVector.push_back(3);
	}
	permutationVector.push_back(2);
	permutationVector.push_back(1);
	mex::MxNumeric<float> pixelsTemp = pixels.permute(permutationVector);
	float* pixelBuffer = pixelsTemp.getData();
	m_file.write(static_cast<char*>(pixels),
					pixelsTemp.getNumberOfElements());
	mexAssert(m_file);
}

PFMOutputFile::~PFMOutputFile() {
	m_file.close();
}

} /* namespace pfm */


