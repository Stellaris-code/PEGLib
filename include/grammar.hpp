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
#include "action.hpp"

template <typename Rule, typename Data>
inline bool match(InputReader& reader, Data&& data)
{
    if constexpr(std::is_base_of_v<Terminal, Rule>)
    {
        return reader.fetch<Rule>();
    }
    else
    {
        reader.push_state();
        bool result = Rule::match_impl(reader, data);
        if (result)
        {
            Action<Rule>::apply(reader.current_input(), data);
        }
        reader.pop_state();

        return result;
    }
}

template <typename Rule>
inline bool match(InputReader& reader)
{
    char dummy;
    return match<Rule>(reader, dummy);
}

[[noreturn]]
inline void fail(InputReader& reader, const std::string& error)
{
    throw_parse_error(reader.current_pos(), error);
}

struct _true
{
    template <typename Data>
    static bool match_impl(InputReader&, Data&&)
    {
        return true;
    }
};

struct _false
{
    template <typename Data>
    static bool match_impl(InputReader&, Data&&)
    {
        return false;
    }
};

template <typename... Rules>
struct seq
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        bool valid = seq_impl<Data, Rules...>(reader, data);
        if (!valid)
        {
            reader.rewind();
        }

        return valid;
    }

private:
    template <typename Data, typename Rule, typename... Rest>
    static bool seq_impl(InputReader& reader, Data&& data)
    {
        if constexpr (sizeof...(Rest) == 0)
        {
            return match<Rule>(reader, data);
        }
        else
        {
            return match<Rule>(reader, data) && seq_impl<Data, Rest...>(reader, data);
        }
    }
};

template <typename... Rules>
struct _or
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        auto prev_policy = reader.failure_policy();
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        return or_impl<Data, Rules...>(reader, prev_policy, data);
    }

private:
    template <typename Data, typename... None, typename std::enable_if_t<sizeof...(None) == 0>* = nullptr>
    static bool or_impl(InputReader& reader, InputReader::FailurePolicy prev_policy, Data&&)
    {
        reader.set_failure_policy(prev_policy);
        return false;
    }

    template <typename Data, typename Rule, typename... Rest>
    static bool or_impl(InputReader& reader, InputReader::FailurePolicy prev_policy, Data&& data)
    {
        if (match<Rule>(reader, data))
        {
            return true;
        }
        else
        {
            reader.rewind();
            return or_impl<Data, Rest...>(reader, prev_policy, data);
        }
    }
};

template <typename Rule>
struct star
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        while (match<Rule>(reader, data)){}

        return true;
    }
};

template <typename Rule>
struct plus
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        if (!match<Rule>(reader, data))
        {
            reader.rewind();
            return false;
        }

        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        while (match<Rule>(reader, data)) {}

        return true;
    }
};

template <typename Rule>
struct opt
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        if (!match<Rule>(reader, data))
        {
            reader.rewind();
        }

        return true;
    }
};

template <typename Rule>
struct _and
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        bool result = match<Rule>(reader, data);
        reader.rewind();
        return result;
    }
};

template <typename Rule>
struct _not
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        bool result = match<Rule>(reader, data);
        reader.rewind();
        return !result;
    }
};

template <typename... Rule>
struct must
{
    template <typename Data>
    static bool match_impl(InputReader& reader, Data&& data)
    {
        if (!match<seq<Rule...>>(reader, data))
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
