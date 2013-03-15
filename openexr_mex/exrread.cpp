/*
 * exrread.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: igkiou
 */

#include "mex_utils.h"
#include "openexr_mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs > 2) {
		mexErrMsgTxt("Two or fewer arguments are required.");
	} else if (nrhs < 1) {
		mexErrMsgTxt("At least one input argument is required.");
	}

	/* Check number of output arguments */
	if (nlhs > 2) {
		mexErrMsgTxt("Too many output arguments.");
	}

	exr::EXRInputFile file(mex::MxString(const_cast<mxArray*>(prhs[0])));
	if ((nrhs >= 2) && (!mex::MxArray(const_cast<mxArray*>(prhs[1])).isEmpty())) {
		mex::MxCell channelNameArray(const_cast<mxArray*>(prhs[1]));
		std::vector<std::string> channelNames;
		for (int iter = 0, end = channelNameArray.getNumberOfElements();
			iter < end;
			++iter) {
			channelNames.push_back(mex::MxString(channelNameArray[iter]).get_string());
		}
		file.readChannel(channelNames);
	} else {
		/*
		 * TODO: Handle Alpha channel in RGB.
		 */
		int numChannels = file.getNumberOfChannels();
		if ((numChannels == 1)
			|| ((numChannels == 2) && file.hasChannel(std::string("A")))) {
			file.readChannelY();
		} else if ((numChannels == 3)
			|| ((numChannels == 4) && file.hasChannel(std::string("A")))) {
			file.readChannelRGB();
		} else {
			file.readChannel(file.getChannelNames());
		}
	}
	plhs[0] = file.readFile().get_array();
	if (nlhs >= 2) {
		plhs[1] = file.getAttribute().get_array();
	}
}
