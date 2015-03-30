/*
 * raw_mex.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: igkiou
 */

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include "raw_mex.h"

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
//	return int(1);
}

/*
 * TODO: Incomplete. Add option for dcraw processing. Copy dcraw args processing
 * from sample.
 */
mex::MxArray RawInputFile::readData() {
	mex::MxNumeric<bool> doSubtractFlag(false);
	mex::MxString dcrawFlags = mex::MxString("-6 -W -j -T -M -o 0 -g 1");
	mex::MxArray retArg(readData(doSubtractFlag, dcrawFlags));
	doSubtractFlag.destroy();
	dcrawFlags.destroy();
	return retArg;
}

mex::MxArray RawInputFile::readData(const mex::MxNumeric<bool>& doSubtractDarkFrame,
									const mex::MxString& dcrawFlags) {

	int errorCode = m_rawProcessor.unpack();
	mexAssert(errorCode == LIBRAW_SUCCESS);

	if (dcrawFlags.get_string().empty()) {
		errorCode = m_rawProcessor.raw2image();
		mexAssert(errorCode == LIBRAW_SUCCESS);

		if (doSubtractDarkFrame[0]) {
			errorCode = m_rawProcessor.subtract_black();
			mexAssert(errorCode == LIBRAW_SUCCESS);
		}

		int width = getWidth();
		int height = getHeight();
		mex::MxNumeric<unsigned short> pixelBuffer(height, width);

		unsigned short* pixelRaw = pixelBuffer.getData();
		for (int pixelWidth = 0; pixelWidth < width; ++pixelWidth) {
			for (int pixelHeight = 0; pixelHeight < height; ++pixelHeight) {
				int arrayIndex = pixelWidth * height + pixelHeight;
				int rawIndex = pixelHeight * width + pixelWidth;
				pixelRaw[arrayIndex] = m_rawProcessor.imgdata.image[rawIndex]
									[m_rawProcessor.COLOR(pixelHeight, pixelWidth)];
			}
		}
		return mex::MxArray(pixelBuffer.get_array());
	} else {
		int argc = std::count(dcrawFlags.get_string().begin(),
							dcrawFlags.get_string().end(), ' ') + 2;
		std::vector<const char*> argv(argc);

		std::string::const_iterator currentPos = dcrawFlags.get_string().begin();
		for (int iter = 1; iter < argc; ++iter) {
			argv[iter] = (&(*currentPos));
			currentPos = ++std::find(currentPos, dcrawFlags.get_string().end(), ' ');
		}

		for (int iter = 1; iter < argv.size(); ++iter) {
			printf("%s\n", argv[iter]);
		}
		printf("argc %d length %d\n", argc, argv.size());

		int i,arg,c,ret;
		char opm,opt,*cp,*sp;
		int use_bigfile=0, use_timing=0;
		void *iobuffer=0;

//		argv[argc] = (char*)"";
		for (arg = 1; arg < argc; ++arg) {
			const char *optstr = argv[arg];
			opt = argv[arg][1];
			if ((cp = std::strchr(sp=(char*)"cnbrkStqmHABCgU", opt))!=0) {
				for (i=0; i < "111411111144221"[cp-sp]-'0'; i++) {
					if ((arg + i >= argc) || (!std::isdigit(argv[arg+i][0]) && !optstr[2])) {
						std::fprintf(stderr,"Non-numeric argument to \"-%c\"\n", opt);
						mexAssertEx(0, "Unknown attribute type");
						return mex::MxArray();
					}
				}
			}
			if(!std::strchr("ftdeam",opt) && (std::strlen(argv[arg]) > 1) && (argv[arg][2] != ' ')) {
				std::fprintf (stderr,"Unknown option \"%s\".\n",argv[arg-1]);
				mexAssertEx(0, "Unknown attribute type");
				return mex::MxArray();
			}
			printf("arg: %d of %d; val: %s\n", arg, argv.size(), argv[arg]);
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
					m_rawProcessor.imgdata.params.adjust_maximum_thr = (float)std::atof(argv[++arg]);
					break;
				}
				case 'U': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.auto_bright_thr   = (float)std::atof(argv[++arg]);
					break;
				}
				case 'n': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.threshold   = (float)std::atof(argv[++arg]);
					break;
				}
				case 'b': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.bright      = (float)std::atof(argv[++arg]);
					break;
				}
				case 'P': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.bad_pixels  = const_cast<char *>(argv[++arg]);
					break;
				}
				case 'K': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.dark_frame  = const_cast<char *>(argv[++arg]);
					break;
				}
				case 'r': {
					for(c=0;c<4;c++) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.user_mul[c] = (float)std::atof(argv[++arg]);
					}
					break;
				}
				case 'C': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.aber[0] = 1 / std::atof(argv[++arg]);
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.aber[2] = 1 / std::atof(argv[++arg]);
					break;
				}
				case 'g': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.gamm[0] = 1 / std::atof(argv[++arg]);
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.gamm[1] =     std::atof(argv[++arg]);
					break;
				}
				case 'k': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.user_black  = std::atoi(argv[++arg]);
					break;
				}
				case 'S': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.user_sat    = std::atoi(argv[++arg]);
					break;
				}
				case 't': {
					if(!argv[arg][2]) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.user_flip   = std::atoi(argv[++arg]);
					} else {
						std::fprintf (stderr,"Unknown option \"%s\".\n",argv[arg]);
						mexAssertEx(0, "Unknown attribute type");
					}
					break;
				}
				case 'q': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.user_qual   = std::atoi(argv[++arg]);
					break;
				}
				case 'm': {
					if(!argv[arg][2]) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.med_passes  = std::atoi(argv[++arg]);
					} else {
						std::fprintf (stderr,"Unknown option \"%s\".\n",argv[arg]);
						mexAssertEx(0, "Unknown attribute type");
					}
					break;
				}
				case 'H': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.highlight   = std::atoi(argv[++arg]);
					break;
				}
				case 's': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.shot_select = std::abs(std::atoi(argv[++arg]));
					break;
				}
				case 'o': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					if(std::isdigit(argv[arg + 1][0]) && !std::isdigit(argv[arg + 1][1])) {
						m_rawProcessor.imgdata.params.output_color = std::atoi(argv[++arg]);
					}
					else {
						m_rawProcessor.imgdata.params.output_profile = const_cast<char *>(argv[++arg]);
					}
					break;
				}
				case 'p': {
					mexAssertEx(arg < argc - 1, "Unknown attribute type");
					m_rawProcessor.imgdata.params.camera_profile = const_cast<char *>(argv[++arg]);
					break;
				}
				case 'h': {
					m_rawProcessor.imgdata.params.half_size         = 1;
					break;
				}
				case 'f': {
					if(!std::strcmp(optstr,"-fbdd")) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.fbdd_noiserd = std::atoi(argv[++arg]);
					} else {
						if(!argv[arg-1][2]) {
							m_rawProcessor.imgdata.params.four_color_rgb    = 1;
						} else {
							std::fprintf (stderr,"Unknown option \"%s\".\n",argv[arg]);
							mexAssertEx(0, "Unknown attribute type");
						}
					}
					break;
				}
				case 'A': {
					for(c=0; c<4;c++) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.greybox[c]  = std::atoi(argv[++arg]);
					}
					break;
				}
				case 'B': {
					for(c=0; c<4;c++) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.cropbox[c]  = std::atoi(argv[++arg]);
					}
					break;

				}
				case 'a': {
					if(!std::strcmp(optstr,"-aexpo")) {
						m_rawProcessor.imgdata.params.exp_correc = 1;
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.exp_shift = (float)std::atof(argv[++arg]);
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.exp_preser = (float)std::atof(argv[++arg]);
					} else if(!argv[arg][2]) {
						m_rawProcessor.imgdata.params.use_auto_wb       = 1;
					} else {
						std::fprintf (stderr,"Unknown option \"%s\".\n",argv[arg]);
						mexAssertEx(0, "Unknown attribute type");
					}
					break;
				}
				case 'w': {
					m_rawProcessor.imgdata.params.use_camera_wb     = 1;
					break;
				}
				case 'M': {
					m_rawProcessor.imgdata.params.use_camera_matrix = (opm == '+');
					break;
				}
				case 'j': {
					m_rawProcessor.imgdata.params.use_fuji_rotate   = 0;
					break;
				}
				case 'W': {
					m_rawProcessor.imgdata.params.no_auto_bright    = 1;
					break;
				}
				case 'T': {
					m_rawProcessor.imgdata.params.output_tiff       = 1;
					break;
				}
				case '4': {
					m_rawProcessor.imgdata.params.gamm[0] = m_rawProcessor.imgdata.params.gamm[1] =  m_rawProcessor.imgdata.params.no_auto_bright    = 1; /* no break here! */
				}
				case '6': {
					m_rawProcessor.imgdata.params.output_bps = 16;
					break;
				}
				case 'F': {
					use_bigfile=1;
					break;
				}
				case 'd': {
					if(!strcmp(optstr,"-dcbi")) {
						mexAssertEx(arg < argc - 1, "Unknown attribute type");
						m_rawProcessor.imgdata.params.dcb_iterations = std::atoi(argv[++arg]);
					} else if(!strcmp(optstr,"-disars")) {
						m_rawProcessor.imgdata.params.use_rawspeed=0;
					} else if(!strcmp(optstr,"-disadcf")) {
						m_rawProcessor.imgdata.params.force_foveon_x3f=1;
					} else if(!strcmp(optstr,"-disinterp")) {
						m_rawProcessor.imgdata.params.no_interpolation=1;
					} else if(!strcmp(optstr,"-dcbe")) {
						m_rawProcessor.imgdata.params.dcb_enhance_fl = 1;
					} else if(!strcmp(optstr,"-dsrawrgb1")) {
						m_rawProcessor.imgdata.params.sraw_ycc = 1;
					} else if(!strcmp(optstr,"-dsrawrgb2")) {
						m_rawProcessor.imgdata.params.sraw_ycc = 2;
					} else if(!strcmp(optstr,"-dbnd")) {
						for(c=0; c<4; c++) {
							mexAssertEx(arg < argc - 1, "Unknown attribute type");
							m_rawProcessor.imgdata.params.wf_deband_treshold[c] = (float)std::atof(argv[++arg]);
							m_rawProcessor.imgdata.params.wf_debanding = 1;
						}
					} else {
						fprintf (stderr,"Unknown option \"%s\".\n",argv[arg]);
						mexAssertEx(0, "Unknown attribute type");
					}
					break;
				}
				default: {
					fprintf (stderr,"Unknown option \"-%c\".\n", opt);
					mexAssertEx(0, "Unknown attribute type");
				}
			}
		}

		errorCode = m_rawProcessor.dcraw_process();
		mexAssert(errorCode == LIBRAW_SUCCESS);

		return mex::MxArray();
	}
}

/*
 * TODO: Add attribute getting.
 */
mex::MxArray RawInputFile::getAttribute(const mex::MxString& attributeName) const {
	return mex::MxArray();
}

mex::MxArray RawInputFile::getAttribute() const {
	return mex::MxArray();
}

}	/* namespace raw */
