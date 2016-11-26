#pragma once

#include "Message.h"
#include "ActiveParticipant.h"
#include "CodedValueType.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"
#include "Event.h"

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
struct SecurityAlert : public Message
{
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

    EntityEvent event;

    void addReportingPerson(ActiveParticipant person);
    std::unique_ptr<EntityActiveParticipant> reportingPerson;

    void addReportingProcess(ActiveParticipant process);
    std::unique_ptr<EntityActiveParticipant> reportingProcess;

    /*! Performing persons or processes */
    void addPerformingParticipant(ActiveParticipant performingParticipant);
    std::vector<EntityActiveParticipant> performingParticipants;

    /*!
        \param  objectId - If \a subject is File: URI of the file or resource.
                           If \a subject is Node: identity of the node that is subject either in the
                           form ofnode_name@domain_name or as an IP address.
    */
    void addAlertSubject(Subject subject, std::string objectId);
    std::vector<EntityParticipantObject> alertSubjects;

private:
    static CodedValueType generateEventTypeCode(Type type);
};

}
