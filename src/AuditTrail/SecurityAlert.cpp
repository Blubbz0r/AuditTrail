#include "SecurityAlert.h"

namespace AuditTrail
{

SecurityAlert::SecurityAlert(Outcome outcome, Type type)
    : event(outcome, EventActionCode::Execute, generateEventID(EventIDCode::SecurityAlert)),
      reportingPerson(nullptr),
      reportingProcess(nullptr)
{
    event.eventTypeCode = generateEventTypeCode(type);
}

std::vector<IO::Node> SecurityAlert::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    // TODO: either a person or a process has to exist
    if (reportingPerson)
        nodes.emplace_back(reportingPerson->toNode());

    if (reportingProcess)
        nodes.emplace_back(reportingProcess->toNode());

    for (const auto& performingParticipant : performingParticipants)
        nodes.emplace_back(EntityActiveParticipant(performingParticipant).toNode());

    for (const auto& alertSubject : alertSubjects)
        nodes.emplace_back(alertSubject.toNode());

    return nodes;
}

void SecurityAlert::addReportingPerson(ActiveParticipant person)
{
    reportingPerson = std::make_unique<EntityActiveParticipant>(std::move(person));
}

void SecurityAlert::addReportingProcess(ActiveParticipant process)
{
    reportingProcess = std::make_unique<EntityActiveParticipant>(std::move(process));
}

void SecurityAlert::addPerformingParticipant(ActiveParticipant performingParticipant)
{
    performingParticipants.emplace_back(EntityActiveParticipant(std::move(performingParticipant)));
}

void SecurityAlert::addAlertSubject(Subject subject, std::string objectId)
{
    ParticipantObjectIDTypeCode participantObjectIdTypeCode;
    switch (subject)
    {
    case Subject::File:
        participantObjectIdTypeCode = ParticipantObjectIDTypeCode::URI;
        break;
    case Subject::Node:
        participantObjectIdTypeCode = ParticipantObjectIDTypeCode::NodeIdentifier;
        break;
    default:
        throw std::logic_error(
            "Unable to create participant object id type code for security alert subject "
            + std::to_string(static_cast<int>(subject)));
    }

    EntityParticipantObject alertSubjectEntity(
        EntityParticipantObject::Type::SystemObject,
        EntityParticipantObject::Role::
            SecurityResource /* TODO: optional; MasterFile is possible, too */,
        generateParticipantObjectIDTypeCode(participantObjectIdTypeCode), objectId);

    // TODO: needs ParticipantObjectDetail, which is not yet supported

    alertSubjects.emplace_back(std::move(alertSubjectEntity));
}

CodedValueType SecurityAlert::generateEventTypeCode(Type type)
{
    switch (type)
    {
    case Type::NodeAuthentication:
        return CodedValueType{ "EventTypeCode", "110126", "Node Authentication" };
    case Type::EmergencyOverrideStarted:
        return CodedValueType{ "EventTypeCode", "110127", "Emergency Override Started" };
    case Type::NetworkConfiguration:
        return CodedValueType{ "EventTypeCode", "110128", "NetworkConfiguration" };
    case Type::SecurityConfiguration:
        return CodedValueType{ "EventTypeCode", "110129", "Security Configuration" };
    case Type::HardwareConfiguration:
        return CodedValueType{ "EventTypeCode", "110130", "Hardware Configuration" };
    case Type::SoftwareConfiguration:
        return CodedValueType{ "EventTypeCode", "110131", "Software Configuration" };
    case Type::UseOfRestrictedFunction:
        return CodedValueType{ "EventTypeCode", "110132", "Use of Restricted Function" };
    case Type::AuditRecordingStopped:
        return CodedValueType{ "EventTypeCode", "110133", "Audit Recording Stopped" };
    case Type::AuditRecordingStarted:
        return CodedValueType{ "EventTypeCode", "110134", "Audit Recording Started" };
    case Type::ObejctSecurityAttributesChanged:
        return CodedValueType{ "EventTypeCode", "110135", "Object Security Attributes Changed" };
    case Type::SecurityRolesChanged:
        return CodedValueType{ "EventTypeCode", "110136", "Security Roles Changed" };
    case Type::UserSecurityAttributesChanged:
        return CodedValueType{ "EventTypeCode", "110137", "User Security Attributes Changed" };
    case Type::EmergencyOverrideStopped:
        return CodedValueType{ "EventTypeCode", "110138", "EmergencyOverrideStopped" };
    case Type::RemoteServiceOperationStarted:
        return CodedValueType{ "EventTypeCode", "110139", "Remote Service Operation Started" };
    case Type::RemoteServiceOperationStopped:
        return CodedValueType{ "EventTypeCode", "110140", "Remote Service Operation Stopped" };
    case Type::LocalServiceOperationStarted:
        return CodedValueType{ "EventTypeCode", "110141", "Local Service Operation Started" };
    case Type::LocalServiceOperationStopped:
        return CodedValueType{ "EventTypeCode", "110142", "Local Service Operation Stopped" };
    default:
        throw std::logic_error("Unable to convert security alert type "
                               + std::to_string(static_cast<int>(type)) + " to event type code");
    }
}
}
