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

template <typename Derived>
struct GrammarRule
{
    static bool match(InputReader& reader)
    {
        reader.push_state();
        bool result = Derived::match_impl(reader);
        reader.pop_state();

        return result;
    }
};

struct _true : public GrammarRule<_true>
{
    static bool match_impl(InputReader&)
    {
        return true;
    }
};

struct _false : public GrammarRule<_false>
{
    static bool match_impl(InputReader&)
    {
        return false;
    }
};

template <typename T>
struct term : public GrammarRule<term<T>>
{
    static_assert(std::is_base_of<Terminal, T>::value);

    static bool match_impl(InputReader& reader)
    {
        return reader.fetch<T>();
    }
};

template <typename... Rules>
struct seq : public GrammarRule<seq<Rules...>>
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
            return Rule::match(reader);
        }
        else
        {
            return Rule::match(reader) && seq_impl<Rest...>(reader);
        }
    }
};

template <typename... Rules>
struct _or : public GrammarRule<_or<Rules...>>
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
        if (Rule::match(reader))
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
struct star : public GrammarRule<star<Rule>>
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        while (Rule::match(reader)){}

        return true;
    }
};

template <typename Rule>
struct plus : public GrammarRule<plus<Rule>>
{
    static bool match_impl(InputReader& reader)
    {
        if (!Rule::match(reader))
        {
            reader.rewind();
            return false;
        }

        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        while (Rule::match(reader)) {}

        return true;
    }
};

template <typename Rule>
struct opt : public GrammarRule<opt<Rule>>
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        if (!Rule::match(reader))
        {
            reader.rewind();
        }

        return true;
    }
};

template <typename Rule>
struct _and : public GrammarRule<_and<Rule>>
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        bool result = Rule::match(reader);
        reader.rewind();
        return result;
    }
};

template <typename Rule>
struct _not : public GrammarRule<_not<Rule>>
{
    static bool match_impl(InputReader& reader)
    {
        reader.set_failure_policy(InputReader::FailurePolicy::Permissive);

        bool result = Rule::match(reader);
        reader.rewind();
        return !result;
    }
};

#endif // GRAMMAR_HPP
