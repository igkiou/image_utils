/*
 * image_utils//image_utils/openexr_mex/test_exr.cpp/test_exr.cpp
 *
 *  Created on: Feb 27, 2013
 *      Author: igkiou
 */

#include "openexr_mex.h"
#include "mex_utils.h"

//const mex::ConstBiMap<std::string, exr::EAttributeType> stringToAttributeType
//	= mex::ConstBiMap<std::string, exr::EAttributeType>
//	(std::string(Imf::ChannelListAttribute::staticTypeName()),		exr::EAttributeChannelList)
//	(std::string(Imf::CompressionAttribute::staticTypeName()),		exr::EAttributeCompression)
//	(std::string(Imf::LineOrderAttribute::staticTypeName()),			exr::EAttributeLineOrder)
//	(std::string(Imf::ChromaticitiesAttribute::staticTypeName()),	exr::EAttributeChromaticities)
//	(std::string(Imf::EnvmapAttribute::staticTypeName()),			exr::EAttributeEnvmap)
//	(std::string(Imf::StringAttribute::staticTypeName()),			exr::EAttributeString)
//	(std::string(Imf::Box2fAttribute::staticTypeName()),				exr::EAttributeBox2f)
//	(std::string(Imf::Box2iAttribute::staticTypeName()),				exr::EAttributeBox2i)
//	(std::string(Imf::V2fAttribute::staticTypeName()),				exr::EAttributeV2f)
//	(std::string(Imf::V2iAttribute::staticTypeName()),				exr::EAttributeV2i)
//	(std::string(Imf::VfAttribute::staticTypeName()),				exr::EAttributeVectorf)
//	(std::string(Imf::ViAttribute::staticTypeName()),				exr::EAttributeVectori)
//	(std::string(Imf::TypedAttribute<double>::staticTypeName()),		exr::EAttributeDouble)
//	(std::string(Imf::FloatAttribute::staticTypeName()),				exr::EAttributeFloat)
//	(std::string(Imf::TypedAttribute<int>::staticTypeName()),		exr::EAttributeInt)
//	(std::string("unknown"),											exr::EAttributeTypeInvalid);

const mex::ConstBiMap<int, exr::EAttributeType> intToAttributeType
	= mex::ConstBiMap<int, exr::EAttributeType>
	(5,		exr::EAttributeChannelList)
	(6,		exr::EAttributeCompression)
	(7,		exr::EAttributeLineOrder)
	(8,		exr::EAttributeChromaticities)
	(9,		exr::EAttributeEnvmap)
	(10,	exr::EAttributeString)
	(11,	exr::EAttributeBox2f)
	(12,	exr::EAttributeBox2i)
	(13,	exr::EAttributeV2f)
	(14,	exr::EAttributeV2i)
	(15,	exr::EAttributeVectorf)
	(16,	exr::EAttributeVectori)
	(17,	exr::EAttributeDouble)
	(18,	exr::EAttributeFloat)
	(19,	exr::EAttributeInt)
	(20,	exr::EAttributeTypeInvalid);

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
	exr::ExrInputFile file(fileName);
	mexAssert(intToAttributeType[6] == exr::EAttributeCompression);
	for (std::map<int, exr::EAttributeType>::const_iterator iter = intToAttributeType.get_map().begin(),
		end = intToAttributeType.get_map().end();
		iter != end;
		++iter) {
		mexPrintf("%d %d\n", iter->first, iter->second);
	}
	mexAssert(stringToAttributeType[std::string("compression")] == exr::EAttributeCompression);
	for (std::map<std::string, exr::EAttributeType>::const_iterator iter = stringToAttributeType.get_map().begin(),
		end = stringToAttributeType.get_map().end();
		iter != end;
		++iter) {
		mexPrintf("%s %d\n", iter->first.c_str(), iter->second);
	}
	mexPrintf("%d %s %d %d %d %d %s %s %d %d\n", stringToAttributeType[std::string("compression")],
							stringToAttributeType.get_map().find("lineOrder")->first.c_str(),
							stringToAttributeType.get_map().find("lineOrder")->second,
							stringToAttributeType[std::string("chlist")],
							stringToAttributeType[std::string("envmap")],
							stringToAttributeType[std::string("unknown")],
							std::string(Imf::ViAttribute::staticTypeName()).c_str(),
							Imf::ViAttribute::staticTypeName(),
							stringToAttributeType[std::string(Imf::ViAttribute::staticTypeName())],
							exr::EAttributeTypeInvalid);
//	file.get

}
