/*
 * image_utils//image_utils/raw_mex/rawread.cpp/rawread.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: igkiou
 */

#include "mex_utils.h"
#include "raw_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs > 3) {
		mexErrMsgTxt("Three or fewer arguments are required.");
	} else if (nrhs < 1) {
		mexErrMsgTxt("At least one input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 3) {
		mexErrMsgTxt("Too many output arguments.");
	}

	raw::RawInputFile file(mex::MxString(const_cast<mxArray*>(prhs[0])));
	if ((nrhs == 2) && (!mex::MxArray(const_cast<mxArray*>(prhs[1])).isEmpty())) {
		plhs[0] = file.readData(mex::MxNumeric<bool>(const_cast<mxArray*>(prhs[1]))).get_array();
	} else if (nrhs == 3) {
		if ((!mex::MxArray(const_cast<mxArray*>(prhs[1])).isEmpty()) &&
				(!mex::MxArray(const_cast<mxArray*>(prhs[2])).isEmpty())) {
			plhs[0] = file.readData(mex::MxNumeric<bool>(const_cast<mxArray*>(prhs[1])),
									mex::MxString(const_cast<mxArray*>(prhs[2]))).get_array();
		} else if ((!mex::MxArray(const_cast<mxArray*>(prhs[1])).isEmpty()) &&
					(mex::MxArray(const_cast<mxArray*>(prhs[2])).isEmpty())) {
			plhs[0] = file.readData(mex::MxNumeric<bool>(const_cast<mxArray*>(prhs[1]))).get_array();
		} else if ((mex::MxArray(const_cast<mxArray*>(prhs[1])).isEmpty()) &&
				(!mex::MxArray(const_cast<mxArray*>(prhs[2])).isEmpty())) {
			plhs[0] = file.readData(mex::MxString(const_cast<mxArray*>(prhs[2]))).get_array();
		} else {
			plhs[0] = file.readData().get_array();
		}
	} else {
		plhs[0] = file.readData().get_array();
	}
	if (nlhs >= 2) {
		plhs[1] = file.getAttribute().get_array();
	}
	if (nlhs >= 3) {
		plhs[2] = file.getCFAInformation().get_array();
	}
}
