#include "InstancesAccessed.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

InstancesAccessed::InstancesAccessed(Outcome outcome, Action action, std::string patientId)
    : m_outcome(outcome),
      m_action(action),
      m_manipulatingPerson(nullptr),
      m_manipulatingProcess(nullptr),
      m_patientId(std::move(patientId))
{
}

InstancesAccessed::~InstancesAccessed()
{
}

std::vector<IO::Node> InstancesAccessed::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, actionToActionCode(), generateEventID(EventIDCode::InstancesAccessed));
    nodes.emplace_back(event.toNode());

    if (m_manipulatingPerson)
        nodes.emplace_back(EntityActiveParticipant(*m_manipulatingPerson).toNode());

    if (m_manipulatingProcess)
        nodes.emplace_back(EntityActiveParticipant(*m_manipulatingProcess).toNode());

    for (const auto& study : m_studies)
        nodes.emplace_back(study.toNode());

    EntityParticipantObject patient(
        EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId), m_patientId);
    patient.objectNameOrQuery = m_patientName;
    nodes.emplace_back(patient.toNode());

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

void InstancesAccessed::setManipulatingPerson(ActiveParticipant manipulatingPerson)
{
    m_manipulatingPerson = std::make_unique<ActiveParticipant>(manipulatingPerson);
}

void InstancesAccessed::setManipulatingProcess(ActiveParticipant manipulatingProcess)
{
    m_manipulatingProcess = std::make_unique<ActiveParticipant>(manipulatingProcess);
}

void InstancesAccessed::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));
    m_studies.emplace_back(std::move(study));
}

}