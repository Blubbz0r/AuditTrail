#include "StudyDeleted.h"

namespace AuditTrail
{

StudyDeleted::StudyDeleted(Outcome outcome, std::string patientID)
    : event(outcome, EventActionCode::Delete, generateEventID(EventIDCode::StudyDeleted)),
      patient(EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
              generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId),
              std::move(patientID)),
      deletingPerson(nullptr),
      deletingProcess(nullptr)
{
}

StudyDeleted::~StudyDeleted()
{
}

std::vector<IO::Node> StudyDeleted::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    if (deletingPerson)
        nodes.emplace_back(deletingPerson->toNode());

    if (deletingProcess)
        nodes.emplace_back(deletingProcess->toNode());

    for (const auto& study : studies)
        nodes.emplace_back(EntityParticipantObject(study).toNode());

    nodes.emplace_back(patient.toNode());

    return nodes;
}

void StudyDeleted::setDeletingPerson(ActiveParticipant person)
{
    deletingPerson = std::make_unique<EntityActiveParticipant>(std::move(person));
}

void StudyDeleted::setDeletingProcess(ActiveParticipant process)
{
    deletingProcess = std::make_unique<EntityActiveParticipant>(std::move(process));
}

void StudyDeleted::addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUID));

    study.setSOPClasses(std::move(sopClasses));
    studies.emplace_back(std::move(study));
}

void StudyDeleted::setPatientName(std::string patientName)
{
    patient.objectNameOrQuery = std::move(patientName);
}

}
