/*
 * image_utils//image_utils/pfm_mex/pfm_mex.cpp/pfm_mex.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include <algorithm>
#include <cmath>
#include <stdint.h>
#include "pfm_mex.h"

namespace pfm {

namespace {

static PfmHeader::EByteOrder getHostByteOrder() {
	union {
		uint8_t  charValue[2];
		uint16_t shortValue;
	};
	charValue[0] = 1;
	charValue[1] = 0;

	return (shortValue == 1)?(PfmHeader::EByteOrder::ELittleEndian)
							:(PfmHeader::EByteOrder::EBigEndian);
}

template<typename T>
inline T endianness_swap(T value) {
	union {
		T value;
		uint8_t byteValue[sizeof(T)];
	} u;

	u.value = value;
	std::reverse(&u.byteValue[0], &u.byteValue[sizeof(T)]);
	return u.value;
}

} /* namespace */

/*
 * PFMHeader implementation.
 */
PfmHeader::PfmHeader()
			: m_colorFormat(),
			  m_width(),
			  m_height(),
			  m_scale(),
			  m_byteOrder(),
			  m_isValidPfmHeader(false) {	}

void PfmHeader::build(const EColorFormat colorFormat,
					const int width,
					const int height,
					const FloatUsed scale,
					const EByteOrder byteOrder) {
	if ((colorFormat == EColorFormat::ERGB) ||
		(colorFormat == EColorFormat::EGrayscale)) {
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
	if ((byteOrder == EByteOrder::ELittleEndian) ||
		(byteOrder == EByteOrder::EBigEndian)) {
		m_byteOrder = byteOrder;
	} else {
		return;
	}
	m_isValidPfmHeader = true;
}

namespace {

enum class EPfmAttributeType {
	EColorFormat = 0,
	EByteOrder,
	EInt,
	EFloat,
	ELength,
	EInvalid = -1
};

mex::ConstMap<std::string, EPfmAttributeType> attributeNameAttributeTypeNameMap =
	mex::ConstMap<std::string, EPfmAttributeType>
	(std::string('colorformat'), EPfmAttributeType::EColorFormat)
	(std::string('byteorder'), EPfmAttributeType::EByteOrder)
	(std::string('width'), EPfmAttributeType::EInt)
	(std::string('height'), EPfmAttributeType::EInt)
	(std::string('scale'), EPfmAttributeType::EFloat);

} /* namespace */

mex::MxArray PfmInputFile::getAttribute(const mex::MxString& attributeName) const {
	const EPfmAttributeType type = attributeTypeNameMap[attributeName.get_string()];
	switch(type) {
		case EPfmAttributeType::EColorFormat: {
			return mex::MxArray(mex::MxString(
					(m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB)
									?("rgb"):("grayscale")).get_array());
		}
		case EPfmAttributeType::EColorFormat: {
			return mex::MxArray(mex::MxString(
					(m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB)
									?("rgb"):("grayscale")).get_array());
		}
		default: {
			mexAssertEx(0, "Unknown attribute type");
			return mex::MxArray();
		}
	}

//mex::MxStruct PfmHeader::toMxArray() const {
//	std::vector<std::string> fieldNames;
//	fieldNames.push_back(std::string("colorformat"));
//	fieldNames.push_back(std::string("width"));
//	fieldNames.push_back(std::string("height"));
//	fieldNames.push_back(std::string("scale"));
//	fieldNames.push_back(std::string("byteorder"));
//	std::vector<mex::MxArray*> fieldValues;
//	fieldValues.push_back(new ;
//	fieldValues.push_back(new mex::MxNumeric<int>(m_width));
//	fieldValues.push_back(new mex::MxNumeric<int>(m_height));
//	fieldValues.push_back(new mex::MxNumeric<FloatUsed>(m_scale));
//	fieldValues.push_back(new mex::MxString((m_byteOrder == PfmHeader::EByteOrder::ELittleEndian)
//											?("littleendian"):("bigendian")));
//	mex::MxStruct retArg(fieldNames, fieldValues);
//	for (int iter = 0, end = fieldValues.size(); iter < end; ++iter) {
//		delete fieldValues[iter];
//	}
//	return retArg;
}

/*
 * ispfm implementation.
 */
mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName) {
	PfmInputFile file(fileName);
	return mex::MxNumeric<bool>(file.isOpenPfmFile());
}

/*
 * PFMInputFile implementation.
 */
PfmInputFile::PfmInputFile(const mex::MxString& fileName)
						: m_file(fileName.c_str(),
								std::ifstream::in |
								std::ifstream::binary),
						  m_header(),
						  m_readHeader(false),
						  m_readFile(false),
						  m_isValidHeader(false) {
	mexAssert(m_file);
	readHeader();
}

