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
#include "ImfExtraAttributes.h"
#include "ImfPixelType.h"
#include "Iex.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdarg.h>

#include "mex.h"
#include "matrix.h"

using namespace Imf;
using namespace Imath;
using namespace Iex;

// Inspired by mitsuba-0.4.1
// TODO: Add mitsuba copyright.
#ifdef NDEBUG
#define Assert(cond) ((void) 0)
#define AssertEx(cond, explanation) ((void) 0)
#else
/* Assertions */
// Assert that a condition is true
#define Assert(cond) do { \
		if (!(cond)) fprintf(stderr, "Assertion \"%s\" failed in %s:%i\n", \
		#cond, __FILE__, __LINE__); \
	} while (0)

// Assertion with a customizable error explanation
#define AssertEx(cond, explanation) do { \
		if (!(cond)) fprintf(stderr, "Assertion \"%s\" failed in %s:%i (" explanation ")\n", \
		#cond, __FILE__, __LINE__); \
	} while (0)
#endif

namespace exr {

#define USED float
#define USEDC FLOAT

/*
 * C++-ize, as also done in nuancefx_mex.
 */
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

inline ATTR_TYPE attrNameToAttrType(const char * query) {
	if (!strcasecmp(query, "gain")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "wavelength")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "extTube")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "lens")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "material")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "chromaticities")) {
		return ATTR_CHROMATICITIES;
	} else if (!strcasecmp(query, "whiteLuminance")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "adoptedNeutral")) {
		return ATTR_V2F;
	} else if (!strcasecmp(query, "renderingTransform")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "lookModTransform")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "xDensity")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "owner")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "comments")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "capDate")) {
		return ATTR_STRING;
	} else if (!strcasecmp(query, "utcOffset")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "longitude")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "latitude")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "altitude")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "focus")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "expTime")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "aperture")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "isoSpeed")) {
		return ATTR_FLOAT;
	} else if (!strcasecmp(query, "multExpTimes")) {
		return ATTR_VF;
	} else if (!strcasecmp(query, "multApertures")) {
		return ATTR_VF;
	} else if (!strcasecmp(query, "multIsoSpeeds")) {
		return ATTR_VF;
	} else if (!strcasecmp(query, "multGains")) {
		return ATTR_VF;
	} else if (!strcasecmp(query, "envmap")) {
		return ATTR_ENVMAP;
	} else {
		return ATTR_TYPE_INVALID;
	}
}

inline ATTR_TYPE stringToAttrType(const char * propertyName) {
	if (!strcasecmp("chlist", propertyName)) {
		return ATTR_CHLIST;
	} else if (!strcasecmp("compression", propertyName)) {
		return ATTR_COMPRESSION;
	} else if (!strcasecmp("lineOrder", propertyName)) {
		return ATTR_LINEORDER;
	} else if (!strcasecmp("chromaticities", propertyName)) {
		return ATTR_CHROMATICITIES;
	} else if (!strcasecmp("envmap", propertyName)) {
		return ATTR_ENVMAP;
	} else if (!strcasecmp("string", propertyName)) {
		return ATTR_STRING;
	} else if (!strcasecmp("box2d", propertyName)) {
		return ATTR_BOX2D;
	} else if (!strcasecmp("box2f", propertyName)) {
		return ATTR_BOX2F;
	} else if (!strcasecmp("box2i", propertyName)) {
		return ATTR_BOX2I;
	} else if (!strcasecmp("v2d", propertyName)) {
		return ATTR_V2D;
	} else if (!strcasecmp("v2f", propertyName)) {
		return ATTR_V2F;
	} else if (!strcasecmp("v2i", propertyName)) {
		return ATTR_V2I;
	} else if (!strcasecmp("vd", propertyName)) {
		return ATTR_VD;
	} else if (!strcasecmp("vf", propertyName)) {
		return ATTR_VF;
	} else if (!strcasecmp("vi", propertyName)) {
		return ATTR_VI;
	} else if (!strcasecmp("double", propertyName)) {
		return ATTR_DOUBLE;
	} else if (!strcasecmp("float", propertyName)) {
		return ATTR_FLOAT;
	} else if (!strcasecmp("int", propertyName)) {
		return ATTR_INT;
	} else {
		return ATTR_TYPE_INVALID;
	}
}

inline void attrTypeToString(const ATTR_TYPE property, \
							char *propertyName) {

	switch (property) {
		case ATTR_CHLIST: { strcpy(propertyName, "chlist"); return; }
		case ATTR_COMPRESSION: { strcpy(propertyName, "compression"); return; }
		case ATTR_LINEORDER: { strcpy(propertyName, "lineOrder"); return; }
		case ATTR_CHROMATICITIES: { strcpy(propertyName, "chromaticities"); return; }
		case ATTR_ENVMAP: { strcpy(propertyName, "envmap"); return; }
		case ATTR_STRING: { strcpy(propertyName, "string"); return; }
		case ATTR_BOX2D: { strcpy(propertyName, "box2d"); return; }
		case ATTR_BOX2F: { strcpy(propertyName, "box2f"); return; }
		case ATTR_BOX2I: { strcpy(propertyName, "box2i"); return; }
		case ATTR_V2D: { strcpy(propertyName, "v2d"); return; }
		case ATTR_V2F: { strcpy(propertyName, "v2f"); return; }
		case ATTR_V2I: { strcpy(propertyName, "v2i"); return; }
		case ATTR_VD: { strcpy(propertyName, "vd"); return; }
		case ATTR_VF: { strcpy(propertyName, "vf"); return; }
		case ATTR_VI: { strcpy(propertyName, "vi"); return; }
		case ATTR_DOUBLE: { strcpy(propertyName, "double"); return; }
		case ATTR_FLOAT: { strcpy(propertyName, "float"); return; }
		case ATTR_INT: { strcpy(propertyName, "int"); return; }
		default: { strcpy(propertyName, "unknown"); return; }
	}
}

