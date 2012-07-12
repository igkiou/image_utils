/*
 * openexr_mex.cpp
 *
 *  Created on: May 28, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

const mxArray * attributeToMxArray(const Attribute & attr) {
	switch (stringToAttrType(attr.typeName())) {
		case ATTR_CHLIST:
		{
			const TypedAttribute<ChannelList>& chlistAttr = static_cast<const TypedAttribute<ChannelList>& >(attr);
			mwSize numChannels = 0;
			for (ChannelList::ConstIterator chIt = chlistAttr.value().begin(); \
				chIt != chlistAttr.value().end(); ++chIt, ++numChannels);
			mxArray* temp = mxCreateCellArray(1, &numChannels);
			int iterCh = 0;
			for (ChannelList::ConstIterator chIt = chlistAttr.value().begin(); \
				chIt != chlistAttr.value().end(); ++chIt, ++iterCh) {
				mxSetCell(temp, iterCh, mxCreateString(chIt.name()));
			}
			return temp;
		}
		case ATTR_COMPRESSION:
		{
			const TypedAttribute<Compression>& comprAttr = static_cast<const TypedAttribute<Compression>& >(attr);
			return mxCreateString(compressionTypeToString(comprAttr.value()));
		}
		case ATTR_LINEORDER:
		{
			const TypedAttribute<LineOrder>& linOrdAttr = static_cast<const TypedAttribute<LineOrder>& >(attr);
			return mxCreateString(lineOrderTypeToString(linOrdAttr.value()));
		}
		case ATTR_CHROMATICITIES:
		{
			const TypedAttribute<Chromaticities>& chromaAttr = static_cast<const TypedAttribute<Chromaticities>& >(attr);
			const mwSize dims[2] = {4, 2};
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
		}
		case ATTR_ENVMAP:
		{
			const TypedAttribute<Envmap>& envmapAttr = static_cast<const TypedAttribute<Envmap>& >(attr);
			return mxCreateString(envmapTypeToString(envmapAttr.value()));
		}
		case ATTR_STRING:
		{
			const TypedAttribute<std::string>& stringAttr = static_cast<const TypedAttribute<std::string>& >(attr);
			return mxCreateString(stringAttr.value().c_str());
		}
		case ATTR_BOX2F:
		{
			const TypedAttribute<Box2f>& box2fAttr = static_cast<const TypedAttribute<Box2f>& >(attr);
			const mwSize numPoints = 2;
			mxArray* temp = mxCreateCellArray(1, &numPoints);
			mxArray* point;
			float *pointData;
			point = mxCreateNumericMatrix(2, 1, mxSINGLE_CLASS, mxREAL);
			pointData = (float *) mxGetData(point);
			pointData[0] = box2fAttr.value().min.x;
			pointData[1] = box2fAttr.value().min.y;
			mxSetCell(temp, 0, point);
			point = mxCreateNumericMatrix(2, 1, mxSINGLE_CLASS, mxREAL);
			pointData = (float *) mxGetData(point);
			pointData[0] = box2fAttr.value().max.x;
			pointData[1] = box2fAttr.value().max.y;
			mxSetCell(temp, 1, point);
			return temp;
		}
		case ATTR_BOX2I:
		{
			const TypedAttribute<Box2i>& box2iAttr = static_cast<const TypedAttribute<Box2i>& >(attr);
			const mwSize numPoints = 2;
			mxArray* temp = mxCreateCellArray(1, &numPoints);
			mxArray* point;
			int *pointData;
			point = mxCreateNumericMatrix(2, 1, mxINT32_CLASS, mxREAL);
			pointData = (int *) mxGetData(point);
			pointData[0] = box2iAttr.value().min.x;
			pointData[1] = box2iAttr.value().min.y;
			mxSetCell(temp, 0, point);
			point = mxCreateNumericMatrix(2, 1, mxINT32_CLASS, mxREAL);
			pointData = (int *) mxGetData(point);
			pointData[0] = box2iAttr.value().max.x;
			pointData[1] = box2iAttr.value().max.y;
			mxSetCell(temp, 1, point);
			return temp;
		}
		case ATTR_V2F:
		{
			const TypedAttribute<V2f>& v2fAttr = static_cast<const TypedAttribute<V2f>& >(attr);
			mxArray* temp = mxCreateNumericMatrix(2, 1, mxSINGLE_CLASS, mxREAL);
			float *data = (float *) mxGetData(temp);
			data[0] = v2fAttr.value().x;
			data[1] = v2fAttr.value().y;
			return temp;
		}
		case ATTR_V2I:
		{
			const TypedAttribute<V2i>& v2iAttr = static_cast<const TypedAttribute<V2i>& >(attr);
			mxArray* temp = mxCreateNumericMatrix(2, 1, mxINT32_CLASS, mxREAL);
			int *data = (int *) mxGetData(temp);
			data[0] = v2iAttr.value().x;
			data[1] = v2iAttr.value().y;
			return temp;
		}
		case ATTR_VF:
		{
			const VectorAttribute<float>& vfAttr = static_cast<const VectorAttribute<float>& >(attr);
			const size_t vecLength = vfAttr.value().size();
			mexPrintf("%d\n", (int) vecLength);
//			std::cout << vecLength << std:endl;
			mxArray* temp = mxCreateNumericMatrix(5, 1, mxSINGLE_CLASS, mxREAL);
			float *data = (float *) mxGetData(temp);
			for (size_t iterVec = 0; iterVec < vecLength; ++iterVec) {
				data[iterVec] = vfAttr.value()[iterVec];
			}
			return temp;
		}
		case ATTR_VI:
		{
			const VectorAttribute<int>& vfAttr = static_cast<const VectorAttribute<int>& >(attr);
			const size_t vecLength = vfAttr.value().size();
			mxArray* temp = mxCreateNumericMatrix(vecLength, 1, mxINT32_CLASS, mxREAL);
			int *data = (int *) mxGetData(temp);
			for (size_t iterVec = 0; iterVec < vecLength; ++iterVec) {
				data[iterVec] = vfAttr.value()[iterVec];
			}
			return temp;
		}
		case ATTR_FLOAT:
		{
			const TypedAttribute<float>& floatAttr = static_cast<const TypedAttribute<float>& >(attr);
			mxArray *temp = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
			float *val = (float *) mxGetData(temp);
			*val = floatAttr.value();
			return temp;
		}
		case ATTR_INT:
		{
			const TypedAttribute<int>& intAttr = static_cast<const TypedAttribute<int>& >(attr);
			mxArray *temp = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
			int *val = (int *) mxGetData(temp);
			*val = (int) intAttr.value();
			return temp;
		}
		default:
			return NULL;
	}
}

mxArray* getAllAttributes(const Header& head) {

	mxArray* matStruct;
	int numAttributes = 0;
	for (Header::ConstIterator attIt = head.begin(); attIt != head.end(); \
		++attIt, ++numAttributes);

	const char **attributeNames = (const char **) mxMalloc(numAttributes * sizeof(*attributeNames));
	int iterField = 0;
	for (Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
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

mxArray* getSingleAttribute(const Header& head, const char attributeName[]) {
	const Attribute& attr = head[attributeName];
	return const_cast<mxArray *>(attributeToMxArray(attr));
}

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
	} else if (!strcmp(attrName, "multApertures")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multApertures', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		addMultApertures(head, value);
	} else if (!strcmp(attrName, "multExpTimes")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multExpTimes', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(lengthVector);
		for (size_t iter = 0; iter < lengthVector; ++iter) {
			value[iter] = vecData[iter];
		}
		addMultExpTimes(head, value);
	} else if (!strcmp(attrName, "multIsoSpeeds")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multIsoSpeeds', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		addMultIsoSpeeds(head, value);
	} else if (!strcmp(attrName, "multGains")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multGains', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		addMultGains(head, value);
	} else {
		mexErrMsgTxt("Setting this attribute not implemented.");
	}
}

void setMultipleAttributes(Header& head, const mxArray* mxstruct) {

	if (mxIsStruct(mxstruct)) {
		int numAttributes = mxGetNumberOfFields(mxstruct);
		for (int iterAttr = 0; iterAttr < numAttributes; ++iterAttr) {
			setSingleAttribute(head, mxGetFieldNameByNumber(mxstruct, iterAttr), \
							mxGetFieldByNumber(mxstruct, 0, iterAttr));
		}
	} else if (!mxIsEmpty(mxstruct)) {
		mexErrMsgTxt("For setting multiple attributes, a struct array must be provided.");
	}
}

void writeScanLine(OutputFile& file, \
		const USED *rPixels, \
		const USED *gPixels, \
		const USED *bPixels, \
		const USED *aPixels, \
		const size_t width,
		const size_t height) {
	FrameBuffer frameBuffer;
	frameBuffer.insert("R", Slice(USEDC, (char *) rPixels, \
			sizeof(*rPixels) * 1, sizeof(*rPixels) * width));
	frameBuffer.insert("G", Slice(USEDC, (char *) gPixels, \
			sizeof(*gPixels) * 1, sizeof(*gPixels) * width));
	frameBuffer.insert("B", Slice(USEDC, (char *) bPixels, \
			sizeof(*bPixels) * 1, sizeof(*bPixels) * width));
	if (aPixels != NULL) {
		frameBuffer.insert("A", Slice(USEDC, (char *) aPixels, \
			sizeof(*aPixels) * 1, sizeof(*aPixels) * width));
	}
	file.setFrameBuffer(frameBuffer);
	file.writePixels(height);
}

void readScanLine(InputFile& file, \
		Array2D<USED> &rPixels, bool &rFlag, \
		Array2D<USED> &gPixels, bool &gFlag, \
		Array2D<USED> &bPixels, bool &bFlag, \
		Array2D<USED> &aPixels, bool &aFlag, \
		size_t& width,
		size_t& height) {

	Box2i dw = file.header().dataWindow();
	width = dw.max.x - dw.min.x + 1;
	height = dw.max.y - dw.min.y + 1;

	FrameBuffer frameBuffer;

	if (file.header().channels().findChannel("R")) {
		rFlag = true;
		rPixels.resizeErase(height, width);
		frameBuffer.insert("R", Slice(USEDC, (char *) (&rPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		rFlag = false;
	}

	if (file.header().channels().findChannel("G")) {
		gFlag = true;
		gPixels.resizeErase(height, width);
		frameBuffer.insert("G", Slice(USEDC, (char *) (&gPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		gFlag = false;
	}

	if (file.header().channels().findChannel("B")) {
		bFlag = true;
		bPixels.resizeErase(height, width);
		frameBuffer.insert("B", Slice(USEDC, (char *) (&bPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		bFlag = false;
	}

	if (file.header().channels().findChannel("A")) {
		aFlag = true;
		aPixels.resizeErase(height, width);
		frameBuffer.insert("A", Slice(USEDC, (char *) (&aPixels[0][0] - dw.min.x - dw.min.y * width), \
		sizeof(rPixels[0][0]) * 1, sizeof(rPixels[0][0]) * width, 1, 1, FLT_MAX));
	} else {
		aFlag = false;
	}

	file.setFrameBuffer(frameBuffer);
	file.readPixels(dw.min.y, dw.max.y);
}
