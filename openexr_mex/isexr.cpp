/*
 * image_utils//image_utils/openexr_mex/isexr.cpp/isexr.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"
#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 1) {
		mexErrMsgTxt("Exactly one input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	openexr::ExrInputFile file(mex::MxString(const_cast<mxArray*>(prhs[0])));
	plhs[0] = file.isValidFile().get_array();
}
