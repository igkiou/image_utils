/*
 * pfm_mex.h
 *
 *  Created on: Jan 29, 2013
 *      Author: igkiou
 */

#ifndef PFM_MEX_H_
#define PFM_MEX_H_

#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <ctype.h>

#include "mex_utils.h"

namespace pfm {

typedef enum EByteOrder {
	EBigEndian = 0,
	ELittleEndian,
	EByteTypeLength,
	EByteTypeInvalid = -1
} EByteOrder;

typedef enum EColorFormat {
	ERGB = 0,
	EGrayscale,
	EColorFormatLength,
	EColorFormatInvalid = -1
} EColorFormat;

//static PFMHeader::EByteOrder getByteOrder() {
//	union {
//		uint8_t  charValue[2];
//		uint16_t shortValue;
//	};
//	charValue[0] = 1;
//	charValue[1] = 0;
//
//	return (shortValue == 1)?(PFMHeader::ELittleEndian):(PFMHeader::EBigEndian);
//}

inline mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName) {
	return mex::MxNumeric<bool>(isPfmFile(fileName.c_str()));
}

class PFMHeader {
	PFMHeader()
			: m_colorFormat(),
			  m_width(),
			  m_height(),
			  m_scale(),
			  m_byteOrder() {	}

	PFMHeader(const EColorFormat colorFormat,
			const int width,
			const int height,
			const double scale,
			const EByteOrder byteOrder)
			: m_colorFormat(colorFormat),
			  m_width(width),
			  m_height(height),
			  m_scale(scale),
			  m_byteOrder(byteOrder) {
		mexAssert((m_colorFormat == ERGB) || (m_colorFormat == EGrayscale));
		mexAssert(m_width > 0);
		mexAssert(m_height > 0);
		mexAssert(m_scale > 0);
		mexAssert((m_byteOrder == ELittleEndian) ||
				(m_byteOrder == EBigEndian));
	}

	inline void readFromStream(std::istream& stream) {
		char format[2];
		stream.read(format, 2);
		mexAssert((format[0] == 'P') &&
				(format[1] == 'F' || format[1] == 'f'));
		m_colorFormat = (format[1] == 'F')?(ERGB):(EGrayscale);
		mexAssert(stream);

		char whitespace;
		stream.get(whitespace);
		mexAssert(std::isspace(whitespace));

		stream >> m_width;
		mexAssert(m_width > 0);
		mexAssert(stream);

		stream.get(whitespace);
		mexAssert(whitespace == ' ');
		mexAssert(stream);

		stream >> m_height;
		mexAssert(m_height > 0);
		mexAssert(stream);

		stream.get(whitespace);
		mexAssert(whitespace == ' ');
		mexAssert(stream);

		double byteOrder;
		stream >> byteOrder;
		m_scale = std::abs(byteOrder);
		m_byteOrder = (byteOrder < 0)?(ELittleEndian):(EBigEndian);
		mexAssert(stream);

		stream.get(whitespace);
		mexAssert(std::isspace(whitespace));
		mexAssert(stream);
	}

	inline void writeToStream(std::ostream& stream) const {
		stream << 'P' << (m_colorFormat == ERGB)?('F'):('f') << '\n';
		stream << m_width << ' ' << m_height << '\n';
		stream << m_scale * (m_byteOrder == ELittleEndian)?(-1):(1) << '\n';
	}

	inline const EColorFormat get_colorFormat() const {
		return m_colorFormat;
	}

	inline const int get_width() const {
		return m_width;
	}

	inline const int get_height() const {
		return m_height;
	}

	inline const double get_scale() const {
		return m_scale;
	}

	inline const EByteOrder get_byteOrder() const {
		return m_byteOrder;
	}

	inline mex::MxStruct toMxArray() const {
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
		fieldValues.push_back(new mex::MxNumeric<double>(m_scale));
		fieldValues.push_back(new mex::MxString((m_byteOrder == ELittleEndian)?
												("littleendian"):
												("bigendian")));
		mex::MxStruct retArg(fieldNames, fieldValues);
		for (int iter = 0, end = fieldValues.size(); iter < end; ++iter) {
			delete fieldValues[iter];
		}
		return retArg;
	}

private:
	EColorFormat m_colorFormat;
	int m_width;
	int m_height;
	double m_scale;
	EByteOrder m_byteOrder;
};

class PFMInputFIle {
public:
	explicit PFMInputFIle(const mex::MxString& fileName)
						: m_file(fileName.c_str(),
								std::ifstream::in | std::ifstream::binary),
						  m_header(),
						  m_readHeader(false),
						  m_readFile(false) {
		mexAssert(m_file);
	}

	inline mex::MxStruct getHeader() const {
		mexAssert((!m_readHeader) && (!m_readFile));
		m_header.readFromStream(m_file);
		m_readHeader = true;
		return m_header.toMxArray();
	}

	inline mex::MxNumeric<float> readFile() const {
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
		mex::MxNumeric<float> pixelArray(3, &dimensions[0]);
		float* pixelBuffer = pixelArray.getData();
		m_file.read(static_cast<char*>(pixelBuffer), numPixels);
		std::vector<int> permutationVector;
		if (m_header.get_colorFormat() == ERGB) {
			permutationVector.push_back(3);
		}
		return pixelBuffer.permute(permutationVector);
	}

	~PFMInputFIle() {
		m_file.close();
	}

private:
	std::ifstream m_file;
	PFMHeader m_header;
	bool m_readHeader;
	bool m_readFile;
};

class PFMOutputFIle {
public:
	explicit PFMOutputFIle(const mex::MxString& fileName)
						: m_file(fileName.c_str(),
							std::ofstream::out | std::ofstream::binary) {	}

