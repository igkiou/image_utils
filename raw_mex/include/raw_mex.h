/*
 * raw_mex.h
 *
 *  Created on: Jan 7, 2014
 *      Author: igkiou
 */

#ifndef RAW_MEX_H_
#define RAW_MEX_H_


#include <math.h>
#include <string.h>

#include "libraw.h"

#include "mex.h"
#include "matrix.h"

namespace raw {

enum {raw_MAX_STRING_LENGTH=128};

/*
 * Replace all casts to static_cast.
 */
const char * const ERROR_ID = "MATLAB:raw_mex";

inline void handleErrorCode(const EdsError errorCode) {

	switch(errorCode) {
		case EDS_ERR_OK:
		{
			return;
		}
		default:
		{
			mexErrMsgIdAndTxt(ERROR_ID, "Error %X.", errorCode);
			return;
		}
	}
}

mxArray* getRawProperty(const EdsCameraRef handle, \
						const RAW_PROPERTY property);

mxArray*

/*
 * TODO: Separate out name handling in case of multiple variants of
 * getters/setters (maybe move in mex?).
 */

/* Camera getters and setters. */
mxArray* getCameraProperty(const EdsCameraRef handle, \
						const RAW_PROPERTY property);

mxArray* getCameraProperty(const EdsCameraRef handle);

void setCameraProperty(const EdsCameraRef handle, \
					const RAW_PROPERTY property, \
					const mxArray* mxarr);

void setCameraProperty(const EdsCameraRef handle, const mxArray* mxstruct);

mxArray* getCameraPropertyInfo(const EdsCameraRef handle, \
						const RAW_PROPERTY property);

int getNumberOfImages(const EdsCameraRef handle);



}	/* namespace raw */

#endif /* RAW_MEX_H_ */
