#pragma once

#include "Message.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"
#include "Event.h"
#include "SOPClass.h"

#include <memory>

namespace AuditTrail
{

/*!
    Describes the event of DICOM SOP instances being viewed, utilized, updated or deleted.

    \todo   "If all instances within a study are deleted, then the EV(110105, DCM, "DICOM Study Deleted") event shall be used"
*/
struct InstancesAccessed : public Message
{
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

    EntityEvent event;
    EntityParticipantObject patient;

    void setManipulatingPerson(ActiveParticipant person);
    std::unique_ptr<EntityActiveParticipant> manipulatingPerson;

    void setManipulatingProcess(ActiveParticipant process);
    std::unique_ptr<EntityActiveParticipant> manipulatingProcess;

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);
    std::vector<EntityParticipantObject> studies;

    void setPatientName(std::string patientName);

private:
    static EventActionCode actionToActionCode(Action action);
};

}
