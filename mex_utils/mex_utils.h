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

/*
 * TODO: Add cell support.
 * TODO: Add struct support.
 * TODO: Fix specialization issue.
 */
namespace mex {

typedef mxArray* pMxArray;

template <typename T>
class MxClassID {
private:
	mxClassID m_class;

public:
	MxClassID() : m_class(mxUNKNOWN_CLASS) {	}
	MxClassID(const MxClassID& mxClass) {
		m_class = mxClass();
	}

	inline const mxClassID getMxClassID() const {
		return m_class;
	}

	inline mxClassID getMxClassID() {
		return m_class;
	}

	inline const mxClassID operator()() const {
		return m_class;
	}

	inline mxClassID operator()() {
		return m_class;
	}

	inline operator const mxClassID() const {
		return m_class;
	}

	inline operator mxClassID() {
		return m_class;
	}
};

template <> MxClassID<unsigned char>::MxClassID() : m_class(mxUINT8_CLASS) {	}
template <> MxClassID<char>::MxClassID() : m_class(mxINT8_CLASS) {	}
template <> MxClassID<short>::MxClassID() : m_class(mxINT16_CLASS) {	}
template <> MxClassID<unsigned short>::MxClassID() : m_class(mxUINT16_CLASS) {	}
template <> MxClassID<int>::MxClassID() : m_class(mxINT32_CLASS) {	}
template <> MxClassID<unsigned int>::MxClassID() : m_class(mxUINT32_CLASS) {	}
template <> MxClassID<long>::MxClassID() : m_class(mxINT64_CLASS) {	}
template <> MxClassID<unsigned long>::MxClassID() : m_class(mxUINT64_CLASS) {	}
template <> MxClassID<float>::MxClassID() : m_class(mxSINGLE_CLASS) {	}
template <> MxClassID<double>::MxClassID() : m_class(mxDOUBLE_CLASS) {	}
template <> MxClassID<bool>::MxClassID() : m_class(mxLOGICAL_CLASS) {	}
template <> MxClassID<pMxArray>::MxClassID() : m_class(mxCELL_CLASS) {	}

//			mxCELL_CLASS,
//		    mxSTRUCT_CLASS,
//		    mxVOID_CLASS,
//		    mxFUNCTION_CLASS,
//		    mxOPAQUE_CLASS,
//		    mxOBJECT_CLASS, /* keep the last real item in the list */

template <typename T>
struct MxArray {
private:
	mxArray *m_mxArray;
	mxClassID m_mxClassID;

public:
	MxArray()
		: m_mxClassID(MxClassID<T>()), m_mxArray() {	};

	MxArray(const int numRows, const int numColumns)
		: m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), mxREAL);
	}

//	MxArray(const int numDims, const int *dims)
//		: m_mxClassID(MxClassID<T>()) {
//		m_mxArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
//	}

	MxArray(const T scalarVar)
		: m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(1, 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		*val = (T) scalarVar;
	}

	MxArray(const std::vector<T>& vecVar)
		: m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(vecVar.size(), 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		std::copy(vecVar.begin(), vecVar.end(), val);
	}

	MxArray(const T* arrVar, const int numRows, const int numColumns)
		: m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numRows * numColumns * sizeof(T));
	}

//	MxArray(const T* arrVar, const int numDims, const int *dims)
//		: m_mxClassID(MxClassID<T>()) {
//		m_mxArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
//		T *val = (T *) mxGetData(m_mxArray);
//		int numElements = 1;
//		for (int iter = 0; iter < numDims; ++iter) {
//			numElements *= dims[iter];
//		}
//		memcpy(val, arrVar, numElements * sizeof(T));
//	}

	inline const mxArray* getMxArray() const {
		return m_mxArray;
	}

	inline mxArray* getMxArray() {
		return m_mxArray;
	}

	inline const mxArray* operator()() const {
		return m_mxArray;
	}

	inline mxArray* operator()() {
		return m_mxArray;
	}

	inline int size() const {
		return mxGetNumberOfElements(m_mxArray);
	}

	inline int numel() const {
		return mxGetNumberOfElements(m_mxArray);
	}

	inline int M() const {
		return mxGetM(m_mxArray);
	}

	inline int N() const {
		return mxGetN(m_mxArray);
	}

	inline int numDim() const {
		return mxGetNumberOfDimensions(m_mxArray);
	}

//	inline int dims() const {
//		return mxGetDimensions(m_mxArray);
//	}

	inline const T* data() const {
		return (T *) mxGetData(m_mxArray);
	}

	inline T* data() {
		return (T *) mxGetData(m_mxArray);
	}

	inline void destroy() {
		mxDestroyArray(m_mxArray);
	}
};

