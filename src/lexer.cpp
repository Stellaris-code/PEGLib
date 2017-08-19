/*  lexer.cpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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

#include "lexer.hpp"

#include "utils.hpp"

#include <iostream>

std::vector<std::unique_ptr<Terminal>> Lexer::tokenize(std::string_view input,
                                                       const std::string& file) const
{
    std::vector<std::unique_ptr<Terminal>> symbols;
    auto lines = split_string(input, "\n");
    for (size_t i { 0 }; i < lines.size(); ++i)
    {
        int column { 0 };
        auto tokens = ::tokenize(lines[i]);
        for (auto token : tokens)
        {
            if (is_whitespace(token)) continue;

reloop:
            for (const auto& pair : m_token_defs)
            {
                const auto& rx = pair.first;
                std::smatch match;
                if (std::regex_search(token, match, rx, std::regex_constants::match_continuous))
                {
                    symbols.emplace_back(pair.second->clone());
                    symbols.back()->fileinfo = {file, int(i + 1), column};
                    symbols.back()->data = match.str();
                    column += match.length();
                    token = match.suffix();
                    if (token.empty())
                    {
                        goto exit;
                    }
                    else
                    {
                        goto reloop;
                    }
                }
            }

            throw LexerError("Unrecognized token '" + token + "'");

exit:
            ;
        }
    }

    for (auto& sym : symbols)
    {
        std::cout << sym->name() << "\n";
    }

    return symbols;
}
