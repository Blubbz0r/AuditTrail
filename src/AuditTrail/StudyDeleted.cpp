#include "StudyDeleted.h"

#include "EntityEvent.h"

namespace AuditTrail
{

StudyDeleted::StudyDeleted(Outcome outcome)
    : m_outcome(outcome), m_deletingPerson(nullptr), m_deletingProcess(nullptr)
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

void StudyDeleted::setDeletingPerson(ActiveParticipant deletingPerson)
{
    m_deletingPerson = std::make_unique<ActiveParticipant>(deletingPerson);
}

void StudyDeleted::setDeletingProcess(ActiveParticipant deletingProcess)
{
    m_deletingProcess = std::make_unique<ActiveParticipant>(deletingProcess);
}

}