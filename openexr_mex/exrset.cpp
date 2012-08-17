/*
 * exrset.cpp
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs < 3) {
		mexErrMsgTxt("At least three input arguments are required.");
	} else if (nrhs > 4) {
		mexErrMsgTxt("Four or fewer input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	const int lengthInFileName = mxGetNumberOfElements(prhs[0]) + 1;
	char* inFileName = (char *) mxMalloc(lengthInFileName * sizeof(char));
	mxGetString(prhs[0], inFileName, lengthInFileName);

	char *outFileName;
	if (!mxIsEmpty(prhs[1])) {
		if (!mxIsChar(prhs[1])) {
			mexErrMsgTxt("Second argument must be a string.");
		}
		const int lengthOutFileName = mxGetNumberOfElements(prhs[1]) + 1;
		outFileName = (char *) mxMalloc(lengthOutFileName * sizeof(char));
		mxGetString(prhs[1], outFileName, lengthOutFileName);
	} else {
		outFileName = inFileName;
	}

	InputFile in(inFileName);
	Header head = in.header();

	if (nrhs == 4) {
		if (!mxIsChar(prhs[2])) {
			mexErrMsgTxt("Third argument must be a string.");
		}
		const int lengthAttributeName = mxGetNumberOfElements(prhs[2]) + 1;
		char* attributeName = (char *) mxMalloc(lengthAttributeName * sizeof(char));
		mxGetString(prhs[2], attributeName, lengthAttributeName);
		setSingleAttribute(head, attributeName, prhs[3]);
		mxFree(attributeName);
	} else {
		setMultipleAttributes(head, prhs[2]);
	}
	OutputFile out(outFileName, head);
	out.copyPixels(in);
	mxFree(inFileName);
	if (!mxIsEmpty(prhs[1])) {
		mxFree(outFileName);
	}
}
