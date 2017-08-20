/*  mathlexer.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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
#ifndef MATHLEXER_HPP
#define MATHLEXER_HPP

#include "lexer.hpp"
#include "mathgrammar.hpp"

inline std::vector<std::unique_ptr<Terminal>> math_lex(std::string_view input)
{
    Lexer lexer;
    lexer.add_def<Number>("\\d+\\.?\\d*");
    lexer.add_def<Alpha>("[a-zA-Z]+");
    lexer.add_def<LeftParenthesis>("\\(");
    lexer.add_def<RightParenthesis>("\\)");
    lexer.add_def<Semicolon>(";");
    lexer.add_def<RightBracket>(">");
    lexer.add_def<EqualOperator>("=");
    lexer.add_def<SumOperator>("\\+|\\-");
    lexer.add_def<ProductOperator>("\\*|\\/");

    return lexer.tokenize(input);
}

#endif // MATHLEXER_HPP
