#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

/*!
    \brief  Describes the event of a system beginning to transfer a set of DICOM instances from one
            node to another within control of the system's security domain.

    \note   This message may only include information about a single patient.
*/
class BeginTransferringInstances : public Message
{
public:
    BeginTransferringInstances(Outcome outcome, ActiveParticipant sendingProcess,
                               ActiveParticipant receivingProcess, std::string patientId);

    std::vector<IO::Node> createNodes() const override;

    void addOtherParticipant(ActiveParticipant otherParticipant);
    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);

    void setPatientName(std::string patientName) { m_patientName = std::move(patientName); }

private:
    Outcome m_outcome;
    ActiveParticipant m_sendingProcess;
    ActiveParticipant m_receivingProcess;
    std::vector<ActiveParticipant> m_otherParticipants;
    std::vector<EntityParticipantObject> m_studies;
    std::string m_patientId;
    std::string m_patientName;
};

}