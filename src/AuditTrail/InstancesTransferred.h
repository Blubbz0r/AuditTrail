#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"
#include "SOPClass.h"

namespace AuditTrail
{

struct EntityParticipantObject;
enum class EventActionCode;

/*!
    \brief  Describes the event of the completion of transferring DICOM SOP instances between two
            application entities.

    \todo   EventDateTime "Shall be the time when the transfer has completed"
*/
class InstancesTransferred : public Message
{
public:
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

    void addOtherParticipant(ActiveParticipant otherParticipant);

    void addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses);

    void setPatientName(std::string patientName) { m_patientName = std::move(patientName); }

private:
    EventActionCode actionToActionCode() const;

    Outcome m_outcome;
    Action m_action;
    ActiveParticipant m_sendingProcess;
    ActiveParticipant m_receivingProcess;
    std::vector<ActiveParticipant> m_otherParticipants;
    std::vector<EntityParticipantObject> m_studies;
    std::string m_patientId;
    std::string m_patientName;
};

}