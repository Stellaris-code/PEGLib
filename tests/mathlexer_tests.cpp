// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*  mathlexer_tests.cpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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

#include "mathlexer.hpp"

namespace
{

TEST(NumberMatching, Single)
{
    {
        InputReader reader(math_lex("5"));
        EXPECT_TRUE(match<NumberLitteral>(reader));
    }
    {
        InputReader reader(math_lex("0"));
        EXPECT_TRUE(match<Value>(reader));
    }
}

TEST(NumberMatching, Negative)
{
    InputReader reader(math_lex("-6"));
    EXPECT_TRUE(match<Value>(reader));
}

TEST(NumberMatching, Float)
{
    InputReader reader(math_lex("5.755"));
    EXPECT_TRUE(match<Value>(reader));
}

TEST(NumberMatching, MultipleDigits)
{
    {
        InputReader reader(math_lex("666"));
        EXPECT_TRUE(match<Value>(reader));
    }
    {
        InputReader reader(math_lex("123456"));
        EXPECT_TRUE(match<Value>(reader));
    }
}

TEST(NumberMatching, DoNoMatch)
{
    {
        InputReader reader(math_lex("abc"));
        EXPECT_FALSE(match<Value>(reader));
    }
    {
        InputReader reader(math_lex("as55"));
        EXPECT_FALSE(match<Value>(reader));
    }
    {
        auto read = []{InputReader reader(math_lex("5.5.5"));};
        EXPECT_THROW(read(), Lexer::LexerError);
    }
    {
        InputReader reader(math_lex(""));
        EXPECT_FALSE(match<Value>(reader));
    }
    {
        auto read = []{InputReader reader(math_lex("..."));};
        EXPECT_THROW(read(), Lexer::LexerError);
    }
    {
        auto read = []{InputReader reader(math_lex("5_5"));};
        EXPECT_THROW(read(), Lexer::LexerError);
    }
}

TEST(MathLex, True)
{
    {
        InputReader reader(math_lex("1"));
        EXPECT_TRUE(match<Number>(reader));
    }
    {
        InputReader reader(math_lex("gilbert"));
        EXPECT_TRUE(match<Alpha>(reader));
    }
    {
        InputReader reader(math_lex("("));
        EXPECT_TRUE(match<LeftParenthesis>(reader));
    }
    {
        InputReader reader(math_lex(")"));
        EXPECT_TRUE(match<RightParenthesis>(reader));
    }
    {
        InputReader reader(math_lex(";"));
        EXPECT_TRUE(match<Semicolon>(reader));
    }
    {
        InputReader reader(math_lex(">"));
        EXPECT_TRUE(match<RightBracket>(reader));
    }
    {
        InputReader reader(math_lex("="));
        EXPECT_TRUE(match<EqualOperator>(reader));
    }
    {
        InputReader reader(math_lex("+"));
        EXPECT_TRUE(match<SumOperator>(reader));
    }
    {
        InputReader reader(math_lex("-"));
        EXPECT_TRUE(match<SumOperator>(reader));
    }
    {
        InputReader reader(math_lex("*"));
        EXPECT_TRUE(match<ProductOperator>(reader));
    }
    {
        InputReader reader(math_lex("/"));
        EXPECT_TRUE(match<ProductOperator>(reader));
    }
}

TEST(MathMatching, True)
{
    {
        InputReader reader(math_lex("5 / 3"));
        EXPECT_TRUE(match<Product>(reader));
    }
    {
        InputReader reader(math_lex("5 + 3"));
        EXPECT_TRUE(match<Sum>(reader));
    }
    {
        InputReader reader(math_lex("5 + 3 - 2"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("5 + 3 - 2 +"));
        EXPECT_THROW(match<Expr>(reader), ParseError);
    }
    {
        InputReader reader(math_lex("5 + 3 - 2 +/"));
        EXPECT_THROW(match<Expr>(reader), ParseError);
    }
    {
        InputReader reader(math_lex("-5-"));
        EXPECT_THROW(match<Expr>(reader), ParseError);
    }
    {
        InputReader reader(math_lex(""));
        EXPECT_FALSE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("+"));
        EXPECT_FALSE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("-5.311*-2"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("-5*(2/5)"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("-5*-(2/-5)"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("(2/-5.3)"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("0"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("(0)"));
        EXPECT_TRUE(match<Expr>(reader));
    }
    {
        InputReader reader(math_lex("(2/-5)+3"));
        EXPECT_TRUE(match<Expr>(reader));
    }
}

}
