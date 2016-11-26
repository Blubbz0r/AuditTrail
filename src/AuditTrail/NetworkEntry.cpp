#include "NetworkEntry.h"
#include "EntityActiveParticipant.h"

namespace AuditTrail
{

NetworkEntry::NetworkEntry(Outcome outcome, Action action, ActiveParticipant node)
    : event(outcome, EventActionCode::Execute, generateEventID(EventIDCode::NetworkEntry)),
      activeParticipant(std::move(node))
{
    switch (action)
    {
    case Action::Attach:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Attach);
        break;
    case Action::Detach:
        event.eventTypeCode = generateEventTypeCode(EventTypeCode::Detach);
        break;
    default:
        throw std::logic_error("Unable to generate event type code for NetworkEntry action "
                               + std::to_string(static_cast<int>(action)));
    }
}

std::vector<IO::Node> NetworkEntry::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(activeParticipant.toNode());

    return nodes;
}

}
