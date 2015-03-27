/*
 * image_utils//image_utils/pfm_mex/pfm_mex.cpp/pfm_mex.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cctype>
#include "pfm_mex.h"

namespace pfm {

namespace {
PfmHeader::EByteOrder getHostByteOrder() {
	union {
		std::uint8_t  charValue[2];
		std::uint16_t shortValue;
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
		std::uint8_t byteValue[sizeof(T)];
	} u;

	u.value = value;
	std::reverse(&u.byteValue[0], &u.byteValue[sizeof(T)]);
	return u.value;
}

}  // namespace

/*
 * PfmHeader implementation.
 */
PfmHeader::PfmHeader():
					m_width(),
					m_height(),
					m_colorFormat(),
					m_scale(),
					m_byteOrder(),
					m_isValidPfmHeader(false) {	}

PfmHeader::PfmHeader(const int width,
					const int height,
					const EColorFormat colorFormat,
					const FloatUsed scale,
					const EByteOrder byteOrder):
					m_width(),
					m_height(),
					m_colorFormat(),
					m_scale(),
					m_byteOrder(),
					m_isValidPfmHeader(false) {
	build(width, height, colorFormat, scale, byteOrder);
}

void PfmHeader::build(const int width,
					const int height,
					const EColorFormat colorFormat,
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

int PfmHeader::get_width() const {
	return m_width;
}

int PfmHeader::get_height() const {
	return m_height;
}

PfmHeader::EColorFormat PfmHeader::get_colorFormat() const {
	return m_colorFormat;
}

FloatUsed PfmHeader::get_scale() const {
	return m_scale;
}

PfmHeader::EByteOrder PfmHeader::get_byteOrder() const {
	return m_byteOrder;
}

bool PfmHeader::isValidPfmHeader() const {
	return m_isValidPfmHeader;
}

void PfmHeader::readFromFile(std::ifstream& file) {
	char format[2];
	file.read(format, 2);
	if ((format[0] != 'P') || ((format[1] != 'F') && (format[1] != 'f'))) {
		return;
	}
	PfmHeader::EColorFormat colorFormat = (format[1] == 'F')
										?(PfmHeader::EColorFormat::ERGB)
										:(PfmHeader::EColorFormat::EGrayscale);
	if (!file) {
		return;
	}

	char whitespace;
	file.get(whitespace);
	if ((!file) || (!std::isspace(whitespace))) {
		return;
	}

	int width;
	file >> width;
	if (!file) {
		return;
	}

	file.get(whitespace);
	if ((!file) || (!std::isspace(whitespace))) {
		return;
	}

	int height;
	file >> height;
	if (!file) {
		return;
	}

	file.get(whitespace);
	if ((!file) || (!std::isspace(whitespace))) {
		return;
	}

	FloatUsed scaledByteOrder;
	file >> scaledByteOrder;
	FloatUsed scale = std::abs(scaledByteOrder);
	PfmHeader::EByteOrder byteOrder = (scaledByteOrder < 0)
									?(PfmHeader::EByteOrder::ELittleEndian)
									:(PfmHeader::EByteOrder::EBigEndian);
	if (!file) {
		return;
	}

	file.get(whitespace);
	if ((!file) || (!std::isspace(whitespace))) {
		return;
	}

	build(width, height, colorFormat, scale, byteOrder);
}

void PfmHeader::writeToFile(std::ofstream& file) const {
	mexAssert(m_isValidPfmHeader);

	file << 'P';
	mexAssert(file);
	file << ((m_colorFormat == PfmHeader::EColorFormat::ERGB)?('F'):('f'));

	mexAssert(file);
	file << '\n';
	mexAssert(file);
	file << m_width;
	mexAssert(file);
	file << ' ';
	mexAssert(file);
	file << m_height;
	mexAssert(file);
	file << '\n';
	mexAssert(file);
	file << (m_scale * ((m_byteOrder == PfmHeader::EByteOrder::ELittleEndian)
						?(-1.0)
						:(1.0)));
	mexAssert(file);
	file << '\n';
	mexAssert(file);
}

/*
 * ispfm implementation.
 */
mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName) {
	PfmInputFile file(fileName);
	return mex::MxNumeric<bool>(file.isValidFile());
}

