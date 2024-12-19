// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SEMANTICS_TYPES_HPP
#define ZIV_TOOLCHAIN_SEMANTICS_TYPES_HPP

#include <memory>
#include <unordered_map>

#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::semantics {

class Type {
public:
    enum class Kind : uint8_t {
#define ZIV_TYPE_KIND(NAME) K##NAME,
#include "type_kind_registry.def"
    };

#define ZIV_TYPE_KIND(NAME) static Type* get_##NAME##_type();
#include "type_kind_registry.def"

    static Type* get_type(Kind kind);

    Kind get_kind() const {
        return kind_;
    }
    bool is_error() const {
        return kind_ == Kind::KError;
    }
    bool is_compatible_with(const Type* other) const;

    explicit Type(Kind kind) : kind_(kind) {}

private:
    Kind kind_;
    static std::unordered_map<Kind, std::unique_ptr<Type>> types_;
};

}  // namespace ziv::toolchain::semantics

#endif  // ZIV_TOOLCHAIN_SEMANTICS_TYPES_HPP
