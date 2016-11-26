#include "DataExport.h"

namespace AuditTrail
{

DataExport::DataExport(Outcome outcome, MediaType mediaType)
    : event(outcome, EventActionCode::Read, generateEventID(EventIDCode::Export)),
      medium(mediumWithRoleIdCode(mediaType)),
      exportingUser(nullptr),
      exportingProcess(nullptr),
      m_mediaType(mediaType)
{
}

DataExport::~DataExport()
{
}

std::vector<IO::Node> DataExport::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    for (const auto& remoteUserOrProcess : remoteUsersAndProcesses)
        nodes.emplace_back(remoteUserOrProcess.toNode());

    if (exportingUser)
        nodes.emplace_back(exportingUser->toNode());

    if (exportingProcess)
        nodes.emplace_back(exportingProcess->toNode());

    nodes.emplace_back(medium.toNode());

    for (const auto& study : studies)
        nodes.emplace_back(study.toNode());

    for (const auto& patient : patients)
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

void DataExport::addRemoteUser(ActiveParticipant remoteUser)
{
    remoteUser.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    remoteUsersAndProcesses.emplace_back(EntityActiveParticipant(std::move(remoteUser)));
}

void DataExport::addRemoteProcess(ActiveParticipant remoteProcess)
{
    remoteProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    remoteUsersAndProcesses.emplace_back(EntityActiveParticipant(std::move(remoteProcess)));
}

void DataExport::setExportingUser(ActiveParticipant user)
{
    user.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    exportingUser = std::make_unique<EntityActiveParticipant>(std::move(user));
}

void DataExport::setExportingProcess(ActiveParticipant process)
{
    process.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    exportingProcess = std::make_unique<EntityActiveParticipant>(std::move(process));
}

void DataExport::setMediaLabel(std::string mediaLabel)
{
    medium.participant.userId = mediaId(m_mediaType, mediaLabel);
}

void DataExport::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    studies.emplace_back(std::move(study));
}

void DataExport::addPatient(std::string patientId, std::string patientName)
{
    patients.emplace_back(patientId, patientName);
}

ActiveParticipant DataExport::mediumWithRoleIdCode(MediaType mediaType)
{
    ActiveParticipant media(mediaTypeToString(mediaType), false);
    media.roleIdCode = generateRoleIDCode(RoleIDCode::DestinationMedia);
    return media;
}

std::string DataExport::mediaId(MediaType mediaType, const std::string& label)
{
    auto id = mediaTypeToString(mediaType);
    if (!label.empty())
        id += " " + label;
    return id;
}

}