inline Compression stringToCompressionType(const char * propertyName) {

	if (!strcmp("no", propertyName)) {
		return NO_COMPRESSION;
	} else if (!strcmp("rle", propertyName)) {
		return RLE_COMPRESSION;
	} else if (!strcmp("zips", propertyName)) {
		return ZIPS_COMPRESSION;
	} else if (!strcmp("zip", propertyName)) {
		return ZIP_COMPRESSION;
	} else if (!strcmp("piz", propertyName)) {
		return PIZ_COMPRESSION;
	} else if (!strcmp("pxr24", propertyName)) {
		return PXR24_COMPRESSION;
	} else if (!strcmp("b44", propertyName)) {
		return B44_COMPRESSION;
	} else if (!strcmp("b44a", propertyName)) {
		return B44A_COMPRESSION;
	} else {
		Assert(0);
	}
}

inline void compressionTypeToString(const Compression property, \
									char * propertyName) {
	switch (property) {
		case NO_COMPRESSION: { strcpy(propertyName, "no"); return; }
		case RLE_COMPRESSION: { strcpy(propertyName, "rle"); return; }
		case ZIPS_COMPRESSION: { strcpy(propertyName, "zips"); return; }
		case ZIP_COMPRESSION: { strcpy(propertyName, "zip"); return; }
		case PIZ_COMPRESSION: { strcpy(propertyName, "piz"); return; }
		case PXR24_COMPRESSION: { strcpy(propertyName, "pxr24"); return; }
		case B44_COMPRESSION: { strcpy(propertyName, "b44"); return; }
		case B44A_COMPRESSION: { strcpy(propertyName, "b44a"); return; }
		case NUM_COMPRESSION_METHODS:
		default: { strcpy(propertyName, "unknown"); return; }
	}
}

inline LineOrder stringToLineOrderType(const char * propertyName) {

	if (!strcmp("increasing_y", propertyName)) {
		return INCREASING_Y;
	} else if (!strcmp("decreasing_y", propertyName)) {
		return DECREASING_Y;
	} else if (!strcmp("random_y", propertyName)) {
		return RANDOM_Y;
	} else {
		Assert(0);
	}
}

inline void lineOrderTypeToString(const LineOrder property, \
								char * propertyName) {
	switch (property) {
		case INCREASING_Y: { strcpy(propertyName, "increasing_y"); return; }
		case DECREASING_Y: { strcpy(propertyName, "decreasing_y"); return; }
		case RANDOM_Y: { strcpy(propertyName, "random_y"); return; }
		case NUM_LINEORDERS:
		default: { strcpy(propertyName, "unknown"); return; }
	}
}

inline Envmap stringToEnvmapType(const char * query) {

	if (!strcmp("latlong", query)) {
		return ENVMAP_LATLONG;
	} else if (!strcmp("cube", query)) {
		return ENVMAP_CUBE;
	} else {
		Assert(0);
	}
}

inline void envmapTypeToString(const Envmap property, \
							char * propertyName) {
	switch (property) {
		case ENVMAP_LATLONG: { strcpy(propertyName, "latlong"); return; }
		case ENVMAP_CUBE: { strcpy(propertyName, "cube"); return; }
		case NUM_ENVMAPTYPES:
		default: { strcpy(propertyName, "unknown"); return; }
	}
}

const mxArray * attributeToMxArray(const Attribute & attr);
mxArray* getAttribute(const Header& head, const char attributeName[]);
mxArray* getAttribute(const Header& head);
void setAttribute(Header& head, const char attrName[], const mxArray* mxarr);
void setAttribute(Header& head, const mxArray* mxstruct);

const Header createHeader(size_t width, size_t height, mxArray* mxstruct);
const Header createHeader(size_t width, size_t height);

void writeScanLine(OutputFile& file, \
		const USED *rPixels, \
		const USED *gPixels, \
		const USED *bPixels, \
		const USED *aPixels, \
		const size_t width, \
		const size_t height);

void writeScanLine(OutputFile& file, \
		const USED *yPixels, \
		const USED *aPixels, \
		const size_t width, \
		const size_t height);

void writeScanLine(OutputFile& file, \
		const USED **cPixels, \
		const char **cNames, \
		const size_t width, \
		const size_t height);

void readScanLine(InputFile& file, \
		Array2D<USED> &rPixels, bool &rFlag, \
		Array2D<USED> &gPixels, bool &gFlag, \
		Array2D<USED> &bPixels, bool &bFlag, \
		Array2D<USED> &aPixels, bool &aFlag, \
		size_t& width, size_t& height);

void readScanLine(InputFile& file, \
		Array2D<USED> &yPixels, bool &yFlag, \
		Array2D<USED> &aPixels, bool &aFlag, \
		size_t& width, size_t& height);

void readScanLine(InputFile& file, \
		std::vector<Array2D<USED> >& cPixels,
		std::vector<char *>& cNames, \
		size_t& width, size_t& height);

}	/* namespace exr */

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
