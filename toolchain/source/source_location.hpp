// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_SOURCE_LOCATION_HPP
#define ZIV_TOOLCHAIN_SOURCE_LOCATION_HPP

#pragma once
#include <cstddef>
#include <string_view>

namespace ziv::toolchain::source {

struct SourceLocation {
    std::string_view file;
    std::size_t line{1};
    std::size_t column{1};
    std::size_t offset{0};
    std::size_t length{0};

    auto operator<=>(const SourceLocation&) const = default;
};

}  // namespace ziv::toolchain::source

#endif  // ZIV_TOOLCHAIN_SOURCE_LOCATION_HPP
