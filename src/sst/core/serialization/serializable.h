/*
 *  This file is part of SST/macroscale:
 *               The macroscale architecture simulator from the SST suite.
 *  Copyright (c) 2009-2021 NTESS.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-NA0003525 with NTESS,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top
 *  SST/macroscale directory.
 */

#ifndef SST_CORE_SERIALIZATION_SERIALIZABLE_H
#define SST_CORE_SERIALIZATION_SERIALIZABLE_H

#include "sst/core/serialization/serializable_fwd.h"
#include "sst/core/serialization/serializer.h"
#include "sst/core/warnmacros.h"

#include <limits>
#include <stdint.h>
#include <typeinfo>
#include <unordered_map>

namespace SST {
namespace Core {
namespace Serialization {

namespace pvt {

// Functions to implement the hash for cls_id at compile time.  The
// hash function being implemented is:

inline uint32_t
type_hash(const char* key)
{
    int      len  = ::strlen(key);
    uint32_t hash = 0;
    for ( int i = 0; i < len; ++i ) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

// Using constexpr is very limited, so the implementation is a bit
// convoluted.  May be able to streamline later.

// computes hash ^= (hash >> 6)
constexpr uint32_t
B(const uint32_t b)
{
    return b ^ (b >> 6);
}

// computes hash += (hash << 10)
constexpr uint32_t
A(const uint32_t a)
{
    return B((a << 10) + a);
}

// recursive piece that computes the for loop
template <size_t idx>
constexpr uint32_t
ct_hash_rec(const char* str)
{
    return A(str[idx] + ct_hash_rec<idx - 1>(str));
}

// End of the recursion (i.e. when you've walked back off the front of
// the string
template <>
constexpr uint32_t
ct_hash_rec<size_t(-1)>(const char* UNUSED(str))
{
    return 0;
}

// computes hash += (hash << 15)
constexpr uint32_t
E(const uint32_t e)
{
    return (e << 15) + e;
}

// computes hash ^= (hash >> 11)
constexpr uint32_t
D(const uint32_t d)
{
    return E((d >> 11) ^ d);
}

// computes hash += (hash << 3)
constexpr uint32_t
C(const uint32_t c)
{
    return D((c << 3) + c);
}

// Main function that computes the final manipulations after calling
// the recursive function to compute the for loop.
template <size_t idx>
constexpr uint32_t
ct_hash(const char* str)
{
    return C(ct_hash_rec<idx>(str));
}

// Macro that should be used to call the compile time hash function
#define COMPILE_TIME_HASH(x) (::SST::Core::Serialization::pvt::ct_hash<sizeof(x) - 2>(x))

} // namespace pvt


#define ImplementVirtualSerializable(obj) \
protected:                                \
    obj(cxn_flag_t flag __attribute__((unused))) {}

#define NotSerializable(obj)                                                                                      \
public:                                                                                                           \
    static void      throw_exc() { serializable_abort(CALL_INFO_LONG, #obj); }                                    \
    virtual void     serialize_order(SST::Core::Serialization::serializer& UNUSED(sst)) override { throw_exc(); } \
    virtual uint32_t cls_id() const override                                                                      \
    {                                                                                                             \
        throw_exc();                                                                                              \
        return NullClsId;                                                                                         \
    }                                                                                                             \
    static obj* construct_deserialize_stub()                                                                      \
    {                                                                                                             \
        throw_exc();                                                                                              \
        return 0;                                                                                                 \
    }                                                                                                             \
    virtual std::string serialization_name() const override                                                       \
    {                                                                                                             \
        throw_exc();                                                                                              \
        return "";                                                                                                \
    }                                                                                                             \
    virtual const char* cls_name() const override                                                                 \
    {                                                                                                             \
        throw_exc();                                                                                              \
        return "";                                                                                                \
    }

#define ImplementSerializableDefaultConstructor(obj)                                      \
public:                                                                                   \
    virtual const char* cls_name() const override { return #obj; }                        \
    virtual uint32_t    cls_id() const override                                           \
    {                                                                                     \
        return SST::Core::Serialization::serializable_builder_impl<obj>::static_cls_id(); \
    }                                                                                     \
    static obj*         construct_deserialize_stub() { return new obj; }                  \
    virtual std::string serialization_name() const override { return #obj; }              \
                                                                                          \
private:                                                                                  \
    friend class SST::Core::Serialization::serializable_builder_impl<obj>;                \
    static bool you_forgot_to_add_ImplementSerializable_to_this_class() { return false; }

#define SER_FORWARD_AS_ONE(...) __VA_ARGS__

#define ImplementSerializable(...) \
public:                            \
    ImplementSerializableDefaultConstructor(SER_FORWARD_AS_ONE(__VA_ARGS__))


// Hold off on trivially_serializable for now, as it's not really safe
// in the case of inheritance
//
// class trivially_serializable {
// };

} // namespace Serialization
} // namespace Core
} // namespace SST

#define SerializableName(obj) #obj

#define DeclareSerializable(obj)

#include "sst/core/serialization/serialize_serializable.h"

#endif
