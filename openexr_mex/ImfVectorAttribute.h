/*
 * ImfVectorAttribute.h
 *
 *  Created on: May 23, 2012
 *      Author: igkiou
 */

#ifndef IMFVECTORATTRIBUTE_H_
#define IMFVECTORATTRIBUTE_H_

//-----------------------------------------------------------------------------
//
//	class V2iAttribute
//	class V2fAttribute
//	class V3iAttribute
//	class V3fAttribute
//
//-----------------------------------------------------------------------------

#include <ImfAttribute.h>
#include "ImathVec.h"
#include <vector>
#include <iterator>

namespace Imf {

//template <class T>
//class VectorAttribute: public Attribute
//{
//  public:
//
//    //----------------------------
//    // Constructors and destructor
//    //------------_---------------
//
//    VectorAttribute ();
//    VectorAttribute (const std::vector<T> &value);
//    VectorAttribute (const VectorAttribute<T> &other);
//    virtual ~VectorAttribute ();
//
//
//
//  private:
//
//    std::vector<T>					_value;
//};

template <class T>
class VectorAttribute: public Attribute
{
  public:

    //----------------------------
    // Constructors and destructor
    //------------_---------------

    VectorAttribute ();
    VectorAttribute (const std::vector<T> &value);
    VectorAttribute (const VectorAttribute<T> &other);
    virtual ~VectorAttribute ();


    //--------------------------------
    // Access to the attribute's value
    //--------------------------------

    std::vector<T> &					value ();
    const std::vector<T> &				value () const;


    //--------------------------------
    // Get this attribute's type name.
    //--------------------------------

    virtual const char *		typeName () const;


    //---------------------------------------------------------
    // Static version of typeName()
    // This function must be specialized for each value type T.
    //---------------------------------------------------------

    static const char *			staticTypeName ();


    //---------------------
    // Make a new attribute
    //---------------------

    static Attribute *			makeNewAttribute ();


    //------------------------------
    // Make a copy of this attribute
    //------------------------------

    virtual Attribute *			copy () const;


    //-----------------------------------------------------------------
    // Type-specific attribute I/O and copying.
    // Depending on type T, these functions may have to be specialized.
    //-----------------------------------------------------------------

    virtual void		writeValueTo (OStream &os,
					      int version) const;

    virtual void		readValueFrom (IStream &is,
					       int size,
					       int version);

    virtual void		copyValueFrom (const Attribute &other);


    //------------------------------------------------------------
    // Dynamic casts that throw exceptions instead of returning 0.
    //------------------------------------------------------------

    static VectorAttribute *		cast (Attribute *attribute);
    static const VectorAttribute *	cast (const Attribute *attribute);
    static VectorAttribute &		cast (Attribute &attribute);
    static const VectorAttribute &	cast (const Attribute &attribute);


    //---------------------------------------------------------------
    // Register this attribute type so that Attribute::newAttribute()
    // knows how to make objects of this type.
    //
    // Note that this function is not thread-safe because it modifies
    // a global variable in the IlmIlm library.  A thread in a multi-
    // threaded program may call registerAttributeType() only when no
    // other thread is accessing any functions or classes in the
    // IlmImf library.
    //
    //---------------------------------------------------------------

    static void				registerAttributeType ();


    //-----------------------------------------------------
    // Un-register this attribute type (for debugging only)
    //-----------------------------------------------------

    static void				 unRegisterAttributeType ();


  private:

    std::vector<T>					_value;
};


template <class T>
VectorAttribute<T>::VectorAttribute (): _value ()
{
    // empty
}


template <class T>
VectorAttribute<T>::VectorAttribute (const std::vector<T> &value): _value (value.begin(), value.end())
{
    // empty
}


template <class T>
VectorAttribute<T>::VectorAttribute (const VectorAttribute<T> &other):
    _value ()
{
    copyValueFrom (other);
}


template <class T>
VectorAttribute<T>::~VectorAttribute ()
{
    // empty
}


template <class T>
inline std::vector<T> &
VectorAttribute<T>::value ()
{
    return _value;
}


template <class T>
inline const std::vector<T> &
VectorAttribute<T>::value () const
{
    return _value;
}

template <class T>
inline VectorAttribute<T> &
VectorAttribute<T>::cast (Attribute &attribute)
{
    return *cast (&attribute);
}


template <class T>
inline const VectorAttribute<T> &
VectorAttribute<T>::cast (const Attribute &attribute)
{
    return *cast (&attribute);
}


template <class T>
inline void
VectorAttribute<T>::registerAttributeType ()
{
    Attribute::registerAttributeType (staticTypeName(), makeNewAttribute);
}


template <class T>
inline void
VectorAttribute<T>::unRegisterAttributeType ()
{
    Attribute::unRegisterAttributeType (staticTypeName());
}



template <class T>
const char *
VectorAttribute<T>::typeName () const
{
    return staticTypeName();
}


template <class T>
Attribute *
VectorAttribute<T>::makeNewAttribute ()
{
    return new VectorAttribute<T>();
}


template <class T>
Attribute *
VectorAttribute<T>::copy () const
{
    Attribute * attribute = new VectorAttribute<T>();
    attribute->copyValueFrom (*this);
    return attribute;
}


template <class T>
void
VectorAttribute<T>::copyValueFrom (const Attribute &other)
{
    _value = cast(other)._value;
}


template <class T>
VectorAttribute<T> *
VectorAttribute<T>::cast (Attribute *attribute)
{
	VectorAttribute<T> *t =
	dynamic_cast <VectorAttribute<T> *> (attribute);

    if (t == 0)
	throw Iex::TypeExc ("Unexpected attribute type.");

    return t;
}


template <class T>
const VectorAttribute<T> *
VectorAttribute<T>::cast (const Attribute *attribute)
{
    const VectorAttribute<T> *t =
	dynamic_cast <const VectorAttribute<T> *> (attribute);

    if (t == 0)
	throw Iex::TypeExc ("Unexpected attribute type.");

    return t;
}


template <class T>
void VectorAttribute<T>::writeValueTo (OStream &os, int version) const
{
	for (size_t vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::write <StreamIO> (os, _value[vecIt]);
	}
}

template <class T>
void VectorAttribute<T>::readValueFrom (IStream &is, int size, int version)
{
	for (size_t vecIt = 0; vecIt < _value.size(); ++vecIt) {
		Xdr::read <StreamIO> (is, _value[vecIt]);
	}
}


typedef VectorAttribute<int> ViAttribute;
template <> const char *ViAttribute::staticTypeName ();

typedef VectorAttribute<float> VfAttribute;
template <> const char *VfAttribute::staticTypeName ();

} // namespace Imf

#endif /* IMFVECTORATTRIBUTE_H_ */
