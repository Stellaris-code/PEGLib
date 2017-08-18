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

#include "gtest/gtest.h"

#include "grammar.hpp"
#include "inputreader.hpp"

namespace
{
TEST(Lits, True)
{
    InputReader r;
    ASSERT_TRUE(_true::match(r));
}

TEST(Lits, False)
{
    InputReader r;
    ASSERT_FALSE(_false::match(r));
}

TEST(SeqTest, True)
{
    InputReader r;
    EXPECT_TRUE((seq<_true>::match(r)));
    EXPECT_TRUE((seq<_true, _true, _true>::match(r)));
    EXPECT_TRUE((seq<_true, _true, _true, _true, _true>::match(r)));
}

TEST(SeqTest, False)
{
    InputReader r;
    EXPECT_FALSE((seq<_false>::match(r)));
    EXPECT_FALSE((seq<_false, _false>::match(r)));
    EXPECT_FALSE((seq<_false, _true, _true>::match(r)));
}

TEST(OrTest, True)
{
    InputReader r;
    EXPECT_TRUE((_or<_true>::match(r)));
    EXPECT_TRUE((_or<_true, _true, _true>::match(r)));
    EXPECT_TRUE((_or<_true, _true, _false, _true, _true>::match(r)));
}

TEST(OrTest, False)
{
    InputReader r;
    EXPECT_FALSE((_or<_false>::match(r)));
    EXPECT_FALSE((_or<_false, _false, _false>::match(r)));
}
}
