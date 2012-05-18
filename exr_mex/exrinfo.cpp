#include "ImathBox.h"
#include "ImfRgba.h"
#include "ImfRgbaFile.h"
#include "ImfInputFile.h"
#include "ImfHeader.h"
#include "ImfArray.h"
#include "ImfChannelList.h"
#include "ImfPixelType.h"
#include "Iex.h"
#include "string"

#include "mex.h"
#include "matrix.h"

using namespace Imf;
using namespace Imath;
using namespace Iex;

#define USED float
#define USEDC FLOAT

std::string printPixelType(const PixelType type) {

	if (type == UINT) {
		return std::string("UINT");
	} else if (type == HALF) {
		return std::string("HALF");
	} else if (type == FLOAT) {
		return std::string("FLOAT");
	} else {
		return std::string("UNKNOWN");
	}
}

void getChannelInfo(const char fileName[]) {
	InputFile in(fileName);

	Header head = in.header();
	ChannelList chan = head.channels();
	ChannelList::Iterator iterChan;
	int iterNum;
	for (iterChan = chan.begin(), iterNum = 0; iterChan != chan.end(); \
		++iterChan, ++iterNum) {
		std::cout << "Channel " << iterNum << " has name " << iterChan.name() << \
				" and type " << printPixelType(iterChan.channel().type) << \
				"." << std::endl;
	}
	std::cout << "Total number of channels is " << iterNum << "." << std::endl;

	Box2i dw = in.header().dataWindow();
	int width = dw.max.x - dw.min.x + 1;
	int height = dw.max.y - dw.min.y + 1;

	std::cout << "Image height is " << height << "." << std::endl;
	std::cout << "Image width is " << width << "." << std::endl;
}

// TODO: Handle case of missing or differently named channels.
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 1) {
		mexErrMsgTxt("One input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 0) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	int length = mxGetNumberOfElements(prhs[0]) + 1;
	char fileName[length];
	mxGetString(prhs[0], fileName, length);
	getChannelInfo(fileName);
}
