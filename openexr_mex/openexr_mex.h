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

#include "Iex.h"
#include "ImathBox.h"
#include "ImfArray.h"
#include "ImfAttribute.h"
#include "ImfChannelList.h"
#include "ImfCompression.h"
#include "ImfExtraAttributes.h"
#include "ImfHeader.h"
#include "ImfInputFile.h"
#include "ImfOutputFile.h"
#include "ImfPixelType.h"
#include "ImfRgba.h"
#include "ImfRgbaFile.h"
#include "ImfStandardAttributes.h"

#include "mex_utils.h"

namespace exr {

typedef float FloatUsed;
typedef Imf::FLOAT EXRFloatUsed;

static const int kEXRMaxStringLength = 32;

typedef enum EAttributeType {
	EAttributeChannelList = 0,
	EAttributeCompression,
	EAttributeLineOrder,
	EAttributeChromaticities,
	EAttributeEnvmap,
	EAttributeString,
	EAttributeBox2d,
	EAttributeBox2f,
	EAttributeBox2i,
	EAttributeV2d,
	EAttributeV2f,
	EAttributeV2i,
	EAttributeVd,
	EAttributeVf,
	EAttributeVi,
	EAttributeDouble,
	EAttributeFloat,
	EAttributeInt,
	EAttributeTypeLength,
	EAttributeTypeInvalid = -1
} EAttributeType;

class EXRAttribute {
public:
	static const mex::ConstMap<std::string, EAttributeType> attributeNameToAttributeType;
	static const mex::ConstMap<std::string, EAttributeType> stringToAttributeType;
	static const mex::ConstMap<EAttributeType, std::string> attributeTypeToString;
	static const mex::ConstMap<std::string, Imf::Compression> stringToCompressionType;
	static const mex::ConstMap<Imf::Compression, std::string> compressionTypeToString;
	static const mex::ConstMap<std::string, Imf::LineOrder> stringToLineOrderType;
	static const mex::ConstMap<Imf::LineOrder, std::string> lineOrderTypeToString;
	static const mex::ConstMap<std::string, Imf::Envmap> stringToEnvmapType;
	static const mex::ConstMap<Imf::Envmap, std::string> envmapTypeToString;

	EXRAttribute()
				: m_type(EAttributeTypeInvalid),
				  m_pAttribute(),
				  m_attributeIsBuilt(false),
				  m_pArray(),
				  m_arrayIsBuilt(false) {	}

	explicit EXRAttribute(const Imf::Attribute* pAttribute)
				: m_type(stringToAttributeType(pAttribute->typeName())),
				  m_pAttribute(pAttribute),
				  m_attributeIsBuilt(true),
				  m_pArray(),
				  m_arrayIsBuilt(false) {	}

	explicit EXRAttribute(const mex::MxArray* pArray,
							const std::string& attributeName)
				: m_type(attributeNameToAttributeType(attributeName)),
				  m_pAttribute(),
				  m_attributeIsBuilt(false),
				  m_pArray(pArray),
				  m_arrayIsBuilt(true) {	}

	inline const EAttributeType get_type() const {
		return m_type;
	}

	inline EAttributeType get_type() {
		return m_type;
	}

	inline const Imf::Attribute* get_pAttribute() const {
		return m_pAttribute;
	}

	inline Imf::Attribute* get_pAttribute() {
		return m_pAttribute;
	}

	inline const mex::MxArray* get_pArray() const {
		return m_pArray;
	}

	inline mex::MxArray* get_pArray() {
		return m_pArray;
	}

	void buildMxArray();

	void buildAttribute();

private:
	EAttributeType m_type;
	Imf::Attribute* m_pAttribute;
	bool m_attributeIsBuilt;
	mex::MxArray* m_pArray;
	bool m_arrayIsBuilt;
};

class EXRInputFile {
public:
	explicit EXRInputFile(const std::string& fileName)
							: m_file(fileName.c_str()) {	}

	inline int getWidth() const {
		Imath::Box2i dw = m_file.header().dataWindow();
		return dw.max.x - dw.min.x + 1;
	}

	inline int getHeight() const {
		Imath::Box2i dw = m_file.header().dataWindow();
		return dw.max.y - dw.min.y + 1;
	}

	inline void getDimensions(int& width, int& height) const {
		Imath::Box2i dw = m_file.header().dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}

	inline int getNumberOfChannels() const {
		int numChannels = 0;
		for (Imf::ChannelList::ConstIterator
				channelIter = m_file.header().channels().begin(),
				channelEnd = m_file.header().channels().end();
				channelIter != channelEnd;
				++channelIter, ++numChannels) {
			continue;
		}
		return numChannels;
	}

	inline std::vector<std::string> getChannelNames() const {
		std::vector<std::string> channelNames(0);
		for (Imf::ChannelList::ConstIterator
				channelIter = m_file.header().channels().begin(),
				channelEnd = m_file.header().channels().end();
				channelIter != channelEnd;
				++channelIter) {
			channelNames.push_back(std::string(channelIter.name()));
		}
		return channelNames;
	}

