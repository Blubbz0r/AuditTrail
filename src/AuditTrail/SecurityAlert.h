#pragma once

#include "Message.h"
#include "Event.h"
#include "CodedValueType.h"
#include "ActiveParticipant.h"
#include <memory>

namespace AuditTrail
{

/*!
    Describes any event for which a node needs to report a security alert, e.g., a node
    authentication failure when establishing a secure communications channel.

    \note   The Node Authentication event can be used to report both successes and failures. If
            reporting of success is done, this could generate a very large number of audit messages,
            since every authenticated DICOM association, HL7 transaction, and HTML connection should
            result in a successful node authentication. It is expected that in most situations only
            the failures will be reported.
*/
class SecurityAlert : public Message
{
public:
    /*! Types of security alerts defined by DICOM Standard part 16 (CID 403) */
    enum class Type
    {
        NodeAuthentication,
        EmergencyOverrideStarted,
        NetworkConfiguration,
        SecurityConfiguration,
        HardwareConfiguration,
        SoftwareConfiguration,
        UseOfRestrictedFunction,
        AuditRecordingStopped,
        AuditRecordingStarted,
        ObejctSecurityAttributesChanged,
        SecurityRolesChanged,
        UserSecurityAttributesChanged,
        EmergencyOverrideStopped,
        RemoteServiceOperationStarted,
        RemoteServiceOperationStopped,
        LocalServiceOperationStarted,
        LocalServiceOperationStopped
    };

    enum class Subject
    {
        File,   /*!< A file or a resource (something that has a URI) is subject of the security alert */
        Node    /*!< A node (e.g. a person or a process) is subject of the security alert */
    };

    SecurityAlert(Outcome outcome, Type type);

    std::vector<IO::Node> createNodes() const override;

    void addReportingPerson(ActiveParticipant reportingPerson);
    void addReportingProcess(ActiveParticipant reporintProcess);

    /*! Performing persons or processes */
    void addPerformingParticipant(ActiveParticipant performingParticipant);

    /*!
        \param  objectId - If \a subject is File: URI of the file or resource.
                           If \a subject is Node: identity of the node that is subject either in the
                           form ofnode_name@domain_name or as an IP address.
    */
    void addAlertSubject(Subject subject, std::string objectId);

private:
    CodedValueType generateEventTypeCode() const;

    Outcome m_outcome;
    Type m_type;
    std::unique_ptr<ActiveParticipant> m_reportingPerson;
    std::unique_ptr<ActiveParticipant> m_reportingProcess;
    std::vector<ActiveParticipant> m_performingParticipants;
    std::vector<std::pair<Subject, std::string>> m_alertSubjects;
};

}
