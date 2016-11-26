#include "DataImport.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

DataImport::DataImport(Outcome outcome, Media sourceMedia)
    : event(outcome, EventActionCode::Create, generateEventID(EventIDCode::Import)),
      sourceMedium(mediumWithRoleIdCode(std::move(sourceMedia)))
{
}

DataImport::~DataImport()
{
}

std::vector<IO::Node> DataImport::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    for (const auto& importingUser : importingUsers)
        nodes.emplace_back(EntityActiveParticipant(importingUser).toNode());

    for (const auto& importingProcess : importingProcesses)
        nodes.emplace_back(EntityActiveParticipant(importingProcess).toNode());

    nodes.emplace_back(sourceMedium.toNode());

    for (const auto& source : sources)
        nodes.emplace_back(EntityActiveParticipant(source).toNode());

    for (const auto& study : studies)
        nodes.emplace_back(study.toNode());

    for (const auto& patient : patients)
        nodes.emplace_back(patient.toNode());

    return nodes;
}

void DataImport::addImportingUser(ActiveParticipant importingUser)
{
    importingUser.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    importingUsers.emplace_back(EntityActiveParticipant(std::move(importingUser)));
}

void DataImport::addImportingProcess(ActiveParticipant importingProcess)
{
    importingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    importingProcesses.emplace_back(EntityActiveParticipant(std::move(importingProcess)));
}

void DataImport::addSource(ActiveParticipant source)
{
    source.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    sources.emplace_back(EntityActiveParticipant(std::move(source)));
}

void DataImport::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    studies.emplace_back(std::move(study));
}

void DataImport::addPatient(std::string patientId, std::string patientName)
{
    EntityParticipantObject patientEntity(
        EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId), patientId);
    patientEntity.objectNameOrQuery = patientName;
    patients.emplace_back(std::move(patientEntity));
}

ActiveParticipant DataImport::mediumWithRoleIdCode(Media media)
{
    ActiveParticipant medium(media.mediaId(), false);
    medium.roleIdCode = generateRoleIDCode(RoleIDCode::SourceMedia);
    return medium;
}

}
