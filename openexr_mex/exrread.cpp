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

	openexr::ExrInputFile file(mex::MxString(const_cast<mxArray*>(prhs[0])));
	if ((nrhs >= 2) && (!mex::MxArray(const_cast<mxArray*>(prhs[1])).isEmpty())) {
		mex::MxCell channelNameArray(const_cast<mxArray*>(prhs[1]));
		std::vector<mex::MxString> channelNames;
		for (int iter = 0, end = channelNameArray.getNumberOfElements();
			iter < end;
			++iter) {
			channelNames.push_back(mex::MxString(channelNameArray[iter]));
		}
		plhs[0] = file.readData(channelNames).get_array();
	} else {
		/*
		 * TODO: Handle Alpha channel in RGB.
		 */
		int numChannels = file.getNumberOfChannels();
		if ((numChannels == 1)
			|| ((numChannels == 2) && file.hasChannel(std::string("A")))) {
			plhs[0] = file.readDataY().get_array();
		} else if ((numChannels == 3)
			|| ((numChannels == 4) && file.hasChannel(std::string("A")))) {
			plhs[0] = file.readDataRGB().get_array();
		} else {
			plhs[0] = file.readData();
		}
	}
	if (nlhs >= 2) {
		plhs[1] = file.getAttribute().get_array();
	}
}
