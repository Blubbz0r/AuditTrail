#include "UserAuthentication.h"

namespace AuditTrail
{

UserAuthentication::UserAuthentication(Outcome outcome, Type type,
                                       ActiveParticipant authenticatedPerson)
    : event(outcome, EventActionCode::Execute, generateEventID(EventIDCode::UserAuthentication)),
      authenticatedPerson(std::move(authenticatedPerson)),
      authenticatingSystem(nullptr)
{
    switch (type)
    {
    case Type::Login:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Login);
        break;
    case Type::Logout:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Logout);
        break;
    default:
        throw std::logic_error("Unable to generate event type code for user authentication type "
                               + std::to_string(static_cast<int>(type)));
    }
}

std::vector<IO::Node> UserAuthentication::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(authenticatedPerson.toNode());

    if (authenticatingSystem)
        nodes.emplace_back(authenticatingSystem->toNode());

    return nodes;
}

void UserAuthentication::setAuthenticatingSystem(ActiveParticipant system)
{
    authenticatingSystem = std::make_unique<EntityActiveParticipant>(std::move(system));
}

}
