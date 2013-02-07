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

typedef float USED;
typedef Imf::FLOAT USEDC;

enum {EXR_MAX_STRING_LENGTH = 32};

typedef enum ATTR_TYPE {
	ATTR_CHLIST = 0,
	ATTR_COMPRESSION = 1,
	ATTR_LINEORDER = 2,
	ATTR_CHROMATICITIES = 3,
	ATTR_ENVMAP = 4,
	ATTR_STRING = 5,
	ATTR_BOX2D = 6,
	ATTR_BOX2F = 7,
	ATTR_BOX2I = 8,
	ATTR_V2D = 9,
	ATTR_V2F = 10,
	ATTR_V2I = 11,
	ATTR_VD = 12,
	ATTR_VF = 13,
	ATTR_VI = 14,
	ATTR_DOUBLE = 15,
	ATTR_FLOAT = 16,
	ATTR_INT = 17,
	ATTR_TYPE_LENGTH = 18,
	ATTR_TYPE_INVALID = -1
} ATTR_TYPE;

/*
 * TODO: Put these maps and routines in class.
 */

//inline ATTR_TYPE attrNameToAttrType(const char * query) {
//	if (!strcasecmp(query, "gain")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "wavelength")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "extTube")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "lens")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "material")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "chromaticities")) {
//		return ATTR_CHROMATICITIES;
//	} else if (!strcasecmp(query, "whiteLuminance")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "adoptedNeutral")) {
//		return ATTR_V2F;
//	} else if (!strcasecmp(query, "renderingTransform")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "lookModTransform")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "xDensity")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "owner")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "comments")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "capDate")) {
//		return ATTR_STRING;
//	} else if (!strcasecmp(query, "utcOffset")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "longitude")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "latitude")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "altitude")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "focus")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "expTime")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "aperture")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "isoSpeed")) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp(query, "multExpTimes")) {
//		return ATTR_VF;
//	} else if (!strcasecmp(query, "multApertures")) {
//		return ATTR_VF;
//	} else if (!strcasecmp(query, "multIsoSpeeds")) {
//		return ATTR_VF;
//	} else if (!strcasecmp(query, "multGains")) {
//		return ATTR_VF;
//	} else if (!strcasecmp(query, "envmap")) {
//		return ATTR_ENVMAP;
//	} else {
//		return ATTR_TYPE_INVALID;
//	}
//}



//inline ATTR_TYPE stringToAttrType(const char * propertyName) {
//	if (!strcasecmp("chlist", propertyName)) {
//		return ATTR_CHLIST;
//	} else if (!strcasecmp("compression", propertyName)) {
//		return ATTR_COMPRESSION;
//	} else if (!strcasecmp("lineOrder", propertyName)) {
//		return ATTR_LINEORDER;
//	} else if (!strcasecmp("chromaticities", propertyName)) {
//		return ATTR_CHROMATICITIES;
//	} else if (!strcasecmp("envmap", propertyName)) {
//		return ATTR_ENVMAP;
//	} else if (!strcasecmp("string", propertyName)) {
//		return ATTR_STRING;
//	} else if (!strcasecmp("box2d", propertyName)) {
//		return ATTR_BOX2D;
//	} else if (!strcasecmp("box2f", propertyName)) {
//		return ATTR_BOX2F;
//	} else if (!strcasecmp("box2i", propertyName)) {
//		return ATTR_BOX2I;
//	} else if (!strcasecmp("v2d", propertyName)) {
//		return ATTR_V2D;
//	} else if (!strcasecmp("v2f", propertyName)) {
//		return ATTR_V2F;
//	} else if (!strcasecmp("v2i", propertyName)) {
//		return ATTR_V2I;
//	} else if (!strcasecmp("vd", propertyName)) {
//		return ATTR_VD;
//	} else if (!strcasecmp("vf", propertyName)) {
//		return ATTR_VF;
//	} else if (!strcasecmp("vi", propertyName)) {
//		return ATTR_VI;
//	} else if (!strcasecmp("double", propertyName)) {
//		return ATTR_DOUBLE;
//	} else if (!strcasecmp("float", propertyName)) {
//		return ATTR_FLOAT;
//	} else if (!strcasecmp("int", propertyName)) {
//		return ATTR_INT;
//	} else {
//		return ATTR_TYPE_INVALID;
//	}
//}