struct MxArrayCreator {
private:
	mxArray *m_mxArray;

public:
	/* Templates for numeric types. */
	template <typename T>
	MxArrayCreator(const MxClassID<T> classID, const int numRows, const int numColumns) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, classID.getMxClassID(), mxREAL);
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
	MxArrayCreator(const T* arrVar, const int numRows, const int numColumns) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numRows * numColumns * sizeof(T));
	}

//	template <typename T>
//	MxArrayCreator(const MxClassID<T> classID, const int numDims, const int *dims) {
//		m_mxArray = mxCreateNumericArray(numDims, dims, classID.getMxClassID(), mxREAL);
//	}
//	template <typename T>
//	MxArrayCreator(const T* arrVar, const int numDims, const int *dims) {
//		m_mxArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
//		T *val = (T *) mxGetData(m_mxArray);
//		int numElements = 1;
//		for (int iter = 0; iter < numDims; ++iter) {
//			numElements *= dims[iter];
//		}
//		memcpy(val, arrVar, numElements * sizeof(T));
//	}

	/* "Specializations" (overloads) for cell. */
	MxArrayCreator(const MxClassID<pMxArray> classID, const int numRows, const int numColumns) {
		m_mxArray = mxCreateCellMatrix(numRows, numColumns);
	}

	MxArrayCreator(const pMxArray scalarVar) {
		m_mxArray = mxCreateCellMatrix(1, 1);
		mxSetCell(m_mxArray, 1, scalarVar);
	}

	MxArrayCreator(const std::vector<pMxArray>& vecVar) {
		m_mxArray = mxCreateCellMatrix(vecVar.size(), 1);
		for (int iter = 0, size = vecVar.size(); iter < size; ++iter) {
			mxSetCell(m_mxArray, iter, vecVar[iter]);
		}
	}

	MxArrayCreator(const pMxArray* arrVar, const int numRows, const int numColumns) {
		m_mxArray = mxCreateCellMatrix(numRows, numColumns);
		for (int iter = 0; iter < numRows * numColumns; ++iter) {
			mxSetCell(m_mxArray, iter, arrVar[iter]);
		}
	}

	/* Special declarations for string. */
	MxArrayCreator(const std::string& stringVar)
				: m_mxArray(mxCreateString(stringVar.c_str())) {	}

	MxArrayCreator(const char* cStringVar)
				: m_mxArray(mxCreateString(cStringVar)) {	}

	virtual ~MxArrayCreator() {}

	inline const mxArray* getMxArray() const {
		return m_mxArray;
	}

	inline mxArray* getMxArray() {
		return m_mxArray;
	}

	inline const mxArray* operator()() const {
		return m_mxArray;
	}

	inline mxArray* operator()() {
		return m_mxArray;
	}

	inline operator const mxArray*() const {
		return m_mxArray;
	}

	inline operator mxArray*() {
		return m_mxArray;
	}

	inline const void * data() const {
		return (void *) mxGetData(m_mxArray);
	}

	inline void * data() {
		return (void *) mxGetData(m_mxArray);
	}
};

/*
 * TODO: These are not working due to gcc bug.
 */
//template <>
//MxArrayCreator::MxArrayCreator<pMxArray>(const MxClassID<pMxArray> classID, const int numRows, const int numColumns) {
//	m_mxArray = mxCreateCellMatrix(numRows, numColumns);
//}
//
//template <>
//MxArrayCreator::MxArrayCreator<pMxArray>(const pMxArray scalarVar) {
//	m_mxArray = mxCreateCellMatrix(1, 1);
//	mxSetCell(m_mxArray, 1, scalarVar);
//}
//
//
//template <>
//MxArrayCreator::MxArrayCreator<pMxArray>(const std::vector<pMxArray>& vecVar) {
//	m_mxArray = mxCreateCellMatrix(vecVar.size(), 1);
//	for (int iter = 0, size = vecVar.size(); iter < size; ++iter) {
//		mxSetCell(m_mxArray, iter, vecVar[iter]);
//	}
//}
//
//template <>
//MxArrayCreator::MxArrayCreator<pMxArray>(const pMxArray* arrVar, const int numRows, const int numColumns) {
//	m_mxArray = mxCreateCellMatrix(numRows, numColumns);
//	for (int iter = 0; iter < numRows * numColumns; ++iter) {
//		mxSetCell(m_mxArray, iter, arrVar[iter]);
//	}
//}
//
//template <>
//inline MxArrayCreator::MxArrayCreator<pMxArray>(const pMxArray* arrVar, const int numDims, const int *dims) {
//	m_mxArray = mxCreateCellArray(numDims, dims);
//	int numElements = 1;
//	for (int iter = 0; iter < numDims; ++iter) {
//		numElements *= dims[iter];
//	}
//	for (int iter = 0; iter < numElements; ++iter) {
//		mxSetCell(m_mxArray, iter, arrVar[iter]);
//	}
//}

}	/* name space mex */

#endif /* MEX_UTILS_H_ */
