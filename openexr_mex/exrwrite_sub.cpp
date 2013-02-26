/*
 * exrwrite_sub.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: igkiou
 */

#include <sstream>

#include "mex_utils.h"
#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs > 5) {
		mexErrMsgTxt("Four or fewer input arguments are required.");
	} else if (nrhs < 2) {
		mexErrMsgTxt("At least two input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	mex::MxNumeric<exr::FloatUsed> image(prhs[0]);
	std::vector<int> dimensions = image.getDimensions();
	mexAssert((dimensions.size() == 2) && (dimensions.size() == 3));
	int numChannels = (dimensions.size() == 2)?(1):(dimensions[2]);
	exr::EXROutputFile file(dimensions[1], dimensions[0]);
	if (nrhs >= 4) {
		mex::MxStruct attributes(prhs[3]);
		file.setAttribute(attributes);
	}
	if ((nrhs >= 3) && (!mex::MxArray(prhs[2]).isEmpty())) {
		mex::MxCell channelNameArray(prhs[2]);
		mexAssert(numChannels == channelNameArray.getNumberOfElements());
		std::vector<std::string> channelNames;
		for (int iter = 0; 	iter < numChannels; ++iter) {
			channelNames.push_back(mex::MxString(channelNameArray[iter]).string());
		}
		file.writeChannel(image, channelNames);
	} else {
		if (numChannels == 1) {
			file.writeChannelY(image);
		} else if (numChannels == 3) {
			file.writeChannelRGB(image);
		} else {
			std::vector<std::string> channelNames;
			for (int iter = 0; 	iter < numChannels; ++iter) {
				std::stringstream temp;
				temp << iter;
				channelNames.push_back(temp.str());
			}
			file.writeChannel(image, channelNames);
		}
	}
	file.writeFile(mex::MxString(prhs[1]));
}
