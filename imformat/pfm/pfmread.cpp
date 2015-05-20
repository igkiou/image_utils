/*
 * pfmread.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"

#include "pfm.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 1) {
		mexErrMsgTxt("Exactly one input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 2) {
		mexErrMsgTxt("Too many output arguments.");
	}

	pfm::PfmInputFile file(mex::MxString(const_cast<mxArray*>(prhs[0])));
	plhs[0] = file.readData().get_array();
	if (nlhs >= 2) {
		plhs[1] = file.getAttribute().get_array();
	}
}
