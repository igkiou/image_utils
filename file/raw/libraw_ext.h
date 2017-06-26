/*
 * libraw_ext.h
 *
 *  Created on: Jan 15, 2014
 *      Author: igkiou
 */

#ifndef LIBRAW_EXT_H_
#define LIBRAW_EXT_H_

#include "libraw.h"

//class DllDef LibRawExt : public LibRaw {
//public:
//	LibRawExt(unsigned int flags = LIBRAW_OPTIONS_NONE)
//			: LibRaw(flags),
//			  m_usedPrealloc(false) {	}
//
//	int unpack_prealloc(void* raw_prealoc);
//
//	/*
//	 * WARNING: Re-definition of non-virtual member! Use with care.
//	 */
//	void recycle();
//
//protected:
//	bool m_usedPrealloc;
//
//};

namespace libraw {

class DllDef LibRawExtension : public LibRaw {
public:
	LibRawExtension(unsigned int flags = LIBRAW_OPTIONS_NONE);
	int copy_processed(unsigned short* pixelBuffer);

private:
	void copy_processed_internal(unsigned short* pixelBuffer);
};

}  // namespace libraw

#endif  // LIBRAW_EXT_H_