void PfmInputFile::readHeader() {
	mexAssert((!m_readHeader) && (!m_readFile));
	char format[2];
	m_file.read(format, 2);
	if ((format[0] != 'P') || ((format[1] != 'F') && (format[1] != 'f'))) {
		return;
	}
	PfmHeader::EColorFormat colorFormat = (format[1] == 'F')
										?(PfmHeader::EColorFormat::ERGB)
										:(PfmHeader::EColorFormat::EGrayscale);
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

	FloatUsed scaledByteOrder;
	m_file >> scaledByteOrder;
	FloatUsed scale = std::abs(scaledByteOrder);
	PfmHeader::EByteOrder byteOrder = (scaledByteOrder < 0)
									?(PfmHeader::EByteOrder::ELittleEndian)
									:(PfmHeader::EByteOrder::EBigEndian);
	if (!m_file) {
		return;
	}

	m_file.get(whitespace);
	if ((!m_file) || (!std::isspace(whitespace))) {
		return;
	}

	m_header.build(colorFormat, width, height, scale, byteOrder);
	m_readHeader = true;
	m_isValidHeader = m_header.isValidPfmHeader();
}

mex::MxNumeric<FloatUsed> PfmInputFile::readFile() {
	mexAssert((m_readHeader && m_header.isValidPfmHeader()) && (!m_readFile));
	int numChannels;
	std::vector<int> dimensions;
	if (m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB) {
		numChannels = 3;
		dimensions.push_back(numChannels);
	} else {
		numChannels = 1;
	}
	dimensions.push_back(m_header.get_width());
	dimensions.push_back(m_header.get_height());
	mex::MxNumeric<FloatUsed> pixels(static_cast<int>(dimensions.size()),
									&dimensions[0]);
	FloatUsed* pixelBuffer = pixels.getData();
	m_file.read(reinterpret_cast<char*>(pixelBuffer),
				pixels.getNumberOfElements() * sizeof(FloatUsed));
	mexAssert(m_file);

	if ((m_header.get_scale() != 1.0) ||
		(getHostByteOrder() != m_header.get_byteOrder())) {
		for (int iter = 0, end = pixels.getNumberOfElements();
			iter < end;
			++iter) {
			if (getHostByteOrder() != m_header.get_byteOrder()) {
				pixelBuffer[iter] = endianness_swap(pixelBuffer[iter]);
			}
			if (m_header.get_scale() != 1.0) {
				pixelBuffer[iter] *= m_header.get_scale();
			}
		}
	}
	m_readFile = true;

	std::vector<int> permutationVector;
	if (m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB) {
		permutationVector.push_back(3);
		permutationVector.push_back(2);
		permutationVector.push_back(1);
	} else {
		permutationVector.push_back(2);
		permutationVector.push_back(1);
	}
	return pixels.permute(permutationVector);
}

mex::MxArray PfmInputFile::getAttribute(const mex::MxString& attributeName) const {

}

mex::MxArray PfmInputFile::getAttribute() const {
	return mex::MxArray(m_header.toMxArray().get_array());
}

/*
 * PFMOutputFile implementation.
 */
PfmOutputFile::PfmOutputFile(const mex::MxString& fileName)
							: m_file(fileName.c_str(),
									std::ofstream::out | std::ofstream::binary),
							  m_header(),
							  m_wroteHeader(false),
							  m_wroteFile(false) {
	mexAssert(m_file);
}

void PfmOutputFile::writeHeader(const mex::MxNumeric<FloatUsed>& pixels) {
	mexAssert((!m_wroteHeader) && (!m_wroteFile));
	std::vector<int> dimensions = pixels.getDimensions();
	mexAssert((dimensions.size() == 2) ||
			((dimensions.size() == 3) &&
			((dimensions[2] == 3) || (dimensions[2] == 1))));
	PfmHeader::EColorFormat colorFormat = ((dimensions.size() == 2) || (dimensions[2] == 1))
								?(PfmHeader::EColorFormat::EGrayscale)
								:(PfmHeader::EColorFormat::ERGB);
	int width = dimensions[1];
	int height = dimensions[0];
	FloatUsed scale = 1.0;
	PfmHeader::EByteOrder byteOrder = getHostByteOrder();
	m_header.build(colorFormat, width, height, scale, byteOrder);
	mexAssert(m_header.isValidPfmHeader());

	m_file << 'P';
	mexAssert(m_file);
	m_file << ((m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB)
																?('F'):('f'));
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
			* (m_header.get_byteOrder() == PfmHeader::EByteOrder::ELittleEndian)
										?(-1):(1));
	mexAssert(m_file);
	m_file << '\n';
	mexAssert(m_file);

	m_wroteHeader = true;
}

void PfmOutputFile::writeFile(const mex::MxNumeric<FloatUsed>& pixels) {
	mexAssert(m_wroteHeader && (!m_wroteFile));
	std::vector<int> permutationVector;
	if (m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB) {
		permutationVector.push_back(3);
	}
	permutationVector.push_back(2);
	permutationVector.push_back(1);
	mex::MxNumeric<FloatUsed> pixelsTemp = pixels.permute(permutationVector);
	FloatUsed* pixelBuffer = pixelsTemp.getData();
	m_file.write(reinterpret_cast<char*>(pixelBuffer),
				pixelsTemp.getNumberOfElements() * sizeof(FloatUsed));
	mexAssert(m_file);

	m_wroteFile = true;
}

} /* namespace pfm */
