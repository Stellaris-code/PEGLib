/*  lexer.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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
#ifndef LEXER_HPP
#define LEXER_HPP

#include <regex>
#include <memory>
#include <stdexcept>

#include "terminal.hpp"
#include "utils.hpp"

class Lexer
{
public:
    template <typename T>
    void add_def(const std::string& regex)
    {
        static_assert(std::is_base_of_v<Terminal, T>);

        m_token_defs.emplace_back(std::regex{regex}, std::make_unique<T>());
    }

    std::vector<std::unique_ptr<Terminal>> tokenize(std::string_view input, const std::string &file = "<inline>") const;

public:
    struct LexerError : public std::runtime_error
    {
        explicit LexerError(const std::string& str)
         : std::runtime_error(str)
         {

         }
    };

private:
    std::vector<std::pair<std::regex, std::unique_ptr<Terminal>>> m_token_defs;
    const std::regex m_whitespace { "\\s*" };
};

#endif // LEXER_HPP
