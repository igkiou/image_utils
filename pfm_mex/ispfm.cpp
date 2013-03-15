/*
 * ispfm.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"
#include "pfm_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 1) {
		mexErrMsgTxt("Exactly one input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	plhs[0] = pfm::isPfmFile(
					mex::MxString(const_cast<mxArray*>(prhs[0]))).get_array();
}
