/*
 * image_utils//image_utils/raw_mex/libraw_ext.cpp/libraw_ext.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: igkiou
 */

#include "libraw.h"
#include "libraw_ext.h"

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


explicit LibRawExtension::LibRawExtension(
		unsigned int flags = LIBRAW_OPTIONS_NONE)
		: LibRaw(flags) {}

int LibRawExtension::copy_processed(unsigned short *imgBuffer) {
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
		libraw_internal_data.internal_data.output = f;
		copy_processed_internal(imgBuffer);
		SET_PROC_FLAG(LIBRAW_PROGRESS_FLIP);
		libraw_internal_data.internal_data.output = NULL;
		return 0;
	} catch (LibRaw_exceptions err) {
		EXCEPTION_HANDLER(err);
	}
}
