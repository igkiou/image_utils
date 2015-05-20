/*
 * image_utils//image_utils/raw_mex/rawget.cpp/rawget.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: igkiou
 */

#include "mex_utils.h"

#include "../raw/raw.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs < 1) {
		mexErrMsgTxt("At least one input argument is required.");
	} else if (nrhs > 2) {
		mexErrMsgTxt("Two or fewer input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 2) {
		mexErrMsgTxt("Too many output arguments.");
	}

	raw::RawInputFile file(mex::MxString(const_cast<mxArray*>(prhs[0])));
	if (nrhs >= 2) {
		mex::MxString attributeName(const_cast<mxArray*>(prhs[1]));
		plhs[0] = file.getAttribute(attributeName).get_array();
	} else {
		plhs[0] = file.getAttribute().get_array();
	}
	if (nlhs >= 2) {
		plhs[1] = file.getCFAInformation().get_array();
	}
}
