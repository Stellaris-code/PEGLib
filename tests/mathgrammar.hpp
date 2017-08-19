/*  mathgrammar.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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
#ifndef MATHGRAMMAR_HPP
#define MATHGRAMMAR_HPP

#include "grammar.hpp"

/// Grammar :
/*
program    ::= statement+
statement  ::= (assignement | call) ";"
assignement::= ident "=" expression
call       ::= ident "(" expr ")"
expression ::= term  (("+" | "-") term)*
term       ::= factor (("*"|"/") factor)*
factor     ::= ("+"/"-") (constant | variable | "("  expression  ")")
variable   ::= "x" | "y" | "z"
constant   ::= digit+
ident      ::= alpha alphanum*
alphanum   ::= alpha | digit
alpha      ::= "a" | "z"
digit      ::= "0" | "1" | "9"
*/

MAKE_TERMINAL(Number)
MAKE_TERMINAL(Alpha)
MAKE_TERMINAL(LeftParenthesis)
MAKE_TERMINAL(RightParenthesis)
MAKE_TERMINAL(RightBracket)
MAKE_TERMINAL(Semicolon)
MAKE_TERMINAL(EqualOperator)
MAKE_TERMINAL(SumOperator)
MAKE_TERMINAL(ProductOperator)

struct NumberLitteral : public term<Number>
{

};

struct Value;

struct Product : public seq<Value, star<seq<term<ProductOperator>, Product>>>
{

};
struct Sum : public seq<Product, star<seq<term<SumOperator>, Sum>>>
{

};
struct Expr : public Sum
{

};
struct Value : public seq<opt<term<SumOperator>>, _or<NumberLitteral, seq<term<LeftParenthesis>, Expr, term<RightParenthesis>>>>
{

};

#endif // MATHGRAMMAR_HPP
