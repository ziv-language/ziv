// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "zivc/command/command_manager.hpp"


namespace ziv::cli::toolchain
{
    class MockToolchainDriver {
        public:
            MOCK_METHOD(void, run, (const std::string &command, const std::string &filename), ());
    };
} // namespace ziv::cli::toolchain

namespace ziv::cli::command
{

    class MockCommandManager : public CommandManager {
        public:
            explicit MockCommandManager(toolchain::MockToolchainDriver &mockDriver): mockDriver(mockDriver) {}

        protected:
            void handle_source(const std::string &filename) override {
                mockDriver.run("source", filename);
            }

            void handle_lexer(const std::string &filename) override {
                mockDriver.run("lexer", filename);
            }

            void handle_parser(const std::string &filename) override {
                mockDriver.run("parser", filename);
            }

        private:
            toolchain::MockToolchainDriver &mockDriver;
        };


    // Fixture for tests
    class CommandManagerTest : public ::testing::Test {
        protected:
            toolchain::MockToolchainDriver mockDriver;
            MockCommandManager commandManager{mockDriver};
    };


    // Unit tests
    TEST_F(CommandManagerTest, ExecuteSourceCommand)
    {
        const char *argv[] = {"program", "toolchain", "--source", "input.ziv"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        EXPECT_CALL(mockDriver, run("source", "input.ziv")).Times(1);

        commandManager.execute(argc, const_cast<char **>(argv));
    }

    TEST_F(CommandManagerTest, ExecuteLexerCommand)
    {
        const char *argv[] = {"program", "toolchain", "--lexer", "input.ziv"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        EXPECT_CALL(mockDriver, run("lexer", "input.ziv")).Times(1);

        commandManager.execute(argc, const_cast<char **>(argv));
    }

    TEST_F(CommandManagerTest, ExecuteWithoutCommand)
    {
        const char *argv[] = {"program"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        EXPECT_CALL(mockDriver, run(::testing::_, ::testing::_)).Times(0);

        testing::internal::CaptureStderr();
        commandManager.execute(argc, const_cast<char **>(argv));
        std::string output = testing::internal::GetCapturedStderr();

        EXPECT_TRUE(output.find("Error: No command specified") != std::string::npos);
    }
} // namespace ziv::cli::command
