// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "terminal.hpp"

InputReader::InputReader(std::vector<std::unique_ptr<Terminal>>&& symbols)
{
    m_terminals = std::move(symbols);
    std::reverse(m_terminals.begin(), m_terminals.end());
    clear_states();
    m_lookahead_pos = m_terminals.end();
}

void InputReader::rewind()
{
    assert(!m_base_pos.empty());
    while (m_lookahead_pos != m_base_pos.top())
    {
        ++m_lookahead_pos;
    }
}

void InputReader::push_state()
{
    m_base_pos.emplace(m_lookahead_pos);
}

void InputReader::pop_state()
{
    m_base_pos.pop();
}

void InputReader::clear_states()
{
    decltype(m_base_pos)().swap(m_base_pos);
    m_base_pos.emplace(m_terminals.end());
}

void InputReader::set_failure_policy(InputReader::FailurePolicy policy)
{
    m_failure_policy = policy;
}

InputReader::FailurePolicy InputReader::failure_policy() const
{
    return m_failure_policy;
}

std::vector<std::unique_ptr<Terminal>> InputReader::current_input() const
{
    assert(!m_base_pos.empty());

    std::vector<std::unique_ptr<Terminal>> terms;

    for (auto it = m_lookahead_pos; it != m_base_pos.top(); ++it)
    {
        terms.emplace_back((*it)->clone());
    }

    return terms;
}

FileInfo InputReader::current_pos() const
{
    if (m_lookahead_pos == m_terminals.begin())
    {
        return FileInfo{};
    }
    else if (m_lookahead_pos == m_terminals.end())
    {
        if (!m_terminals.empty())
        {
            return FileInfo{m_terminals.back()->fileinfo.filename, 0, 0}; // return beggining of file
        }
        else
        {
            return FileInfo{};
        }
    }
    else
    {
        return (*m_lookahead_pos)->fileinfo;
    }
}
