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

typedef enum EColorFormat {
	ERGB = 0,
	EGrayscale,
	EColorFormatLength,
	EColorFormatInvalid = -1
} EColorFormat;

typedef enum EByteOrder {
	EBigEndian = 0,
	ELittleEndian,
	EByteTypeLength,
	EByteTypeInvalid = -1
} EByteOrder;

class PFMHeader {
public:
	PFMHeader();

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

	inline bool isValidPFMHeader() const {
		return m_isValidPFMHeader;
	}

private:
	EColorFormat m_colorFormat;
	int m_width;
	int m_height;
	FloatUsed m_scale;
	EByteOrder m_byteOrder;
	bool m_isValidPFMHeader;
};

mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName);

class PFMInputFile {
public:
	explicit PFMInputFile(const mex::MxString& fileName);

	void readHeader();

	mex::MxNumeric<FloatUsed> readFile();

	inline const PFMHeader& get_header() const {
		return m_header;
	}

	virtual ~PFMInputFile() {	}

private:
	std::ifstream m_file;
	PFMHeader m_header;
	bool m_readHeader;
	bool m_readFile;
};

class PFMOutputFile {
public:
	explicit PFMOutputFile(const mex::MxString& fileName);

	void writeHeader(const mex::MxNumeric<FloatUsed>& pixels);

	void writeFile(const mex::MxNumeric<FloatUsed>& pixels);

	virtual ~PFMOutputFile() {	}

private:
	std::ofstream m_file;
	PFMHeader m_header;
	bool m_wroteHeader;
	bool m_wroteFile;
};

}	/* namespace pfm */

#endif /* PFM_MEX_H_ */
