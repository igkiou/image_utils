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

inline mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName);

class PFMHeader {
	PFMHeader();

	void build(const EColorFormat colorFormat,
			const int width,
			const int height,
			const float scale,
			const EByteOrder byteOrder);

	bool read(std::ifstream& stream);

	void write(std::ofstream& stream) const;

	mex::MxStruct toMxArray() const;

	inline const EColorFormat get_colorFormat() const {
		return m_colorFormat;
	}

	inline const int get_width() const {
		return m_width;
	}

	inline const int get_height() const {
		return m_height;
	}

	inline const float get_scale() const {
		return m_scale;
	}

	inline const EByteOrder get_byteOrder() const {
		return m_byteOrder;
	}

private:
	EColorFormat m_colorFormat;
	int m_width;
	int m_height;
	float m_scale;
	EByteOrder m_byteOrder;
};

class PFMInputFile {
public:
	explicit PFMInputFile(const mex::MxString& fileName);

	mex::MxStruct readHeader() const;

	mex::MxNumeric<float> readFile() const;

	~PFMInputFile();

private:
	std::ifstream m_file;
	PFMHeader m_header;
	bool m_readHeader;
	bool m_readFile;
};

class PFMOutputFile {
public:
	explicit PFMOutputFile(const mex::MxString& fileName);

	void writeFile(mex::MxNumeric<float>& pixels) const;

	~PFMOutputFile();

private:
	std::ofstream m_file;
	PFMHeader m_header;
};

}	/* namespace pfm */

#endif /* PFM_MEX_H_ */
