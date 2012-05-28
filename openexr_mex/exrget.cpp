/*
 * exrget.cpp
 *
 *  Created on: May 22, 2012
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
			const TypedAttribute<std::vector<float> >& vfAttr = static_cast<const TypedAttribute<std::vector<float> >& >(attr);
			int vecLength = vfAttr.value().size();
			mxArray* temp = mxCreateNumericMatrix(vecLength, 1, mxSINGLE_CLASS, mxREAL);
			float *data = (float *) mxGetData(temp);
			for (int iterVec = 0; iterVec < vecLength; ++iterVec) {
				data[iterVec] = vfAttr.value()[iterVec];
			}
			return temp;
		}
		case ATTR_VI:
		{
			const TypedAttribute<std::vector<int> >& vfAttr = static_cast<const TypedAttribute<std::vector<int> >& >(attr);
			int vecLength = vfAttr.value().size();
			mxArray* temp = mxCreateNumericMatrix(vecLength, 1, mxINT32_CLASS, mxREAL);
			int *data = (int *) mxGetData(temp);
			for (int iterVec = 0; iterVec < vecLength; ++iterVec) {
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

mxArray* getSingleAttribute(const Header& head, const char attributeName[]) {
	const Attribute& attr = head[attributeName];
	return const_cast<mxArray *>(attributeToMxArray(attr));
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs < 1) {
		mexErrMsgTxt("At least one input argument is required.");
	} else if (nrhs > 2) {
		mexErrMsgTxt("Two or fewer input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	if (!mxIsChar(prhs[0])) {
		mexErrMsgTxt("First argument must be a string.");
	}
	const int lengthFileName = mxGetNumberOfElements(prhs[0]) + 1;
	char* fileName = (char *) mxMalloc(lengthFileName * sizeof(char));
	mxGetString(prhs[0], fileName, lengthFileName);
	InputFile in(fileName);
	Header head = in.header();

	if (nrhs >= 2) {
		if (!mxIsChar(prhs[1])) {
			mexErrMsgTxt("Second argument must be a string.");
		}
		const int lengthAttributeName = mxGetNumberOfElements(prhs[1]) + 1;
		char* attributeName = (char *) mxMalloc(lengthAttributeName * sizeof(char));
		mxGetString(prhs[1], attributeName, lengthAttributeName);
		plhs[0] = getSingleAttribute(head, attributeName);
		mxFree(attributeName);
	} else {
		plhs[0] = getAllAttributes(head);
	}

	mxFree(fileName);
}
