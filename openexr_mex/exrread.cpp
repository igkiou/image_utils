/*
 * exrread.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: igkiou
 */

#include "openexr_mex.h"

// TODO: Handle case of missing or differently named channels.
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 1) {
		mexErrMsgTxt("One input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 2) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	Array2D<USED> rPixels;
	Array2D<USED> gPixels;
	Array2D<USED> bPixels;
	Array2D<USED> aPixels;
	bool rFlag, gFlag, bFlag, aFlag;
	size_t width, height;
	const int lengthFileName = mxGetNumberOfElements(prhs[0]) + 1;
	char* fileName = (char *) mxMalloc(lengthFileName * sizeof(char));
	mxGetString(prhs[0], fileName, lengthFileName);
	InputFile file(fileName);
	exr::readScanLine(file, rPixels, rFlag, gPixels, gFlag, bPixels, bFlag, \
			aPixels, aFlag, width, height);

	const mwSize dims[3] = {height, width, 3};
	plhs[0] = mxCreateNumericArray(3, dims, mxSINGLE_CLASS, mxREAL); /* x */
	float *r = (float *) mxGetData(plhs[0]);
	float *g = &r[width * height];
	float *b = &r[2 * width * height];
	float *a = NULL;
	if (nlhs >= 2) {
		if (aFlag == true) {
			plhs[1] = mxCreateNumericMatrix(width, height, mxSINGLE_CLASS, mxREAL); /* x */
			a = (float *) mxGetData(plhs[1]);
		} else {
			plhs[1] = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL); /* x */
			a = (float *) mxGetData(plhs[1]);
			a[0] = 0;
		}
	}

	for (size_t iter1 = 0; iter1 < width; ++iter1) {
		for (size_t iter2 = 0; iter2 < height; ++iter2) {
			if (rFlag == true) {
				r[height * iter1 + iter2] = static_cast<float>(rPixels[iter2][iter1]);
			}
			if (gFlag == true) {
				g[height * iter1 + iter2] = static_cast<float>(gPixels[iter2][iter1]);
			}
			if (bFlag == true) {
				b[height * iter1 + iter2] = static_cast<float>(bPixels[iter2][iter1]);
			}
			if ((nlhs >= 2) && (aFlag == true)) {
				a[height * iter1 + iter2] = static_cast<float>(aPixels[iter2][iter1]);
			}
		}
	}
	mxFree(fileName);
}
