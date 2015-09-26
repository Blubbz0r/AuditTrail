#pragma once

#include "Log.h"

namespace Test
{

class LogNoop : public Logging::Log
{
public:
    void info(const std::string& msg) override
    {
    }

    void warning(const std::string& msg) override
    {
    }

    void error(const std::string& msg) override
    {
    }

    void debug(const std::string& msg) override
    {
    }

    void trace(const std::string& msg) override
    {
    }

    void fatal(const std::string& msg) override
    {
    }
};

}