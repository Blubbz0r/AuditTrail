#pragma once

#include <memory>
#include <string>

namespace Logging
{

static const std::string DefaultLoggerName = "default";

class Log
{
public:
    virtual ~Log() {}

    /*!
     * \brief Used to log informative messages about the current status of the application.
     */
    virtual void info(const std::string& msg) = 0;

    /*!
     * \brief Used to report warnings and recoverable errors of the application.
     */
    virtual void warning(const std::string& msg) = 0;

    /*!
     * \brief Used to report error messages and report system errors.
     */
    virtual void error(const std::string& msg) = 0;

    /*!
     * \brief Informational events most useful for developers to debug application.
     * \note This might only produce output in debug mode. Check the documentation of the concrete
     *       subclass for further information.
     */
    virtual void debug(const std::string& msg) = 0;

    /*!
     * \brief Information that can be useful to back-trace certain events.
     */
    virtual void trace(const std::string& msg) = 0;

    /*!
     * \brief Used to write fatal error messages shortly before exiting.
     */
    virtual void fatal(const std::string& msg) = 0;
};

}

extern Logging::Log* Logger;