/*
 * PFMInputFile implementation.
 */
PfmInputFile::PfmInputFile(const mex::MxString& fileName):
						m_fileName(fileName.get_string()),
						m_file(fileName.c_str(),
							std::ifstream::in |
							std::ifstream::binary),
						m_header(),
						m_readHeader(false),
						m_readFile(false) {
	mexAssert(m_file);
	m_header.readFromFile(m_file);
	m_readHeader = true;
}

mex::MxNumeric<bool> PfmInputFile::isValidFile() const {
	return mex::MxNumeric<bool>(m_header.isValidPfmHeader());
}

mex::MxString PfmInputFile::getFileName() const {
	return mex::MxString(m_fileName);
}

int PfmInputFile::getHeight() const {
	return m_header.get_height();
}

int PfmInputFile::getWidth() const {
	return m_header.get_width();
}

int PfmInputFile::getNumberOfChannels() const {
	PfmHeader::EColorFormat colorFormat = m_header.get_colorFormat();
	return (colorFormat == PfmHeader::EColorFormat::ERGB)?(3):(1);
}

mex::MxArray PfmInputFile::getAttribute(const mex::MxString& attributeName) const {
	if (!attributeName.get_string().compare("width")) {
		return mex::MxArray(mex::MxNumeric<int>(m_header.get_width()).get_array());
	} else if (!attributeName.get_string().compare("height")) {
		return mex::MxArray(mex::MxNumeric<int>(m_header.get_height()).get_array());
	} else if (!attributeName.get_string().compare("colorFormat")) {
		return mex::MxArray(mex::MxString(
					(m_header.get_colorFormat() == PfmHeader::EColorFormat::ERGB)
					?("rgb"):("grayscale")).get_array());
	} else if (!attributeName.get_string().compare("scale")) {
		return mex::MxArray(mex::MxNumeric<FloatUsed>(m_header.get_scale()).get_array());
	} else if (!attributeName.get_string().compare("byteOrder")) {
		return mex::MxArray(mex::MxString(
					(m_header.get_byteOrder() == PfmHeader::EByteOrder::EBigEndian)
					?("big endian"):("little endian")).get_array());
	} else {
		mexAssertEx(0, "Unknown attribute type");
		return mex::MxArray();
	}
}

mex::MxArray PfmInputFile::getAttribute() const {

	std::vector<std::string> nameVec;
	std::vector<mex::MxArray*> arrayVec;

	nameVec.push_back(std::string("width"));
	arrayVec.push_back(new mex::MxArray(getAttribute(mex::MxString("width")).get_array()));
	nameVec.push_back(std::string("height"));
	arrayVec.push_back(new mex::MxArray(getAttribute(mex::MxString("height")).get_array()));
	nameVec.push_back(std::string("colorFormat"));
	arrayVec.push_back(new mex::MxArray(getAttribute(mex::MxString("colorFormat")).get_array()));
	nameVec.push_back(std::string("scale"));
	arrayVec.push_back(new mex::MxArray(getAttribute(mex::MxString("scale")).get_array()));
	nameVec.push_back(std::string("byteOrder"));
	arrayVec.push_back(new mex::MxArray(getAttribute(mex::MxString("byteOrder")).get_array()));

	mex::MxArray retArg(mex::MxStruct(nameVec, arrayVec).get_array());
	for (int iter = 0, numAttributes = arrayVec.size();
		iter < numAttributes;
		++iter) {
		delete arrayVec[iter];
	}
	return retArg;
}

