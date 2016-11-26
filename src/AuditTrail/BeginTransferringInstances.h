#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityActiveParticipant.h"
#include "EntityParticipantObject.h"
#include "Event.h"
#include "SOPClass.h"

namespace AuditTrail
{

/*!
    Describes the event of a system beginning to transfer a set of DICOM instances from one node to
    another within control of the system's security domain.

    \note   This message may only include information about a single patient.
*/
struct BeginTransferringInstances : public Message
{
    BeginTransferringInstances(Outcome outcome, ActiveParticipant sender,
                               ActiveParticipant receiver, std::string patientId);
    ~BeginTransferringInstances();

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityActiveParticipant sendingProcess;
    EntityActiveParticipant receivingProcess;
    EntityParticipantObject patient;

    void addOtherParticipant(ActiveParticipant otherParticipant);
    std::vector<EntityActiveParticipant> otherParticipants;

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);
    std::vector<EntityParticipantObject> studies;

    void setPatientName(std::string patientName);

private:
    static ActiveParticipant senderWithRoleIdCode(ActiveParticipant sender);
    static ActiveParticipant receiverWithRoleIdCode(ActiveParticipant receiver);
};

}
