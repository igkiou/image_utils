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
#include <map>

#include <string.h>

#include "mex.h"
#include "matrix.h"

/*
 * TODO: Add cell support.
 * TODO: Add struct support.
 * TODO: Fix specialization issue.
 */
namespace mex {

// Inspired by mitsuba-0.4.1
// TODO: Add mitsuba copyright.
#ifdef NDEBUG
#define Assert(cond) ((void) 0)
#define AssertEx(cond, explanation) ((void) 0)
#else
/* Assertions */
// Assert that a condition is true
#define Assert(cond) do { \
		if (!(cond)) mexErrMsgIdAndTxt("MATLAB:image_utils", "Assertion \"%s\" failed in %s:%i\n", \
		#cond, __FILE__, __LINE__); \
	} while (0)

// Assertion with a customizable error explanation
#define AssertEx(cond, explanation) do { \
		if (!(cond)) mexErrMsgIdAndTxt("MATLAB:image_utils", "Assertion \"%s\" failed in %s:%i (" explanation ")\n", \
		#cond, __FILE__, __LINE__); \
	} while (0)
#endif

template <typename T, typename U>
class ConstMap {
private:
	std::map<T, U> m_;
public:

	ConstMap(const T& key, const U& val) {
		m_[key] = val;
	}

	ConstMap<T, U>& operator()(const T& key, const U& val) {
		m_[key] = val;
		return *this;
	}

	operator std::map<T, U>() { return m_; }

	U operator [](const T& key) const {
		typename std::map<T,U>::const_iterator it = m_.find(key);
		if (it==m_.end())
			mexErrMsgIdAndTxt("openexr_mex:error", "Value not found");
		return (*it).second;
	}
};

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
//template <> MxClassID<pMxArray>::MxClassID() : m_class(mxCELL_CLASS) {	}
//template <> MxClassID<std::string>::MxClassID() : m_class(mxCHAR_CLASS) {	}

//			mxCELL_CLASS,
//		    mxSTRUCT_CLASS,
//		    mxVOID_CLASS,
//		    mxFUNCTION_CLASS,
//		    mxOPAQUE_CLASS,
//		    mxOBJECT_CLASS, /* keep the last real item in the list */

struct MxArray {
protected:
	pMxArray m_mxArray;

public:
	MxArray()
		: m_mxArray() {	}

	MxArray(const MxArray& arr)
		: m_mxArray(arr()) {	}

	MxArray& operator=(const MxArray& rhs) {
		if (this != &rhs) {
			this->m_mxArray = rhs.m_mxArray;
		}
		return *this;
	}

	MxArray(const pMxArray mxarr)
		: m_mxArray(mxarr) {	}

	virtual ~MxArray() {}

/*
 * TODO: Decide whether to provide these, possible conflict with T* data in
 * numeric.
 */
//	inline const void * data() const {
//		return (void *) mxGetData(m_mxArray);
//	}
//
//	inline void * data() {
//		return (void *) mxGetData(m_mxArray);
//	}

	/* getter */
	inline const pMxArray getMxArray() const {
		return m_mxArray;
	}

	inline pMxArray getMxArray() {
		return m_mxArray;
	}

	/* parenthesis operator */
	inline const pMxArray operator()() const {
		return m_mxArray;
	}

	inline pMxArray operator()() {
		return m_mxArray;
	}

	/* cast to getMxArray* */
	inline operator const pMxArray() const {
		return m_mxArray;
	}

	inline operator pMxArray() {
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

	inline void destroy() {
		mxDestroyArray(m_mxArray);
	}
};


/* Special declarations for string. */
//MxString(const std::string& stringVar)
//			: m_mxArray(mxCreateString(stringVar.c_str())) {	}
//
//MxString(const char* cStringVar)
//			: m_mxArray(mxCreateString(cStringVar)) {	}

template <typename T>
struct MxNumeric : public MxArray {
private:
	mxClassID m_mxClassID;

public:
	MxNumeric()
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {	}

