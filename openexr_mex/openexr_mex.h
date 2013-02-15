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

const static mex::ConstMap<std::string, EAttributeType> attributeNameToAttributeType = mex::ConstMap<std::string, EAttributeType>
	("gain",				EAttributeFloat)
	("wavelength",			EAttributeFloat)
	("extTube",				EAttributeString)
	("lens",				EAttributeString)
	("material",			EAttributeString)
	("chromaticities",		EAttributeChromaticities)
	("whiteLuminance",		EAttributeFloat)
	("adoptedNeutral",		EAttributeV2f)
	("renderingTransform",	EAttributeString)
	("lookModTransform",	EAttributeFloat)
	("xDensity",			EAttributeFloat)
	("owner",				EAttributeString)
	("comments",			EAttributeString)
	("capDate",				EAttributeString)
	("utcOffset",			EAttributeFloat)
	("longitude",			EAttributeFloat)
	("latitude",			EAttributeFloat)
	("altitude",			EAttributeFloat)
	("focus",				EAttributeFloat)
	("expTime",				EAttributeFloat)
	("aperture",			EAttributeFloat)
	("isoSpeed",			EAttributeFloat)
	("multExpTimes",		EAttributeVf)
	("multApertures",		EAttributeVf)
	("multIsoSpeeds",		EAttributeVf)
	("multGains",			EAttributeVf)
	("envmap",				EAttributeEnvmap)
	("unknown",				EAttributeTypeInvalid);

const static mex::ConstMap<std::string, EAttributeType> stringToAttributeType = mex::ConstMap<std::string, EAttributeType>
	("chlist",			EAttributeChannelList)
	("compression",		EAttributeCompression)
	("lineOrder",		EAttributeLineOrder)
	("chromaticities",	EAttributeChromaticities)
	("envmap",			EAttributeEnvmap)
	("string",			EAttributeString)
	("box2d",			EAttributeBox2d)
	("box2f",			EAttributeBox2f)
	("box2i",			EAttributeBox2i)
	("v2d",				EAttributeV2d)
	("v2f",				EAttributeV2f)
	("v2i",				EAttributeV2i)
	("vd",				EAttributeVd)
	("vf",				EAttributeVf)
	("vi",				EAttributeVi)
	("double",			EAttributeDouble)
	("float",			EAttributeFloat)
	("int",				EAttributeInt)
	("unknown",			EAttributeTypeInvalid);

const static mex::ConstMap<EAttributeType, std::string> attributeTypeToString = mex::ConstMap<EAttributeType, std::string>
	(EAttributeChannelList, 	"chlist")
	(EAttributeCompression,		"compression")
	(EAttributeLineOrder,		"lineOrder")
	(EAttributeChromaticities,	"chromaticities")
	(EAttributeEnvmap,			"envmap")
	(EAttributeString,			"string")
	(EAttributeBox2d,			"box2d")
	(EAttributeBox2f,			"box2f")
	(EAttributeBox2i,			"box2i")
	(EAttributeV2d,				"v2d")
	(EAttributeV2f,				"v2f")
	(EAttributeV2i,				"v2i")
	(EAttributeVd,				"vd")
	(EAttributeVf,				"vf")
	(EAttributeVi,				"vi")
	(EAttributeDouble,			"double")
	(EAttributeFloat,			"float")
	(EAttributeInt,				"int")
	(EAttributeTypeInvalid,		"unknown");

const static mex::ConstMap<std::string, Imf::Compression> stringToCompressionType = mex::ConstMap<std::string, Imf::Compression>
	("no", 		Imf::NO_COMPRESSION)
	("rle",		Imf::RLE_COMPRESSION)
	("zips",	Imf::ZIPS_COMPRESSION)
	("zip",		Imf::ZIP_COMPRESSION)
	("piz",		Imf::PIZ_COMPRESSION)
	("pxr24",	Imf::PXR24_COMPRESSION)
	("b44",		Imf::B44_COMPRESSION)
	("b44a",	Imf::B44A_COMPRESSION)
	("unknown",	Imf::NUM_COMPRESSION_METHODS);

