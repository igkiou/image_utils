/*
 * raw_mex.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: igkiou
 */

#include <iostream>
#include "raw_mex.h"

namespace raw {

/*
 * Check valid file function.
 */
mex::MxNumeric<bool> isRawFile(const mex::MxString& fileName) {
	LibRaw rawProcessor;
	return mex::MxNumeric<bool>(rawProcessor.open_file(fileName.c_str())
								== LIBRAW_SUCCESS);
}

/*
 * Input file handling.
 */
RawInputFile::RawInputFile(const mex::MxString& fileName):
						m_fileName(fileName.get_string()),
						m_rawProcessor() {
	int errorCode = m_rawProcessor.open_file(m_fileName.c_str());
	mexAssert(errorCode == 0);
}

mex::MxString RawInputFile::getFileName() const {
	return mex::MxString(m_file.fileName());
}

mex::MxNumeric<bool> RawInputFile::isValidFile() const {
	/*
	 * LibRaw always checks validity at construction, so if we're here, file is
	 * valid.
	 */
	return mex::MxArray(mex::MxNumeric<bool>(true).get_array());
}

int RawInputFile::getHeight() const {
	return m_rawProcessor.imgdata.sizes.iheight;
}

int RawInputFile::getWidth() const {
	return m_rawProcessor.imgdata.sizes.iwidth;
}

int RawInputFile::getNumberOfChannels() const {
	return int(4);
}

/*
 * TODO: Incomplete. Add copying of image to MxArray. Add option for subtracting
 * dark frame or dcraw processing. Copy dcraw args processing from sample.
 */
mex::MxArray RawInputFile::readData(mex::MxNumeric<bool> subtractDarkFrame = false, ) {

	m_rawProcessor.unpack();

	int width = getWidth();
	int height = getHeight();
	int numChannels = getNumberOfChannels();
	std::vector<int> dimensions;
	dimensions.push_back(width);
	dimensions.push_back(height);
	dimensions.push_back(numChannels);
	mex::MxNumeric<unsigned short> pixelBuffer(static_cast<int>(dimensions.size()),
										&dimensions[0]);




	Imf::FrameBuffer frameBuffer;
	for (int iter = 0; iter < numChannels; ++iter) {
		mexAssert(m_file.header().channels().findChannel(channelNames[iter].c_str()));
		FloatUsed* tempBuffer = &pixelBuffer[iter * width * height];
		frameBuffer.insert(channelNames[iter].c_str(),
						Imf::Slice(kEXRFloatUsed,
								(char *) (tempBuffer - dw.min.x - dw.min.y * width),
								sizeof(*tempBuffer) * 1,
								sizeof(*tempBuffer) * width,
								1,
								1,
								FLT_MAX));
	}

	mexAssert(isComplete());
	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);
	std::vector<int> transposePermutation;
	transposePermutation.push_back(2);
	transposePermutation.push_back(1);
	if (numChannels > 1) {
		transposePermutation.push_back(3);
	}
	mex::MxArray retArg = mex::MxArray(pixelBuffer.permute(transposePermutation));
	pixelBuffer.destroy();
	return retArg;
}

/*
 * TODO: Add attribute getting.
 */

}	/* namespace raw */
