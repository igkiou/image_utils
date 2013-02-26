/*
 * exrget.cpp
 *
 *  Created on: May 22, 2012
 *      Author: igkiou
 */

#include "mex_utils.h"
#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs < 1) {
		mexErrMsgTxt("At least one input argument is required.");
	} else if (nrhs > 2) {
		mexErrMsgTxt("Two or fewer input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	mex::MxString fileName(prhs[0]);
	exr::EXRInputFile file(fileName);
	if (nlhs >= 2) {
		mex::MxString attributeName(prhs[1]);
		mex::MxArray* temp = file.getAttribute(attributeName);
		plhs[0] = temp->get_array();
		delete temp;
	} else {
		mex::MxStruct* temp = file.getAttribute();
		plhs[0] = temp->get_array();
		delete temp;
	}
}
