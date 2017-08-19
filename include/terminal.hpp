/*  terminal.hpp %{Cpp:License:ClassName} - Yann BOUCHER (yann) 17
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
#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <string>
#include <memory>

struct Terminal
{
    virtual inline std::string dyn_name() const
    {
        return "Terminal";
    }

    static inline std::string name()
    {
        return "Terminal";
    }

    virtual std::unique_ptr<Terminal> clone() const
    {
        return std::make_unique<Terminal>(*this);
    }

    std::string data;

    struct FileInfo
    {
        std::string filename { "INVALID" };
        int line { -1 };
        int column { -1 };
    } fileinfo;
};

#define MAKE_TERMINAL(TerminalName) \
struct TerminalName : public Terminal \
{ \
    virtual inline std::string dyn_name() const override \
    { \
        return std::string(#TerminalName) + " ('" + data + "')"; \
    } \
    static inline std::string name() \
    { \
        return #TerminalName; \
    } \
    virtual std::unique_ptr<Terminal> clone() const override \
    { \
        return std::make_unique<TerminalName>(*this); \
    } \
};

MAKE_TERMINAL(_EOF)

#endif // TERMINAL_HPP