mex::MxArray PfmInputFile::readData() {
	mexAssert((m_readHeader && m_header.isValidPfmHeader()) && (!m_readFile));
	int numChannels = getNumberOfChannels();
	std::vector<int> dimensions;
	if (numChannels == 3) {
		dimensions.push_back(numChannels);
	}
	dimensions.push_back(m_header.get_width());
	dimensions.push_back(m_header.get_height());
	mex::MxNumeric<FloatUsed> pixelBuffer(static_cast<int>(dimensions.size()),
										&dimensions[0]);
	FloatUsed* pixelRaw = pixelBuffer.getData();
	m_file.read(reinterpret_cast<char*>(pixelRaw),
				pixelBuffer.getNumberOfElements() * sizeof(FloatUsed));
	mexAssert(m_file);

	if ((m_header.get_scale() != 1.0) ||
		(getHostByteOrder() != m_header.get_byteOrder())) {
		for (int iter = 0, end = pixelBuffer.getNumberOfElements();
			iter < end;
			++iter) {
			if (getHostByteOrder() != m_header.get_byteOrder()) {
				pixelRaw[iter] = endianness_swap(pixelRaw[iter]);
			}
			if (m_header.get_scale() != 1.0) {
				pixelRaw[iter] *= m_header.get_scale();
			}
		}
	}
	m_readFile = true;

	std::vector<int> permutationVector;
	if (numChannels == 3) {
		permutationVector.push_back(3);
		permutationVector.push_back(2);
		permutationVector.push_back(1);
	} else {
		permutationVector.push_back(2);
		permutationVector.push_back(1);
	}
	mex::MxArray retArg(pixelBuffer.permute(permutationVector).get_array());
	pixelBuffer.destroy();
	return retArg;
}

/*
 * PFMOutputFile implementation.
 */
PfmOutputFile::PfmOutputFile(const mex::MxString& fileName, int width, int height):
							m_fileName(fileName.get_string()),
							m_file(fileName.c_str(),
								std::ofstream::out | std::ofstream::binary),
							m_width(width),
							m_height(height),
							m_scale(1.0),
							m_writtenFile(false) {
	mexAssert(m_file);
}

mex::MxString PfmOutputFile::getFileName() const {
	return mex::MxString(m_fileName);
}

int PfmOutputFile::getHeight() const {
	return m_height;
}

int PfmOutputFile::getWidth() const {
	return m_width;
}

void PfmOutputFile::setAttribute(const mex::MxString& attributeName,
										const mex::MxArray& attribute) {
	if (!attributeName.get_string().compare("scale")) {
		const mex::MxNumeric<double> tempArray(attribute.get_array());
		mexAssert(tempArray.getNumberOfElements() == 1);
		m_scale = tempArray[0];
	} else {
		mexAssertEx(0, "Unknown attribute type");
	}
}

void PfmOutputFile::setAttribute(const mex::MxStruct& attributes) {

	for (int iter = 0, numFields = attributes.getNumberOfFields();
		iter < numFields;
		++iter) {
		setAttribute(mex::MxString(attributes.getFieldName(iter)),
					mex::MxArray(attributes[iter]));
	}
}

void PfmOutputFile::writeData(const mex::MxArray& data) {

	mexAssert(!m_writtenFile);
	mex::MxNumeric<FloatUsed> pixelArray(data.get_array());
	std::vector<int> dimensions = pixelArray.getDimensions();
	int numChannels = (dimensions.size() == 2)?(1):(dimensions[2]);

	mexAssert(((numChannels == 1) || (numChannels = 3)) &&
			(dimensions[0] == m_height) && (dimensions[1] == m_width));

	PfmHeader::EColorFormat colorFormat = (numChannels == 3)
										?(PfmHeader::EColorFormat::ERGB)
										:(PfmHeader::EColorFormat::EGrayscale);
	PfmHeader header = PfmHeader(m_width, m_height, colorFormat, m_scale,
								getHostByteOrder());
	header.writeToFile(m_file);

	std::vector<int> permutationVector;
	if (colorFormat == PfmHeader::EColorFormat::ERGB) {
		permutationVector.push_back(3);
	}
	permutationVector.push_back(2);
	permutationVector.push_back(1);
	mex::MxNumeric<FloatUsed> pixelBuffer = pixelArray.permute(permutationVector);
	FloatUsed* pixelsRaw = pixelBuffer.getData();
	m_file.write(reinterpret_cast<char*>(pixelsRaw),
				pixelBuffer.getNumberOfElements() * sizeof(FloatUsed));
	mexAssert(m_file);
	pixelBuffer.destroy();
	m_writtenFile = true;
}

} /* namespace pfm */
