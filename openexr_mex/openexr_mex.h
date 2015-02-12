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

#include "IlmBase/Iex/Iex.h"
#include "IlmBase/Imath/ImathBox.h"
#include "OpenEXR/IlmImf/ImfArray.h"
#include "OpenEXR/IlmImf/ImfAttribute.h"
#include "OpenEXR/IlmImf/ImfBoxAttribute.h"
#include "OpenEXR/IlmImf/ImfChannelList.h"
#include "OpenEXR/IlmImf/ImfChannelListAttribute.h"
#include "OpenEXR/IlmImf/ImfChromaticitiesAttribute.h"
#include "OpenEXR/IlmImf/ImfCompressionAttribute.h"
#include "OpenEXR/IlmImf/ImfEnvmapAttribute.h"
//#include "ImfExtraAttributes.h"
#include "OpenEXR/IlmImf/ImfFloatAttribute.h"
#include "OpenEXR/IlmImf/ImfFrameBuffer.h"
#include "OpenEXR/IlmImf/ImfHeader.h"
#include "OpenEXR/IlmImf/ImfInputFile.h"
#include "OpenEXR/IlmImf/ImfLineOrderAttribute.h"
#include "OpenEXR/IlmImf/ImfOutputFile.h"
#include "OpenEXR/IlmImf/ImfPixelType.h"
#include "OpenEXR/IlmImf/ImfRgba.h"
#include "OpenEXR/IlmImf/ImfRgbaFile.h"
#include "OpenEXR/IlmImf/ImfStandardAttributes.h"
#include "OpenEXR/IlmImf/ImfStringAttribute.h"
#include "OpenEXR/IlmImf/ImfTestFile.h"
#include "OpenEXR/IlmImf/ImfVecAttribute.h"
//#include "ImfVectorAttribute.h"

#include "fileformat_mex.h"

namespace openexr {

/*
 * TODO: Design: Everything that could be called from mexfile is using MxArray.
 * TODO: Maybe add support for preview images?
 * TODO: Add general support for alpha channel?
 */

typedef float FloatUsed;
const Imf::PixelType kEXRFloatUsed = Imf::FLOAT;

inline mex::MxNumeric<bool> isOpenExrFile(const mex::MxString& fileName) {
	return mex::MxNumeric<bool>(Imf::isOpenExrFile(fileName.c_str()));
}

/*
 * TODO: Maybe replace remaining arguments of these functions to use directly
 * MxArrays?
 */

class ExrInputFile : public fileformat::InputFileInterface {
public:

	explicit ExrInputFile(const mex::MxString& fileName);

	mex::MxString getFileName() const;
	mex::MxNumeric<bool> isValidFile() const;

	int getHeight() const;
	int getWidth() const;
	int getNumberOfChannels() const;
	mex::MxArray readData();
	mex::MxArray readDataRGB();
	mex::MxArray readDataY();
	mex::MxArray readData(const mex::MxString& channelName);
	mex::MxArray readData(const std::vector<mex::MxString>& channelNames);

	inline std::vector<mex::MxString> getChannelNames() const {
		std::vector<mex::MxString> channelNames(0);
		for (Imf::ChannelList::ConstIterator
				channelIter = m_file.header().channels().begin(),
				channelEnd = m_file.header().channels().end();
				channelIter != channelEnd;
				++channelIter) {
			channelNames.push_back(mex::MxString(channelIter.name()));
		}
		return channelNames;
	}

	inline bool isComplete() const {
		return m_file.isComplete();
	}

	inline bool hasChannel(const std::string& channelName) const {
		return m_file.header().channels().findChannel(channelName.c_str()) != 0;
	}

	virtual mex::MxArray getAttribute(const mex::MxString& attributeName) const;
	virtual mex::MxArray getAttribute() const;

	virtual void setAttribute(const mex::MxString& /* attributeName */,
					const mex::MxArray& /* attribute */) {	};
	virtual void setAttribute(const mex::MxStruct& /* attributes */) {	};

	~ExrInputFile() {	}

private:
	Imf::InputFile m_file;
};

class ExrOutputFile : public fileformat::OutputFileInterface {
public:
	/*
	 * TODO: Should this be initialized directly from pixels MxNumeric? It may
	 * be desirable to create just the header. On the other hand, it would be
	 * safer in terms of passing width and height arguments to the
	 * initialization of the header.
	 */
	ExrOutputFile(const mex::MxString& fileName, int width, int height,
				int numberOfChannels);

	mex::MxString getFileName() const;
	int getHeight() const;
	int getWidth() const;

	virtual void setAttribute(const mex::MxString& attributeName,
						const mex::MxArray& attribute);
	virtual void setAttribute(const mex::MxStruct& attributes);

	void writeChannelRGB(const mex::MxNumeric<FloatUsed>& rgbPixels);
	void writeChannelY(const mex::MxNumeric<FloatUsed>& yPixels);
	void writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
						const std::string& channelName);
	void writeChannel(const mex::MxNumeric<FloatUsed>& channelPixels,
						const std::vector<std::string>& channelNames);

	void writeData(const mex::MxArray& data);
	void writeDataRGB(const mex::MxArray& data);
	void writeDataY(const mex::MxArray& data);
	void writeData(const mex::MxString& channelName, const mex::MxArray& data);
	void writeData(const std::vector<mex::MxString>& channelNames,
				const mex::MxArray& data);

	~ExrOutputFile() {
		if (m_createdFrameBuffer) {
			m_pixelBuffer.destroy();
		}
	}

private:
	Imf::Header m_header;
	std::string m_fileName;
	Imf::FrameBuffer m_frameBuffer;
	mex::MxNumeric<FloatUsed> m_pixelBuffer;
	bool m_createdFrameBuffer;
	bool m_writtenFile;
};

}	/* namespace exr */

#endif /* OPENEXR_MEX_H_ */
