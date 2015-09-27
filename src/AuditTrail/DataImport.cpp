#include "DataImport.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

DataImport::DataImport(Outcome outcome, Media sourceMedia)
    : m_outcome(outcome), m_sourceMedia(sourceMedia)
{
}

DataImport::~DataImport()
{
}

std::vector<IO::Node> DataImport::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Create, generateEventID(EventIDCode::Import));
    nodes.emplace_back(event.toNode());

    for (const auto& importingUser : m_importingUsers)
        nodes.emplace_back(EntityActiveParticipant(importingUser).toNode());

    for (const auto& importingProcess : m_importingProcesses)
        nodes.emplace_back(EntityActiveParticipant(importingProcess).toNode());

    ActiveParticipant sourceMedia(m_sourceMedia.mediaId(), false);
    sourceMedia.roleIdCode = generateRoleIDCode(RoleIDCode::SourceMedia);
    nodes.emplace_back(EntityActiveParticipant(sourceMedia).toNode());

    for (const auto& source : m_sources)
        nodes.emplace_back(EntityActiveParticipant(source).toNode());

    for (const auto& study : m_studies)
        nodes.emplace_back(study.toNode());

    for (const auto& patient : m_patients)
    {
        EntityParticipantObject patientEntity(
            EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
            generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId),
            patient.first);
        patientEntity.objectNameOrQuery = patient.second;
        nodes.emplace_back(patientEntity.toNode());
    }

    return nodes;
}

void DataImport::addImportingUser(ActiveParticipant importingUser)
{
    importingUser.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    m_importingUsers.emplace_back(std::move(importingUser));
}

void DataImport::addImportingProcess(ActiveParticipant importingProcess)
{
    importingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    m_importingProcesses.emplace_back(std::move(importingProcess));
}

void DataImport::addSource(ActiveParticipant source)
{
    source.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    m_sources.emplace_back(std::move(source));
}

void DataImport::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    m_studies.emplace_back(std::move(study));
}

void DataImport::addPatient(std::string patientId, std::string patientName /*= std::string()*/)
{
    m_patients.emplace_back(patientId, patientName);
}

}
