#include "InstancesTransferred.h"

#include "EntityEvent.h"

namespace AuditTrail
{

InstancesTransferred::InstancesTransferred(Outcome outcome, Action action,
                                           ActiveParticipant sendingProcess)
    : m_outcome(outcome), m_action(action), m_sendingProcess(sendingProcess)
{
}

std::vector<IO::Node> InstancesTransferred::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, actionToActionCode(),
                      generateCode(CodeType::InstancesTransferred));
    nodes.emplace_back(event.toNode());

    return nodes;
}

EventActionCode InstancesTransferred::actionToActionCode() const
{
    switch (m_action)
    {
    case InstancesTransferred::Action::Create:
        return EventActionCode::Create;
    case InstancesTransferred::Action::Read:
        return EventActionCode::Read;
    case InstancesTransferred::Action::Update:
        return EventActionCode::Update;
    default:
        throw std::logic_error("Unable to convert action " + std::to_string(static_cast<int>(m_action)) + " to string.");
    }
}

}