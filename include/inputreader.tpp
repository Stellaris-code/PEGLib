/*  inputreader.tpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 17
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
#include <cassert>
#include <iostream>

#include "parse_error.hpp"

#include "terminal.hpp"

template <typename T>
const Terminal* InputReader::fetch()
{
    static_assert(std::is_base_of_v<Terminal, T>);
    if (m_lookahead_pos == m_terminals.begin())
    {
        if (failure_policy() == FailurePolicy::Permissive)
        {
            return nullptr;
        }
        else
        {
            throw_parse_error(current_pos(), "Expected '" + T::name() + "' got 'End Of File' !");
        }
    }

    if (typeid(T) != typeid(**(m_lookahead_pos - 1)))
    {
        if (failure_policy() == FailurePolicy::Permissive)
        {
            return nullptr;
        }
        else
        {
            throw_parse_error(current_pos(), "Expected '" + T::name() + "' got '" + (*(m_lookahead_pos - 1))->dyn_name() + "' !");
        }
    }

    //std::cout << "Symbol read : " << (*(m_lookahead_pos - 1))->dyn_name() << "\n";

    return (*(--m_lookahead_pos)).get();
}
