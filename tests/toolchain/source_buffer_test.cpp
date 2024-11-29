// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <gtest/gtest.h>
#include "toolchain/source/source_buffer.hpp"
#include "llvm/Support/VirtualFileSystem.h"

namespace {
    using namespace ziv::toolchain::source;

    class SourceBufferTest : public ::testing::Test {
    protected:
        llvm::vfs::InMemoryFileSystem fs;

        void SetUp() override {
            // Create some test files in the virtual filesystem
            fs.addFile("/test/valid.ziv", 0, 
                llvm::MemoryBuffer::getMemBuffer("test content"));
            fs.addFile("/test/valid.z", 0,
                llvm::MemoryBuffer::getMemBuffer("another test"));
            fs.addFile("/test/invalid.txt", 0,
                llvm::MemoryBuffer::getMemBuffer("invalid file"));
            fs.addFile("/test/empty.ziv", 0,
                llvm::MemoryBuffer::getMemBuffer(""));
        }
    };

    TEST_F(SourceBufferTest, ValidExtensions) {
        // Test valid .ziv file
        auto buffer1 = SourceBuffer::from_file(fs, "/test/valid.ziv");
        ASSERT_TRUE(buffer1.has_value());
        EXPECT_EQ(buffer1->get_contents(), "test content");
        EXPECT_EQ(buffer1->get_filename(), "/test/valid.ziv");

        // Test valid .z file
        auto buffer2 = SourceBuffer::from_file(fs, "/test/valid.z");
        ASSERT_TRUE(buffer2.has_value());
        EXPECT_EQ(buffer2->get_contents(), "another test");
        EXPECT_EQ(buffer2->get_filename(), "/test/valid.z");
    }

    TEST_F(SourceBufferTest, InvalidExtension) {
        auto buffer = SourceBuffer::from_file(fs, "/test/invalid.txt");
        EXPECT_FALSE(buffer.has_value());
    }

    TEST_F(SourceBufferTest, EmptyFile) {
        auto buffer = SourceBuffer::from_file(fs, "/test/empty.ziv");
        EXPECT_FALSE(buffer.has_value());
    }

    TEST_F(SourceBufferTest, NonexistentFile) {
        auto buffer = SourceBuffer::from_file(fs, "/test/nonexistent.ziv");
        EXPECT_FALSE(buffer.has_value());
    }
}
