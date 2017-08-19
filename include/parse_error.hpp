/*  parse_error.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 19
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
#ifndef PARSE_ERROR_HPP
#define PARSE_ERROR_HPP

#include <stdexcept>

#include "fileinfo.hpp"

struct ParseError : public std::runtime_error
{
    ParseError(const std::string& error)
        : std::runtime_error("Parsing error : " + error)
    {

    }
};

[[noreturn]]
inline void throw_parse_error(const FileInfo& pos, const std::string& error)
{
    throw ParseError("In file '" + pos.filename + "', at line " + std::to_string(pos.line)
                     + ", column " + std::to_string(pos.column) + " : " + error);
}

#endif // PARSE_ERROR_HPP
