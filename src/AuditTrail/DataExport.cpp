#include "DataExport.h"

#include "EntityActiveParticipant.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

DataExport::DataExport(Outcome outcome, MediaType mediaType)
    : m_outcome(outcome),
      m_exportingUser(nullptr),
      m_exportingProcess(nullptr),
      m_mediaType(mediaType)
{
}

std::vector<IO::Node> DataExport::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Read, generateCode(CodeType::Export));
    nodes.emplace_back(event.toNode());

    for (const auto& remoteUserOrProcess : m_remoteUsersAndProcesses)
        nodes.emplace_back(EntityActiveParticipant(remoteUserOrProcess).toNode());

    if (m_exportingUser)
        nodes.emplace_back(EntityActiveParticipant(*m_exportingUser).toNode());

    if (m_exportingProcess)
        nodes.emplace_back(EntityActiveParticipant(*m_exportingProcess).toNode());

    std::string mediaId = mediaTypeToString(m_mediaType);
    if (!m_mediaLabel.empty())
        mediaId += " " + m_mediaLabel;

    ActiveParticipant media(mediaId, false);
    media.roleIdCode = generateCode(CodeType::DestinationMedia);
    nodes.emplace_back(EntityActiveParticipant(media).toNode());

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

void DataExport::addRemoteUser(ActiveParticipant remoteUser)
{
    remoteUser.roleIdCode = generateCode(CodeType::Destination);
    m_remoteUsersAndProcesses.emplace_back(std::move(remoteUser));
}

void DataExport::addRemoteProcess(ActiveParticipant remoteProcess)
{
    remoteProcess.roleIdCode = generateCode(CodeType::Destination);
    m_remoteUsersAndProcesses.emplace_back(std::move(remoteProcess));
}

void DataExport::setExportingUser(ActiveParticipant exportingUser)
{
    exportingUser.roleIdCode = generateCode(CodeType::Source);
    m_exportingUser = std::make_unique<ActiveParticipant>(exportingUser);
}

void DataExport::setExportingProcess(ActiveParticipant exportingProcess)
{
    exportingProcess.roleIdCode = generateCode(CodeType::Source);
    m_exportingProcess = std::make_unique<ActiveParticipant>(exportingProcess);
}

void DataExport::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateCode(CodeType::StudyInstanceUid), std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    m_studies.emplace_back(std::move(study));
}

void DataExport::addPatient(std::string patientId, std::string patientName /*= std::string()*/)
{
    m_patients.emplace_back(patientId, patientName);
}

}