	mex::MxArray getAttribute(const std::string& attributeName) const;
	mex::MxArray getAttribute() const;

	/*
	 * TODO: Find way to avoid code replication in these, similar to how it is
	 * done in writeChannel in EXROutputFile.
	 * TODO: Change these to word directly with pointers to float, as in
	 * writeChannel in EXROutputFile.
	 */
	void readChannelRGBA(Imf::Array2D<FloatUsed>& rPixels, bool& rFlag,
						Imf::Array2D<FloatUsed>& gPixels, bool& gFlag,
						Imf::Array2D<FloatUsed>& bPixels, bool& bFlag,
						Imf::Array2D<FloatUsed>& aPixels, bool& aFlag) const;

	void readChannelYA(Imf::Array2D<FloatUsed>& yPixels, bool& yFlag,
						Imf::Array2D<FloatUsed>& aPixels, bool& aFlag) const;

	void readChannel(Imf::Array2D<FloatUsed>& cPixels, bool& cFlag,
					const std::string& cName) const;

	void readChannel(std::vector<Imf::Array2D<FloatUsed> >& cPixels,
					std::vector<bool>& cFlags,
					const std::vector<std::string>& cNames) const;

	virtual ~EXRInputFile() {	}

private:
	Imf::InputFile m_file;
};

class EXROutputFile {
public:
	EXROutputFile(const int width, const int height)
				: m_header(width, height),
				  m_frameBuffer(),
				  m_createdFrameBuffer(false),
				  m_wroteFile(false) {	}

	inline int getWidth() const {
		Imath::Box2i dw = m_header.dataWindow();
		return dw.max.x - dw.min.x + 1;
	}

	inline int getHeight() const {
		Imath::Box2i dw = m_header.dataWindow();
		return dw.max.y - dw.min.y + 1;
	}

	inline void getDimensions(int& width, int& height) const {
		Imath::Box2i dw = m_header.dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}

	inline int getNumberOfChannels() const {
			int numChannels = 0;
			for (Imf::ChannelList::ConstIterator
					channelIter = m_header.channels().begin(),
					channelEnd = m_header.channels().end();
					channelIter != channelEnd;
					++channelIter, ++numChannels) {
				continue;
			}
			return numChannels;
		}

	inline std::vector<std::string>& getChannelNames() const {
		std::vector<std::string> channelNames(0);
		for (Imf::ChannelList::ConstIterator
				channelIter = m_header.channels().begin(),
				channelEnd = m_header.channels().end();
				channelIter != channelEnd;
				++channelIter) {
			channelNames.push_back(std::string(channelIter.name()));
		}
		return channelNames;
	}

	inline void addChannelRGBA() {
		m_header.channels().insert("R", Imf::Channel(EXRFloatUsed));
		m_header.channels().insert("G", Imf::Channel(EXRFloatUsed));
		m_header.channels().insert("B", Imf::Channel(EXRFloatUsed));
		m_header.channels().insert("A", Imf::Channel(EXRFloatUsed));
	}

	inline void addChannelYA() {
		m_header.channels().insert("Y", Imf::Channel(EXRFloatUsed));
		m_header.channels().insert("A", Imf::Channel(EXRFloatUsed));
	}

	inline void addChannel(const std::string& channelName) {
		m_header.channels().insert(channelName.c_str(),
									Imf::Channel(EXRFloatUsed));
	}

	inline void addChannel(const std::vector<const std::string>&
							channelNames) {
		for (int iter = 0, numChannels = channelNames.size();
			iter < numChannels;
			++iter) {
			m_header.channels().insert(channelNames[iter].c_str(),
										Imf::Channel(EXRFloatUsed));
		}
	}

	void setAttribute(const std::string& attrName,
						const mex::MxArray& mxarr);

	void setAttribute(const mex::MxArray& mxstruct);

	void writeChannelRGBA(const FloatUsed* rPixels,
							const FloatUsed* gPixels,
							const FloatUsed* bPixels,
							const FloatUsed* aPixels);

	void writeChannelYA(const FloatUsed* yPixels,
						const FloatUsed* aPixels);

	void writeChannel(const FloatUsed* cPixels,
						const std::string& cName);

	void writeChannel(const std::vector<const FloatUsed*>& cPixels,
						const std::vector<const std::string>& cNames);

	inline void writeFile(const std::string& fileName) {
		Assert((m_createdFrameBuffer) && (!m_wroteFile));
		int height = getHeight();
		Imf::OutputFile outFile(fileName.c_str(), m_header);
		outFile.setFrameBuffer(m_frameBuffer);
		outFile.writePixels(height);
		m_wroteFile = true;
	}

	virtual ~EXROutputFile() {	}

private:
	Imf::Header m_header;
	Imf::FrameBuffer m_frameBuffer;
	bool m_createdFrameBuffer;
	bool m_wroteFile;
};

}	/* namespace exr */

#endif /* OPENEXR_MEX_H_ */