//inline void attrTypeToString(const ATTR_TYPE property, \
//							char *propertyName) {
//
//	switch (property) {
//		case ATTR_CHLIST: { strcpy(propertyName, "chlist"); return; }
//		case ATTR_COMPRESSION: { strcpy(propertyName, "compression"); return; }
//		case ATTR_LINEORDER: { strcpy(propertyName, "lineOrder"); return; }
//		case ATTR_CHROMATICITIES: { strcpy(propertyName, "chromaticities"); return; }
//		case ATTR_ENVMAP: { strcpy(propertyName, "envmap"); return; }
//		case ATTR_STRING: { strcpy(propertyName, "string"); return; }
//		case ATTR_BOX2D: { strcpy(propertyName, "box2d"); return; }
//		case ATTR_BOX2F: { strcpy(propertyName, "box2f"); return; }
//		case ATTR_BOX2I: { strcpy(propertyName, "box2i"); return; }
//		case ATTR_V2D: { strcpy(propertyName, "v2d"); return; }
//		case ATTR_V2F: { strcpy(propertyName, "v2f"); return; }
//		case ATTR_V2I: { strcpy(propertyName, "v2i"); return; }
//		case ATTR_VD: { strcpy(propertyName, "vd"); return; }
//		case ATTR_VF: { strcpy(propertyName, "vf"); return; }
//		case ATTR_VI: { strcpy(propertyName, "vi"); return; }
//		case ATTR_DOUBLE: { strcpy(propertyName, "double"); return; }
//		case ATTR_FLOAT: { strcpy(propertyName, "float"); return; }
//		case ATTR_INT: { strcpy(propertyName, "int"); return; }
//		case ATTR_TYPE_INVALID:
//		default: { strcpy(propertyName, "unknown"); return; }
//	}
//}



//inline Imf::Compression stringToCompressionType(const char * propertyName) {
//
//	if (!strcmp("no", propertyName)) {
//		return Imf::NO_COMPRESSION;
//	} else if (!strcmp("rle", propertyName)) {
//		return Imf::RLE_COMPRESSION;
//	} else if (!strcmp("zips", propertyName)) {
//		return Imf::ZIPS_COMPRESSION;
//	} else if (!strcmp("zip", propertyName)) {
//		return Imf::ZIP_COMPRESSION;
//	} else if (!strcmp("piz", propertyName)) {
//		return Imf::PIZ_COMPRESSION;
//	} else if (!strcmp("pxr24", propertyName)) {
//		return Imf::PXR24_COMPRESSION;
//	} else if (!strcmp("b44", propertyName)) {
//		return Imf::B44_COMPRESSION;
//	} else if (!strcmp("b44a", propertyName)) {
//		return Imf::B44A_COMPRESSION;
//	} else {
//		Assert(0);
//	}
//}



//inline void compressionTypeToString(const Imf::Compression property, \
//									char * propertyName) {
//	switch (property) {
//		case Imf::NO_COMPRESSION: { strcpy(propertyName, "no"); return; }
//		case Imf::RLE_COMPRESSION: { strcpy(propertyName, "rle"); return; }
//		case Imf::ZIPS_COMPRESSION: { strcpy(propertyName, "zips"); return; }
//		case Imf::ZIP_COMPRESSION: { strcpy(propertyName, "zip"); return; }
//		case Imf::PIZ_COMPRESSION: { strcpy(propertyName, "piz"); return; }
//		case Imf::PXR24_COMPRESSION: { strcpy(propertyName, "pxr24"); return; }
//		case Imf::B44_COMPRESSION: { strcpy(propertyName, "b44"); return; }
//		case Imf::B44A_COMPRESSION: { strcpy(propertyName, "b44a"); return; }
//		case Imf::NUM_COMPRESSION_METHODS:
//		default: { strcpy(propertyName, "unknown"); return; }
//	}
//}


//inline Imf::LineOrder stringToLineOrderType(const char * propertyName) {
//
//	if (!strcmp("increasing_y", propertyName)) {
//		return Imf::INCREASING_Y;
//	} else if (!strcmp("decreasing_y", propertyName)) {
//		return Imf::DECREASING_Y;
//	} else if (!strcmp("random_y", propertyName)) {
//		return Imf::RANDOM_Y;
//	} else {
//		Assert(0);
//		return -1;
//	}
//}



