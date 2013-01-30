/*
 * mex_utils.h
 *
 *  Created on: Jan 30, 2013
 *      Author: igkiou
 */

#ifndef MEX_UTILS_H_
#define MEX_UTILS_H_

#include <algorithm>
#include <string>
#include <vector>

#include <string.h>

#include "mex.h"
#include "matrix.h"

namespace mex {


template <typename T>
class MxClassID {
private:
	mxClassID m_class;

public:
	MxClassID();
	MxClassID(const MxClassID<T> mxClass) {
		m_class = mxClass();
	}

	inline const mxClassID mxClass() const {
		return m_class;
	}

	inline mxClassID mxClass() const {
		return m_class;
	}

};

MxClassID<int>::MxClassID() : m_class(mxINT32_CLASS) {	};
MxClassID<float>::MxClassID() : m_class(mxSINGLE_CLASS) {	}
MxClassID<double>::MxClassID() : m_class(mxDOUBLE_CLASS) {	}
MxClassID<char>::MxClassID() : m_class(mxCHAR_CLASS) {	}



//		mxCELL_CLASS,
//		    mxSTRUCT_CLASS,
//		    mxLOGICAL_CLASS,
//		    mxCHAR_CLASS,
//		    mxVOID_CLASS,
//		    mxINT8_CLASS,
//		    mxUINT8_CLASS,
//		    mxINT16_CLASS,
//		    mxUINT16_CLASS,
//		    mxINT32_CLASS,
//		    mxUINT32_CLASS,
//		    mxINT64_CLASS,
//		    mxUINT64_CLASS,
//		    mxFUNCTION_CLASS,
//		    mxOPAQUE_CLASS,
//		    mxOBJECT_CLASS, /* keep the last real item in the list */
//		#if defined(_LP64) || defined(_WIN64)
//		    mxINDEX_CLASS = mxUINT64_CLASS,
//		#else
//		    mxINDEX_CLASS = mxUINT32_CLASS,
//		#endif

struct MxArrayCreator {
private:
	mxArray *m_mxArray;

public:
	MxArrayCreator(const std::string& stringVar) {
		m_mxArray = mxCreateString(stringVar.c_str());
	}

	MxArrayCreator(const char* cStringVar) {
		m_mxArray = mxCreateString(cStringVar);
	}



	MxArrayCreator(const int intVar) {
		m_mxArray = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
		int *val = (int *) mxGetData(m_mxArray);
		*val = (int) intVar;
	}

	MxArrayCreator(const std::vector<int>& intVec) {
		m_mxArray = mxCreateNumericMatrix(intVec.size(), 1, mxINT32_CLASS, mxREAL);
		int *val = (int *) mxGetData(m_mxArray);
		std::copy(intVec.begin(), intVec.end(), val);
	}

	MxArrayCreator(const int* intArr, const int numRows, const int numColumns) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, mxINT32_CLASS, mxREAL);
		int *val = (int *) mxGetData(m_mxArray);
		memcpy(val, intArr, numRows * numColumns * sizeof(int));
	}

};


}	/* name space mex */


#endif /* MEX_UTILS_H_ */
