/*
 * openexr_mex.h
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#ifndef OPENEXR_MEX_H_
#define OPENEXR_MEX_H_

#include <vector>
#include <string>
#include <stdio.h>
#include <stdarg.h>

#include "Iex.h"
#include "ImathBox.h"
#include "ImfArray.h"
#include "ImfAttribute.h"
#include "ImfBoxAttribute.h"
#include "ImfChannelList.h"
#include "ImfChannelListAttribute.h"
#include "ImfChromaticitiesAttribute.h"
#include "ImfCompressionAttribute.h"
#include "ImfEnvmapAttribute.h"
//#include "ImfExtraAttributes.h"
#include "ImfFloatAttribute.h"
#include "ImfFrameBuffer.h"
#include "ImfHeader.h"
#include "ImfInputFile.h"
#include "ImfLineOrderAttribute.h"
#include "ImfOutputFile.h"
#include "ImfPixelType.h"
#include "ImfRgba.h"
#include "ImfRgbaFile.h"
//#include "ImfStandardAttributes.h"
#include "ImfStringAttribute.h"
#include "ImfVecAttribute.h"
#include "ImfVectorAttribute.h"

#include "mex_utils.h"

namespace exr {

/*
 * TODO: Design: Everything that could be called from mexfile is using MxArray.
 */

typedef float FloatUsed;
const Imf::PixelType kEXRFloatUsed = Imf::FLOAT;

typedef enum EAttributeType {
	EAttributeChannelList = 0,
	EAttributeCompression,
	EAttributeLineOrder,
	EAttributeChromaticities,
	EAttributeEnvmap,
	EAttributeString,
	EAttributeBox2f,
	EAttributeBox2i,
	EAttributeV2f,
	EAttributeV2i,
	EAttributeVectorf,
	EAttributeVectori,
	EAttributeDouble,
	EAttributeFloat,
	EAttributeInt,
	EAttributeTypeLength,
	EAttributeTypeInvalid = -1
} EAttributeType;

/*
 * TODO: Maybe replace remaining arguments of these functions to use directly
 * MxArrays?
 */
class EXRInputFile {
public:
	explicit EXRInputFile(const mex::MxString& fileName)
							: m_file(fileName.c_str()),
							  m_frameBuffer(),
							  m_pixelBuffer(NULL),
							  m_foundChannel(),
							  m_createdFrameBuffer(false),
							  m_readFile(false) {	}

	inline int getWidth() const {
		Imath::Box2i dw = m_file.header().dataWindow();
		return dw.max.x - dw.min.x + 1;
	}

	inline int getHeight() const {
		Imath::Box2i dw = m_file.header().dataWindow();
		return dw.max.y - dw.min.y + 1;
	}

	inline void getDimensions(int& width, int& height) const {
		Imath::Box2i dw = m_file.header().dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}

	inline int getNumberOfChannels() const {
		int numChannels = 0;
		for (Imf::ChannelList::ConstIterator
				channelIter = m_file.header().channels().begin(),
				channelEnd = m_file.header().channels().end();
				channelIter != channelEnd;
				++channelIter, ++numChannels) {
			continue;
		}
		return numChannels;
	}

	inline std::vector<std::string> getChannelNames() const {
		std::vector<std::string> channelNames(0);
		for (Imf::ChannelList::ConstIterator
				channelIter = m_file.header().channels().begin(),
				channelEnd = m_file.header().channels().end();
				channelIter != channelEnd;
				++channelIter) {
			channelNames.push_back(std::string(channelIter.name()));
		}
		return channelNames;
	}

	/*
	 * TODO: Find way to do these without pointers. Maybe pass as argument?
	 */
	mex::MxArray* getAttribute(const mex::MxString& attributeName) const;
	mex::MxArray* getAttribute() const;

	void readChannelRGB();
	void readChannelY();
	void readChannel(const std::string& channelName);
	void readChannel(const std::vector<std::string>& channelNames);

	mex::MxNumeric<FloatUsed> readFile();

	virtual ~EXRInputFile() {
		if (m_createdFrameBuffer) {
			delete[] m_pixelBuffer;
		}
	}

private:
	Imf::InputFile m_file;
	Imf::FrameBuffer m_frameBuffer;
	Imf::Array2D<FloatUsed>* m_pixelBuffer;
	std::vector<bool> m_foundChannel;
	bool m_createdFrameBuffer;
	bool m_readFile;
};

class EXROutputFile {
public:
	EXROutputFile(const int width, const int height)
				: m_header(width, height),
				  m_frameBuffer(),
				  m_createdFrameBuffer(false),
				  m_wroteFile(false) {	}

	inline int getWidth() const {
		Imath::Box2i dw = m_header.dataWindow();
		return dw.max.x - dw.min.x + 1;
	}

	inline int getHeight() const {
		Imath::Box2i dw = m_header.dataWindow();
		return dw.max.y - dw.min.y + 1;
	}

	inline void getDimensions(int& width, int& height) const {
		Imath::Box2i dw = m_header.dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}

	void setAttribute(const mex::MxArray& attribute,
					const mex::MxString& attributeName);
	void setAttribute(const mex::MxStruct& attributes);

	void writeChannelRGB(const mex::MxNumeric<FloatUsed>& rgbPixels);
	void writeChannelY(const mex::MxNumeric<FloatUsed>& yPixels);
	void writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
						const std::string& channelName);
	void writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
						const std::vector<std::string>& channelNames);

	void writeFile(const mex::MxString& fileName);

	virtual ~EXROutputFile() {	}

private:
	Imf::Header m_header;
	Imf::FrameBuffer m_frameBuffer;
	bool m_createdFrameBuffer;
	bool m_wroteFile;
};

}	/* namespace exr */

#endif /* OPENEXR_MEX_H_ */