//inline void lineOrderTypeToString(const Imf::LineOrder property, \
//								char * propertyName) {
//	switch (property) {
//		case Imf::INCREASING_Y: { strcpy(propertyName, "increasing_y"); return; }
//		case Imf::DECREASING_Y: { strcpy(propertyName, "decreasing_y"); return; }
//		case Imf::RANDOM_Y: { strcpy(propertyName, "random_y"); return; }
//		case Imf::NUM_LINEORDERS:
//		default: { strcpy(propertyName, "unknown"); return; }
//	}
//}



//inline Imf::Envmap stringToEnvmapType(const char * query) {
//
//	if (!strcmp("latlong", query)) {
//		return Imf::ENVMAP_LATLONG;
//	} else if (!strcmp("cube", query)) {
//		return Imf::ENVMAP_CUBE;
//	} else {
//		Assert(0);
//		return -1;
//	}
//}



//inline void envmapTypeToString(const Imf::Envmap property, \
//							char * propertyName) {
//	switch (property) {
//		case Imf::ENVMAP_LATLONG: { strcpy(propertyName, "latlong"); return; }
//		case Imf::ENVMAP_CUBE: { strcpy(propertyName, "cube"); return; }
//		case Imf::NUM_ENVMAPTYPES:
//		default: { strcpy(propertyName, "unknown"); return; }
//	}
//}

const static mex::ConstMap<std::string, ATTR_TYPE> attrNameToAttrType = mex::ConstMap<std::string, ATTR_TYPE>
	("gain",				ATTR_FLOAT)
	("wavelength",			ATTR_FLOAT)
	("extTube",				ATTR_STRING)
	("lens",				ATTR_STRING)
	("material",			ATTR_STRING)
	("chromaticities",		ATTR_CHROMATICITIES)
	("whiteLuminance",		ATTR_FLOAT)
	("adoptedNeutral",		ATTR_V2F)
	("renderingTransform",	ATTR_STRING)
	("lookModTransform",	ATTR_FLOAT)
	("xDensity",			ATTR_FLOAT)
	("owner",				ATTR_STRING)
	("comments",			ATTR_STRING)
	("capDate",				ATTR_STRING)
	("utcOffset",			ATTR_FLOAT)
	("longitude",			ATTR_FLOAT)
	("latitude",			ATTR_FLOAT)
	("altitude",			ATTR_FLOAT)
	("focus",				ATTR_FLOAT)
	("expTime",				ATTR_FLOAT)
	("aperture",			ATTR_FLOAT)
	("isoSpeed",			ATTR_FLOAT)
	("multExpTimes",		ATTR_VF)
	("multApertures",		ATTR_VF)
	("multIsoSpeeds",		ATTR_VF)
	("multGains",			ATTR_VF)
	("envmap",				ATTR_ENVMAP)
	("unknown",				ATTR_TYPE_INVALID);

const static mex::ConstMap<std::string, ATTR_TYPE> stringToAttrType = mex::ConstMap<std::string, ATTR_TYPE>
	("chlist",			ATTR_CHLIST)
	("compression",		ATTR_COMPRESSION)
	("lineOrder",		ATTR_LINEORDER)
	("chromaticities",	ATTR_CHROMATICITIES)
	("envmap",			ATTR_ENVMAP)
	("string",			ATTR_STRING)
	("box2d",			ATTR_BOX2D)
	("box2f",			ATTR_BOX2F)
	("box2i",			ATTR_BOX2I)
	("v2d",				ATTR_V2D)
	("v2f",				ATTR_V2F)
	("v2i",				ATTR_V2I)
	("vd",				ATTR_VD)
	("vf",				ATTR_VF)
	("vi",				ATTR_VI)
	("double",			ATTR_DOUBLE)
	("float",			ATTR_FLOAT)
	("int",				ATTR_INT)
	("unknown",			ATTR_TYPE_INVALID);

