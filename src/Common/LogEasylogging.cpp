#include "LogEasylogging.h"
#include "DebugNew.h"

#include "easylogging++.h"

namespace Logging
{

void LogEasylogging::info(const std::string& msg)
{
    LOG(INFO) << msg;
}

void LogEasylogging::warning(const std::string& msg)
{
    LOG(WARNING) << msg;
}

void LogEasylogging::error(const std::string& msg)
{
    LOG(ERROR) << msg;
}

void LogEasylogging::debug(const std::string& msg)
{
    LOG(DEBUG) << msg;
}

void LogEasylogging::trace(const std::string& msg)
{
    LOG(TRACE) << msg;
}

void LogEasylogging::fatal(const std::string& msg)
{
    LOG(FATAL) << msg;
}

}