	MxNumeric(const MxNumeric<T>& arr)
		: 	MxArray(arr), \
			m_mxClassID(MxClassID<T>()) {	}

	MxNumeric<T>& operator=(const MxNumeric<T>& rhs) {
		if (this != &rhs) {
			this->m_mxArray = rhs.m_mxArray;
			this->m_mxClassID = rhs.m_mxClassID;
		}
		return *this;
	}

	MxNumeric(const pMxArray mxarr)
		: MxArray(mxarr), \
		  m_mxClassID(MxClassID<T>()) {
		Assert(m_mxClassID == mxGetClassID(mxarr));
	}

	MxNumeric(const int numRows, const int numColumns)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), mxREAL);
	}

//	MxNumeric(const std::vector<int> &dims)
//		: MxArray(), \
//		  m_mxClassID(MxClassID<T>()) {
//		const int numDims = dims.size();
//		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
//		for (int iter = 0; iter < numDims; ++iter) {
//			tempDims[iter] = dims[iter];
//		}
//		m_mxArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
//		mxFree(tempDims);
//	}

	MxNumeric(const int numDims, const int *dims)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
		mxFree(tempDims);
	}

	MxNumeric(const int numDims, const mwSize *dims)
		: m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
	}

	MxNumeric(const T scalarVar)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(1, 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		*val = (T) scalarVar;
	}

	MxNumeric(const std::vector<T>& vecVar)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(vecVar.size(), 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		std::copy(vecVar.begin(), vecVar.end(), val);
	}

	MxNumeric(const T* arrVar, const int numRows, const int numColumns)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numRows * numColumns * sizeof(T));
	}

	MxNumeric(const T* arrVar, const std::vector<int>& dims)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		const int numDims = dims.size();
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	MxNumeric(const T* arrVar, const int numDims, const int *dims)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	MxNumeric(const T* arrVar, const int numDims, const mwSize *dims)
	: MxArray(), \
	  m_mxClassID(MxClassID<T>()) {
		m_mxArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		T *val = (T *) mxGetData(m_mxArray);
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	/* getter */
	inline mxClassID getMxClassID() {
		return m_mxClassID;
	}

	inline const mxClassID getMxClassID() const {
		return m_mxClassID;
	}

	inline const T* data() const {
		return (T *) mxGetData(m_mxArray);
	}

	inline T* data() {
		return (T *) mxGetData(m_mxArray);
	}

	inline std::vector<T> vectorize() {
		int numElements = numel();
		const T* pData = data();
		return std::vector<T>(pData, pData + numElements);
	}

	inline const std::vector<T> vectorize() const {
		int numElements = numel();
		const T* pData = data();
		return std::vector<T>(pData, pData + numElements);
	}
};

struct MxString: public MxArray {
private:
	const static mxClassID m_mxClassID = mxCHAR_CLASS;

public:
	MxString(const MxString& arr)
		: MxArray(arr) {	}

	MxString& operator=(const MxString& rhs) {
		if (this != &rhs) {
			this->m_mxArray = rhs.m_mxArray;
		}
		return *this;
	}

	MxString(const pMxArray mxarr)
		: MxArray(mxarr) {
		Assert(m_mxClassID == mxGetClassID(mxarr));
	}

	MxString(const std::string& strVar) {
		m_mxArray = mxCreateString(strVar.c_str());
	}

	MxString(const char* charVar) {
		m_mxArray = mxCreateString(charVar);
	}

	std::string string() {
		char *temp = mxArrayToString(m_mxArray);
		std::string retArg(temp);
		mxFree(temp);
		return retArg;
	}

	const std::string string() const {
		char *temp = mxArrayToString(m_mxArray);
		std::string retArg(temp);
		mxFree(temp);
		return retArg;
	}