const static mex::ConstMap<ATTR_TYPE, std::string> attrTypeToString = mex::ConstMap<ATTR_TYPE, std::string>
	(ATTR_CHLIST, 			"chlist")
	(ATTR_COMPRESSION,		"compression")
	(ATTR_LINEORDER,		"lineOrder")
	(ATTR_CHROMATICITIES,	"chromaticities")
	(ATTR_ENVMAP,			"envmap")
	(ATTR_STRING,			"string")
	(ATTR_BOX2D,			"box2d")
	(ATTR_BOX2F,			"box2f")
	(ATTR_BOX2I,			"box2i")
	(ATTR_V2D,				"v2d")
	(ATTR_V2F,				"v2f")
	(ATTR_V2I,				"v2i")
	(ATTR_VD,				"vd")
	(ATTR_VF,				"vf")
	(ATTR_VI,				"vi")
	(ATTR_DOUBLE,			"double")
	(ATTR_FLOAT,			"float")
	(ATTR_INT,				"int")
	(ATTR_TYPE_INVALID,		"unknown");

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


template <typename T>
class AttrType {
private:
	ATTR_TYPE m_type;

public:
	AttrType() : m_type(mxUNKNOWN_CLASS) {	}
	AttrType(const AttrType& mxClass) {
		m_type = mxClass();
	}

	inline const ATTR_TYPE getAttrType() const {
		return m_type;
	}

	inline ATTR_TYPE getAttrType() {
		return m_type;
	}

	inline const ATTR_TYPE operator()() const {
		return m_type;
	}

	inline ATTR_TYPE operator()() {
		return m_type;
	}

	inline operator const ATTR_TYPE() const {
		return m_type;
	}

	inline operator ATTR_TYPE() {
		return m_type;
	}
};

template <> AttrType<int>::AttrType() : m_type(ATTR_INT) {	}
template <> AttrType<float>::AttrType() : m_type(ATTR_FLOAT) {	}
template <> AttrType<double>::AttrType() : m_type(ATTR_DOUBLE) {	}
template <> AttrType<Imath::V2i>::AttrType() : m_type(ATTR_V2I) {	}
template <> AttrType<Imath::V2f>::AttrType() : m_type(ATTR_V2F) {	}
template <> AttrType<Imath::V2d>::AttrType() : m_type(ATTR_V2D) {	}
template <> AttrType<std::vector<int> >::AttrType() : m_type(ATTR_VI) {	}
template <> AttrType<std::vector<float> >::AttrType() : m_type(ATTR_VF) {	}
template <> AttrType<std::vector<double> >::AttrType() : m_type(ATTR_VD) {	}
template <> AttrType<Imath::Box2i>::AttrType() : m_type(ATTR_BOX2I) {	}
template <> AttrType<Imath::Box2f>::AttrType() : m_type(ATTR_BOX2F) {	}
template <> AttrType<Imath::Box2d>::AttrType() : m_type(ATTR_BOX2D) {	}
template <> AttrType<std::string>::AttrType() : m_type(ATTR_STRING) {	}
template <> AttrType<Imf::Envmap>::AttrType() : m_type(ATTR_ENVMAP) {	}
template <> AttrType<Imf::Chromaticities>::AttrType() : m_type(ATTR_CHROMATICITIES) {	}
template <> AttrType<Imf::LineOrder>::AttrType() : m_type(ATTR_LINEORDER) {	}
template <> AttrType<Imf::Compression>::AttrType() : m_type(ATTR_COMPRESSION) {	}
template <> AttrType<Imf::ChannelList>::AttrType() : m_type(ATTR_CHLIST) {	}


template <typename T>
struct EXRTypedAttribute {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<T> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<T> * attr)
					: m_type(AttrType<T>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxNumeric<T>(m_attr->value());
	}

	inline mex::MxArray toMxArray() {
        return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<T> &>(*this).toMxArray());
	}
};

template <typename T>
struct EXRTypedAttribute<Imath::Vec2<T> > {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imath::Vec2<T> > *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imath::Vec2<T> > * attr)
					: m_type(AttrType<Imath::Vec2<T> >()), \
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
struct EXRTypedAttribute<std::vector<T> > {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<std::vector<T> > *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<std::vector<T> > * attr)
					: m_type(AttrType<std::vector<T> >()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxNumeric<T>(m_attr->value());
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<std::vector<T> > &>(*this).toMxArray());
	}
};

