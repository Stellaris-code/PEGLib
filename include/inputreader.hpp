/*  include/inputreader.hpp InputReader - Yann BOUCHER (yann) 17
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
#ifndef INPUTREADER_HPP
#define INPUTREADER_HPP

#include <vector>
#include <memory>

#include "terminal.hpp"

class InputReader
{
public:
    enum class Policy
    {
        Consume,
        Ignore
    };

public:
    template <typename T>
    const Terminal& lookahead();

    void consume();

    void reset_lookahead();

    void set_policy(Policy policy);

    Policy policy() const;

private:
    Policy m_policy { Policy::Ignore };

    /// used in a FIFO way
    std::vector<std::unique_ptr<Terminal>> m_terminals;
    decltype(m_terminals)::iterator m_lookahead_pos { m_terminals.end() };
};

#include "inputreader.tpp"

#endif // INPUTREADER_HPP
