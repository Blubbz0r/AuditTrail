#include "BeginTransferringInstances.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

BeginTransferringInstances::BeginTransferringInstances(Outcome outcome,
                                                       ActiveParticipant sendingProcess,
                                                       ActiveParticipant receivingProcess,
                                                       std::string patientId)
    : m_outcome(outcome),
      m_sendingProcess(std::move(sendingProcess)),
      m_receivingProcess(std::move(receivingProcess)),
      m_patientId(std::move(patientId))
{
    m_sendingProcess.roleIdCode = generateCode(CodeType::Source);
    m_receivingProcess.roleIdCode = generateCode(CodeType::Destination);
}

BeginTransferringInstances::~BeginTransferringInstances()
{
}

std::vector<IO::Node> BeginTransferringInstances::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute,
                      generateCode(CodeType::BeginTransferringInstances));

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(EntityActiveParticipant(m_sendingProcess).toNode());
    nodes.emplace_back(EntityActiveParticipant(m_receivingProcess).toNode());

    for (const auto& otherParticipant : m_otherParticipants)
        nodes.emplace_back(EntityActiveParticipant(otherParticipant).toNode());

    for (const auto& study : m_studies)
        nodes.emplace_back(study.toNode());

    EntityParticipantObject patient(EntityParticipantObject::Type::Person,
                                    EntityParticipantObject::Role::Patient,
                                    generateCode(CodeType::PatientId), m_patientId);
    patient.objectNameOrQuery = m_patientName;
    nodes.emplace_back(patient.toNode());

    return nodes;
}

void BeginTransferringInstances::addOtherParticipant(ActiveParticipant otherParticipant)
{
    m_otherParticipants.emplace_back(std::move(otherParticipant));
}

void BeginTransferringInstances::addStudy(std::string studyInstanceUid,
                                          std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateCode(CodeType::StudyInstanceUid), std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    m_studies.emplace_back(std::move(study));
}

}