	char* c_str() {
		return mxArrayToString(m_mxArray);
	}

	const char* c_str() const {
		return mxArrayToString(m_mxArray);
	}

	inline int length() const {
		return mxGetNumberOfElements(m_mxArray);
	}
};

struct MxCell : public MxArray {
private:
	const static mxClassID m_mxClassID = mxCELL_CLASS;

public:
	MxCell(const MxCell& arr)
		: MxArray(arr) {	}

	MxCell& operator=(const MxCell& rhs) {
		if (this != &rhs) {
			this->m_mxArray = rhs.m_mxArray;
		}
		return *this;
	}

	MxCell(const pMxArray mxarr)
		: MxArray() {
		if (m_mxClassID == mxGetClassID(mxarr)) {
			m_mxArray = mxarr;
		} else {
			m_mxArray = mxCreateCellMatrix(1, 1);
			mxSetCell(m_mxArray, 0, mxarr);
		}
	}

	MxCell(const int numRows, const int numColumns)
		: MxArray() {
		m_mxArray = mxCreateCellMatrix(numRows, numColumns);
	}

	MxCell(const std::vector<int> &dims)
		: MxArray() {
		const int numDims = dims.size();
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
	}

	MxCell(const int numDims, const int *dims)
		: MxArray() {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
	}

	MxCell(const int numDims, const mwSize *dims)
		: MxArray() {
		m_mxArray = mxCreateCellArray(numDims, dims);
	}

	MxCell(const std::vector<pMxArray>& vecVar)
		: MxArray() {
		m_mxArray = mxCreateCellMatrix(vecVar.size(), 1);
		for (int iter = 0, numElements = vecVar.size(); iter < numElements; ++iter) {
			mxSetCell(m_mxArray, iter, vecVar[iter]);
		}
	}

	MxCell(const std::vector<MxArray>& vecVar)
		: MxArray() {
		m_mxArray = mxCreateCellMatrix(vecVar.size(), 1);
		for (int iter = 0, numElements = vecVar.size(); iter < numElements; ++iter) {
			mxSetCell(m_mxArray, iter, (pMxArray) vecVar[iter]);
		}
	}

	MxCell(const pMxArray* arrVar, const int numRows, const int numColumns)
		: MxArray() {
		m_mxArray = mxCreateCellMatrix(numRows, numColumns);
		int numElements = numRows * numColumns;
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_mxArray, iter, arrVar[iter]);
		}
	}

	MxCell(const pMxArray* arrVar, const std::vector<int>& dims)
		: MxArray() {
		const int numDims = dims.size();
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_mxArray, iter, arrVar[iter]);
		}
	}

	MxCell(const pMxArray* arrVar, const int numDims, const int *dims)
		: MxArray() {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_mxArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_mxArray, iter, arrVar[iter]);
		}
	}

	MxCell(const pMxArray* arrVar, const int numDims, const mwSize *dims)
		: MxArray() {
		m_mxArray = mxCreateCellArray(numDims, dims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_mxArray, iter, arrVar[iter]);
		}
	}

	inline const pMxArray* data() const {
		return (pMxArray *) mxGetData(m_mxArray);
	}

	inline pMxArray* data() {
		return (pMxArray *) mxGetData(m_mxArray);
	}

	template <typename T>
	inline const std::vector<T> vectorize() const {
		int numElements = numel();
		std::vector<T> retArg;
		for (int iter = 0; iter < numElements; ++iter) {
			retArg.push_back(T(mxGetCell(m_mxArray, iter)));
		}
		return retArg;
	}

	template <typename T>
	inline std::vector<T> vectorize() {
		int numElements = numel();
		std::vector<T> retArg;
		for (int iter = 0; iter < numElements; ++iter) {
			retArg.push_back(T(mxGetCell(m_mxArray, iter)));
		}
		return retArg;
	}
};

}	/* name space mex */

#endif /* MEX_UTILS_H_ */
