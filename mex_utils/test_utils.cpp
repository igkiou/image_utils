/*
 * test_utils.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	int height = 10;
	int width = 10;
//	const int dims[3] = {height, width, 3};
//	plhs[0] = mxCreateNumericArray(3, dims, mxSINGLE_CLASS, mxREAL); /* x */
	mex::MxNumeric<float> temp(height, width);
	plhs[0] = temp();
	plhs[1] = mex::MxNumeric<float>(temp.data(), height, width);
	temp = mex::MxNumeric<float>(height, width);
	plhs[2] = (mxArray*) temp;
	plhs[3] = mxCreateNumericMatrix(height, width, mex::MxClassID<bool>(), mxREAL);
	mex::MxNumeric<int> foo(height, width);
	foo.data()[0] = 10;
	plhs[4] = foo.pArray();
	mexPrintf("M %d N %d size %d numel %d.\n", foo.M(), foo.N(), foo.size(), foo.numel());
	plhs[5] = mex::MxString(std::string("gkiou"));
	plhs[6] = mex::MxString("gkiou");
	std::vector<bool> dummy(10, false);
	plhs[7] = mex::MxNumeric<bool>(dummy);
	plhs[8] = mex::MxNumeric<bool>(dummy).pArray();

}
