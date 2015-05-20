/*
 * image_utils//image_utils/raw_mex/israw.cpp/israw.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: igkiou
 */

#include "mex_utils.h"

#include "../raw/raw.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 1) {
		mexErrMsgTxt("Exactly one input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	plhs[0] = raw::isRawFile(
					mex::MxString(const_cast<mxArray*>(prhs[0]))).get_array();
}
