/*
 * pfm_mex.h
 *
 *  Created on: Jan 29, 2013
 *      Author: igkiou
 */

#ifndef PFM_MEX_H_
#define PFM_MEX_H_

#include <fstream>
#include <string>
#include <vector>

#include "mex_utils.h"

namespace pfm {

typedef enum ENDIANNESS {
	BIG = 0,
	LITTLE = 1,
	ENDIANNESS_LENGTH = 2,
	ENDIANNESS_INVALID = -1
} ENDIANNESS;

struct PFMInputFIle {
private:
	std::fstream m_file;

public:
	PFMInputFIle(const std::string& fileName)
			: m_file(fileName.c_str(), fstream::in | fstream::binary) {	}


	inline void read() {

	}

	inline void get() {

	}

	~PFMInputFIle() {
		m_file.close();
	}
};

struct PFMOutputFIle {
private:
	std::fstream m_file;

public:
	PFMOutputFIle(const std::string& fileName)
			: m_(fileName.c_str(), fstream::out | fstream::binary) {	}


	inline void write() {

	}

	inline void get() {

	}

	~PFMOutputFIle() {
		m_file.close();
	}
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
