/*
 * raw_mex.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: igkiou
 */

#include <cstring>
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
	return mex::MxString(m_fileName);
}

mex::MxNumeric<bool> RawInputFile::isValidFile() const {
	/*
	 * LibRaw always checks validity at construction, so if we're here, file is
	 * valid.
	 */
	return mex::MxNumeric<bool>(true);
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
 * TODO: Incomplete. Add option for dcraw processing. Copy dcraw args processing
 * from sample.
 */
mex::MxArray RawInputFile::readData() {
	mex::MxNumeric<bool> argumentArray(false);
	mex::MxArray retArg(readData(argumentArray));
	argumentArray.destroy();
	return retArg;
}

mex::MxArray RawInputFile::readData(mex::MxNumeric<bool> doSubtractDarkFrame) {

	int errorCode = m_rawProcessor.unpack();
	mexAssert(errorCode == 0);

	if (doSubtractDarkFrame[0]) {
		errorCode = m_rawProcessor.subtract_black();
		mexAssert(errorCode == 0);
	}

	int width = getWidth();
	int height = getHeight();
	int numChannels = getNumberOfChannels();
	std::vector<int> dimensions;
	dimensions.push_back(width);
	dimensions.push_back(height);
	dimensions.push_back(numChannels);
	mex::MxNumeric<unsigned short> pixelBuffer(static_cast<int>(dimensions.size()),
										&dimensions[0]);
	unsigned short* pixelData = pixelBuffer.getData();
	for (int channel = 0; channel < numChannels; ++channel) {
		pixelData[channel*width*height] = m_rawProcessor.imgdata.image[0][0];
//		std::memcpy(&pixelData[channel * width * height],
//					m_rawProcessor.imgdata.image[channel],
//					width * height * sizeof(unsigned short));
	}
	return mex::MxArray(pixelBuffer.get_array());
}

/*
 * TODO: Add attribute getting.
 */
mex::MxArray RawInputFile::getAttribute(const mex::MxString& attributeName) const {
	return mex::MxArray();
}

mex::MxArray RawInputFile::getAttribute() const {
	return mex::MxArray();
}

}	/* namespace raw */
