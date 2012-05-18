#include "ImathBox.h"
#include "ImfRgba.h"
#include "ImfRgbaFile.h"
#include "ImfInputFile.h"
#include "ImfHeader.h"
#include "ImfArray.h"
#include "ImfChannelList.h"
#include "ImfPixelType.h"
#include "Iex.h"

#include "mex.h"
#include "matrix.h"

using namespace Imf;
using namespace Imath;
using namespace Iex;

#define USED float
#define USEDC FLOAT

void readScanLine(const char fileName[], \
		Array2D<USED> &rPixels, bool &rFlag, \
		Array2D<USED> &gPixels, bool &gFlag, \
		Array2D<USED> &bPixels, bool &bFlag, \
		Array2D<USED> &aPixels, bool &aFlag, \
		int &width,
		int &height) {
	InputFile in(fileName);

	Box2i dw = in.header().dataWindow();
	width = dw.max.x - dw.min.x + 1;
	height = dw.max.y - dw.min.y + 1;

	FrameBuffer frameBuffer;

	if (in.header().channels().findChannel("R")) {
		rFlag = true;
		rPixels.resizeErase(height, width);
		frameBuffer.insert("R", Slice(USEDC, (char *) (&rPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		rFlag = false;
	}

	if (in.header().channels().findChannel("G")) {
		gFlag = true;
		gPixels.resizeErase(height, width);
		frameBuffer.insert("G", Slice(USEDC, (char *) (&gPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		gFlag = false;
	}

	if (in.header().channels().findChannel("B")) {
		bFlag = true;
		bPixels.resizeErase(height, width);
		frameBuffer.insert("B", Slice(USEDC, (char *) (&bPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		bFlag = false;
	}

	if (in.header().channels().findChannel("A")) {
		aFlag = true;
		aPixels.resizeErase(height, width);
		frameBuffer.insert("A", Slice(USEDC, (char *) (&aPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		aFlag = false;
	}

	in.setFrameBuffer(frameBuffer);
	in.readPixels(dw.min.y, dw.max.y);
}

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
	int width;
	int height;
	int length = mxGetNumberOfElements(prhs[0]) + 1;
	char fileName[length];
	mxGetString(prhs[0], fileName, length);
	readScanLine(fileName, rPixels, rFlag, gPixels, gFlag, bPixels, bFlag, \
			aPixels, aFlag, width, height);

	const int dims[3] = {height, width, 3};
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

	for (int iter1 = 0; iter1 < width; ++iter1) {
		for (int iter2 = 0; iter2 < height; ++iter2) {
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
}
