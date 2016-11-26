#include "BeginTransferringInstances.h"

namespace AuditTrail
{

BeginTransferringInstances::BeginTransferringInstances(Outcome outcome, ActiveParticipant sender,
                                                       ActiveParticipant receiver,
                                                       std::string patientId)
    : event(outcome, EventActionCode::Execute,
            generateEventID(EventIDCode::BeginTransferringInstances)),
      sendingProcess(senderWithRoleIdCode(std::move(sender))),
      receivingProcess(receiverWithRoleIdCode(std::move(receiver))),
      patient(EntityParticipantObject::Type::Person, EntityParticipantObject::Role::Patient,
              generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::PatientId),
              std::move(patientId))
{
}

BeginTransferringInstances::~BeginTransferringInstances()
{
}

std::vector<IO::Node> BeginTransferringInstances::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(sendingProcess.toNode());
    nodes.emplace_back(receivingProcess.toNode());

    for (const auto& otherParticipant : otherParticipants)
        nodes.emplace_back(otherParticipant.toNode());

    for (const auto& study : studies)
        nodes.emplace_back(study.toNode());

    nodes.emplace_back(patient.toNode());

    return nodes;
}

void BeginTransferringInstances::addOtherParticipant(ActiveParticipant otherParticipant)
{
    otherParticipants.emplace_back(EntityActiveParticipant(std::move(otherParticipant)));
}

void BeginTransferringInstances::addStudy(std::string studyInstanceUid,
                                          std::vector<SOPClass> sopClasses)
{
    EntityParticipantObject study(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::StudyInstanceUid),
        std::move(studyInstanceUid));

    study.setSOPClasses(std::move(sopClasses));

    studies.emplace_back(std::move(study));
}

void BeginTransferringInstances::setPatientName(std::string patientName)
{
    patient.objectNameOrQuery = std::move(patientName);
}

    ActiveParticipant BeginTransferringInstances::senderWithRoleIdCode(ActiveParticipant sender)
{
    sender.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    return sender;
}

ActiveParticipant BeginTransferringInstances::receiverWithRoleIdCode(ActiveParticipant receiver)
{
    receiver.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    return receiver;
}

}
