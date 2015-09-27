#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"
#include "SOPClass.h"

#include <memory>

namespace AuditTrail
{

struct EntityParticipantObject;
enum class EventActionCode;

/*!
    \brief  Describes the event of DICOM SOP instances being viewed, utilized, updated or deleted.

    \todo   "If all instances within a study are deleted, then the EV(110105, DCM, "DICOM Study Deleted") event shall be used"
*/
class InstancesAccessed : public Message
{
public:
    enum class Action
    {
        Create,
        Read,
        Update,
        Delete
    };

    InstancesAccessed(Outcome outcome, Action action, std::string patientId);
    ~InstancesAccessed();

    std::vector<IO::Node> createNodes() const override;

    void setManipulatingPerson(ActiveParticipant manipulatingPerson);
    void setManipulatingProcess(ActiveParticipant manipulatingProcess);

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);

    void setPatientName(std::string patientName) { m_patientName = std::move(patientName); }

private:
    EventActionCode actionToActionCode() const;

    Outcome m_outcome;
    Action m_action;
    std::unique_ptr<ActiveParticipant> m_manipulatingPerson;
    std::unique_ptr<ActiveParticipant> m_manipulatingProcess;
    std::vector<EntityParticipantObject> m_studies;
    std::string m_patientId;
    std::string m_patientName;
};

}