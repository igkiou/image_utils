/*
 * test_utils.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: igkiou
 */

#include "mex_utils.h"

mex::MxString test() {
	return mex::MxString("gkiou");
}

mex::MxString test2() {
	mex::MxString ret("gkiou");
	mex::MxString ret2(ret);
	return ret2;
}

std::string test3() {
	return std::string("gkiou");
}

//mex::MxArray test3() {
//	return mex::MxArray(NULL);
//}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	int height = 10;
	int width = 10;
//	const int dims[3] = {height, width, 3};
//	plhs[0] = mxCreateNumericArray(3, dims, mxSINGLE_CLASS, mxREAL); /* x */
	mex::MxNumeric<float> temp(height, width);
	plhs[0] = temp.get_array();
	plhs[1] = mex::MxNumeric<float>(temp.data(), height, width).get_array();
	temp = mex::MxNumeric<float>(height, width);
	plhs[2] = temp.get_array();
	temp[1] = 5.0;
	plhs[3] = mxCreateNumericMatrix(height, width, mex::MxClassId<bool>(), mxREAL);
	mex::MxNumeric<int> foo(height, width);
	foo.data()[0] = 10;
	plhs[4] = foo.get_array();
	std::vector<bool> dummy(10, false);
	plhs[5] = mex::MxNumeric<bool>(dummy).get_array();
	mexPrintf("M %d N %d size %d numel %d.\n", foo.M(), foo.N(), foo.size(), foo.numel());
	plhs[6] = mex::MxString(std::string("gkiou")).get_array();
	plhs[7] = mex::MxString("gkiou").get_array();
	std::string gkiou("gkiou");
	plhs[8] = mex::MxString(gkiou).get_array();
	mex::MxString gkiou2("gkiou");
	plhs[9] = mex::MxString(gkiou2).get_array();
//	plhs[10] = test().get_array();
	plhs[11] = test2().get_array();
}

