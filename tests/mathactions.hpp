/*  mathactions.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 20
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
#ifndef MATHACTIONS_HPP
#define MATHACTIONS_HPP

#include "action.hpp"
#include "mathgrammar.hpp"

template <>
struct Action<NumberLitteral>
{
    static void apply(const std::vector<std::unique_ptr<Terminal>>& input, std::string& str)
    {
        assert(!input.empty());

        str = input.back()->data;
    }
};

#endif // MATHACTIONS_HPP