template <typename T>
struct EXRTypedAttribute<Imath::Box<Imath::Vec2<T> > > {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > > *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imath::Box<Imath::Vec2<T> > > * attr)
					: m_type(AttrType<Imath::Box<Imath::Vec2<T> > >()), \
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
struct EXRTypedAttribute<std::string> {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<std::string> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<std::string> * attr)
					: m_type(AttrType<std::string>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(m_attr->value());
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<std::string> &>(*this).toMxArray());
	}
};

template <>
struct EXRTypedAttribute<Imf::Envmap> {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imf::Envmap> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::Envmap> * attr)
					: m_type(AttrType<Imf::Envmap>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(envmapTypeToString[m_attr->value()]);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::Envmap> &>(*this).toMxArray());
	}
};

template <>
struct EXRTypedAttribute<Imf::Chromaticities> {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imf::Chromaticities> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::Chromaticities> * attr)
					: m_type(AttrType<Imf::Chromaticities>()), \
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
struct EXRTypedAttribute<Imf::LineOrder> {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imf::LineOrder> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::LineOrder> * attr)
					: m_type(AttrType<Imf::LineOrder>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(lineOrderTypeToString[m_attr->value()]);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::LineOrder> &>(*this).toMxArray());
	}
};

template <>
struct EXRTypedAttribute<Imf::Compression> {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imf::Compression> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::Compression> * attr)
					: m_type(AttrType<Imf::Compression>()), \
					  m_attr(attr) {	}

	inline const mex::MxArray toMxArray() const {
		return mex::MxString(compressionTypeToString[m_attr->value()]);
	}

	inline mex::MxArray toMxArray() {
		return const_cast<mex::MxArray>(static_cast<const EXRTypedAttribute<Imf::Compression> &>(*this).toMxArray());
	}
};

template <>
struct EXRTypedAttribute<Imf::ChannelList> {
protected:
	ATTR_TYPE m_type;
	Imf::TypedAttribute<Imf::ChannelList> *m_attr;

public:
	EXRTypedAttribute(const Imf::TypedAttribute<Imf::ChannelList> * attr)
					: m_type(AttrType<Imf::ChannelList>()), \
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

struct EXRInputFile {
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
	void readChannelRGBA(Imf::Array2D<USED> &rPixels, bool &rFlag, \
						Imf::Array2D<USED> &gPixels, bool &gFlag, \
						Imf::Array2D<USED> &bPixels, bool &bFlag, \
						Imf::Array2D<USED> &aPixels, bool &aFlag) const;

	void readChannelYA(Imf::Array2D<USED> &yPixels, bool &yFlag, \
					Imf::Array2D<USED> &aPixels, bool &aFlag) const;

	void readChannel(Imf::Array2D<USED> &cPixels, bool &cFlag, \
					const std::string &cName) const;

	void readChannel(std::vector<Imf::Array2D<USED> > &cPixels, \
					std::vector<bool> &cFlags, \
					const std::vector<std::string> &cNames) const;

	virtual ~EXRInputFile() {	}
};

struct EXROutputFile {
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
		m_header.channels().insert("R", Imf::Channel(USEDC));
		m_header.channels().insert("G", Imf::Channel(USEDC));
		m_header.channels().insert("B", Imf::Channel(USEDC));
		m_header.channels().insert("A", Imf::Channel(USEDC));
	}

	inline void addChannelYA() {
		m_header.channels().insert("Y", Imf::Channel(USEDC));
		m_header.channels().insert("A", Imf::Channel(USEDC));
	}

	inline void addChannel(const std::string& channelName) {
		m_header.channels().insert(channelName.c_str(), Imf::Channel(USEDC));
	}

	inline void addChannel(const std::vector<const std::string> &channelNames) {
		for (int iter = 0, numChannels = channelNames.size(); \
			iter < numChannels; \
			++iter) {
			m_header.channels().insert(channelNames[iter].c_str(), Imf::Channel(USEDC));
		}
	}

	void setAttribute(const std::string &attrName, const mex::MxArray &mxarr);
	void setAttribute(const mex::MxArray &mxstruct);

	void writeChannelRGBA(const USED *rPixels, \
							const USED *gPixels, \
							const USED *bPixels, \
							const USED *aPixels);

	void writeChannelYA(const USED *yPixels, \
							const USED *aPixels);

	void writeChannel(const USED *cPixels, \
						const std::string &cName);

	void writeChannel(const std::vector<const USED *> &cPixels, \
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
