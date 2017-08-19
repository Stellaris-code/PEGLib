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

#include <stdexcept>
#include <vector>
#include <stack>
#include <memory>

#include "terminal.hpp"

class InputReader
{
public:
    enum class FailurePolicy
    {
        Permissive,
        Strict
    };

public:
    InputReader() = default;
    explicit InputReader(std::vector<std::unique_ptr<Terminal>>&& symbols);

    template <typename T>
    const Terminal* fetch();

    void rewind();

    void push_state();
    void pop_state();
    void clear_states();

    void set_failure_policy(FailurePolicy policy);
    FailurePolicy failure_policy() const;

    FileInfo current_pos() const;

private:    
    FailurePolicy m_failure_policy { FailurePolicy::Strict };

    /// used in a FIFO way
    std::vector<std::unique_ptr<Terminal>> m_terminals;
    using terminal_iterator = decltype(m_terminals)::iterator;


    terminal_iterator m_lookahead_pos { m_terminals.end() };
    std::stack<terminal_iterator> m_base_pos { {m_terminals.begin()} };
};

#include "inputreader.tpp"

#endif // INPUTREADER_HPP
