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
	MxClassID(const MxClassID& mxClass) {
		m_class = mxClass();
	}

	inline const mxClassID mxClass() const {
		return m_class;
	}

	inline mxClassID mxClass() const {
		return m_class;
	}

	inline const mxClassID operator()() const {
		return m_class;
	}

	inline mxClassID operator()() const {
		return m_class;
	}

	inline operator const mxClassID() const {
		return m_class;
	}

	inline operator mxClassID() const {
		return m_class;
	}
};

MxClassID<unsigned char>::MxClassID() : m_class(mxUINT8_CLASS) {	}
MxClassID<char>::MxClassID() : m_class(mxINT8_CLASS) {	}
MxClassID<short>::MxClassID() : m_class(mxINT16_CLASS) {	}
MxClassID<unsigned short>::MxClassID() : m_class(mxUINT16_CLASS) {	}
MxClassID<int>::MxClassID() : m_class(mxINT32_CLASS) {	}
MxClassID<unsigned int>::MxClassID() : m_class(mxUINT32_CLASS) {	}
MxClassID<long>::MxClassID() : m_class(mxINT64_CLASS) {	}
MxClassID<unsigned long>::MxClassID() : m_class(mxUINT64_CLASS) {	}
MxClassID<float>::MxClassID() : m_class(mxSINGLE_CLASS) {	}
MxClassID<double>::MxClassID() : m_class(mxDOUBLE_CLASS) {	}
MxClassID<bool>::MxClassID() : m_class(mxLOGICAL_CLASS) {	}

//			mxCELL_CLASS,
//		    mxSTRUCT_CLASS,
//		    mxVOID_CLASS,
//		    mxFUNCTION_CLASS,
//		    mxOPAQUE_CLASS,
//		    mxOBJECT_CLASS, /* keep the last real item in the list */

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

	template <typename T>
	MxArrayCreator(const T scalarVar) {
		m_mxArray = mxCreateNumericMatrix(1, 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		*val = (T) scalarVar;
	}

	template <typename T>
	MxArrayCreator(const std::vector<T>& vecVar) {
		m_mxArray = mxCreateNumericMatrix(vecVar.size(), 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		std::copy(vecVar.begin(), vecVar.end(), val);
	}

	template <typename T>
	MxArrayCreator(const T* arrVar, const int numRows, const int numColumns = 1) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numRows * numColumns * sizeof(T));
	}

	template <typename T>
	MxArrayCreator(const T* arrVar, const int numDims, const int *dims) {
		m_mxArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	inline const mxArray* mxArray() const {
		return m_mxArray;
	}

	inline mxArray* mxArray() const {
		return m_mxArray;
	}

	inline const mxArray* operator()() const {
		return m_mxArray;
	}

	inline mxArray* operator()() const {
		return m_mxArray;
	}

	inline operator const mxArray*() const {
		return m_mxArray;
	}

	inline operator mxArray*() const {
		return m_mxArray;
	}

};

}	/* name space mex */

#endif /* MEX_UTILS_H_ */
