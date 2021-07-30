// Copyright 2009-2021 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2021, NTESS
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef SST_CORE_SERIALIZATION_SERIALIZABLE_FWD_H
#define SST_CORE_SERIALIZATION_SERIALIZABLE_FWD_H

#include "sst/core/serialization/serializer_fwd.h"
#include <unordered_map>
#include <string>

namespace SST {
namespace Core {
namespace Serialization {


class serializable
{
public:
    static constexpr uint32_t NullClsId = std::numeric_limits<uint32_t>::max();

    virtual const char* cls_name() const = 0;

    virtual void serialize_order(serializer& ser) = 0;

    virtual uint32_t    cls_id() const             = 0;
    virtual std::string serialization_name() const = 0;

    virtual ~serializable() {}

protected:
    typedef enum { ConstructorFlag } cxn_flag_t;
    static void serializable_abort(uint32_t line, const char* file, const char* func, const char* obj);
};

class serializable_builder
{
public:
    virtual serializable* build() const = 0;

    virtual ~serializable_builder() {}

    virtual const char* name() const = 0;

    virtual uint32_t cls_id() const = 0;

    virtual bool sanity(serializable* ser) = 0;
};

class serializable_factory
{
protected:
    typedef std::unordered_map<long, serializable_builder*> builder_map;
    static builder_map*                                     builders_;

public:
    static serializable* get_serializable(uint32_t cls_id);

    /**
       @return The cls id for the given builder
    */
    static uint32_t
    // add_builder(serializable_builder* builder, uint32_t cls_id);
    add_builder(serializable_builder* builder, const char* name);

    static bool sanity(serializable* ser, uint32_t cls_id) { return (*builders_)[cls_id]->sanity(ser); }

    static void delete_statics();
};

template <class T>
class serializable_builder_impl : public serializable_builder
{
protected:
    static const char*    name_;
    static const uint32_t cls_id_;

public:
    serializable* build() const override { return T::construct_deserialize_stub(); }

    const char* name() const override { return name_; }

    uint32_t cls_id() const override { return cls_id_; }

    static uint32_t static_cls_id() { return cls_id_; }

    static const char* static_name() { return name_; }

    bool sanity(serializable* ser) override { return (typeid(T) == typeid(*ser)); }
};


template <class T>
const char* serializable_builder_impl<T>::name_ = typeid(T).name();
template <class T>
const uint32_t serializable_builder_impl<T>::cls_id_
    = serializable_factory::add_builder(new serializable_builder_impl<T>, typeid(T).name());

template <class T>
class serializable_type
{};

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

} // namespace Serialization
} // namespace Core
} // namespace SST

#endif // SST_CORE_SERIALIZATION_SERIALIZABLE_FWD_H
