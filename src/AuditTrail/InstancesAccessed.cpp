#include "InstancesAccessed.h"

#include "EntityEvent.h"

namespace AuditTrail
{

InstancesAccessed::InstancesAccessed(Outcome outcome, Action action)
    : m_outcome(outcome), m_action(action)
{
}

std::vector<IO::Node> InstancesAccessed::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, actionToActionCode(), generateCode(CodeType::InstancesAccessed));
    nodes.emplace_back(event.toNode());

    return nodes;
}

EventActionCode InstancesAccessed::actionToActionCode() const
{
    switch (m_action)
    {
    case InstancesAccessed::Action::Create:
        return EventActionCode::Create;
    case InstancesAccessed::Action::Read:
        return EventActionCode::Read;
    case InstancesAccessed::Action::Update:
        return EventActionCode::Update;
    case InstancesAccessed::Action::Delete:
        return EventActionCode::Delete;
    default:
        throw std::logic_error("Unable to convert action " + std::to_string(static_cast<int>(m_action)) + " to action code.");
    }
}

}