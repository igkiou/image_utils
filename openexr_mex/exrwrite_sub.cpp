/*
 * writeexr.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: igkiou
 */

#include "ImathBox.h"
#include "ImfRgba.h"
#include "ImfRgbaFile.h"
#include "ImfInputFile.h"
#include "ImfOutputFile.h"
#include "ImfHeader.h"
#include "ImfArray.h"
#include "ImfChannelList.h"
#include "ImfPixelType.h"
#include "ImfStandardAttributes.h"
#include "Iex.h"

#include "mex.h"
#include "matrix.h"

using namespace Imf;
using namespace Imath;
using namespace Iex;

#define USED float
#define USEDC FLOAT

void writeScanLineRGB(const char fileName[], \
		const USED *rPixels, \
		const USED *gPixels, \
		const USED *bPixels, \
		const USED *aPixels, \
		int &width,
		int &height) {
	Header header(width, height);
    Chromaticities c1(V2f(0.6400, 0.3300), V2f(0.3000, 0.6000), V2f(0.1500, 0.0600), V2f(0.3127, 0.3290));
    addChromaticities(header, c1);

	header.channels().insert("R", Channel(USEDC));
	header.channels().insert("G", Channel(USEDC));
	header.channels().insert("B", Channel(USEDC));
	header.channels().insert("A", Channel(USEDC));

	OutputFile file(fileName, header);
	FrameBuffer frameBuffer;
	frameBuffer.insert("R", Slice(USEDC, (char *) rPixels, \
			sizeof(*rPixels) * 1, sizeof(*rPixels) * width));
	frameBuffer.insert("G", Slice(USEDC, (char *) gPixels, \
			sizeof(*gPixels) * 1, sizeof(*gPixels) * width));
	frameBuffer.insert("B", Slice(USEDC, (char *) bPixels, \
			sizeof(*bPixels) * 1, sizeof(*bPixels) * width));
	if (aPixels != NULL) {
		frameBuffer.insert("A", Slice(USEDC, (char *) aPixels, \
			sizeof(*aPixels) * 1, sizeof(*aPixels) * width));
	}
	file.setFrameBuffer(frameBuffer);
	file.writePixels(height);
}

void writeScanLineXYZ(const char fileName[], \
		const USED *rPixels, \
		const USED *gPixels, \
		const USED *bPixels, \
		const USED *aPixels, \
		int &width,
		int &height) {
	Header header(width, height);
    Chromaticities c1(V2f(1.0, 0.0), V2f(0.0, 0.1), V2f(0.0, 0.0), V2f(1.0 / 3.0, 1.0 / 3.0));
    addChromaticities(header, c1);

	header.channels().insert("R", Channel(USEDC));
	header.channels().insert("G", Channel(USEDC));
	header.channels().insert("B", Channel(USEDC));
	header.channels().insert("A", Channel(USEDC));

	OutputFile file(fileName, header);
	FrameBuffer frameBuffer;
	frameBuffer.insert("R", Slice(USEDC, (char *) rPixels, \
			sizeof(*rPixels) * 1, sizeof(*rPixels) * width));
	frameBuffer.insert("G", Slice(USEDC, (char *) gPixels, \
			sizeof(*gPixels) * 1, sizeof(*gPixels) * width));
	frameBuffer.insert("B", Slice(USEDC, (char *) bPixels, \
			sizeof(*bPixels) * 1, sizeof(*bPixels) * width));
	if (aPixels != NULL) {
		frameBuffer.insert("A", Slice(USEDC, (char *) aPixels, \
			sizeof(*aPixels) * 1, sizeof(*aPixels) * width));
	}
	file.setFrameBuffer(frameBuffer);
	file.writePixels(height);
}

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
	int width, height;
	const int numDims = mxGetNumberOfDimensions(prhs[0]);
	if (numDims == 2) {
		width = mxGetN(prhs[0]);
		height = mxGetM(prhs[0]);
		r = (float *) mxGetData(prhs[0]);
		g = r; b = r;
	} else if (numDims == 3) {
		const int *dims = mxGetDimensions(prhs[0]);
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
	int length = mxGetNumberOfElements(prhs[2]) + 1;
	char fileName[length];
	mxGetString(prhs[2], fileName, length);

	char writeFormat;
	if (nrhs >= 4) {
		if ((!mxIsChar(prhs[3])) || (mxGetNumberOfElements(prhs[3]) != 1)) {
			mexErrMsgTxt("Fourth argument must be a single character.");
		}
		writeFormat = (char)*(char*) mxGetData(prhs[3]);
	} else {
		writeFormat = 'r';
	}

	if ((writeFormat == 'r') || (writeFormat == 'R')) {
		writeScanLineRGB(fileName, r, g, b, a, width, height);
	} else if ((writeFormat == 'x') || (writeFormat == 'X')) {
		writeScanLineXYZ(fileName, r, g, b, a, width, height);
	} else {
		mexErrMsgTxt("Unknown option for fourth argument.");
	}
}
