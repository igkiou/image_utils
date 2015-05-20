/*
 * raw_mex.cpp
 *
 *  Created on: Jan 7, 2014
 * Author: igkiou
 */

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include <string>

#include "../raw/raw.h"

namespace raw {

/*
 * Check valid file function.
 */
mex::MxNumeric<bool> isRawFile(const mex::MxString& fileName) {
	LibRaw rawProcessor;
	return mex::MxNumeric<bool>(rawProcessor.open_file(fileName.c_str())
								== LIBRAW_SUCCESS);
}

/*
 * Input file handling.
 */
RawInputFile::RawInputFile(const mex::MxString& fileName):
						m_fileName(fileName.get_string()),
						m_unpackedFile(false),
						m_rawProcessor() {
	int errorCode = m_rawProcessor.open_file(m_fileName.c_str());
	mexAssert(errorCode == LIBRAW_SUCCESS);
}

mex::MxString RawInputFile::getFileName() const {
	return mex::MxString(m_fileName);
}

mex::MxNumeric<bool> RawInputFile::isValidFile() const {
	/*
	 * LibRaw always checks validity at construction, so if we're here, file is
	 * valid.
	 */
	return mex::MxNumeric<bool>(true);
}

int RawInputFile::getHeight() const {
	return m_rawProcessor.imgdata.sizes.iheight;
}

int RawInputFile::getWidth() const {
	return m_rawProcessor.imgdata.sizes.iwidth;
}

int RawInputFile::getNumberOfChannels() const {
	return m_rawProcessor.imgdata.idata.colors;
}

mex::MxArray RawInputFile::readData() {
	return readData(false, "");
}

mex::MxArray RawInputFile::readData(
							const mex::MxNumeric<bool>& doSubtractDarkFrame) {
	return readData(doSubtractDarkFrame[0], "");
}

mex::MxArray RawInputFile::readData(const mex::MxString& dcrawFlags) {
	return readData(false, dcrawFlags.get_string());
}

mex::MxArray RawInputFile::readData(
								const mex::MxNumeric<bool>& doSubtractDarkFrame,
								const mex::MxString& dcrawFlags) {
	return readData(doSubtractDarkFrame[0], dcrawFlags.get_string());
}

mex::MxArray RawInputFile::readData(bool doSubtractDarkFrame,
										const std::string& dcrawFlags) {
	unpackFile();
	int errorCode;

	if (dcrawFlags.empty()) {
		errorCode = m_rawProcessor.raw2image();
		mexAssert(errorCode == LIBRAW_SUCCESS);

		if (doSubtractDarkFrame) {
			errorCode = m_rawProcessor.subtract_black();
			mexAssert(errorCode == LIBRAW_SUCCESS);
		}

		int width = getWidth();
		int height = getHeight();
		mex::MxNumeric<PixelType> pixelArray(height, width);

		PixelType* pixelBuffer = pixelArray.getData();
		for (int pixelWidth = 0; pixelWidth < width; ++pixelWidth) {
			for (int pixelHeight = 0; pixelHeight < height; ++pixelHeight) {
				int arrayIndex = pixelWidth * height + pixelHeight;
				int rawIndex = pixelHeight * width + pixelWidth;
				pixelBuffer[arrayIndex] = m_rawProcessor.imgdata.image[rawIndex]
								[m_rawProcessor.COLOR(pixelHeight, pixelWidth)];
			}
		}
		return mex::MxArray(pixelArray.get_array());
	} else {
		parseDcrawFlags(dcrawFlags);

		/*
		 * raw_mex only supports 16-bit formats and processes data as if it was
		 * to be written to tiff..
		 */
		m_rawProcessor.imgdata.params.output_tiff = 1;
		m_rawProcessor.imgdata.params.output_bps = 16;

		errorCode = m_rawProcessor.dcraw_process();
		mexAssert(errorCode == LIBRAW_SUCCESS);

		int width = getWidth();
		int height = getHeight();
		int numChannels = getNumberOfChannels();
		std::vector<int> dimensions;
		dimensions.push_back(height);
		dimensions.push_back(width);
		if (numChannels > 1) {
			dimensions.push_back(numChannels);
		}
		mex::MxNumeric<PixelType> pixelArray(
						static_cast<int>(dimensions.size()), &dimensions[0]);
		PixelType* pixelBuffer = pixelArray.getData();

		errorCode = m_rawProcessor.copy_processed(pixelBuffer);
		mexAssert(errorCode == LIBRAW_SUCCESS);

		return mex::MxArray(pixelArray.get_array());
	}
}

mex::MxArray RawInputFile::getCFAInformation() {
	int width = getWidth();
	int height = getHeight();
	mex::MxNumeric<unsigned char> filterArray(height, width);

	unsigned char* filterBuffer = filterArray.getData();
	for (int pixelWidth = 0; pixelWidth < width; ++pixelWidth) {
		for (int pixelHeight = 0; pixelHeight < height; ++pixelHeight) {
			int arrayIndex = pixelWidth * height + pixelHeight;
			filterBuffer[arrayIndex] = static_cast<unsigned char>(
								m_rawProcessor.COLOR(pixelHeight, pixelWidth));
		}
	}

    mex::MxString filterNames(m_rawProcessor.imgdata.idata.cdesc);

    std::vector<std::string> arrayNames;
    arrayNames.push_back("filterArray");
    arrayNames.push_back("filterNames");
    std::vector<mex::MxArray*> arrayVars;
    arrayVars.push_back(&filterArray);
    arrayVars.push_back(&filterNames);
    return mex::MxArray(mex::MxStruct(arrayNames, arrayVars).get_array());
}

void RawInputFile::parseDcrawFlags(const std::string& dcrawFlags) {
	int argc = std::count(dcrawFlags.begin(), dcrawFlags.end(), ' ') + 2;
	std::vector<const char*> argv(argc);

	std::string::const_iterator currentPos = dcrawFlags.begin();
	for (int iter = 1; iter < argc; ++iter) {
		argv[iter] = (&(*currentPos));
		currentPos = ++std::find(currentPos, dcrawFlags.end(), ' ');
	}

	const char* sp = "cnbrkStqmHABCgU";
	for (int arg = 1; arg < argc; ++arg) {
		const char *optstr = argv[arg];
		const char opm = argv[arg][0];
		const char opt = argv[arg][1];
		const char *cp = std::strchr(sp, opt);
		if (cp != nullptr) {
			for (int i = 0; i < "111411111144221"[cp-sp]-'0'; ++i) {
				if ((arg + i >= argc) ||
						(!std::isdigit(argv[arg+i][0]) && !optstr[2])) {
					std::fprintf(stderr, "Non-numeric argument to \"-%c\"\n",
																		opt);
					mexAssertEx(0, "Unknown attribute type");
				}
			}
		}
		if (!std::strchr("ftdeam", opt) && (std::strlen(argv[arg]) > 1) &&
														(argv[arg][2] != ' ')) {
			std::fprintf(stderr, "Unknown option \"%s\".\n", argv[arg - 1]);
			mexAssertEx(0, "Unknown attribute type");
		}
		switch (opt) {
			case 'v': {
				break;
			}
			case 'G': {
				m_rawProcessor.imgdata.params.green_matching = 1;
				break;
			}
			case 'c': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.adjust_maximum_thr =
									static_cast<float>(std::atof(argv[++arg]));
				break;
			}
			case 'U': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.auto_bright_thr =
									static_cast<float>(std::atof(argv[++arg]));
				break;
			}
			case 'n': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.threshold =
									static_cast<float>(std::atof(argv[++arg]));
				break;
			}
			case 'b': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.bright =
									static_cast<float>(std::atof(argv[++arg]));
				break;
			}
			case 'P': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.bad_pixels =
												const_cast<char *>(argv[++arg]);
				break;
			}
			case 'K': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.dark_frame =
												const_cast<char *>(argv[++arg]);
				break;
			}
			case 'r': {
				for(int c = 0; c < 4; ++c) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.user_mul[c] =
									static_cast<float>(std::atof(argv[++arg]));
				}
				break;
			}
			case 'C': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.aber[0]
												   = 1 / std::atof(argv[++arg]);
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.aber[2]
												   = 1 / std::atof(argv[++arg]);
				break;
			}
			case 'g': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.gamm[0]
												   = 1 / std::atof(argv[++arg]);
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.gamm[1]
												   =  std::atof(argv[++arg]);
				break;
			}
			case 'k': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.user_black
													= std::atoi(argv[++arg]);
				break;
			}
			case 'S': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.user_sat
													= std::atoi(argv[++arg]);
				break;
			}
			case 't': {
				if(!argv[arg][2]) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.user_flip
													= std::atoi(argv[++arg]);
				} else {
					std::fprintf(stderr, "Unknown option \"%s\".\n", argv[arg]);
					mexAssertEx(0, "Unknown attribute type");
				}
				break;
			}
			case 'q': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.user_qual
													= std::atoi(argv[++arg]);
				break;
			}
			case 'm': {
				if(!argv[arg][2]) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.med_passes
													= std::atoi(argv[++arg]);
				} else {
					std::fprintf(stderr, "Unknown option \"%s\".\n", argv[arg]);
					mexAssertEx(0, "Unknown attribute type");
				}
				break;
			}
			case 'H': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.highlight
											= std::atoi(argv[++arg]);
				break;
			}
			case 's': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.shot_select
											= std::abs(std::atoi(argv[++arg]));
				break;
			}
			case 'o': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				if (std::isdigit(argv[arg + 1][0]) &&
						!std::isdigit(argv[arg + 1][1])) {
					m_rawProcessor.imgdata.params.output_color
													= std::atoi(argv[++arg]);
				} else {
					m_rawProcessor.imgdata.params.output_profile
											= const_cast<char *>(argv[++arg]);
				}
				break;
			}
			case 'p': {
				mexAssertEx(arg < argc - 1, "Unknown attribute type");
				m_rawProcessor.imgdata.params.camera_profile
											= const_cast<char *>(argv[++arg]);
				break;
			}
			case 'h': {
				m_rawProcessor.imgdata.params.half_size = 1;
				break;
			}
			case 'f': {
				if (!std::strcmp(optstr, "-fbdd")) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.fbdd_noiserd
													= std::atoi(argv[++arg]);
				} else {
					if(!argv[arg-1][2]) {
						m_rawProcessor.imgdata.params.four_color_rgb = 1;
					} else {
						std::fprintf(stderr, "Unknown option \"%s\".\n",
																	argv[arg]);
						mexAssertEx(0, "Unknown attribute type");
					}
				}
				break;
			}
			case 'A': {
				for(int c = 0; c < 4; ++c) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.greybox[c]
													= std::atoi(argv[++arg]);
				}
				break;
			}
			case 'B': {
				for(int c = 0; c < 4; ++c) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.cropbox[c]
													= std::atoi(argv[++arg]);
				}
				break;
			}
			case 'a': {
				if (!std::strcmp(optstr, "-aexpo")) {
					m_rawProcessor.imgdata.params.exp_correc = 1;
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.exp_shift
								= static_cast<float>(std::atof(argv[++arg]));
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.exp_preser
								= static_cast<float>(std::atof(argv[++arg]));
				} else if (!argv[arg][2]) {
					m_rawProcessor.imgdata.params.use_auto_wb = 1;
				} else {
					std::fprintf(stderr, "Unknown option \"%s\".\n", argv[arg]);
					mexAssertEx(0, "Unknown attribute type");
				}
				break;
			}
			case 'w': {
				m_rawProcessor.imgdata.params.use_camera_wb = 1;
				break;
			}
			case 'M': {
				m_rawProcessor.imgdata.params.use_camera_matrix = (opm == '+');
				break;
			}
			case 'j': {
				m_rawProcessor.imgdata.params.use_fuji_rotate = 0;
				break;
			}
			case 'W': {
				m_rawProcessor.imgdata.params.no_auto_bright = 1;
				break;
			}
			case 'd': {
				if (!strcmp(optstr, "-dcbi")) {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.dcb_iterations
													= std::atoi(argv[++arg]);
				} else if (!strcmp(optstr, "-disars")) {
					m_rawProcessor.imgdata.params.use_rawspeed = 0;
				} else if (!strcmp(optstr, "-disadcf")) {
					m_rawProcessor.imgdata.params.force_foveon_x3f = 1;
				} else if (!strcmp(optstr, "-disinterp")) {
					m_rawProcessor.imgdata.params.no_interpolation = 1;
				} else if (!strcmp(optstr, "-dcbe")) {
					m_rawProcessor.imgdata.params.dcb_enhance_fl = 1;
				} else if (!strcmp(optstr, "-dsrawrgb1")) {
					m_rawProcessor.imgdata.params.sraw_ycc = 1;
				} else if (!strcmp(optstr, "-dsrawrgb2")) {
					m_rawProcessor.imgdata.params.sraw_ycc = 2;
				} else if (!strcmp(optstr, "-dbnd")) {
					for(int c = 0; c < 4; ++c) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.wf_deband_treshold[c]
								= static_cast<float>(std::atof(argv[++arg]));
						m_rawProcessor.imgdata.params.wf_debanding = 1;
					}
				} else {
					fprintf(stderr, "Unknown option \"%s\".\n", argv[arg]);
					mexAssertEx(0, "Unknown attribute type");
				}
				break;
			}
			default: {
				fprintf(stderr, "Unknown option \"-%c\".\n", opt);
				mexAssertEx(0, "Unknown attribute type");
				break;
			}
		}
	}
}

