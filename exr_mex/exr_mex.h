/*
 * exr_mex.h
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#ifndef EXR_MEX_H_
#define EXR_MEX_H_

#include <vector>
#include <string>
//#include <stdio.h>
//#include <stdarg.h>

#include "IlmBase/Iex/Iex.h"
#include "IlmBase/Imath/ImathBox.h"
#include "OpenEXR/IlmImf/ImfArray.h"
#include "OpenEXR/IlmImf/ImfAttribute.h"
#include "OpenEXR/IlmImf/ImfBoxAttribute.h"
#include "OpenEXR/IlmImf/ImfChannelList.h"
#include "OpenEXR/IlmImf/ImfChannelListAttribute.h"
#include "OpenEXR/IlmImf/ImfChromaticitiesAttribute.h"
#include "OpenEXR/IlmImf/ImfCompressionAttribute.h"
#include "OpenEXR/IlmImf/ImfDoubleAttribute.h" // new addition
#include "OpenEXR/IlmImf/ImfEnvmapAttribute.h"
//#include "ImfExtraAttributes.h"
#include "OpenEXR/IlmImf/ImfFloatAttribute.h"
#include "OpenEXR/IlmImf/ImfFloatVectorAttribute.h" // new addition
#include "OpenEXR/IlmImf/ImfFrameBuffer.h"
#include "OpenEXR/IlmImf/ImfHeader.h"
#include "OpenEXR/IlmImf/ImfIntAttribute.h" // new addition
#include "OpenEXR/IlmImf/ImfInputFile.h"
#include "OpenEXR/IlmImf/ImfLineOrderAttribute.h"
#include "OpenEXR/IlmImf/ImfOutputFile.h"
#include "OpenEXR/IlmImf/ImfPixelType.h"
#include "OpenEXR/IlmImf/ImfRgba.h"
#include "OpenEXR/IlmImf/ImfRgbaFile.h"
#include "OpenEXR/IlmImf/ImfStandardAttributes.h"
#include "OpenEXR/IlmImf/ImfStringAttribute.h"
#include "OpenEXR/IlmImf/ImfStringVectorAttribute.h" // new addition
#include "OpenEXR/IlmImf/ImfTestFile.h"
#include "OpenEXR/IlmImf/ImfVecAttribute.h"
//#include "ImfVectorAttribute.h"

#include "fileformat_mex.h"

namespace exr {

/*
 * TODO: Maybe add support for preview images?
 * TODO: Add general support for alpha channel?
 * TODO: Move EExrAttributeType enum and registeredAttributeNameAttributeTypeMap
 * in header, to allow for easy expansion of registered attributes.
 * TODO: Provide better exposure of registered attributes and their types to
 * user.
 * TODO: Provide set attribute variant that allows specifying type of attribute.
 * TODO: Replace readData(const std::vector<mex::MxString>& channelNames) with
 * readData(mex::MxCell& channelNames). Similar for getChannelNames.
 */

using PixelType = float;

mex::MxNumeric<bool> isExrFile(const mex::MxString& fileName);

class ExrInputFile : public fileformat::InputFileInterface {
public:
	explicit ExrInputFile(const mex::MxString& fileName);

	mex::MxString getFileName() const override;
	mex::MxNumeric<bool> isValidFile() const override;

	int getHeight() const override;
	int getWidth() const override;
	int getNumberOfChannels() const override;
	mex::MxArray readData() override;
	mex::MxArray readDataRGB();
	mex::MxArray readDataY();
	mex::MxArray readData(const mex::MxString& channelName);
	mex::MxArray readData(const std::vector<mex::MxString>& channelNames);

	std::vector<mex::MxString> getChannelNames() const;

	mex::MxArray getAttribute(const mex::MxString& attributeName) const override;
	mex::MxArray getAttribute() const override;

	/*
	 * TODO: Should be made private.
	 */
	bool hasChannel(const std::string& channelName) const;

	~ExrInputFile() override {	}

private:
	bool isComplete() const;


	Imf::InputFile m_file;
};

class ExrOutputFile : public fileformat::OutputFileInterface {
public:
	ExrOutputFile(const mex::MxString& fileName, int width, int height);

	mex::MxString getFileName() const override;
	int getHeight() const override;
	int getWidth() const override;

	void setAttribute(const mex::MxString& attributeName,
						const mex::MxArray& attribute) override;
	void setAttribute(const mex::MxStruct& attributes) override;

	void writeData(const mex::MxArray& data) override;
	void writeDataRGB(const mex::MxArray& data);
	void writeDataY(const mex::MxArray& data);
	void writeData(const mex::MxString& channelName, const mex::MxArray& data);
	void writeData(const std::vector<mex::MxString>& channelNames,
				const mex::MxArray& data);

	~ExrOutputFile() override {	}

private:
	Imf::Header m_header;
	std::string m_fileName;
	bool m_writtenFile;
};

}	/* namespace exr */

#endif /* EXR_MEX_H_ */
