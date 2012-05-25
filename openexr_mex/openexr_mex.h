/*
 * openexr_mex.h
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#ifndef OPENEXR_MEX_H_
#define OPENEXR_MEX_H_

#include "ImathBox.h"
#include "ImfRgba.h"
#include "ImfRgbaFile.h"
#include "ImfInputFile.h"
#include "ImfOutputFile.h"
#include "ImfHeader.h"
#include "ImathBox.h"
#include "ImfInputFile.h"
#include "ImfHeader.h"
#include "ImfArray.h"
#include "ImfChannelList.h"
#include "ImfAttribute.h"
#include "ImfStandardAttributes.h"
#include "ImfChromaticitiesAttribute.h"
#include "ImfVectorAttribute.h"
#include "ImfStandardAttributes.h"
#include "ImfPixelType.h"
#include "Iex.h"
#include <string>
#include <vector>

#include "mex.h"
#include "matrix.h"

using namespace Imf;
using namespace Imath;
using namespace Iex;

#define USED float
#define USEDC FLOAT

enum ATTR_TYPE {
	ATTR_CHLIST,
	ATTR_COMPRESSION,
	ATTR_LINEORDER,
	ATTR_CHROMATICITIES,
	ATTR_ENVMAP,
	ATTR_STRING,
	ATTR_BOX2D,
	ATTR_BOX2F,
	ATTR_BOX2I,
	ATTR_V2D,
	ATTR_V2F,
	ATTR_V2I,
	ATTR_VD,
	ATTR_VF,
	ATTR_VI,
	ATTR_DOUBLE,
	ATTR_FLOAT,
	ATTR_INT,
	ATTR_UNKNOWN = 18
};

inline ATTR_TYPE stringToAttrType(const char * query) {
	if (!strcmp("chlist", query)) {
		return ATTR_CHLIST;
	} else if (!strcmp("compression", query)) {
		return ATTR_COMPRESSION;
	} else if (!strcmp("lineOrder", query)) {
		return ATTR_LINEORDER;
	} else if (!strcmp("chromaticities", query)) {
		return ATTR_CHROMATICITIES;
	} else if (!strcmp("envmap", query)) {
		return ATTR_ENVMAP;
	} else if (!strcmp("string", query)) {
		return ATTR_STRING;
	} else if (!strcmp("box2d", query)) {
		return ATTR_BOX2D;
	} else if (!strcmp("box2f", query)) {
		return ATTR_BOX2F;
	} else if (!strcmp("box2i", query)) {
		return ATTR_BOX2I;
	} else if (!strcmp("v2d", query)) {
		return ATTR_V2D;
	} else if (!strcmp("v2f", query)) {
		return ATTR_V2F;
	} else if (!strcmp("v2i", query)) {
		return ATTR_V2I;
	} else if (!strcmp("vd", query)) {
		return ATTR_VD;
	} else if (!strcmp("vf", query)) {
		return ATTR_VF;
	} else if (!strcmp("vi", query)) {
		return ATTR_VI;
	} else if (!strcmp("double", query)) {
		return ATTR_DOUBLE;
	} else if (!strcmp("float", query)) {
		return ATTR_FLOAT;
	} else if (!strcmp("int", query)) {
		return ATTR_INT;
	} else {
		return ATTR_UNKNOWN;
	}
}

inline const char * attrTypeToString(ATTR_TYPE attrType) {

	switch (attrType) {
		case ATTR_CHLIST: { return std::string("chlist").c_str(); }
		case ATTR_COMPRESSION: { return std::string("compression").c_str(); }
		case ATTR_LINEORDER: { return std::string("lineOrder").c_str(); }
		case ATTR_CHROMATICITIES: { return std::string("chromaticities").c_str(); }
		case ATTR_ENVMAP: { return std::string("envmap").c_str(); }
		case ATTR_STRING: { return std::string("string").c_str(); }
		case ATTR_BOX2D: { return std::string("box2d").c_str(); }
		case ATTR_BOX2F: { return std::string("box2f").c_str(); }
		case ATTR_BOX2I: { return std::string("box2i").c_str(); }
		case ATTR_V2D: { return std::string("v2d").c_str(); }
		case ATTR_V2F: { return std::string("v2f").c_str(); }
		case ATTR_V2I: { return std::string("v2i").c_str(); }
		case ATTR_VD: { return std::string("vd").c_str(); }
		case ATTR_VF: { return std::string("vf").c_str(); }
		case ATTR_VI: { return std::string("vi").c_str(); }
		case ATTR_DOUBLE: { return std::string("double").c_str(); }
		case ATTR_FLOAT: { return std::string("float").c_str(); }
		case ATTR_INT: { return std::string("int").c_str(); }
		case ATTR_UNKNOWN: { return std::string("unknown").c_str(); }
	}
}

inline Compression stringToCompressionType(const char * query) {

	if (!strcmp("no", query)) {
		return NO_COMPRESSION;
	} else if (!strcmp("rle", query)) {
		return RLE_COMPRESSION;
	} else if (!strcmp("zips", query)) {
		return ZIPS_COMPRESSION;
	} else if (!strcmp("zip", query)) {
		return ZIP_COMPRESSION;
	} else if (!strcmp("piz", query)) {
		return PIZ_COMPRESSION;
	} else if (!strcmp("pxr24", query)) {
		return PXR24_COMPRESSION;
	} else if (!strcmp("b44", query)) {
		return B44_COMPRESSION;
	} else if (!strcmp("b44a", query)) {
		return B44A_COMPRESSION;
	}
}

inline const char * compressionTypeToString(Compression compressionType) {
	switch (compressionType) {
		case NO_COMPRESSION: { return std::string("no").c_str(); }
		case RLE_COMPRESSION: { return std::string("rle").c_str(); }
		case ZIPS_COMPRESSION: { return std::string("zips").c_str(); }
		case ZIP_COMPRESSION: { return std::string("zip").c_str(); }
		case PIZ_COMPRESSION: { return std::string("piz").c_str(); }
		case PXR24_COMPRESSION: { return std::string("pxr24").c_str(); }
		case B44_COMPRESSION: { return std::string("b44").c_str(); }
		case B44A_COMPRESSION: { return std::string("b44a").c_str(); }
		case NUM_COMPRESSION_METHODS: { return std::string("unknown").c_str(); }
	}
}

inline LineOrder stringToLineOrderType(const char * query) {

	if (!strcmp("increasing_y", query)) {
		return INCREASING_Y;
	} else if (!strcmp("decreasing_y", query)) {
		return DECREASING_Y;
	} else if (!strcmp("random_y", query)) {
		return RANDOM_Y;
	}
}

inline const char * lineOrderTypeToString(LineOrder lineOrderType) {
	switch (lineOrderType) {
		case INCREASING_Y: { return std::string("increasing_y").c_str(); }
		case DECREASING_Y: { return std::string("decreasing_y").c_str(); }
		case RANDOM_Y: { return std::string("random_y").c_str(); }
		case NUM_LINEORDERS: { return std::string("unknown").c_str(); }
	}
}

inline Envmap stringToEnvmapType(const char * query) {

	if (!strcmp("latlong", query)) {
		return ENVMAP_LATLONG;
	} else if (!strcmp("cube", query)) {
		return ENVMAP_CUBE;
	}
}

inline const char * envmapTypeToString(Envmap envmapType) {
	switch (envmapType) {
		case ENVMAP_LATLONG: { return std::string("latlong").c_str(); }
		case ENVMAP_CUBE: { return std::string("cube").c_str(); }
		case NUM_ENVMAPTYPES: { return std::string("unknown").c_str(); }
	}
}

/* TODO: Implement these using std::map types. */
/*
template<class K, class V>
struct map_list_of_type_sub {
	typedef std::map<K, V> Map;
	Map data;
	map_list_of_type_sub(K k, V v) { data[k] = v; }
	map_list_of_type_sub& operator()(K k, V v) { data[k] = v; return *this; }
	operator Map const&() const { return data; }
};

template<class K, class V>
map_list_of_type_sub<K, V> map_list_of(K k, V v) {
  return map_list_of_type_sub<K, V>(k, v);
}

const std::map<ATTR_TYPE, std::string> attrTypeToString = \
		map_list_of(ATTR_CHLIST, "chlist") (ATTR_COMPRESSION, "compression") \
		(ATTR_LINEORDER, "lineOrder") (ATTR_CHROMATICITIES, "chromaticities") \
		(ATTR_ENVMAP, "envmap") (ATTR_STRING, "string") \
		(ATTR_BOX2D, "box2d") (ATTR_BOX2F, "box2f") (ATTR_BOX2I, "box2i") \
		(ATTR_V2D, "v2d") (ATTR_V2F, "v2f") (ATTR_V2I, "v2i") \
		(ATTR_VD, "vd") (ATTR_VF, "vf") (ATTR_VI, "vi") \
		(ATTR_DOUBLE, "double") (ATTR_FLOAT, "float") (ATTR_INT, "int");

const std::map<std::string, ATTR_TYPE> stringToAttrType = \
		map_list_of("chlist", ATTR_CHLIST) ("compression", ATTR_COMPRESSION) \
		("lineOrder", ATTR_LINEORDER) ("chromaticities", ATTR_CHROMATICITIES) \
		("envmap", ATTR_ENVMAP) ("string", ATTR_STRING) \
		("box2d", ATTR_BOX2D) ("box2f", ATTR_BOX2F) ("box2i", ATTR_BOX2I) \
		("v2d", ATTR_V2D) ("v2f", ATTR_V2F) ("v2i", ATTR_V2I) \
		("vd", ATTR_VD) ("vf", ATTR_VF) ("vi", ATTR_VI) \
		("double", ATTR_DOUBLE) ("float", ATTR_FLOAT) (ATTR_INT, "int");
*/

#endif /* OPENEXR_MEX_H_ */
