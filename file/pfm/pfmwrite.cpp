/*
 * pfmwrite.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"

#include "pfm.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	(void) plhs;
	/* Check number of input arguments */
	if (nrhs > 3) {
		mexErrMsgTxt("Three or fewer input arguments are required.");
	} else if (nrhs < 2) {
		mexErrMsgTxt("At least two input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	const mex::MxArray image(const_cast<mxArray*>(prhs[0]));
	std::vector<int> dimensions = image.getDimensions();
	mexAssert((dimensions.size() == 2) || (dimensions.size() == 3));
	mex::MxString fileName(mex::MxString(const_cast<mxArray*>(prhs[1])));
	pfm::PfmOutputFile file(fileName, dimensions[1], dimensions[0]);

	if (nrhs >= 3) {
		mex::MxStruct attributes(const_cast<mxArray*>(prhs[2]));
		file.setAttribute(attributes);
	}

	file.writeData(image);
}
