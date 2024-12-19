// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "types.hpp"

namespace ziv::toolchain::semantics {

std::unordered_map<Type::Kind, std::unique_ptr<Type>> Type::types_;

#define ZIV_TYPE_KIND(NAME)             \
    Type* Type::get_##NAME##_type() {   \
        return get_type(Kind::K##NAME); \
    }
#include "type_kind_registry.def"

Type* Type::get_type(Kind kind) {
    auto it = types_.find(kind);
    if (it == types_.end()) {
        auto type = std::make_unique<Type>(kind);
        auto* ptr = type.get();
        types_[kind] = std::move(type);
        return ptr;
    }
    return it->second.get();
}

bool Type::is_compatible_with(const Type* other) const {
    if (kind_ == Kind::KNone || other->kind_ == Kind::KNone) {
        return true;
    }
    return kind_ == other->kind_;
}

}  // namespace ziv::toolchain::semantics
