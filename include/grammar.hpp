/*  grammar.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 17
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
#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <type_traits>

#include "inputreader.hpp"

struct _true
{
    static bool match(InputReader&)
    {
        return true;
    }
};

struct _false
{
    static bool match(InputReader&)
    {
        return false;
    }
};

template <typename... Rules>
struct seq
{
    static bool match(InputReader& reader)
    {
        auto last_state = reader.policy(); // TODO : faire un système de pile de states
        reader.set_policy(InputReader::Policy::Ignore);
        bool valid = seq_impl<Rules...>(reader);
        reader.set_policy(last_state);
        if (valid)
        {
            reader.consume();
        }

        return valid;
    }

private:
    template <typename Rule, typename... Rest, typename std::enable_if_t<sizeof...(Rest) == 0>* = nullptr>
    static bool seq_impl(InputReader& reader)
    {
        return Rule::match(reader);
    }

    template <typename Rule, typename... Rest, typename std::enable_if_t<sizeof...(Rest) != 0>* = nullptr>
    static bool seq_impl(InputReader& reader)
    {
        return Rule::match(reader) && seq_impl<Rest...>(reader);
    }
};

// TODO : implémenter _or correctement !
template <typename... Rules>
struct _or
{
    static bool match(InputReader& reader)
    {
        return or_impl<Rules...>(reader);
    }

private:
    template <typename... None, typename std::enable_if_t<sizeof...(None) == 0>* = nullptr>
    static bool or_impl(InputReader&)
    {
        return false;
    }

    template <typename Rule, typename... Rest>
    static bool or_impl(InputReader& reader)
    {
        if (Rule::match(reader))
        {
            reader.consume();
            return true;
        }
        else
        {
            reader.reset_lookahead();
            return or_impl<Rest...>(reader);
        }
    }
};

template <typename Rule>
struct star
{
    static bool match(InputReader& reader)
    {
        while (Rule::match(reader)){}

        reader.consume();

        return true;
    }
};

template <typename Rule>
struct plus
{
    static bool match(InputReader& reader)
    {
        if (!Rule::match(reader)) return false;

        while (Rule::match(reader)) {}

        reader.consume();

        return true;
    }
};

template <typename Rule>
struct opt
{
    static bool match(InputReader& reader)
    {
        if (Rule::match(reader))
        {
            reader.consume();
            return true;
        }
        else
        {
            return false;
        }
    }
};

template <typename Rule>
struct _and
{
    static bool match(InputReader& reader)
    {
        bool result = Rule::match(reader);
        reader.reset_lookahead();
        return result;
    }
};

template <typename Rule>
struct _not
{
    static bool match(InputReader& reader)
    {
        bool result = Rule::match(reader);
        reader.reset_lookahead();
        return !result;
    }
};

#endif // GRAMMAR_HPP