const static mex::ConstMap<Imf::Compression, std::string> compressionTypeToString = mex::ConstMap<Imf::Compression, std::string>
	(Imf::NO_COMPRESSION, 			"no")
	(Imf::RLE_COMPRESSION,			"rle")
	(Imf::ZIPS_COMPRESSION,			"zips")
	(Imf::ZIP_COMPRESSION,			"zip")
	(Imf::PIZ_COMPRESSION,			"piz")
	(Imf::PXR24_COMPRESSION,		"pxr24")
	(Imf::B44_COMPRESSION,			"b44")
	(Imf::B44A_COMPRESSION,			"b44a")
	(Imf::NUM_COMPRESSION_METHODS,	"unknown");

const static mex::ConstMap<std::string, Imf::LineOrder> stringToLineOrderType = mex::ConstMap<std::string, Imf::LineOrder>
	("increasing_y", 	Imf::INCREASING_Y)
	("decreasing_y",	Imf::DECREASING_Y)
	("random_y",		Imf::RANDOM_Y)
	("unknown",			Imf::NUM_LINEORDERS);

const static mex::ConstMap<Imf::LineOrder, std::string> lineOrderTypeToString = mex::ConstMap<Imf::LineOrder, std::string>
	(Imf::INCREASING_Y,		"increasing_y")
	(Imf::DECREASING_Y,		"decreasing_y")
	(Imf::RANDOM_Y,			"random_y")
	(Imf::NUM_LINEORDERS,	"unknown");

const static mex::ConstMap<std::string, Imf::Envmap> stringToEnvmapType = mex::ConstMap<std::string, Imf::Envmap>
	("latlong", Imf::ENVMAP_LATLONG)
	("cube",	Imf::ENVMAP_CUBE)
	("unknown",	Imf::NUM_ENVMAPTYPES);

const static mex::ConstMap<Imf::Envmap, std::string> envmapTypeToString = mex::ConstMap<Imf::Envmap, std::string>
	(Imf::ENVMAP_LATLONG, 	"latlong")
	(Imf::ENVMAP_CUBE,		"cube")
	(Imf::NUM_ENVMAPTYPES,	"unknown");

class EXRAttribute {
public:
	EXRAttribute()
				: m_type(EAttributeTypeInvalid),
				  m_pAttribute() {	}

	EXRAttribute(const Imf::Attribute* attribute)
				: m_type(attributeNameToAttributeType(attribute->typeName())), \
				  m_pAttribute(attribute) {	}

	inline const EAttributeType get_type() const {
		return m_type;
	}

	inline EAttributeType get_type() {
		return m_type;
	}

	inline const Imf::Attribute* get_attribute() const {
		return m_pAttribute;
	}

	inline Imf::Attribute* get_attribute() {
		return m_pAttribute;
	}

	inline const mex::MxArray get_attribute() const {
		return m_array;
	}

	inline mex::MxArray get_array() {
		return m_array;
	}

	void buildMxArray();

	void buildAttribute();

private:
	EAttributeType m_type;
	Imf::Attribute *m_pAttribute;
	mex::MxArray m_array;
};

template <typename T>
class EXRTypedAttribute<Imath::Vec2<T> > {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imath::Vec2<T> > *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imath::Vec2<T> > * attr)
					: m_type(EXRAttribute<Imath::Vec2<T> >()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		std::vector<T> temp;
		temp.push_back(m_attr->value().x);
		temp.push_back(m_attr->value().y);
		return mex::MxNumeric<T>(temp);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imath::Vec2<T> > &>(*this).toMxArray());
	}
};

template <typename T>
class EXRTypedAttribute<std::vector<T> > {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<std::vector<T> > *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<std::vector<T> > * attr)
					: m_type(EXRAttribute<std::vector<T> >()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxNumeric<T>(m_attr->value());
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<std::vector<T> > &>(*this).toMxArray());
	}
};

template <typename T>
class EXRTypedAttribute<Imath::Box<Imath::Vec2<T> > > {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > > *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > > * attr)
					: m_type(EXRAttribute<Imath::Box<Imath::Vec2<T> > >()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		std::vector<mex::MxArray> temp;
		std::vector<T> tempV;
		tempV.push_back(m_attr->value().min.x);
		tempV.push_back(m_attr->value().min.y);
		temp.push_back(mex::MxNumeric<T>(tempV));
		tempV.clear();
		tempV.push_back(m_attr->value().max.x);
		tempV.push_back(m_attr->value().max.y);
		temp.push_back(mex::MxNumeric<T>(tempV));
		return mex::MxCell(temp);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imath::Box<Imath::Vec2<T> > > &>(*this).toMxArray());
	}
};

