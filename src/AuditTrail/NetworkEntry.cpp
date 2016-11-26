#include "NetworkEntry.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"

namespace AuditTrail
{

NetworkEntry::NetworkEntry(Outcome outcome, Action action, ActiveParticipant node)
    : m_outcome(outcome), m_action(action), m_activeParticipant(std::move(node))
{
}

std::vector<IO::Node> NetworkEntry::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute,
                      generateEventID(EventIDCode::NetworkEntry));

    switch (m_action)
    {
    case Action::Attach:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Attach);
        break;
    case Action::Detach:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Detach);
        break;
    default:
        throw std::logic_error("Unable to generate event type code for NetworkEntry action "
                               + std::to_string(static_cast<int>(m_action)));
    }

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(EntityActiveParticipant(m_activeParticipant).toNode());

    return nodes;
}

}
