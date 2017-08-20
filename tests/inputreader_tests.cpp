/*  inputreader_tests.cpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 20
**
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**                    Version 2, December 2004
**
** Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
**
** Everyone is permitted to copy and distribute verbatim or modified
** copies of this license document, and changing it is allowed as long
** as the name is changed.
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
**
**  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include "gtest/gtest.h"

#include "inputreader.hpp"
#include "terminal.hpp"

MAKE_TERMINAL(DummyToken)
MAKE_TERMINAL(TargetToken)

namespace
{

    TEST(InputReaderFailure, NoMoreTokens)
    {
        InputReader reader;
        EXPECT_THROW(reader.fetch<Terminal>(), ParseError);
    }

    TEST(InputReaderFailure, InvalidToken)
    {
        std::vector<std::unique_ptr<Terminal>> tokens;
        tokens.emplace_back(std::make_unique<DummyToken>());

        InputReader reader(std::move(tokens));
        reader.set_failure_policy(InputReader::FailurePolicy::Strict);

        EXPECT_THROW(reader.fetch<TargetToken>(), ParseError);
    }
}
