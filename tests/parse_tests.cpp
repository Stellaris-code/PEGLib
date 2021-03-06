/*  parse_tests.cpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 20
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

#include "parse.hpp"
#include "mathlexer.hpp"
#include "mathactions.hpp"

namespace
{
    TEST(ParseTest, MathValueEvalPositive)
    {
        InputReader reader(math_lex("6"));
        std::string str;

        EXPECT_TRUE(parse<Expr>(reader, str));
        EXPECT_EQ(str, "6");
    }
    TEST(ParseTest, MathValueEvalFloat)
    {
        InputReader reader(math_lex("50.11"));
        std::string str;

        EXPECT_TRUE(parse<Expr>(reader, str));
        EXPECT_EQ(str, "50.11");
    }
}
