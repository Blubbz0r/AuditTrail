#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"
#include "SOPClass.h"

#include <memory>

namespace AuditTrail
{

struct EntityParticipantObject;

/*!
    \brief  Describes the event of deletion of one or more studies and all associated SOP instances
            in a single action.
*/
class StudyDeleted : public Message
{
public:
    StudyDeleted(Outcome outcome, std::string patientID);
    ~StudyDeleted();

    std::vector<IO::Node> createNodes() const override;

    void setDeletingPerson(ActiveParticipant deletingPerson);
    void setDeletingProcess(ActiveParticipant deletingProcess);

    void addStudy(std::string studyInstanceUID, std::vector<SOPClass> sopClasses);

    void setPatientName(std::string patientName) { m_patientName = std::move(patientName); }

private:
    Outcome m_outcome;
    std::unique_ptr<ActiveParticipant> m_deletingPerson;
    std::unique_ptr<ActiveParticipant> m_deletingProcess;
    std::vector<EntityParticipantObject> m_studies;
    std::string m_patientID;
    std::string m_patientName;
};

}