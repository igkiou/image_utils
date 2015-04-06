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

#include "fileformat_mex.h"

namespace pfm {

/*
 * TODO: Expand to custom file format for multiple images or multiple channels.
 */

using PixelType = float;

class PfmHeader {
public:
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

	PfmHeader();

	PfmHeader(const int width,
			const int height,
			const EColorFormat colorFormat,
			const PixelType scale,
			const EByteOrder byteOrder);

	int get_width() const;
	int get_height() const;
	EColorFormat get_colorFormat() const;
	PixelType get_scale() const;
	EByteOrder get_byteOrder() const;
	bool isValidPfmHeader() const;

	void readFromFile(std::ifstream& file);
	void writeToFile(std::ofstream& file) const;

private:
	void build(const int width,
			const int height,
			const EColorFormat colorFormat,
			const PixelType scale,
			const EByteOrder byteOrder);

	int m_width;
	int m_height;
	EColorFormat m_colorFormat;
	PixelType m_scale;
	EByteOrder m_byteOrder;
	bool m_isValidPfmHeader;
};

mex::MxNumeric<bool> isPfmFile(const mex::MxString& fileName);

class PfmInputFile : public fileformat::InputFileInterface {
public:
	explicit PfmInputFile(const mex::MxString& fileName);

	mex::MxString getFileName() const override;
	mex::MxNumeric<bool> isValidFile() const override;
	int getHeight() const override;
	int getWidth() const override;
	int getNumberOfChannels() const override;
	mex::MxArray getAttribute(const mex::MxString& attributeName) const override;
	mex::MxArray getAttribute() const override;
	mex::MxArray readData() override;

	~PfmInputFile() override {	}

private:
	mex::MxArray getAttribute(const std::string& attributeName) const;

	std::string m_fileName;
	std::ifstream m_file;
	PfmHeader m_header;
	bool m_readHeader;
	bool m_readFile;
};

class PfmOutputFile : public fileformat::OutputFileInterface {
public:
	PfmOutputFile(const mex::MxString& fileName, int width, int height);

	mex::MxString getFileName() const override;
	int getHeight() const override;
	int getWidth() const override;
	void setAttribute(const mex::MxString& attributeName,
					const mex::MxArray& attribute) override;
	void setAttribute(const mex::MxStruct& attributes) override;
	void writeData(const mex::MxArray& data) override;

	~PfmOutputFile() override {	}

private:
	void setAttribute(const std::string& attributeName,
					const mex::MxArray& attribute);

	std::string m_fileName;
	std::ofstream m_file;
	int m_width;
	int m_height;
	PixelType m_scale;
	PfmHeader m_header;
	bool m_writtenFile;
};

}	/* namespace pfm */

#endif /* PFM_MEX_H_ */
