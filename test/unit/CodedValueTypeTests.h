#pragma once

#include "Node.h"
#include "Event.h"

namespace Tests
{

void checkEventIdentification(const IO::Node& eventIdentification,
                              const std::string& eventActionCode,
                              AuditTrail::Outcome eventOutcomeIndicator);

void checkCodedValueType(const IO::Node& eventId, const std::string& code,
                         const std::string& displayName);

void checkCodedValueType(const IO::Node& eventId, const std::string& code);

}
