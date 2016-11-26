#include "InstancesAccessed.h"

#include "EntityActiveParticipant.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

InstancesAccessed::InstancesAccessed(Outcome outcome, Action action, std::string patientId)
    : event(outcome, toActionCode(action), generateEventID(EventIDCode::InstancesAccessed)),
      patient(EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
              generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId),
              std::move(patientId)),
      manipulatingPerson(nullptr),
      manipulatingProcess(nullptr)
{
}

InstancesAccessed::~InstancesAccessed()
{
}

std::vector<IO::Node> InstancesAccessed::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    if (manipulatingPerson)
        nodes.emplace_back(manipulatingPerson->toNode());

    if (manipulatingProcess)
        nodes.emplace_back(manipulatingProcess->toNode());

    for (const auto& study : studies)
        nodes.emplace_back(study.toNode());

    nodes.emplace_back(patient.toNode());

    return nodes;
}

void InstancesAccessed::setManipulatingPerson(ActiveParticipant person)
{
    manipulatingPerson = std::make_unique<EntityActiveParticipant>(std::move(person));
}

void InstancesAccessed::setManipulatingProcess(ActiveParticipant process)
{
    manipulatingProcess = std::make_unique<EntityActiveParticipant>(std::move(process));
}

void InstancesAccessed::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));
    studies.emplace_back(std::move(study));
}

void InstancesAccessed::setPatientName(std::string patientName)
{
    patient.objectNameOrQuery = std::move(patientName);
}

EventActionCode InstancesAccessed::toActionCode(Action action)
{
    switch (action)
    {
    case Action::Create:
        return EventActionCode::Create;
    case Action::Read:
        return EventActionCode::Read;
    case Action::Update:
        return EventActionCode::Update;
    case Action::Delete:
        return EventActionCode::Delete;
    default:
        throw std::logic_error("Unable to convert action " + std::to_string(static_cast<int>(action)) + " to action code.");
    }
}

}
