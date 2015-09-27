#include "StudyDeleted.h"

#include "EntityEvent.h"

namespace AuditTrail
{

StudyDeleted::StudyDeleted(Outcome outcome) : m_outcome(outcome)
{
}

std::vector<IO::Node> StudyDeleted::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Delete,
                      generateEventID(EventIDCode::StudyDeleted));
    nodes.emplace_back(event.toNode());

    return nodes;
}

}