void RawInputFile::unpackFile() {
	if (!m_unpackedFile) {
		int errorCode = m_rawProcessor.unpack();
		mexAssert(errorCode == LIBRAW_SUCCESS);
		m_unpackedFile = true;
	}
}

/*
 * TODO: Add attribute getting.
 * --sizes:
 * iwidth - width
 * iheight - height
 * --color:
 * complicated, lots of potentially useful info, but varies with camera
 */
mex::MxArray RawInputFile::getAttribute(const mex::MxString& attributeName)
																		const {
	return getAttribute(attributeName.get_string());
}

mex::MxArray RawInputFile::getAttribute() const {
	std::vector<std::string> nameVec;
	std::vector<mex::MxArray*> arrayVec;

	nameVec.push_back(std::string("cameraManufacturer"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("cameraModel"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("dngVersion"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("iso"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("shutter"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("aperture"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("focalLength"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("timestamp"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("shotOrder"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("gpsData"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("imageDescription"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));
	nameVec.push_back(std::string("owner"));
	arrayVec.push_back(new mex::MxArray(getAttribute(*(--(nameVec.end()))).get_array()));

	mex::MxArray retArg(mex::MxStruct(nameVec, arrayVec).get_array());
	for (int iter = 0, numAttributes = arrayVec.size();
		iter < numAttributes;
		++iter) {
		delete arrayVec[iter];
	}
	return retArg;
}

mex::MxArray RawInputFile::getAttribute(const std::string& attributeName) const {
	if (!attributeName.compare("cameraManufacturer")) {
		return mex::MxArray(mex::MxString(m_rawProcessor.imgdata.idata.make).get_array());
	} else if (!attributeName.compare("cameraModel")) {
		return mex::MxArray(mex::MxString(m_rawProcessor.imgdata.idata.model).get_array());
	} else if (!attributeName.compare("dngVersion")) {
		return mex::MxArray(mex::MxNumeric<unsigned int>(m_rawProcessor.imgdata.idata.dng_version).get_array());
	} else if (!attributeName.compare("iso")) {
		return mex::MxArray(mex::MxNumeric<float>(m_rawProcessor.imgdata.other.iso_speed).get_array());
	} else if (!attributeName.compare("shutter")) {
		return mex::MxArray(mex::MxNumeric<float>(m_rawProcessor.imgdata.other.shutter).get_array());
	} else if (!attributeName.compare("aperture")) {
		return mex::MxArray(mex::MxNumeric<float>(m_rawProcessor.imgdata.other.aperture).get_array());
	} else if (!attributeName.compare("focalLength")) {
		return mex::MxArray(mex::MxNumeric<float>(m_rawProcessor.imgdata.other.focal_len).get_array());
	} else if (!attributeName.compare("timestamp")) {
		std::string timestampString(std::asctime(std::gmtime(
								&(m_rawProcessor.imgdata.other.timestamp))));
		if (!timestampString.empty()) {
			timestampString.pop_back();
		}
		return mex::MxArray(mex::MxString(timestampString).get_array());
	} else if (!attributeName.compare("shotOrder")) {
		return mex::MxArray(mex::MxNumeric<unsigned int>(m_rawProcessor.imgdata.other.shot_order).get_array());
	} else if (!attributeName.compare("gpsData")) {
		return mex::MxArray(mex::MxNumeric<unsigned int>(m_rawProcessor.imgdata.other.gpsdata, 32, 1).get_array());
	} else if (!attributeName.compare("imageDescription")) {
		return mex::MxArray(mex::MxString(m_rawProcessor.imgdata.other.desc).get_array());
	} else if (!attributeName.compare("owner")) {
		return mex::MxArray(mex::MxString(m_rawProcessor.imgdata.other.artist).get_array());
	} else {
		mexAssertEx(0, "Unknown attribute type");
		return mex::MxArray();
	}
}

}	/* namespace raw */
