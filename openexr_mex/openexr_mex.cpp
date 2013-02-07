/*
 * openexr_mex.cpp
 *
 *  Created on: May 28, 2012
 *      Author: igkiou
 */

#include "openexr_mex.h"

namespace exr {

mex::MxArray EXRInputFile::getAttribute(const std::string &attributeName) {
	const Imf::Attribute& attr = m_file.header[attributeName];
	return const_cast<mxArray *>(attributeToMxArray(attr));
}

mex::MxArray EXRInputFile::getAttribute(const Imf::Header& head) {

	mxArray* matStruct;
	int numAttributes = 0;
	for (Imf::Header::ConstIterator attIt = head.begin(); attIt != head.end(); \
		++attIt, ++numAttributes);

	const char **attributeNames = (const char **) mxMalloc(numAttributes * sizeof(*attributeNames));
	int iterField = 0;
	for (Imf::Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
		attributeNames[iterField] = attIt.name();
	}
	matStruct = mxCreateStructMatrix(1, 1, numAttributes, attributeNames);
	mxFree((void *) attributeNames);
	iterField = 0;
	for (Imf::Header::ConstIterator attIt = head.begin(); attIt != head.end(); ++attIt, ++iterField) {
		mxSetFieldByNumber(matStruct, 0, iterField, const_cast<mxArray*>(attributeToMxArray(attIt.attribute())));
	}
	return matStruct;
}

void setAttribute(Imf::Header& head, const char attrName[], const mxArray* mxarr) {

	if (!strcmp(attrName, "gain")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'gain', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addGain(head, value);
	} else if (!strcmp(attrName, "wavelength")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'wavelength', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addWavelength(head, value);
	} else if (!strcmp(attrName, "extTube")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'extTube', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addExtTube(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "lens")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'lens', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addLens(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "material")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'material', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addMaterial(head, std::string(stringValue));
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
		Imf::addWhiteLuminance(head, value);
	} else if (!strcmp(attrName, "adoptedNeutral")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 2))) {
			mexErrMsgTxt("For setting attribute 'adoptedNeutral', a single array with 2 elements must be provided.");
		}
		const float *value = (float *) mxGetData(mxarr);
		Imf::addAdoptedNeutral(head, V2f(value[0], value[1]));
	} else if (!strcmp(attrName, "renderingTransform")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'renderingTransform', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addRenderingTransform(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "lookModTransform")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'lookModTransform', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addLookModTransform(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "xDensity")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'xDensity', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addXDensity(head, value);
	} else if (!strcmp(attrName, "owner")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'owner', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addOwner(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "comments")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'comments', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addComments(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "capDate")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'capDate', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addCapDate(head, std::string(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "utcOffset")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'utcOffset', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addUtcOffset(head, value);
	} else if (!strcmp(attrName, "longitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'longitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addLongitude(head, value);
	} else if (!strcmp(attrName, "latitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'latitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addLatitude(head, value);
	} else if (!strcmp(attrName, "altitude")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'altitude', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addAltitude(head, value);
	} else if (!strcmp(attrName, "focus")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'focus', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addFocus(head, value);
	} else if (!strcmp(attrName, "expTime")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'expTime', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addExpTime(head, value);
	} else if (!strcmp(attrName, "aperture")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'aperture', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addAperture(head, value);
	} else if (!strcmp(attrName, "isoSpeed")) {
		if (!(mxIsSingle(mxarr) && (mxGetNumberOfElements(mxarr) == 1))) {
			mexErrMsgTxt("For setting attribute 'isoSpeed', a single array with 1 element must be provided.");
		}
		const float value = * (float *) mxGetData(mxarr);
		Imf::addIsoSpeed(head, value);
	} else if (!strcmp(attrName, "envmap")) {
		if (!mxIsChar(mxarr)) {
			mexErrMsgTxt("For setting attribute 'envmap', a string array must be provided.");
		}
		const int lengthStringValue = mxGetNumberOfElements(mxarr) + 1;
		char *stringValue = (char *) mxMalloc(lengthStringValue * sizeof(char));
		mxGetString(mxarr, stringValue, lengthStringValue);
		Imf::addEnvmap(head, stringToEnvmapType(stringValue));
		mxFree(stringValue);
	} else if (!strcmp(attrName, "multApertures")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multApertures', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		Imf::addMultApertures(head, value);
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
		Imf::addMultExpTimes(head, value);
	} else if (!strcmp(attrName, "multIsoSpeeds")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multIsoSpeeds', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		Imf::addMultIsoSpeeds(head, value);
	} else if (!strcmp(attrName, "multGains")) {
		if (!mxIsSingle(mxarr)) {
			mexErrMsgTxt("For setting attribute 'multGains', a single array must be provided.");
		}
		const size_t lengthVector = mxGetNumberOfElements(mxarr);
		float *vecData = (float *) mxGetData(mxarr);
		std::vector<float> value(vecData, vecData + lengthVector);
		Imf::addMultGains(head, value);
	} else {
		mexErrMsgTxt("Setting this attribute not implemented.");
	}
}

void setAttribute(Imf::Header& head, const mxArray* mxstruct) {

	if (mxIsStruct(mxstruct)) {
		int numAttributes = mxGetNumberOfFields(mxstruct);
		for (int iterAttr = 0; iterAttr < numAttributes; ++iterAttr) {
			setAttribute(head, mxGetFieldNameByNumber(mxstruct, iterAttr), \
							mxGetFieldByNumber(mxstruct, 0, iterAttr));
		}
	} else if (!mxIsEmpty(mxstruct)) {
		mexErrMsgTxt("For setting multiple attributes, a struct array must be provided.");
	}
}

void EXRInputFile::readChannelRGBA(Imf::Array2D<USED> &rPixels, bool &rFlag, \
								Imf::Array2D<USED> &gPixels, bool &gFlag, \
								Imf::Array2D<USED> &bPixels, bool &bFlag, \
								Imf::Array2D<USED> &aPixels, bool &aFlag) const {

	int width, height;
	getDimensions(width, height);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	if (m_file.header().channels().findChannel("R")) {
		rFlag = true;
		rPixels.resizeErase(height, width);
		frameBuffer.insert("R", \
						Imf::Slice(USEDC, \
								(char *) (&rPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(rPixels[0][0]) * 1, \
								sizeof(rPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		rFlag = false;
	}

	if (m_file.header().channels().findChannel("G")) {
		gFlag = true;
		gPixels.resizeErase(height, width);
		frameBuffer.insert("G", \
						Imf::Slice(USEDC, \
								(char *) (&gPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(gPixels[0][0]) * 1, \
								sizeof(gPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		gFlag = false;
	}

	if (m_file.header().channels().findChannel("B")) {
		bFlag = true;
		bPixels.resizeErase(height, width);
		frameBuffer.insert("B", \
						Imf::Slice(USEDC, \
								(char *) (&bPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(bPixels[0][0]) * 1, \
								sizeof(bPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		bFlag = false;
	}

	if (m_file.header().channels().findChannel("A")) {
		aFlag = true;
		aPixels.resizeErase(height, width);
		frameBuffer.insert("A", \
						Imf::Slice(USEDC, \
								(char *) (&aPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(aPixels[0][0]) * 1, \
								sizeof(aPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		aFlag = false;
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);

}

void EXRInputFile::readChannelYA(Imf::Array2D<USED> &yPixels, bool &yFlag, \
								Imf::Array2D<USED> &aPixels, bool &aFlag) const {

	int width, height;
	getDimensions(width, height);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	if (m_file.header().channels().findChannel("Y")) {
		yFlag = true;
		yPixels.resizeErase(height, width);
		frameBuffer.insert("R", \
						Imf::Slice(USEDC, \
								(char *) (&yPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(yPixels[0][0]) * 1, \
								sizeof(yPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		yFlag = false;
	}

	if (m_file.header().channels().findChannel("A")) {
		aFlag = true;
		aPixels.resizeErase(height, width);
		frameBuffer.insert("A", \
						Imf::Slice(USEDC, \
								(char *) (&aPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(aPixels[0][0]) * 1, \
								sizeof(aPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		aFlag = false;
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);

}

void EXRInputFile::readChannel(Imf::Array2D<USED> &cPixels, bool &cFlag, \
							const std::string &cName) const {

	int width, height;
	getDimensions(width, height);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	if (m_file.header().channels().findChannel(cName.c_str())) {
		cFlag = true;
		cPixels.resizeErase(height, width);
		frameBuffer.insert("R", \
						Imf::Slice(USEDC, \
								(char *) (&cPixels[0][0] - dw.min.x - dw.min.y * width), \
								sizeof(cPixels[0][0]) * 1, \
								sizeof(cPixels[0][0]) * width, \
								1, \
								1, \
								FLT_MAX));
	} else {
		cFlag = false;
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);

}


void EXRInputFile::readChannel(std::vector<Imf::Array2D<USED> > &cPixels, \
							std::vector<bool> &cFlags, \
							const std::vector<std::string> &cNames) const {

	int width, height;
	getDimensions(width, height);
	int numChannels = cNames.size();
	cPixels.resize(numChannels);
	cFlags.resize(numChannels, false);
	Imf::FrameBuffer frameBuffer;
	Imath::Box2i dw = m_file.header().dataWindow();

	for (int iter = 0; iter < numChannels; ++iter) {
		if (m_file.header().channels().findChannel(cNames[iter].c_str())) {
			cFlags[iter] = true;
			cPixels[iter].resizeErase(height, width);
			frameBuffer.insert(cNames[iter].c_str(), \
							Imf::Slice(USEDC, \
									(char *) (&cPixels[iter][0][0] - dw.min.x - dw.min.y * width), \
									sizeof(cPixels[iter][0][0]) * 1, \
									sizeof(cPixels[iter][0][0]) * width, \
									1, \
									1, \
									FLT_MAX));
		}
	}

	m_file.setFrameBuffer(frameBuffer);
	m_file.readPixels(dw.min.y, dw.max.y);
}

void EXROutputFile::writeChannelRGBA(const USED *rPixels, \
										const USED *gPixels, \
										const USED *bPixels, \
										const USED *aPixels) {

	std::vector<const USED *> cPixels;
	cPixels.push_back(rPixels);
	cPixels.push_back(gPixels);
	cPixels.push_back(bPixels);
	cPixels.push_back(aPixels);
	std::vector<const std::string> cNames;
	cNames.push_back(std::string("R"));
	cNames.push_back(std::string("G"));
	cNames.push_back(std::string("B"));
	cNames.push_back(std::string("A"));
	writeChannel(cPixels, cNames);
}

void EXROutputFile::writeChannelYA(const USED *yPixels, \
										const USED *aPixels) {

	std::vector<const USED *> cPixels;
	cPixels.push_back(yPixels);
	cPixels.push_back(aPixels);
	std::vector<const std::string> cNames;
	cNames.push_back(std::string("Y"));
	cNames.push_back(std::string("A"));
	writeChannel(cPixels, cNames);
}

void EXROutputFile::writeChannel(const USED *cPixels, \
									const std::string &cName) {

	std::vector<const USED *> ccPixels;
	ccPixels.push_back(cPixels);
	std::vector<const std::string> cNames;
	cNames.push_back(cName);
	writeChannel(ccPixels, cNames);
}

void EXROutputFile::writeChannel(const std::vector<const USED *> &cPixels, \
								const std::vector<const std::string> &cNames) {
	Assert((!m_createdFrameBuffer) && (!m_wroteFile));
	Assert(cPixels.size() == cNames.size());
	int channels = cPixels.size();
	int width, height;
	getDimensions(width, height);
	for (int iterChannel = 0; iterChannel < channels; ++iterChannel) {
		if (cPixels[iterChannel] != NULL) {
			m_frameBuffer.insert(cNames[iterChannel].c_str(), \
					Imf::Slice(USEDC, (char *) cPixels[iterChannel], \
					sizeof(*cPixels[iterChannel]) * 1, sizeof(*cPixels[iterChannel]) * width));
		}
	}
	m_createdFrameBuffer = true;
}

}	/* namespace nuance */