	inline void writeFile(const mex::MxNumeric<float>& pixels);

	inline void setHeader();

	~PFMOutputFIle() {
		m_file.close();
	}

private:
	std::ofstream m_file;

};


//void writeToFilePFM(const std::string& fileName, const ENDIANNESS fileEndianness) const {
//
//	Assert((fileEndianness == BIG) || (fileEndianness == LITTLE));
//
//	FILE* outFile = fopen(fileName.c_str(), "wb");
//	if (outFile == NULL) {
//		AssertEx(outFile != NULL, "Problem opening output file.");
//	}
//
//	/* Lazy string creation, make something better. */
//	std::ostringstream temp;
//	temp << "Pf" << '\n';
//	temp << m_xRes << " " << m_yRes << "\n";
//	temp << (fileEndianness == BIG ? 1.0f : -1.0f) << "\n";
//	std::string pfmHeader = temp.str();
//	std::cout << pfmHeader << std::endl;
//	if (fwrite(pfmHeader.c_str(), \
//			1, \
//			pfmHeader.length(), \
//			outFile) != pfmHeader.length()) {
//		AssertEx(outFile != NULL, "Problem writing output file.");
//	}
//
//	for (int iterY = 0; iterY < m_yRes; ++iterY) {
//		if (fwrite(m_pixels + m_xRes * (m_yRes - 1 - iterY), \
//				1, \
//				m_xRes * sizeof(float), \
//				outFile) != m_xRes * sizeof(float)) {
//			AssertEx(0, "Problem writing output file.");
//		}
//	}
//
//	if (fclose(outFile)) {
//		AssertEx(0, "Problem closing output file.");
//	}
//}
//
//void readPFM(Stream *stream) {
//	char header[3];
//	stream->read(header, 3);
//	if (header[0] != 'P' || !(header[1] == 'F' || header[1] == 'f'))
//		Log(EError, "readPFM(): Invalid header!");
//
//	bool color = (header[1] == 'F');
//	m_pixelFormat = color ? ERGB : ELuminance;
//	m_componentFormat = EFloat32;
//	m_channelCount = color ? 3 : 1;
//	m_gamma = 1.0f;
//
//	char *end_ptr = NULL;
//	std::string widthString = pfmReadString(stream);
//	m_size.x = (int) strtol(widthString.c_str(), &end_ptr, 10);
//	if (*end_ptr != '\0')
//		SLog(EError, "Could not parse image dimensions!");
//
//	std::string heightString = pfmReadString(stream);
//	m_size.y = (int) strtol(heightString.c_str(), &end_ptr, 10);
//	if (*end_ptr != '\0')
//		SLog(EError, "Could not parse image dimensions!");
//
//	std::string scaleAndOrderString = pfmReadString(stream);
//	float scaleAndOrder = (float) strtod(scaleAndOrderString.c_str(), &end_ptr);
//	if (*end_ptr != '\0')
//		SLog(EError, "Could not parse scale/order information!");
//
//	m_data = static_cast<uint8_t *>(allocAligned(getBufferSize()));
//	float *data = (float *) m_data;
//
//	Stream::EByteOrder backup = stream->getByteOrder();
//	size_t size = getPixelCount() * m_channelCount;
//	stream->setByteOrder(scaleAndOrder <= 0.0f ? Stream::ELittleEndian : Stream::EBigEndian);
//
//	try {
//		stream->readSingleArray(data, size);
//	} catch (...) {
//		stream->setByteOrder(backup);
//		throw;
//	}
//
//	stream->setByteOrder(backup);
//	Float scale = std::abs(scaleAndOrder);
//	if (scale != 1) {
//		for (size_t i=0; i<size; ++i)
//			data[i] *= scale;
//	}
//	flipVertically();
//}
//
//void writePFM(const std::string filename) {
//	if (m_componentFormat != EFloat32)
//		Log(EError, "writePFM(): component format must be EFloat32!");
//	if (m_pixelFormat != ERGB && m_pixelFormat != ERGBA && m_pixelFormat != ELuminance)
//		Log(EError, "writePFM(): pixel format must be ERGB, ERGBA, ELuminance, or ELuminanceAlpha!");
//
//	/* Write the header */
//	std::ostringstream oss;
//	oss << 'P' << ((m_pixelFormat == ERGB || m_pixelFormat == ERGBA) ? 'F' : 'f') << '\n';
//	oss << m_size.x << ' ' << m_size.y << '\n';
//	oss << (Stream::getHostByteOrder() == Stream::ELittleEndian ? "-1" : "1") << '\n';
//	std::string header = oss.str();
//	stream->write(header.c_str(), header.length());
//
//	float *data = (float *) m_data;
//	if (m_pixelFormat == ERGB || m_pixelFormat == ELuminance) {
//		size_t scanline = (size_t) m_size.x * m_channelCount;
//
//		for (int y=0; y<m_size.y; ++y)
//			stream->write(data + scanline*(m_size.y - 1 - y), scanline * sizeof(float));
//	} else {
//		/* For convenience: also handle images with an alpha
//		   channel, but strip it out while saving the data */
//		size_t scanline = (size_t) m_size.x * m_channelCount;
//		float *temp = (float *) alloca(scanline * sizeof(float));
//
//		for (int y=0; y<m_size.y; ++y) {
//			const float *source = data + scanline*(m_size.y - 1 - y);
//			float *dest = temp;
//
//			for (int x=0; x<m_size.x; ++x) {
//				for (int j=0; j<m_channelCount-1; ++j)
//					*dest++ = *source++;
//				source++;
//			}
//
//			stream->write(temp, sizeof(float) * m_size.x * (m_channelCount-1));
//		}
//	}
//}

}	/* namespace pfm */

#endif /* PFM_MEX_H_ */
