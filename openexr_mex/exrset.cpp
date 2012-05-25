/*
 * exrset.cpp
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

/* TODO: Replace TypedAttribute<type> with named TypedAttribute types. */
const Attribute & mxArrayToAttribute(const mxArray* mxarr) {
	if (mxIsChar(mxarr)) {
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char stringValue[lengthStringValue];
		mxGetString(mxarr, stringValue, lengthStringValue);
		return TypedAttribute<std::string>(std::string(stringValue));
	} else if (mxIsInt32(mxarr)) {
		int* data = (int *) mxGetData(mxarr);
		const int length = mxGetNumberOfElements(mxarr);
		if (length == 1) {
			return TypedAttribute<int>(*data);
		} else if (length == 2) {
			return TypedAttribute<V2i>(V2i(data[0], data[1]));
		} else if (length == 3) {
			return TypedAttribute<V3i>(V3i(data[0], data[1], data[2]));
		} else {
			return TypedAttribute<std::vector<int> >(std::vector<int>(data, data + length));
		}
	} else if (mxIsSingle(mxarr)) {
		float* data = (float *) mxGetData(mxarr);
		const int length = mxGetNumberOfElements(mxarr);
		if (length == 1) {
			return TypedAttribute<float>(*data);
		} else if (length == 2) {
			return TypedAttribute<V2f>(V2f(data[0], data[1]));
		} else if (length == 3) {
			return TypedAttribute<V3f>(V3f(data[0], data[1], data[2]));
		} else {
			return TypedAttribute<std::vector<float> >(std::vector<float>(data, data + length));
		}
	} else if (mxIsDouble(mxarr)) {
		double* data = (double *) mxGetData(mxarr);
		const int length = mxGetNumberOfElements(mxarr);
		if (length == 1) {
			return TypedAttribute<double>(*data);
//		} else if (length == 2) {
//			return TypedAttribute<V2d>(V2d(data[0], data[1]));
//		} else if (length == 3) {
//			return TypedAttribute<V3d>(V3d(data[0], data[1], data[2]));
		} else {
			return TypedAttribute<std::vector<double> >(std::vector<double>(data, data + length));
		}
	} else {
		mexErrMsgTxt("Direct inference of attribute type is only implemented for arrays of type char, int32, single, and double.");
	}
}

const Attribute & mxArrayToAttribute(const mxArray* mxarr, const char* typeName) {
	switch (stringToAttrType(typeName)) {
		case ATTR_ENVMAP:
		{
			if (mxIsChar(mxarr)) {
				const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
				char stringValue[lengthStringValue];
				mxGetString(mxarr, stringValue, lengthStringValue);
				return TypedAttribute<Envmap>(stringToEnvmapType(std::string(stringValue).c_str()));
			} else {
				mexErrMsgTxt("For attribute of type 'envmap', a string array must be provided.");
			}
		}
		/*case ATTR_CHROMATICITIES:
		{
			const TypedAttribute<Chromaticities>& chromaAttr = static_cast<const TypedAttribute<Chromaticities>& >(attr);
			int dims[2] = {4, 2};
			mxArray* temp = mxCreateCellArray(2, dims);
			mxSetCell(temp, 0, mxCreateString("red"));
			mxSetCell(temp, 1, mxCreateString("green"));
			mxSetCell(temp, 2, mxCreateString("blue"));
			mxSetCell(temp, 3, mxCreateString("white"));
			mxArray* point;
			float *pointData;
			point = mxCreateNumericMatrix(1, 2, mxSINGLE_CLASS, mxREAL);
			pointData = (float *) mxGetData(point);
			pointData[0] = chromaAttr.value().red.x;
			pointData[1] = chromaAttr.value().red.y;
			mxSetCell(temp, 4, point);
			point = mxCreateNumericMatrix(1, 2, mxSINGLE_CLASS, mxREAL);
			pointData = (float *) mxGetData(point);
			pointData[0] = chromaAttr.value().green.x;
			pointData[1] = chromaAttr.value().green.y;
			mxSetCell(temp, 5, point);
			point = mxCreateNumericMatrix(1, 2, mxSINGLE_CLASS, mxREAL);
			pointData = (float *) mxGetData(point);
			pointData[0] = chromaAttr.value().blue.x;
			pointData[1] = chromaAttr.value().blue.y;
			mxSetCell(temp, 6, point);
			point = mxCreateNumericMatrix(1, 2, mxSINGLE_CLASS, mxREAL);
			pointData = (float *) mxGetData(point);
			pointData[0] = chromaAttr.value().white.x;
			pointData[1] = chromaAttr.value().white.y;
			mxSetCell(temp, 7, point);
			return temp;
		}*/
		case ATTR_STRING:
		{
			if (mxIsChar(mxarr)) {
				const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
				char stringValue[lengthStringValue];
				mxGetString(mxarr, stringValue, lengthStringValue);
				return TypedAttribute<std::string>(std::string(stringValue));
			} else {
				mexErrMsgTxt("For attribute of type 'string', a string array must be provided.");
			}
		}
//		case ATTR_V2D:
//		{
//			if (mxIsDouble(mxarr) && (mxGetNumberOfElements(mxarr) == 2)) {
//				double* data = (double *) mxGetData(mxarr);
//				return TypedAttribute<V2d>(V2d(data[0], data[1]));
//			} else {
//				mexErrMsgTxt("For attribute of type 'v2d', a double array with 2 elements must be provided.");
//			}
//		}
		case ATTR_V2F:
		{
			if (mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 2)) {
				float* data = (float *) mxGetData(mxarr);
				return TypedAttribute<V2f>(V2f(data[0], data[1]));
			} else {
				mexErrMsgTxt("For attribute of type 'v2f', a single array with 2 elements must be provided.");
			}
		}
		case ATTR_V2I:
		{
			if (mxIsInt32(mxarr) && (mxGetNumberOfElements(mxarr) == 2)) {
				int* data = (int *) mxGetData(mxarr);
				return TypedAttribute<V2i>(V2d(data[0], data[1]));
			} else {
				mexErrMsgTxt("For attribute of type 'v2i', an integer array with 2 elements must be provided.");
			}
		}
		case ATTR_VD:
		{
			if (mxIsDouble(mxarr)) {
				double* data = (double *) mxGetData(mxarr);
				const int length = mxGetNumberOfElements(mxarr);
				return TypedAttribute<std::vector<double> >(std::vector<double>(data, data + length));
			} else {
				mexErrMsgTxt("For attribute of type 'vd', a double array must be provided.");
			}
		}
		case ATTR_VF:
		{
			if (mxIsSingle(mxarr)) {
				float* data = (float *) mxGetData(mxarr);
				const int length = mxGetNumberOfElements(mxarr);
				return TypedAttribute<std::vector<float> >(std::vector<float>(data, data + length));
			} else {
				mexErrMsgTxt("For attribute of type 'vf', a single array must be provided.");
			}
		}
		case ATTR_VI:
		{
			if (mxIsInt32(mxarr)) {
				int* data = (int *) mxGetData(mxarr);
				const int length = mxGetNumberOfElements(mxarr);
				return TypedAttribute<std::vector<int> >(std::vector<int>(data, data + length));
			} else {
				mexErrMsgTxt("For attribute of type 'vi', an integer array must be provided.");
			}
		}
		case ATTR_DOUBLE:
		{
			if (mxIsDouble(mxarr) && (mxGetNumberOfElements(mxarr) == 1)) {
				double* data = (double *) mxGetData(mxarr);
				return TypedAttribute<double>(*data);
			} else {
				mexErrMsgTxt("For attribute of type 'double', a double array with 1 element must be provided.");
			}
		}
		case ATTR_FLOAT:
		{
			if (mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1)) {
				float* data = (float *) mxGetData(mxarr);
				return TypedAttribute<float>(*data);
			} else {
				mexErrMsgTxt("For attribute of type 'double', a single array with 1 element must be provided.");
			}
		}
		case ATTR_INT:
		{
			if (mxIsInt32(mxarr) && (mxGetNumberOfElements(mxarr) == 1)) {
				int* data = (int *) mxGetData(mxarr);
				return TypedAttribute<int>(*data);
			} else {
				mexErrMsgTxt("For attribute of type 'int', an integer array with 1 element must be provided.");
			}
		}
		default:
			mexErrMsgTxt("Setting attribute of this type not implemented.");
	}
}

