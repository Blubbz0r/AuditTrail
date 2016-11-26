#include "InstancesTransferred.h"

namespace AuditTrail
{

InstancesTransferred::InstancesTransferred(Outcome outcome, Action action,
                                           ActiveParticipant sendingProcess,
                                           ActiveParticipant receivingProcess,
                                           std::string patientId)
    : event(outcome, toActionCode(action), generateEventID(EventIDCode::InstancesTransferred)),
      sendingProcess(std::move(sendingProcess)),
      receivingProcess(std::move(receivingProcess)),
      patient(EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
              generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId),
              std::move(patientId))
{
    this->sendingProcess.participant.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    this->receivingProcess.participant.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
}

InstancesTransferred::~InstancesTransferred()
{
}

std::vector<IO::Node> InstancesTransferred::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(sendingProcess.toNode());
    nodes.emplace_back(receivingProcess.toNode());

    for (const auto& otherParticipant : otherParticipants)
        nodes.emplace_back(EntityActiveParticipant(otherParticipant).toNode());

    for (const auto& study : studies)
        nodes.emplace_back(study.toNode());

    nodes.emplace_back(patient.toNode());

    return nodes;
}

void InstancesTransferred::addOtherParticipant(ActiveParticipant otherParticipant)
{
    otherParticipants.emplace_back(EntityActiveParticipant(std::move(otherParticipant)));
}

void InstancesTransferred::addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUID));

    study.setSOPClasses(std::move(sopClasses));
    studies.emplace_back(std::move(study));
}

void InstancesTransferred::setPatientName(std::string patientName)
{
    patient.objectNameOrQuery = std::move(patientName);
}

EventActionCode InstancesTransferred::toActionCode(Action action)
{
    switch (action)
    {
    case Action::Create:
        return EventActionCode::Create;
    case Action::Read:
        return EventActionCode::Read;
    case Action::Update:
        return EventActionCode::Update;
    default:
        throw std::logic_error("Unable to convert action " + std::to_string(static_cast<int>(action)) + " to string.");
    }
}

}
