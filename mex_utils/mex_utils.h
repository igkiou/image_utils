/*
 * mex_utils.h
 *
 *  Created on: Jan 30, 2013
 *      Author: igkiou
 */

#ifndef MEX_UTILS_H_
#define MEX_UTILS_H_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "mex.h"
#include "matrix.h"

/*
 * TODO: Add struct support.
 */
namespace mex {

#ifdef NDEBUG
#define Assert(cond) ((void) 0)
#define AssertEx(cond, explanation) ((void) 0)
#else
#define Assert(cond) do { \
		if (!(cond)) \
		mexErrMsgIdAndTxt("MATLAB:mex", "Assertion \"%s\" failed in %s:%i\n", \
		#cond, __FILE__, __LINE__); \
	} while (0)

#define AssertEx(cond, explanation) do { \
		if (!(cond)) \
		mexErrMsgIdAndTxt(\
		"MATLAB:mex", "Assertion \"%s\" failed in %s:%i (" explanation ")\n", \
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

	U operator[] (const T& key) const {
		typename std::map<T, U>::const_iterator it = m_.find(key);
		AssertEx(it == m_.end(), "Value not found.");
		return (*it).second;
	}
};

typedef mxArray MxArrayNative;
typedef MxArrayNative* PMxArrayNative;

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

class MxArray {
protected:
	PMxArrayNative m_pArray;

public:
	MxArray()
		: m_pArray() {	}

	MxArray(const MxArray& arr)
		: m_pArray(arr()) {	}

	MxArray& operator=(const MxArray& rhs) {
		if (this != &rhs) {
			this->m_pArray = rhs.m_pArray;
		}
		return *this;
	}

	explicit MxArray(const PMxArrayNative mxarr)
		: m_pArray(mxarr) {	}

	virtual ~MxArray() {}

/*
 * TODO: Decide whether to provide these, possible conflict with T* data in
 * numeric.
 */
//	inline const void * data() const {
//		return (void *) mxGetData(m_pArray);
//	}
//
//	inline void * data() {
//		return (void *) mxGetData(m_pArray);
//	}

	/* getter */
	inline const PMxArrayNative pArray() const {
		return m_pArray;
	}

	inline PMxArrayNative pArray() {
		return m_pArray;
	}

	/* parenthesis operator */
	inline const PMxArrayNative operator()() const {
		return m_pArray;
	}

	inline PMxArrayNative operator()() {
		return m_pArray;
	}

	inline int size() const {
		return mxGetNumberOfElements(m_pArray);
	}

	inline int numel() const {
		return mxGetNumberOfElements(m_pArray);
	}

	inline int M() const {
		return mxGetM(m_pArray);
	}

	inline int N() const {
		return mxGetN(m_pArray);
	}

	inline int numDim() const {
		return mxGetNumberOfDimensions(m_pArray);
	}

//	inline int dims() const {
//		return mxGetDimensions(m_pArray);
//	}

	inline void destroy() {
		mxDestroyArray(m_pArray);
	}
};

template <typename T>
class MxNumeric : public MxArray {
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
			this->m_pArray = rhs.m_pArray;
			this->m_mxClassID = rhs.m_mxClassID;
		}
		return *this;
	}

	explicit MxNumeric(const PMxArrayNative mxarr)
		: MxArray(mxarr), \
		  m_mxClassID(MxClassID<T>()) {
		Assert(m_mxClassID == mxGetClassID(mxarr));
	}

	MxNumeric(const int numRows, const int numColumns)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_pArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(), \
										mxREAL);
	}

