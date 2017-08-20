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
#include "parse_error.hpp"
#include "demangle.hpp"

template <typename Rule>
inline bool match(InputReader& reader)
{
    if constexpr(std::is_base_of_v<Terminal, Rule>)
    {
        return reader.fetch<Rule>();
    }
    else
    {
        reader.push_state();
        bool result = Rule::match_impl(reader);
        reader.pop_state();

        return result;
    }
}

[[noreturn]]
inline void fail(InputReader& reader, const std::string& error)
{
    throw_parse_error(reader.current_pos(), error);
}

struct _true
{
    static bool match_impl(InputReader&)
    {
        return true;
    }
};

struct _false
{
    static bool match_impl(InputReader&)
    {
        return false;
    }
};

template <typename... Rules>
struct seq
{
    static bool match_impl(InputReader& reader)
    {
        bool valid = seq_impl<Rules...>(reader);
        if (!valid)
        {
            reader.rewind();
        }

        return valid;
    }

private:
    template <typename Rule, typename... Rest>
    static bool seq_impl(InputReader& reader)
    {
        if constexpr (sizeof...(Rest) == 0)
        {
            return match<Rule>(reader);
        }
        else
        {
            return match<Rule>(reader) && seq_impl<Rest...>(reader);
        }
    }
};

template <typename... Rules>
struct _or
{
    static bool match_impl(InputReader& reader)
    {
        auto prev_policy = reader.failure_policy();
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        return or_impl<Rules...>(reader, prev_policy);
    }

private:
    template <typename... None, typename std::enable_if_t<sizeof...(None) == 0>* = nullptr>
    static bool or_impl(InputReader& reader, InputReader::FailurePolicy prev_policy)
    {
        reader.set_failure_policy(prev_policy);
        return false;
    }

    template <typename Rule, typename... Rest>
    static bool or_impl(InputReader& reader, InputReader::FailurePolicy prev_policy)
    {
        if (match<Rule>(reader))
        {
            return true;
        }
        else
        {
            reader.rewind();
            return or_impl<Rest...>(reader, prev_policy);
        }
    }
};

template <typename Rule>
struct star
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        while (match<Rule>(reader)){}

        return true;
    }
};

template <typename Rule>
struct plus
{
    static bool match_impl(InputReader& reader)
    {
        if (!match<Rule>(reader))
        {
            reader.rewind();
            return false;
        }

        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        while (match<Rule>(reader)) {}

        return true;
    }
};

template <typename Rule>
struct opt
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        if (!match<Rule>(reader))
        {
            reader.rewind();
        }

        return true;
    }
};

template <typename Rule>
struct _and
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        bool result = match<Rule>(reader);
        reader.rewind();
        return result;
    }
};

template <typename Rule>
struct _not
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        bool result = match<Rule>(reader);
        reader.rewind();
        return !result;
    }
};

template <typename... Rule>
struct must
{
    static bool match_impl(InputReader& reader)
    {
        if (!match<seq<Rule...>>(reader))
        {
            fail(reader, "Expected " + demangle<seq<Rule...>>());
        }
        else
        {
            return true;
        }
    }
};

template <typename R, typename... S>
struct if_must : seq<R, must<S...>> {};

#endif // GRAMMAR_HPP
