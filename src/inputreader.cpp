/*  inputreader.cpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 17
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

#include "inputreader.hpp"

#include <cassert>
#include <algorithm>

InputReader::InputReader(std::vector<std::unique_ptr<Terminal>>&& symbols)
{
    m_terminals = std::move(symbols);
    std::reverse(m_terminals.begin(), m_terminals.end());
    clear_states();
    reset_lookahead();
}

void InputReader::reset_lookahead()
{
    m_lookahead_pos = m_terminals.end();
}

void InputReader::push_state()
{
    // TODO : implémenter !
}

void InputReader::pop_state()
{
    // TODO : implémenter !
}

void InputReader::clear_states()
{
   decltype(m_base_pos)().swap(m_base_pos);
   m_base_pos.push(m_terminals.begin());
}

void InputReader::consume()
{
    if (m_policy == InputReader::Policy::Consume)
    {
        while (m_terminals.end() != m_lookahead_pos)
        {
            assert(!m_terminals.empty());

            m_terminals.pop_back();
        }
    }
}

void InputReader::set_policy(InputReader::Policy policy)
{
    m_policy = policy;
}

InputReader::Policy InputReader::policy() const
{
    return m_policy;
}
