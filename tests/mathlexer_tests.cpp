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
        EXPECT_TRUE(Value::match(reader));
    }
    {
        InputReader reader(math_lex("0"));
        EXPECT_TRUE(Value::match(reader));
    }
}

TEST(NumberMatching, Negative)
{
    InputReader reader(math_lex("-6"));
    EXPECT_TRUE(Value::match(reader));
}

TEST(NumberMatching, Float)
{
    InputReader reader(math_lex("5.755"));
    EXPECT_TRUE(Value::match(reader));
}

TEST(NumberMatching, MultipleDigits)
{
    {
        InputReader reader(math_lex("666"));
        EXPECT_TRUE(Value::match(reader));
    }
    {
        InputReader reader(math_lex("123456"));
        EXPECT_TRUE(Value::match(reader));
    }
}

TEST(NumberMatching, DoNoMatch)
{
    {
        InputReader reader(math_lex("abc"));
        EXPECT_FALSE(Value::match(reader));
    }
    {
        InputReader reader(math_lex("as55"));
        EXPECT_FALSE(Value::match(reader));
    }
    {
        auto read = []{InputReader reader(math_lex("5.5.5"));};
        EXPECT_THROW(read(), Lexer::LexerError);
    }
    {
        InputReader reader(math_lex(""));
        EXPECT_FALSE(Value::match(reader));
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
        EXPECT_TRUE(term<Number>::match(reader));
    }
    {
        InputReader reader(math_lex("gilbert"));
        EXPECT_TRUE(term<Alpha>::match(reader));
    }
    {
        InputReader reader(math_lex("("));
        EXPECT_TRUE(term<LeftParenthesis>::match(reader));
    }
    {
        InputReader reader(math_lex(")"));
        EXPECT_TRUE(term<RightParenthesis>::match(reader));
    }
    {
        InputReader reader(math_lex(";"));
        EXPECT_TRUE(term<Semicolon>::match(reader));
    }
    {
        InputReader reader(math_lex(">"));
        EXPECT_TRUE(term<RightBracket>::match(reader));
    }
    {
        InputReader reader(math_lex("="));
        EXPECT_TRUE(term<EqualOperator>::match(reader));
    }
    {
        InputReader reader(math_lex("+"));
        EXPECT_TRUE(term<SumOperator>::match(reader));
    }
    {
        InputReader reader(math_lex("-"));
        EXPECT_TRUE(term<SumOperator>::match(reader));
    }
    {
        InputReader reader(math_lex("*"));
        EXPECT_TRUE(term<ProductOperator>::match(reader));
    }
    {
        InputReader reader(math_lex("/"));
        EXPECT_TRUE(term<ProductOperator>::match(reader));
    }
}

TEST(MathMatching, True)
{
    {
        InputReader reader(math_lex("5 + 3 - 2"));
        EXPECT_TRUE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("5 + 3 - 2 +"));
        EXPECT_FALSE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex(""));
        EXPECT_FALSE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("+"));
        EXPECT_FALSE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("-5-"));
        EXPECT_FALSE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("-5.311*-2"));
        EXPECT_TRUE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("-5*-(2/-5)"));
        EXPECT_TRUE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("(2/-5.3)"));
        EXPECT_TRUE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("0"));
        EXPECT_TRUE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("(0)"));
        EXPECT_TRUE(Expr::match(reader));
    }
    {
        InputReader reader(math_lex("(2/-5)+3"));
        EXPECT_TRUE(Expr::match(reader));
    }
}

}
