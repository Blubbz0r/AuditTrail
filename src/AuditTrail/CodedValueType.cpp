#include "CodedValueType.h"

namespace AuditTrail
{

CodedValueType::CodedValueType() : m_isValid(false)
{
}

CodedValueType::CodedValueType(std::string name, std::string code)
    : m_isValid(true), m_name(std::move(name)), m_code(std::move(code))
{
}

CodedValueType::CodedValueType(std::string name, std::string code, std::string displayName)
    : m_isValid(true),
      m_name(std::move(name)),
      m_code(std::move(code)),
      m_codeSystemName("DCM"),
      m_displayName(std::move(displayName))
{
}

IO::Node CodedValueType::toNode() const
{
    IO::Node eventId(m_name);
    eventId.addAttribute(IO::Attribute{ "code", m_code });
    if (!m_codeSystemName.empty())
        eventId.addAttribute(IO::Attribute{ "codeSystemName", m_codeSystemName });
    if (!m_displayName.empty())
        eventId.addAttribute(IO::Attribute{ "displayName", m_displayName });
    return eventId;
}

CodedValueType generateEventID(EventIDCode eventID)
{
    switch (eventID)
    {
    case EventIDCode::ApplicationActivity:
        return CodedValueType{"EventID", "110100", "Application Activity"};
    case EventIDCode::AuditLogUsed:
        return CodedValueType{"EventID", "110101", "Audit Log Used"};
    case EventIDCode::BeginTransferringInstances:
        return CodedValueType{"EventID", "110102", "Begin Transferring DICOM Instances"};
    case EventIDCode::Export:
        return CodedValueType{"EventID", "110106", "Export"};
    case EventIDCode::Import:
        return CodedValueType{"EventID", "110107", "Import"};
    case EventIDCode::InstancesAccessed:
        return CodedValueType{"EventID", "110103", "DICOM Instances Accessed"};
    case EventIDCode::InstancesTransferred:
        return CodedValueType{"EventID", "110104", "DICOM Instances Transferred"};
    case EventIDCode::NetworkEntry:
        return CodedValueType{ "EventID", "110108", "Network Entry" };
    case EventIDCode::Query:
        return CodedValueType{ "EventID", "110112", "Query" };
    case EventIDCode::SecurityAlert:
        return CodedValueType{ "EventID", "110113", "Security Alert" };
    case EventIDCode::StudyDeleted:
        return CodedValueType{"EventID", "110105", "DICOM Study Deleted"};
    default:
        throw std::logic_error("Unable to generate event id code from " + std::to_string(static_cast<int>(eventID)));
    }
}

CodedValueType generateEventTypeCode(EventTypeCode eventTypeCode)
{
    switch (eventTypeCode)
    {
    case EventTypeCode::ApplicationStart:
        return CodedValueType{ "EventTypeCode", "110120", "Application Start" };
    case EventTypeCode::ApplicationStop:
        return CodedValueType{ "EventTypeCode", "110121", "Application Stop" };
    case EventTypeCode::Attach:
        return CodedValueType{ "EventTypeCode", "110124", "Attach" };
    case EventTypeCode::Detach:
        return CodedValueType{ "EventTypeCode", "110125", "Detach" };
    default:
        throw std::logic_error("Unable to generate event type code from " + std::to_string(static_cast<int>(eventTypeCode)));
    }
}

CodedValueType generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode idTypeCode)
{
    switch (idTypeCode)
    {
    case ParticipantObjectIDTypeCode::NodeIdentifier:
        return CodedValueType{ "ParticipantObjectIDTypeCode", "110182", "Node ID" };
    case ParticipantObjectIDTypeCode::PatientId:
        return CodedValueType{ "ParticipantObjectIDTypeCode", "2" };
    case ParticipantObjectIDTypeCode::SOPClassUID:
        return CodedValueType{ "ParticipantObjectIDTypeCode", "110181", "SOP Class UID" };
    case ParticipantObjectIDTypeCode::StudyInstanceUid:
        return CodedValueType{ "ParticipantObjectIDTypeCode", "110180", "Study Instance UID" };
    case ParticipantObjectIDTypeCode::URI:
        return CodedValueType{ "ParticipantObjectIDTypeCode", "12" };
    default:
        throw std::logic_error("Unable to generate participant object id type code from " + std::to_string(static_cast<int>(idTypeCode)));
    }
}

CodedValueType generateRoleIDCode(RoleIDCode roleIDCode)
{
    switch (roleIDCode)
    {
    case RoleIDCode::Application:
        return CodedValueType{ "RoleIDCode", "110150", "Application" };
    case RoleIDCode::ApplicationLauncher:
        return CodedValueType{ "RoleIDCode", "110151", "Application Launcher" };
    case RoleIDCode::Destination:
        return CodedValueType{ "RoleIDCode", "110152", "Destination Role ID" };
    case RoleIDCode::DestinationMedia:
        return CodedValueType{ "RoleIDCode", "110154", "Destination Media" };
    case RoleIDCode::Source:
        return CodedValueType{ "RoleIDCode", "110153", "Source Role ID" };
    case RoleIDCode::SourceMedia:
        return CodedValueType{ "RoleIDCode", "110155", "Source Media" };
    default:
        throw std::logic_error("Unable to generate role id code from " + std::to_string(static_cast<int>(roleIDCode)));
    }
}

}