#pragma once

#include "Entity.h"
#include "CodedValueType.h"

namespace AuditTrail
{

enum class EventActionCode
{
    Create,
    Read,
    Update,
    Delete,
    Execute
};

std::string eventActionCodeToString(EventActionCode code);

enum class Outcome
{
    Success = 0,
    MinorFailure = 4,
    SeriousFailure = 8,
    MajorFailure = 12
};

struct EntityEvent : public Entity
{
    EntityEvent(Outcome outcome, EventActionCode eventActionCode, CodedValueType eventId);

    IO::Node toNode() const override;

    CodedValueType eventTypeCode;

private:
    std::string currentDateTime() const;

    Outcome m_outcome;
    EventActionCode m_eventActionCode;
    CodedValueType m_eventId;
};

}