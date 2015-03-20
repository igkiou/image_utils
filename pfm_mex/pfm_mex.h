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
#include <ctype.h>

#include "fileformat_mex.h"

namespace pfm {

typedef float FloatUsed;

class PfmHeader {
public:
	PfmHeader();

	enum class EColorFormat {
		ERGB = 0,
		EGrayscale,
		EMultichannel,
		ELength,
		EInvalid = -1
	};

	enum class EByteOrder {
		EBigEndian = 0,
		ELittleEndian,
		ELength,
		EInvalid = -1
	};

	void build(const EColorFormat colorFormat,
			const int width,
			const int height,
			const FloatUsed scale,
			const EByteOrder byteOrder);

	EColorFormat get_colorFormat() const;

	int get_width() const;

	int get_height() const;

	inline FloatUsed get_scale() const {
		return m_scale;
	}

	inline EByteOrder get_byteOrder() const {
		return m_byteOrder;
	}

	inline bool isValidPfmHeader() const {
		return m_isValidPfmHeader;
	}

private:
	EColorFormat m_colorFormat;
	int m_width;
	int m_height;
	FloatUsed m_scale;
	EByteOrder m_byteOrder;
	bool m_isValidPfmHeader;
};

mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName);

class PfmInputFile : public fileformat::InputFileInterface {
public:
	explicit PfmInputFile(const mex::MxString& fileName);

	mex::MxString getFileName() const;
	mex::MxNumeric<bool> isValidFile() const;
	int getHeight() const;
	int getWidth() const;
	int getNumberOfChannels() const;
	mex::MxArray getAttribute(const mex::MxString& attributeName) const;
	mex::MxArray getAttribute() const;
	mex::MxArray readData();

	~PfmInputFile() {	}

private:
	void readHeader();

	std::ifstream m_file;
	PfmHeader m_header;
	bool m_readHeader;
	bool m_readFile;
	bool m_isValidHeader;
};

class PfmOutputFile : public fileformat::OutputFileInterface {
public:
	PfmOutputFile(const mex::MxString& fileName, int width, int height);

	mex::MxString getFileName() const;
	int getHeight() const;
	int getWidth() const;
	void setAttribute(const mex::MxString& attributeName,
					const mex::MxArray& attribute);
	void setAttribute(const mex::MxStruct& attributes);
	void writeData(const mex::MxArray& data);

	~PfmOutputFile() {	}

private:
	void writeHeader(const mex::MxNumeric<FloatUsed>& pixels);

	std::ofstream m_file;
	PfmHeader m_header;
	bool m_wroteHeader;
	bool m_wroteFile;
};

}	/* namespace pfm */

#endif /* PFM_MEX_H_ */
