// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_AST_NODE_KIND_HPP
#define ZIV_TOOLCHAIN_AST_NODE_KIND_HPP

#include <cstdint>
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::ast {
    class NodeKind {
        public:
            #define ZIV_NODE_KIND(NAME) \
            static constexpr NodeKind NAME() { return KindEnum::NAME; }
            #include "node_kind_registry.def"

            NodeKind() = delete; // Should be constructed only by static methods

            auto operator==(const NodeKind& other) const -> bool {
                return kind == other.kind;
            }
            auto operator!=(const NodeKind& other) const -> bool {
                return kind != other.kind;
            }

            auto operator< (const NodeKind& other) const -> bool {
                return kind < other.kind;
            }

            llvm::StringRef get_name() const;

        private:
            enum class KindEnum: uint8_t {
                #define ZIV_NODE_KIND(NAME) NAME,
                #include "node_kind_registry.def"
            };

            constexpr NodeKind(KindEnum kind): kind(kind) {}

            explicit constexpr operator KindEnum() const { return kind;}
            KindEnum kind;
    };
} // namespace ziv::toolchain::ast

#endif // ZIV_TOOLCHAIN_AST_NODE_KIND_HPP