template <>
class EXRTypedAttribute<std::string> {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<std::string> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<std::string> * attr)
					: m_type(EXRAttribute<std::string>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(m_attr->value());
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<std::string> &>(*this).toMxArray());
	}
};

template <>
class EXRTypedAttribute<Imf::Envmap> {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imf::Envmap> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::Envmap> * attr)
					: m_type(EXRAttribute<Imf::Envmap>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(envmapTypeToString[m_attr->value()]);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::Envmap> &>(*this).toMxArray());
	}
};

template <>
class EXRTypedAttribute<Imf::Chromaticities> {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imf::Chromaticities> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::Chromaticities> * attr)
					: m_type(EXRAttribute<Imf::Chromaticities>()), \
					  m_attr(attr) {	}

	/*
	 * TODO: Change this to return struct instead.
	 */
	inline const mex::MxArray toMxArray() const {
		mex::MxArray* temp[8];
		temp[0] = mex::MxString("red");
		temp[1] = mex::MxString("green");
		temp[2] = mex::MxString("blue");
		temp[3] = mex::MxString("white");
		std::vector<float> tempV;
		tempV.push_back(m_attr->value().red.x);
		tempV.push_back(m_attr->value().red.y);
		temp[4] = mex::MxNumeric<float>(tempV);
		tempV.clear();
		tempV.push_back(m_attr->value().green.x);
		tempV.push_back(m_attr->value().green.y);
		temp[5] = mex::MxNumeric<float>(tempV);
		tempV.clear();
		tempV.push_back(m_attr->value().blue.x);
		tempV.push_back(m_attr->value().blue.y);
		temp[6] = mex::MxNumeric<float>(tempV);
		tempV.clear();
		tempV.push_back(m_attr->value().white.x);
		tempV.push_back(m_attr->value().white.y);
		temp[7] = mex::MxNumeric<float>(tempV);
		return MxCell(temp, 3, 2);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::Chromaticities> &>(*this).toMxArray());
	}
};

template <>
class EXRTypedAttribute<Imf::LineOrder> {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imf::LineOrder> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::LineOrder> * attr)
					: m_type(EXRAttribute<Imf::LineOrder>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(lineOrderTypeToString[m_attr->value()]);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::LineOrder> &>(*this).toMxArray());
	}
};

template <>
class EXRTypedAttribute<Imf::Compression> {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imf::Compression> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::Compression> * attr)
					: m_type(EXRAttribute<Imf::Compression>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(compressionTypeToString[m_attr->value()]);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::Compression> &>(*this).toMxArray());
	}
};

template <>
class EXRTypedAttribute<Imf::ChannelList> {
protected:
	EAttributeType m_type;
	Imf::TypedAttribute<Imf::ChannelList> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::ChannelList> * attr)
					: m_type(EXRAttribute<Imf::ChannelList>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		std::vector<mex::MxString> temp;
		for (Imf::ChannelList::ConstIterator chIt = m_attr->value().begin(), \
			chEnd = m_attr->value().end(); \
			chIt != chEnd; \
			++chIt) {
			temp.push_back(mex::MxString(chIt.name()));
		}
		return mex::MxCell(temp);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::ChannelList> &>(*this).toMxArray());
	}
};

class EXRInputFile {
private:
	Imf::InputFile m_file;

public:
	EXRInputFile(const std::string &fileName)
				: m_file(fileName.c_str()) {	}

	inline int getWidth() const {
		Imath::Box2i dw = m_file.header().dataWindow();
		return dw.max.x - dw.min.x + 1;
	}

	inline int getHeight() const {
		Imath::Box2i dw = m_file.header().dataWindow();
		return dw.max.y - dw.min.y + 1;
	}

	inline void getDimensions(int &width, int &height) const {
		Imath::Box2i dw = m_file.header().dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}

	inline int getNumberOfChannels() const {
		int numChannels = 0;
		for (Imf::ChannelList::ConstIterator channelIter = m_file.header().channels().begin(), \
				channelEnd = m_file.header().channels().end(); \
				channelIter != channelEnd; \
				++channelIter, ++numChannels);
		return numChannels;
	}

	inline std::vector<std::string>& getChannelNames() const {
		std::vector<std::string> channelNames(0);
		for (Imf::ChannelList::ConstIterator channelIter = m_file.header().channels().begin(), \
				channelEnd = m_file.header().channels().end(); \
				channelIter != channelEnd; \
				++channelIter) {
			channelNames.push_back(std::string(channelIter.name()));
		}
		return channelNames;
	}

