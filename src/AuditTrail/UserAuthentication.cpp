#include "UserAuthentication.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"

namespace AuditTrail
{

UserAuthentication::UserAuthentication(Outcome outcome, Type type,
                                       ActiveParticipant authenticatedPerson)
    : m_outcome(outcome),
      m_type(type),
      m_authenticatedPerson(std::move(authenticatedPerson)),
      m_authenticatingSystem(nullptr)
{
}

std::vector<IO::Node> UserAuthentication::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute,
                      generateEventID(EventIDCode::UserAuthentication));

    switch (m_type)
    {
    case Type::Login:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Login);
        break;
    case Type::Logout:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Logout);
        break;
    default:
        throw std::logic_error("Unable to generate event type code for user authentication type "
                               + std::to_string(static_cast<int>(m_type)));
    }

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(EntityActiveParticipant(m_authenticatedPerson).toNode());

    if (m_authenticatingSystem)
        nodes.emplace_back(EntityActiveParticipant(*m_authenticatingSystem).toNode());

    return nodes;
}

void UserAuthentication::setAuthenticatingSystem(ActiveParticipant authenticatingSystem)
{
    m_authenticatingSystem = std::make_unique<ActiveParticipant>(authenticatingSystem);
}

}
