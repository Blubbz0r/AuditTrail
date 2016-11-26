#include "EntityEvent.h"

#include <ctime>

namespace AuditTrail
{

EntityEvent::EntityEvent(Outcome outcome, EventActionCode eventActionCode, CodedValueType eventId)
    : m_outcome(outcome),
      m_eventActionCode(eventActionCode),
      m_eventId(std::move(eventId)),
      m_eventDateTime(currentDateTime())
{
}

IO::Node EntityEvent::toNode() const
{
    IO::Node eventIdentification("EventIdentification");
    eventIdentification.addAttribute(
        IO::Attribute{"EventActionCode", eventActionCodeToString(m_eventActionCode)});
    eventIdentification.addAttribute(IO::Attribute{"EventDateTime", m_eventDateTime});
    eventIdentification.addAttribute(IO::Attribute{
        "EventOutcomeIndicator", std::to_string(static_cast<int>(m_outcome))});

    eventIdentification.addNode(m_eventId.toNode());
    if (eventTypeCode.isValid())
        eventIdentification.addNode(eventTypeCode.toNode());
    return eventIdentification;
}

std::string EntityEvent::currentDateTime() const
{
    time_t rawtime;
    std::tm timeinfo;
    char buffer[80];

    time(&rawtime);
    auto result = localtime_s(&timeinfo, &rawtime);

    std::strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S", &timeinfo);
    return buffer;
}

std::string eventActionCodeToString(EventActionCode code)
{
    switch (code)
    {
    case EventActionCode::Create:   return "C";
    case EventActionCode::Read:     return "R";
    case EventActionCode::Update:   return "U";
    case EventActionCode::Delete:   return "D";
    case EventActionCode::Execute:  return "E";
    default: throw std::logic_error("Unable to convert event action code " + std::to_string(static_cast<int>(code)) + " to string");
    }
}

}