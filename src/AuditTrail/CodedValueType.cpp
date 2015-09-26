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

CodedValueType generateCode(CodeType type)
{
    switch (type)
    {
    case CodeType::Application:
        return CodedValueType{"RoleIDCode", "110150", "Application"};
    case CodeType::ApplicationActivity:
        return CodedValueType{"EventID", "110100", "Application Activity"};
    case CodeType::ApplicationLauncher:
        return CodedValueType{"RoleIDCode", "110151", "Application Launcher"};
    case CodeType::ApplicationStart:
        return CodedValueType{"EventTypeCode", "110120", "Application Start"};
    case CodeType::ApplicationStop:
        return CodedValueType{"EventTypeCode", "110121", "Application Stop"};
    case CodeType::AuditLogUsed:
        return CodedValueType{"EventID", "110101", "Audit Log Used"};
    case CodeType::BeginTransferringInstances:
        return CodedValueType{"EventID", "110102", "Begin Transferring DICOM Instances"};
    case CodeType::Destination:
        return CodedValueType{ "RoleIDCode", "110152", "Destination Role ID" };
    case CodeType::DestinationMedia:
        return CodedValueType{"RoleIDCode", "110154", "Destination Media"};
    case CodeType::Export:
        return CodedValueType{"EventID", "110106", "Export"};
    case CodeType::Import:
        return CodedValueType{"EventID", "110107", "Import"};
    case CodeType::PatientId:
        return CodedValueType{"ParticipantObjectIDTypeCode", "2"};
    case CodeType::Source:
        return CodedValueType{ "RoleIDCode", "110153", "Source Role ID" };
    case CodeType::SourceMedia:
        return CodedValueType{"RoleIDCode", "110155", "Source Media"};
    case CodeType::StudyInstanceUid:
        return CodedValueType{ "ParticipantObjectIDTypeCode", "110180", "Study Instance UID" };
    case CodeType::URI:
        return CodedValueType{"ParticipantObjectIDTypeCode", "12"};
    default:
        throw std::logic_error("Unable to convert event type " + std::to_string(static_cast<int>(type)) + " to event");
    }
}

}