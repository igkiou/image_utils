/*
 * exrget.cpp
 *
 *  Created on: May 22, 2012
 *      Author: igkiou
 */

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

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	const int lengthFileName = mxGetNumberOfElements(prhs[0]) + 1;
	char* fileName = (char *) mxMalloc(lengthFileName * sizeof(char));
	mxGetString(prhs[0], fileName, lengthFileName);
	InputFile in(fileName);
	Header head = in.header();

	if (nrhs >= 2) {
		if (!mxIsChar(prhs[1])) {
			mexErrMsgTxt("Second argument must be a string.");
		}
		const int lengthAttributeName = mxGetNumberOfElements(prhs[1]) + 1;
		char* attributeName = (char *) mxMalloc(lengthAttributeName * sizeof(char));
		mxGetString(prhs[1], attributeName, lengthAttributeName);
		plhs[0] = getSingleAttribute(head, attributeName);
		mxFree(attributeName);
	} else {
		plhs[0] = getAllAttributes(head);
	}

	mxFree(fileName);
}
