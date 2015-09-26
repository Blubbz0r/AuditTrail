#include "DataImport.h"

#include "EntityActiveParticipant.h"

namespace AuditTrail
{

DataImport::DataImport(Outcome outcome, MediaType sourceMedia)
    : m_outcome(outcome), m_sourceMedia(sourceMedia)
{
}

std::vector<IO::Node> DataImport::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Create, generateCode(CodeType::Import));
    nodes.emplace_back(event.toNode());

    for (const auto& importingUser : m_importingUsers)
        nodes.emplace_back(EntityActiveParticipant(importingUser).toNode());

    for (const auto& importingProcess : m_importingProcesses)
        nodes.emplace_back(EntityActiveParticipant(importingProcess).toNode());

    std::string mediaId = mediaTypeToString(m_sourceMedia);
    if (!m_mediaLabel.empty())
        mediaId += " " + m_mediaLabel;

    ActiveParticipant sourceMedia(mediaId, false);
    sourceMedia.roleIdCode = generateCode(CodeType::SourceMedia);
    nodes.emplace_back(EntityActiveParticipant(sourceMedia).toNode());

    for (const auto& source : m_sources)
        nodes.emplace_back(EntityActiveParticipant(source).toNode());

    for (const auto& study : m_studies)
        nodes.emplace_back(study.toNode());

    for (const auto& patient : m_patients)
    {
        EntityParticipantObject patientEntity(EntityParticipantObject::Type::Person,
                                              EntityParticipantObject::Role::Patient,
                                              generateCode(CodeType::PatientId), patient.first);
        patientEntity.objectNameOrQuery = patient.second;
        nodes.emplace_back(patientEntity.toNode());
    }

    return nodes;
}

void DataImport::addImportingUser(ActiveParticipant importingUser)
{
    importingUser.roleIdCode = generateCode(CodeType::Destination);
    m_importingUsers.emplace_back(std::move(importingUser));
}

void DataImport::addImportingProcess(ActiveParticipant importingProcess)
{
    importingProcess.roleIdCode = generateCode(CodeType::Destination);
    m_importingProcesses.emplace_back(std::move(importingProcess));
}

void DataImport::addSource(ActiveParticipant source)
{
    source.roleIdCode = generateCode(CodeType::Source);
    m_sources.emplace_back(std::move(source));
}

void DataImport::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateCode(CodeType::StudyInstanceUid), std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    m_studies.emplace_back(std::move(study));
}

void DataImport::addPatient(std::string patientId, std::string patientName /*= std::string()*/)
{
    m_patients.emplace_back(patientId, patientName);
}

}
