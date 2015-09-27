#pragma once

#include "Message.h"

#include "Event.h"

namespace AuditTrail
{

enum class EventActionCode;

class InstancesAccessed : public Message
{
public:
    enum class Action
    {
        Create,
        Read,
        Update,
        Delete
    };

    InstancesAccessed(Outcome outcome, Action action);

    std::vector<IO::Node> createNodes() const override;

private:
    EventActionCode actionToActionCode() const;

    Outcome m_outcome;
    Action m_action;
};

}