	mex::MxArray getAttribute(const std::string &attributeName) const;
	mex::MxArray getAttribute() const;

	/*
	 * TODO: Find way to avoid code replication in these, similar to how it is
	 * done in writeChannel in EXROutputFile.
	 * TODO: Change these to word directly with pointers to float, as in
	 * writeChannel in EXROutputFile.
	 */
	void readChannelRGBA(Imf::Array2D<FloatUsed> &rPixels, bool &rFlag, \
						Imf::Array2D<FloatUsed> &gPixels, bool &gFlag, \
						Imf::Array2D<FloatUsed> &bPixels, bool &bFlag, \
						Imf::Array2D<FloatUsed> &aPixels, bool &aFlag) const;

	void readChannelYA(Imf::Array2D<FloatUsed> &yPixels, bool &yFlag, \
					Imf::Array2D<FloatUsed> &aPixels, bool &aFlag) const;

	void readChannel(Imf::Array2D<FloatUsed> &cPixels, bool &cFlag, \
					const std::string &cName) const;

	void readChannel(std::vector<Imf::Array2D<FloatUsed> > &cPixels, \
					std::vector<bool> &cFlags, \
					const std::vector<std::string> &cNames) const;

	virtual ~EXRInputFile() {	}
};

class EXROutputFile {
private:
	Imf::Header m_header;
	Imf::FrameBuffer m_frameBuffer;
	bool m_createdFrameBuffer;
	bool m_wroteFile;

public:
	EXROutputFile(const int width, const int height)
				: m_header(width, height), \
				  m_frameBuffer(), \
				  m_createdFrameBuffer(false), \
				  m_wroteFile(false) {	}

	inline int getWidth() const {
		Imath::Box2i dw = m_header.dataWindow();
		return dw.max.x - dw.min.x + 1;
	}

	inline int getHeight() const {
		Imath::Box2i dw = m_header.dataWindow();
		return dw.max.y - dw.min.y + 1;
	}

	inline void getDimensions(int &width, int &height) const {
		Imath::Box2i dw = m_header.dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}

	inline int getNumberOfChannels() const {
			int numChannels = 0;
			for (Imf::ChannelList::ConstIterator channelIter = m_header.channels().begin(), \
					channelEnd = m_header.channels().end(); \
					channelIter != channelEnd; \
					++channelIter, ++numChannels);
			return numChannels;
		}

	inline std::vector<std::string>& getChannelNames() const {
		std::vector<std::string> channelNames(0);
		for (Imf::ChannelList::ConstIterator channelIter = m_header.channels().begin(), \
				channelEnd = m_header.channels().end(); \
				channelIter != channelEnd; \
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
		m_header.channels().insert(channelName.c_str(), Imf::Channel(EXRFloatUsed));
	}

	inline void addChannel(const std::vector<const std::string> &channelNames) {
		for (int iter = 0, numChannels = channelNames.size(); \
			iter < numChannels; \
			++iter) {
			m_header.channels().insert(channelNames[iter].c_str(), Imf::Channel(EXRFloatUsed));
		}
	}

	void setAttribute(const std::string &attrName, const mex::MxArray &mxarr);
	void setAttribute(const mex::MxArray &mxstruct);

	void writeChannelRGBA(const FloatUsed *rPixels, \
							const FloatUsed *gPixels, \
							const FloatUsed *bPixels, \
							const FloatUsed *aPixels);

	void writeChannelYA(const FloatUsed *yPixels, \
							const FloatUsed *aPixels);

	void writeChannel(const FloatUsed *cPixels, \
						const std::string &cName);

	void writeChannel(const std::vector<const FloatUsed *> &cPixels, \
						const std::vector<const std::string> &cNames);

	inline void writeFile(const std::string &fileName) {
		Assert((m_createdFrameBuffer) && (!m_wroteFile));
		int height = getHeight();
		Imf::OutputFile outFile(fileName.c_str(), m_header);
		outFile.setFrameBuffer(m_frameBuffer);
		outFile.writePixels(height);
		m_wroteFile = true;
	}

	virtual ~EXROutputFile() {	}
};

}	/* namespace exr */

#endif /* OPENEXR_MEX_H_ */
