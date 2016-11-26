#include "SecurityAlert.h"
#include "EntityEvent.h"
#include "EntityActiveParticipant.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

SecurityAlert::SecurityAlert(Outcome outcome, Type type)
    : m_outcome(outcome), m_type(type), m_reportingPerson(nullptr), m_reportingProcess(nullptr)
{
}

std::vector<IO::Node> SecurityAlert::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute,
                      generateEventID(EventIDCode::SecurityAlert));
    event.eventTypeCode = generateEventTypeCode();
    nodes.emplace_back(event.toNode());

    // TODO: either a person or a process has to exist
    if (m_reportingPerson)
        nodes.emplace_back(EntityActiveParticipant(*m_reportingPerson).toNode());

    if (m_reportingProcess)
        nodes.emplace_back(EntityActiveParticipant(*m_reportingProcess).toNode());

    for (const auto& performingParticipant : m_performingParticipants)
        nodes.emplace_back(EntityActiveParticipant(performingParticipant).toNode());

    for (const auto& alertSubject : m_alertSubjects)
    {
        const auto& subject = alertSubject.first;
        const auto& objectId = alertSubject.second;

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

        nodes.emplace_back(alertSubjectEntity.toNode());
    }

    return nodes;
}

void SecurityAlert::addReportingPerson(ActiveParticipant reportingPerson)
{
    m_reportingPerson = std::make_unique<ActiveParticipant>(reportingPerson);
}

void SecurityAlert::addReportingProcess(ActiveParticipant reporintProcess)
{
    m_reportingPerson = std::make_unique<ActiveParticipant>(reporintProcess);
}

void SecurityAlert::addPerformingParticipant(ActiveParticipant performingParticipant)
{
    m_performingParticipants.emplace_back(std::move(performingParticipant));
}

void SecurityAlert::addAlertSubject(Subject subject, std::string objectId)
{
    m_alertSubjects.emplace_back(subject, std::move(objectId));
}

    CodedValueType SecurityAlert::generateEventTypeCode() const
{
    switch (m_type)
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
                               + std::to_string(static_cast<int>(m_type)) + " to event type code");
    }
}
}
