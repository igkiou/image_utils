/*
 * exrset.cpp
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

void setSingleAttribute(Header& head, const char attrName[], const mxArray* mxarr) {

	if (!strcmp(attrName, "gain")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'gain', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addGain(head, value);
	} else if (!strcmp(attrName, "wavelength")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'wavelength', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addWavelength(head, value);
	} else if (!strcmp(attrName, "extTube")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'extTube', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addExtTube(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "lens")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'lens', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addLens(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "material")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'material', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addMaterial(head, std::string(stringValue));
		mxFree(stringValue);
	} /* else if (!strcmp(attrName, "chromaticities")) {

		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'material', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addMaterial(head, std::string(stringValue));
		mxFree(stringValue);
	}*/ else if (!strcmp(attrName, "whiteLuminance")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'whiteLuminance', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addWhiteLuminance(head, value);
	} else if (!strcmp(attrName, "adoptedNeutral")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 2))) {
			mexErrMsgTxt("For setting attribute 'adoptedNeutral', a single array with 2 elements must be provided.");
		}
		const float *value = (float *) mxGetData(mxarr);
		addAdoptedNeutral(head, V2f(value[0], value[1]));
	} else if (!strcmp(attrName, "renderingTransform")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'renderingTransform', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addRenderingTransform(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "lookModTransform")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'lookModTransform', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addLookModTransform(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "xDensity")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'xDensity', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addXDensity(head, value);
	} else if (!strcmp(attrName, "owner")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'owner', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addOwner(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "comments")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'comments', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addComments(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "capDate")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'capDate', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addCapDate(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "utcOffset")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'utcOffset', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addUtcOffset(head, value);
	} else if (!strcmp(attrName, "longitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'longitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addLongitude(head, value);
	} else if (!strcmp(attrName, "latitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'latitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addLatitude(head, value);
	} else if (!strcmp(attrName, "altitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'altitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addAltitude(head, value);
	} else if (!strcmp(attrName, "focus")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'focus', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addFocus(head, value);
	} else if (!strcmp(attrName, "expTime")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'expTime', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addExpTime(head, value);
	} else if (!strcmp(attrName, "aperture")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'aperture', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addAperture(head, value);
	} else if (!strcmp(attrName, "isoSpeed")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'isoSpeed', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		addIsoSpeed(head, value);
	} else if (!strcmp(attrName, "envmap")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'envmap', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		addEnvmap(head, stringToEnvmapType(stringValue));
		mxFree(stringValue);
	} else {
		mexErrMsgTxt("Setting this attribute not implemented.");
	}
}

void setMultipleAttributes(Header& head, mxArray* mxstruct) {

	mxArray* matStruct;
	int numAttributes = 0;
	for (Header::ConstIterator attIt = head.begin(); attIt != head.end(); \
		++attIt, ++numAttributes);

	const char **attributeNames = (const char **) mxMalloc(numAttributes * sizeof(*attributeNames));
	int iterField = 0;
	for (Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
//		std::cout << attIt.name() << ": " << attIt.attribute().typeName() << ", " << getAttrTypeIndex(attIt.attribute().typeName()) << std::endl;
		attributeNames[iterField] = attIt.name();
	}
	matStruct = mxCreateStructMatrix(1, 1, numAttributes, attributeNames);
	mxFree((void *) attributeNames);
	iterField = 0;
	for (Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
		mxSetFieldByNumber(matStruct, 0, iterField, const_cast<mxArray*>(attributeToMxArray(attIt.attribute())));
	}
	return matStruct;
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs < 3) {
		mexErrMsgTxt("At least three input arguments are required.");
	} else if (nrhs > 4) {
		mexErrMsgTxt("Four or fewer input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	const int lengthInFileName = mxGetNumberOfElements(prhs[0]) + 1;
	char* inFileName = (char *) mxMalloc(lengthInFileName * sizeof(char));
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

	InputFile in(inFileName);
	Header head = in.header();

	if (nrhs == 4) {
		if (!mxIsChar(prhs[2])) {
			mexErrMsgTxt("Third argument must be a string.");
		}
		const int lengthAttributeName = mxGetNumberOfElements(prhs[2]) + 1;
		char* attributeName = (char *) mxMalloc(lengthAttributeName * sizeof(char));
		mxGetString(prhs[2], attributeName, lengthAttributeName);
		setAttribute(head, attributeName, prhs[3]);
	} else {
		if (!mxIsStruct(prhs[2])) {
			mexErrMsgTxt("Third argument must be a struct.");
		}
		setMultipleAttributes(head, prhs[3]);
	}
	OutputFile out(outFileName, head);
	out.copyPixels(in);
	mxFree(inFileName);
	if (!mxIsEmpty(prhs[1])) {
		mxFree(outFileName);
	}
}
