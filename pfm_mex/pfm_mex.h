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

#include "mex_utils.h"

namespace pfm {

typedef float FloatUsed;

class PfmHeader {
public:
	PfmHeader();

	enum class EColorFormat {
		ERGB = 0,
		EGrayscale,
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

	mex::MxStruct toMxArray() const;

	inline EColorFormat get_colorFormat() const {
		return m_colorFormat;
	}

	inline int get_width() const {
		return m_width;
	}

	inline int get_height() const {
		return m_height;
	}

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

class PfmInputFile : public mex::MxObject {
public:
	explicit PfmInputFile(const mex::MxString& fileName);

	mex::MxNumeric<FloatUsed> readFile();

	virtual mex::MxArray getAttribute(const mex::MxString& attributeName) const;
	virtual mex::MxArray getAttribute() const;

	inline bool isOpenPfmFile() const {
		return m_isValidHeader;
	}

	virtual ~PfmInputFile() {	}

private:
	void readHeader();

	std::ifstream m_file;
	PfmHeader m_header;
	bool m_readHeader;
	bool m_readFile;
	bool m_isValidHeader;
};

class PfmOutputFile : public mex::MxObject {
public:
	explicit PfmOutputFile(const mex::MxString& fileName);

	void writeHeader(const mex::MxNumeric<FloatUsed>& pixels);

	void writeFile(const mex::MxNumeric<FloatUsed>& pixels);

	virtual ~PfmOutputFile() {	}

private:
	std::ofstream m_file;
	PfmHeader m_header;
	bool m_wroteHeader;
	bool m_wroteFile;
};

}	/* namespace pfm */

#endif /* PFM_MEX_H_ */
