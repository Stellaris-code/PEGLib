/*  utils.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18
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
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <string_view>
#include <algorithm>
#include <vector>

inline bool is_whitespace(std::string_view str)
{
    return std::all_of(str.begin(), str.end(), [](char c)
    {
        return std::isspace(c);
    });
}

inline std::vector<std::string_view> split_string(std::string_view str,
        std::string_view delimiter)
{
    std::vector<std::string_view> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

inline std::vector<std::string> tokenize(std::string_view str)
{
    std::vector<std::string> tokens;
    std::string buffer;
    auto iter = str.cbegin();

    bool in_string = false;
    bool in_spaces = false;

    while (iter != str.cend())
    {
        char c = *iter;
        if (std::isspace(c))
        {
            if (in_string)
            {
                buffer.push_back(c);
            }
            else
            {
                if (!in_spaces)
                {
                    in_spaces = true;
                    tokens.push_back(buffer);
                    buffer.clear();
                }
                else
                {
                    buffer.push_back(c);
                }
            }
        }
        else
        {
            if (in_spaces)
            {
                in_spaces = false;
                tokens.push_back(buffer);
                buffer.clear();
            }
        }
        if (c == '"')
        {
            if (in_string)
            {
                tokens.push_back(c + buffer + c);
                buffer.clear();
            }
            in_string = !in_string;
        }
        else
        {
            buffer.push_back(c);
        }

        ++iter;
    }

    if (!buffer.empty())
    {
        tokens.push_back(buffer);
    }

    return tokens;
}

#endif // UTILS_HPP
