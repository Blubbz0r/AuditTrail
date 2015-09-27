#include "InstancesTransferred.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

InstancesTransferred::InstancesTransferred(Outcome outcome, Action action,
                                           ActiveParticipant sendingProcess,
                                           ActiveParticipant receivingProcess,
                                           std::string patientId)
    : m_outcome(outcome),
      m_action(action),
      m_sendingProcess(sendingProcess),
      m_receivingProcess(receivingProcess),
      m_patientId(std::move(patientId))
{
    m_sendingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    m_receivingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
}

InstancesTransferred::~InstancesTransferred()
{
}

std::vector<IO::Node> InstancesTransferred::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, actionToActionCode(),
                      generateEventID(EventIDCode::InstancesTransferred));
    nodes.emplace_back(event.toNode());
    nodes.emplace_back(EntityActiveParticipant(m_sendingProcess).toNode());
    nodes.emplace_back(EntityActiveParticipant(m_receivingProcess).toNode());

    for (const auto& otherParticipant : m_otherParticipants)
        nodes.emplace_back(EntityActiveParticipant(otherParticipant).toNode());

    for (const auto& study : m_studies)
        nodes.emplace_back(study.toNode());

    EntityParticipantObject patient(
        EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId), m_patientId);

    patient.objectNameOrQuery = m_patientName;
    nodes.emplace_back(patient.toNode());

    return nodes;
}

void InstancesTransferred::addOtherParticipant(ActiveParticipant otherParticipant)
{
    m_otherParticipants.emplace_back(std::move(otherParticipant));
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

void InstancesTransferred::addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUID));

    study.setSOPClasses(std::move(sopClasses));
    m_studies.emplace_back(std::move(study));
}

}