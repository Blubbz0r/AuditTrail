#pragma once

#include "Entity.h"

namespace AuditTrail
{

/*!
    \brief  Corresponds to DICOM "Code Meaning"
*/
class CodedValueType : public Entity
{
public:
    CodedValueType();
    CodedValueType(std::string name, std::string code);
    CodedValueType(std::string name, std::string code, std::string displayName);

    bool isValid() const { return m_isValid; }

    IO::Node toNode() const override;

private:
    bool m_isValid;

    std::string m_name;
    std::string m_code;
    std::string m_codeSystemName;
    std::string m_displayName;
};

enum class CodeType
{
    Application,
    ApplicationActivity,
    ApplicationLauncher,
    ApplicationStart,
    ApplicationStop,
    AuditLogUsed,
    BeginTransferringInstances,
    Destination,
    DestinationMedia,
    Export,
    Import,
    PatientId,
    Source,
    SourceMedia,
    StudyInstanceUid,
    URI
};

CodedValueType generateCode(CodeType type);

}