/*
 * exrwrite_sub.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: igkiou
 */

#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs > 5) {
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

	const int numDims = mxGetNumberOfDimensions(prhs[0]);
	const mwSize *dims = mxGetDimensions(prhs[0]);
	const int width = dims[0];
	const int height = dims[1];
	exr::EXROutputFile outFile(width, height);

	if (nrhs >= 4) {
		outFile.setAttribute(prhs[3]);
	}

	if (numDims == 2) {
		/* Luminance image. */
		outFile.addChannelYA();

		const float *y = (float *) mxGetData(prhs[0]);
		/* Second argument must be alpha. */
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
		outFile.writeChannelYA(y, a);

	} else if ((numDims == 3) && (dims[2] == 3)) {
		/* RGB image. */
		outFile.addChannelRGBA();

		const float *r = (float *) mxGetData(prhs[0]);
		const float *g = &r[width * height];
		const float *b = &r[2 * width * height];

		/* Second argument must be alpha. */
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
		outFile.writeChannelRGBA(r, g, b, a);
	} else if (numDims == 3) {
		/* Multi-channel image. */

		int numChannels = dims[2];
		/* Second argument must be channel names. */
		std::vector<const std::string> cNames;
		if (mxIsEmpty(prhs[1])) {
			for (int iterChannel = 0; iterChannel < numChannels; ++iterChannel) {
				std::ostringstream temp;
				temp << iterChannel;
				cNames.push_back(temp.str());
			}
		} else {
			if (mxGetNumberOfElements(prhs[1]) != numChannels) {
				mexErrMsgTxt("Second argument must be of same length as multi-dimensional image.");
			}
			for (int iterChannel = 0; iterChannel < numChannels; ++iterChannel) {
				mxArray *temp = (mxArray *) mxGetCell(prhs[1], iterChannel);
				if (!mxIsChar(temp)) {
					mexErrMsgTxt("All of the contents of second argument must be strings.");
				}
				char *tempName = mxArrayToString(temp);
				cNames.push_back(std::string(tempName));
				mxFree(tempName);
			}
		}
		outFile.addChannel(cNames);

		std::vector<const float *> ccPixels;
		const float *c = (float *) mxGetData(prhs[0]);
		for (int iterChannel = 0; iterChannel < numChannels; ++iterChannel) {
			ccPixels.push_back(&c[iterChannel * width * height]);
		}
		outFile.writeChannel(ccPixels, cNames);

	} else {
		mexErrMsgTxt("First argument must be two- or three-dimensional.");
	}


	if (!mxIsChar(prhs[2])) {
		mexErrMsgTxt("Third argument must be a string.");
	}
	char* fileName = mxArrayToString(prhs[2]);
	outFile.writeFile(std::string(fileName));
	mxFree(fileName);

}
