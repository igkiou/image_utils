/*
 * raw_mex.h
 *
 *  Created on: Jan 7, 2014
 *      Author: igkiou
 */

#ifndef RAW_MEX_H_
#define RAW_MEX_H_

#include <string>

#include "../include/imformat.h"
#include "../raw/libraw_ext.h"

/*
 * TODO: Read color and lens attributes.
 */

namespace raw {

using PixelType = unsigned short;

mex::MxNumeric<bool> isRawFile(const mex::MxString& fileName);

class RawInputFile : public imformat::ImformatInputFileInterface {
public:
	explicit RawInputFile(const mex::MxString& fileName);

	mex::MxString getFileName() const override;
	mex::MxNumeric<bool> isValidFile() const override;
	int getHeight() const override;
	int getWidth() const override;
	int getNumberOfChannels() const override;
	mex::MxArray getAttribute(const mex::MxString& attributeName) const override;
	mex::MxArray getAttribute() const override;
	mex::MxArray readData() override;
	mex::MxArray readData(const mex::MxNumeric<bool>& doSubtractDarkFrame);
	mex::MxArray readData(const mex::MxString& dcrawFlags);
	mex::MxArray readData(const mex::MxNumeric<bool>& doSubtractDarkFrame,
						const mex::MxString& dcrawFlags);

	mex::MxArray getCFAInformation();

	~RawInputFile() override = default;

private:
	mex::MxArray readData(bool doSubtractDarkFrame,
						const std::string& dcrawFlags);
	mex::MxArray getAttribute(const std::string& attributeName) const;
	void parseDcrawFlags(const std::string& dcrawFlags);
	void unpackFile();

	std::string m_fileName;
	bool m_unpackedFile;
	libraw::LibRawExtension m_rawProcessor;
};

}  // namespace raw

#endif  // RAW_MEX_H_