void setAttribute(const char inFileName[], const char outFileName[], const char attrName[], const mxArray* mxarr) {
	InputFile in(inFileName);

	Header head = in.header();
	const Attribute& attr = mxArrayToAttribute(mxarr);
//	addFocus(head, 10);
	head.insert(attrName, attr);
//	head.insert(attrName, mxArrayToAttribute(mxarr));
	OutputFile out(outFileName, head);
	out.copyPixels(in);
}

void setAttribute(const char inFileName[], const char outFileName[], const char attrName[], const mxArray* mxarr, const char attrTypeName[]) {
	InputFile in(inFileName);

	Header head = in.header();
	head.insert(attrName, mxArrayToAttribute(mxarr, attrTypeName));
	OutputFile out(outFileName, head);
	out.copyPixels(in);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs < 4) {
		mexErrMsgTxt("At least four input arguments are required.");
	} else if (nrhs > 5) {
		mexErrMsgTxt("Fewer than five input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	const int lengthInFileName = mxGetNumberOfElements(prhs[0]) + 1;
	char inFileName[lengthInFileName];
	mxGetString(prhs[0], inFileName, lengthInFileName);

	char *outFileName;
	if (!mxIsEmpty(prhs[1])) {
		if (!mxIsChar(prhs[1])) {
			mexErrMsgTxt("First argument must be a string.");
		}
		const int lengthOutFileName = mxGetNumberOfElements(prhs[1]) + 1;
		outFileName = (char *) mxMalloc(lengthOutFileName * sizeof(char));
		mxGetString(prhs[1], outFileName, lengthOutFileName);
	} else {
		outFileName = inFileName;
	}

	if (!mxIsChar(prhs[2])) {
		mexErrMsgTxt("Third argument must be a string.");
	}
	const int lengthAttributeName = mxGetNumberOfElements(prhs[2]) + 1;
	char attributeName[lengthAttributeName];
	mxGetString(prhs[2], attributeName, lengthAttributeName);

	if (nrhs < 5) {
		setAttribute(inFileName, outFileName, attributeName, prhs[3]);
	} else if (nrhs >= 5) {
		if (!mxIsChar(prhs[4])) {
			mexErrMsgTxt("Fifth argument must be a string.");
		}
		const int lengthAttributeTypeName = mxGetNumberOfElements(prhs[4]) + 1;
		char attributeTypeName[lengthAttributeTypeName];
		mxGetString(prhs[4], attributeTypeName, lengthAttributeTypeName);
		setAttribute(inFileName, outFileName, attributeName, prhs[3], attributeTypeName);
	}

	if (!mxIsEmpty(prhs[1])) {
		mxFree(outFileName);
	}
}
