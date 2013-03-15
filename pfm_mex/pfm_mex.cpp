/*
 * image_utils//image_utils/pfm_mex/pfm_mex.cpp/pfm_mex.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include "pfm_mex.h"
#include <stdint.h>
#include <cmath>

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

/*
 * PFMHeader implementation.
 */
PFMHeader::PFMHeader()
			: m_colorFormat(),
			  m_width(),
			  m_height(),
			  m_scale(),
			  m_byteOrder(),
			  m_isValidPFMHeader(false) {	}

void PFMHeader::build(const EColorFormat colorFormat,
					const int width,
					const int height,
					const float scale,
					const EByteOrder byteOrder) {
	if ((colorFormat == ERGB) || (colorFormat == EGrayscale)) {
		m_colorFormat = colorFormat;
	} else {
		return;
	}
	if (width > 0) {
		m_width = width;
	} else {
		return;
	}
	if (height > 0) {
		m_height = height;
	} else {
		return;
	}
	if (scale > 0) {
		m_scale = scale;
	} else {
		return;
	}
	if ((byteOrder == ELittleEndian) || (byteOrder == EBigEndian)) {
		m_byteOrder = byteOrder;
	} else {
		return;
	}
	m_isValidPFMHeader = true;
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
 * ispfm implementation.
 */
mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName) {
	PFMInputFile file(fileName);
	file.readHeader();
	return mex::MxNumeric<bool>(file.get_header().isValidPFMHeader());
}

/*
 * PFMInputFile implementation.
 */
PFMInputFile::PFMInputFile(const mex::MxString& fileName)
						: m_file(fileName.c_str(),
								std::ifstream::in |
								std::ifstream::binary),
						  m_header(),
						  m_readHeader(false),
						  m_readFile(false) {
	mexAssert(m_file);
}

void PFMInputFile::readHeader() {
	mexAssert((!m_readHeader) && (!m_readFile));
	char format[2];
	m_file.read(format, 2);
	if ((format[0] != 'P') || ((format[1] != 'F') && (format[1] != 'f'))) {
		return;
	}
	EColorFormat colorFormat = (format[1] == 'F')?(ERGB):(EGrayscale);
	if (!m_file) {
		return;
	}

	char whitespace;
	m_file.get(whitespace);
	if ((!m_file) || (!std::isspace(whitespace))) {
		return;
	}

	int width;
	m_file >> width;
	if (!m_file) {
		return;
	}

	m_file.get(whitespace);
	if ((!m_file) || (!(whitespace == ' '))) {
		return;
	}

	int height;
	m_file >> height;
	if (!m_file) {
		return;
	}

	m_file.get(whitespace);
	if ((!m_file) || (!std::isspace(whitespace))) {
		return;
	}

	float scaledByteOrder;
	m_file >> scaledByteOrder;
	float scale = std::abs(scaledByteOrder);
	EByteOrder byteOrder = (scaledByteOrder < 0)?(ELittleEndian):(EBigEndian);
	if (!m_file) {
		return;
	}

	m_file.get(whitespace);
	if ((!m_file) || (!std::isspace(whitespace))) {
		return;
	}

	m_header.build(colorFormat, width, height, scale, byteOrder);
	m_readHeader = true;
}

mex::MxNumeric<float> PFMInputFile::readFile() {
	mexAssert((m_readHeader && m_header.isValidPFMHeader()) && (!m_readFile));
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

	m_file.read(reinterpret_cast<char*>(pixelBuffer), numPixels);
	mexAssert(m_file);
	std::vector<int> permutationVector;
	if (m_header.get_colorFormat() == ERGB) {
		permutationVector.push_back(3);
	}
	permutationVector.push_back(2);
	permutationVector.push_back(1);
	m_readFile = true;
	return pixels.permute(permutationVector);
}

/*
 * PFMOutputFile implementation.
 */
PFMOutputFile::PFMOutputFile(const mex::MxString& fileName)
							: m_file(fileName.c_str(),
									std::ofstream::out | std::ofstream::binary),
							  m_header(),
							  m_wroteHeader(false),
							  m_wroteFile(false) {
	mexAssert(m_file);
}

void PFMOutputFile::writeHeader(const mex::MxNumeric<float>& pixels) {
	mexAssert((!m_wroteHeader) && (!m_wroteFile));
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
	mexAssert(m_header.isValidPFMHeader());

	m_file << 'P';
	mexAssert(m_file);
	m_file << ((m_header.get_colorFormat() == ERGB)?('F'):('f'));
	mexAssert(m_file);
	m_file << '\n';
	mexAssert(m_file);
	m_file << m_header.get_width();
	mexAssert(m_file);
	m_file << ' ';
	mexAssert(m_file);
	m_file << m_header.get_height();
	mexAssert(m_file);
	m_file << '\n';
	mexAssert(m_file);
	m_file << (m_header.get_scale()
				* (m_header.get_byteOrder() == ELittleEndian)?(-1):(1));
	mexAssert(m_file);
	m_file << '\n';
	mexAssert(m_file);

	m_wroteHeader = true;
}

void PFMOutputFile::writeFile(const mex::MxNumeric<float>& pixels) {
	mexAssert(m_wroteHeader && (!m_wroteFile));
	std::vector<int> permutationVector;
	if (m_header.get_colorFormat() == ERGB) {
		permutationVector.push_back(3);
	}
	permutationVector.push_back(2);
	permutationVector.push_back(1);
	mex::MxNumeric<float> pixelsTemp = pixels.permute(permutationVector);
	float* pixelBuffer = pixelsTemp.getData();
	m_file.write(reinterpret_cast<char*>(pixelBuffer),
					pixelsTemp.getNumberOfElements());
	mexAssert(m_file);

	m_wroteFile = true;
}

} /* namespace pfm */


