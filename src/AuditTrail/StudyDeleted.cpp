#include "StudyDeleted.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

StudyDeleted::StudyDeleted(Outcome outcome)
    : m_outcome(outcome), m_deletingPerson(nullptr), m_deletingProcess(nullptr)
{
}

StudyDeleted::~StudyDeleted()
{
}

std::vector<IO::Node> StudyDeleted::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Delete,
                      generateEventID(EventIDCode::StudyDeleted));
    nodes.emplace_back(event.toNode());

    if (m_deletingPerson)
        nodes.emplace_back(EntityActiveParticipant(*m_deletingPerson).toNode());

    if (m_deletingProcess)
        nodes.emplace_back(EntityActiveParticipant(*m_deletingProcess).toNode());

    for (const auto& study : m_studies)
        nodes.emplace_back(EntityParticipantObject(study).toNode());

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

void StudyDeleted::addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUID));

    study.setSOPClasses(std::move(sopClasses));
    m_studies.emplace_back(std::move(study));
}

}