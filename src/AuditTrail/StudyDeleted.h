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

struct EntityParticipantObject;

/*!
    Describes the event of deletion of one or more studies and all associated SOP instances in a
    single action.
*/
class StudyDeleted : public Message
{
public:
    StudyDeleted(Outcome outcome, std::string patientID);
    ~StudyDeleted();

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityParticipantObject patient;

    void setDeletingPerson(ActiveParticipant person);
    std::unique_ptr<EntityActiveParticipant> deletingPerson;

    void setDeletingProcess(ActiveParticipant process);
    std::unique_ptr<EntityActiveParticipant> deletingProcess;

    void addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses);
    std::vector<EntityParticipantObject> studies;

    void setPatientName(std::string patientName);
};

}
