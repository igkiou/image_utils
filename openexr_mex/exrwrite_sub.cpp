/*
 * exrwrite_sub.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: igkiou
 */

#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs > 4) {
		mexErrMsgTxt("Four or fewer input arguments are required.");
	} else if (nrhs < 3) {
		mexErrMsgTxt("At least three input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsSingle(prhs[0])) {
		mexErrMsgTxt("First argument must be of type single.");
	}
	float *r, *g, *b;
	size_t width, height;
	const int numDims = mxGetNumberOfDimensions(prhs[0]);
	if (numDims == 2) {
		width = mxGetN(prhs[0]);
		height = mxGetM(prhs[0]);
		r = (float *) mxGetData(prhs[0]);
		g = r; b = r;
	} else if (numDims == 3) {
		const mwSize *dims = mxGetDimensions(prhs[0]);
		width = dims[0];
		height = dims[1];
		r = (float *) mxGetData(prhs[0]);
		g = &r[width * height];
		b = &r[2 * width * height];
	} else {
		mexErrMsgTxt("First argument must be two- or three-dimensional.");
	}

	if (!mxIsEmpty(prhs[1])) {
		if (!mxIsSingle(prhs[1])) {
			mexErrMsgTxt("Second argument must be of type single.");
		} else if (mxGetNumberOfDimensions(prhs[1]) != 2) {
			mexErrMsgTxt("Second argument must be two-dimensional.");
		} else if (mxGetM(prhs[1]) != width) {
			mexErrMsgTxt("Width of second argument does not match width of first argument.");
		} else if (mxGetN(prhs[1]) != height) {
			mexErrMsgTxt("Height of second argument does not match height of first argument.");
		}
	}
	const float *a = (float *) mxGetData(prhs[1]);

	if (!mxIsChar(prhs[2])) {
		mexErrMsgTxt("Third argument must be a string.");
	}
	const int lengthFileName = mxGetNumberOfElements(prhs[2]) + 1;
	char* fileName = (char *) mxMalloc(lengthFileName * sizeof(char));
	mxGetString(prhs[2], fileName, lengthFileName);

	Header head(width, height);
	head.channels().insert("R", Channel(USEDC));
	head.channels().insert("G", Channel(USEDC));
	head.channels().insert("B", Channel(USEDC));
	head.channels().insert("A", Channel(USEDC));

	if (nrhs >= 4) {
		setMultipleAttributes(head, prhs[3]);
	}

	OutputFile file(fileName, head);
	writeScanLine(file, r, g, b, a, width, height);

	mxFree(fileName);
}
