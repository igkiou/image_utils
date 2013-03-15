/*
 * pfmwrite.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"
#include "pfm_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 2) {
		mexErrMsgTxt("Exactly two input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	mex::MxNumeric<float> image(const_cast<mxArray*>(prhs[0]));
	pfm::PFMOutputFile file(mex::MxString(const_cast<mxArray*>(prhs[1])));
	file.writeHeader(image);
	file.writeFile(image);
}
