/*
 * image_utils//image_utils/openexr_mex/test_exr.cpp/test_exr.cpp
 *
 *  Created on: Feb 27, 2013
 *      Author: igkiou
 */

#include "openexr_mex.h"
#include "mex_utils.h"

enum class EExrAttributeType {
	EChannelList = 0,
	ECompression,
	ELineOrder,
	EChromaticities,
	EEnvmap,
	EString,
	EBox2f,
	EBox2i,
	EV2f,
	EV2i,
	EDouble,
	EFloat,
	EInt,
	ELength,
	EInvalid = -1
};

mex::ConstBiMap<EExrAttributeType, std::string> attributeTypeNameMap =
	mex::ConstBiMap<EExrAttributeType, std::string>
	(EExrAttributeType::EChannelList, std::string(Imf::ChannelListAttribute::staticTypeName()))
	(EExrAttributeType::ECompression, std::string(Imf::CompressionAttribute::staticTypeName()))
	(EExrAttributeType::ELineOrder, std::string(Imf::LineOrderAttribute::staticTypeName()))
	(EExrAttributeType::EChromaticities, std::string(Imf::ChromaticitiesAttribute::staticTypeName()))
	(EExrAttributeType::EEnvmap, std::string(Imf::EnvmapAttribute::staticTypeName()))
	(EExrAttributeType::EString, std::string(Imf::StringAttribute::staticTypeName()))
	(EExrAttributeType::EBox2f, std::string(Imf::Box2fAttribute::staticTypeName()))
	(EExrAttributeType::EBox2i, std::string(Imf::Box2iAttribute::staticTypeName()))
	(EExrAttributeType::EV2f, std::string(Imf::V2fAttribute::staticTypeName()))
	(EExrAttributeType::EV2i, std::string(Imf::V2iAttribute::staticTypeName()))
	(EExrAttributeType::EDouble, std::string(Imf::TypedAttribute<double>::staticTypeName()))
	(EExrAttributeType::EFloat, std::string(Imf::FloatAttribute::staticTypeName()))
	(EExrAttributeType::EInt, std::string(Imf::TypedAttribute<int>::staticTypeName()))
	(EExrAttributeType::EInvalid, std::string("unknown"));

const mex::ConstMap<int, EExrAttributeType> intAttributeTypeMap
	= mex::ConstMap<int, EExrAttributeType>
	(5,		EExrAttributeType::EChannelList)
	(6,		EExrAttributeType::ECompression)
	(7,		EExrAttributeType::ELineOrder)
	(8,		EExrAttributeType::EChromaticities)
	(9,		EExrAttributeType::EEnvmap)
	(10,	EExrAttributeType::EString)
	(11,	EExrAttributeType::EBox2f)
	(12,	EExrAttributeType::EBox2i)
	(13,	EExrAttributeType::EV2f)
	(14,	EExrAttributeType::EV2i)
	(15,	EExrAttributeType::EDouble)
	(16,	EExrAttributeType::EFloat)
	(17,	EExrAttributeType::EInt)
	(18,	EExrAttributeType::EInvalid);

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

	mex::MxString fileName(const_cast<mxArray *>(prhs[0]));
	openexr::ExrInputFile file(fileName);
	mexAssert(intAttributeTypeMap[6] == EExrAttributeType::ECompression);
	for (std::map<int, EExrAttributeType>::const_iterator iter = intAttributeTypeMap.get_map().begin(),
		end = intAttributeTypeMap.get_map().end();
		iter != end;
		++iter) {
		mexPrintf("%d %d\n", iter->first, iter->second);
	}
	mexAssert(attributeTypeNameMap.find(std::string("compression")) == EExrAttributeType::ECompression);
	for (std::map<std::string, EExrAttributeType>::const_iterator iter = attributeTypeNameMap.get_mapRightToLeft().begin(),
		end = attributeTypeNameMap.get_mapRightToLeft().end();
		iter != end;
		++iter) {
		mexPrintf("%s %d\n", iter->first.c_str(), iter->second);
	}
	mexPrintf("%d %s %d %d %d %d %d\n", attributeTypeNameMap.find(std::string("compression")),
							attributeTypeNameMap.get_mapRightToLeft().find("lineOrder")->first.c_str(),
							attributeTypeNameMap.get_mapRightToLeft().find("lineOrder")->second,
							attributeTypeNameMap.find(std::string("chlist")),
							attributeTypeNameMap.find(std::string("envmap")),
							attributeTypeNameMap.find(std::string("unknown")),
							EExrAttributeType::EInvalid);
//	file.get

}
