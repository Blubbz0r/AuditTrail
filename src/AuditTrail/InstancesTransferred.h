#pragma once

#include "Message.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"
#include "Event.h"
#include "SOPClass.h"

namespace AuditTrail
{

/*!
    Describes the event of the completion of transferring DICOM SOP instances between two
    application entities.

    \todo   EventDateTime "Shall be the time when the transfer has completed"
*/
struct InstancesTransferred : public Message
{
    enum class Action
    {
        Create, /*! If the receiver did not hold copies of the instances transferred. If the audit source is either not the receiver, or otherwise does not know whether or not the instances previously were held by the receiving node. */
        Read,   /*! If the receiver already holds copies of the SOP instances transferred, and has determined that no changes are needed to the copies hold. */
        Update  /*! If the receiver is altering its held copies and the received copies. */
    };

    InstancesTransferred(Outcome outcome, Action action, ActiveParticipant sendingProcess,
                         ActiveParticipant receivingProcess, std::string patientId);
    ~InstancesTransferred();

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityActiveParticipant sendingProcess;
    EntityActiveParticipant receivingProcess;
    EntityParticipantObject patient;

    void addOtherParticipant(ActiveParticipant otherParticipant);
    std::vector<EntityActiveParticipant> otherParticipants;

    void addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses);
    std::vector<EntityParticipantObject> studies;

    void setPatientName(std::string patientName);

private:
    static EventActionCode toActionCode(Action action);
};

}
