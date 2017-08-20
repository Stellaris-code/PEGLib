/*  action.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 20
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
#ifndef ACTION_HPP
#define ACTION_HPP

#include <vector>
#include <memory>

#include "terminal.hpp"

template <typename Rule>
struct Action
{
    template <typename... Data>
    static void apply(const std::vector<std::unique_ptr<Terminal>>&, Data&&...)
    {

    }
};

#endif // ACTION_HPP