//	MxNumeric(const std::vector<int> &dims)
//		: MxArray(), \
//		  m_mxClassID(MxClassID<T>()) {
//		const int numDims = dims.size();
//		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
//		for (int iter = 0; iter < numDims; ++iter) {
//			tempDims[iter] = dims[iter];
//		}
//		m_pArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
//		mxFree(tempDims);
//	}

	MxNumeric(const int numDims, const int *dims)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_pArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), \
										mxREAL);
		mxFree(tempDims);
	}

	MxNumeric(const int numDims, const mwSize *dims)
		: m_mxClassID(MxClassID<T>()) {
		m_pArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
	}

	explicit MxNumeric(const T scalarVar)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_pArray = mxCreateNumericMatrix(1, 1, MxClassID<T>(), mxREAL);
		T *val = (T *) mxGetData(m_pArray);
		*val = (T) scalarVar;
	}

	explicit MxNumeric(const std::vector<T>& vecVar)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_pArray = mxCreateNumericMatrix(vecVar.size(), 1, MxClassID<T>(),
										mxREAL);
		T *val = (T *) mxGetData(m_pArray);
		std::copy(vecVar.begin(), vecVar.end(), val);
	}

	MxNumeric(const T* arrVar, const int numRows, const int numColumns)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		m_pArray = mxCreateNumericMatrix(numRows, numColumns, MxClassID<T>(),
										mxREAL);
		T *val = (T *) mxGetData(m_pArray);
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
		m_pArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		T *val = (T *) mxGetData(m_pArray);
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	MxNumeric(const T* arrVar, const int numDims, const int *dims)
		: MxArray(), \
		  m_mxClassID(MxClassID<T>()) {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_pArray = mxCreateNumericArray(numDims, tempDims, MxClassID<T>(), mxREAL);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		T *val = (T *) mxGetData(m_pArray);
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	MxNumeric(const T* arrVar, const int numDims, const mwSize *dims)
	: MxArray(), \
	  m_mxClassID(MxClassID<T>()) {
		m_pArray = mxCreateNumericArray(numDims, dims, MxClassID<T>(), mxREAL);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		T *val = (T *) mxGetData(m_pArray);
		memcpy(val, arrVar, numElements * sizeof(T));
	}

	/* getter */
	inline mxClassID getMxClassID() {
		return m_mxClassID;
	}

	inline const mxClassID getMxClassID() const {
		return m_mxClassID;
	}

	inline T& operator[] (int i) {
		T* temp = (T *) mxGetData(m_pArray);
		return temp[i];
	}

	inline const T& operator[] (int i) const {
		T* temp = (T *) mxGetData(m_pArray);
		return temp[i];
	}

	inline const T* data() const {
		return (T *) mxGetData(m_pArray);
	}

	inline T* data() {
		return (T *) mxGetData(m_pArray);
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

class MxString: public MxArray {
private:
	const static mxClassID m_mxClassID = mxCHAR_CLASS;

public:
	MxString(const MxString& arr)
		: MxArray(arr) {	}

	MxString& operator=(const MxString& rhs) {
		if (this != &rhs) {
			this->m_pArray = rhs.m_pArray;
		}
		return *this;
	}

	MxString(const PMxArrayNative mxarr)
		: MxArray(mxarr) {
		Assert(m_mxClassID == mxGetClassID(mxarr));
	}

	MxString(const std::string& strVar) {
		m_pArray = mxCreateString(strVar.c_str());
	}

	MxString(const char* charVar) {
		m_pArray = mxCreateString(charVar);
	}

	inline std::string string() {
		char *temp = mxArrayToString(m_pArray);
		std::string retArg(temp);
		mxFree(temp);
		return retArg;
	}

	inline const std::string string() const {
		char *temp = mxArrayToString(m_pArray);
		std::string retArg(temp);
		mxFree(temp);
		return retArg;
	}

	inline char* c_str() {
		return mxArrayToString(m_pArray);
	}

	inline const char* c_str() const {
		return mxArrayToString(m_pArray);
	}

	inline int length() const {
		return mxGetNumberOfElements(m_pArray);
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
			this->m_pArray = rhs.m_pArray;
		}
		return *this;
	}

	MxCell(const PMxArrayNative mxarr)
		: MxArray() {
		if (m_mxClassID == mxGetClassID(mxarr)) {
			m_pArray = mxarr;
		} else {
			m_pArray = mxCreateCellMatrix(1, 1);
			mxSetCell(m_pArray, 0, mxarr);
		}
	}

	MxCell(const int numRows, const int numColumns)
		: MxArray() {
		m_pArray = mxCreateCellMatrix(numRows, numColumns);
	}

	MxCell(const std::vector<int> &dims)
		: MxArray() {
		const int numDims = dims.size();
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_pArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
	}

	MxCell(const int numDims, const int *dims)
		: MxArray() {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_pArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
	}

	MxCell(const int numDims, const mwSize *dims)
		: MxArray() {
		m_pArray = mxCreateCellArray(numDims, dims);
	}

	MxCell(const std::vector<PMxArrayNative>& vecVar)
		: MxArray() {
		m_pArray = mxCreateCellMatrix(vecVar.size(), 1);
		for (int iter = 0, numElements = vecVar.size(); iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, vecVar[iter]);
		}
	}

	MxCell(const std::vector<MxArray *>& vecVar)
		: MxArray() {
		m_pArray = mxCreateCellMatrix(vecVar.size(), 1);
		for (int iter = 0, numElements = vecVar.size(); iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, (PMxArrayNative) (*(vecVar[iter])));
		}
	}

	MxCell(const std::vector<const MxArray *>& vecVar)
		: MxArray() {
		m_pArray = mxCreateCellMatrix(vecVar.size(), 1);
		for (int iter = 0, numElements = vecVar.size(); iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, (PMxArrayNative) (*(vecVar[iter])));
		}
	}

	MxCell(const PMxArrayNative* arrVar, const int numRows, const int numColumns)
		: MxArray() {
		m_pArray = mxCreateCellMatrix(numRows, numColumns);
		int numElements = numRows * numColumns;
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, arrVar[iter]);
		}
	}

	MxCell(const PMxArrayNative* arrVar, const std::vector<int>& dims)
		: MxArray() {
		const int numDims = dims.size();
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_pArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, arrVar[iter]);
		}
	}

	MxCell(const PMxArrayNative* arrVar, const int numDims, const int *dims)
		: MxArray() {
		mwSize *tempDims = (mwSize *) malloc(numDims * sizeof(mwSize));
		for (int iter = 0; iter < numDims; ++iter) {
			tempDims[iter] = dims[iter];
		}
		m_pArray = mxCreateCellArray(numDims, tempDims);
		mxFree(tempDims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, arrVar[iter]);
		}
	}

	MxCell(const PMxArrayNative* arrVar, const int numDims, const mwSize *dims)
		: MxArray() {
		m_pArray = mxCreateCellArray(numDims, dims);
		int numElements = 1;
		for (int iter = 0; iter < numDims; ++iter) {
			numElements *= dims[iter];
		}
		for (int iter = 0; iter < numElements; ++iter) {
			mxSetCell(m_pArray, iter, arrVar[iter]);
		}
	}

	inline PMxArrayNative& operator[] (int i) {
		return mxGetCell(m_pArray, i);
	}

	inline const PMxArrayNative& operator[] (int i) const {
		return mxGetCell(m_pArray, i);
	}

	inline const PMxArrayNative* data() const {
		return (PMxArrayNative *) mxGetData(m_pArray);
	}

	inline PMxArrayNative* data() {
		return (PMxArrayNative *) mxGetData(m_pArray);
	}

	template <typename T>
	inline const std::vector<T> vectorize() const {
		int numElements = numel();
		std::vector<T> retArg;
		for (int iter = 0; iter < numElements; ++iter) {
			retArg.push_back(T(mxGetCell(m_pArray, iter)));
		}
		return retArg;
	}

	template <typename T>
	inline std::vector<T> vectorize() {
		int numElements = numel();
		std::vector<T> retArg;
		for (int iter = 0; iter < numElements; ++iter) {
			retArg.push_back(T(mxGetCell(m_pArray, iter)));
		}
		return retArg;
	}
};

