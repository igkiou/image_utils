/*
 * libraw_ext.h
 *
 *  Created on: Jan 15, 2014
 *      Author: igkiou
 */

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

class LibRawExtension : protected LibRaw {
public:
	explicit LibRawExtension(unsigned int flags = LIBRAW_OPTIONS_NONE);
	int copy_processed(unsigned short* imgBuffer);

private:
	int copy_processed_internal(unsigned short* imgBuffer);
};
