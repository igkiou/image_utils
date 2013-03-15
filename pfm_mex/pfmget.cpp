/*
 * pfmget.cpp
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

	mex::MxString fileName(const_cast<mxArray*>(prhs[0]));
	pfm::PFMInputFile file(fileName);
	file.readHeader();
	plhs[0] = file.get_header().toMxArray().get_array();
}
