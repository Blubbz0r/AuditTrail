#include "InstancesTransferred.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"

namespace AuditTrail
{

InstancesTransferred::InstancesTransferred(Outcome outcome, Action action,
                                           ActiveParticipant sendingProcess,
                                           ActiveParticipant receivingProcess)
    : m_outcome(outcome),
      m_action(action),
      m_sendingProcess(sendingProcess),
      m_receivingProcess(receivingProcess)
{
    m_sendingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    m_receivingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
}

std::vector<IO::Node> InstancesTransferred::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, actionToActionCode(),
                      generateEventID(EventIDCode::InstancesTransferred));
    nodes.emplace_back(event.toNode());
    nodes.emplace_back(EntityActiveParticipant(m_sendingProcess).toNode());
    nodes.emplace_back(EntityActiveParticipant(m_receivingProcess).toNode());

    return nodes;
}

void InstancesTransferred::addOtherParticipant(ActiveParticipant otherParticipant)
{

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