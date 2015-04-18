/*
 * image_utils//image_utils/include/fileformat_mex.h/fileformat_mex.h
 *
 *  Created on: Feb 11, 2015
 *      Author: igkiou
 */

#ifndef FILEFORMAT_MEX_H_
#define FILEFORMAT_MEX_H_

#include "mex_utils.h"

namespace fileformat {

class InputFileInterface {
public:
	virtual mex::MxString getFileName() const = 0;
	virtual mex::MxNumeric<bool> isValidFile() const = 0;
	virtual int getHeight() const = 0;
	virtual int getWidth() const = 0;
	virtual int getNumberOfChannels() const = 0;
	virtual mex::MxArray getAttribute(const mex::MxString& attributeName)
									const = 0;
	virtual mex::MxArray getAttribute() const = 0;
	virtual mex::MxArray readData() = 0;
	virtual ~InputFileInterface() = default;
};

class OutputFileInterface {
public:
	virtual mex::MxString getFileName() const = 0;
	virtual int getHeight() const = 0;
	virtual int getWidth() const = 0;
	virtual void setAttribute(const mex::MxString& attributeName,
							const mex::MxArray& attribute) = 0;
	virtual void setAttribute(const mex::MxStruct& attributes) = 0;
	virtual void writeData(const mex::MxArray& data) = 0;
	virtual ~OutputFileInterface() = default;
};

}  // namespace fileformat

#endif /* FILEFORMAT_MEX_H_ */