struct MxStruct : public MxArray {
private:
	const static mxClassID m_mxClassID = mxSTRUCT_CLASS;

public:
	MxStruct(const MxStruct& arr)
		: MxArray(arr) {	}

	MxStruct& operator=(const MxStruct& rhs) {
		if (this != &rhs) {
			this->m_pArray = rhs.m_pArray;
		}
		return *this;
	}

	MxStruct(const PMxArrayNative mxarr)
		: MxArray() {
		Assert(m_mxClassID == mxGetClassID(mxarr));
	}



	inline PMxArrayNative& operator[] (int i) {
		return mxGetCell(m_pArray, i);
	}

	inline const PMxArrayNative& operator[] (int i) const {
		return mxGetCell(m_pArray, i);
	}

	inline const PMxArrayNative* data() const {
		return (PMxArrayNative *) mxGetData(m_pArray);
	}

	inline PMxArrayNative* data() {
		return (PMxArrayNative *) mxGetData(m_pArray);
	}

	template <typename T>
	inline const std::vector<T> vectorize() const {
		int numElements = numel();
		std::vector<T> retArg;
		for (int iter = 0; iter < numElements; ++iter) {
			retArg.push_back(T(mxGetCell(m_pArray, iter)));
		}
		return retArg;
	}

	template <typename T>
	inline std::vector<T> vectorize() {
		int numElements = numel();
		std::vector<T> retArg;
		for (int iter = 0; iter < numElements; ++iter) {
			retArg.push_back(T(mxGetCell(m_pArray, iter)));
		}
		return retArg;
	}
};

}	/* name space mex */

#endif /* MEX_UTILS_H_ */
