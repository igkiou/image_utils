/*
 * image_utils//image_utils/raw_mex/libraw_ext.cpp/libraw_ext.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: igkiou
 */

#include "libraw_ext.h"
//#define LIBRAW_IO_REDEFINED
//#include "internal/defines.h"
//#include "internal/var_defines.h"

#define EXCEPTION_HANDLER(e) do{                        \
    /* fprintf(stderr,"Exception %d caught\n",e);*/     \
    switch(e)                                           \
      {                                                 \
      case LIBRAW_EXCEPTION_ALLOC:                      \
        recycle();                                      \
        return LIBRAW_UNSUFFICIENT_MEMORY;              \
      case LIBRAW_EXCEPTION_DECODE_RAW:                 \
      case LIBRAW_EXCEPTION_DECODE_JPEG:                \
        recycle();                                      \
        return LIBRAW_DATA_ERROR;                       \
      case LIBRAW_EXCEPTION_DECODE_JPEG2000:            \
        recycle();                                      \
        return LIBRAW_DATA_ERROR;                       \
      case LIBRAW_EXCEPTION_IO_EOF:                     \
      case LIBRAW_EXCEPTION_IO_CORRUPT:                 \
        recycle();                                      \
        return LIBRAW_IO_ERROR;                                 \
      case LIBRAW_EXCEPTION_CANCELLED_BY_CALLBACK:              \
        recycle();                                              \
        return LIBRAW_CANCELLED_BY_CALLBACK;                    \
      case LIBRAW_EXCEPTION_BAD_CROP:                           \
        recycle();                                              \
        return LIBRAW_BAD_CROP;                                 \
      default:                                                  \
        return LIBRAW_UNSPECIFIED_ERROR;                        \
      }                                                         \
  }while(0)


LibRawExtension::LibRawExtension(unsigned int flags) : LibRaw(flags) {}

int LibRawExtension::copy_processed(unsigned short* pixelBuffer) {
	CHECK_ORDER_LOW(LIBRAW_PROGRESS_LOAD_RAW);

	if(!imgdata.image) {
		return LIBRAW_OUT_OF_ORDER_CALL;
	}

	try {
		if(!libraw_internal_data.output_data.histogram) {
			libraw_internal_data.output_data.histogram =
			(int (*)[LIBRAW_HISTOGRAM_SIZE]) malloc(sizeof(*libraw_internal_data.output_data.histogram)*4);
			merror(libraw_internal_data.output_data.histogram,"LibRaw::dcraw_ppm_tiff_writer()");
		}
		copy_processed_internal(pixelBuffer);
		SET_PROC_FLAG(LIBRAW_PROGRESS_FLIP);
		return 0;
	} catch (LibRaw_exceptions err) {
		EXCEPTION_HANDLER(err);
	}
}

void LibRawExtension::copy_processed_internal(unsigned short* pixelBuffer) {

	int perc = imgdata.sizes.width * imgdata.sizes.height * imgdata.params.auto_bright_thr;
	if (libraw_internal_data.internal_output_params.fuji_width) {
		perc /= 2;
	}

	int t_white = 0x2000;
	if (!((imgdata.params.highlight & ~2) || imgdata.params.no_auto_bright)) {
		for (int t_white = 0, c = 0; c < imgdata.idata.colors; ++c) {
			int val = 0x2000;
			for (int total = 0; --val > 32;) {
				if ((total += libraw_internal_data.output_data.histogram[c][val]) > perc) {
					break;
				}
			}
			if (t_white < val) {
				t_white = val;
			}
		}
	}
	gamma_curve(imgdata.params.gamm[0], imgdata.params.gamm[1], 2, (t_white << 3) / imgdata.params.bright);
	imgdata.sizes.iheight = imgdata.sizes.height;
	imgdata.sizes.iwidth = imgdata.sizes.width;
//	if (imgdata.sizes.flip & 4) {
//		SWAP(imgdata.sizes.height, imgdata.sizes.width);
//	}

	int width = imgdata.sizes.width;
	int height = imgdata.sizes.height;
	int colors = imgdata.idata.colors;
	for (int pixelChannel = 0; pixelChannel < colors; ++pixelChannel) {
		for (int pixelWidth = 0; pixelWidth < width; ++pixelWidth) {
			for (int pixelHeight = 0; pixelHeight < height; ++pixelHeight) {
				int arrayIndex = pixelChannel * width * height + pixelWidth * height + pixelHeight;
				int rawIndex = pixelHeight * width + pixelWidth;
				pixelBuffer[arrayIndex] = imgdata.color.curve[imgdata.image[rawIndex][pixelChannel]];
			}
		}
	}
}
