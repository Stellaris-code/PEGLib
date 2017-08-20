// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*  grammar_tests.cpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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

#include <typeinfo>
#include <algorithm>

#include "gtest/gtest.h"

#include "grammar.hpp"
#include "inputreader.hpp"

MAKE_TERMINAL(DummyToken1)
MAKE_TERMINAL(DummyToken2)
MAKE_TERMINAL(DummyToken3)


template<class TContainer>
bool begins_with(const TContainer& input, const TContainer& match)
{
    return input.size() >= match.size()
            && equal(match.begin(), match.end(), input.begin());
}

namespace
{
TEST(Lits, True)
{
    InputReader r;
    ASSERT_TRUE(match<_true>(r));
}

TEST(Lits, False)
{
    InputReader r;
    ASSERT_FALSE(match<_false>(r));
}

TEST(SeqTest, True)
{
    InputReader r;
    EXPECT_TRUE((match<seq<_true>>(r)));
    EXPECT_TRUE((match<seq<_true, _true, _true>>(r)));
    EXPECT_TRUE((match<seq<_true, _true, _true, _true, _true>>(r)));
}

TEST(SeqTest, False)
{
    InputReader r;
    EXPECT_FALSE((match<seq<_false>>(r)));
    EXPECT_FALSE((match<seq<_false, _false>>(r)));
    EXPECT_FALSE((match<seq<_false, _true, _true>>(r)));
}

TEST(OrTest, True)
{
    InputReader r;
    EXPECT_TRUE((match<_or<_true>>(r)));
    EXPECT_TRUE((match<_or<_true, _true, _true>>(r)));
    EXPECT_TRUE((match<_or<_true, _true, _false, _true, _true>>(r)));
}

TEST(OrTest, False)
{
    InputReader r;
    EXPECT_FALSE((match<_or<_false>>(r)));
    EXPECT_FALSE((match<_or<_false, _false, _false>>(r)));
}

TEST(AndTest, True)
{
    std::vector<std::unique_ptr<Terminal>> tokens;
    tokens.emplace_back(std::make_unique<DummyToken1>());

    InputReader reader(std::move(tokens));

    EXPECT_TRUE(match<_and<DummyToken1>>(reader));
    EXPECT_TRUE(match<_and<DummyToken1>>(reader));
    reader.rewind();
    EXPECT_TRUE(match<_and<DummyToken1>>(reader));
}

TEST(AndTest, False)
{
    std::vector<std::unique_ptr<Terminal>> tokens;
    tokens.emplace_back(std::make_unique<DummyToken2>());
    tokens.emplace_back(std::make_unique<DummyToken1>());

    InputReader reader(std::move(tokens));

    EXPECT_FALSE(match<_and<DummyToken1>>(reader));
    EXPECT_FALSE(match<_and<DummyToken1>>(reader));
    reader.rewind();
    EXPECT_FALSE(match<_and<DummyToken1>>(reader));
}

TEST(NotTest, True)
{
    std::vector<std::unique_ptr<Terminal>> tokens;
    tokens.emplace_back(std::make_unique<DummyToken2>());
    tokens.emplace_back(std::make_unique<DummyToken1>());

    InputReader reader(std::move(tokens));

    EXPECT_TRUE(match<_not<DummyToken1>>(reader));
    EXPECT_TRUE(match<_not<DummyToken1>>(reader));
    reader.rewind();
    EXPECT_TRUE(match<_not<DummyToken1>>(reader));
}

TEST(NotTest, False)
{
    std::vector<std::unique_ptr<Terminal>> tokens;
    tokens.emplace_back(std::make_unique<DummyToken2>());
    tokens.emplace_back(std::make_unique<DummyToken1>());

    InputReader reader(std::move(tokens));

    EXPECT_FALSE(match<_not<DummyToken2>>(reader));
    EXPECT_FALSE(match<_not<DummyToken2>>(reader));
    reader.rewind();
    EXPECT_FALSE(match<_not<DummyToken2>>(reader));
}

TEST(PlusTest, True)
{
    std::vector<std::unique_ptr<Terminal>> tokens;
    tokens.emplace_back(std::make_unique<DummyToken1>());
    tokens.emplace_back(std::make_unique<DummyToken2>());
    tokens.emplace_back(std::make_unique<DummyToken1>());

    InputReader reader(std::move(tokens));

    EXPECT_TRUE(match<plus<DummyToken1>>(reader));
    EXPECT_TRUE(match<plus<DummyToken2>>(reader));
    EXPECT_TRUE(match<plus<DummyToken1>>(reader));
}

TEST(PlusTest, False)
{
    {
        std::vector<std::unique_ptr<Terminal>> tokens;
        tokens.emplace_back(std::make_unique<DummyToken2>());

        InputReader reader(std::move(tokens));

        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        EXPECT_FALSE(match<plus<DummyToken1>>(reader));
    }
    {
        InputReader reader;

        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        EXPECT_FALSE(match<plus<DummyToken1>>(reader));
    }
}

TEST(Terminal, Names)
{
    Terminal term;
    EXPECT_TRUE(begins_with(term.dyn_name(), Terminal::name()));

    DummyToken1 token1;
    EXPECT_TRUE(begins_with(token1.dyn_name(), DummyToken1::name()));

    DummyToken2 token2;
    EXPECT_TRUE(begins_with(token2.dyn_name(), DummyToken2::name()));
}

TEST(Terminal, Clone)
{
    Terminal term;

    DummyToken1 token1;
    token1.data = "Test";

    auto clone = token1.clone();

    EXPECT_TRUE(typeid(term.clone()) == typeid(std::unique_ptr<Terminal>));
    EXPECT_TRUE(typeid(clone) == typeid(std::unique_ptr<Terminal>));
    EXPECT_TRUE(token1.data == clone->data);
    EXPECT_TRUE(begins_with(clone->dyn_name(), DummyToken1::name()));
}
}
